#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <vorbis/vorbisenc.h>
#ifdef _DEBUG
#include <crtdbg.h>
#endif

// Compile the real wrapper once, including its private memory callbacks.
#include "../src/sound/ogg_decode.c"

enum { SAMPLE_RATE = 22050, SAMPLE_COUNT = 4096, FIXTURE_CAPACITY = 65536 };

typedef struct Fixture {
	unsigned char data[FIXTURE_CAPACITY];
	int size;
} Fixture;

static int failures;
static int crash_prompts;

static void check(int condition, const char *message)
{
	if (!condition) {
		printf("FAIL: %s\n", message);
		failures++;
	}
}

// The only unavailable Game UI dependency; any invocation fails the test.
int winMsgYesNo(char *message)
{
	(void)message;
	crash_prompts++;
	return 0;
}

static int append_page(Fixture *fixture, const ogg_page *page)
{
	long available = FIXTURE_CAPACITY - fixture->size;
	if (page->header_len < 0 || page->header_len > available ||
		page->body_len < 0 ||
		page->body_len > available - page->header_len)
		return 0;
	memcpy(fixture->data + fixture->size, page->header, page->header_len);
	fixture->size += page->header_len;
	memcpy(fixture->data + fixture->size, page->body, page->body_len);
	fixture->size += page->body_len;
	return 1;
}

static int encode_fixture(Fixture *fixture, int channels)
{
	vorbis_info info;
	vorbis_comment comment;
	vorbis_dsp_state encoder;
	vorbis_block block;
	ogg_stream_state stream;
	ogg_packet header, comment_header, code_header, packet;
	ogg_page page;
	float **pcm;
	int i, channel, success = 0;
	int have_encoder = 0, have_block = 0, have_stream = 0;

	fixture->size = 0;
	vorbis_info_init(&info);
	vorbis_comment_init(&comment);
	if (vorbis_encode_init_vbr(&info, channels, SAMPLE_RATE, 0.4f))
		goto cleanup;
	if (vorbis_analysis_init(&encoder, &info)) goto cleanup;
	have_encoder = 1;
	if (vorbis_block_init(&encoder, &block)) goto cleanup;
	have_block = 1;
	if (ogg_stream_init(&stream, channels)) goto cleanup;
	have_stream = 1;
	if (vorbis_analysis_headerout(&encoder, &comment, &header,
		&comment_header, &code_header) ||
		ogg_stream_packetin(&stream, &header) ||
		ogg_stream_packetin(&stream, &comment_header) ||
		ogg_stream_packetin(&stream, &code_header)) goto cleanup;
	while (ogg_stream_flush(&stream, &page))
		if (!append_page(fixture, &page)) goto cleanup;

	pcm = vorbis_analysis_buffer(&encoder, SAMPLE_COUNT);
	if (!pcm) goto cleanup;
	for (channel = 0; channel < channels; channel++)
		for (i = 0; i < SAMPLE_COUNT; i++)
			pcm[channel][i] = (float)(0.5 * sin(
				2.0 * 3.141592653589793 *
				(440.0 + channel * 220.0) * i / SAMPLE_RATE));
	if (vorbis_analysis_wrote(&encoder, SAMPLE_COUNT) ||
		vorbis_analysis_wrote(&encoder, 0))
		goto cleanup;
	while (vorbis_analysis_blockout(&encoder, &block) == 1) {
		if (vorbis_analysis(&block, NULL) ||
			vorbis_bitrate_addblock(&block))
			goto cleanup;
		while (vorbis_bitrate_flushpacket(&encoder, &packet)) {
			if (ogg_stream_packetin(&stream, &packet)) goto cleanup;
			// Give the initial PCM offset an unchanged page so
			// the final granule controls the advertised length.
			while (ogg_stream_flush(&stream, &page))
				if (!append_page(fixture, &page)) goto cleanup;
		}
	}
	while (ogg_stream_flush(&stream, &page))
		if (!append_page(fixture, &page)) goto cleanup;
	success = fixture->size > 0;

cleanup:
	if (have_stream) ogg_stream_clear(&stream);
	if (have_block) vorbis_block_clear(&block);
	if (have_encoder) vorbis_dsp_clear(&encoder);
	vorbis_comment_clear(&comment);
	vorbis_info_clear(&info);
	return success;
}

static SoundFileInfo sound_info(Fixture *fixture)
{
	SoundFileInfo info = {0};
	info.data = (char *)fixture->data;
	info.length = fixture->size;
	strcpy(info.name, "generated-ogg-fixture");
	return info;
}

