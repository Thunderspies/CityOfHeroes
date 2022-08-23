#include "uiAmountSlider.h"
#include <utilitieslib/stdtypes.h>
#include <utilitieslib/components/earray.h>
#include "UI/uiWindows.h"
#include "gameComm/wdwbase.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "uiSlider.h"
#include "UIEdit.h"
#include "UI/uiInput.h"
#include "win/input.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include <utilitieslib/language/MessageStoreUtil.h>

#define TEXT_OFFSET_Y 18
#define TEXT_OFFSET_X 15
#define INPUT_H 25
#define INPUT_OFFSET_Y 31
#define INPUT_OFFSET_X 90
#define OK_BUTTON_W 40
#define OK_BUTTON_OFFSET_Y (INPUT_OFFSET_Y+INPUT_H+15)
#define OK_BUTTON_OFFSET_X (INPUT_OFFSET_X+OK_BUTTON_W/2)
#define SLIDER_W 120
#define SLIDER_OFFSET_Y 95
#define SLIDER_OFFSET_X 74
#define IMG_OFFSET_Y (INPUT_OFFSET_Y-8)
#define IMG_OFFSET_X 10

typedef struct AmountSlider
{
    int initializing;
    float min;
    float max;
    int dontLimitTo99;
    const char *text;
    AmountSliderOKCallback okCallback;
    AmountSliderDrawCallback drawCallback;
    void *userData;
} AmountSlider;

static AmountSlider sAmountSlider = {0};

int amountSliderWindowSetupAndDisplay(int min, int max, const char *text, AmountSliderDrawCallback drawCallback, AmountSliderOKCallback okCallback, void *userData, int dontLimit)
{
    if ( min == max ) 
    {
        okCallback(max, userData);
        return 1;
    }

    if ( min > max ) SWAP32(min, max);
    sAmountSlider.initializing = 1;
    sAmountSlider.okCallback = okCallback;
    sAmountSlider.drawCallback = drawCallback;
    sAmountSlider.min = min;
    sAmountSlider.max = max;
    sAmountSlider.text = text ? text : "GenericAmountSliderText";
    sAmountSlider.userData = userData;
    sAmountSlider.dontLimitTo99 = dontLimit && max > 99;
    window_setMode(WDW_AMOUNTSLIDER, WINDOW_DISPLAYING);
    return 1;
}

