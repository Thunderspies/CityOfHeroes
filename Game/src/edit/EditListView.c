#include "EditListView.h"
#include <stdlib.h>
#include <string.h>
#include "graphics/font.h"
#include "win/input.h"
#include "UI/uiInput.h"
#include "cmdparse/cmdcommon.h"
#include "group/group.h"
#include <utilitieslib/utils/mathutil.h>
#include "edit/edit_select.h"
#include "edit/edit_net.h"
#include "edit/edit_cmd.h"
#include "win/win_init.h"
#include "graphics/ttFontUtil.h"
#include "entity/entDebug.h"
#include "graphics/ttFont.h"
#include "UI/sprite/sprite_base.h"
#include "render/tex.h"
#include "UI/sprite/sprite_text.h"
#include "graphics/textureatlas.h"

EditListView * newELV(int x,int y,int width,int height) {
    EditListView * lv = (EditListView *)calloc(1, sizeof(EditListView));
    int i;
    lv->x=x;
    lv->y=y;
    lv->width=width;
    lv->height=height;
    lv->lineHeight=12;
    lv->realHeight=0;
    lv->totalItemsDisplayed=1;
    lv->hasFocus=0;
    lv->colorColumns=0;
    lv->amountScrolled=0;
    lv->theWindowCorner=EDIT_LIST_VIEW_UPPERLEFT;
    lv->theCorner=EDIT_LIST_VIEW_UPPERLEFT;
    lv->getSelected=NULL;
    lv->info=NULL;
    lv->growUp=0;
    lv->titleBarMove=0;
    lv->windowResize=0;
    lv->scrollBarActive=0;
    lv->scrollBarMove=0;
    lv->scrollBarStart=0;
    lv->scrollBarEnd=0;
    lv->resizerActive=0;
    lv->resizerMove=0;
    lv->clickx=-1;
    lv->clicky=-1;
    lv->titleLastClicked=0;
    lv->titleBarDoubleClick=0;
    lv->hidden=0;
    lv->shrunk=0;
    lv->hiddenOffsetX=0;
    lv->hiddenTime=0;
    lv->columnWidth=75;
    i=0;
    while (masterELVList[i]!=NULL) i++;
    masterELVList[i]=lv;
    lv->level=9800-i*200;
    lv->topLatch=NULL;
    lv->bottomLatch=NULL;
    lv->leftLatch=NULL;
    lv->rightLatch=NULL;
    return lv;
};

void destroyELV(EditListView * elv) {
    int i=0;
    while (masterELVList[i]!=elv) i++;
    masterELVList[i]=NULL;
}

void handleAllELV() {
    int i;
    for (i=0;i<MAX_EDIT_LIST_VIEWS;i++)
        if (masterELVList[i]!=NULL)
            handleELV(masterELVList[i]);
}
void displayAllELV() {
    int i;
    for (i=0;i<MAX_EDIT_LIST_VIEWS;i++)
        if (masterELVList[i]!=NULL)
            displayELV(masterELVList[i]);
}

void hideAllELV() {
    int i;
    for (i=0;i<MAX_EDIT_LIST_VIEWS;i++)
        if (masterELVList[i]!=NULL)
            hideELV(masterELVList[i]);
}

int anyHaveFocusELV() {
    int i;
    for (i=0;i<MAX_EDIT_LIST_VIEWS;i++)
        if (masterELVList[i]!=NULL && masterELVList[i]->hasFocus)
            return 1;
    return 0;
}

int busyELV(EditListView * elv) {
    return (elv->hidden || elv->hiddenTime);
}

void setELVTextCallback(EditListView * lv,void (*getList)(int start,int end,char ** text,int * colors,int * max,void * info)) {
    lv->getList=getList;
}

void setELVInfo(EditListView * lv,void * info) {
    lv->info=info;
}

void setELVSelectedCallback(EditListView * lv,void (*getSelected)(int ** lineNums,int * size)) {
    lv->getSelected=getSelected;
}

void hideELV(EditListView * elv) {
    elv->hidden^=1;
    elv->hiddenTime=clock();
    elv->hiddenLastMoveTime=elv->hiddenTime;
}

