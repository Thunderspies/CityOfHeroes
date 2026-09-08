#include "graphics/font.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include "win/input.h"
#include "graphics/gfx.h"
#include <utilitieslib/utils/memcheck.h>
#include "win/win_init.h"
#include "UI/uiConsole.h"
#include "UI/uiChat.h"
#include "UI/uiFocus.h"
#include "render/renderUtil.h"
#include <utilitieslib/components/MRUList.h>
#include <utilitieslib/version/AppRegCache.h>
#include "render/renderprim.h"
#include <utilitieslib/utils/mathutil.h>
#include "demo.h"
#include "UI/uiCursor.h"
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/sysutil.h>

int show_console=0;

#define MAX_LINES 400
#define MAX_HIST_LINES 32
#define LINE_SIZE 256

typedef struct
{
    char    lines[MAX_LINES][LINE_SIZE];
    int        oldest_line,newest_line;
    int        hist_line;
    char    last_input[LINE_SIZE];
    char    input_buf[LINE_SIZE];
    char    tab_comp_buf[LINE_SIZE];
    int        input_pos;
    int        h_chars,v_chars;
    int        scroll_up;
    int        cursor_anim;
    MRUList *mruHist;
} Console;

Console    *curr_con;

static int conEnterMode(int enter);

MRUList *conGetMRUList(void)
{
    return curr_con->mruHist;
}


void conCreate()
{
    curr_con = calloc(1,sizeof(*curr_con));
    curr_con->mruHist = createMRUList(regGetAppName(), "Console", MAX_HIST_LINES, LINE_SIZE);
    curr_con->h_chars = 80;
    curr_con->v_chars = 30;
    curr_con->hist_line = curr_con->mruHist->count;
}

static int lineDist(int old,int curr)
{
int        dist;

    dist = curr - old;
    if (dist < 0)
        dist += MAX_LINES;

    return dist;
}

static int addLine(int line,int amt)
{
    line += amt;
    if (line >= MAX_LINES)
        line -= MAX_LINES;
    return line;
}

static int subLine(int line,int amt)
{
    line -= amt;
    if (line < 0)
        line += MAX_LINES;
    return line;
}

#include "cmdparse/cmdgame.h"
void conPrint(char *s)
{
    if (!curr_con)
        return;
    if (lineDist(curr_con->oldest_line,curr_con->newest_line) >= MAX_LINES -1)
        curr_con->oldest_line = addLine(curr_con->oldest_line,1);
    strncpy(curr_con->lines[curr_con->newest_line],s,LINE_SIZE-1);
    curr_con->lines[curr_con->newest_line][LINE_SIZE-1] = 0;
    curr_con->newest_line = addLine(curr_con->newest_line,1);
}

void conPrintf(char const *fmt, ...)
{
    char str[20000],*s,*s2;
    va_list ap;

    va_start(ap, fmt);
    _vsnprintf(str, sizeof(str), fmt, ap);
    va_end(ap);
    str[ARRAY_SIZE(str)-1]='\0';
    for(s2=str;;)
    {
        s = strchr(s2,'\n');
        if (s)
        {
            *s = 0;
            if (s[1] == 0)
                s = 0;
        }
        if (!conVisible())
        {
            if (!s2[0])
                s2 = ".";
            InfoBoxDbg( INFO_DEBUG_INFO, s2 );
        }
        else
            conPrint(s2);
        if (!s)
            break;
        s2 = s + 1;
    }
}

void conPrintfUpdate(char const *fmt, ...)
{
char str[100];
va_list ap;

    va_start(ap, fmt);
    _vsnprintf(str, sizeof(str), fmt, ap);
    str[ARRAY_SIZE(str)-1]='\0';
    va_end(ap);
    conPrint(str);
}

void conPrintfVerbose(char const *fmt, ...)
{
char str[100];
va_list ap;

    if (!game_state.verbose)
        return;
    va_start(ap, fmt);
    _vsnprintf(str, sizeof(str), fmt, ap);
    str[ARRAY_SIZE(str)-1]='\0';
    va_end(ap);
    conPrint(str);
}

