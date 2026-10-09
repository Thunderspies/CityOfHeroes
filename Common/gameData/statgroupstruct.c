#include "statgroupstruct.h"
#include "container/dbcontainerpack.h"
#include "entity/teamCommon.h"

LineDesc league_teamleaders_line_desc[] =
{
    // Note there's no offset, because we find the base address of the
    // array elements and they're just numbers so there's no offsetting
    // within a larger structure.
    {{PACKTYPE_INT, SIZE_INT32, "TeamLeadId", 0},
    "This members teamLeader Id"},
    {0}
};

StructDesc league_teamleaders_desc[] =
{
    //    array of team leaders
    //    this is meant to store the team leaders while there is a member update occuring
    sizeof(U32),
    {AT_STRUCT_ARRAY,{INDIRECTION(League, teamLeaderIDs, 0)}},
    league_teamleaders_line_desc,

    "League team leader ids\n"
};

LineDesc league_teamlock_line_desc[] =
{
    // Note there's no offset, because we find the base address of the
    // array elements and they're just numbers so there's no offsetting
    // within a larger structure.
    {{PACKTYPE_INT, SIZE_INT32, "TeamLockStatus", 0},
    "This members team lock status"},
    {0}
};
StructDesc league_teamlock_desc[] =
{
    //    array of team locks
    //    this is meant to store the team locks while there is a member update occuring
    sizeof(U32),
    {AT_STRUCT_ARRAY,{INDIRECTION(League, lockStatus, 0)}},
    league_teamlock_line_desc,

    "League team lock ids\n"
};

LineDesc league_line_desc[] =
{
    {{ PACKTYPE_INT,            SIZE_INT32,                        "LeaderId",        OFFSET2(League, members, TeamMembers, leader  ) },
                "DB ID - ContainerID of the leader of the team"},

    {{ PACKTYPE_INT,    SIZE_INT32,    "version",                OFFSET(League, revision) },        
                "The league revision. When members get added/quit too fast, the db can lag behind the statserver" },
    { PACKTYPE_SUB, MAX_LEAGUE_MEMBERS,    "TeamLeaderIds",        (intptr_t)league_teamleaders_desc        },

    { PACKTYPE_SUB, MAX_LEAGUE_MEMBERS,    "TeamLockStatus",        (intptr_t)league_teamlock_desc        },

    { 0 },
};

StructDesc league_desc[] =
{
    sizeof(League),
    {AT_NOT_ARRAY,{0}},
    league_line_desc,

    "Describe a league that consists of teams."    
};