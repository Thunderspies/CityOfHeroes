#############################################################
## ENCOUNTERS_MOTION.INC - AnimLists that require Modes
#############################################################

## PPD Patrol Talk Walk ###########################################################

Move PPD_Patrol_Talk_Pre
	Requires	IDLE PPD PATROL TALK
	Priority	4
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		ReqInputs Cycle NoSizeScale
	CycleMove        PPD_Patrol_Talk_Pre
	CycleMove        PPD_Patrol_Talk_Post
	CycleMove        PPD_Patrol_Talk_Cycle
	CycleMove        PPD_Patrol_Talk_Post

	Type		male
	  Anim          male\ppd_patrol_talk 1 65
	  PlayFX	Feet\NPCMaleLeft.fx 0
	  PlayFX	Feet\NPCMaleRight.fx 16
	  PlayFX	Feet\NPCMaleLeft.fx 32
	  PlayFX	Feet\NPCMaleRight.fx 48
	  PlayFX	Feet\NPCMaleLeft.fx 64
	  PlayFX	Feet\NPCMaleRight.fx 80
	  PlayFX	Feet\NPCMaleLeft.fx 96
	  PlayFX	Feet\NPCMaleRight.fx 112
	  PlayFX	Feet\NPCMaleLeft.fx 128
	TEnd

	Type		fem
	  Anim          fem\ppd_patrol_talk 1 65
	  PlayFX	Feet\NPCMaleLeft.fx 0
	  PlayFX	Feet\NPCMaleRight.fx 16
	  PlayFX	Feet\NPCMaleLeft.fx 32
	  PlayFX	Feet\NPCMaleRight.fx 48
	  PlayFX	Feet\NPCMaleLeft.fx 64
	  PlayFX	Feet\NPCMaleRight.fx 80
	  PlayFX	Feet\NPCMaleLeft.fx 96
	  PlayFX	Feet\NPCMaleRight.fx 112
	  PlayFX	Feet\NPCMaleLeft.fx 128
	TEnd

	Type		huge
	  Anim          huge\ppd_patrol_talk 1 65
	  PlayFX	Feet\NPCMaleLeft.fx 0
	  PlayFX	Feet\NPCMaleRight.fx 16
	  PlayFX	Feet\NPCMaleLeft.fx 32
	  PlayFX	Feet\NPCMaleRight.fx 48
	  PlayFX	Feet\NPCMaleLeft.fx 64
	  PlayFX	Feet\NPCMaleRight.fx 80
	  PlayFX	Feet\NPCMaleLeft.fx 96
	  PlayFX	Feet\NPCMaleRight.fx 112
	  PlayFX	Feet\NPCMaleLeft.fx 128
	TEnd

	Type		PraetorianClockworkMale
	  Anim          male\ppd_patrol_talk 1 65
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 58
	TEnd

	Type		PraetorianClockworkFem
	  Anim          fem\ppd_patrol_talk 1 65
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 58
	TEnd

	Type		PraetorianClockworkHuge
	  Anim          huge\ppd_patrol_talk 1 65
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 58
	TEnd

MEnd

