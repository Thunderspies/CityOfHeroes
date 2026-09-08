/***************************************************************************
 *     Copyright (c) 2007-2007, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 *
 * $Workfile: $
 * $Revision: 40606 $
 * $Date: 2007-06-07 12:09:46 -0700 (Thu, 07 Jun 2007) $
 *
 * Module Description:
 *
 * Revision History:
 *
 ***************************************************************************/
#ifndef CMDACCOUNT_H
#define CMDACCOUNT_H

#include <utilitieslib/utils/cmdoldparse.h>

C_DECLARATIONS_BEGIN

extern Cmd g_accountserver_cmds_server[];

#if defined(SERVER) || defined(CLIENT)
void AccountServer_ClientCommand(Cmd *cmd, int message_dbid, U32 auth_id, int dbid, char *source_str);
#endif

C_DECLARATIONS_END

#endif //CMDACCOUNT_H
