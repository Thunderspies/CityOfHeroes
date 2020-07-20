#include <utilitieslib/stdtypes.h>
#include "graphics/font.h"
#include "edit/edit_select.h"
#include "edit/edit_cmd.h"
#include "win/win_init.h"
#include <utilitieslib/utils/mathutil.h>
#include "win/input.h"
#include "edit_errcheck.h"
#include "render/renderUtil.h"
#include "edit/edit_net.h"
#include "UI/sprite/sprite_base.h"
#include "UI/uiInput.h"
#include <utilitieslib/utils/rgb_hsv.h>
#include "render/tex.h"
#include "graphics/textureatlas.h"
#include <utilitieslib/version/AppRegCache.h>
#include <utilitieslib/utils/RegistryReader.h>

#include "cmdparse/cmdgame.h"
#include "editorUI.h"

static int inBox(int x,int y,int px,int py,int width,int height)
{
    return (x >= px && x < px+width && y >= py && y < py+height);
}


static int setcolor = 1;

static U8 curr_color[4]={0,0,0,254};
static U8 original_color[4]={0,0,0,254};

// Colors as picked from the UI
static Vec3 uiHsv = {0,0,0};
static Vec4 uiRgba = {0,0,0};
static int isNegative = 0;

static char s_RgbText[3][4] = {"0", "0", "0"};
static char s_HsvText[3][4] = {"0", "0", "0"};

static int s_colorPickerID = -1;

static enum {
    COLORPICKER_NONE,
    COLORPICKER_ACTIVE,
    COLORPICKER_CANCEL,
    COLORPICKER_OK,
} s_colorPickerState = COLORPICKER_NONE;

void edit_colorPicker_close() {
    if (s_colorPickerID >= 0) {
        float x, y;
        RegReader rr = createRegReader();
        initRegReader(rr, regGetAppKey());
        editorUIGetPosition(s_colorPickerID, &x, &y);
        rrWriteInt(rr, "ColorPickerX", x);
        rrWriteInt(rr, "ColorPickerY", y);
        destroyRegReader(rr);

        s_colorPickerState = COLORPICKER_NONE;
        editorUIDestroyWindow(s_colorPickerID);
        s_colorPickerID = -1;
    }
}

void getPickedColor(Vec3 rgbOut)
{
    scaleVec3(curr_color, 1./255., rgbOut);
}

static void pickColorAbort()
{
    setcolor = 1;
}

static void onColorPickerOK(char* text) {
    edit_colorPicker_close();
    s_colorPickerState = COLORPICKER_OK;
}

static void onColorPickerCancel(char* text) {
    edit_colorPicker_close();
    s_colorPickerState = COLORPICKER_CANCEL;
}

static void updateHsvFromRgb(void) {
    Vec3 rgbf;    //for using conversion functions
    rgbf[0]=(double)curr_color[0]/255.0;
    rgbf[1]=(double)curr_color[1]/255.0;
    rgbf[2]=(double)curr_color[2]/255.0;
    rgbToHsv(rgbf,uiHsv);
    uiHsv[0] = round(uiHsv[0]);
    uiHsv[1] = round(100 * uiHsv[1]);
    uiHsv[2] = round(100 * uiHsv[2]);
}

static void updateRgbFromHsv(void) {
    Vec3 rgbf;    //for using conversion functions
    Vec3 hsvNormalized;
    hsvNormalized[0] = uiHsv[0];
    hsvNormalized[1] = uiHsv[1] / 100;
    hsvNormalized[2] = uiHsv[2] / 100;
    hsvToRgb(hsvNormalized, rgbf);
    uiRgba[0] = curr_color[0] = round(rgbf[0]*255);
    uiRgba[1] = curr_color[1] = round(rgbf[1]*255);
    uiRgba[2] = curr_color[2] = round(rgbf[2]*255);
}

static void onRgbChanged(void* userData) {
    curr_color[0] = round(uiRgba[0]);
    curr_color[1] = round(uiRgba[1]);
    curr_color[2] = round(uiRgba[2]);

    if (editorUIIsUserInteracting((int)userData))
        updateHsvFromRgb();
}

