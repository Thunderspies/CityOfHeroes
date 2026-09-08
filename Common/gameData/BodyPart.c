/***************************************************************************
*     Copyright (c) 2000-2006, Cryptic Studios
*     All Rights Reserved
*     Confidential Property of Cryptic Studios
***************************************************************************/
#include <utilitieslib/stdtypes.h>
#include "gameData/BodyPart.h"
#include <utilitieslib/utils/textparser.h>
#include "seq/seq.h"
#include <string.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/strings_opt.h>
#include <utilitieslib/assert/assert.h>

SHARED_MEMORY BodyPartList bodyPartList;

TokenizerParseInfo ParseBodyPart[] =
{
    { "Name",            TOK_STRING(BodyPart, name, 0)            },
    { "SourceFile",        TOK_CURRENTFILE(BodyPart,sourceFile)    },
    { "BoneCount",        TOK_INT(BodyPart, num_parts, 0)            },
    { "InfluenceCost",    TOK_INT(BodyPart, influenceCost, 0)        },
    { "GeoName",        TOK_STRING(BodyPart, name_geo, 0)        },
    { "TexName",        TOK_STRING(BodyPart, name_tex, 0)        },
    { "BaseName",        TOK_STRING(BodyPart, base_var, 0)        },
    { "BoneCount",        TOK_INT(BodyPart, num_parts, 0)            },
    { "DontClear",        TOK_BOOLFLAG(BodyPart, dont_clear, 0)    },

    // these are calculated, but have parser entries for copying to shared memory
    { "",                TOK_INT(BodyPart, bone_num[0], 0)        },
    { "",                TOK_INT(BodyPart, bone_num[1], 0)        },
    { "",                TOK_INT(BodyPart, bodyPartIndex, -1)    },

    { "End",            TOK_END                                    },
    { "", 0, 0 }
};

TokenizerParseInfo ParseBodyPartBegin[] =
{
    { "BodyPart",        TOK_STRUCT(BodyPartList, bodyParts, ParseBodyPart)    },
    { "", 0, 0 }
};

bool ParseBoneNames(ParseTable* pti, void* structptr)
{
    BodyPartList* bplist = (BodyPartList*)structptr;
    int i;
    
    // Fill in fields that were skipped by the parser.
    if(eaSize(&bodyPartList.bodyParts) >= MAX_BODYPARTS)
        FatalErrorf("Too many body parts!");

    for(i = 0; i < eaSize(&bodyPartList.bodyParts); i++)
    {
        BodyPart *part = cpp_const_cast(BodyPart*)(bplist->bodyParts[i]);
        part->bodyPartIndex = i;
        
        if( part->num_parts == 2 ) // represent 2 parts so cat on Left and right values
        {
            char tmp[128];

            sprintf( tmp, "%sR", part->name_geo ); 
            part->bone_num[0] = bone_IdFromName(tmp);
//             if(!bone_IdIsValid(part->bone_num[0]))
//                 FatalErrorf("Could not find right bone %s for body part %s", tmp, part->name);

            sprintf( tmp, "%sL", part->name_geo );
            part->bone_num[1] = bone_IdFromName(tmp);
//             if(!bone_IdIsValid(part->bone_num[1]))
//                 FatalErrorf("Could not find left bone %s for body part %s", tmp, part->name);
        }
        else if(part->num_parts == 1)
        {
            part->bone_num[0] = bone_IdFromName(part->name_geo);
//             if(!bone_IdIsValid(part->bone_num[0]))
//                 FatalErrorf("Could not find bone %s for body part %s", part->name_geo, part->name);
        }
        else
        {
            FatalErrorf("%s has an invalid number of bones %d!", part->name, part->num_parts);
        }

    }
    return 1;
}

void bpReadBodyPartFiles(void)
{
    // Load up all bone definitions.
    ParserLoadFilesShared("SM_BodyParts.bin", "Defs\\UI", ".bp", "BodyParts.bin", 0, ParseBodyPartBegin, &bodyPartList, sizeof(bodyPartList), NULL, NULL, ParseBoneNames, NULL, NULL);
}

const BodyPart* bpGetBodyPartFromName(const char* name)
{
    int i = 0;

    while( i < eaSize(&bodyPartList.bodyParts) )
        if( stricmp( name, bodyPartList.bodyParts[i]->name ) == 0 )
            return bodyPartList.bodyParts[i];
        else
            i++;

    return NULL;
}

int bpGetIndexFromName(const char *name)
{
    if(name)
    {
        int i;
        for(i = 0; i < eaSize(&bodyPartList.bodyParts); ++i)
            if(!stricmp(name, bodyPartList.bodyParts[i]->name))
                return i;
    }
    return -1;
}

int GetBodyPartCount(void)
{
    return eaSize(&bodyPartList.bodyParts);
}
