#include "gameComm/wdwbase.h"
#include <utilitieslib/utils/utils.h>
#include "comm_game.h"
#include <utilitieslib/components/EString.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/sysutil.h>

#include "graphics/textureatlas.h"
#include "win/input.h"
#include "cmdparse/cmdgame.h"
#include "UI/uiWindows.h"
#include "UI/uiBox.h"
#include "UI/uiUtil.h"
#include "UI/uiGame.h"
#include "UI/uiUtilGame.h"
#include "UI/uiFocus.h"
#include "UI/uiScrollBar.h"
#include "UI/uiTabControl.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiInput.h"
#include "UIEdit.h"
#include "UI/uiPetition.h"
#include "UI/uiClipper.h"
#include "UI/uiOptions.h"
#include "UI/uiChat.h"
#include "UI/uiNet.h"
#include "UI/uiTarget.h"
#include "UI/uiLogin.h"

#include "entity/entVarUpdate.h"
#include "UI/sprite/sprite_font.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_base.h"
#include <utilitieslib/components/EString.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/language/MessageStoreUtil.h>

#include "formatter/smf_main.h"
#include "entity/entclient.h"
#include "graphics/ttFontUtil.h"


static TextAttribs s_taChatHistory =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&smfSmall,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0x80e080ff,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0x66ff66ff,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)1,
    /* piShadow      */  (int *)0,
};

typedef struct PlayerNote
{
    int rating;  // 0-5
    int bDirty;

    char * pchName;        // global player name
    char * pchNote;        // players description of player

    char * pchChatLog;    // Formatted chat history
    char * pchAlias;    // Formatted alias list

    char **ppChatLog;    // history of chat messages
    char **ppLocalNames; // List of aliases
}PlayerNote;

StashTable st_PlayerNotes;
StashTable st_GlobalNames;

static UIEdit *s_peditNote;
static PlayerNote * g_pCurrentNote;
static PlayerNote g_Placeholder;
typedef struct MessageCacheEntry
{
    char *pchName;
    char *pchText;
} MessageCacheEntry;

MessageCacheEntry **messageCache = NULL;

static void playerNote_cacheMessage(char *pchName, char *pchText)
{
    MessageCacheEntry *entry = malloc(sizeof(MessageCacheEntry));
    assert(entry != NULL);
    entry->pchName = strdup(pchName);
    entry->pchText = strdup(pchText);
    assert(entry->pchName != NULL && entry->pchText != NULL);

    if (messageCache == NULL)
    {
        eaCreate(&messageCache);
    }
    eaPush(&messageCache, entry);
}

void playerNote_addGlobalName( char * localName, char * globalName )
{
    int i;
    PlayerNote *pNote = NULL;
    FILE *file = NULL;
    char buf[2048];
    char filename[MAX_PATH];

    if( st_GlobalNames && !stashFindPointer(st_GlobalNames, localName, 0) ) 
        stashAddPointer(st_GlobalNames, localName, strdup(globalName), 0 );

    for (i = eaSize(&messageCache) - 1; i >= 0; i--)
    {
        MessageCacheEntry *entry = messageCache[i];
        if (strcmpi(entry->pchName, localName) == 0)
        {
            if (pNote == NULL)
            {
                // Growl - this should not be necessary - why is uiPlayerNote.h not already included?
                extern PlayerNote *playerNote_Get(char *pchName, int alloc);
                pNote = playerNote_Get(globalName, 0);
            }
            if (pNote != NULL)
            {
                eaPush(&pNote->ppChatLog, strdup(entry->pchText));
                pNote->bDirty = 1;
            }

            if (file == NULL)
            {
                sprintf(buf, "%s/%s private chatlog.txt", getAccountDir(filename), globalName);
                file = fopen(buf, "a+t" );

                if (file == NULL)
                {
                    // make sure the directory exists and try again
                    makeDirectoriesForFile(buf);
                    file = fopen(buf, "a+t" );
                    if (file == NULL)
                    {
                        optionSet(kUO_LogPrivateMessages,0,0);
                        addSystemChatMsg( textStd("CouldNotOpenPrivateChatLog"), INFO_USER_ERROR, 0 );
                    }
                }
            }
            
            if (file && fwrite(entry->pchText, 1, strlen(entry->pchText), file) <= 0)
            {
                optionSet(kUO_LogPrivateMessages,0,0);
                addSystemChatMsg( textStd("ChatLogHardDiskFull"), INFO_USER_ERROR, 0 );
            }
            eaRemove(&messageCache, i);
            // Not optimal, but I really don't care.
            // Don't subtract 1 here, the loop update will take care of it for us.
            i = eaSize(&messageCache);
        }
    }
    if (file != NULL)
    {
        fclose(file);
    }
}