static void onHsvChanged(void* userData) {
    if (editorUIIsUserInteracting((int)userData))
        updateRgbFromHsv();
}

static void onAlphaChanged(void* userData) {
    curr_color[3] = round(uiRgba[3]);
}

static void onNegativeChanged(void* userData) {
    curr_color[3] = isNegative ? 0xff : 0xfe;
}

static bool createColorPicker(int* initialColor, int alpha) {
    int id;
    int x = 0, y = 0;
    RegReader rr;

    s_colorPickerID = editorUICreateWindow("Pick Color");

    if (s_colorPickerID < 0)
        return false;

    rr = createRegReader();
    initRegReader(rr, regGetAppKey());
    if (!rrReadInt(rr, "ColorPickerX", &x))
        x = 0;
    if (!rrReadInt(rr, "ColorPickerY", &y))
        y = 0;
    destroyRegReader(rr);

    editorUISetPosition(s_colorPickerID, x, y);

    editorUISetModal(s_colorPickerID, 1);
    if (initialColor) {
        int i;
        for (i = 0; i < 3; ++i) {
            uiRgba[i] = curr_color[i] = original_color[i] = 
                (*initialColor >> ((2 - i) * 8)) & 0xff;
        }
    }

    if (alpha)
        original_color[3] = (*initialColor >> 24) & 0xff;
    else
        original_color[3] = 0xfe;
    
    uiRgba[3] = curr_color[3] = original_color[3];

    updateHsvFromRgb();

    editorUIAddDualColorSwatch(s_colorPickerID, original_color, curr_color);
    id = editorUIAddSaturationValueSelector(s_colorPickerID, uiHsv, onHsvChanged);
    editorUISetWidgetCallbackParam(id, (void*)id);
    id = editorUIAddHueSelector(s_colorPickerID, uiHsv, onHsvChanged);
    editorUISetWidgetCallbackParam(id, (void*)id);

    id = editorUIAddEditSlider(s_colorPickerID, uiRgba, 0, 255, 0, 255, 1,
        onRgbChanged, "Red:");
    editorUISetWidgetCallbackParam(id, (void*)id);
    id = editorUIAddEditSlider(s_colorPickerID, uiRgba + 1, 0, 255, 0, 255, 1,
        onRgbChanged, "Green:");
    editorUISetWidgetCallbackParam(id, (void*)id);
    id = editorUIAddEditSlider(s_colorPickerID, uiRgba + 2, 0, 255, 0, 255, 1,
        onRgbChanged, "Blue:");
    editorUISetWidgetCallbackParam(id, (void*)id);

    id = editorUIAddEditSlider(s_colorPickerID, uiHsv, 0, 360, 0, 360, 1,
        onHsvChanged, "Hue (deg):");
    editorUISetWidgetCallbackParam(id, (void*)id);
    id = editorUIAddEditSlider(s_colorPickerID, uiHsv + 1, 0, 100, 0, 100, 1,
        onHsvChanged, "Saturation (%):");
    editorUISetWidgetCallbackParam(id, (void*)id);
    id = editorUIAddEditSlider(s_colorPickerID, uiHsv + 2, 0, 100, 0, 100, 1,
        onHsvChanged, "Value (%):");
    editorUISetWidgetCallbackParam(id, (void*)id);

    if (alpha) {
        editorUIAddEditSlider(s_colorPickerID, uiRgba + 3, 0, 255, 0, 255, 1,
            onAlphaChanged, "Alpha:");
    } else {
        editorUIAddCheckBox(s_colorPickerID, &isNegative, onNegativeChanged, "Negative");
    }

    editorUIAddCheckBox(s_colorPickerID, &game_state.fullRelight, NULL, "Full relight each frame");
    editorUIAddButtonRow(s_colorPickerID, NULL,
                         "OK", onColorPickerOK,
                         "Cancel", onColorPickerCancel,
                         NULL);
    s_colorPickerState = COLORPICKER_ACTIVE;
    return true;
}

