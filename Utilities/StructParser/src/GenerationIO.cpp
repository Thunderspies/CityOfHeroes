#include "GenerationIO.h"
#include <array>
#include <chrono>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

namespace fs = std::filesystem;

struct Output {
	fs::path destination;
	fs::path temporary;
	FILE *stream;
};

static std::vector<Output> outputs;

static fs::path temporaryPath(const fs::path& destination)
{
	static unsigned long serial = 0;
	auto time = std::chrono::steady_clock::now().time_since_epoch().count();
	return destination.string() + ".structparser-tmp-" +
		std::to_string(time) + "-" + std::to_string(++serial);
}

static void replace(const fs::path& from, const fs::path& to)
{
#ifdef _WIN32
	if (!MoveFileExW(from.c_str(), to.c_str(), MOVEFILE_REPLACE_EXISTING))
		throw std::runtime_error("Cannot replace " + to.string());
#else
	fs::rename(from, to);
#endif
}

static bool equalContents(const fs::path& left, const fs::path& right)
{
	if (!fs::exists(right) || fs::file_size(left) != fs::file_size(right))
		return false;
	std::ifstream a(left, std::ios::binary), b(right, std::ios::binary);
	if (!a || !b)
		throw std::runtime_error("Cannot compare " + right.string());
	std::array<char, 8192> x, y;
	do {
		a.read(x.data(), x.size());
		b.read(y.data(), y.size());
		if (a.bad() || b.bad())
			throw std::runtime_error(
				"Error comparing " + right.string());
		if (a.gcount() != b.gcount() ||
		    !std::equal(x.begin(), x.begin() + a.gcount(), y.begin()))
			return false;
	} while (a);
	return true;
}

FILE *OpenOutput(const fs::path& destination)
{
	auto path = fs::absolute(destination).lexically_normal();
	for (auto const& output : outputs)
		if (output.destination == path)
			throw std::runtime_error(
				"Duplicate output: " + path.string());
	fs::create_directories(path.parent_path());
	auto temporary = temporaryPath(path);
	auto file = std::fopen(temporary.string().c_str(), "wb");
	if (!file)
		throw std::runtime_error("Cannot create " + temporary.string());
	outputs.push_back({path, temporary, file});
	return file;
}

FILE *OpenInput(const char *path, const char *mode)
{
	auto absolute = fs::absolute(path).lexically_normal();
	for (auto const& output : outputs) {
		if (output.destination == absolute) {
			if (output.stream)
				throw std::runtime_error(
					"Reading unclosed output");
			auto name = output.temporary.string();
			return std::fopen(name.c_str(), mode);
		}
	}
	return std::fopen(path, mode);
}

void CloseFile(FILE *file)
{
	bool error = std::ferror(file);
	error = std::fclose(file) != 0 || error;
	for (auto& output : outputs)
		if (output.stream == file)
			output.stream = nullptr;
	if (error)
		throw std::runtime_error("Error closing parser file");
}

std::vector<fs::path> GeneratedPaths()
{
	std::vector<fs::path> paths;
	for (auto const& output : outputs)
		paths.push_back(output.destination);
	return paths;
}

void CommitOutputs()
{
	for (auto const& output : outputs) {
		if (output.stream)
			throw std::runtime_error("Unclosed generated output");
		if (equalContents(output.temporary, output.destination))
			fs::remove(output.temporary);
		else
			replace(output.temporary, output.destination);
	}
	outputs.clear();
}

void DiscardOutputs() noexcept
{
	for (auto const& output : outputs) {
		if (output.stream)
			std::fclose(output.stream);
		std::error_code error;
		fs::remove(output.temporary, error);
	}
	outputs.clear();
}

void WriteState(const fs::path& path, const std::string& bytes)
{
	auto temporary = temporaryPath(path);
	try {
		std::ofstream file(temporary, std::ios::binary);
		file.exceptions(std::ios::failbit | std::ios::badbit);
		file.write(bytes.data(), bytes.size());
		file.close();
		replace(temporary, path);
	} catch (...) {
		std::error_code error;
		fs::remove(temporary, error);
		throw;
	}
}