int amountSliderWindow(void)
{
    float x, y, z, wd, ht, sc;
    int clr, bkclr;
    static float curSel = 1.0f;
    static UIEdit *edit = NULL;
    static int draggingInThisWindow = 0;

    window_getDims( WDW_AMOUNTSLIDER, &x, &y, &z, &wd, &ht, &sc, &clr, &bkclr );

    if ( window_getMode(WDW_AMOUNTSLIDER) == WINDOW_DISPLAYING )
    {
        float newCurSel = 0.0f;
        static char text[8] = {0};
        char *editText = NULL;
        UIBox bounds;
        CBox box;
        int rangeLimit = sAmountSlider.dontLimitTo99 ? 9999 : 99;
        // Set countLimit one greater than the actual number of digits allowed.  The clamp code will ensure we never exceed the slider's max value.
        // By setting this one greater, it allows entry of an extra digit to force the value to it's maximum.  If we have a max of 99, and a countLimit of 2
        // if the field contains 40, it's impossible to type another number and have it jump to the max.  Since this behavior is present in other cases
        // e.g. max == 10, countlimit == 2, we adjust here to make it present in all.
        int countLimit = sAmountSlider.dontLimitTo99 ? 6 : 3;

        // this is a total hack copied from the options menu
        drawFrame( PIX3, R10, x, y, z, wd, ht, sc, clr|0xff, bkclr|0xff);

        font(&game_12);
        font_color( CLR_WHITE, CLR_WHITE );
        prnt(x+TEXT_OFFSET_X*sc, y+TEXT_OFFSET_Y*sc, z, sc, sc, textStd(sAmountSlider.text));

        curSel = doSlider( x + SLIDER_OFFSET_X*sc, y + SLIDER_OFFSET_Y*sc, z, SLIDER_W, sc, sc, curSel, 0, clr, 0);
        curSel += sAmountSlider.dontLimitTo99 ? 0.0001f : 0.01f;

        if ( isDown(MS_LEFT) )
        {
            newCurSel = ((int)(((curSel + 1.0f) / 2.0f) * (sAmountSlider.max - sAmountSlider.min)) + sAmountSlider.min);
            if ( INRANGE((int)newCurSel, 0, rangeLimit) )
            {
                itoa((int)newCurSel, text, 10);
                if (newCurSel >= 1000)
                {
                    text[5] = 0;
                    text[4] = text[3];
                    text[3] = text[2];
                    text[2] = text[1];
                    text[1] = ',';
                }
                uiEditSetUTF8Text(edit, text);
            }
        }
        
        if ( !edit )
        {
            // Is there any reason not to use uiEditCreateSimple(...) here?
            edit = uiEditCreate();
            edit->textColor = CLR_WHITE;
            uiEditSetFont(edit, &game_12);
            edit->limitType = UIECL_UTF8_ESCAPED;
            edit->disallowReturn = 1;
            edit->inputHandler = uiEditCommaNumericInputHandler;
            newCurSel = ((int)(((curSel + 1.0f) / 2.0f) * (sAmountSlider.max - sAmountSlider.min)) + sAmountSlider.min);
            if ( INRANGE((int)newCurSel, 0, rangeLimit) )
            {
                itoa((int)newCurSel, text, 10);
                uiEditSetUTF8Text(edit, text);
            }
        }
        edit->limitCount = countLimit;

        if ( sAmountSlider.initializing )
        {
            uiEditTakeFocus(edit);
            sAmountSlider.initializing = 0;
            window_setDims(WDW_AMOUNTSLIDER, gMouseInpCur.x - OK_BUTTON_OFFSET_X*sc, gMouseInpCur.y - OK_BUTTON_OFFSET_Y*sc, wd, ht);
        }

        box.left = x+INPUT_OFFSET_X*sc;
        box.top = y+(INPUT_OFFSET_Y)*sc;
        box.right = box.left + OK_BUTTON_W*sc;
        box.bottom = box.top + (INPUT_H)*sc;

        if (sAmountSlider.dontLimitTo99)
        {
            box.left -= 16 * sc;
        }

        drawFrameBox(PIX3, R10, &box, z, clr, bkclr);
        uiBoxFromCBox(&bounds, &box);
        uiBoxAlter(&bounds, UIBAT_SHRINK, UIBAD_LEFT, 8*sc);
        uiBoxAlter(&bounds, UIBAT_SHRINK, UIBAD_TOP, 3*sc);
        uiEditSetBounds(edit, bounds);
        edit->z = z;
        uiEditSetFont(edit, &game_12);
        uiEditSetFontScale(edit,sc);
        uiEditProcess(edit);
        editText = uiEditGetUTF8Text(edit);
        if ( editText )
        {
            char *srcp;
            char *dstp;
            char nocommaEditText[16];
            int oldCurSel;        // holds newCurSel before it's clamped.  This detects the clamp processs changing this value, which in turn forces a redraw

            for (srcp = editText, dstp = nocommaEditText; *dstp = *srcp; srcp++)
            {
                if (*dstp != ',')
                {
                    dstp++;
                }
            }

            if ( !strIsNumeric(nocommaEditText) )
            {
                newCurSel = sAmountSlider.max;
                oldCurSel = newCurSel - 1;    // Anything, as long as it's different - this forces a redraw on nonnumeric entry, because we've just jumped to the max value
            }
            else
            {
                newCurSel = atoi(nocommaEditText);
                oldCurSel = newCurSel;
                if (newCurSel > sAmountSlider.max)
                {
                    newCurSel = sAmountSlider.max;
                }
                else if ( newCurSel < sAmountSlider.min )
                {
                    newCurSel = sAmountSlider.min;
                }
                if (newCurSel >= 1000 && editText[4] == 0)
                {
                    // Special case handling growing from a three digit number to a four digit.  We force a redraw to get the comma inserted
                    oldCurSel = newCurSel - 1;    // Put in something different to force a redraw
                }
            }
            // Did the clamping of newCurSel change it, or do we need to redraw for some other reason?
            if (oldCurSel != newCurSel)
            {
                // If so, reformat the contents of the edit field
                itoa((int)newCurSel, text, 10);
                uiEditSetUTF8Text(edit, text);
                edit->cursor.characterIndex = newCurSel >= 1000 ? 4 : strlen(text);
                uiEditCommaNumericUpdate(edit);
            }
        }

        curSel = ((newCurSel - sAmountSlider.min) / (float) (sAmountSlider.max - sAmountSlider.min)) * 2.0f - 1.0f;

        //display_sprite(sAmountSlider.img, x + IMG_OFFSET_X*sc, y + IMG_OFFSET_Y*sc, z, sc, sc, CLR_WHITE);
        sAmountSlider.drawCallback(sAmountSlider.userData, x + IMG_OFFSET_X*sc, y + IMG_OFFSET_Y*sc, z, sc, CLR_WHITE);

        if ( isDown(MS_LEFT) ) 
        {
            CBox windowBox = {x, y-50, x+wd, y+ht};
            if( mouseCollision( &box ) )
                uiEditTakeFocus(edit);
            else if ( !mouseCollision(&windowBox) && !gWindowDragging && !draggingInThisWindow )
            { 
                uiEditReturnFocus(edit);
                window_setMode(WDW_AMOUNTSLIDER, WINDOW_SHRINKING);
            }
            else 
                draggingInThisWindow = 1;
        }
        else
            draggingInThisWindow = 0;

        if ( drawStdButton(x+OK_BUTTON_OFFSET_X*sc, y + OK_BUTTON_OFFSET_Y*sc, z, OK_BUTTON_W*sc, 25*sc, clr, "OkString", sc, 0) == D_MOUSEHIT ||
            ( uiEditHasFocus(edit) && inpEdge(INP_RETURN) ) )
        {
            if ( sAmountSlider.okCallback )
            {
                editText = uiEditGetUTF8Text(edit);
                if (editText)
                {
                    char *srcp1;

                    newCurSel = 0;
                    for (srcp1 = editText; *srcp1; srcp1++)
                    {
                        if (*srcp1 != ',')
                        {
                            newCurSel = newCurSel * 10 + *srcp1 - '0';
                        }
                    }
                }
                sAmountSlider.okCallback((int) newCurSel, sAmountSlider.userData);
            }
            uiEditReturnFocus(edit);
            window_setMode(WDW_AMOUNTSLIDER, WINDOW_SHRINKING);
        }
        else if ( !uiEditHasFocus(edit) )
            window_setMode(WDW_AMOUNTSLIDER, WINDOW_SHRINKING);
    } else {
        if ( edit && uiEditHasFocus(edit) )
            uiEditReturnFocus(edit);
    }

    return 0;
}