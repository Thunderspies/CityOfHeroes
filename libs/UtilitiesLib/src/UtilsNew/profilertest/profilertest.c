#include "../../../include/utilitieslib/UtilsNew/profiler.h"
#include <stdio.h>

#ifdef ENABLE_PROFILER
#include <process.h>
#include <time.h>
#include <assert.h>

#pragma inline_depth(0)
int four(int a, int b, int c, int d)
{
	return a+b+c+d;
}

int three(int a, int b, int c)
{
	int ret = four(1,2,3,4);
	assert(ret == 10);
	return a+b+c;
}

int two(int a, int b)
{
	int ret = three(1,2,3);
	assert(ret == 6);
	return a+b;
}

int one(int a)
{
	int ret = two(1, 2);
	assert(ret == 3);
	return a;
}

int test(void)
{
	int i;
	int ret = 0;
	for(i=0; i<10000; i++) {
		ret += four(1,one(1),two(1,one(1)),three(1,one(1),two(1,one(1))));
	}
	return ret;
}

volatile int threadShouldExit = 0;
void threadtest(void * arg)
{
	int ret = 0;
	while(!threadShouldExit) {
		ret += test();
	}
	*(int*)arg = ret;
}

int main(int argc, char *argv[])
{
	int i;
	clock_t start;
	unsigned __int64 startTSC;
	int ret;

	(void)argc;
	(void)argv;

	ret = one(1);
	assert(ret == 1);

	// Abort profile test
	BeginProfile(32*1024*1024);
	ret += test();
	EndProfile(NULL);

	// Memory allocation failure
	BeginProfile(~0U);
	ret += test();
	EndProfile(NULL);

	// Out of memory test
	BeginProfile(16);
	ret += test();
	EndProfile(NULL);

	// Repeat runs test
	start = clock();
	for(i=0; i<10000; i++) {
		BeginProfile(64*1024*1024);
		EndProfile(NULL);
	}
	printf("Begin/End = %u\n", clock()-start);

	// Thread test
	threadShouldExit = 0;
	for(i=0; i<4; i++) {
		_beginthread(threadtest, 0, &ret);
	}
	start = clock();
	for(i=0; i<1000; i++) {
		BeginProfile(64*1024*1024);
		ret += one(1);
		EndProfile(NULL);
	}	
	threadShouldExit = 1;
	printf("Thread = %u\n", clock()-start);

	// Lots of data test with profiler off
	start = clock();
	startTSC = __rdtsc();
	for(i=0; i<10; i++) {
		ret += test();
	}
	printf("No Profile = %u (%I64u)\n", clock()-start, (__rdtsc() - startTSC) / 10);

	// Lots of data test with profiler on
	BeginProfile(128*1024*1024);
	start = clock();
	startTSC = __rdtsc();
	for(i=0; i<10; i++) {
		ret += test();
	}
	printf("Profile = %u (%I64u)\n", clock()-start, (__rdtsc() - startTSC) / 10);
	EndProfile(NULL);

	// Save results test
	BeginProfile(32*1024*1024);
	ret += test();
	EndProfile("test.txt");

	// Force the optimizer to not delete everything
	printf("Result = %d\n", ret);

	ret = one(1);
	assert(ret == 1);
	return 0;
}

#else

int main(void)
{
    puts("Profiling is unsupported in this build.");
    return 0;
}

#endif // ENABLE_PROFILER
