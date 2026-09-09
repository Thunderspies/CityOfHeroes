#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace fs = std::filesystem;
using Files = std::map<std::string, std::string>;

static void require(bool condition, const std::string& message)
{
    if (!condition)
        throw std::runtime_error(message);
}

static void checkWin32(BOOL result, const char* operation)
{
    if (!result)
        throw std::runtime_error(std::string(operation) + " failed: Win32 error " +
                                 std::to_string(GetLastError()));
}

class Handle
{
public:
    explicit Handle(HANDLE value) : value_(value)
    {
        checkWin32(value && value != INVALID_HANDLE_VALUE, "Opening handle");
    }
    ~Handle() { CloseHandle(value_); }
    Handle(const Handle&) = delete;
    Handle& operator=(const Handle&) = delete;
    operator HANDLE() const { return value_; }
private:
    HANDLE value_;
};

static fs::path pigPath()
{
    wchar_t buffer[32768];
    const DWORD length = GetModuleFileNameW(nullptr, buffer, 32768);
    checkWin32(length != 0 && length < 32768, "GetModuleFileNameW");
    const auto path = fs::path(buffer).parent_path() / L"pig.exe";
    require(fs::is_regular_file(path), "pig.exe must be beside PigTests.exe");
    return path;
}

static fs::path temporaryDirectory(const std::string& name)
{
    wchar_t buffer[MAX_PATH + 1];
    const DWORD length = GetTempPathW(MAX_PATH + 1, buffer);
    checkWin32(length != 0 && length <= MAX_PATH, "GetTempPathW");
    const std::string prefix = name + "-" + std::to_string(GetCurrentProcessId()) +
                               "-" + std::to_string(GetTickCount64()) + "-";
    for (unsigned i = 0; i < 100; ++i) {
        const auto path = fs::path(buffer) / (prefix + std::to_string(i));
        if (CreateDirectoryW(path.c_str(), nullptr))
            return path;
        if (GetLastError() != ERROR_ALREADY_EXISTS)
            checkWin32(FALSE, "CreateDirectoryW");
    }
    throw std::runtime_error("Could not reserve a unique temporary directory");
}

static std::string readFile(const fs::path& path)
{
    std::ifstream stream(path, std::ios::binary);
    require(stream.is_open(), "Cannot read " + path.string());
    std::string data{std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>()};
    require(!stream.bad(), "Read failed: " + path.string());
    return data;
}

static void writeFile(const fs::path& path, const std::string& data)
{
    fs::create_directories(path.parent_path());
    std::ofstream stream(path, std::ios::binary);
    stream.write(data.data(), static_cast<std::streamsize>(data.size()));
    stream.close();
    require(bool(stream), "Cannot write " + path.string());
}

// Quote for the Windows CRT argv parser, including trailing backslashes.
static std::wstring quote(const std::wstring& value)
{
    std::wstring result = L"\"";
    size_t slashes = 0;
    for (wchar_t character : value) {
        if (character == L'\\') {
            ++slashes;
        } else {
            result.append(character == L'"' ? 2 * slashes + 1 : slashes, L'\\');
            result += character;
            slashes = 0;
        }
    }
    result.append(2 * slashes, L'\\');
    return result + L'"';
}

static DWORD launch(const fs::path& executable, std::wstring command,
                    const fs::path& cwd, const fs::path& stdoutPath,
                    const fs::path& stderrPath)
{
    // File redirection cannot deadlock if a child fills a pipe before exiting.
    SECURITY_ATTRIBUTES security{sizeof(SECURITY_ATTRIBUTES), nullptr, TRUE};
    Handle out(CreateFileW(stdoutPath.c_str(), GENERIC_WRITE, FILE_SHARE_READ,
                           &security, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr));
    Handle err(CreateFileW(stderrPath.c_str(), GENERIC_WRITE, FILE_SHARE_READ,
                           &security, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr));
    Handle input(CreateFileW(L"NUL", GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
                             &security, OPEN_EXISTING, 0, nullptr));
    Handle job(CreateJobObjectW(nullptr, nullptr));
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION limits{};
    limits.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
    checkWin32(SetInformationJobObject(job, JobObjectExtendedLimitInformation,
                                      &limits, sizeof(limits)), "SetInformationJobObject");

    STARTUPINFOW startup{};
    startup.cb = sizeof(startup);
    startup.dwFlags = STARTF_USESTDHANDLES;
    startup.hStdInput = input;
    startup.hStdOutput = out;
    startup.hStdError = err;
    PROCESS_INFORMATION process{};
    checkWin32(CreateProcessW(executable.c_str(), command.data(), nullptr, nullptr, TRUE,
                              CREATE_SUSPENDED | CREATE_NO_WINDOW, nullptr, cwd.c_str(),
                              &startup, &process), "CreateProcessW");
    Handle child(process.hProcess);
    Handle thread(process.hThread);
    try {
        // Assign before resuming so descendants cannot escape the test's job.
        checkWin32(AssignProcessToJobObject(job, child), "AssignProcessToJobObject");
        checkWin32(ResumeThread(thread) != static_cast<DWORD>(-1), "ResumeThread");
        const DWORD wait = WaitForSingleObject(child, 30000);
        require(wait != WAIT_TIMEOUT, "Pig exceeded its 30-second timeout");
        checkWin32(wait == WAIT_OBJECT_0, "WaitForSingleObject");
        DWORD code = 0;
        checkWin32(GetExitCodeProcess(child, &code), "GetExitCodeProcess");
        // Also reap descendants if the main process exited first.
        checkWin32(TerminateJobObject(job, code), "TerminateJobObject");
        return code;
    } catch (...) {
        TerminateJobObject(job, 1);
        // This also handles failure to assign the still-suspended child to the job.
        TerminateProcess(child, 1);
        WaitForSingleObject(child, 5000);
        throw;
    }
}

