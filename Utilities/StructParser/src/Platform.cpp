#include "Platform.h"
#include <stdexcept>
#include <vector>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

int CompareNoCase(const char *left, const char *right)
{
	auto lower = [](unsigned char c) {
		return c >= 'A' && c <= 'Z' ? c + ('a' - 'A') : c;
	};
	while (*left && lower(*left) == lower(*right)) {
		++left;
		++right;
	}
	return lower(*left) - lower(*right);
}

std::filesystem::path ExecutablePath()
{
#ifdef _WIN32
	std::vector<wchar_t> buffer(32768);
	auto length = GetModuleFileNameW(nullptr, buffer.data(), buffer.size());
	if (!length || length == buffer.size())
		throw std::runtime_error(
			"Cannot locate StructParser executable");
	return std::filesystem::path(buffer.data());
#else
	return std::filesystem::read_symlink("/proc/self/exe");
#endif
}

void SetAutoGenPath(char *out, const char *directory,
	const std::string& filename)
{
	auto path = (std::filesystem::path(directory) / "AutoGen" /
		std::filesystem::path(filename).filename()).generic_string();
	if (path.size() >= MAX_PATH)
		throw std::runtime_error("Generated path is too long: " + path);
	std::copy(path.begin(), path.end(), out);
	out[path.size()] = 0;
}