void conResizeCheck()
{
    int x, y;
    int w, h, consoleh;
    static bool dragging=false;
    if (!show_console) {
        dragging=false;
        return;
    }
    windowSize(&w,&h);
    consoleh = h*2/3;
    if (game_state.console_scale > 0 && game_state.console_scale < 1)
        consoleh *= game_state.console_scale;
    inpMousePos(&x, &y);
    if (ABS(y-consoleh) < 6) {
        if (inpLevel(INP_LBUTTON))
            dragging=true;
        if (!dragging)
            setCursor( "cursor_win_vertresize.tga", NULL, FALSE, 10, 15 );
    }
    if (!inpLevel(INP_LBUTTON))
        dragging = false;
    if (dragging) {
        game_state.console_scale = y * 3.f / 2.f / h;
    }
}


int conGetCurCommandStart()
{
#define CHECK_FOR_TWO_CHARACTERS(c) (curr_con->input_buf[cmd_start_pos-1] == (c) && cmd_start_pos >= 1 && curr_con->input_buf[cmd_start_pos-2] == (c))
#define THERE_WERE_TWO_CHARACTERS(c) (curr_con->input_buf[cmd_start_pos] == (c) && curr_con->input_buf[cmd_start_pos-1] == (c))

    int cmd_start_pos = strlen(curr_con->input_buf);
    while ( cmd_start_pos && curr_con->input_buf[cmd_start_pos-1] != ' ' &&
        curr_con->input_buf[cmd_start_pos-1] != '\t' &&
        curr_con->input_buf[cmd_start_pos-1] != '+' &&
        !CHECK_FOR_TWO_CHARACTERS('$') &&
        !CHECK_FOR_TWO_CHARACTERS('-') && 
        !CHECK_FOR_TWO_CHARACTERS('+') )
    {
        --cmd_start_pos;
    }

    if (THERE_WERE_TWO_CHARACTERS('$') || THERE_WERE_TWO_CHARACTERS('-') || THERE_WERE_TWO_CHARACTERS('+') )
        ++cmd_start_pos;

    return cmd_start_pos;
}


void conRender()
{
    int        i,line,vis_lines,show_cursor,drawto_line,max_scroll,w,h;
    char    buf[LINE_SIZE+20];

    windowSize(&w,&h);

    //game_state.console_scale = 0.2; // for developers
    if (game_state.console_scale > 0 && game_state.console_scale < 1)
        h *= game_state.console_scale;

    curr_con->h_chars = w / 8;
    curr_con->v_chars = (h / 10) * 2 / 3;

    if (inpLevel(INP_PRIOR)) 
        curr_con->scroll_up++;
    if (inpLevel(INP_NEXT))
        curr_con->scroll_up--;
    if (inpLevel(INP_HOME))
        curr_con->scroll_up = MAX_LINES;
    if (inpLevel(INP_END))
        curr_con->scroll_up = 0;
    {
        int x,y;
        inpMousePos(&x,&y);
        if (y <= h*2/3)
        {
            curr_con->scroll_up += inpLevel(INP_MOUSEWHEEL) * 3;
            game_state.scrolled = 1;
        }
    }
    if( inpEdge(INP_C) && inpLevel(INP_CONTROL) )
    {
        char * consoleStr;
        estrCreate(&consoleStr);
        for( i = curr_con->oldest_line; i!=curr_con->newest_line; i=addLine(i,1) )
            estrConcatf( &consoleStr, "%s\n", curr_con->lines[i] );
        winCopyToClipboard(consoleStr);
        estrDestroy(&consoleStr);
    }

    fontSet(0);
    fontScale((curr_con->h_chars) * 22,(curr_con->v_chars) * 10);
    fontColor(0x3f3f3f);
    fontAlpha(100);
    fontText(0,0,"\001");
    fontAlpha(255);
    fontDefaults();

    vis_lines = curr_con->v_chars - 7;
    max_scroll = subLine(curr_con->newest_line,curr_con->oldest_line) - vis_lines;
    if (curr_con->scroll_up > max_scroll)
        curr_con->scroll_up = max_scroll;
     if (curr_con->scroll_up < 0)
        curr_con->scroll_up = 0;
    drawto_line = subLine(curr_con->newest_line,curr_con->scroll_up);
    if (lineDist(curr_con->oldest_line,drawto_line) < vis_lines)
        line = 0;
    else
        line = subLine(drawto_line,vis_lines);
    for(i=0;line != drawto_line;line = addLine(line,1),i++)
    {
        fontColor(0xffff80);
        fontText(8,32 + 10*i,curr_con->lines[line]);
    }

    // tab complete buffer
    {
        int cmd_start = conGetCurCommandStart();
        F32 x;
        char temp = curr_con->input_buf[cmd_start], 
            *cmpInp =&curr_con->input_buf[cmd_start],
            *cmpTab =curr_con->tab_comp_buf;

        curr_con->input_buf[cmd_start] = 0;
        x = fontStringWidth(curr_con->input_buf);
        curr_con->input_buf[cmd_start] = temp;

        if ( cmdIsServerCommand(curr_con->tab_comp_buf) )
            fontColor(0xffff00);
        else
            fontColor(0xffffff);
        // correct the capitolization on the tab complete buffer
        while ( *cmpInp && *cmpTab )
        {
            if ( *cmpInp == '_' && *cmpTab != '_' )
            {
                memmove( cmpTab + 1, cmpTab, strlen(cmpTab) + 1 );
                *cmpTab = '_';
            }
            else if ( *cmpTab != *cmpInp )
                *cmpTab = *cmpInp;
            ++cmpTab;
            ++cmpInp;
        }
        fontText(x+16,(curr_con->v_chars - 2) * 10,curr_con->tab_comp_buf);
    }

    // input buffer
    show_cursor = ((++curr_con->cursor_anim) >> 3) & 1;
    sprintf(buf,">%s",curr_con->input_buf);
    fontColor(0x0ff00);
    fontText(8,(curr_con->v_chars - 2) * 10,buf);

    if(show_cursor){
        char temp = curr_con->input_buf[curr_con->input_pos];
        F32 x;

        curr_con->input_buf[curr_con->input_pos] = '\0';
    
        x = fontStringWidth(curr_con->input_buf);

        curr_con->input_buf[curr_con->input_pos] = temp;
        
        fontText(16 + x,(curr_con->v_chars - 2) * 10,"_");
    }
}

