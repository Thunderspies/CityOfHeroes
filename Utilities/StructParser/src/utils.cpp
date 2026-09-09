#include "GenerationIO.h"
#include "pch.h"
#include "tokenizer.h"
#include "utils.h"

FILE *fopen_nofail(const char *name, const char *mode)
{
	auto file = strchr(mode, 'w') ? OpenOutput(name) :
		OpenInput(name, mode);
	Tokenizer::StaticAssertf(file != nullptr,
		"Couldn't open file %s(%s)", name, mode);
	return file;
}

bool FileExists(const char *path)
{
	return std::filesystem::is_regular_file(path);
}
