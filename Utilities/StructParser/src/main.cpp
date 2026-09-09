#include "Platform.h"
#include "GenerationIO.h"
#include "SourceParser.h"
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>

namespace fs = std::filesystem;

// Process-wide tracing is enabled exclusively by --verbose.
int gVerbose = 0;

static const char *usage =
	"Usage: StructParser --target NAME --source-dir DIR --common-dir DIR\n"
	"  --project-dir DIR --kind executable|library --sources-file FILE\n"
	"  --state-dir DIR [--force] [--verbose] [--help]\n";

struct Stamp {
	fs::path path;
	bool exists;
	std::uintmax_t size;
	fs::file_time_type::duration::rep time;
};

static Stamp stamp(const fs::path& path)
{
	if (!fs::exists(path))
		return {path, false, 0, 0};
	return {path, true, fs::file_size(path),
		fs::last_write_time(path).time_since_epoch().count()};
}

static void writeStamp(std::ostream& out, const Stamp& value)
{
	out << std::quoted(value.path.generic_string()) << ' ' << value.exists
		<< ' ' << value.size << ' ' << value.time << '\n';
}

static bool unchanged(const Stamp& old)
{
	auto now = stamp(old.path);
	return old.exists == now.exists && old.size == now.size &&
		old.time == now.time;
}

struct State {
	std::string invocation;
	std::vector<Stamp> inputs;
	std::vector<Stamp> configuration;
	std::vector<Stamp> outputs;
};

static std::string serialize(const State& state)
{
	std::ostringstream out;
	out << "StructParser-state-1\n"
		<< std::quoted(state.invocation) << '\n';
	for (auto list : {&state.inputs, &state.configuration,
		&state.outputs}) {
		out << list->size() << '\n';
		for (auto const& item : *list)
			writeStamp(out, item);
	}
	out << "end\n";
	return out.str();
}

static bool loadState(const fs::path& path, State& state)
{
	std::ifstream in(path, std::ios::binary);
	std::string version;
	if (!std::getline(in, version) || version != "StructParser-state-1")
		return false;
	if (!(in >> std::quoted(state.invocation)))
		return false;
	for (auto list : {&state.inputs, &state.configuration,
		&state.outputs}) {
		std::size_t count;
		if (!(in >> count) || count > 100000)
			return false;
		for (std::size_t i = 0; i < count; ++i) {
			Stamp value{};
			std::string name;
			if (!(in >> std::quoted(name) >> value.exists >>
			      value.size >> value.time) ||
			    !fs::path(name).is_absolute())
				return false;
			value.path = name;
			list->push_back(value);
		}
	}
	std::string end;
	return (in >> end) && end == "end" && (in >> std::ws).eof();
}

static bool fresh(const State& old, const State& current)
{
	if (old.invocation != current.invocation ||
	    old.inputs.size() != current.inputs.size())
		return false;
	for (std::size_t i = 0; i < old.inputs.size(); ++i)
		if (old.inputs[i].path != current.inputs[i].path)
			return false;
	for (auto list : {&old.inputs, &old.configuration, &old.outputs})
		for (auto const& item : *list)
			if (!unchanged(item))
				return false;
	return !old.outputs.empty() && !old.configuration.empty();
}

static std::vector<fs::path> readSources(const fs::path& file)
{
	std::ifstream in(file);
	if (!in)
		throw std::runtime_error("Cannot read " + file.string());
	std::vector<fs::path> sources;
	std::set<fs::path> seen;
	std::string line;
	while (std::getline(in, line)) {
		if (!line.empty() && line.back() == '\r')
			line.pop_back();
		if (line.empty())
			continue;
		auto path = fs::absolute(line).lexically_normal();
		auto extension = path.extension().string();
		if (CompareNoCase(extension.c_str(), ".c") &&
		    CompareNoCase(extension.c_str(), ".h"))
			continue;
		bool generated = false;
		for (auto const& part : path) {
			auto name = part.string();
			MakeStringUpcase(name.data());
			generated |= name.find("AUTOGEN") != std::string::npos;
		}
		if (!generated && seen.insert(path).second)
			sources.push_back(path);
	}
	if (in.bad())
		throw std::runtime_error("Error reading " + file.string());
	return sources;
}