static void check_callbacks(void)
{
	char data[] = "0123456789";
	unsigned char output[16];
	MemFile file = {data, 0, 10};
	int cursor;

	memset(output, 0xa5, sizeof(output));
	check(readbytes(output, 0, SIZE_MAX, &file) == 0 && file.curr == 0,
		"zero-size callback reads do not move the cursor");
	check(readbytes(output, SIZE_MAX, SIZE_MAX, &file) == 0 &&
		file.curr == 0,
		"oversized items do not overflow callback multiplication");
	check(readbytes(output, 3, SIZE_MAX, &file) == 3 && file.curr == 9 &&
		!memcmp(output, data, 9) && output[9] == 0xa5,
		"callback reads return and consume complete items only");
	check(readbytes(output, 2, 1, &file) == 0 && file.curr == 9,
		"partial trailing item remains unread");
	check(readbytes(output, 1, SIZE_MAX, &file) == 1 && file.curr == 10,
		"large callback count is bounded by remaining bytes");
	check(readbytes(output, 1, 1, &file) == 0 && file.curr == 10,
		"callback reaches EOF without overread");

	check(!seekbytes(&file, -1, SEEK_END) && file.curr == 9,
		"SEEK_END uses a signed offset from the end");
	check(!seekbytes(&file, -4, SEEK_CUR) && file.curr == 5,
		"SEEK_CUR supports valid negative offsets");
	cursor = file.curr;
	check(seekbytes(&file, -1, SEEK_SET) == -1 && file.curr == cursor,
		"negative SEEK_SET preserves the cursor");
	check(seekbytes(&file, 1, SEEK_END) == -1 && file.curr == cursor,
		"seek past EOF preserves the cursor");
	check(seekbytes(&file, INT64_MAX, SEEK_CUR) == -1 &&
		file.curr == cursor &&
		seekbytes(&file, INT64_MIN, SEEK_CUR) == -1 &&
		file.curr == cursor,
		"64-bit offset extremes do not overflow or change the cursor");
	check(seekbytes(&file, 0, 12345) == -1 && file.curr == cursor,
		"invalid seek origin preserves the cursor");
	check(!seekbytes(&file, 0, SEEK_END) && file.curr == 10 &&
		!seekbytes(&file, 0, SEEK_SET) && file.curr == 0,
		"seek accepts both stream boundaries");
	file.curr = -1;
	check(readbytes(output, 1, 1, &file) == 0 && file.curr == -1,
		"negative callback cursor is rejected");
	file.curr = 11;
	check(readbytes(output, 1, 1, &file) == 0 && file.curr == 11,
		"callback cursor beyond input is rejected");
}

static void check_state_count(int expected)
{
	OggState *state, *previous = NULL;
	int count = 0;
	for (state = oggStateList; state && count <= expected;
		state = state->sibling.next) {
		check(state->in_use && state->sibling.prev == previous,
			"live decoder list has consistent links");
		previous = state;
		count++;
	}
	check(!state && count == expected && oggStatesInUse == expected,
		"live decoder list and count agree");
}

static void release_decoder(DecodeState *decode)
{
	ogg_funcs.reset(decode);
	free(decode->codec_state);
	decode->codec_state = NULL;
}