void keepOnScreen(EditListView * elv,int xoff,int yoff) {
    int x,y;
    int wx,wy;    //window dimensions
    windowSize(&wx,&wy);
    wy+=10;
    x=elv->x+xoff;
    y=elv->y+yoff;
    if (elv->theWindowCorner==EDIT_LIST_VIEW_UPPERRIGHT || elv->theWindowCorner==EDIT_LIST_VIEW_LOWERRIGHT)
        x+=wx;
    if (elv->theWindowCorner==EDIT_LIST_VIEW_LOWERLEFT  || elv->theWindowCorner==EDIT_LIST_VIEW_LOWERRIGHT) {
        y+=wy;
        y-=elv->realHeight+10;
    }
    if (elv->theCorner==EDIT_LIST_VIEW_UPPERRIGHT || elv->theCorner==EDIT_LIST_VIEW_LOWERRIGHT)
        x-=elv->width;
    if (elv->theCorner==EDIT_LIST_VIEW_LOWERLEFT  || elv->theCorner==EDIT_LIST_VIEW_LOWERRIGHT)
        y-=elv->realHeight;
    if (x<5)
        elv->x+=5-x;
    if (x>wx-5)
        elv->x-=x-(wx-5);
    if (y<15)
        elv->y+=15-y;
    if (y>wy-5)
        elv->y-=y-(wy-5);
}

void figureLatches(EditListView * elv) {
    if (elv->bottomLatch!=NULL) {
        elv->bottomLatch->x=elv->x;
        elv->bottomLatch->y=elv->y+(elv->shrunk?0:elv->realHeight)+27;
        elv->bottomLatch->width=elv->width;
        keepOnScreen(elv->bottomLatch,0,0);
        keepOnScreen(elv->bottomLatch,elv->bottomLatch->width,0);
    }
    if (elv->topLatch!=NULL) {
        elv->topLatch->x=elv->x;
        elv->topLatch->y=elv->y-(elv->topLatch->shrunk?0:elv->topLatch->realHeight)-27;
        elv->topLatch->width=elv->width;
        keepOnScreen(elv->topLatch,0,0);
        keepOnScreen(elv->topLatch,elv->topLatch->width,0);
    }
}

