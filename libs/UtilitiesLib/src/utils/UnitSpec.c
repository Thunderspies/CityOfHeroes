#include "utilitieslib/utils/UnitSpec.h"

UnitSpec byteSpec[] = 
{
    {"byte",    1,                        1},
    {"KB",        1024,                    1000},
    {"MB",        1024*1024,                1000*1024},
    {"GB",        1024*1024*1024,            1000*1024*1024},
//    {"TB",        1024*1024*1024*1024,    1000*1024*1024*1024},
    {0},
};

UnitSpec kbyteSpec[] = 
{
    {"KB",    1,                1},
    {"MB",    1024,            1000},
    {"GB",    1024*1024,        1000*1024},
    {"TB",    1024*1024*1024,    1000*1024*1024},
    {0},
};

UnitSpec mbyteSpec[] = 
{
    {"MB",    1,                1},
    {"GB",    1024,            1000},
    {"TB",    1024*1024,        1000*1024},
    {"PB",    1024*1024*1024,    1000*1024*1024},
    {0},
};

UnitSpec timeSpec[] = 
{
    {"sec",    1, 1},
    {"min",    60, 60},
    {"hr",    60 * 60, 60*60},
    {"day",    24 * 60 * 60, 24*60*60},
    {0},
};

UnitSpec microSecondSpec[] = 
{
    {"us",    1, 1},
    {"ms",    1000, 1000},
    {"sec",    1000 * 1000, 1000 * 1000},
    {"min",    60 * 1000 * 1000, 60 * 1000 * 1000},
    {0},
};

UnitSpec* usFindProperUnitSpec(UnitSpec* specs, S64 size)
{
    int i;
    size = size<0?-size:size;
    for(i = 0; specs[i].unitName; i++)
    {
        if(specs[i].switchBoundary > size)
            return &specs[i > 0 ? i-1 : 0];
    }
    return &specs[i-1];
}

char *friendlyBytes(S64 numbytes)
{
    static char buf[128];
    UnitSpec *spec = usFindProperUnitSpec(byteSpec, numbytes);

    if(1 != spec->unitBoundary)
        snprintf(buf, ARRAY_SIZE(buf), "%1.2f %s", (F32)numbytes / spec->unitBoundary, spec->unitName);
    else
        snprintf(buf, ARRAY_SIZE(buf), "%1.f %s", (F32)numbytes / spec->unitBoundary, spec->unitName);
    return buf;
}