char * playerNote_getGlobalName( char * localName )
{
    char * globalName;
    if( st_GlobalNames && stashFindPointer(st_GlobalNames, localName, &globalName) ) 
        return globalName;
    return NULL;
}

static int playerNote_Write(FILE *file, StashElement element)
{
    PlayerNote *pNote = (PlayerNote*)stashElementGetPointer(element);
    char * alias;
    int i;

    estrCreate(&alias);
    for( i = 0; i < eaSize(&pNote->ppLocalNames); i++ )
    {
//        estrConcatChar(&alias, ' ');
        estrConcatCharString(&alias, pNote->ppLocalNames[i] );
        if( i < eaSize(&pNote->ppLocalNames)-1)
            estrConcatChar(&alias, ',');
    }

    if ( pNote && file )
        fprintf(file, "\"%s\" \"%i\" \"%s\" \"%s\"\n", pNote->pchName, pNote->rating, alias, escapeString(pNote->pchNote)  );
    return 1;
}

// Save File
void playerNote_Save()
{
    FILE *file;
    char filename[FILENAME_MAX];
    char buff[MAX_PATH];

    if( g_pCurrentNote )
    {
        SAFE_FREE(g_pCurrentNote->pchNote);
        g_pCurrentNote->pchNote = strdup(uiEditGetUTF8Text(s_peditNote));
    }

    if(isDevelopmentMode())
        strcpy(filename, "C:/playernotes.txt");
    else
        sprintf(filename, "%s/playernotes.txt", getAccountDir(buff) );

    file = fileOpen( filename, "wt" );
    if(!file)
    {
        makeDirectoriesForFile(filename);
        file = fileOpen( filename, "wt" );
    }
    if (!file || !st_PlayerNotes )
        return;
    
    stashForEachElementEx( st_PlayerNotes, playerNote_Write, file );
    fclose(file);
}

// Load File
void playerNote_Load()
{
    FILE *file;
    char buf[1500];
    char filename[FILENAME_MAX];

    if(!st_PlayerNotes)
    {
        st_PlayerNotes = stashTableCreateWithStringKeys( 24, StashDeepCopyKeys);
        st_GlobalNames = stashTableCreateWithStringKeys( 24, StashDeepCopyKeys);
    }

    if(isDevelopmentMode())
        strcpy(filename, "C:/playernotes.txt");
    else
        sprintf(filename, "%s/playernotes.txt", getAccountDir(buf) );

    file = fileOpen( filename, "rt" );
    if(!file)
    {
        makeDirectoriesForFile(filename);
        file = fileOpen( filename, "rt" );
    }
    if(!file)
        return;

    stashTableClear(st_PlayerNotes);

    while(fgets(buf,sizeof(buf),file))
    {
        PlayerNote * pNote = calloc(1,sizeof(PlayerNote)); // TODO: mempool this
        char *args[4];
        int count = tokenize_line_safe(buf, args, 4, 0);

        if( count != 4 )
            addSystemChatMsg( textStd("InvalidPlayerNote"), INFO_USER_ERROR, 0 );
        else
        {
            char * alias_args[100];
            int i, alias_count;
            pNote->pchName = strdup(args[0]);
            pNote->rating = atoi(args[1]);

            // Alias List
            alias_count = tokenize_line_quoted_delim(args[2], alias_args, 100, 0, ",", ",");
            for( i=0; i < alias_count; i++ )
            {
                eaPush(&pNote->ppLocalNames, strdup(alias_args[i]));
                playerNote_addGlobalName( alias_args[i], pNote->pchName );
            }

            // Actual Message
            pNote->pchNote = strdup(unescapeString(args[3]));

            // Import chat history
            {
                char subfile[FILENAME_MAX];
                char sub_buf[1000];
                FILE *sub;
                sprintf( subfile, "%s/%s private chatlog.txt", getAccountDir(sub_buf), pNote->pchName );
                sub = fileOpen(subfile, "rt" );
                if( sub )
                {
                    while(fgets(sub_buf,sizeof(sub_buf),sub))
                        eaPush(&pNote->ppChatLog, strdup(sub_buf));
                    fclose(sub);
                }
            }

            stashAddPointer(st_PlayerNotes, pNote->pchName, pNote, 0 );
        }
    }
    fclose(file);
}

