/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#ifndef CMDSTATSERVER_H
#define CMDSTATSERVER_H

#include <utilitieslib/utils/cmdoldparse.h>
#include <utilitieslib/stdtypes.h>
 
extern Cmd client_sgstat_cmds[];
extern Cmd server_sgstat_cmds[];

void SgrpStat_SendPassthru(int idEnt, int idSgrp, char *format, ...);

#endif //CMDSTATSERVER_H