Move PPD_Patrol_Talk_Cycle
	Requires	 PPD PATROL
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle NoSizeScale
	CycleMove        PPD_Patrol_Talk_Pre
	CycleMove        PPD_Patrol_Talk_Post
	CycleMove        PPD_Patrol_Talk_Post
	CycleMove        PPD_Patrol_Talk_Cycle

	Type		male
	  Anim          male\ppd_patrol_talk 65	193
	  PlayFX	Feet\NPCMaleLeft.fx 0
	  PlayFX	Feet\NPCMaleRight.fx 16
	  PlayFX	Feet\NPCMaleLeft.fx 32
	  PlayFX	Feet\NPCMaleRight.fx 48
	  PlayFX	Feet\NPCMaleLeft.fx 64
	  PlayFX	Feet\NPCMaleRight.fx 80
	  PlayFX	Feet\NPCMaleLeft.fx 96
	  PlayFX	Feet\NPCMaleRight.fx 112
	  PlayFX	Feet\NPCMaleLeft.fx 128
	TEnd

	Type		fem
	  Anim          fem\ppd_patrol_talk 65 193
	  PlayFX	Feet\NPCMaleLeft.fx 0
	  PlayFX	Feet\NPCMaleRight.fx 16
	  PlayFX	Feet\NPCMaleLeft.fx 32
	  PlayFX	Feet\NPCMaleRight.fx 48
	  PlayFX	Feet\NPCMaleLeft.fx 64
	  PlayFX	Feet\NPCMaleRight.fx 80
	  PlayFX	Feet\NPCMaleLeft.fx 96
	  PlayFX	Feet\NPCMaleRight.fx 112
	  PlayFX	Feet\NPCMaleLeft.fx 128
	TEnd

	Type		huge
	  Anim          huge\ppd_patrol_talk 65 193
	  PlayFX	Feet\NPCMaleLeft.fx 0
	  PlayFX	Feet\NPCMaleRight.fx 16
	  PlayFX	Feet\NPCMaleLeft.fx 32
	  PlayFX	Feet\NPCMaleRight.fx 48
	  PlayFX	Feet\NPCMaleLeft.fx 64
	  PlayFX	Feet\NPCMaleRight.fx 80
	  PlayFX	Feet\NPCMaleLeft.fx 96
	  PlayFX	Feet\NPCMaleRight.fx 112
	  PlayFX	Feet\NPCMaleLeft.fx 128
	TEnd

	Type		PraetorianClockworkmale
	  Anim          male\ppd_patrol_talk 65	193
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 58
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 74
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 90
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 106
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 122
	TEnd

	Type		PraetorianClockworkfem
	  Anim          fem\ppd_patrol_talk 65 193
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 58
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 74
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 90
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 106
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 122
	TEnd

	Type		PraetorianClockworkhuge
	  Anim          huge\ppd_patrol_talk 65 193
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 58
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 74
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 90
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 106
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 122
	TEnd

MEnd

Move PPD_Patrol_Talk_Post
	Requires	 PPD PATROL
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove        PPD_Patrol_Talk_Post
	CycleMove        PPD_Patrol_Talk_Post
	CycleMove        PPD_Patrol_Talk_Cycle
	CycleMove        PPD_Patrol_Talk_Pre

	Type		male
	  Anim          male\ppd_patrol_talk 193 321
	  PlayFX	Feet\NPCMaleLeft.fx 0
	  PlayFX	Feet\NPCMaleRight.fx 16
	  PlayFX	Feet\NPCMaleLeft.fx 32
	  PlayFX	Feet\NPCMaleRight.fx 48
	  PlayFX	Feet\NPCMaleLeft.fx 64
	  PlayFX	Feet\NPCMaleRight.fx 80
	  PlayFX	Feet\NPCMaleLeft.fx 96
	  PlayFX	Feet\NPCMaleRight.fx 112
	  PlayFX	Feet\NPCMaleLeft.fx 128
	TEnd

	Type		fem
	  Anim          fem\ppd_patrol_talk 193 321
	  PlayFX	Feet\NPCMaleLeft.fx 0
	  PlayFX	Feet\NPCMaleRight.fx 16
	  PlayFX	Feet\NPCMaleLeft.fx 32
	  PlayFX	Feet\NPCMaleRight.fx 48
	  PlayFX	Feet\NPCMaleLeft.fx 64
	  PlayFX	Feet\NPCMaleRight.fx 80
	  PlayFX	Feet\NPCMaleLeft.fx 96
	  PlayFX	Feet\NPCMaleRight.fx 112
	  PlayFX	Feet\NPCMaleLeft.fx 128
	TEnd

	Type		huge
	  Anim          huge\ppd_patrol_talk 193 321
	  PlayFX	Feet\NPCMaleLeft.fx 0
	  PlayFX	Feet\NPCMaleRight.fx 16
	  PlayFX	Feet\NPCMaleLeft.fx 32
	  PlayFX	Feet\NPCMaleRight.fx 48
	  PlayFX	Feet\NPCMaleLeft.fx 64
	  PlayFX	Feet\NPCMaleRight.fx 80
	  PlayFX	Feet\NPCMaleLeft.fx 96
	  PlayFX	Feet\NPCMaleRight.fx 112
	  PlayFX	Feet\NPCMaleLeft.fx 128
	TEnd

	Type		PraetorianClockworkmale
	  Anim          male\ppd_patrol_talk 193 321
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 58
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 74
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 90
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 106
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 122
	TEnd

	Type		PraetorianClockworkfem
	  Anim          fem\ppd_patrol_talk 193 321
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 58
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 74
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 90
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 106
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 122
	TEnd

	Type		PraetorianClockworkhuge
	  Anim          huge\ppd_patrol_talk 193 321
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 58
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 74
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 90
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 106
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 122
	TEnd

