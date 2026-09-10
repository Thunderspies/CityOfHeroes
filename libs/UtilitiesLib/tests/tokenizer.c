#include <stdio.h>
#include <string.h>
#include "utilitieslib/utils/wininclude.h"
#include "utilitieslib/utils/memcheck.h"
#include "utilitieslib/utils/SuperAssert.h"
#include "utilitieslib/utils/utils.h"
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

int main(void)
{
	memCheckInit();
	setGuiDisable(true);
	setAssertUnitTesting(true);
	setAssertMode(ASSERTMODE_EXIT | ASSERTMODE_STDERR);
	CHECK(check_line_tokenizer() == 0);
	puts("Tokenizer line-ending checks passed");
	return 0;
}
