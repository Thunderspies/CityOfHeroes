#pragma once

#include <algorithm>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <memory>
#include <string>

// Existing parser buffers retain their historical maximum path length.
constexpr int MAX_PATH = 260;

// Compare borrowed, non-null strings using the parser's ASCII case folding.
int CompareNoCase(const char *left, const char *right);

// Return the running executable's absolute path; failures throw.
std::filesystem::path ExecutablePath();

/* Write an AutoGen destination into out (at least MAX_PATH bytes). Directory
 * and filename are borrowed non-null inputs. Only the filename's final path
 * component is used. Throws if the resulting path exceeds the parser buffer.
 */
void SetAutoGenPath(char *out, const char *directory,
	const std::string& filename);
