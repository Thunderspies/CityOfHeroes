#ifndef TTFONT_H
#define    TTFONT_H

#include "graphics/truetype/ttFontDraw.h"
#include "graphics/truetype/ttFontManager.h"

extern TTFontManager* fontManager;


//------------------------------------------------------------------------------
// TrueType test functions
//------------------------------------------------------------------------------
void ttLoad();
void ttDrawTest();
void ttFontDebug();

#endif