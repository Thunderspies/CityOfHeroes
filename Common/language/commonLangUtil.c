
#include "language/commonLangUtil.h"
#include <utilitieslib/components/ArrayOld.h>
#include <utilitieslib/language/MessageStore.h>
#include <utilitieslib/language/MessageStoreUtil.h>
#include <utilitieslib/language/AppLocale.h>
#include <utilitieslib/utils/utils.h>
#include "entity/powers.h"
#include <utilitieslib/utils/scriptvars.h>
#include "entity/character_base.h"
#include "entity/character_level.h"
#include "entity/classes.h"

#if CLIENT | SERVER
    #if CLIENT
        #include <utilitieslib/language/MessageStoreUtil.h>
        #include "language/langClientUtil.h"
        #include "cmdparse/cmdgame.h"
    #elif SERVER
        #include "language/langServerUtil.h"
        #include "dbcomm/dbnamecache.h"
        #include "storyarc/storyarcprivate.h"
        #include "cmdparse/cmdserver.h"
    #endif

    #include "entity/entity.h"
    #include "entity/costume.h"
    #include <utilitieslib/components/EString.h>
    #include "entity/entPlayer.h"

    StaticDefineInt ParseGender[] = {
        DEFINE_INT
        { "undefined",                GENDER_UNDEFINED            },
        { "neuter",                    GENDER_NEUTER                },
        { "male",                    GENDER_MALE                    },
        { "female",                    GENDER_FEMALE                },
        DEFINE_END
    };

    #if SERVER
        StaticDefineInt ParseStatus[] = {
            DEFINE_INT
            { "undefined",                STATUS_UNDEFINED            },
            { "low",                    STATUS_LOW                    },
            { "medium",                    STATUS_MEDIUM                },
            { "high",                    STATUS_HIGH                    },
            DEFINE_END
        };
    #endif

    StaticDefineInt ParsePlayerType[] = {
        DEFINE_INT
        { "hero",                    kPlayerType_Hero            },
        { "villain",                kPlayerType_Villain            },
        DEFINE_END
    };

    StaticDefineInt ParsePlayerSubType[] = {
        DEFINE_INT
        { "normal",                    kPlayerSubType_Normal        },
        { "rogue",                    kPlayerSubType_Rogue        },
        { "paragon",                kPlayerSubType_Paragon        },
        DEFINE_END
    };

    StaticDefineInt ParseAlignment[] = {
        DEFINE_INT
        { "hero",                    ENT_ALIGNMENT_TYPE_HERO        },
        { "vigilante",                ENT_ALIGNMENT_TYPE_VIGILANTE},
        { "rogue",                    ENT_ALIGNMENT_TYPE_ROGUE    },
        { "villain",                ENT_ALIGNMENT_TYPE_VILLAIN    },
        DEFINE_END
    };

    StaticDefineInt ParsePraetorianProgress[] = {
        DEFINE_INT
        { "normal",                    kPraetorianProgress_PrimalBorn        },
        { "tutorial",                kPraetorianProgress_Tutorial        },
        { "praetoria",                kPraetorianProgress_Praetoria        },
        { "earth",                    kPraetorianProgress_PrimalEarth        },
        { "travelhero",                kPraetorianProgress_TravelHero        },
        { "travelvillain",            kPraetorianProgress_TravelVillain    },
        { "neutralInPrimalTutorial",    kPraetorianProgress_NeutralInPrimalTutorial    },
        DEFINE_END
    };

    StaticDefineInt ParseTeamNames[] = {
        DEFINE_INT
        { "none",            kAllyID_None        },
        { "hero",            kAllyID_Hero        },
        { "monster",        kAllyID_Monster        },
        { "villain",        kAllyID_Villain        },
        { "villain2",        kAllyID_Villain2    },
        DEFINE_END
    };

    StaticDefineInt ParseBodyType[] = {
        DEFINE_INT
        { "male",            kBodyType_Male        },
        { "female",            kBodyType_Female    },
        { "huge",            kBodyType_Huge        },
        DEFINE_END
    };
    
    typedef enum {
        ATTR_GENDER = 1,
        ATTR_NAMEGENDER,
        ATTR_TYPE,
        ATTR_ALIGNMENT,
        #if SERVER
            ATTR_CONTACTSTATUS
        #endif
    } Attribute;

    StaticDefineInt ParseAttributes[] = {
        DEFINE_INT
        { "Gender",                    ATTR_GENDER                    },
        { "NameGender",                ATTR_NAMEGENDER                },
        { "Type",                    ATTR_TYPE                    },
        { "Alignment",                ATTR_ALIGNMENT                },
        #if SERVER
            { "Status",                ATTR_CONTACTSTATUS            },
        #endif
        DEFINE_END
    };
    
    typedef struct ParseUserData {
        S32            gender;
        S32            nameGender;
        S32            type;
        S32            alignment;
        #if SERVER
            S32        contactStatus;
        #endif
    } ParseUserData;

    static StaticDefineInt* attributeToValue[]    = {
        NULL,
        ParseGender,
        ParseGender,
        ParsePlayerType,
        ParseAlignment,
        #if SERVER
            ParseStatus,
        #endif
    };

    static U32 valueOffsetIntoUserData[] = {
        0,
        offsetof(ParseUserData, gender),
        offsetof(ParseUserData, nameGender),
        offsetof(ParseUserData, type),
        offsetof(ParseUserData, alignment),
        #if SERVER
            offsetof(ParseUserData, contactStatus),
        #endif
    };