static void check_decoding(Fixture *fixture, int channels)
{
	SoundFileInfo info = sound_info(fixture);
	DecodeState decode = {0};
	struct {
		unsigned char before[16];
		unsigned char pcm[SAMPLE_COUNT * 2 * 2];
		unsigned char after[16];
	} output;
	unsigned char first[512];
	int total = 0, bytes, i, expected = SAMPLE_COUNT * channels * 2;
	double energy[2] = {0};

	decode.info = &info;
	if (!ogg_funcs.init(&decode)) {
		check(0, "real Game wrapper opens generated Ogg");
		release_decoder(&decode);
		return;
	}
	check_state_count(1);
	check(decode.frequency == SAMPLE_RATE &&
		decode.num_channels == channels && decode.pcm_len == expected,
		"Game wrapper reports fixture PCM metadata");
	memset(&output, 0xa5, sizeof(output));
	decode.decode_buffer = output.pcm;
	decode.decode_len = sizeof(first);
	check(ogg_funcs.decode(&decode) == sizeof(first),
		"Game decodes the first PCM block");
	memcpy(first, output.pcm, sizeof(first));
	ogg_funcs.rewind(&decode);
	check(ogg_funcs.decode(&decode) == sizeof(first) &&
		!memcmp(first, output.pcm, sizeof(first)),
		"Game rewind repeats the exact first block");
	ogg_funcs.rewind(&decode);

	while (total < expected) {
		decode.decode_buffer = output.pcm + total;
		decode.decode_len = min(512, expected - total);
		bytes = ogg_funcs.decode(&decode);
		if (bytes <= 0 || bytes > decode.decode_len) break;
		total += bytes;
	}
	check(total == expected,
		"Game wrapper decodes the complete sample count");
	decode.decode_buffer = first;
	decode.decode_len = sizeof(first);
	check(ogg_funcs.decode(&decode) == 0 && decode.decode_count == 0,
		"Game wrapper reports EOF");
	for (i = 0; i < 16; i++)
		check(output.before[i] == 0xa5 && output.after[i] == 0xa5,
			"Game decode preserves output canaries");
	for (i = expected; i < sizeof(output.pcm); i++)
		check(output.pcm[i] == 0xa5,
			"Game decode stays within the PCM buffer length");
	for (i = 0; i < total / 2; i++) {
		short sample;
		memcpy(&sample, output.pcm + i * 2, sizeof(sample));
		energy[i % channels] += (double)sample * sample;
	}
	for (i = 0; i < channels; i++)
		check(energy[i] / SAMPLE_COUNT > 1000000.0,
			"each decoded channel contains signal");

	decode.decode_buffer = NULL;
	decode.decode_len = 0;
	check(ogg_funcs.decode(&decode) == 0,
		"zero-length decode needs no buffer");
	decode.decode_len = 1;
	check(ogg_funcs.decode(&decode) == -1,
		"positive-length decode rejects NULL output");
	decode.decode_len = -1;
	check(ogg_funcs.decode(&decode) == -1,
		"decode rejects negative output length");
	ogg_funcs.reset(&decode);
	check(!decode.frequency && !decode.num_channels && !decode.pcm_len &&
		!decode.decode_count && decode.codec_state,
		"reset clears decoder metadata and retains "
		"caller-owned allocation");
	ogg_funcs.reset(&decode);
	ogg_funcs.rewind(&decode);
	check(ogg_funcs.decode(&decode) == -1,
		"reset decoder cannot decode or rewind");
	check_state_count(0);
	check(ogg_funcs.init(&decode),
		"Game reuses caller-owned state after reset");
	release_decoder(&decode);
	check_state_count(0);

	check(oggToPcm(&info),
		"Game converts the entire fixture to a PCM cache");
	if (info.pcm_data) {
		check(info.frequency == SAMPLE_RATE &&
			info.num_channels == channels &&
			info.pcm_len == expected,
			"PCM cache reports correct metadata");
		check(info.pcm_len == expected &&
			!memcmp(info.pcm_data, output.pcm, expected),
			"PCM cache exactly matches streaming output");
		free(info.pcm_data);
	}
	check_state_count(0);
}

static void check_two_decoders(Fixture *fixture)
{
	SoundFileInfo info = sound_info(fixture);
	DecodeState first = {0}, second = {0};
	first.info = second.info = &info;
	check(ogg_funcs.init(&first) && ogg_funcs.init(&second),
		"two Game decoders open together");
	check_state_count(2);
	ogg_funcs.reset(&first);
	check_state_count(1);
	check(ogg_funcs.init(&first),
		"reinsert a reset decoder at the list head");
	check_state_count(2);
	release_decoder(&first);
	check_state_count(1);
	release_decoder(&second);
	check_state_count(0);
}

static void check_rejected(SoundFileInfo *info)
{
	SoundFileInfo original = *info;
	DecodeState decode = {0};
	decode.info = info;
	decode.frequency = decode.num_channels = decode.pcm_len = 123;
	check(!ogg_funcs.init(&decode), "Game rejects invalid Ogg input");
	check(!decode.frequency && !decode.num_channels && !decode.pcm_len,
		"failed Game decoder clears stale PCM metadata");
	release_decoder(&decode);
	check(!oggToPcm(info) && !memcmp(info, &original, sizeof(*info)),
		"failed full conversion preserves caller PCM output");
	if (info->pcm_data != original.pcm_data)
		free(info->pcm_data);
	*info = original;
	check_state_count(0);
}

// Keep packets intact while changing the final advertised PCM count. This
// exercises validation and short conversion through upstream decoding itself.
static int set_final_granule(Fixture *fixture, ogg_int64_t granule)
{
	int offset = 0;
	while (offset + 27 <= fixture->size) {
		unsigned char *header = fixture->data + offset;
		int header_size = 27 + header[26], body_size = 0, i;
		ogg_page page;
		if (memcmp(header, "OggS", 4) ||
			header_size > fixture->size - offset)
			return 0;
		for (i = 27; i < header_size; i++) body_size += header[i];
		if (body_size > fixture->size - offset - header_size) return 0;
		if (offset + header_size + body_size == fixture->size) {
			for (i = 0; i < 8; i++)
				header[6 + i] = (unsigned char)(
					(uint64_t)granule >> (i * 8));
			page.header = header;
			page.header_len = header_size;
			page.body = header + header_size;
			page.body_len = body_size;
			ogg_page_checksum_set(&page);
			return 1;
		}
		offset += header_size + body_size;
	}
	return 0;
}