// Get Note from playerName
PlayerNote * playerNote_Get( char * pchName, int alloc)
{
    PlayerNote * pNote = 0;
    if(!pchName)
        return 0;

    stashFindPointer(st_PlayerNotes,pchName,&pNote);
    if(!pNote && alloc)
    {
        pNote = calloc(1,sizeof(PlayerNote));
        pNote->pchName = strdup(pchName);
        stashAddPointer(st_PlayerNotes, pNote->pchName, pNote, 0 );
    }
    return pNote;
}


void playerNote_LogPrivate( char * pchName, char * pchMessage, int clear, int globalName, int arrow )
{
    PlayerNote * pNote;
    FILE * file = 0;
    char buf[2048];
    char filename[MAX_PATH];
    char date_buf[64];
    char *pchGlobal = 0;

    if( !optionGet(kUO_LogPrivateMessages) )
        return;

    if(!globalName)
    {
        pchGlobal = playerNote_getGlobalName(pchName);
        if( !pchGlobal )
            pchGlobal = handleFromNameSlow(pchName);
        if( pchGlobal && *pchGlobal )
            globalName = 1;
        else
            pchGlobal = pchName;
    }

    pNote = playerNote_Get(pchGlobal, globalName);
    if(!pNote)
    {
        timerMakeDateString(date_buf);
        sprintf(buf, "%s %s%s: %s\n", date_buf, arrow ? "-->" : "", pchName, pchMessage);
        playerNote_cacheMessage(pchName, buf);
        cmdParsef( "get_global_silent %s", pchGlobal ); // we'll lose a message or two until we get the global name
        return;
    }

    sprintf( buf, "%s/%s private chatlog.txt", getAccountDir(filename), pchGlobal );

    if( clear )
    {
        file = fopen(buf, "wt" );
        if( file )
            fclose(file);
        return;
    }
    else
        file = fopen(buf, "a+t" );

    if( !file )
    {
        // make sure the directory exists and try again
        makeDirectoriesForFile(buf);
        file = fopen(buf, "a+t" );
        if ( !file )
        {
            optionSet(kUO_LogPrivateMessages,0,0);
            addSystemChatMsg( textStd("CouldNotOpenPrivateChatLog"), INFO_USER_ERROR, 0 );
            return;
        }
    }

    timerMakeDateString(date_buf);
    if(arrow)
        sprintf( buf, "%s -->%s: %s\n", date_buf, pchName, pchMessage );
    else
        sprintf( buf, "%s %s: %s\n", date_buf, pchName, pchMessage );
    eaPush( &pNote->ppChatLog, strdup(buf) );
    pNote->bDirty = 1;
    if (fwrite( buf, 1, strlen(buf), file ) <= 0)
    {
        optionSet(kUO_LogPrivateMessages,0,0);
        addSystemChatMsg( textStd("ChatLogHardDiskFull"), INFO_USER_ERROR, 0 );
    }

    fclose(file);
}

void playerNote_GetWindow( char * pchName, int global )
{
    char * globalName;
    
    // Open window
    window_setMode(WDW_PLAYERNOTE, WINDOW_GROWING);

    // deal with local/global nonsense
    if( global )
        globalName = pchName;
    else
        globalName = playerNote_getGlobalName( pchName );

    if( !globalName )
    {
        g_pCurrentNote = &g_Placeholder;
        SAFE_FREE(g_Placeholder.pchName);
        g_Placeholder.pchName = strdup(pchName);
        cmdParsef( "get_global_name %s", pchName );
        return;
    }
    else
        playerNote_Save();

    g_pCurrentNote = playerNote_Get( globalName, 1 );

    // Add alias if we have not already
    if(!global)
    {
        int i, found = 0;
        for( i = 0; i < eaSize(&g_pCurrentNote->ppLocalNames); i++)
        {
            if( strcmp( g_pCurrentNote->ppLocalNames[i], pchName )==0)
            {
                found = 1;
                break;
            }
        }
        if(!found)
        {
            eaPush(&g_pCurrentNote->ppLocalNames,strdup(pchName));
            if(g_pCurrentNote->pchAlias)
                estrDestroy(&g_pCurrentNote->pchAlias);
        }

    }

    // init text boxes
    if( !s_peditNote )
        s_peditNote = uiEditCreateSimple(NULL, 2048, &game_12, CLR_WHITE, NULL);

    if( g_pCurrentNote->pchNote )
        uiEditSetUTF8Text(s_peditNote, g_pCurrentNote->pchNote);
    else
        uiEditClear(s_peditNote);
}

