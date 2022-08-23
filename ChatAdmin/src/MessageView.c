#include "MessageView.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>


TextEntry * TextEntryCreate(char * msg)
{
    TextEntry * te = calloc(sizeof(TextEntry), 1);

    te->line = strdup(msg);
    te->time = timerSecondsSince2000();

    return te;
}

void TextEntryDestroy(TextEntry * te)
{
    free(te->line);
    free(te);
}


MessageView * MessageViewCreate(HWND hWndView, int maxMsgs)
{
    MessageView * mv = calloc(sizeof(MessageView), 1);

    mv->hWndView    = hWndView;
    mv->maxMsgs        = maxMsgs;
    eaCreate(&mv->msgs);

    return mv;
}

void MessageViewDestroy(MessageView * mv)
{
    eaClearEx(&mv->msgs, TextEntryDestroy);
}

//void MessageViewProcessCmd(MessageView * mv, WPARAM wParam, LPARAM lParam);

void MessageViewRefresh(MessageView * mv)
{
    char * text = 0;
    int i, size = eaSize(&mv->msgs);

    estrReserveCapacity(&text, 10000);
    
    if(mv->showTime)
    {
        for(i=0;i<size;i++)
        {
            char buf[200];
            TextEntry * te = mv->msgs[i];
            timerMakeDateStringFromSecondsSince2000(buf, te->time);
            estrConcatf(&text, "%s%s:  %s", (i ? "\r\n" : ""), buf, te->line);
        }
    }
    else
    {
        for(i=0;i<size;i++)
        {
            estrConcatf(&text, "%s%s", (i ? "\r\n" : ""), mv->msgs[i]->line);
        }
    }

    SetWindowText(mv->hWndView, text);
     SendMessage(mv->hWndView, WM_VSCROLL, SB_BOTTOM, 0);


    estrDestroy(&text);
}


void MessageViewEmpty(MessageView * mv)
{
    eaClearEx(&mv->msgs, TextEntryDestroy);
    SetWindowText(mv->hWndView, "");
}

void MessageViewAdd(MessageView * mv, char * msg, int type)
{
    eaPush(&mv->msgs, TextEntryCreate(msg));
    if(eaSize(&mv->msgs) > mv->maxMsgs)
        eaRemove(&mv->msgs, 0);

    MessageViewRefresh(mv);
}

void MessageViewShowTime(MessageView * mv, bool showTime)
{
    if(mv->showTime != showTime)
    {
        mv->showTime = showTime;
        MessageViewRefresh(mv);
    }
}