static int fixture_has_samples(Fixture *fixture, ogg_int64_t expected)
{
	OggVorbis_File decoder;
	MemFile file = {(char *)fixture->data, 0, fixture->size};
	ogg_int64_t samples;
	if (ov_open_callbacks(&file, &decoder, NULL, 0, callbacks))
		return 0;
	samples = ov_pcm_total(&decoder, -1);
	ov_clear(&decoder);
	return samples == expected;
}

static void check_invalid_inputs(Fixture *fixture)
{
	SoundFileInfo info = sound_info(fixture);
	Fixture altered = *fixture;
	char junk[] = "This is not an Ogg stream";
	int attempt;
	DecodeState reused = {0};
#ifdef _DEBUG
	_CrtMemState before, after, difference;
#endif

	info.data = NULL;
	check_rejected(&info);
	// Sound playback shares codec_state allocations between PCM and Ogg.
	reused.info = &info;
	reused.codec_state = calloc(1, sizeof(MemFile));
	check(reused.codec_state && !ogg_funcs.init(&reused),
		"failed Ogg init can reuse a smaller PCM allocation safely");
	ogg_funcs.rewind(&reused);
	check(ogg_funcs.decode(&reused) == -1,
		"failed reused decoder stays inactive");
	release_decoder(&reused);
	check_state_count(0);
	info.data = (char *)fixture->data;
	info.length = 0;
	check_rejected(&info);
	info.length = -1;
	check_rejected(&info);
	info.length = 16;
	check_rejected(&info);
	info.data = junk;
	info.length = sizeof(junk);
	check_rejected(&info);

	if (!set_final_granule(&altered, (ogg_int64_t)INT_MAX) ||
		!fixture_has_samples(&altered, (ogg_int64_t)INT_MAX)) {
		check(0,
			"oversized fixture advertises INT_MAX PCM frames "
			"upstream");
		return;
	}
	info = sound_info(&altered);
	check_rejected(&info);

#ifdef _DEBUG
	_CrtMemCheckpoint(&before);
#endif
	for (attempt = 0; attempt < 64; attempt++) {
		info.data = junk;
		info.length = sizeof(junk);
		check_rejected(&info);
		info = sound_info(fixture);
		info.length = 16;
		check_rejected(&info);
		info = sound_info(&altered);
		check_rejected(&info);
	}
#ifdef _DEBUG
	_CrtMemCheckpoint(&after);
	check(!_CrtMemDifference(&difference, &before, &after),
		"rejected Game Ogg decoders release all allocations");
#endif
}

static void check_incomplete_cache(Fixture *fixture)
{
	Fixture altered = *fixture;
	SoundFileInfo info;
	DecodeState decode = {0};
	unsigned char output[SAMPLE_COUNT * 4];
	int result;
	if (!set_final_granule(&altered, SAMPLE_COUNT * 2) ||
		!fixture_has_samples(&altered, SAMPLE_COUNT * 2)) {
		check(0, "short fixture advertises twice its PCM frame count "
			"upstream");
		return;
	}
	info = sound_info(&altered);
	decode.info = &info;
	if (ogg_funcs.init(&decode)) {
		decode.decode_buffer = output;
		decode.decode_len = sizeof(output);
		result = ogg_funcs.decode(&decode);
		check(result >= 0 && result < decode.pcm_len,
			"short fixture can stream a bounded decoded prefix");
	}
	release_decoder(&decode);
	check(!oggToPcm(&info) && !info.pcm_data && !info.pcm_len,
		"full conversion never publishes a short PCM cache "
		"with silent padding");
	free(info.pcm_data);
	check_state_count(0);
}

int main(void)
{
	Fixture fixture;
	int channels;
	check_state_count(0);
	check_callbacks();
	for (channels = 1; channels <= 2; channels++) {
		if (!encode_fixture(&fixture, channels)) {
			check(0, "encode bounded test fixture");
			return 1;
		}
		check_decoding(&fixture, channels);
		check_two_decoders(&fixture);
	}
	check_invalid_inputs(&fixture);
	check_incomplete_cache(&fixture);
	check(!crash_prompts, "Game decoder never invokes crash UI");
	if (!failures)
		puts("Game Ogg callback, decode, rewind, reset, cache "
			"and cleanup checks passed");
	return failures ? 1 : 0;
}
