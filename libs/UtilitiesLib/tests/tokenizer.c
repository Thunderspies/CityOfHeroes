#include <stdio.h>
#include <string.h>
#include "utilitieslib/utils/wininclude.h"
#include "utilitieslib/utils/memcheck.h"
#include "utilitieslib/utils/SuperAssert.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/utils/structTokenizer.h"
#include "utilitieslib/utils/textparser.h"
#include "checks.h"

#undef fprintf
#undef snprintf

static int check_line_tokenizer(void)
{
	const char* endings[] = {"\n", "\r\n", "\r"};
	for (int i = 0; i < ARRAY_SIZE(endings); ++i) {
		char storage[256], *cursor = storage + 1, *args[4];
		/* Model the heap metadata byte that triggered the MapServer crash. */
		storage[0] = '\r';
		snprintf(cursor, sizeof(storage) - 1, "%s \t%sDef \"Example Name\"%sEnd",
			endings[i], endings[i], endings[i]);
		CHECK(tokenize_line_safe(cursor, args, ARRAY_SIZE(args), &cursor) == 0);
		CHECK(storage[0] == '\r');
		CHECK(tokenize_line_safe(cursor, args, ARRAY_SIZE(args), &cursor) == 0);
		CHECK(tokenize_line_safe(cursor, args, ARRAY_SIZE(args), &cursor) == 2);
		CHECK(strcmp(args[0], "Def") == 0 && strcmp(args[1], "Example Name") == 0);
		CHECK(tokenize_line_safe(cursor, args, ARRAY_SIZE(args), &cursor) == 1);
		CHECK(strcmp(args[0], "End") == 0 && cursor == NULL);
		CHECK(storage[0] == '\r');
	}
	return 0;
}

typedef struct ParserFixture {
	int first;
	int second;
} ParserFixture;

static ParseTable fixture_parse[] = {
	{"First", TOK_INT(ParserFixture, first, 0)},
	{"Second", TOK_INT(ParserFixture, second, 0)},
	{"", 0}
};

static int check_textparser(const char* eol)
{
	char input[256];
	ParserFixture fixture = {0};
	snprintf(input, sizeof(input), " \t%s#%sFirst \t7%s// comment%s\tSecond 9",
		eol, eol, eol, eol);
	CHECK(ParserReadText(input, -1, fixture_parse, &fixture));
	CHECK(fixture.first == 7 && fixture.second == 9);
	TokenizerHandle tok = TokenizerCreateString(input, -1);
	const char* token = TokenizerGet(tok, 1, 1);
	CHECK(token && strcmp(token, "First") == 0);
	CHECK(TokenizerGetCurLine(tok) == 3);
	TokenizerDestroy(tok);
	return 0;
}

int main(void)
{
	memCheckInit();
	setGuiDisable(true);
	setAssertUnitTesting(true);
	setAssertMode(ASSERTMODE_EXIT | ASSERTMODE_STDERR);
	CHECK(check_line_tokenizer() == 0);
	const char* endings[] = {"\n", "\r\n"};
	for (int i = 0; i < ARRAY_SIZE(endings); ++i)
		CHECK(check_textparser(endings[i]) == 0);
	puts("Tokenizer and TextParser line-ending checks passed");
	return 0;
}
