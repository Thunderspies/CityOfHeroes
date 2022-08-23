/*\
 *
 *    ConsoleDebug.h/c - Copyright 2004, 2005 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    A keyboard handler for console server apps that want to have a 
 *  simple interactive debug menu.
 *
 */
#ifndef _XBOX

#include "utilitieslib/utils/ConsoleDebug.h"
#include "utilitieslib/stdtypes.h"
#include <stdio.h>
#include <conio.h>
#include "utilitieslib/utils/wininclude.h"

void ConsoleDebugPause(void)
{
    printf("SERVER PAUSED FOR 2 SECONDS PRESS 'p' AGAIN TO PAUSE INDEFINITELY...\n");
    Sleep(2000);
    if (_kbhit() && _getch()=='p') {
        printf("SERVER PAUSED\n");
        _getch();
    }
    printf("Server resumed\n");
}

void ConsoleDebugPrintHelp(ConsoleDebugMenu* menu)
{
    while (menu->cmd || menu->helptext)
    {
        if (menu->cmd && menu->helptext)
            printf("   %c - %s\n", menu->cmd, menu->helptext);
        else if (menu->cmd)
            ; // (hidden command)
        else
            printf("%s\n", menu->helptext);
        menu++;
    }
}

ConsoleDebugMenu* ConsoleDebugFind(ConsoleDebugMenu* menu, char ch)
{
    while (menu->cmd || menu->helptext)
    {
        if (menu->cmd == ch)
            return menu;
        menu++;
    }
    return NULL;
}

void DoConsoleDebugMenu(ConsoleDebugMenu* menu)
{
    static char param[200];
    static int param_idx=0;            // i'th letter in param string, 0 if not entering a param
    static char param_cmd=' ';
    static int debug_enabled=0;
    char ch;
    ConsoleDebugMenu* cmd;

    if (!_kbhit())
        return;

    if (debug_enabled < 2)
    {
        if (_getch()=='d') {
            debug_enabled++;
            if (debug_enabled==2) {
                printf("Debugging hotkeys enabled (Press '?' for a list).\n");
            }
        } else {
            debug_enabled = 0;
        }
    }
    // cheesy way to get a parameter from the user..
    else if (param_idx)
    {
        ch = _getch();
        if (ch == '\b')
        {
            if (param_idx > 1) 
            {
                param_idx--;
                printf("\b \b");
            }
        }
        else if (ch == '\n' || ch == '\r')
        {
            param[param_idx-1] = 0;
            printf("\n");
            cmd = ConsoleDebugFind(menu, param_cmd);
            if (cmd && cmd->paramfunc)
                cmd->paramfunc(param);
            param_idx = 0;
        }
        else
        {
            param[param_idx-1] = ch;
            param_idx++;
            printf("%c", ch);
            if (param_idx >= ARRAY_SIZE(param)-1)
            {
                param[ARRAY_SIZE(param)-1] = 0;
                printf("\nString too long!  Ignoring..\n");
                param_idx = 0;
            }
        }
    }
    // otherwise, just a single-keystroke command..
    else
    {
        ch = _getch();
        cmd = ConsoleDebugFind(menu, ch);
        if (cmd)
        {
            if (cmd->singlekeyfunc)
                cmd->singlekeyfunc();
            if (cmd->paramfunc)
            {
                param_idx = 1;
                param_cmd = ch;
            }
        }
        else if (ch == '?')
        {
            ConsoleDebugPrintHelp(menu);
        }
        else
        {
            printf("unrecognized command '%c' ('?' for list of commands)\n", ch);
        }
    }
}

#endif