bool pickFromFrameBuffer() {
    if (GetAsyncKeyState(VK_SHIFT) & 0x8000000) {
        // While holding shift, grab the color from anywhere
        HDC hdc;
        hdc = GetDC(NULL); // Get the DrawContext of the desktop
        if (hdc) {
            COLORREF cr;
            POINT pCursor;
            U8 alpha=curr_color[3];
            GetCursorPos(&pCursor);
            cr = GetPixel(hdc, pCursor.x, pCursor.y);
            ReleaseDC(NULL, hdc);
            *(int*)curr_color = cr;
            curr_color[3] = alpha;
            return true;
        }
    }

    return false;
}

int pickColorNew(int * color, int alpha) {
    if (!sel_count || edit_state.unsel)
        onColorPickerCancel(NULL);

    if (s_colorPickerState == COLORPICKER_OK) {
        s_colorPickerState = COLORPICKER_NONE;
        return 1;
    }

    if (s_colorPickerState == COLORPICKER_CANCEL) {
        *color = (original_color[0] << 16) | (original_color[1] << 8) | (original_color[2]) | (original_color[3] << 24);
        s_colorPickerState = COLORPICKER_NONE;
        return -2;
    }

    if (s_colorPickerID < 0) {
        if (!createColorPicker(color, alpha))
            return -2;
    }

    if (pickFromFrameBuffer()) {
        uiRgba[0] = curr_color[0];
        uiRgba[1] = curr_color[1];
        uiRgba[2] = curr_color[2];
        updateHsvFromRgb();
    }

    *color = (curr_color[0] << 16) | (curr_color[1] << 8) | (curr_color[2]) | (curr_color[3] << 24);

    edit_state.sel = 0;
    return 0;
}

// return vals:
// 0: not done picking color yet
// 1: got it
// -1: delete color
// -2: cancel and keep previous color