#endif

char* printLocalizedEnt(const char *msg, ...)
{
    #if CLIENT
        MessageStore* store = menuMessages;
    #else
        MessageStore* store = svrMenuMessages->localizedMessageStore[getCurrentLocale()];
    #endif

    static Array*        my_typedef;
    static char            buffer[256];
    
    if(!my_typedef){
       my_typedef = msCompileMessageType("{Hero, %E}");
    }
        
    VA_START(va, msg);
        msvaPrintfInternalEx(store, SAFESTR(buffer), msg, my_typedef, NULL, 0, 0, va);
    VA_END();

    return buffer;
}

char* printLocalizedEntFromEntLocale(const char *msg, ...)
{
    static Array* my_typedef;
    static char   buffer[128];

    VA_START(va, msg);
    #if CLIENT
        MessageStore* store = menuMessages;
    #else
        Entity* e = va_arg(va, Entity*);
        MessageStore* store = svrMenuMessages->localizedMessageStore[e->playerLocale];
    #endif
    
    if(!my_typedef){
        my_typedef = msCompileMessageType("{Hero, %E}");
    }
    
    msvaPrintfInternalEx(store, SAFESTR(buffer), msg, my_typedef, NULL, 0, 0, va);

    VA_END();

    return buffer;
}

static int formatHandlerC(MessageStoreFormatHandlerParams* params){
    ParseUserData* data = params->userData;

    #if CLIENT
        if(params->fileName){
            ErrorFilenamef(    params->fileName,
                            "Client trying to read contact %s, file %s, message %s, line %i",
                            params->srcstr,
                            params->fileName,
                            params->wholemsg,
                            params->lineCount);
        }else{
            Errorf("Client trying to read contact %s", params->srcstr);
        }
        if(params->outputBuffer){
            params->outputBuffer[0] = '\0';
        }
        return 0;
    #elif SERVER
        ContactDef* c = (ContactDef*)params->param;
        
        if(    !params->attribstr &&
            params->outputBuffer)
        {
            ScriptVarsTable vars = {0};
            ScriptVarsTablePushScope(&vars, &c->vs);
            strcpy(params->outputBuffer, saUtilTableLocalize(c->displayname, &vars));
        }
        else
        {
            data->gender = c->gender;
            data->nameGender = c->gender; // these are always gonna be the same... i think...
            data->contactStatus = c->status;
        }
    #endif
    
    return 1;
}

