#include <utilitieslib/language/AppLocale.h>
#include "entity/entPlayer.h"
#include "player/player.h"
#include "entity/character_base.h"
#include "entity/entity.h"
#include "entity/entVarUpdate.h"

#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/language/MessageStoreUtil.h>
#include <utilitieslib/components/StashTable.h>
#include "player/pophelp.h"
#include "clientError.h"
#include "entity/LoadDefCommon.h"
#ifdef SERVER
#include "svr/svr_base.h"
#include "comm_game.h"
#endif

#define POP_HELP_END MAX_POP_HELP

// Data structures
//---------------------------------------------------------------------
typedef struct PopHelpDictionary
{
    int init;
    PopHelpItem **items;
} PopHelpDictionary;

// Parse structures
//---------------------------------------------------------------------

TokenizerParseInfo ParsePopHelp[] =
{
    { "{",                TOK_START,        0                            },
    { "Tag",            TOK_STRING(PopHelpItem, tag, 0)                },
    { "InYourFace",        TOK_BOOLFLAG(PopHelpItem, inYourFace, 0)    },
    { "DisplayTitle",    TOK_STRING(PopHelpItem, name, 0)            },
    { "DisplayText",    TOK_STRING(PopHelpItem, text, 0)            },
    { "SoundName",        TOK_STRING(PopHelpItem, soundName, 0)        },
    { "TimeTriggered",    TOK_INT64(PopHelpItem, timeTriggered, 0)    },
    { "}",                TOK_END,        0                            },
    { "", 0, 0 }
};

TokenizerParseInfo ParsePopHelpDictionary[] =
{
    { "PopHelp", TOK_STRUCT(PopHelpDictionary, items, ParsePopHelp) },
    { "", 0, 0 }
};

PopHelpDictionary gPopHelpDictionary = {0};

static StashTable g_popHelpTable;

PopHelpItem *pop_help_index[POP_HELP_END];
int g_num_pop_help;

static int loaded_pop_help = 0;

void loadPopHelp(char *def_filename, char *attribute_filename)
{
    StashElement element;
    PopHelpItem *phi;
    unsigned int v;
    int j;
    char *tag_name;
    const char* bin_filename;

    if (loaded_pop_help)
        return;
    loaded_pop_help = 1;

    bin_filename = MakeBinFilename(def_filename);
    if (!ParserLoadFiles(NULL, def_filename, bin_filename, 0, ParsePopHelpDictionary, &gPopHelpDictionary, NULL, NULL, NULL))
    {
        devassert(printf("Couldn't load pop-help.\n"));
        return;
    } 

    g_popHelpTable = stashTableCreateWithStringKeys(MAX_POP_HELP, StashDeepCopyKeys);
    g_num_pop_help = badge_LoadNames(g_popHelpTable, attribute_filename);

    for( j = 0; j < eaSize(&gPopHelpDictionary.items); j++)
    {
        phi = gPopHelpDictionary.items[j];
        tag_name = phi->name;
        if (phi->tag)
            tag_name = phi->tag;

        if (tag_name)
        {
            if (stashFindElement(g_popHelpTable,tag_name,&element))
            {
                v = stashElementGetInt(element);
                if (v>=0 && v < MAX_POP_HELP)
                {
                    pop_help_index[v] = phi;
                }
            } else {
                // error?
                g_num_pop_help++;
                stashAddInt(g_popHelpTable, tag_name, g_num_pop_help, false);
                pop_help_index[g_num_pop_help] = phi;
            }
        }
    }
}


PopHelpItem *getPopHelpItem(int event)
{
    if (!gPopHelpDictionary.items || event < 0 || event >= MAX_POP_HELP )
        return NULL;
    return pop_help_index[event];
}

int getPopHelpEvent(char *tag)
{
    StashElement element;

    if (stashFindElement(g_popHelpTable, tag, &element))
        return stashElementGetInt(element);

    return -1;
}


//
//
void reloadPopHelp(void)
{
    loaded_pop_help = 0;
    loadPopHelp("defs/PopHelp.def", "server/db/templates/pophelp.attribute");
}

int loadedPopHelp(void)
{
    return loaded_pop_help;
}

char *popHelpTags(void)
{
    StuffBuff sb = {0};
    int i;
    PopHelpItem *phi;

    if( verify( loadedPopHelp() ))
    {
        initStuffBuff(&sb, 256);

        for(i=1; i<=g_num_pop_help; i++)
        {
            phi = pop_help_index[i];
            if (phi)
                addStringToStuffBuff(&sb, "%s\n", phi->tag);
        }
    }

    // This function is only called in -template mode, so we're letting
    //    this allocation dangle. (Same thing is done for attribs.)
    return sb.buff;
}

PopHelpState getPopHelpState(Entity *e, unsigned int n)
{
    int i, shift;

    if (!e || !e->pl)
        return -1;

    if (n >= MAX_POP_HELP)
        return -1;

    shift = (n & (32 / BITS_PER_POP_HELP - 1)) * BITS_PER_POP_HELP;
    i = n / (32 / BITS_PER_POP_HELP);

    return (e->pl->popHelpStatus[i] >> shift) & ((1 << BITS_PER_POP_HELP) - 1);
}

PopHelpState getPopHelpStateByTag(Entity *e, char *tag)
{
    return getPopHelpState(e, getPopHelpEvent(tag));
}

void setPopHelpState(Entity *e, unsigned int n, PopHelpState new_state)
{
    int i, shift, mask;
    U32 bv;

    if (!e || !e->pl)
        return;

    if (n >= MAX_POP_HELP || new_state > ((1 << BITS_PER_POP_HELP) - 1))
        return;

    shift = (n & (32 / BITS_PER_POP_HELP - 1)) * BITS_PER_POP_HELP;
    i = n / (32 / BITS_PER_POP_HELP);
    mask = (((1 << BITS_PER_POP_HELP) - 1) << shift) ^ 0xffffffff;
    bv = new_state << shift;

    e->pl->popHelpStatus[i] = (e->pl->popHelpStatus[i] & mask) | bv;
}

#ifdef SERVER
void triggerPopHelpEventHappenedByTag(Entity *e, const char *tag)
{
    START_PACKET(pak, e, SERVER_POPHELP_EVENT_HAPPENED_BY_TAG);
    pktSendString(pak, tag);
    END_PACKET
}
#endif