//alpha=0 if you don't want an alpha slider, non-zero if you do
int pickColor(int *color,int alpha)
{
    #define SWATCHSIZE 128
    static U8 do_color,cancel,ok,negative;
    static char color_msg[100],bright_msg[20],hsv_msg[100],neg_msg[100];
    static U8 hsv[3];
    //regionSelected is -1 if no item is selected, otherwise
    //regions[regionSelected] is the region that the user has the mouse
    //down on, used for the sliders and the color palette
    static int regionSelected=-1;
    static int mouseDownLastRun=0;
    static int mouseDownThisFrame=0;
    U8    final_color[4];
    int    i,x,y,w,h,c,copycolor,exitval=0;
    int    px = 25,py,imgx=128,imgy=128,no_ambient=0;
    int spriteColor;    //color stored in different order for sprite functions
    int oldColor=*color|(alpha?0x00:0xff000000);
    static color_font_id;
    static char increment[4]=" + ";
    static char decrement[4]=" - ";
    Vec3    hsvf,rgb;    //for using conversion functions
    static int originalColor;
    static int originalColorSet=0;
    static int lastUpdatedColor;

    //regions[] holds the regions that the parts of the color picker occupy
    //i.e. the color swatch, the menu items, etc...
    static struct
    {
        int x,y,w,h;
        U8 *val_ptr;
        char *message;
        int allowsSliding;//if sliding is allowed, mousedown is treated like mouseclick
    } regions[] =
    {//    x                y                width            height        val_ptr            message        sliding?    
        {0                ,0                ,SWATCHSIZE        ,SWATCHSIZE    ,&do_color        ,0            ,1},//color palette
        {SWATCHSIZE        ,124            ,0                ,8            ,&cancel        ,"CANCEL"    ,0},//cancel button
        {SWATCHSIZE        ,98                ,0                ,8            ,&ok            ," OK "        ,0},//ok button
        {SWATCHSIZE        ,90                ,0                ,8            ,&negative        ,"NEGATIVE"    ,0},//negative button
        {SWATCHSIZE+100    ,90                ,0                ,8            ,0                ,neg_msg    ,0},//negative readout
        {SWATCHSIZE        ,82                ,0                ,8            ,0                ,color_msg    ,0},//RGB value readout
        {SWATCHSIZE        ,0                ,SWATCHSIZE-25    ,16            ,&curr_color[0]    ,0            ,1},//Red slider
        {SWATCHSIZE        ,16                ,SWATCHSIZE-25    ,16            ,&curr_color[1]    ,0            ,1},//Green slider
        {SWATCHSIZE        ,32                ,SWATCHSIZE-25    ,16            ,&curr_color[2]    ,0            ,1},//Blue slider
        {SWATCHSIZE        ,48                ,SWATCHSIZE-25    ,16            ,&curr_color[3]    ,0            ,1},//Alpha slider
        {SWATCHSIZE*2    ,0                ,SWATCHSIZE-25    ,16            ,&hsv[0]        ,0            ,1},//Hue slider
        {SWATCHSIZE*2    ,16                ,SWATCHSIZE-25    ,16            ,&hsv[1]        ,0            ,1},//Saturation slider
        {SWATCHSIZE*2    ,32                ,SWATCHSIZE-25    ,16            ,&hsv[2]        ,0            ,1},//Value slider
        {SWATCHSIZE*2-24,0                ,24                ,8            ,&curr_color[0]    ,increment    ,0},//Red increment
        {SWATCHSIZE*2-24,8                ,24                ,8            ,&curr_color[0]    ,decrement    ,0},//Red decrement
        {SWATCHSIZE*2-24,16                ,24                ,8            ,&curr_color[1]    ,increment    ,0},//Green increment
        {SWATCHSIZE*2-24,24                ,24                ,8            ,&curr_color[1]    ,decrement    ,0},//Green decrement
        {SWATCHSIZE*2-24,32                ,24                ,8            ,&curr_color[2]    ,increment    ,0},//Blue increment
        {SWATCHSIZE*2-24,40                ,24                ,8            ,&curr_color[2]    ,decrement    ,0},//Blue decrement
        {SWATCHSIZE*2-24,48                ,24                ,8            ,&curr_color[3]    ,increment    ,0},//Alpha increment
        {SWATCHSIZE*2-24,56                ,24                ,8            ,&curr_color[3]    ,decrement    ,0},//Alpha decrement
        {SWATCHSIZE*2    ,82                ,0                ,8            ,0                ,hsv_msg    ,0},//HSV value readout
    };

    if (game_state.useNewColorPicker)
        return pickColorNew(color, alpha);

    mouseDownLastRun=mouseDownThisFrame;
    mouseDownThisFrame=isDown(MS_LEFT);
    if (alpha) {
        sprintf(neg_msg,"Color cannot be negative, you're using alpha.");
        negative=0;
    } else {
        if (curr_color[3]&1) {
            sprintf(neg_msg,"Color is Negative");
            negative=1;
        } else {
            sprintf(neg_msg,"Color is NOT Negative");
            negative=0;
        }
    }

    if (alpha!=0) alpha=1;
    else curr_color[3]=0xfe;
    if (!sel_count)
    {
        exitval = -2;
        goto done;
    }
    copycolor = setcolor & edit_state.colormemory;
    do_color=cancel=ok = 0;
    if (edit_state.colorbynumber)
    {
        static char namebuf[TEXT_DIALOG_MAX_STRLEN];
        char    buf[200],*s, *delim = " \t,";
        int        divisor = 1;
        if( !winGetString("Enter color numbers",namebuf) )
        {
            exitval = -2;
            goto done;
        }
        s = strrchr(namebuf,'\\');
        if (!s++)
            s = namebuf;
        strcpy(buf,s);
        s = strtok(buf,delim);
        if (s)
            curr_color[0] = atoi(s) / divisor;
        s = strtok(0,delim);
        if (s)
            curr_color[1] = atoi(s) / divisor;
        s = strtok(0,delim);
        if (s)
            curr_color[2] = atoi(s) / divisor;
        ok = 1;
        goto textpicked;
    }
    if (copycolor)
    {
        for(i=0;i<3;i++)
        {
            c = (((*color) >> (i*8)) & 255);
            curr_color[2-i] = c;
        }
    }
//    py = fontLocY(py);
    inpMousePos(&x,&y);
    windowSize(&w,&h);
    py=h-SWATCHSIZE-25;
    fontSet(0);
    fontDefaults();


    if (!isDown(MS_LEFT))
        regionSelected=-1;
    else
        printf("");

    //this loop runs for each region, prints appropriate text,
    //and handles any mouse events (does not draw the images)
    for(i=0;i<ARRAY_SIZE(regions);i++)
    {
        if (regions[i].val_ptr==&curr_color[3] && alpha==0) continue;
        //make the width of the region appropriate for the message contained therein
        //it is silly to calculate this every single time this function is run,
        //considering that the message never changes length
        if (regions[i].message)
            regions[i].w = strlen(regions[i].message) * 8;


        //deselects a region if the mouse button is no longer held down

        if (((i==0 || i>=4)  && inBox(x,y,px+regions[i].x,py+regions[i].y,regions[i].w,regions[i].h)) ||
            (i>=1 && i<=3 && inBox(x,y,px+regions[i].x,py+regions[i].y+alpha*16,regions[i].w,regions[i].h)))
        {
            //edit_state.sel must mean mouseclick, 
            //maybe in the future make this a mouse down event instead
            if (edit_state.sel)
            {
                if (regions[i].message==increment)
                    (*regions[i].val_ptr)++;
                else if (regions[i].message==decrement)
                    (*regions[i].val_ptr)--;
                else if (regions[i].val_ptr && regions[i].message!=color_msg)
                    (*regions[i].val_ptr)^=1;

                //if a sliding region is selected, note it, so that
                //future mouse locations will also affect the region
                if (regions[i].allowsSliding) {
                    regionSelected=i;
                }
            }
        }

        //handle sliding regions
        if (i==regionSelected) {
            int curx,cury;
            curx=x;
            if (curx<regions[i].x+px) curx=regions[i].x+px;
            if (curx>=regions[i].x+px+regions[i].w) 
                curx=regions[i].x+px+regions[i].w-1;
            cury=y;
            if (cury<regions[i].y+py) cury=regions[i].y+py;
            if (cury>=regions[i].y+py+regions[i].h) 
                cury=regions[i].y+py+regions[i].h-1;
            if (regions[i].val_ptr==&do_color){
                rdrGetPixelColor(curx, h-cury, curr_color);
            } else
                (*regions[i].val_ptr)=((curx-regions[i].x-px)*255)/(regions[i].w-1);
        }

        if (regions[i].allowsSliding) {
            int startColor,endColor;
            startColor=    (curr_color[0]<<24)+
                        (curr_color[1]<<16)+
                        (curr_color[2]<<8)+
                        (0xff);
            endColor=    (curr_color[0]<<24)+
                        (curr_color[1]<<16)+
                        (curr_color[2]<<8)+
                        (0xff);
            if (regions[i].val_ptr==&curr_color[0]) {
                startColor&=0x00ffffff;
                endColor  |=0xff000000;
            }
            if (regions[i].val_ptr==&curr_color[1]) {
                startColor&=0xff00ffff;
                endColor  |=0x00ff0000;
            }
            if (regions[i].val_ptr==&curr_color[2]) {
                startColor&=0xffff00ff;
                endColor  |=0x0000ff00;
            }
            if (regions[i].val_ptr==&curr_color[3]) {
                startColor&=0xffffff00;
                endColor  |=0x000000ff;
            }
            if (regions[i].val_ptr>=hsv && regions[i].val_ptr<=hsv+2) {
                int dex=regions[i].val_ptr-hsv;
                hsvf[0]=((double)hsv[0]/255.0)*360.0;
                hsvf[1]=(double)hsv[1]/255;
                hsvf[2]=(double)hsv[2]/255;
                hsvf[dex]=0;
                hsvToRgb(hsvf,rgb);
                startColor=    (((int)(rgb[0]*255))<<24)+
                            (((int)(rgb[1]*255))<<16)+
                            (((int)(rgb[2]*255))<<8)+
                            (0xff);
                hsvf[dex]=1;
                if (dex==0)
                    hsvf[dex]=360;
                hsvToRgb(hsvf,rgb);
                endColor=    (((int)(rgb[0]*255))<<24)+
                            (((int)(rgb[1]*255))<<16)+
                            (((int)(rgb[2]*255))<<8)+
                            (0xff);
            }
            if (regions[i].val_ptr==hsv) {
                double increment=(double)regions[i].w/6.0;
                int curx=regions[i].x;
                int j;
                hsvf[1]=(double)hsv[1]/255.0;
                hsvf[2]=(double)hsv[2]/255.0;
                for (j=0;j<6;j++) {
                    hsvf[0]=60*j;
                    hsvToRgb(hsvf,rgb);
                    startColor=    (((int)(rgb[0]*255))<<24)+
                                (((int)(rgb[1]*255))<<16)+
                                (((int)(rgb[2]*255))<<8)+
                                (0xff);
                    hsvf[0]=60*(j+1);
                    hsvToRgb(hsvf,rgb);
                    endColor=    (((int)(rgb[0]*255))<<24)+
                                (((int)(rgb[1]*255))<<16)+
                                (((int)(rgb[2]*255))<<8)+
                                (0xff);
                    display_sprite_UV_4Color(white_tex_atlas,(px+regions[i].x+((double)j*increment)),py+regions[i].y,2,
                                            (double)regions[i].w/48,(double)regions[i].h/8,
                                            startColor,endColor,endColor,startColor,0,0,1,1);
                }
                display_sprite(white_tex_atlas,
                                            px+regions[i].x+((*regions[i].val_ptr)*regions[i].w)/255-1,
                                            py+regions[i].y,
                                            3,
                                            .125,
                                            2,
                                            0xffffffff);
                display_sprite(white_tex_atlas,
                                            px+regions[i].x+((*regions[i].val_ptr)*regions[i].w)/255+1,
                                            py+regions[i].y,
                                            3,
                                            .125,
                                            2,
                                            0xffffffff);
            } else {
                display_sprite_UV_4Color(white_tex_atlas,px+regions[i].x,py+regions[i].y,2,(double)regions[i].w/8,(double)regions[i].h/8,
                                            startColor,endColor,endColor,startColor, 0,0,1,1);
                display_sprite(white_tex_atlas,
                                            px+regions[i].x+((*regions[i].val_ptr)*regions[i].w)/255-1,
                                            py+regions[i].y,
                                            3,
                                            .125,
                                            2,
                                            0xffffffff);
                display_sprite(white_tex_atlas,
                                            px+regions[i].x+((*regions[i].val_ptr)*regions[i].w)/255+1,
                                            py+regions[i].y,
                                            3,
                                            .125,
                                            2,
                                            0xffffffff);
            }
        }

        pickFromFrameBuffer();

        if (regions[i].message)
        {
            int offset=alpha*16;
            if (regions[i].message==increment || regions[i].message==decrement)
                    offset=0;
            fontColor(0);
            fontAlpha(140);
            fontScale(8*strlen(regions[i].message),8);
            fontText(px+regions[i].x,py+regions[i].y+offset,"\001");
            fontScale(1,1);
            fontAlpha(255);
            if (((i==0 || i>=4)  && inBox(x,y,px+regions[i].x,py+regions[i].y,regions[i].w,regions[i].h)) ||
            (i>=1 && i<=3 && inBox(x,y,px+regions[i].x,py+regions[i].y+alpha*16,regions[i].w,regions[i].h)))
                fontColor(0x00ff00);
            else
                fontColor(0xffffff);
            fontText(px+regions[i].x,py+regions[i].y+offset,regions[i].message);
        }    
    }
    if (regions[regionSelected].val_ptr>=hsv && regions[regionSelected].val_ptr<=hsv+2) {
        hsvf[0]=((double)hsv[0]/255.0)*360.0;
        hsvf[1]=(double)hsv[1]/255.0;
        hsvf[2]=(double)hsv[2]/255.0;
        hsvToRgb(hsvf,rgb);
        curr_color[0]=rgb[0]*255;
        curr_color[1]=rgb[1]*255;
        curr_color[2]=rgb[2]*255;
    } else {
        rgb[0]=(double)curr_color[0]/255.0;
        rgb[1]=(double)curr_color[1]/255.0;
        rgb[2]=(double)curr_color[2]/255.0;
        rgbToHsv(rgb,hsvf);
        hsv[0]=hsvf[0]*(255.0/360.0);
        hsv[1]=hsvf[1]*255;
        hsv[2]=hsvf[2]*255;
    }
    edit_state.sel = 0;

textpicked:

    if (negative)
        curr_color[3]|=1;
    else
        curr_color[3]&=~1;
    for(i=0;i<4;i++)
        final_color[i] = curr_color[i];
    if (!color_font_id)
        color_font_id = fontLoadImage("colorpicker_edit");
    if (!color_font_id)
        return 0;
    fontSet(color_font_id);
    fontDefaults();
    fontScale(1.f,1.f);
    fontText(px,py,"X");
    fontSet(0);

    if (alpha) {
        sprintf(color_msg,"RGBA: %d %d %d %d",(unsigned int)final_color[0],(unsigned int)final_color[1],(unsigned int)final_color[2],(unsigned int)final_color[3]);
        *color = ((final_color[0] >> 0) << 16) | ((final_color[1] >> 0) << 8) | (final_color[2] >> 0) | (final_color[3] << 24);
        spriteColor=(final_color[0] << 24) | (final_color[1] << 16) | (final_color[2] << 8) | (final_color[3] << 0);
    } else {
        sprintf(color_msg,"RGB: %d %d %d",(unsigned int)final_color[0],(unsigned int)final_color[1],(unsigned int)final_color[2]);
        *color = ((final_color[0] >> 0) << 16) | ((final_color[1] >> 0) << 8) | (final_color[2] >> 0) | (final_color[3] << 24);
        spriteColor=(final_color[0] << 24) | (final_color[1] << 16) | (final_color[2] << 8) | (final_color[3] << 0);
    }
    sprintf(hsv_msg,"HSV: %d %d %d",(unsigned int)hsv[0],(unsigned int)hsv[1],(unsigned int)hsv[2]);
    if (!originalColorSet) {
        originalColorSet=1;
        originalColor=*color;
    }
/*    fontScale(imgx,29+alpha*16);
    fontColor(*color);

    fontText(px+SWATCHSIZE,py+48+5+alpha*16,"\001");
    fontDefaults();

    fontScale(imgx,5);
    fontColor(0);
    fontText(px+SWATCHSIZE,py+48+alpha*16,"\001");
    fontDefaults();
*/
    display_sprite(white_tex_atlas,
                                px+SWATCHSIZE,
                                py+48+alpha*16,
                                2,
                                (double)(imgx-24)/16.0,
                                29.0/8,
                                0xff);
    display_sprite(white_tex_atlas,
                                px+SWATCHSIZE+imgx/2-12,
                                py+48+alpha*16,
                                2,
                                (double)(imgx-24)/16.0,
                                29.0/8,
                                0xffffffff);
    display_sprite(white_tex_atlas,
                                px+SWATCHSIZE,
                                py+48+alpha*16,
                                3,
                                (double)(imgx-24)/8.0,
                                29.0/8,
                                spriteColor);

    spriteColor=0;
    oldColor=originalColor;
    spriteColor|=((oldColor>>24)&0xff)<< 0;
    spriteColor|=((oldColor>> 8)&0xff)<<16;
    spriteColor|=((oldColor>>16)&0xff)<<24;
    spriteColor|=((oldColor>> 0)&0xff)<< 8;
    display_sprite(white_tex_atlas,
                                px+SWATCHSIZE*2,
                                py+48+alpha*16,
                                2,
                                (double)(imgx-24)/16.0,
                                29.0/8,
                                0xff);
    display_sprite(white_tex_atlas,
                                px+SWATCHSIZE*2+imgx/2-12,
                                py+48+alpha*16,
                                2,
                                (double)(imgx-24)/16.0,
                                29.0/8,
                                0xffffffff);
    display_sprite(white_tex_atlas,
                                px+SWATCHSIZE*2,
                                py+48+alpha*16,
                                3,
                                (double)(imgx-24)/8.0,
                                29.0/8,
                                spriteColor);


    if (cancel || edit_state.unsel) {
        exitval = 1;
        *color=originalColor;
        originalColorSet=0;
    } else if (ok) {
        exitval = 1;
        originalColorSet=0;
    }

done:
    if (!exitval)
        setcolor = 0;
    else
        setcolor = 1;

/*
    fontScale(1,1);
    fontAlpha(200);
    fontText(px+50,py+50,"|[])IooO");
*/
    edit_state.changedColor=0;
    if (exitval==0) {
        if (mouseDownLastRun && !mouseDownThisFrame && originalColor!=*color && lastUpdatedColor!=*color) {
            lastUpdatedColor=*color;
            edit_state.changedColor=1;
        }
        return 0;
    } else
        return exitval;
}


