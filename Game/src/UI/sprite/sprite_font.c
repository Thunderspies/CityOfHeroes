/***************************************************************************
 *     Copyright (c) 2000-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include "graphics/ttFont.h"
#include "graphics/ttFontUtil.h"

#include "UI/sprite/sprite_font.h"

#include <utilitieslib/assert/assert.h>

#include <utilitieslib/utils/file.h>
#include "cmdparse/cmdgame.h"

#include <utilitieslib/language/AppLocale.h>

/*****************************************************
 * True type font stuff
 */
int ttDrawContextInitialized = 0;

TTDrawContext tinyfont;

TTDrawContext hybrid_9;
TTDrawContext hybrid_9_dynamic;
TTDrawContext hybrid_12;
TTDrawContext hybridbold_12;
TTDrawContext hybrid_14;
TTDrawContext hybridbold_14;
TTDrawContext hybrid_18;
TTDrawContext hybridbold_18;

TTDrawContext game_9;
TTDrawContext game_9_dynamic;
TTDrawContext game_10;
TTDrawContext game_10_dynamic;
TTDrawContext game_12; 
TTDrawContext game_12_dynamic; 
TTDrawContext game_14;
TTDrawContext game_18;

TTDrawContext gamebold_9;
TTDrawContext gamebold_12;
TTDrawContext gamebold_14;
TTDrawContext gamebold_18;

TTDrawContext title_9;
TTDrawContext title_12;
TTDrawContext title_12_no_outline;
TTDrawContext title_14;
TTDrawContext title_18;
TTDrawContext title_18_thick;
TTDrawContext title_24;

TTDrawContext chatThin;
TTDrawContext chatFont;
TTDrawContext chatBold;

TTDrawContext smfLarge;
TTDrawContext smfDefault;
TTDrawContext smfSmall;
TTDrawContext smfVerySmall;
TTDrawContext smfMono;
TTDrawContext smfComputer;

TTDrawContext editorFont;

TTDrawContext profileFont;
TTDrawContext profileNameFont;


TTDrawContext entityNameText;
TTDrawContext referenceFont; // For kerning tests.

// Base fonts
TTSimpleFont* ttRedCircle;
TTSimpleFont* ttMontBold;
TTSimpleFont* ttMont;
TTSimpleFont* ttVerdana;
TTSimpleFont* ttVerdanab;
TTSimpleFont* ttMingLiu;
TTSimpleFont* ttGulim;
TTSimpleFont* ttDotum;
TTSimpleFont* ttAsiaPFB;
TTSimpleFont* ttYGO230;
TTSimpleFont* ttTahomaBD;
TTSimpleFont* ttTahoma;
TTSimpleFont* ttCourier;
TTSimpleFont* ttDigitalb;
TTSimpleFont* ttTiny;
TTSimpleFont* ttGothic;
TTSimpleFont* ttGothicB;

// Composite fonts
TTCompositeFont* tinyTextFont;
TTCompositeFont* gameTextFont;
TTCompositeFont* smfProfileNameFont;
TTCompositeFont* gameBoldFont;
TTCompositeFont* titleTextFont;
TTCompositeFont* chatThinFont;
TTCompositeFont* chatTextFont;
TTCompositeFont* chatBoldFont;
TTCompositeFont* referenceTextFont;
TTCompositeFont* smfMonoFont;
TTCompositeFont* smfComputerFont;
TTCompositeFont* smfLargeFont;
TTCompositeFont* smfSmallFont;
TTCompositeFont* editorCFont;
TTCompositeFont* gameHybrid;
TTCompositeFont* gameHybridBold;

/*
 * True type font stuff
 *****************************************************/