MEnd



## PPD Patrol Flashlight ###########################################################

Move PPD_Patrol_Flashlight_Pre
	Requires	IDLE PPD PATROL FLASHLIGHT
	Priority	4
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		ReqInputs Cycle NoSizeScale
	CycleMove        PPD_Patrol_Flashlight_Pre
	CycleMove        PPD_Patrol_Flashlight_Post
	CycleMove        PPD_Patrol_Flashlight_Post
	CycleMove        PPD_Patrol_Flashlight_Cycle

	Type		male
	  Anim          male\ppd_patrol_flashlight 1 129
	  PlayFX	Generic\Flashlight_Small.fx 0 Constant
	  PlayFX	Feet\NPCMaleLeft.fx 0
	  PlayFX	Feet\NPCMaleRight.fx 16
	  PlayFX	Feet\NPCMaleLeft.fx 32
	  PlayFX	Feet\NPCMaleRight.fx 48
	  PlayFX	Feet\NPCMaleLeft.fx 64
	  PlayFX	Feet\NPCMaleRight.fx 80
	  PlayFX	Feet\NPCMaleLeft.fx 96
	  PlayFX	Feet\NPCMaleRight.fx 112
	  PlayFX	Feet\NPCMaleLeft.fx 128
	TEnd

	Type		PraetorianClockworkmale
	  Anim          male\ppd_patrol_flashlight 1 129
	  PlayFX	Generic\Flashlight_Small.fx 0 Constant
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 58
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 74
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 90
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 106
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 122
	TEnd

	Type		PraetorianClockworkFem
	  Anim          male\ppd_patrol_flashlight 1 129
	  PlayFX	Generic\Flashlight_Small.fx 0 Constant
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 58
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 74
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 90
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 106
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 122
	TEnd

	Type		PraetorianClockworkHuge
	  Anim          male\ppd_patrol_flashlight 1 129
	  PlayFX	Generic\Flashlight_Small.fx 0 Constant
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 58
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 74
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 90
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 106
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 122
	TEnd

MEnd

Move PPD_Patrol_Flashlight_Cycle
	Requires	 IDLE PPD PATROL
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle NoSizeScale
	CycleMove        PPD_Patrol_Flashlight_Pre
	CycleMove        PPD_Patrol_Flashlight_Post
	CycleMove        PPD_Patrol_Flashlight_Post
	CycleMove        PPD_Patrol_Flashlight_Cycle

	Type		male
	  Anim          male\ppd_patrol_flashlight 129 257
	  PlayFX	Generic\Flashlight_Small.fx 0 Constant
	  PlayFX	Feet\NPCMaleLeft.fx 0
	  PlayFX	Feet\NPCMaleRight.fx 16
	  PlayFX	Feet\NPCMaleLeft.fx 32
	  PlayFX	Feet\NPCMaleRight.fx 48
	  PlayFX	Feet\NPCMaleLeft.fx 64
	  PlayFX	Feet\NPCMaleRight.fx 80
	  PlayFX	Feet\NPCMaleLeft.fx 96
	  PlayFX	Feet\NPCMaleRight.fx 112
	  PlayFX	Feet\NPCMaleLeft.fx 128
	TEnd

	Type		PraetorianClockworkmale
	  Anim          male\ppd_patrol_flashlight 129 257
	  PlayFX	Generic\Flashlight_Small.fx 0 Constant
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 58
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 74
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 90
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 106
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 122
	TEnd

	Type		PraetorianClockworkFem
	  Anim          male\ppd_patrol_flashlight 129 257
	  PlayFX	Generic\Flashlight_Small.fx 0 Constant
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 58
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 74
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 90
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 106
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 122
	TEnd

	Type		PraetorianClockworkHuge
	  Anim          male\ppd_patrol_flashlight 129 257
	  PlayFX	Generic\Flashlight_Small.fx 0 Constant
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 58
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 74
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 90
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 106
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 122
	TEnd

