
#include "ArrayOld.h"
#include "cmdgame.h"
#include "uiConsole.h"
#include "tex.h"
#include "uiCursor.h"
#include "font.h"
#include "ttFont.h"
#include "renderUtil.h"
#include "render.h"
#include "win_init.h"
#include "utils.h"
#include "uigame.h"
#include "gfxwindow.h"
#include "player.h"
#include "camera.h"
#include "position.h"
#include "gridcoll.h"
#include "cmdcommon.h"
#include "input.h"
#include "textparser.h"
#include "earray.h"
#include "timing.h"
#include "memorypool.h"
#include "MemoryMonitor.h"
#include "initClient.h"
#include "clientcomm.h"
#include "assert.h"
#include "entrecv.h"
#include "edit_cmd.h"
#include "entclient.h"
#include "character_base.h"
#include "uiKeybind.h"
#include "renderstats.h"
#include "Npc.h"
#include "EString.h"
#include "strings_opt.h"
#include "motion.h"
#include "cmdcontrols.h"
#include "scriptDebugClient.h"
#include "entDebug.h"
#include "edit_drawlines.h"
#include "entDebugPrivate.h"
#include "uiUtil.h"
#include "renderprim.h"
#include "gfx.h"
#include "gfxLoadScreens.h"
#include "entity.h"
#include "pmotion.h"
#include "NwWrapper.h"
#include "groupfileload.h"
#include "StringUtil.h"
#include "sprite_base.h"

MP_DEFINE(PerfInfoUserData);

void destroyPerfInfoUserData(PerformanceInfo* info){
	MP_FREE(PerfInfoUserData, info->userData.data);
	info->userData.destructor = NULL;
}

PerfInfoUserData* getPerfInfoUserData(PerformanceInfo* info){
	PerfInfoUserData* userData;
	
	MP_CREATE(PerfInfoUserData, 100);
	
	if(!info->userData.data){
		int i;
		
		info->userData.data = userData = MP_ALLOC(PerfInfoUserData);
		info->userData.destructor = destroyPerfInfoUserData;
		
		for(i = 0; i < ARRAY_SIZE(userData->history); i++){
			userData->history[i].count = ~0;
		}
		
		if(!info->parent && info->locName && !strnicmp(info->locName, "Sleep(", 6)){
			userData->hidden = 1;
		}
	}
	
	return info->userData.data;
}

static void entDebugAdvanceTimerHistoryHelper(PerformanceInfo* cur){
	for(; cur; cur = cur->nextSibling){
		if(!debug_state.perfInfo.paused){
			PerfInfoUserData* infoData = getPerfInfoUserData(cur);
			S64 cycles64 = cur->curTick.cycles64;
			S32 count = cur->curTick.count;
			infoData->history[debug_state.perfInfo.historyPos].cycles = cycles64;
			infoData->history[debug_state.perfInfo.historyPos].count = count;
			cur->totalTime += cycles64;
			cur->opCountInt += count;
		}
			
		entDebugAdvanceTimerHistoryHelper(cur->child.head);
	}
}

static void entDebugAdvanceSystemTimer(PerformanceInfo* info){
	PerfInfoUserData* infoData = getPerfInfoUserData(info);
	infoData->history[debug_state.perfInfo.historyPos].cycles = info->curTick.cycles64;
	infoData->history[debug_state.perfInfo.historyPos].count = 0;
}

static void entDebugAdvanceSystemTimerTree(PerformanceInfo* info){
	entDebugAdvanceSystemTimer(info);
	
	for(info = info->child.head; info; info = info->nextSibling){
		entDebugAdvanceSystemTimerTree(info);
	}
}

PerformanceInfo* autoTimerGetOtherProcesses(void);

void entDebugAdvanceTimerHistory(){
	if(PERFINFO_RUN_CONDITIONS){
		if(!debug_state.perfInfo.paused){
			PerfInfoIterator iter = {0};
		
			PERFINFO_AUTO_START("entDebugAdvanceTimerHistory", 1);

				// Advance the history position.
				
				debug_state.perfInfo.historyPos = (debug_state.perfInfo.historyPos + 1) % TYPE_ARRAY_SIZE(PerfInfoUserData, history);

				entDebugAdvanceSystemTimer(autoTimerGetPageFaults());
				entDebugAdvanceSystemTimer(autoTimerGetProcessTimeUser());
				entDebugAdvanceSystemTimer(autoTimerGetProcessTimeKernel());
				entDebugAdvanceSystemTimerTree(autoTimerGetOtherProcesses());

				while(autoTimerIterateThreadRoots(&iter)){
					autoTimerDestroyLockAcquireByThreadID(iter.curThreadID);
					
					PERFINFO_AUTO_START("advance", 1);
						entDebugAdvanceSystemTimer(&iter.publicData->systemCounters.userMode);
						entDebugAdvanceSystemTimer(&iter.publicData->systemCounters.kernelMode);

						entDebugAdvanceTimerHistoryHelper(iter.publicData->timerRoot);
					PERFINFO_AUTO_STOP();

					autoTimerDestroyLockReleaseByThreadID(iter.curThreadID);
				}

			PERFINFO_AUTO_STOP();
		}
	}
}

