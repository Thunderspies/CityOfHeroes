// This file now only contains TTFont test functions.
// Todo - Make more useful test functions!

#include "graphics/ttFont.h"
#include "win/input.h"
#include "render/tex.h"
#include "graphics/ttFontUtil.h"

TTFontManager* fontManager;
TTDrawContext* drawContext;

#if 0
#include "ogl_h"

void ttDrawTest(){
    static unsigned short str[128] = L"";
    static unsigned short strlength = 0;
    KeyInput* input;

    input = inpGetKeyBuf();
    while(input){
        if(input->type == KIT_EditKey && input->key == INP_BACKSPACE){
            strlength = 0;
            inpGetNextKey(&input);
            continue;
        }
        if(input->type != KIT_EditKey){
            if(strlength < 128 - 1)
                str[strlength++] = input->unicodeCharacter;
        }
        inpGetNextKey(&input);
    }

    ttDrawText2D(drawContext, 0.0, 0.0, str, strlength);

    // Draw a red square for as a size reference.
    {
        static unsigned short character = 'a';

#define texWidth 64
#define texHeight 64
        unsigned int testTexture[texWidth * texHeight];
        int i;
        unsigned int* dstRowBuffer;
        static int texture;

        if(!testTexture) {
            glGenTextures(1, testTexture); CHECKGL;
        }

        dstRowBuffer = testTexture;
        memset(testTexture, 128, texWidth * texHeight * 4);
        for(i = 0; i < 64 * 64; i++){
            testTexture[i] = 0;
            testTexture[i] |= 255 << 24;
            testTexture[i] |= 0 << 16;
            testTexture[i] |= 0 << 8;
            testTexture[i] |= 128 << 0;
        }        

        WCW_BindTexture(GL_TEXTURE_2D, 0, texture); CHECKGL;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, testTexture); CHECKGL;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); CHECKGL;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); CHECKGL;

        texSetWhite(TEXLAYER_BASE);

        glBegin(GL_QUADS);  
        glTexCoord2f(0, 0); 
        glVertex3i(0, 0, -50);

        glTexCoord2f(0, 1);
        glVertex3i(0, 64, -50);

        glTexCoord2f(1, 1);
        glVertex3i(64, 64, -50); 

        glTexCoord2f(1, 0);
        glVertex3i(64, 0, -50);
        glEnd(); CHECKGL;
    }
}
#endif

static TTDrawContext* ttGetTestFont(){
    static TTDrawContext* drawContext = NULL;

    if(!fontManager)
        return NULL;

    if(!drawContext){
        TTCompositeFont* font = createTTCompositeFont();
        drawContext = createTTDrawContext();
        initTTDrawContext(drawContext , 11);
        ttFontAddFallback(font, ttFontLoadCached("fonts/tahoma.ttf", 1));
        ttFMAddFont(fontManager, font);

        drawContext->font = font;
        drawContext->renderParams.dropShadow = 1;
        drawContext->renderParams.dropShadowXOffset = 1;
        drawContext->renderParams.dropShadowYOffset = 1;
    }

    return drawContext;
}