class Test
{
public:
    explicit Test(const std::string& name) : pig(pigPath()), root(temporaryDirectory(name)) {}

    std::string run(const fs::path& cwd, const std::vector<std::wstring>& arguments,
                    DWORD expectedCode = 0)
    {
        std::wstring command = quote(pig.wstring());
        for (const auto& argument : arguments)
            command += L" " + quote(argument);
        const auto stem = root / ("command-" + std::to_string(++commands));
        const fs::path out = stem.string() + ".stdout";
        const fs::path err = stem.string() + ".stderr";
        transcript << "\nWorking directory: " << cwd.string() << "\nCommand: "
                   << fs::path(command).string() << '\n';
        DWORD code;
        try {
            code = launch(pig, command, cwd, out, err);
        } catch (...) {
            capture(out, err);
            throw;
        }
        transcript << "Exit code: " << code << '\n';
        capture(out, err);
        require(code == expectedCode, "Expected exit code " + std::to_string(expectedCode) +
                                     ", got " + std::to_string(code));
        return readFile(out);
    }

    fs::path pig;
    fs::path root;
    std::ostringstream transcript;

private:
    unsigned commands = 0;
    void capture(const fs::path& out, const fs::path& err)
    {
        for (const auto& path : {out, err}) {
            transcript << path.filename().string() << ":\n";
            if (fs::exists(path))
                transcript << readFile(path);
            transcript << '\n';
        }
    }
};

static void writeFiles(const fs::path& directory, const Files& files)
{
    for (const auto& file : files)
        writeFile(directory / file.first, file.second);
}

static void verifyArchive(Test& test, const fs::path& archive,
                          const fs::path& extracted, const Files& expected)
{
    const auto listing = test.run(test.root, {L"w", archive.wstring(), L"--nocrash"});
    std::istringstream lines(listing);
    std::vector<std::string> actualNames;
    std::string line;
    while (std::getline(lines, line)) {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        if (!line.empty())
            actualNames.push_back(line);
    }
    std::vector<std::string> expectedNames;
    for (const auto& file : expected)
        expectedNames.push_back(file.first);
    std::sort(actualNames.begin(), actualNames.end());
    require(actualNames == expectedNames, "Archive listing does not match fixture: " + archive.string());

    require(fs::create_directory(extracted), "Extraction directory must be fresh");
    test.run(extracted, {L"x", archive.wstring(), L"--nocrash"});
    Files actual;
    for (const auto& entry : fs::recursive_directory_iterator(extracted)) {
        if (entry.is_directory())
            continue;
        require(entry.is_regular_file(), "Unexpected non-file: " + entry.path().string());
        const auto name = entry.path().lexically_relative(extracted).generic_string();
        actual.emplace(name, readFile(entry.path()));
    }
    require(actual.size() == expected.size(), "Extracted file count does not match fixture");
    for (const auto& file : expected) {
        const auto found = actual.find(file.first);
        require(found != actual.end(), "Missing extracted file: " + file.first);
        require(found->second == file.second, "Extracted bytes differ: " + file.first);
    }
}

static void usage(Test& test)
{
    // Supply a third argument for the invalid mode to test mode validation itself.
    for (const auto& arguments : std::vector<std::vector<std::wstring>>{
             {}, {L"--help"}, {L"q", L"unused.pigg"}}) {
        const auto output = test.run(test.root, arguments, 1);
        require(output.find("usage:") != std::string::npos &&
                output.find("modes (one required):") != std::string::npos &&
                output.find("flags:") != std::string::npos, "Expected Pig usage text");
    }
}