static void initialOwnership(State& state, const fs::path& source,
	const fs::path& common, const std::string& target)
{
	// Match the original target's nonrecursive cleanup boundaries.
	for (auto dir : {source / "AutoGen", source / "wiki",
		common / "AutoGen"}) {
		if (!fs::exists(dir))
			continue;
		for (auto const& entry : fs::directory_iterator(dir)) {
			if (!entry.is_regular_file())
				continue;
			auto name = entry.path().filename().string();
			if (dir == common / "AutoGen" &&
			    name.rfind(target + "_", 0) != 0)
				continue;
			state.outputs.push_back(stamp(entry.path()));
		}
	}
}

static int run(int argc, char **argv)
{
	std::map<std::string, std::string> options;
	bool force = false;
	const std::set<std::string> names = {"--target", "--source-dir",
		"--common-dir", "--project-dir", "--kind", "--sources-file",
		"--state-dir"};
	for (int i = 1; i < argc; ++i) {
		std::string name = argv[i];
		if (name == "--help") {
			std::cout << usage;
			return 0;
		}
		if (name == "--force") {
			force = true;
			continue;
		}
		if (name == "--verbose") {
			gVerbose = 1;
			continue;
		}
		if (!names.count(name) || i + 1 == argc ||
		    !*argv[i + 1] || options.count(name))
			throw std::invalid_argument("Invalid option: " + name);
		options[name] = argv[++i];
	}
	for (auto const& name : names)
		if (!options.count(name))
			throw std::invalid_argument("Missing " + name);
	const auto target = options.at("--target");
	if (target.size() > 100 || !IsOKForIdentStart(target.front()) ||
	    !std::all_of(target.begin(), target.end(), IsOKForIdent))
		throw std::invalid_argument("Target must be a C identifier");
	const auto kind = options.at("--kind");
	if (kind != "executable" && kind != "library")
		throw std::invalid_argument("Invalid --kind");
	for (auto const& name : names) {
		if (name == "--target" || name == "--kind")
			continue;
		options[name] = fs::absolute(options[name]).lexically_normal()
			.generic_string();
	}
	for (auto name : {"--source-dir", "--common-dir", "--project-dir"})
		if (!fs::is_directory(options[name]))
			throw std::invalid_argument(
				std::string("Invalid ") + name);
	fs::path stateDir = options.at("--state-dir");
	fs::create_directories(stateDir);
	const auto stateFile = stateDir / "state";
	const auto ownershipFile = stateDir / "ownership";
	State current;
	std::ostringstream invocation;
	for (auto const& option : options)
		invocation << option.first << ' '
			<< std::quoted(option.second) << '\n';
	current.invocation = invocation.str();
	auto sources = readSources(options.at("--sources-file"));
	current.inputs.push_back(stamp(ExecutablePath()));
	for (auto const& source : sources)
		current.inputs.push_back(stamp(source));
	State previous;
	bool valid = loadState(stateFile, previous);
	if (!force && valid && fresh(previous, current)) {
		TRACE("StructParser: unchanged %s\n", target.c_str());
		return 0;
	}
	State owned;
	if (!loadState(ownershipFile, owned)) {
		owned = valid ? previous : State{};
		if (!valid)
			initialOwnership(owned, options.at("--source-dir"),
				options.at("--common-dir"), target);
	}
	fs::remove(stateFile);
	WriteState(ownershipFile, serialize(owned));
	TRACE("StructParser: regenerating %s\n", target.c_str());
	auto parser = std::make_unique<SourceParser>();
	parser->LoadConfiguration(options.at("--project-dir"));
	for (auto const& path : parser->ConfigurationDependencies())
		current.configuration.push_back(stamp(path));
	parser->ParseSource(target, options.at("--source-dir"),
		options.at("--common-dir"), kind == "executable", sources);
	for (auto list : {&current.inputs, &current.configuration})
		for (auto const& item : *list)
			if (!unchanged(item))
				throw std::runtime_error(
					"Input changed during generation");
	auto generated = GeneratedPaths();
	std::set<fs::path> keep(generated.begin(), generated.end());
	// Keep ownership across failed commits or cleanup for the next retry.
	for (auto const& path : generated)
		owned.outputs.push_back(stamp(path));
	WriteState(ownershipFile, serialize(owned));
	CommitOutputs();
	for (auto const& output : owned.outputs)
		if (!keep.count(output.path))
			fs::remove(output.path);
	for (auto const& path : generated)
		current.outputs.push_back(stamp(path));
	WriteState(ownershipFile, serialize(current));
	WriteState(stateFile, serialize(current));
	return 0;
}

int main(int argc, char **argv)
{
	std::atexit(DiscardOutputs);
	try {
		return run(argc, argv);
	} catch (const std::exception& error) {
		std::cerr << "StructParser: " << error.what() << '\n';
	}
	return 1;
}