MEnd


Move PPD_Patrol_Flashlight_Post
	Requires	 IDLE PATROL
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove        PPD_Patrol_Flashlight_Post
	CycleMove        PPD_Patrol_Flashlight_Pre
	CycleMove        PPD_Patrol_Flashlight_Cycle
	CycleMove        PPD_Patrol_Flashlight_Cycle

	Type		male
	  Anim          male\ppd_patrol_flashlight 257 321
	  PlayFX	Generic\Flashlight_Small.fx 0 Constant
	  PlayFX	Feet\NPCMaleLeft.fx 0
	  PlayFX	Feet\NPCMaleRight.fx 16
	  PlayFX	Feet\NPCMaleLeft.fx 32
	  PlayFX	Feet\NPCMaleRight.fx 48
	  PlayFX	Feet\NPCMaleLeft.fx 64
	  PlayFX	Feet\NPCMaleRight.fx 80
	  PlayFX	Feet\NPCMaleLeft.fx 96
	  PlayFX	Feet\NPCMaleRight.fx 112
	  PlayFX	Feet\NPCMaleLeft.fx 128
	TEnd

	Type		PraetorianClockworkmale
	  Anim          male\ppd_patrol_flashlight 257 321
	  PlayFX	Generic\Flashlight_Small.fx 0 Constant
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 58
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 74
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 90
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 106
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 122
	TEnd

	Type		PraetorianClockworkFem
	  Anim          male\ppd_patrol_flashlight 257 321
	  PlayFX	Generic\Flashlight_Small.fx 0 Constant
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 58
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 74
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 90
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 106
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 122
	TEnd

	Type		PraetorianClockworkHuge
	  Anim          male\ppd_patrol_flashlight 257 321
	  PlayFX	Generic\Flashlight_Small.fx 0 Constant
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 58
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 74
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 90
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 106
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 122
	TEnd

MEnd

## Sneak_Patrol_Ready Walk ###########################################################

Move Sneak_Patrol_Pre
	Requires	IDLE WALK SCARED
	Priority	3
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		ReqInputs Cycle NoSizeScale
	CycleMove        Sneak_Patrol_Pre
	CycleMove        Sneak_Patrol_Post
	CycleMove        Sneak_Patrol_Cycle
	CycleMove        Sneak_Patrol_Cycle

	Type		male
	  Anim          male\sneak_patrol 1 129
	  PlayFX	Feet\NPCMaleLeft.fx 0
	  PlayFX	Feet\NPCMaleRight.fx 16
	  PlayFX	Feet\NPCMaleLeft.fx 32
	  PlayFX	Feet\NPCMaleRight.fx 48
	  PlayFX	Feet\NPCMaleLeft.fx 64
	  PlayFX	Feet\NPCMaleRight.fx 80
	  PlayFX	Feet\NPCMaleLeft.fx 96
	  PlayFX	Feet\NPCMaleRight.fx 112
	  PlayFX	Feet\NPCMaleLeft.fx 128
	TEnd

	Type		fem
	  Anim          fem\sneak_patrol 1 129
	  PlayFX	Feet\NPCMaleLeft.fx 0
	  PlayFX	Feet\NPCMaleRight.fx 16
	  PlayFX	Feet\NPCMaleLeft.fx 32
	  PlayFX	Feet\NPCMaleRight.fx 48
	  PlayFX	Feet\NPCMaleLeft.fx 64
	  PlayFX	Feet\NPCMaleRight.fx 80
	  PlayFX	Feet\NPCMaleLeft.fx 96
	  PlayFX	Feet\NPCMaleRight.fx 112
	  PlayFX	Feet\NPCMaleLeft.fx 128
	TEnd

	Type		PraetorianClockworkmale
	  Anim          male\sneak_patrol 1 129
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 58
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 74
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 90
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 106
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 122
	TEnd

	Type		PraetorianClockworkfem
	  Anim          fem\sneak_patrol 1 129
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 58
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 74
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 90
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 106
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 122
	TEnd

	Type		PraetorianClockworkHuge
	  Anim          fem\sneak_patrol 1 129
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 58
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 74
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 90
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 106
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 122
	TEnd