static void roundTrip(Test& test)
{
    std::string text;
    for (int i = 0; i < 4096; ++i)
        text += "Repeatable Pig compression fixture.\r\n";
    std::string binary;
    for (int i = 0; i < 8192; ++i)
        binary += static_cast<char>(i % 256);
    const Files files{{"compressible.txt", text}, {"binary.bin", binary},
                      {"nested/deeper/file with spaces.txt", "A nested file with spaces.\n"},
                      {"empty.txt", ""}};
    const auto source = test.root / "source files";
    writeFiles(source, files);
    const auto plain = test.root / "uncompressed archive.pigg";
    const auto packed = test.root / "compressed archive.pigg";
    test.run(test.root, {L"cif", plain.wstring(), source.wstring(), L"--nocrash"});
    verifyArchive(test, plain, test.root / "plain extraction", files);
    test.run(test.root, {L"cizf", packed.wstring(), source.wstring(), L"--nocrash"});
    verifyArchive(test, packed, test.root / "packed extraction", files);
    require(fs::file_size(packed) < fs::file_size(plain),
            "Compression did not reduce archive size");
}

static void setTimestamp(const fs::path& path, unsigned seconds)
{
    SYSTEMTIME time{};
    time.wYear = 2020;
    time.wMonth = 1;
    time.wDay = 2;
    FILETIME stamp{};
    checkWin32(SystemTimeToFileTime(&time, &stamp), "SystemTimeToFileTime");
    ULARGE_INTEGER ticks{};
    ticks.LowPart = stamp.dwLowDateTime;
    ticks.HighPart = stamp.dwHighDateTime;
    ticks.QuadPart += static_cast<ULONGLONG>(seconds) * 10000000;
    stamp.dwLowDateTime = ticks.LowPart;
    stamp.dwHighDateTime = ticks.HighPart;
    Handle file(CreateFileW(path.c_str(), FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ,
                            nullptr, OPEN_EXISTING, 0, nullptr));
    checkWin32(SetFileTime(file, nullptr, nullptr, &stamp), "SetFileTime");
}

static void incremental(Test& test)
{
    Files files{{"unchanged.txt", std::string(16384, 'U')},
                {"deleted.txt", "Remove this file.\n"},
                {"size.txt", "Original size.\n"},
                {"same size.txt", std::string(4096, 'A')}};
    const auto source = test.root / "source files";
    const auto archive = test.root / "incremental archive.pigg";
    writeFiles(source, files);
    for (const auto& file : files)
        setTimestamp(source / file.first, 0);
    test.run(test.root, {L"cizf", archive.wstring(), source.wstring(), L"--nocrash"});
    verifyArchive(test, archive, test.root / "initial extraction", files);

    require(fs::remove(source / "deleted.txt"), "Could not remove incremental fixture");
    files.erase("deleted.txt");
    files["added/new file.txt"] = "New incremental content.\n";
    writeFile(source / "added/new file.txt", files.at("added/new file.txt"));
    files["size.txt"] += "Now this file has a different size.\n";
    writeFile(source / "size.txt", files.at("size.txt"));
    // Keep this timestamp equal so the size change alone invalidates reuse.
    setTimestamp(source / "size.txt", 0);
    files["same size.txt"] = std::string(4096, 'B');
    writeFile(source / "same size.txt", files.at("same size.txt"));
    // Advance explicitly, well beyond filesystem timestamp granularity; no sleeps.
    setTimestamp(source / "same size.txt", 120);

    const auto output = test.run(test.root,
        {L"czvvf", archive.wstring(), source.wstring(), L"--nocrash"});
    require(output.find("unchanged.txt (reused old compressed version)") != std::string::npos,
            "Incremental creation did not report reuse of the unchanged file");
    verifyArchive(test, archive, test.root / "updated extraction", files);
}

int main(int argc, char** argv)
{
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);
    try {
        require(argc == 2, "Usage: PigTests PigUsage|PigRoundTrip|PigIncremental");
        const std::string name = argv[1];
        require(name == "PigUsage" || name == "PigRoundTrip" || name == "PigIncremental",
                "Unknown test case: " + name);
        Test test(name);
        try {
            if (name == "PigUsage") usage(test);
            else if (name == "PigRoundTrip") roundTrip(test);
            else incremental(test);
            fs::remove_all(test.root);
            std::cout << name << " passed\n";
        } catch (const std::exception& error) {
            std::cerr << name << " failed: " << error.what()
                      << "\nFixtures preserved at: " << test.root.string()
                      << '\n' << test.transcript.str();
            return 1;
        }
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
    return 0;
}
