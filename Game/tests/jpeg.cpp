#include <array>
#include <cstdio>
#include <fstream>
#include <iterator>
#include <vector>
#ifdef _DEBUG
#include <crtdbg.h>
#endif
#include <jpeglib.h>
#include <jpge.h>
#include <utilitieslib/stdtypes.h>
#include <utilitieslib/utils/memcheck.h>
#include "graphics/jpeg.h"
#include "render/tex.h"

static int failures;

static void check(bool condition, const char *message)
{
	if (!condition) {
		printf("FAIL: %s\n", message);
		failures++;
	}
}

static void expect_rejected(char *data, int size, const char *message)
{
	TexReadInfo info;
	memset(&info, 0x5a, sizeof(info));
	TexReadInfo original;
	memcpy(&original, &info, sizeof(info));

	int result = jpegLoad(data, size, &info);
	check(!result, message);
	if (result)
		free(info.data);
	else
		check(!memcmp(&info, &original, sizeof(info)),
			"failure preserves caller output");
}

static std::vector<U8> encode(const U8 *pixels, int channels)
{
	std::vector<U8> jpeg(65536);
	int size = (int)jpeg.size();
	jpge::params params;
	params.m_quality = 100;
	params.m_subsampling = channels == 1 ? jpge::Y_ONLY : jpge::H1V1;
	bool ok = jpge::compress_image_to_jpeg_file_in_memory(jpeg.data(),
		size, 8, 8, channels, pixels, params);
	check(ok, "encode test fixture");
	jpeg.resize(ok ? size : 0);
	return jpeg;
}

static void check_saved_metadata(char *path, U8 *pixels)
{
	char metadata[] = { 'C', 'o', 'X', 0, 1, 2, 3, (char)255 };
	jpgSaveEx(path, pixels, 3, 8, 8, metadata, sizeof(metadata));
	std::ifstream input(path, std::ios::binary);
	std::vector<char> encoded((std::istreambuf_iterator<char>(input)),
		std::istreambuf_iterator<char>());
	check(!encoded.empty(), "Game writer creates JPEG fixture");
	if (encoded.empty())
		return;

	jpeg_decompress_struct decoder = {};
	jpeg_error_mgr errors;
	decoder.err = jpeg_std_error(&errors);
	jpeg_create_decompress(&decoder);
	jpeg_mem_src(&decoder, (const unsigned char *)encoded.data(),
		(unsigned long)encoded.size());
	jpeg_save_markers(&decoder, JPEG_APP0 + 13, 65535);
	check(jpeg_read_header(&decoder, TRUE) == JPEG_HEADER_OK,
		"Game writer emits a valid JPEG header");
	bool found = false;
	for (jpeg_saved_marker_ptr marker = decoder.marker_list; marker;
		marker = marker->next) {
		if (marker->marker == JPEG_APP0 + 13 &&
			marker->data_length == sizeof(metadata) &&
			!memcmp(marker->data, metadata, sizeof(metadata)))
			found = true;
	}
	check(found, "Game writer preserves binary APP13 metadata");
	jpeg_destroy_decompress(&decoder);

	TexReadInfo info = {};
	int result = jpegLoad(encoded.data(), (int)encoded.size(), &info);
	check(result == 1, "Game decodes its own saved JPEG");
	if (result)
		free(info.data);
}

int main(int argc, char **argv)
{
	if (argc != 2)
		return 2;
	std::array<U8, 8 * 8 * 3> pixels;
	for (size_t i = 0; i < pixels.size(); i += 3) {
		pixels[i] = 32;
		pixels[i + 1] = 128;
		pixels[i + 2] = 224;
	}
	std::vector<U8> rgb = encode(pixels.data(), 3);
	if (rgb.empty())
		return 1;

	TexReadInfo info = {};
	int result = jpegLoad((char *)rgb.data(), (int)rgb.size(), &info);
	check(result == 1, "decode RGB image");
	if (result) {
		check(info.width == 8 && info.height == 8 &&
			info.size == 8 * 8 * 3, "packed RGB dimensions");
		if (info.size == (int)pixels.size()) {
			for (size_t i = 0; i < pixels.size(); i++) {
				int difference = info.data[i] - pixels[i];
				check(difference >= -2 && difference <= 2,
					"RGB channels preserve color");
			}
		}
		free(info.data);
	}

	std::array<U8, 8 * 8> gray = {};
	std::vector<U8> grayscale = encode(gray.data(), 1);
	expect_rejected((char *)grayscale.data(), (int)grayscale.size(),
		"grayscale remains unsupported");
	expect_rejected(NULL, (int)rgb.size(), "reject NULL input");
	expect_rejected((char *)rgb.data(), 0, "reject empty input");
	expect_rejected((char *)rgb.data(), -1, "reject negative input size");
	check(!jpegLoad((char *)rgb.data(), (int)rgb.size(), NULL),
		"reject NULL output");
	char invalid[] = "This is not a JPEG";
	expect_rejected(invalid, sizeof(invalid), "reject malformed JPEG");
	expect_rejected((char *)rgb.data(), 2, "reject truncated JPEG header");
#ifdef _DEBUG
	_CrtMemState before, after, difference;
	_CrtMemCheckpoint(&before);
	for (int attempt = 0; attempt < 128; attempt++) {
		expect_rejected(invalid, sizeof(invalid),
			"repeated malformed JPEG rejection");
		expect_rejected((char *)rgb.data(), 2,
			"repeated truncated JPEG rejection");
	}
	_CrtMemCheckpoint(&after);
	check(!_CrtMemDifference(&difference, &before, &after),
		"failed JPEG decoders release their allocations");
#endif

	bool found_frame = false;
	for (size_t i = 0; i + 8 < rgb.size(); i++) {
		if (rgb[i] == 0xff && rgb[i + 1] == 0xc0) {
			found_frame = true;
			std::vector<U8> invalid_size = rgb;
			invalid_size[i + 5] = 0;
			invalid_size[i + 6] = 0;
			expect_rejected((char *)invalid_size.data(),
				(int)invalid_size.size(), "reject zero height");
			invalid_size[i + 5] = invalid_size[i + 6] = 0xff;
			invalid_size[i + 7] = invalid_size[i + 8] = 0xff;
			expect_rejected((char *)invalid_size.data(),
				(int)invalid_size.size(), "reject oversized image");
			break;
		}
	}
	check(found_frame, "fixture contains baseline frame dimensions");
	check_saved_metadata(argv[1], pixels.data());
	if (!failures)
		printf("JPEG consumer checks passed\n");
	return failures ? 1 : 0;
}