MEnd

Move Sneak_Patrol_Cycle
	Requires	 IDLE WALK
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle NoSizeScale
	CycleMove        Sneak_Patrol_Pre
	CycleMove        Sneak_Patrol_Post
	CycleMove        Sneak_Patrol_Pre
	CycleMove        Sneak_Patrol_Cycle

	Type		male
	  Anim          male\sneak_patrol 129 257
	  PlayFX	Feet\NPCMaleLeft.fx 0
	  PlayFX	Feet\NPCMaleRight.fx 16
	  PlayFX	Feet\NPCMaleLeft.fx 32
	  PlayFX	Feet\NPCMaleRight.fx 48
	  PlayFX	Feet\NPCMaleLeft.fx 64
	  PlayFX	Feet\NPCMaleRight.fx 80
	  PlayFX	Feet\NPCMaleLeft.fx 96
	  PlayFX	Feet\NPCMaleRight.fx 112
	  PlayFX	Feet\NPCMaleLeft.fx 128
	TEnd

	Type		fem
	  Anim          fem\sneak_patrol 129 257
	  PlayFX	Feet\NPCMaleLeft.fx 0
	  PlayFX	Feet\NPCMaleRight.fx 16
	  PlayFX	Feet\NPCMaleLeft.fx 32
	  PlayFX	Feet\NPCMaleRight.fx 48
	  PlayFX	Feet\NPCMaleLeft.fx 64
	  PlayFX	Feet\NPCMaleRight.fx 80
	  PlayFX	Feet\NPCMaleLeft.fx 96
	  PlayFX	Feet\NPCMaleRight.fx 112
	  PlayFX	Feet\NPCMaleLeft.fx 128
	TEnd

	Type		PraetorianClockworkMale
	  Anim          fem\sneak_patrol 129 257
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 58
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 74
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 90
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 106
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 122
	TEnd

	Type		PraetorianClockworkfem
	  Anim          fem\sneak_patrol 129 257
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 58
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 74
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 90
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 106
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 122
	TEnd

	Type		PraetorianClockworkHuge
	  Anim          fem\sneak_patrol 129 257
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 58
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 74
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 90
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 106
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 122
	TEnd

MEnd

