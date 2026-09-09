#pragma once

#include <cstdio>
#include <filesystem>
#include <string>
#include <vector>

/* Stages generated files until CommitOutputs. These process-global interfaces
 * are single-threaded. Paths are borrowed, absolute destinations. I/O errors
 * throw. OpenOutput returns an owned stream that must pass through CloseFile.
 */
FILE *OpenOutput(const std::filesystem::path& destination);
// Read a staged output if present, otherwise the input path; null on failure.
FILE *OpenInput(const char *path, const char *mode);
// Close a generated or ordinary input stream, checking write/close errors.
void CloseFile(FILE *file);
// Return the unique generated destinations, including unchanged files.
std::vector<std::filesystem::path> GeneratedPaths();
// Compare staged contents, replacing only changed destinations. Errors throw.
void CommitOutputs();
// Remove this process's temporary files, including on exception exit.
void DiscardOutputs() noexcept;
// Atomically replace a private state file with the supplied bytes.
void WriteState(const std::filesystem::path& path, const std::string& bytes);