static void conInsertChar(char c)
{
    // Move everything after the cursor one character forward.
    
    memmove(curr_con->input_buf + curr_con->input_pos + 1,
            curr_con->input_buf + curr_con->input_pos,
            strlen(curr_con->input_buf + curr_con->input_pos) + 1);
    
    curr_con->input_buf[curr_con->input_pos++] = c;
}

static void conDeleteCharAt(int pos)
{
    if(pos >= 0){
        memmove(curr_con->input_buf + pos,
                curr_con->input_buf + pos + 1,
                strlen(curr_con->input_buf + pos));
    }
}

void conRunCommand()
{
    if(curr_con->input_buf[0]){
        strcpy(curr_con->last_input,curr_con->input_buf);
        mruAddToList(curr_con->mruHist, curr_con->input_buf);
        curr_con->input_pos = 0;
        curr_con->scroll_up = 0;
        curr_con->hist_line = curr_con->mruHist->count;
        curr_con->input_buf[0] = '\0';
        if (curr_con->last_input[0]=='/')
            cmdParse(curr_con->last_input+1);
        else
            cmdParse(curr_con->last_input);
    }
}
    
void conGetInput()
{
    U8    buf[64];
    KeyInput* input;
    static int last_tab_search = 0, last_key_hit_was_tab = 0;
    static char last_tab_search_string[256] = {0};

    for (input = inpGetKeyBuf(buf); input; inpGetNextKey(&input))
    {
        curr_con->cursor_anim = 8;
    
        // Ignore all unicode input.
        if(KIT_Unicode == input->type)
            continue;
        
        if(KIT_EditKey == input->type){
            if(INP_UP == input->key || INP_DOWN == input->key)
            {
                if (INP_UP == input->key)
                    curr_con->hist_line--;
                if (INP_DOWN == input->key)
                    curr_con->hist_line++;
                if (curr_con->hist_line < 0){
                    curr_con->hist_line = 0;
                }
                if (curr_con->hist_line >= curr_con->mruHist->count)
                    curr_con->hist_line = curr_con->mruHist->count;

                if(curr_con->hist_line < curr_con->mruHist->count)
                    strcpy(curr_con->input_buf,curr_con->mruHist->values[curr_con->hist_line]);
                else
                    curr_con->input_buf[0] = '\0';

                curr_con->input_pos = strlen(curr_con->input_buf);

                // clear out tab completion buffers
                last_tab_search_string[0] = 0;
                curr_con->tab_comp_buf[0] = 0;
            }
            if (input->key == INP_BACKSPACE && curr_con->input_pos > 0)
            {
                conDeleteCharAt(curr_con->input_pos - 1);
                curr_con->input_pos--;
                curr_con->tab_comp_buf[0] = 0;
            }
            if (input->key == INP_DELETE && curr_con->input_buf[curr_con->input_pos] != '\0')
            {
                if(inpLevel(INP_CONTROL)){
                }else{
                    conDeleteCharAt(curr_con->input_pos);
                }
            }
            if (input->key == INP_RETURN || input->key == INP_NUMPADENTER)
            {
                //conPrint(curr_con->input_buf);
                if(strlen(curr_con->input_buf)){
                    conRunCommand();
                    curr_con->tab_comp_buf[0] = 0;
                }
            }
            if (input->key == INP_LEFT && curr_con->input_pos > 0){
                if(input->attrib & KIA_CONTROL)
                {
                    // Go to character after first non-whitespace to the left.

                    for(;
                        curr_con->input_pos > 0 && isspace((unsigned char)curr_con->input_buf[curr_con->input_pos - 1]);
                        curr_con->input_pos--);
                        
                    // Go to character after first whitespace to the left.

                    for(;
                        curr_con->input_pos > 0 && !isspace((unsigned char)curr_con->input_buf[curr_con->input_pos - 1]);
                        curr_con->input_pos--);
                }
                else
                {
                    curr_con->input_pos--;
                }
            }
            if (input->key == INP_RIGHT && curr_con->input_buf[curr_con->input_pos] != '\0'){
                if(input->attrib & KIA_CONTROL)
                {
                    // Go to first whitespace to the right.

                    for(;
                        curr_con->input_buf[curr_con->input_pos] && !isspace((unsigned char)curr_con->input_buf[curr_con->input_pos]);
                        curr_con->input_pos++);
                        
                    // Go to first non-whitespace to the right.

                    for(;
                        curr_con->input_buf[curr_con->input_pos] && isspace((unsigned char)curr_con->input_buf[curr_con->input_pos]);
                        curr_con->input_pos++);
                }
                else
                {
                    curr_con->input_pos++;
                }
            }
            if (input->key == INP_HOME)
            {
                curr_con->input_pos = 0;
            }
            if (input->key == INP_END)
            {
                curr_con->input_pos = strlen(curr_con->input_buf);
            }
            if (input->key == INP_V &&input->attrib & KIA_CONTROL)
            {
                int runLast = 0;

                if(OpenClipboard(hwnd)){
                    HANDLE handle = GetClipboardData(CF_TEXT);

                    if(handle){
                        char* data = GlobalLock(handle);

                        for(; data && *data; data++){
                            if(*data == '\n' || *data == '\r'){
                                conRunCommand();
                                runLast = 1;
                            }else{
                                conInsertChar(*data);
                            }
                        }
                    }

                    GlobalUnlock(handle);
                    CloseClipboard();

                    if(runLast && curr_con->input_buf[0]){
                        conRunCommand();
                    }
                }
            }
            if (input->key == INP_ESCAPE)
            {
                curr_con->input_buf[0] = 0;
                curr_con->tab_comp_buf[0] = 0;
                curr_con->input_pos = 0;
            }
            if (input->key == INP_TAB)
            {
                char tempbuf[256] = {0};
                int search_backwards = 0, cmd_start;

                if ( last_key_hit_was_tab )
                {
                    int i = strlen(curr_con->input_buf) - 1;
                    // remove trailing spaces
                    while ( curr_con->input_buf[i] == ' ' )
                    {
                         curr_con->input_buf[i] = 0; --i;
                    }
                }

                cmd_start = conGetCurCommandStart();

                if ( input->attrib & KIA_SHIFT )
                    search_backwards = 1;
                
                if ( !last_tab_search_string[0] )
                    strcpy(last_tab_search_string,&curr_con->input_buf[cmd_start]);
                if ( curr_con->input_pos - cmd_start >= (int)strlen(curr_con->tab_comp_buf) )
                {
                    last_tab_search = cmdCompleteComand(last_tab_search_string,tempbuf,last_tab_search,search_backwards);
                }

                if ( tempbuf[0] )
                {
                    strcpy(&curr_con->input_buf[cmd_start],tempbuf);
                    curr_con->input_pos = strlen(curr_con->input_buf);
                    conInsertChar(' ');
                    curr_con->tab_comp_buf[0] = 0;
                    //strcpy(curr_con->tab_comp_buf,tempbuf);
                }
                else if ( curr_con->tab_comp_buf[0] )
                {
                    // remove trailing spaces from the string
                    int i;
                    strcpy( &curr_con->input_buf[cmd_start], curr_con->tab_comp_buf );
                    curr_con->tab_comp_buf[0] = 0;
                    i = strlen(curr_con->input_buf) - 1;
                    while ( curr_con->input_buf[i] == ' ' )
                    {
                         curr_con->input_buf[i] = 0; --i;
                    }
                    curr_con->input_pos = strlen(curr_con->input_buf);
                    conInsertChar(' ');
                }

                last_key_hit_was_tab = 1;
            }
            else 
            {
                last_tab_search = 0;
                last_tab_search_string[0] = 0;
                last_key_hit_was_tab = 0;
            }

        }
        
        if(KIT_Ascii == input->type){
            if (input->asciiCharacter < 128 && 
                input->asciiCharacter >= 32 && 
                input->asciiCharacter != '`' && 
                isprint((unsigned char)input->asciiCharacter) && 
                curr_con->input_pos < (LINE_SIZE - 2))
            {
                char tempbuf[256];
                int newSearch = 0, cmd_start_pos;

                last_key_hit_was_tab = 0;

                // find the start of the command to complete
                cmd_start_pos = conGetCurCommandStart();

                if ( !curr_con->tab_comp_buf[0] ||
                    input->asciiCharacter != curr_con->tab_comp_buf[curr_con->input_pos-cmd_start_pos] )
                {
                    newSearch = 1;
                    last_tab_search = 0;
                }
                else
                    conDeleteCharAt(curr_con->input_pos);

                // we are performing a new search and we havent done a search from this input position yet
                if ( newSearch && cmd_start_pos <= curr_con->input_pos &&
                    !curr_con->tab_comp_buf[curr_con->input_pos-cmd_start_pos] )
                    last_tab_search = 0;

                conInsertChar(input->asciiCharacter);

                // dont try to tab complete if they are editing previous input
                if ( cmd_start_pos <= curr_con->input_pos )
                {
                    strncpy(last_tab_search_string,&curr_con->input_buf[cmd_start_pos],curr_con->input_pos-cmd_start_pos);
                    last_tab_search_string[curr_con->input_pos-cmd_start_pos] = 0;
                    // do a new search if the last character they typed didnt match the 
                    // string we gave them
                    if (newSearch)
                    {
                        if ( input->asciiCharacter == ' ' )
                            last_tab_search = 0;

                        last_tab_search = cmdCompleteComand(last_tab_search_string,tempbuf,last_tab_search,0);

                        if ( tempbuf[0] )
                            strcpy(curr_con->tab_comp_buf,tempbuf);
                        else
                            curr_con->tab_comp_buf[0] = 0;
                    }
                }
            }
        }
    }
}

void conProcess()
{
    // The tilde key toggles the console.
    if(cmdAccessLevel() > 0 || demoIsPlaying()){
        // The Vista Infared driver remaps the TILDE key to the KANJI key for some unknown reason.
        if (inpEdge(INP_TILDE) || inpEdge(INP_KANJI))
        {
            conEnterMode(!conVisible());
        }
    }else{
        conEnterMode(0);
    }
    conResizeCheck();
    if (!show_console)
        return;

    conGetInput();

    conRender();
}

int conVisible()
{
    return show_console;
}

static void* conInputToken;
static int conEnterMode(int enter)
{
    if(enter)
    {
        if(uiGetFocusEx(&conInputToken, TRUE, uiFocusRefuseAllRequests))
        {
            show_console = 1;
            return 1;
        }
        return 0;
    }
    else
    {
        uiReturnFocus(&conInputToken);
        show_console = 0;
        return 1;
    }
}