Move Sneak_Patrol_Post
	Requires	IDLE WALK
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove        Sneak_Patrol_Pre
	CycleMove        Sneak_Patrol_Pre
	CycleMove        Sneak_Patrol_Post
	CycleMove        Sneak_Patrol_Cycle

	Type		male
	  Anim          male\sneak_patrol 257 321
	  PlayFX	Feet\NPCMaleLeft.fx 0
	  PlayFX	Feet\NPCMaleRight.fx 16
	  PlayFX	Feet\NPCMaleLeft.fx 32
	  PlayFX	Feet\NPCMaleRight.fx 48
	  PlayFX	Feet\NPCMaleLeft.fx 64
	  PlayFX	Feet\NPCMaleRight.fx 80
	  PlayFX	Feet\NPCMaleLeft.fx 96
	  PlayFX	Feet\NPCMaleRight.fx 112
	  PlayFX	Feet\NPCMaleLeft.fx 128
	TEnd

	Type		fem
	  Anim          fem\sneak_patrol 257 321
	  PlayFX	Feet\NPCMaleLeft.fx 0
	  PlayFX	Feet\NPCMaleRight.fx 16
	  PlayFX	Feet\NPCMaleLeft.fx 32
	  PlayFX	Feet\NPCMaleRight.fx 48
	  PlayFX	Feet\NPCMaleLeft.fx 64
	  PlayFX	Feet\NPCMaleRight.fx 80
	  PlayFX	Feet\NPCMaleLeft.fx 96
	  PlayFX	Feet\NPCMaleRight.fx 112
	  PlayFX	Feet\NPCMaleLeft.fx 128
	TEnd

	Type		PraetorianClockworkMale
	  Anim          fem\sneak_patrol 257 321
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 58
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 74
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 90
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 106
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 122
	TEnd

	Type		PraetorianClockworkFem
	  Anim          fem\sneak_patrol 257 321
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 58
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 74
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 90
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 106
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 122
	TEnd

	Type		PraetorianClockworkHuge
	  Anim          fem\sneak_patrol 257 321
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 58
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 74
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 90
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 106
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 122
	TEnd

MEnd

## PPD Patrol Megaphone ###########################################################

Move PPD_Patrol_Megaphone_Pre
	Requires	IDLE PPD PATROL YELL MEGAPHONE
	Priority	5
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		ReqInputs Cycle NoSizeScale
	CycleMove        PPD_Patrol_Megaphone_Pre
	CycleMove        PPD_Patrol_Megaphone_Post
	CycleMove        PPD_Patrol_Megaphone_Cycle_Two
	CycleMove        PPD_Patrol_Megaphone_Cycle_One

	Type		male
	  Anim          male\ppd_patrol_megaphone 1 65
  	  PlayFX	Feet\NPCMaleLeft.fx 0
	  PlayFX	Feet\NPCMaleRight.fx 16
	  PlayFX	Feet\NPCMaleLeft.fx 32
	  PlayFX	Feet\NPCMaleRight.fx 48
	  PlayFX	Feet\NPCMaleLeft.fx 64
	  PlayFX	Feet\NPCMaleRight.fx 80
	  PlayFX	Feet\NPCMaleLeft.fx 96
	  PlayFX	Feet\NPCMaleRight.fx 112
	  PlayFX	Feet\NPCMaleLeft.fx 128
	  PlayFX	\WORLD\Objects\Megaphone\Megaphone.fx 0
	TEnd

	Type		PraetorianClockworkmale
	  Anim          male\ppd_patrol_megaphone 1 65
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 58
	  PlayFX	\WORLD\Objects\Megaphone\Megaphone.fx 0
	TEnd

	Type		PraetorianClockworkFem
	  Anim          male\ppd_patrol_megaphone 1 65
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 58
	  PlayFX	\WORLD\Objects\Megaphone\Megaphone.fx 0
	TEnd

	Type		PraetorianClockworkHuge
	  Anim          male\ppd_patrol_megaphone 1 65
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 58
	  PlayFX	\WORLD\Objects\Megaphone\Megaphone.fx 0
	TEnd

MEnd