void playerNote_TargetEdit(void *foo)
{
    char* targetName = targetGetName();
    if(!targetName)
    {
        addSystemChatMsg( textStd("NoTargetError"), INFO_USER_ERROR, 0 );
        return;
    }
    playerNote_GetWindow( targetName, 0 );
}

char * playerNote_TargetEditName(void *foo)
{
    char* targetName = targetGetName();
    char* globalName;

    if(!targetName)
        return textStd("NoTargetError");

    globalName = playerNote_getGlobalName(targetName);
    if( globalName && playerNote_Get(globalName,0) )
        return textStd( "CMEditNoteString" );
    else
        return textStd( "CMAddNoteString" );
}

int playerNote_GetRating( char * localName )
{
    char * globalName = playerNote_getGlobalName(localName);
    PlayerNote * pNote = playerNote_Get(globalName,0);
    if( pNote )
        return pNote->rating;
    else
        return 0;
}

#define LINE_HT 15

int drawRatingStar( int idx, F32 x, F32 y, F32 z, F32 sc )
{
    AtlasTex * star = atlasLoadTexture( "MissionPicker_icon_star_16x16.tga");
    CBox box;
      int color = 0xffffff44;

    if( g_pCurrentNote->rating >= idx+1 )
            color = CLR_WHITE;

     BuildCBox( &box, x+star->width*sc*idx*1.1f, y, star->width*sc*1.1f, star->height*sc*1.1f );
    if( mouseCollision(&box))
           display_sprite(star, x+star->width*sc*idx*1.1f - star->width*.1f*sc, y - star->width*.1f*sc, z+1, sc*1.2f, sc*1.2f, CLR_GREEN );

    display_sprite(star, x+star->width*sc*idx*1.1f, y, z, sc, sc, color );

    if( mouseClickHit(&box, MS_LEFT) )
    {
        if( idx == 0 && g_pCurrentNote->rating == 1 ) // special case to zero rating
            g_pCurrentNote->rating = 0;
        else
            g_pCurrentNote->rating = idx+1;

        return 1;
    }

    return 0;
}

