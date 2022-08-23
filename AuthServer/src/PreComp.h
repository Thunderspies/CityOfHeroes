#pragma once

#define LINEAGE2_GAME_CODE 8
#include "WantedSocket.h"
#include "util.h"
#include "Config.h"
#include "Account.h"
#include "DBConn.h"
#include "AccountDB.h"
#include "Thread.h"
#include "IOServer.h"
#include "IPSessionDB.h"
#include "ServerList.h"
#include "LogSocket.h"

extern BOOL SendSocket(in_addr , const char *format, ...);