Move PPD_Patrol_Megaphone_Cycle_One
	Requires	 IDLE PPD PATROL YELL
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle NoSizeScale
	CycleMove        PPD_Patrol_Megaphone_Pre
	CycleMove        PPD_Patrol_Megaphone_Pre
	CycleMove        PPD_Patrol_Megaphone_Post
	CycleMove        PPD_Patrol_Megaphone_Post

	Type		male
	  Anim          male\ppd_patrol_megaphone 65 193
	  PlayFX	Feet\NPCMaleLeft.fx 0
	  PlayFX	Feet\NPCMaleRight.fx 16
	  PlayFX	Feet\NPCMaleLeft.fx 32
	  PlayFX	Feet\NPCMaleRight.fx 48
	  PlayFX	Feet\NPCMaleLeft.fx 64
	  PlayFX	Feet\NPCMaleRight.fx 80
	  PlayFX	Feet\NPCMaleLeft.fx 96
	  PlayFX	Feet\NPCMaleRight.fx 112
	  PlayFX	Feet\NPCMaleLeft.fx 128
	TEnd

	Type		huge
	  Anim          huge\ppd_patrol_megaphone 65 193
	  PlayFX	Feet\NPCMaleLeft.fx 0
	  PlayFX	Feet\NPCMaleRight.fx 16
	  PlayFX	Feet\NPCMaleLeft.fx 32
	  PlayFX	Feet\NPCMaleRight.fx 48
	  PlayFX	Feet\NPCMaleLeft.fx 64
	  PlayFX	Feet\NPCMaleRight.fx 80
	  PlayFX	Feet\NPCMaleLeft.fx 96
	  PlayFX	Feet\NPCMaleRight.fx 112
	  PlayFX	Feet\NPCMaleLeft.fx 128
	TEnd

	Type		PraetorianClockworkMale
	  Anim          huge\ppd_patrol_megaphone 65 193
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 58
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 74
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 90
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 106
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 122
	TEnd

	Type		PraetorianClockworkFem
	  Anim          huge\ppd_patrol_megaphone 65 193
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 58
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 74
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 90
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 106
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 122
	TEnd

	Type		PraetorianClockworkHuge
	  Anim          huge\ppd_patrol_megaphone 65 193
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 58
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 74
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 90
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 106
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 122
	TEnd

MEnd

Move PPD_Patrol_Megaphone_Post
	Requires	 IDLE PPD PATROL
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle NoSizeScale
	CycleMove        PPD_Patrol_Megaphone_Pre
	CycleMove        PPD_Patrol_Megaphone_Post
	CycleMove        PPD_Patrol_Megaphone_Post
	CycleMove        PPD_Patrol_Megaphone_Cycle_Two

	Type		male
	  Anim          male\ppd_patrol_megaphone 193 321
	  PlayFX	Feet\NPCMaleLeft.fx 0
	  PlayFX	Feet\NPCMaleRight.fx 16
	  PlayFX	Feet\NPCMaleLeft.fx 32
	  PlayFX	Feet\NPCMaleRight.fx 48
	  PlayFX	Feet\NPCMaleLeft.fx 64
	  PlayFX	Feet\NPCMaleRight.fx 80
	  PlayFX	Feet\NPCMaleLeft.fx 96
	  PlayFX	Feet\NPCMaleRight.fx 112
	  PlayFX	Feet\NPCMaleLeft.fx 128
	TEnd

	Type		PraetorianClockworkmale
	  Anim          male\ppd_patrol_megaphone 193 321
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 58
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 74
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 90
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 106
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 122
	TEnd

	Type		PraetorianClockworkFem
	  Anim          male\ppd_patrol_megaphone 193 321
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 58
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 74
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 90
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 106
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 122
	TEnd

	Type		PraetorianClockworkHuge
	  Anim          male\ppd_patrol_megaphone 193 321
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 58
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 74
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 90
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 106
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 122
	TEnd

MEnd