void loadFonts()
{
    TTDrawContext* font;

    // Configure base fonts.
    ttMontBold    = ttFontLoadCached("mont_hvbold.ttf", 1);
    ttRedCircle    = ttFontLoadCached("redcircl.ttf", 1);
    ttMont        = ttFontLoadCached("mont_demibold.ttf", 1);
    ttTahoma    = ttFontLoadCached("tahoma.ttf", 1);
    ttTahomaBD    = ttFontLoadCached("tahomabd.ttf", 1);
    ttVerdana    = ttFontLoadCached("verdana.ttf", 1);
    ttVerdanab    = ttFontLoadCached("verdanab.ttf", 1);
     ttGulim        = ttFontLoadCachedFace("gulim.ttc", 1, "Gulim");
    ttDotum        = ttFontLoadCachedFace("gulim.ttc", 1, "Dotum");
    ttAsiaPFB    = ttFontLoadCached("AsiaPFB.ttf", 1);
    ttYGO230    = ttFontLoadCached("YGO230.ttf", 1);
    ttMingLiu    = ttFontLoadCached("msgothic.ttc", 1);
    ttCourier    = ttFontLoadCached("cour.ttf", 1);
    ttDigitalb    = ttFontLoadCached("digitalb.ttf", 1);
    ttTiny        = ttFontLoadCached("slkscr.ttf", 1);
    ttGothic    = ttFontLoadCached("GOTHIC.TTF",1);
    ttGothicB    = ttFontLoadCached("GOTHICB.TTF",1);


    // If we can't load any of these, the client is just unusable.
    assert(ttRedCircle && ttMontBold && ttTahomaBD && ttTahoma && ttVerdana && ttVerdanab && ttCourier && ttDigitalb && ttTiny);

    // These are backup fonts which the game _could_ be playable without,
    //   at least in North America. They don't always seem to load (esp. MingLiu)
    if(!ttMingLiu)
    {
        //printf("Unable to load MingLiu font.\n");
    }
    else
    {
        // These fonts require the freetype byte code interpreter to have the glyphs
        // generated correctly.
        ttMingLiu->glyphGenMode = TTGGM_INTERPRETER;
    }

    if(!ttGulim)
    {
        printf("Unable to load Gulim font.\n");
    }
    else
    {
        // These fonts require the freetype byte code interpreter to have the glyphs
        // generated correctly.
        ttGulim->glyphGenMode = TTGGM_INTERPRETER;

        // gulim has notes as characters
        ttGulim->excludeCharacters = createArray();
        arrayPushBack(ttGulim->excludeCharacters, (void *) '\r');
        arrayPushBack(ttGulim->excludeCharacters, (void *) '\t');
    }

    if(!ttDotum) {}//printf("Unable to load Dotum font.\n");
    else ttDotum->glyphGenMode = TTGGM_INTERPRETER;
    if(!ttAsiaPFB) printf("Unable to load AsiaPFB font.\n");
    else ttAsiaPFB->glyphGenMode = TTGGM_INTERPRETER;
    if(!ttYGO230) printf("Unable to load YGO230 font.\n");
    else ttYGO230->glyphGenMode = TTGGM_INTERPRETER;

    // Hinting makes this font look hideous.
    ttMontBold->glyphGenMode = TTGGM_NOHINT;

    // Some characters are not available in RedCircle.  Exclude them.
    ttRedCircle->excludeCharacters = createArray();
    arrayPushBack(ttRedCircle->excludeCharacters, (void *) '_');
    arrayPushBack(ttRedCircle->excludeCharacters, (void *) '(');
    arrayPushBack(ttRedCircle->excludeCharacters, (void *) ')');
    arrayPushBack(ttRedCircle->excludeCharacters, (void *) '@');
    arrayPushBack(ttRedCircle->excludeCharacters, (void *) '|');
    arrayPushBack(ttRedCircle->excludeCharacters, (void *) '\'');
    arrayPushBack(ttRedCircle->excludeCharacters, (void *) '\\');

    // Configure composite fonts.
    gameTextFont = createTTCompositeFont();
    if (getCurrentLocale() == 3) ttFontAddFallback(gameTextFont, ttAsiaPFB);    
    ttFontAddFallback(gameTextFont, ttMont);
    ttFontAddFallback(gameTextFont, ttVerdanab);
    ttFontAddFallback(gameTextFont, ttMingLiu); 
    ttFontAddFallback(gameTextFont, ttAsiaPFB);    
    ttFontAddFallback(gameTextFont, ttGulim);    

    // for profile strip out the mingliu fallback so missing
    // composition characters don't show up. This is an experiment by
    // NCSeoul because they don't want to buy the complete ttYGO230
    // font.
    smfProfileNameFont = createTTCompositeFont();
    if (getCurrentLocale() == 3) ttFontAddFallback(smfProfileNameFont, ttAsiaPFB);    
    ttFontAddFallback(smfProfileNameFont, ttMont);
    ttFontAddFallback(smfProfileNameFont, ttVerdanab);
    ttFontAddFallback(smfProfileNameFont, ttMingLiu); 
    ttFontAddFallback(smfProfileNameFont, ttAsiaPFB);    

    gameBoldFont = createTTCompositeFont();
    if (getCurrentLocale() == 3) ttFontAddFallback(gameBoldFont, ttAsiaPFB);    
    ttFontAddFallback(gameBoldFont, ttMontBold);
    //ttFontAddFallback(gameBoldFont, ttMont);
    ttFontAddFallback(gameBoldFont, ttMingLiu);
    ttFontAddFallback(gameBoldFont, ttAsiaPFB);
    ttFontAddFallback(gameBoldFont, ttGulim);    

    titleTextFont = createTTCompositeFont();
    if (getCurrentLocale() == 3) ttFontAddFallback(titleTextFont, ttAsiaPFB);    
    ttFontAddFallback(titleTextFont, ttRedCircle);
    ttFontAddFallback(titleTextFont, ttMontBold);
    ttFontAddFallback(titleTextFont, ttMingLiu);
    ttFontAddFallback(titleTextFont, ttAsiaPFB);
    ttFontAddFallback(titleTextFont, ttGulim);    

    chatThinFont = createTTCompositeFont();
    if (getCurrentLocale() == 3) ttFontAddFallback(chatThinFont, ttYGO230);    
    ttFontAddFallback(chatThinFont, ttVerdana);
    ttFontAddFallback(chatThinFont, ttMingLiu);
    ttFontAddFallback(chatThinFont, ttYGO230);    

    chatTextFont = createTTCompositeFont();
    if (getCurrentLocale() == 3) ttFontAddFallback(chatTextFont, ttGulim);    
    ttFontAddFallback(chatTextFont, ttMont);
    ttFontAddFallback(chatTextFont, ttVerdana);
    ttFontAddFallback(chatTextFont, ttMingLiu);
    ttFontAddFallback(chatTextFont, ttGulim);

    chatBoldFont = createTTCompositeFont();
    if (getCurrentLocale() == 3) ttFontAddFallback(chatBoldFont, ttGulim);    
    ttFontAddFallback(chatBoldFont, ttVerdanab);
    ttFontAddFallback(chatBoldFont, ttVerdana);
    ttFontAddFallback(chatBoldFont, ttMingLiu);
    ttFontAddFallback(chatBoldFont, ttGulim);    

    referenceTextFont = createTTCompositeFont();
    if (getCurrentLocale() == 3) ttFontAddFallback(referenceTextFont, ttGulim);    
    ttFontAddFallback(referenceTextFont, ttTahomaBD);
    ttFontAddFallback(referenceTextFont, ttMingLiu);
    ttFontAddFallback(referenceTextFont, ttGulim);

    smfMonoFont = createTTCompositeFont();
    if (getCurrentLocale() == 3) ttFontAddFallback(smfMonoFont, ttGulim);    
    ttFontAddFallback(smfMonoFont, ttCourier);
    ttFontAddFallback(smfMonoFont, ttMingLiu);
    ttFontAddFallback(smfMonoFont, ttGulim);    

    smfComputerFont = createTTCompositeFont();
    if (getCurrentLocale() == 3) ttFontAddFallback(smfComputerFont, ttYGO230);    
    ttFontAddFallback(smfComputerFont, ttDigitalb);
    ttFontAddFallback(smfComputerFont, ttMingLiu);
    ttFontAddFallback(smfComputerFont, ttYGO230);    

    smfLargeFont = createTTCompositeFont();
    if (getCurrentLocale() == 3) ttFontAddFallback(smfLargeFont, ttYGO230);    
    ttFontAddFallback(smfLargeFont, ttMont);
    ttFontAddFallback(smfLargeFont, ttVerdanab);
    ttFontAddFallback(smfLargeFont, ttMingLiu);
    ttFontAddFallback(smfLargeFont, ttYGO230);

    smfSmallFont = createTTCompositeFont();
    if (getCurrentLocale() == 3) ttFontAddFallback(smfSmallFont, ttAsiaPFB);    
    ttFontAddFallback(smfSmallFont, ttMont);
    ttFontAddFallback(smfSmallFont, ttVerdana);
    ttFontAddFallback(smfSmallFont, ttMingLiu);
    ttFontAddFallback(smfSmallFont, ttAsiaPFB);    
    ttFontAddFallback(smfSmallFont, ttGulim);    

    tinyTextFont = createTTCompositeFont();
    if (getCurrentLocale() == 3) ttFontAddFallback(tinyTextFont, ttGulim);    
    ttFontAddFallback(tinyTextFont, ttTiny);
    ttFontAddFallback(tinyTextFont, ttMingLiu);
    ttFontAddFallback(tinyTextFont, ttGulim);    

    editorCFont = createTTCompositeFont();
    ttFontAddFallback(editorCFont, ttTahoma);

    gameHybrid = createTTCompositeFont();
    ttFontAddFallback(gameHybrid, ttGothic);
    ttFontAddFallback(gameHybrid, ttVerdana);

    gameHybridBold = createTTCompositeFont();
    ttFontAddFallback(gameHybridBold, ttGothicB);
    ttFontAddFallback(gameHybridBold, ttVerdanab);


    // Add composite fonts to a font manager so generated bitmaps
    // can be reused + unloaded as appropriate.
    fontManager = createTTFontManager();
    ttFMAddFont(fontManager, gameTextFont);
    ttFMAddFont(fontManager, smfProfileNameFont);
    ttFMAddFont(fontManager, gameBoldFont);
    ttFMAddFont(fontManager, titleTextFont);
    ttFMAddFont(fontManager, chatThinFont);
    ttFMAddFont(fontManager, chatTextFont);
    ttFMAddFont(fontManager, chatBoldFont);
    ttFMAddFont(fontManager, referenceTextFont);
    ttFMAddFont(fontManager, smfMonoFont);
    ttFMAddFont(fontManager, smfComputerFont);
    ttFMAddFont(fontManager, smfLargeFont);
    ttFMAddFont(fontManager, smfSmallFont);
    ttFMAddFont(fontManager, tinyTextFont);
    ttFMAddFont(fontManager, editorCFont);
    ttFMAddFont(fontManager, gameHybrid);
    ttFMAddFont(fontManager, gameHybridBold);

    // Configure the TTDrawContexts so that text can be drawn with correct settings.
    font = &title_9;
    initTTDrawContext(font, 9);
    font->font = titleTextFont;
    font->renderParams.italicize = 1;
    if (getCurrentLocale() == 3) font->renderParams.italicize = 0;
    font->renderParams.outline = 1;
    font->renderParams.outlineWidth = 1;
    //font->renderParams.dropShadow = 1;
    //font->renderParams.dropShadowXOffset = 1;
    //font->renderParams.dropShadowYOffset = 1;

    font = &title_12;
    initTTDrawContext(font, 12);
    font->font = titleTextFont;
    font->renderParams.italicize = 1;
    if (getCurrentLocale() == 3) font->renderParams.italicize = 0;
    font->renderParams.outline = 1;
    font->renderParams.outlineWidth = 1;

    font = &title_12_no_outline;
    initTTDrawContext(font, 12);
    font->font = titleTextFont;
    font->renderParams.italicize = 1;
    if (getCurrentLocale() == 3) font->renderParams.italicize = 0;
    font->renderParams.outline = 0;
    font->renderParams.outlineWidth = 1;

    font = &title_14;
    initTTDrawContext(font, 14);
    font->font = titleTextFont;
    font->renderParams.italicize = 1;
    if (getCurrentLocale() == 3) font->renderParams.italicize = 0;
    font->renderParams.outline = 1;
    font->renderParams.outlineWidth = 1;

    font = &title_18;
    initTTDrawContext(font, 18);
    font->font = titleTextFont;
    font->renderParams.defaultSize = 64;
    font->renderParams.italicize = 1;
    if (getCurrentLocale() == 3) font->renderParams.italicize = 0;
    font->renderParams.outline = 1;
    font->renderParams.outlineWidth = 1;
    font->dynamic = 1;

    font = &title_18_thick;
    initTTDrawContext(font, 18);
    font->font = titleTextFont;
    font->renderParams.defaultSize = 64;
    font->renderParams.italicize = 1;
    if (getCurrentLocale() == 3) font->renderParams.italicize = 0;
    font->renderParams.outline = 1;
    font->renderParams.outlineWidth = 2;
    font->dynamic = 1;

    font = &title_24;
    initTTDrawContext(font, 24);
    font->font = titleTextFont;
    font->renderParams.defaultSize = 64;
    font->renderParams.italicize = 1;
    if (getCurrentLocale() == 3) font->renderParams.italicize = 0;
    font->renderParams.outline = 1;
    font->renderParams.outlineWidth = 1;
    font->dynamic = 1;

    font = &game_9;
    initTTDrawContext(font, 9);
    font->font = gameTextFont;
    font->renderParams.italicize = 0;
    font->renderParams.outline = 1;
    font->renderParams.outlineWidth = 1;

    font = &game_9_dynamic;
    initTTDrawContext(font, 9);
    font->font = gameTextFont;
    font->renderParams.italicize = 0;
    font->renderParams.outline = 1;
    font->renderParams.outlineWidth = 1;
    font->dynamic = 1;

    font = &game_10;
    initTTDrawContext(font, 10);
    font->font = gameTextFont;
    font->renderParams.italicize = 0;
    font->renderParams.outline = 1;
    font->renderParams.outlineWidth = 1;

    font = &game_10_dynamic;
    initTTDrawContext(font, 10);
    font->font = gameTextFont;
    font->renderParams.italicize = 0;
    font->renderParams.outline = 1;
    font->renderParams.outlineWidth = 1;
    font->dynamic = 1;

    font = &game_12;
    initTTDrawContext(font, 12);
    font->font = gameTextFont;
    font->renderParams.italicize = 0;
    font->renderParams.outline = 1;
    font->renderParams.outlineWidth = 1;

    font = &game_12_dynamic;
    initTTDrawContext(font, 12);
    font->font = gameTextFont;
    font->renderParams.italicize = 0;
    font->renderParams.outline = 1;
    font->renderParams.outlineWidth = 1;
    font->dynamic = 1;

    font = &game_14;
    initTTDrawContext(font, 14);
    font->font = gameTextFont;
    font->renderParams.italicize = 0;
    font->renderParams.outline = 1;
    font->renderParams.outlineWidth = 1;

    font = &game_18;
    initTTDrawContext(font, 18);
    font->font = gameTextFont;
    font->renderParams.defaultSize = 64;
    font->renderParams.italicize = 0;
    font->renderParams.outline = 1;
    font->renderParams.outlineWidth = 1;
    font->dynamic = 1;

    font = &gamebold_9;
    initTTDrawContext(font, 9);
    font->font = gameBoldFont;
    font->renderParams.italicize = 1;
    font->renderParams.outline = 1;
    font->renderParams.outlineWidth = 1;

    font = &gamebold_12;
    initTTDrawContext(font, 12);
    font->font = gameBoldFont;
    font->renderParams.italicize = 1;
    font->renderParams.outline = 1;
    font->renderParams.outlineWidth = 1;

    font = &gamebold_14;
    initTTDrawContext(font, 14);
    font->font = gameBoldFont;
    font->renderParams.italicize = 1;
    font->renderParams.outline = 1;
    font->renderParams.outlineWidth = 1;

    font = &gamebold_18;
    initTTDrawContext(font, 18);
    font->font = gameBoldFont;
    font->renderParams.defaultSize = 64;
    font->renderParams.italicize = 1;
    if (getCurrentLocale() == 3) font->renderParams.italicize = 0;
    font->renderParams.outline = 1;
    font->renderParams.outlineWidth = 1;
    font->dynamic = 1;

    font = &chatThin;
    initTTDrawContext(font, 12);
    font->font = chatThinFont;
    //font->renderParams.italicize = 0;
    //font->renderParams.dropShadow = 1;
    //font->renderParams.outline = 1;
    //font->renderParams.outlineWidth = 1;
    //font->renderParams.dropShadowXOffset = 1;
    //font->renderParams.dropShadowYOffset = 1;
    font->dynamic = 1;
    //font->dynamic = 0; //  // -AB: jimb thinks this may fix screwed up asian characters :2005 Jun 07 11:37 AM

    font->renderParams.defaultSize = 64;

    font = &hybrid_9;
    initTTDrawContext(font, 9);
    font->font = gameHybrid;
    font->renderParams.italicize = 0;
    font->renderParams.outline = 0;
    font->renderParams.outlineWidth = 1;

    font = &hybrid_9_dynamic;
    initTTDrawContext(font, 9);
    font->font = gameHybrid;
    font->renderParams.italicize = 0;
    font->renderParams.outline = 0;
    font->renderParams.outlineWidth = 1;
    font->dynamic = 1;

    font = &hybrid_12;
    initTTDrawContext(font, 12);
    font->font = gameHybrid;
    font->renderParams.italicize = 0;
    font->renderParams.outline = 0;
    font->renderParams.outlineWidth = 1;

    font = &hybridbold_12;
    initTTDrawContext(font, 12);
    font->font = gameHybridBold;
    font->renderParams.italicize = 0;
    font->renderParams.outline = 0;
    font->renderParams.outlineWidth = 1;

    font = &hybrid_14;
    initTTDrawContext(font, 14);
    font->font = gameHybrid;
    font->renderParams.italicize = 0;
    font->renderParams.outline = 0;
    font->renderParams.outlineWidth = 1;

    font = &hybridbold_14;
    initTTDrawContext(font, 14);
    font->font = gameHybridBold;
    font->renderParams.italicize = 0;
    font->renderParams.outline = 0;
    font->renderParams.outlineWidth = 1;

    font = &hybrid_18;
    initTTDrawContext(font, 18);
    font->font = gameHybrid;
    font->renderParams.italicize = 0;
    font->renderParams.outline = 0;
    font->renderParams.outlineWidth = 1;

    font = &hybridbold_18;
    initTTDrawContext(font, 18);
    font->font = gameHybridBold;
    font->renderParams.italicize = 0;
    font->renderParams.outline = 0;
    font->renderParams.outlineWidth = 1;

    font = &chatFont;
    initTTDrawContext(font, 12);
    font->font = chatTextFont;
    font->renderParams.italicize = 0;
    font->renderParams.outline = 1;
    font->renderParams.outlineWidth = 1;

    font = &chatBold;
    initTTDrawContext(font, 12);
    font->font = chatBoldFont;
    font->renderParams.italicize = 0;
    font->renderParams.outline = 1;
    font->renderParams.outlineWidth = 1;

    font = &profileFont;
    initTTDrawContext(font, 15);
    font->font = gameTextFont;

    // special font just for entering the player's name.
    font = &profileNameFont;
    initTTDrawContext(font, 15);
    font->font = smfProfileNameFont;

    font = &referenceFont;
    initTTDrawContext(font, 12);
    font->renderParams.renderSize = 12;
    font->renderParams.defaultSize = 64;
    font->font = referenceTextFont;
    font->renderParams.italicize = 0;
    font->dynamic = 1;

    font = &entityNameText;
    initTTDrawContext(font, 12);
    font->renderParams.renderSize = 12;
    font->renderParams.defaultSize = 64;
    font->font = gameBoldFont;
    font->renderParams.italicize = 0;
    font->renderParams.outline = 1;
    font->renderParams.outlineWidth = 1;
    font->dynamic = 1;

    font = &smfLarge;
    initTTDrawContext(font, 15);
    font->font = smfLargeFont;
    font->renderParams.italicize = 0;
    font->renderParams.outline = 1;
    font->renderParams.outlineWidth = 1;

    font = &smfDefault;
    initTTDrawContext(font, 14);
    font->font = gameTextFont;
    font->renderParams.italicize = 0;
    font->renderParams.outline = 1;
    font->renderParams.outlineWidth = 1;

    font = &smfSmall;
    initTTDrawContext(font, 12);
    font->font = smfSmallFont;
    font->renderParams.italicize = 0;
    font->renderParams.outline = 1;
    font->renderParams.outlineWidth = 1;

    font = &smfVerySmall;
    initTTDrawContext(font, 9);
    font->font = gameTextFont;
    font->renderParams.italicize = 0;
    font->renderParams.outline = 1;
    font->renderParams.outlineWidth = 1;

    font = &smfMono;
    initTTDrawContext(font, 11);
    font->font = smfMonoFont;
    font->renderParams.italicize = 0;
    font->renderParams.outline = 1;
    font->renderParams.outlineWidth = 1;

    font = &smfComputer;
    initTTDrawContext(font, 11);
    font->font = smfComputerFont;
    font->renderParams.italicize = 0;
    font->renderParams.outline = 1;
    font->renderParams.outlineWidth = 1;

    font = &tinyfont;
    initTTDrawContext(font, 8);
    font->font = tinyTextFont;
    font->renderParams.italicize = 0;
    font->renderParams.outline = 1;
    font->renderParams.outlineWidth = 1;

    font = &editorFont;
    initTTDrawContext(font, 10);
    font->font = editorCFont;

    // Free less-often used, large fallback fonts, unless we're in a locale that needs them immediately
    if (getCurrentLocale() == 0 || getCurrentLocale() == 5 || getCurrentLocale() == 6) {
        if (ttMingLiu) fileFreeZippedBuffer(ttMingLiu->stream->descriptor.pointer);
        if (ttGulim) fileFreeZippedBuffer(ttGulim->stream->descriptor.pointer);
        if (ttDotum) fileFreeZippedBuffer(ttDotum->stream->descriptor.pointer);
        if (ttAsiaPFB) fileFreeZippedBuffer(ttAsiaPFB->stream->descriptor.pointer);
        if (ttYGO230) fileFreeZippedBuffer(ttYGO230->stream->descriptor.pointer);
    }
}

/* End of File */
