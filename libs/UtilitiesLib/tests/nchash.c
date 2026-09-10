#include <stdio.h>
#include <string.h>
#include "utilitieslib/stdtypes.h"
#include "utilitieslib/UtilsNew/ncHash.h"
#include "checks.h"

/* Exercise the actual wrapper, including its ownership and lazy creation. */
int check_nchash(void)
{
	int values[512];
	HashStr *strings = NULL;
	HashPtr *pointers = NULL;
	CHECK(!HashStr_Find(&strings, "missing"));
	CHECK(!HashStr_Remove(&strings, "missing"));
	CHECK(!HashStr_Insert(&strings, NULL, &values[0], FALSE));
	CHECK(strings == NULL);
	for (int i = 0; i < 512; ++i) {
		char key[32];
		values[i] = i;
		snprintf(key, sizeof(key), "key-%d", i);
		CHECK(HashStr_Insert(&strings, key, &values[i], FALSE)
			== &values[i]);
		/* The temporary stack key must have been copied by lazy creation. */
		memset(key, 'x', sizeof(key));
	}
	for (int i = 0; i < 512; ++i) {
		char key[32];
		snprintf(key, sizeof(key), "key-%d", i);
		CHECK(HashStr_Find(&strings, key) == &values[i]);
	}
	CHECK(!HashStr_Insert(&strings, "key-10", &values[11], FALSE));
	CHECK(HashStr_Find(&strings, "key-10") == &values[10]);
	CHECK(HashStr_Insert(&strings, "key-10", &values[11], TRUE)
		== &values[11]);
	CHECK(HashStr_Find(&strings, "key-10") == &values[11]);
	CHECK(HashStr_Remove(&strings, "key-10"));
	CHECK(!HashStr_Remove(&strings, "key-10"));
	CHECK(HashStr_Insert(&strings, "key-10", &values[10], FALSE)
		== &values[10]);
	HashStr_Destroy(&strings);
	CHECK(strings == NULL);
	HashStr_Destroy(&strings);

	/* A borrowed key stays caller-owned across erase and destruction. */
	char borrowed[] = "borrowed";
	strings = HashStr_Create(0, FALSE);
	CHECK(HashStr_Insert(&strings, borrowed, &values[0], FALSE)
		== &values[0]);
	CHECK(HashStr_Remove(&strings, borrowed));
	CHECK(HashStr_Insert(&strings, borrowed, &values[1], FALSE)
		== &values[1]);
	HashStr_Destroy(&strings);
	CHECK(strcmp(borrowed, "borrowed") == 0);

	CHECK(!HashPtr_Find(&pointers, &values[0]));
	CHECK(!HashPtr_Insert(&pointers, NULL, &values[0], FALSE));
	CHECK(pointers == NULL);
	for (int i = 0; i < 512; ++i)
		CHECK(HashPtr_Insert(&pointers, &values[i], &values[i], FALSE)
			== &values[i]);
	for (int i = 0; i < 512; ++i)
		CHECK(HashPtr_Find(&pointers, &values[i]) == &values[i]);
	CHECK(!HashPtr_Insert(&pointers, &values[0], &values[1], FALSE));
	CHECK(HashPtr_Insert(&pointers, &values[0], &values[1], TRUE)
		== &values[1]);
	CHECK(HashPtr_Find(&pointers, &values[0]) == &values[1]);
	for (int i = 0; i < 512; i += 2) {
		CHECK(HashPtr_Remove(&pointers, &values[i]));
		CHECK(!HashPtr_Find(&pointers, &values[i]));
		CHECK(!HashPtr_Remove(&pointers, &values[i]));
	}
	for (int i = 0; i < 512; i += 2)
		CHECK(HashPtr_Insert(&pointers, &values[i], &values[i], FALSE)
			== &values[i]);
	HashPtr_Destroy(&pointers);
	CHECK(pointers == NULL);
	HashPtr_Destroy(&pointers);
	return 0;
}