Move PPD_Patrol_Megaphone_Cycle_Two
	Requires	 IDLE PPD PATROL YELL
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove        PPD_Patrol_Megaphone_Pre
	CycleMove        PPD_Patrol_Megaphone_Pre
	CycleMove        PPD_Patrol_Megaphone_Post
	CycleMove        PPD_Patrol_Megaphone_Post

	Type		male
	  Anim          male\ppd_patrol_megaphone 321 449
	  PlayFX	Feet\NPCMaleLeft.fx 0
	  PlayFX	Feet\NPCMaleRight.fx 16
	  PlayFX	Feet\NPCMaleLeft.fx 32
	  PlayFX	Feet\NPCMaleRight.fx 48
	  PlayFX	Feet\NPCMaleLeft.fx 64
	  PlayFX	Feet\NPCMaleRight.fx 80
	  PlayFX	Feet\NPCMaleLeft.fx 96
	  PlayFX	Feet\NPCMaleRight.fx 112
	  PlayFX	Feet\NPCMaleLeft.fx 128
	TEnd

	Type		huge
	  Anim          huge\ppd_patrol_megaphone 321 449
	  PlayFX	Feet\NPCMaleLeft.fx 0
	  PlayFX	Feet\NPCMaleRight.fx 16
	  PlayFX	Feet\NPCMaleLeft.fx 32
	  PlayFX	Feet\NPCMaleRight.fx 48
	  PlayFX	Feet\NPCMaleLeft.fx 64
	  PlayFX	Feet\NPCMaleRight.fx 80
	  PlayFX	Feet\NPCMaleLeft.fx 96
	  PlayFX	Feet\NPCMaleRight.fx 112
	  PlayFX	Feet\NPCMaleLeft.fx 128
	TEnd

	Type		PraetorianClockworkMale
	  Anim          huge\ppd_patrol_megaphone 321 449
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 58
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 74
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 90
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 106
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 122
	TEnd

	Type		PraetorianClockworkFem
	  Anim          huge\ppd_patrol_megaphone 321 449
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 58
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 74
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 90
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 106
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 122
	TEnd

	Type		PraetorianClockworkHuge
	  Anim          huge\ppd_patrol_megaphone 321 449
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 10
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 26
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 42
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 58
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 74
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 90
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 106
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_FeetHuge.fx 122
	TEnd

MEnd

## Clockwork_StreetClean_walk ###########################################################

Move Clockwork_Streetclean_walk_pre
	Requires	LOITER HAND BLOW
	Priority	3
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		ReqInputs Cycle NoSizeScale MoveScale
	CycleMove       Clockwork_Streetclean_walk
	Interpolate	10

	Type		male
	  Anim          male\clockwork_streetclean_walk 1 10
	  PlayFX	ENEMYFX\Clockwork\Antimatter\Android_Idle_AirBlowing.fx 10 CONSTANT
	  MoveRate	0.1125 #5 feet per second
	TEnd
MEnd

Move Clockwork_Streetclean_walk
	Requires	LOITER HAND BLOW
	Include sequencers\Members\Encounters_Motion.inc
	Flags		NoSizeScale Cycle MoveScale
	NextMove	Clockwork_Streetclean_walk_Post

	Type		male
	  Anim          male\clockwork_streetclean_walk 11 204
	  PlayFX	ENEMYFX\Clockwork\Antimatter\Android_Idle_AirBlowing.fx 10 CONSTANT
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_Feet.fx 0
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_Feet.fx 24
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_Feet.fx 48
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_Feet.fx 72
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_Feet.fx 96
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_Feet.fx 120
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_Feet.fx 144
	  PlayFX	ENEMYFX\Clockwork\antimatter\Clockwork_Feet.fx 168
	  MoveRate	0.1125 #5 feet per second
	TEnd
MEnd

Move Clockwork_Streetclean_walk_Post
	Requires	LOITER HAND BLOW
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove       Clockwork_Streetclean_walk_pre

	Type		male
	  Anim          male\clockwork_streetclean_walk 204 240
	  PlayFX	ENEMYFX\Clockwork\Antimatter\Android_Idle_AirBlowing.fx 10 CONSTANT
	TEnd
MEnd

###################
###Clockwork Malfunction Patrol ##
###################

Move Clockwork_NPC_Patrol_Malfunction_Cycle
	Requires	LOITER MALFUNCTION HIGH
	Priority	3
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		ReqInputs Cycle NoSizeScale
	CycleMove	Clockwork_NPC_Patrol_Malfunction_Cycle
	SticksOnChild	COMBAT

	Type		male
	  Anim          male\clockwork_patrol_malfunction 1
	TEnd

MEnd

##################
###Clockwork Carrying Large Object ##
######################

Move Clockwork_NPC_Patrol_Carrying_Cycle
	Requires	LOITER CARRY HIGH
	Priority	3
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		ReqInputs Cycle NoSizeScale
	CycleMove	Clockwork_NPC_Patrol_Carrying_Cycle

	Type		male
	  Anim          male\clockwork_patrol_carry 1
	TEnd

MEnd