static int formatHandlerD(MessageStoreFormatHandlerParams* params){
    ParseUserData* data = params->userData;

    #if CLIENT
        if(params->fileName){
            ErrorFilenamef(    params->fileName,
                            "Client trying to read db_id %i in %s, file %s, message %s, line %i",
                            (int)params->param,
                            params->srcstr,
                            params->fileName,
                            params->wholemsg,
                            params->lineCount);
        }else{
            Errorf("Client trying to read db_id %i in %s", (int)params->param, params->srcstr);
        }
        if(params->outputBuffer){
            params->outputBuffer[0] = '\0';
        }
        return 0;
    #elif SERVER
        Gender        gender = GENDER_MALE;
        Gender        nameGender = GENDER_MALE;
        int            playerType = kPlayerType_Hero;
        int            playerSubType = ENT_ALIGNMENT_TYPE_HERO;
        const char* name = dbPlayerMessageVarsFromId((int)(intptr_t)params->param, &gender, &nameGender, &playerType, &playerSubType);
        
        data->gender = gender;
        data->nameGender = nameGender;
        data->type = playerType;
        if(playerType == kPlayerType_Villain)
        {
            if(playerSubType == kPlayerSubType_Rogue)
                data->alignment = ENT_ALIGNMENT_TYPE_ROGUE;
            else
                data->alignment = ENT_ALIGNMENT_TYPE_VILLAIN;
        }
        else
        {
            if(playerSubType == kPlayerSubType_Rogue)
                data->alignment = ENT_ALIGNMENT_TYPE_VIGILANTE;
            else
                data->alignment = ENT_ALIGNMENT_TYPE_HERO;
        }

        if(    !params->attribstr &&
            params->outputBuffer)
        {
            strcpy(params->outputBuffer,name);
        }

        // gend and namegend get filled in by dbPlayerNameAndGenderFromId anyway
    #endif
    
    return 1;
}

static int formatHandlerE(MessageStoreFormatHandlerParams* params){
    ParseUserData*    data = params->userData;
    Entity*            e = (Entity *)params->param;
    //check for attributes
    
    if (!params->attribstr &&
         params->outputBuffer)
    {
        if (!e) {
            Errorf("NULL entity in ParseAttrCond while parsing %s", params->srcstr);
            
            params->outputBuffer[0] = '\0';
            
            return 0;
        } else {
            strcpy(params->outputBuffer, e->name);
        }
    }
    else
    {
        if (!e) {
            data->gender = GENDER_MALE;
            data->nameGender = GENDER_MALE;
            data->type = kPlayerType_Hero;
            data->alignment = ENT_ALIGNMENT_TYPE_HERO;
        } else {
            if(    e->gender == GENDER_UNDEFINED ||
                e->name_gender == GENDER_UNDEFINED)
            {
                switch(e->costume->appearance.bodytype)
                {
                    xcase kBodyType_Male:
                    case kBodyType_BasicMale:
                    case kBodyType_Huge:
                    case kBodyType_Enemy:
                    case kBodyType_Villain:
                        e->gender = GENDER_MALE;
                        e->name_gender = GENDER_MALE;
                    xcase kBodyType_Female:
                    case kBodyType_BasicFemale:
                        e->gender = GENDER_FEMALE;
                        e->name_gender = GENDER_FEMALE;
                    xdefault:
                        Errorf("Entity at %p doesn't appear to have a valid bodytype, or this function needs updating", e);
                        e->gender = GENDER_MALE;
                        e->name_gender = GENDER_MALE;
                }
            }
            data->gender = e->gender;
            data->nameGender = e->name_gender;
            if(e->pl)
            {
                data->type = e->pl->playerType;
                data->alignment = getEntAlignment(e);
            }
            else
            {
                data->type    = kPlayerType_Hero;
                data->alignment    = ENT_ALIGNMENT_TYPE_HERO;
            }
        }
    }
    
    return 1;
}


