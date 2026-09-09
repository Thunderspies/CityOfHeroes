#include <cmath>
#include <cstdio>
#include <limits>

#if defined(_MSC_VER)
#define COX_NOINLINE __declspec(noinline)
#else
#define COX_NOINLINE __attribute__((noinline))
#endif

namespace
{
// Taking the inputs at runtime prevents the assertions below from becoming
// tests of the front-end's constant evaluator instead of generated code.
COX_NOINLINE bool verifyPreciseBehavior(float infinity, float nan)
{
    // These are the canonical results produced by an optimized Release build
    // from the vs2026 CMake preset. They exercise the IEEE special-value
    // behavior that MSVC is permitted to discard under /fp:fast.
    const bool nanComparesUnequal = nan != nan;
    const bool infinityDifferenceIsNan = std::isnan(infinity - infinity);
    const bool nanDifferenceIsNan = std::isnan(nan - nan);

    return nanComparesUnequal && infinityDifferenceIsNan && nanDifferenceIsNan;
}
} // namespace

int main()
{
    const float infinity = std::numeric_limits<float>::infinity();
    const float nan = std::numeric_limits<float>::quiet_NaN();

    if (!verifyPreciseBehavior(infinity, nan))
    {
        std::fputs("Floating-point behavior is not precise.\n", stderr);
        return 1;
    }

    std::puts("Precise floating-point behavior verified.");
    return 0;
}