void handleELV(EditListView * lv) {
    int mx,my;                //mouse coordinates
    int wx,wy;                //window dimensions
    extern int mouse_dy;

    inpMousePos(&mx,&my);
    windowSize(&wx,&wy);

    lv->realHeight=lv->lineHeight*lv->totalItemsDisplayed;
    if (lv->realHeight>lv->height)
        lv->realHeight=lv->height;

    lv->finalx=lv->x;
    lv->finaly=lv->y;
    if (lv->theWindowCorner==EDIT_LIST_VIEW_LOWERLEFT || lv->theWindowCorner==EDIT_LIST_VIEW_LOWERRIGHT)
        lv->finaly+=wy;
    if (lv->theWindowCorner==EDIT_LIST_VIEW_UPPERRIGHT || lv->theWindowCorner==EDIT_LIST_VIEW_LOWERRIGHT)
        lv->finalx+=wx;

    if (lv->theCorner==EDIT_LIST_VIEW_LOWERLEFT || lv->theCorner==EDIT_LIST_VIEW_LOWERRIGHT)
        lv->finaly-=lv->realHeight;
    if (lv->theCorner==EDIT_LIST_VIEW_UPPERRIGHT || lv->theCorner==EDIT_LIST_VIEW_LOWERRIGHT)
        lv->finalx-=lv->width;

    
    //this line is needed because if height is not a multiple of lineHeight
    //the last line could get cut off the LIST_VIEW, which shouldn't be able
    //to happen anyway, but this works
//    lv->height-=lv->height%lv->lineHeight;

    if (lv->growUp)
        lv->finaly-=lv->realHeight;

    if (edit_state.look && lv->hasFocus) {
        lv->amountScrolled-=mouse_dy;
    }


    //callbacks
    mx-=lv->finalx;
    my-=lv->finaly;

    mx+=2;    //this makes selection look a little bit better;

    lv->lineOver=-1;
    lv->lineClicked=-1;

    if (!isDown(MS_LEFT) && !isDown(MS_RIGHT))
        lv->hasFocus=0;

    if (busyELV(lv)) {
        lv->hasFocus=0;
        lv->titleBarMove=0;
        lv->resizerActive=0;
        lv->scrollBarMove=0;
    }

    if (lv->realHeight>=lv->height-lv->lineHeight)
        lv->resizerActive=1;
    else
        lv->resizerActive=0;

    lv->titleBarDoubleClick=0;
    if (mx>=0 && mx<=lv->width && my>=-lv->lineHeight-10 && my<=lv->realHeight+5 && !edit_state.focusStolen && (my<0 || !lv->shrunk)) {
        int move=lv->titleBarMove | lv->scrollBarMove | lv->resizerMove;
//        if (lv->takeFocus && (edit_state.sel || edit_state.look))
//            lv->hasFocus=1;
        if (!busyELV(lv) && !isDown(MS_LEFT) && !edit_state.look && !edit_state.zoom && !edit_state.pan && !edit_state.lightcolor) {
            edit_state.focusStolen=1;
            lv->hasFocus=1;
        }
        if (lv->hasFocus) {
            if (edit_state.sel && my<-2 && !lv->titleBarMove && !move) {    //<-2 makes it look a little nicer
                int ticks=clock();
                if (ticks-lv->titleLastClicked<(CLK_TCK/2)) {
                    lv->titleBarDoubleClick=1;
                    lv->titleLastClicked=0;
                } else {
                    lv->titleLastClicked=clock();
                    lv->titleLastClicked=ticks;
                }
                lv->titleBarMove=1;

                lv->clickx=mx;
                lv->clicky=my;
            }
            if (edit_state.sel && lv->scrollBarActive && !lv->scrollBarMove && my>0 && my<lv->realHeight-10 && mx>lv->width-10 && !move) {
                lv->scrollBarMove=1;
                if (my<lv->scrollBarStart-lv->finaly || my>lv->scrollBarEnd-lv->finaly)
                    lv->clicky=-1;
                else
                    lv->clicky=lv->scrollBarStart-lv->finaly-my;
            }
            if (edit_state.sel && lv->resizerActive && !lv->resizerMove && mx>lv->width-10 && my>lv->realHeight-10 && !move) {
                lv->resizerMove=1;
                lv->clickx=lv->width-mx;
                lv->clicky=lv->height-my;
            }
        }
        lv->widthOver=mx;
        if (my>0 && (!lv->scrollBarActive || mx<lv->width-15) && !lv->shrunk) {
            lv->lineOver=(my+lv->amountScrolled%lv->lineHeight)/lv->lineHeight;
            if (edit_state.sel && !edit_state.lightcolor) {
                lv->lineClicked=lv->lineOver;
                lv->clickInfo.line=lv->lineClicked;
                lv->clickInfo.x=mx;
            }
        }
    }
    if (lv->titleBarDoubleClick)
        lv->shrunk=!lv->shrunk;
    if (!isDown(MS_LEFT)) {
        lv->titleBarMove=0;
        lv->scrollBarMove=0;
        lv->resizerMove=0;
    }
    if (lv->titleBarMove) {
        lv->x-=lv->clickx-mx;
        lv->y-=lv->clicky-my;
    }

    if (lv->scrollBarMove) {
        if (lv->clicky==-1)
            lv->amountScrolled=((my-(lv->scrollBarEnd-lv->scrollBarStart)/2)*lv->totalItemsDisplayed*lv->lineHeight)/(lv->realHeight-10);
        else
            lv->amountScrolled=((my+lv->clicky)*lv->totalItemsDisplayed*lv->lineHeight)/(lv->realHeight-10);
    }

    if (lv->resizerMove) {
        lv->width=lv->clickx+mx;
        if (lv->growUp) {
            int change=lv->height-(lv->clicky+my);
            lv->height-=change;
            if (lv->height<50) {
                change=50-lv->height;
                lv->height=50;
            } else
            if (lv->height>lv->totalItemsDisplayed*lv->lineHeight) {
                change=lv->height-lv->totalItemsDisplayed*lv->lineHeight;
                lv->height=lv->totalItemsDisplayed*lv->lineHeight;
            } else
            lv->y-=change;
        } else
            lv->height=lv->clicky+my;
        if (lv->width<50)
            lv->width=50;
        if (lv->height<50)
            lv->height=50;
        if (lv->finalx+lv->width+5>wx)
            lv->width=wx-5-lv->finalx;
        if (lv->finaly+lv->height+5>wy)
            lv->height=wy-5-lv->finaly;
    }

    if (lv->amountScrolled>lv->totalItemsDisplayed*lv->lineHeight-lv->height)
        lv->amountScrolled=lv->totalItemsDisplayed*lv->lineHeight-lv->height;
    if (lv->amountScrolled<0)
        lv->amountScrolled=0;

    if (lv->hasFocus)
        edit_state.sel=0;

    if (!lv->hidden && lv->hiddenTime==0) {
        keepOnScreen(lv,0,0);
        keepOnScreen(lv,lv->width,0);
    } else if (!lv->hidden) {
        int timeToUnhide=.5*CLK_TCK;        //half a second, looks good
        int currentTime=clock();
        if (currentTime-lv->hiddenTime<timeToUnhide) {
            int xToMove=-lv->hiddenOffsetX;
            lv->hiddenOffsetX+=xToMove*(double)(currentTime-lv->hiddenLastMoveTime)/(double)timeToUnhide;
        } else {
            lv->hiddenLastMoveTime=-1;
            lv->hiddenTime=0;
            lv->hiddenOffsetX=0;
        }
    } else {
        int timeToHide=.5*CLK_TCK;        //half a second, looks good
        int currentTime=clock();
        if (currentTime-lv->hiddenTime<timeToHide) {
            int xToMove;
            if (lv->finalx+lv->width/2<wx/2)
                xToMove=-(lv->finalx+lv->hiddenOffsetX+lv->width+50);
            else
                xToMove=wx+50-(lv->finalx+lv->hiddenOffsetX);
            lv->hiddenOffsetX+=xToMove*(double)(currentTime-lv->hiddenLastMoveTime)/(double)timeToHide;
        } else {
            lv->hiddenLastMoveTime=-1;
            lv->hiddenTime=0;
            if (lv->finalx+lv->width/2<wx/2)
                lv->hiddenOffsetX=-(lv->finalx+lv->width+50);
            else
                lv->hiddenOffsetX=wx+50-(lv->finalx);
        }
    }
    lv->finalx+=lv->hiddenOffsetX;
    figureLatches(lv);
};

