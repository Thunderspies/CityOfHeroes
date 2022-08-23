/*\
 *
 *    scriptDebugClient.h/c - Copyright 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Provides client UI for debugging scripts
 *
 */

#ifndef SCRIPTDEBUGCLIENT_H
#define SCRIPTDEBUGCLIENT_H

#include <utilitieslib/network/net_typedefs.h>

void receiveScriptDebugInfo(Packet *pak);
void ScriptDebugSet(int on);
void ScriptDebugSetVarTop(int id, int top);
void ScriptDebugSelect(int id);
void displayScriptDebugInfo(void);

#endif // SCRIPTDEBUGCLIENT_H