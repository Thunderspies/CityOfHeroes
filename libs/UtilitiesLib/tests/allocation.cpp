#include <cstdio>
#include <new>
#include <vector>
#include "utilitieslib/stdtypes.h"
#include "utilitieslib/components/earray.h"
#include "checks.h"

int check_cpp_allocation(void)
{
	int *value = new int(32);
	int **array = NULL;
	const int **readonly = NULL;
	eaPush(&array, value);
	eaPushConst(&readonly, value);
	CHECK(eaSize(&array) == 1 && eaSize(&readonly) == 1);
	eaPushArrayConst(&readonly, &array);
	CHECK(eaSize(&readonly) == 2);
	CHECK(eaGet(&array, 0) == value && *value == 32);
	CHECK(eaGetConst(&readonly, 0) == value);
	eaDestroy(&array);
	eaDestroyConst(&readonly);
	delete value;
	char *bytes = new (std::nothrow) char[2048];
	CHECK(bytes != NULL);
	bytes[2047] = 42;
	CHECK(bytes[2047] == 42);
	delete[] bytes;
	std::vector<int> numbers(1024, 73);
	CHECK(numbers.front() == 73 && numbers.back() == 73);
	return 0;
}