int playerNoteWindow()
{
    F32 x, y, z, wd, ht, sc;
    int i, color, bcolor;
    static uiTabControl * tc;
    static ScrollBar sb = { WDW_PLAYERNOTE };
    static int save;
    int * showPrivate = NULL;
    
    int text_ht;
    UIBox box;
    

    if(!window_getDims(WDW_PLAYERNOTE, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor ) || !g_pCurrentNote )
    {
        if(s_peditNote)
        {
            uiEditReturnFocus(s_peditNote);
            if( save )
                playerNote_Save(); 
            save = 0;
        }
        return 0;
    }

    if(!tc)
    {
        tc = uiTabControlCreate(TabType_Undraggable, 0, 0, 0, 0, 0 );
        uiTabControlAdd(tc, "PlayerInfo", (int*)0 );
        uiTabControlAdd(tc, "PrivateHistory", (int*)1 );
    }

    drawFrame( PIX3, R10, x, y, z, wd, ht, sc, color, bcolor );

    font(&game_12);
    font_color(CLR_WHITE, CLR_WHITE);
    if( g_pCurrentNote == &g_Placeholder )
    {
        static float timer = 0;
        char * globalName = playerNote_getGlobalName(g_Placeholder.pchName);
        timer += TIMESTEP; 

        cprntEx( x+wd/2, y+ht/2, z, sc, sc, CENTER_X|CENTER_Y, "FetchingNoteInfo" );
        if( globalName )
            playerNote_GetWindow( globalName, 1 );

        if( timer > 5*30 ) // wait 5 seconds then forge note anyways
        {
            playerNote_GetWindow( g_Placeholder.pchName, 1 );
            timer = 0;
        }

        return 0;
    }

     if( window_getMode(WDW_PLAYERNOTE) == WINDOW_DISPLAYING ) 
        showPrivate = drawTabControl(tc, x+10*sc, y, z, wd - 20*sc, 15, sc, color, color, TabDirection_Horizontal);

    font(&game_12);
    font_color(CLR_WHITE, CLR_WHITE);
    if( !showPrivate )
    {
        static SMFBlock *pBlock = 0;

        if (!pBlock)
        {
            pBlock = smfBlock_Create();
        }

        // Global Name Entry Text Box
         cprntEx( x+10*sc, y+40*sc, z, sc, sc,0, textStd("PlayerNoteGlobalName", g_pCurrentNote->pchName) );

        // Rating
        cprntEx( x+10*sc, y+60*sc, z, sc, sc,0, "PlayerNoteRating" );
        for(i=0; i<5; i++)
               save |= drawRatingStar( i, x + (20+str_wd(&game_12,sc,sc,"PlayerNoteRating"))*sc, y + 45*sc, z, sc );

        // Local names
        if( !g_pCurrentNote->pchAlias )
        {
            estrCreate(&g_pCurrentNote->pchAlias);
            estrConcatf(&g_pCurrentNote->pchAlias, "%s ", textStd("PlayerNoteLocalName"));
            for( i = 0; g_pCurrentNote->ppLocalNames && i < eaSize(&g_pCurrentNote->ppLocalNames); i++ )
            {
                estrConcatCharString(&g_pCurrentNote->pchAlias, g_pCurrentNote->ppLocalNames[i] );
                if( i < eaSize(&g_pCurrentNote->ppLocalNames)-1)
                    estrConcatStaticCharArray(&g_pCurrentNote->pchAlias, ", ");
            }
            smf_ParseAndFormat( pBlock, g_pCurrentNote->pchAlias, 0, 0, 0, wd, 0, false, 1, 1, &s_taChatHistory, 0 );
        }
        s_taChatHistory.piScale = (int *)((int)(sc*SMF_FONT_SCALE));
        text_ht = smf_ParseAndDisplay( pBlock, g_pCurrentNote->pchAlias, x+10*sc, y+65*sc, z, wd, 0, 0, 0, &s_taChatHistory, NULL, 0, true);

        // Large description box
        cprntEx( x+10*sc, y+(80+text_ht)*sc, z, sc, sc, 0, "PlayerNoteDescription" );
         if( TEXTAREA_WANTS_FOCUS == textarea_Edit(s_peditNote, x+sc*15, y+(text_ht+85)*sc, z, wd-sc*15*2, ht-(95+text_ht)*sc, sc))
            uiEditTakeFocus(s_peditNote);

        save = 1;
    }
    else
    {
        static SMFBlock *pBlock = 0;

        if (!pBlock)
        {
            pBlock = smfBlock_Create();
        }

        uiBoxDefine(&box, x+PIX3*sc, y+20*sc, wd - PIX3*2*sc, ht - 50*sc );
        clipperPush(&box);

        if( g_pCurrentNote->bDirty || !g_pCurrentNote->pchChatLog )
        {
            if( g_pCurrentNote->pchChatLog )
                estrClear(&g_pCurrentNote->pchChatLog);
            else
                estrCreate(&g_pCurrentNote->pchChatLog);

            if( eaSize(&g_pCurrentNote->ppChatLog) == 0 )
                estrConcatCharString(&g_pCurrentNote->pchChatLog, textStd("NoPrivateMessages"));
            else
            {
                for( i=0; i < eaSize(&g_pCurrentNote->ppChatLog); i++ )
                    estrConcatf(&g_pCurrentNote->pchChatLog, "%s<br>", g_pCurrentNote->ppChatLog[i] );
            }
        }

        s_taChatHistory.piScale = (int *)((int)(sc*SMF_FONT_SCALE));
        text_ht = smf_ParseAndDisplay( pBlock, g_pCurrentNote->pchChatLog, x+10*sc, y+20*sc-sb.offset, z, wd, 0, g_pCurrentNote->bDirty, g_pCurrentNote->bDirty, &s_taChatHistory, NULL, 0, true);
        g_pCurrentNote->bDirty = 0;
        clipperPop(&box);
        
         doScrollBar(&sb, ht - 50*sc, text_ht, x+wd, 15*sc, z, 0, &box );

        if( D_MOUSEHIT == drawStdButton( x+102*sc, y+ht-15*sc, z, 185*sc, 20*sc, color, "ClearChatlog", 1.f, !eaSize(&g_pCurrentNote->ppChatLog)) )
        {
            eaDestroyEx(&g_pCurrentNote->ppChatLog,NULL);
            estrDestroy(&g_pCurrentNote->pchChatLog);
            g_pCurrentNote->bDirty = 1;
            playerNote_LogPrivate(g_pCurrentNote->pchName, 0, 1, 1, 0);
        }

        if( D_MOUSEHIT == drawStdButton( x+wd-102*sc, y+ht-15*sc, z, 185*sc, 20*sc, color, optionGet(kUO_LogPrivateMessages)?"DisableAllPrivateLogging":"EnableAllPrivateLogging", 1.f, 0) )
        {
            optionToggle(kUO_LogPrivateMessages, 1);
        }
    }

    return 0;
}