// returns:
// 0 if still deciding color
// 1 if picked
int get2Color(int idx,int is_tint,int *memp)
{
    static int            rgb;
    GroupDef    *def;
    int            i,done,set_val = 1;
    static int initialUpdate;
    DefTracker *lightRef;

    if (!edit_state.lightHandle && !isNonLibSingleSelect(memp,0)) {
        *memp=0;
        initialUpdate=0;
        return 1;
    }

    if (!edit_state.lightHandle && !sel_count) {
        *memp=0;
        initialUpdate=0;
        return 1;
    }
    if (!initialUpdate)
    {
        if (!edit_state.editorig)
            editUpdateTracker(sel_list[0].def_tracker);
        initialUpdate=1;
    }
    if (!edit_state.lightHandle) {
        def = sel_list[0].def_tracker->def;
        rgb = (def->color[idx][0] << 16) | (def->color[idx][1] << 8)
                | (def->color[idx][2] << 0) | (def->color[idx][3] << 24);
    }
    if (sel_count==1 && !edit_state.lightHandle) {
        edit_state.lightHandle = trackerHandleCreate(sel_list[0].def_tracker);
        if (edit_state.lightHandle)
        {
            lightRef = groupRefFind(edit_state.lightHandle->ref_id);
            if(lightRef->tricks)
                lightRef->tricks->flags2 &= ~TRICK2_WIREFRAME;
        }
    }
    if (edit_state.lightHandle && !editIsWaitingForServer() &&sel_count==0)
        selAdd(trackerFromHandle(edit_state.lightHandle), edit_state.lightHandle->ref_id, 1, EDITOR_LIST);

    done = pickColor(&rgb,0);
//    if (!done && !edit_state.changedColor)
//        return 0;

    editSelSort();
//    if (done == -2) {
//        *memp=0;
//        return 1;
//    }
    if (edit_state.lightHandle)
    {
        lightRef = groupRefFind(edit_state.lightHandle->ref_id);
        if(lightRef->tricks)
            lightRef->tricks->flags2 &= ~TRICK2_WIREFRAME;
    }
    if (done == -1)
        set_val = 0;

    lightRef = edit_state.lightHandle ? groupRefFind(edit_state.lightHandle->ref_id) : NULL;
    for (i=0;i<sel_count;i++)
    {
        def = sel_list[i].def_tracker->def;
        def->color[idx][0] = (rgb >> 16)    & 255;
        def->color[idx][1] = (rgb >> 8)    & 255;
        def->color[idx][2] = (rgb >> 0)    & 255;
        def->color[idx][3] = (rgb >> 24)    & 255;
        if (lightRef->tricks)
        {
            lightRef->tricks->trick_rgba[0]=def->color[0][0];
            lightRef->tricks->trick_rgba[1]=def->color[0][1];
            lightRef->tricks->trick_rgba[2]=def->color[0][2];
            lightRef->tricks->trick_rgba[3]=def->color[0][3];
            lightRef->tricks->trick_rgba2[0]=def->color[1][0];
            lightRef->tricks->trick_rgba2[1]=def->color[1][1];
            lightRef->tricks->trick_rgba2[2]=def->color[1][2];
            lightRef->tricks->trick_rgba2[3]=def->color[1][3];
        }
        if (!set_val)
            memset(def->color,0,sizeof(def->color));

        if (is_tint)
            def->has_tint_color = set_val;
        else
        {
            def->has_fog = set_val;
            if (edit_state.fogcolor1)
                memcpy(def->color[1],def->color[0],sizeof(def->color[0]));
        }
        if (done) {
            editUpdateTracker(sel_list[i].def_tracker);
            unSelect(2);
        }
    }
    {
        if (done)
        {
            *memp=0;
            trackerHandleDestroy(edit_state.lightHandle);
            edit_state.lightHandle = NULL;
            initialUpdate=0;
        }
    }
    return !edit_state.changedColor;
}