static TTDrawContext* getDebugFont(){
    static TTDrawContext* debugFont = NULL;

    if(!debugFont){
        extern TTFontManager* fontManager;
        TTCompositeFont* font = createTTCompositeFont();

        debugFont = createTTDrawContext();
        initTTDrawContext(debugFont, 64);
        ttFontAddFallback(font, ttFontLoadCreate("fonts/tahomabd.ttf",0));
        ttFMAddFont(fontManager, font);
        debugFont->font = font;
        debugFont->dynamic = 1;
        debugFont->renderParams.renderSize = 11;
        debugFont->renderParams.outline = 1;
        debugFont->renderParams.outlineWidth = 1;
    }

    return debugFont;
}

void displayELV(EditListView * lv) {
    int i;
    int start=lv->amountScrolled/lv->lineHeight-1;    //starting line number
    int end=start+(lv->height+lv->lineHeight-1)/lv->lineHeight;    //ending line number
    char ** text=(char **)_alloca((end-start+1)*sizeof(char *)); // now an array of EStrings!
    int * colors=(int *)_alloca((end-start+1)*sizeof(int));
    int * lineNums=NULL;
    int numLinesSelected=0;
    int rgba[4];
    int halfrgba[4];
    int color;
    TTDrawContext * font=getDebugFont();
    int plus=str_wd(font,1,1,"%c",'+');
    int minus=str_wd(font,1,1,"%c",'-');
    int column;
    static int widths[128];
    if (widths['a']==0) {
        for (i=0;i<128;i++)
            widths[i]=str_wd(font,1,1,"%c",(char)i);
    }
    if (lv->hidden && lv->hiddenLastMoveTime==-1)
        return;

    //title bar
    if (lv->titleBarMove)
        color=EDIT_LIST_VIEW_ACTIVE;
    else
        color=EDIT_LIST_VIEW_INACTIVE;
    display_sprite(white_tex_atlas,
        lv->finalx-5,
        lv->finaly-15,
        10+lv->level,
        (double)(lv->width+10)/8.0,
        (double)lv->lineHeight/8.0,
        color);

    if (lv->shrunk)
        return;


    for (i=0;i<end-start+1;i++) {
        estrCreate(&text[i]);
    }

    //these two lines let us get data for one extra line (the one that is dissapearing under the
    //                                                      titlebar is the user scrolls down)
    if (lv->lineOver!=-1) lv->lineOver++;
    if (lv->lineClicked!=-1) lv->lineClicked++;
    lv->getList(start,end,text,colors,&lv->totalItemsDisplayed,lv->info);
    if (lv->getSelected!=NULL)
        lv->getSelected(&lineNums,&numLinesSelected);
    column=lv->widthOver/lv->columnWidth;
    if (lv->totalItemsDisplayed*lv->lineHeight>lv->height) {
        if (!lv->scrollBarActive)
            lv->scrollBarActive=1;
        if (lv->totalItemsDisplayed==0)
            lv->totalItemsDisplayed=1;        //don't want to divide by zero, shouldn't actually ever get to this line
        lv->scrollBarStart=lv->finaly+((start)*(lv->realHeight-10))/lv->totalItemsDisplayed;
        lv->scrollBarEnd=lv->finaly+((end)*(lv->realHeight-10))/lv->totalItemsDisplayed;
    } else {
        if (lv->scrollBarActive)
            lv->scrollBarActive=0;
    }

    //shaded background
    display_sprite(white_tex_atlas,
        lv->finalx-5,
        lv->finaly-lv->lineHeight/2,
        -100+lv->level,
        (double)(lv->width+10)/8.0,
        (double)(lv->realHeight+1.5*lv->lineHeight)/8.0,
         0x000000A7);
    for (i = 0; i < end - start + 1; i++)
    {
        int x=lv->finalx;
        int y=lv->finaly+lv->lineHeight/2+i*lv->lineHeight-lv->amountScrolled%lv->lineHeight+5;
        int textStart=0;
        int textStop;
        int lastChar;
        int totalWidth=0;
        int curColumn=0;
        int lowColumn=0;
        int highColumn=10;
        if (y<lv->finaly+lv->lineHeight-5)
            continue;
        while (text[i][textStart]==' ') {
            x+=8;
            textStart++;
            totalWidth+=8;
        }
        rgba[0]=colors[i];
        rgba[1]=colors[i];
        rgba[2]=colors[i];
        rgba[3]=colors[i];
        if (strchr(text[i],'^')!=NULL)
            halfrgba[0]=(((colors[i]&0xff000000)>>1)&0xff000000)+(((colors[i]&0xff0000)>>1)&0xff0000)+(((colors[i]&0xff00)>>1)&0xff00)+((colors[i])&0xff);
        else
            halfrgba[0]=colors[i];
        halfrgba[1]=halfrgba[0];
        halfrgba[2]=halfrgba[0];
        halfrgba[3]=halfrgba[0];
        if (text[i][textStart]=='+' || text[i][textStart]=='-') {
            int width;
            int smallWidth;
            if (plus>minus) {
                width=plus;
                smallWidth=minus;
            } else {
                width=minus;
                smallWidth=plus;
            }
            if (x-lv->finalx<lv->width-20-(lv->scrollBarActive?10:0)) {
                if (text[i][textStart]=='+') {
                    printBasic(font,
                        x+(plus>minus?0:smallWidth/2),
                        y-lv->lineHeight,
                        0+lv->level,
                        1,
                        1,
                        0,
                        "+",
                        1,
                        rgba);
                } else if (text[i][textStart]=='-') {
                    printBasic(font,
                        x+(minus>plus?0:smallWidth/2),
                        y-lv->lineHeight,
                        0+lv->level,
                        1,
                        1,
                        0,
                        "-",
                        1,
                        rgba);
                }
            }
            x+=width;
            textStart++;
            totalWidth+=width;
        }
        lastChar=textStart;
        while (totalWidth<lv->width-20-(lv->scrollBarActive?10:0) && text[i][lastChar]) {
            if (text[i][lastChar]=='^' && (text[i][lastChar+1]>='0' && text[i][lastChar+1]<='9')) {    //handle escape characters here, shouldn't count towards width
                int col=text[i][lastChar+1]-'0';
                totalWidth=col*lv->columnWidth;    //must account for the change in columns
                lastChar+=2;                    //assuming "^#", and only one digit allowed
                if (col>lowColumn && col<=column)
                    lowColumn=col;
                if (col<highColumn && col>column)
                    highColumn=col;
                continue;
            }
            totalWidth+=widths[text[i][lastChar]];
            totalWidth--;
            lastChar++;
        }
        do {
            textStop=textStart;
            while (text[i][textStop]!='\0' && !(text[i][textStop]=='^' && (text[i][textStop+1]>='0' && text[i][textStop+1]<='9')))
                textStop++;
            printBasic(font,
                x,
                y-lv->lineHeight,
                0+lv->level,
                1,
                1,
                0,
                &text[i][textStart],
                textStop-textStart,
                (!lv->colorColumns || (curColumn>=lowColumn && curColumn<highColumn) || i!=(lv->lineOver))?rgba:halfrgba);
            if (text[i][textStop]=='^' && (text[i][textStop+1]>='0' && text[i][textStop+1]<='9')) {
                x=lv->finalx+(text[i][textStop+1]-'0')*lv->columnWidth;
                textStart=textStop+2;
                curColumn=text[i][textStop+1]-'0';
            }
        } while (text[i][textStop]!='\0');
    }

    if (lv->scrollBarActive) {
        display_sprite(white_tex_atlas,
            lv->finalx+lv->width-10,
            lv->finaly,
            -100+lv->level,
            10.0/8.0,
            (double)(lv->realHeight+5)/8.0,
            0x101010FF);

        if (lineNums!=NULL) {
            for (i=0;i<numLinesSelected;i++) {
                int mark=((lineNums[i])*(lv->realHeight-10))/lv->totalItemsDisplayed+(lv->realHeight-10)/(2*lv->totalItemsDisplayed);
                display_sprite(white_tex_atlas,
                    lv->finalx+lv->width-10,
                    lv->finaly+mark,
                    -90+lv->level,
                    10.0/8.0,
                    1.0/8.0,
                    0xFF0000FF);
            }
        }

        //scroll bar
        if (lv->scrollBarMove)
            color=EDIT_LIST_VIEW_ACTIVE;
        else
            color=EDIT_LIST_VIEW_INACTIVE;
        display_sprite(white_tex_atlas,
            lv->finalx+lv->width-10,
            lv->scrollBarStart,
            -80+lv->level,
            10.0/8.0,
            (double)(lv->scrollBarEnd-lv->scrollBarStart)/8.0,
            color);
    }

    if (lv->resizerActive) {
        if (lv->resizerMove)
            color=EDIT_LIST_VIEW_ACTIVE;
        else
            color=EDIT_LIST_VIEW_INACTIVE;
        display_sprite(white_tex_atlas,
            lv->finalx+lv->width-10,
            lv->finaly+lv->height-10,
            -80+lv->level,
            10.0/8.0,
            10.0/8.0,
            color);
    }

    for (i=0;i<end-start+1;i++) {
        estrDestroy(&text[i]);
    }
};

void ELVLatchTopBottom(EditListView * top,EditListView * bottom) {
    top->bottomLatch=bottom;
    bottom->topLatch=top;
    figureLatches(top);
    figureLatches(bottom);
}

void ELVLatchLeftRight(EditListView * left,EditListView * right) {
    left->rightLatch=right;
    right->leftLatch=left;
    figureLatches(left);
    figureLatches(right);
}
