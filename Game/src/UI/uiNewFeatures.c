#include "uiNewFeatures.h"
#include <utilitieslib/utils/Cbox.h>
#include "gameComm/wdwbase.h"
#include "UI/uiWindows.h"
#include "UI/uiInput.h"
#include "UI/Hybrid/uiWebStoreFrame.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtil.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include <utilitieslib/components/StringCache.h>
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "player/player.h"
#include "UI/uiNet.h"
#include <utilitieslib/utils/timing.h>

typedef enum WebDataState
{
    WEBDATA_IDLE,
    WEBDATA_WAITING,
    WEBDATA_PROCESSING,
} WebDataState;

#define MAX_QUERY_TIME 60

static const char * webDataReceived;
static int webDataVersion;
static WebDataState webDataProcess = WEBDATA_IDLE;
static U32 timeSinceQuery;

int newFeaturesWindow()
{
    F32 x, y, z, wd, ht, sc;
    int color, bcolor;
    CBox box;
    int textColor = CLR_RED;

    // Do everything common windows are supposed to do.
    if ( !window_getDims( WDW_NEW_FEATURES, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor ))
        return 0;

    drawFrame(PIX3, R10, x, y, z-2, wd, ht, sc, color, bcolor);

    BuildCBox(&box, x, y, wd, ht);
    if (mouseCollision(&box))
    {
        if (mouseClickHitNoCol(MS_LEFT))
        {
            //webStoreOpenNewFeatures();
        }
        //hover color
        textColor = CLR_WHITE;
    }

    font(&game_12);
    font_color(textColor, textColor);
    cprnt(x+wd/2.f, y+ht*0.8f, z, sc, sc, webDataReceived ? webDataReceived : "NewFeaturesDefaultString");
    
    return 0;
}

int newFeatures_webDataRetrieveCallback(void * data)
{
#if defined(USE_QTWEBKIT_BROWSER)
    HeroWebNoticeDataGeneric* pGeneric = (HeroWebNoticeDataGeneric*)data;
    webDataVersion = pGeneric->i1;
    webDataReceived = allocAddString(pGeneric->s1);
    webDataProcess = WEBDATA_PROCESSING;
    return true;
#else
    return false;
#endif // USE_QTWEBKIT_BROWSER
}

void newFeatures_queryWebData(void)
{
    //if (PlaySpanStoreLauncher_NewFeaturesUpdate())
    //{
    //    webDataProcess = WEBDATA_WAITING;
    //    timeSinceQuery = timerSecondsSince2000();
    //}
}

void newFeatures_update(void)
{
    if (webDataProcess == WEBDATA_PROCESSING)
    {
        Entity * e = playerPtr();
        if (e && e->pl)
        {
            webDataProcess = WEBDATA_IDLE;
            if (webDataVersion > e->pl->newFeaturesVersion)
            {
                e->pl->newFeaturesVersion = webDataVersion;
                // update entity
                sendNewFeatureVersion();
                // force reopen new features window
                window_setMode(WDW_NEW_FEATURES, WINDOW_GROWING);
            }

            //close off connection
#if defined(USE_QTWEBKIT_BROWSER)
            webDataRetriever_destroy();
#endif // USE_QTWEBKIT_BROWSER
        }
    }
    else if (webDataProcess == WEBDATA_WAITING)
    {
        //timeout
        if ((timerSecondsSince2000() - timeSinceQuery) > MAX_QUERY_TIME)
        {
            webDataProcess = WEBDATA_IDLE;
            timeSinceQuery = 0;
        }
    }
}