static int formatHandlerI(MessageStoreFormatHandlerParams* params)
{
    ParseUserData* data = params->userData;
    int i = *(int *)&params->param;

    if(params->outputBuffer)
    {
        itoa_with_commas(i, params->outputBuffer);
    }

    return 1;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//  P O W E R S  &   B O O S T S
///////////////////////////////////////////////////////////////////////////////////////////////

static int formatHandlerP(MessageStoreFormatHandlerParams* params)
{
    char            vartype;
    int                retval = 1;
    BasePower*        pPower = (BasePower *)params->param;

    if (params->attribstr != NULL)
    {
        char *parse;
        char *attribCopy = (char *) strdup(params->attribstr);

        parse = attribCopy;

        // more to look at for this power
        parse = strtok(parse, ".");
        if (stricmp(parse, "attrib") == 0)
        {
            // it's an attribute of this power
            if (parse = strtok(NULL, "."))
            {
                const AttribModTemplate *pAttrib = basepower_GetAttribByName(pPower, parse);
                char *pName = parse;

                if (pAttrib)
                {
                    // more?
                    if (parse = strtok(NULL, "."))
                    {
                        if (stricmp(parse, "scale") == 0)
                        {
                            Entity*    pPlayer = (Entity *) ScriptVarsTableLookupTyped(params->vars, "Hero", &vartype);
                            int iLevel = atoi(ScriptVarsTableLookup(params->vars, "Level"));
                            int iNumCombines = atoi(ScriptVarsTableLookup(params->vars, "Combines"));

                            if (vartype == 's' || iLevel < 0)
                            {
                                strcpy(params->outputBuffer, textStd(pName));
                            } else {
                                int iCharLevel = character_CalcExperienceLevel(pPlayer->pchar);
                                float fEff;
                                float fMagnitude;
                                
                                if (pPower->bBoostIgnoreEffectiveness)
                                {        
                                    /*
                                    if (iCharLevel >= power_MinBoostUseLevel(pPower, iLevel) &&
                                        iCharLevel <= power_MaxBoostUseLevel(pPower, iLevel))
                                        fEff = 1.0f;
                                    else
                                        fEff = 0.0f;
                                    */
                                    if (pPower->bBoostBoostable && iNumCombines > 0 && pAttrib->offAttrib != kSpecialAttrib_Null)
                                        fEff =  boost_BoosterEffectivenessLevelByCombines(iNumCombines);
                                    else
                                        fEff =  1.0f;
                                } else {
                                    fEff = boost_EffectivenessLevel(iLevel, iCharLevel);
                                }
                                fMagnitude = pPlayer->pchar->pclass ? class_GetNamedTableValue(pPlayer->pchar->pclass, pAttrib->pchTable, iLevel) : 0;

                                sprintf(params->outputBuffer, "%3.1f", (fMagnitude * fEff * pAttrib->fScale) * 100.0f);
                            }
                        } else {
                            strcpy(params->outputBuffer, textStd(pName));
                        }
                    } else{
                        strcpy(params->outputBuffer, textStd(pName));
                    }
                } else {
                    // couldn't find named attrib
                    Errorf(    "Couldn't find attrib (%s) in power (%s)",
                        parse,
                        pPower->pchName);

                    retval = 0;
                }
            } else {
                // bare attrib, not very useful
                retval = 0;
            }
        } else {
            // don't know what this is...
            retval = 0;
        }
        free(attribCopy);
    } else {
        strcpy(params->outputBuffer, textStd(pPower->pchDisplayName));
    }

    if (retval)
        params->done = true;

    return retval;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//  F O R M A T    H A N D L E R 
///////////////////////////////////////////////////////////////////////////////////////////////

void installCustomMessageStoreHandlers(MessageStore* store)
{
    msSetFormatHandler(store, 'C', formatHandlerC);
    msSetFormatHandler(store, 'D', formatHandlerD);
    msSetFormatHandler(store, 'E', formatHandlerE);
    msSetFormatHandler(store, 'P', formatHandlerP);
    msSetFormatHandler(store, 'I', formatHandlerI);
    
    msSetAttributeTables(store, ParseAttributes, attributeToValue, valueOffsetIntoUserData);
}

