/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#ifndef LAUNCHER_ENUM_H
#define LAUNCHER_ENUM_H

#include <utilitieslib/stdtypes.h>

typedef enum LauncherEnabledServer
{
    kLauncherEnabledServer_None,
    kLauncherEnabledServer_Map = 1,
    kLauncherEnabledServer_Stat = kLauncherEnabledServer_Map<<1,
    kLauncherEnabledServer_All = (kLauncherEnabledServer_Stat | kLauncherEnabledServer_Map | kLauncherEnabledServer_None),
} LauncherEnabledServer;

static _inline bool enabledserver_Valid( LauncherEnabledServer e )
{
    return e == kLauncherEnabledServer_None || (e & kLauncherEnabledServer_All);
}

#endif //LAUNCHER_ENUM_H
