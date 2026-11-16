#############################################################
## Listen
#############################################################

Move Listen_Loop1
	Requires	ENCOUNTER LISTEN
	Priority        2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly
	NextMove        Ready
	CycleMove       Listen_gesture1
	CycleMove       Listen_Loop1
	CycleMove       Listen_nod1
	CycleMove       Listen_shift1

	Type		male
	  Anim          male\listen01 1 60
	TEnd
MEnd

Move Listen_gesture1
	Requires	ENCOUNTER LISTEN
	Priority        2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly
	NextMove        Ready
	CycleMove       Listen_gesture1a
	CycleMove       Listen_gesture1b
	CycleMove       Listen_gesture1c

	Type		male
	  Anim          male\listen01 60 61
	TEnd
MEnd

Move Listen_gesture1a
	Requires	ENCOUNTER LISTEN
	Priority        2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly
	NextMove        Ready
	CycleMove       Listen_nod1
	CycleMove       Listen_shift1
	CycleMove       Listen_Loop1
	CycleMove       Listen_Loop1

	Type		male
	  Anim          male\listen01 61 96
	TEnd
MEnd

Move Listen_gesture1b
	Requires	ENCOUNTER LISTEN
	Priority        2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly
	NextMove        Ready
	CycleMove       Listen_nod1
	CycleMove       Listen_shift1
	CycleMove       Listen_Loop1
	CycleMove       Listen_Loop1

	Type		male
	  Anim          male\listen01 97 132
	TEnd
MEnd

Move Listen_gesture1c
	Requires	ENCOUNTER LISTEN
	Priority        2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly
	NextMove        Ready
	CycleMove       Listen_nod1
	CycleMove       Listen_shift1
	CycleMove       Listen_Loop1
	CycleMove       Listen_Loop1

	Type		male
	  Anim          male\listen01 133 168
	TEnd
MEnd

Move Listen_nod1
	Requires	ENCOUNTER LISTEN
	Priority        2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly
	NextMove        Ready
	CycleMove       Listen_Loop1
	CycleMove       Listen_shift1
	CycleMove       Listen_gesture1
	CycleMove       Listen_Loop1

	Type		male
	  Anim          male\listen01 168 220
	TEnd
MEnd

Move Listen_shift1
	Requires	ENCOUNTER LISTEN
	Priority        2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly
	NextMove        Listen_Loop2

	Type		male
	  Anim          male\listen01 220 246
	TEnd
MEnd

Move Listen_Loop2
	Requires	ENCOUNTER LISTEN
	Priority        2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly
	NextMove        Ready
	CycleMove       Listen_gesture2
	CycleMove       Listen_Loop2
	CycleMove       Listen_nod2
	CycleMove       Listen_shift2

	Type		male
	  Anim          male\listen01 246 305
	TEnd
MEnd

Move Listen_gesture2
	Requires	ENCOUNTER LISTEN
	Priority        2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly
	NextMove        Ready
	CycleMove       Listen_gesture2a
	CycleMove       Listen_gesture2b
	CycleMove       Listen_gesture2c

	Type		male
	  Anim          male\listen01 305 306
	TEnd
MEnd

Move Listen_gesture2a
	Requires	ENCOUNTER LISTEN
	Priority        2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly
	NextMove        Ready
	CycleMove       Listen_nod2
	CycleMove       Listen_shift2
	CycleMove       Listen_Loop2
	CycleMove       Listen_Loop2

	Type		male
	  Anim          male\listen01 306 352
	TEnd
MEnd

Move Listen_gesture2b
	Requires	ENCOUNTER LISTEN
	Priority        2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly
	NextMove        Ready
	CycleMove       Listen_nod2
	CycleMove       Listen_shift2
	CycleMove       Listen_Loop2
	CycleMove       Listen_Loop2

	Type		male
	  Anim          male\listen01 353 399
	TEnd
MEnd

Move Listen_gesture2c
	Requires	ENCOUNTER LISTEN
	Priority        2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly
	NextMove        Ready
	CycleMove       Listen_nod2
	CycleMove       Listen_shift2
	CycleMove       Listen_Loop2
	CycleMove       Listen_Loop2

	Type		male
	  Anim          male\listen01 400 446
	TEnd
MEnd

Move Listen_nod2
	Requires	ENCOUNTER LISTEN
	Priority        2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly
	NextMove        Ready
	CycleMove       Listen_Loop2
	CycleMove       Listen_shift2
	CycleMove       Listen_gesture2
	CycleMove       Listen_Loop2

	Type		male
	  Anim          male\listen01 446 492
	TEnd
MEnd

Move Listen_shift2
	Requires	ENCOUNTER LISTEN
	Priority        2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly
	NextMove        Listen_Loop1

	Type		male
	  Anim          male\listen01 492 518
	TEnd
MEnd

#############################################################
##  ReadBook
#############################################################

Move ReadBook_ReadBook01
	Requires	ENCOUNTER READBOOK
	Priority        2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly FixArmReg
	CycleMove       ReadBook_ReadBookTrans01
	CycleMove       ReadBook_ReadBookTrans01
	CycleMove       ReadBook_ReadBookTransAlt01
	Scale		0.5

	Type		male
	  Anim          male\ReadBook01 1 122
	  PlayFX	Emotes\ReadBook_Generic.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\ReadBook01 1 121
	  PlayFX	Emotes\ReadBook_Generic.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\ReadBook01 1 121
	  PlayFX	Emotes\ReadBook_Generic.fx 0 CONSTANT
	TEnd

	Type		BM
	  Anim          male\npc_ReadBook01 1 121
	  PlayFX	Emotes\ReadBook_Generic.fx 0 CONSTANT
	TEnd
MEnd

Move ReadBook_ReadBookTrans01
	Requires	ENCOUNTER READBOOK
	Priority        2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly FixArmReg
	CycleMove	ReadBook_ReadBook02

	Type		male
	  Anim          male\ReadBook01 122 140
	  PlayFX	Emotes\ReadBook_Generic.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\ReadBook01 121 139
	  PlayFX	Emotes\ReadBook_Generic.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\ReadBook01 121 139
	  PlayFX	Emotes\ReadBook_Generic.fx 0 CONSTANT
	TEnd

	Type		BM
	  Anim          male\npc_ReadBook01 121 139
	  PlayFX	Emotes\ReadBook_Generic.fx 0 CONSTANT
	TEnd
MEnd

Move ReadBook_ReadBook02
	Requires	ENCOUNTER READBOOK
	Priority        2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly FixArmReg
	CycleMove	ReadBook_ReadBookTrans02
	CycleMove	ReadBook_ReadBookTrans02
	CycleMove	ReadBook_ReadBookTransAlt02
	Scale		0.5

	Type		male
	  Anim          male\ReadBook01 139 260
	  PlayFX	Emotes\ReadBook_Generic.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\ReadBook01 139 259
	  PlayFX	Emotes\ReadBook_Generic.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\ReadBook01 139 259
	  PlayFX	Emotes\ReadBook_Generic.fx 0 CONSTANT
	TEnd

	Type		BM
	  Anim          male\npc_ReadBook01 139 259
	  PlayFX	Emotes\ReadBook_Generic.fx 0 CONSTANT
	TEnd
MEnd

Move ReadBook_ReadBookTrans02
	Requires	ENCOUNTER READBOOK
	Priority        2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly FixArmReg
	CycleMove	ReadBook_ReadBook01

	Type		male
	  Anim          male\ReadBook01 260 278
	  PlayFX	Emotes\ReadBook_Generic.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\ReadBook01 259 277
	  PlayFX	Emotes\ReadBook_Generic.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\ReadBook01 259 277
	  PlayFX	Emotes\ReadBook_Generic.fx 0 CONSTANT
	TEnd

	Type		BM
	  Anim          male\npc_ReadBook01 259 277
	  PlayFX	Emotes\ReadBook_Generic.fx 0 CONSTANT
	TEnd
MEnd

Move ReadBook_ReadBookTransAlt01
	Requires	ENCOUNTER READBOOK
	Priority        2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly FixArmReg
	CycleMove	ReadBook_ReadBook02

	Type		male
	  Anim          male\ReadBook01 279 339
	  PlayFX	Emotes\ReadBook_Generic.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\ReadBook01 278 338
	  PlayFX	Emotes\ReadBook_Generic.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\ReadBook01 278 338
	  PlayFX	Emotes\ReadBook_Generic.fx 0 CONSTANT
	TEnd

	Type		BM
	  Anim          male\npc_ReadBook01 278 338
	  PlayFX	Emotes\ReadBook_Generic.fx 0 CONSTANT
	TEnd
MEnd

Move ReadBook_ReadBookTransAlt02
	Requires	ENCOUNTER READBOOK
	Priority        2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly FixArmReg
	CycleMove	ReadBook_Readbook01

	Type		male
	  Anim          male\ReadBook01 340 376
	  PlayFX	Emotes\ReadBook_Generic.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\ReadBook01 339 375
	  PlayFX	Emotes\ReadBook_Generic.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\ReadBook01 339 375
	  PlayFX	Emotes\ReadBook_Generic.fx 0 CONSTANT
	TEnd

	Type		BM
	  Anim          male\npc_ReadBook01 339 375
	  PlayFX	Emotes\ReadBook_Generic.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
## Romulus Transformation - Cutscene
#############################################################

Move RomulusTransformation
	Requires	ENCOUNTER ROMULUSTRANSFORMATION
	Priority        2
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove        RomulusTransformation_Post
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_RomulusTransformation01 1 140
	  PlayFx        Scripted\RomulusNictus\Romulus_Transformation.fx 0 #CONSTANT
	TEnd
MEnd

Move RomulusTransformation_Post
	Requires	ENCOUNTER ROMULUSTRANSFORMATION
	Priority        2
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove	RomulusTransformation_PostCycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_RomulusTransformation01 140
	TEnd
MEnd

Move RomulusTransformation_PostCycle
	Requires	ENCOUNTER ROMULUSTRANSFORMATION
	Priority        2
	Include sequencers\Members\Encounters_Scripted.inc
	Flags		Cycle
	NextMove	postReady_back
	Scale		0.50

	Type		male
	  Anim		male\shield_Ready 1 31
	TEnd
MEnd

#############################################################
## OracleFloat
#############################################################

Move OracleFloat_In
	Requires	ENCOUNTER LEVITATE ONE
	Priority        3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	Flags		NoSizeScale
	NextMove	OracleFloat_Cycle

	Type		male
	  Anim          fem\OracleFloatTransIn01 1 46
	  PlayFX	Scripted\OracleFloating\OracleFloating.fx 0 CONSTANT
	TEnd
MEnd

Move OracleFloat_Cycle
	Requires	ENCOUNTER LEVITATE ONE
	Priority        3
	Member		<DeathIRQ>
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          fem\OracleFloat01 1 241
	  PlayFX	Scripted\OracleFloating\OracleFloating.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
##  OracleFloatTransOut
#############################################################

Move OracleFloat_Out
	Requires	ENCOUNTER LEVITATE TWO
	Priority        3
	Interrupts	OracleFloat_Cycle
	Include sequencers\Members\Encounters_Idle.inc
	Flags		NoSizeScale
	NextMove	OracleFloat_Out_PostCycle
	Interpolate	15

	Type		male
	  Anim          fem\OracleFloatTransOut01 1 61
	TEnd
MEnd

Move OracleFloat_Out_PostCycle
	Requires	ENCOUNTER LEVITATE TWO
	Priority        3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale
	NextMove	Ready

	Type		male
	  Anim          fem\Skel_ready2 1
	TEnd
MEnd

#############################################################
## Sit Talk
#############################################################

Move Emote_Sit_Talk_01_Intro
	Requires	ENCOUNTER SIT TALK MID
	Priority	3
	Interrupts	<IRQEmotesALL> <Ready> <PostMoveCycle> <MonsterGroup> <Combat> #<WepCombat> <TwoHandCombat> <DualHandCombat> <Block>
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale FixArmReg
	NextMove	Emote_Sit_Talk_01

	Type		male
	  Anim          male\Sit_Talk 1 133
	TEnd

	Type		fem
	  Anim          fem\Sit_Talk 1 133
	TEnd

	Type		huge
	  Anim          huge\Sit_Talk 1 133
	TEnd
MEnd

Move Emote_Sit_Talk_01
	Requires	ENCOUNTER SIT TALK MID
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly FixArmReg
	CycleMove       Emote_Sit_Talk_01
	CycleMove       Emote_Sit_Talk_01_Break_01
	CycleMove       Emote_Sit_Talk_01_Break_02
	CycleMove       Emote_Sit_Talk_Trans_01

	Type		male
	  Anim          male\Sit_Talk 1 133
	TEnd

	Type		fem
	  Anim          fem\Sit_Talk 1 133
	TEnd

	Type		huge
	  Anim          huge\Sit_Talk 1 133
	TEnd
MEnd

Move Emote_Sit_Talk_01_Break_01
	Requires	ENCOUNTER SIT TALK MID
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Sit_Talk_01

	Type		male
	  Anim          male\Sit_Talk 134 203
	TEnd

	Type		fem
	  Anim          fem\Sit_Talk 134 203
	TEnd

	Type		huge
	  Anim          huge\Sit_Talk 134 203
	TEnd
MEnd

Move Emote_Sit_Talk_01_Break_02
	Requires	ENCOUNTER SIT TALK MID
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Sit_Talk_01

	Type		male
	  Anim          male\Sit_Talk 204 264
	TEnd

	Type		fem
	  Anim          fem\Sit_Talk 204 264
	TEnd

	Type		huge
	  Anim          huge\Sit_Talk 204 264
	TEnd
MEnd

Move Emote_Sit_Talk_Trans_01
	Requires	ENCOUNTER SIT TALK MID
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Sit_Talk_02

	Type		male
	  Anim          male\Sit_Talk 265 286
	TEnd

	Type		fem
	  Anim          fem\Sit_Talk 265 286
	TEnd

	Type		huge
	  Anim          huge\Sit_Talk 265 286
	TEnd
MEnd

Move Emote_Sit_Talk_02
	Requires	ENCOUNTER SIT TALK MID
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly FixArmReg
	CycleMove       Emote_Sit_Talk_02
	CycleMove       Emote_Sit_Talk_02_Break_01
	CycleMove       Emote_Sit_Talk_02_Break_02
	CycleMove       Emote_Sit_Talk_Trans_02

	Type		male
	  Anim          male\Sit_Talk 287 423
	TEnd

	Type		fem
	  Anim          fem\Sit_Talk 287 423
	TEnd

	Type		huge
	  Anim          huge\Sit_Talk 287 423
	TEnd
MEnd

Move Emote_Sit_Talk_02_Break_01
	Requires	ENCOUNTER SIT TALK MID
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Sit_Talk_02

	Type		male
	  Anim          male\Sit_Talk 424 475
	TEnd

	Type		fem
	  Anim          fem\Sit_Talk 424 475
	TEnd

	Type		huge
	  Anim          huge\Sit_Talk 424 475
	TEnd
MEnd

Move Emote_Sit_Talk_02_Break_02
	Requires	ENCOUNTER SIT TALK MID
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Sit_Talk_02

	Type		male
	  Anim          male\Sit_Talk 476 520
	TEnd

	Type		fem
	  Anim          fem\Sit_Talk 476 520
	TEnd

	Type		huge
	  Anim          huge\Sit_Talk 476 520
	TEnd
MEnd

Move Emote_Sit_Talk_Trans_02
	Requires	ENCOUNTER SIT TALK MID
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Sit_Talk_01

	Type		male
	  Anim          male\Sit_Talk 521 556
	TEnd

	Type		fem
	  Anim          fem\Sit_Talk 521 556
	TEnd

	Type		huge
	  Anim          huge\Sit_Talk 521 556
	TEnd
MEnd

#############################################################
## Sit Listen
#############################################################

Move Emote_Sit_Listen_01
	Requires	ENCOUNTER SIT LISTEN MID
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly FixArmReg
	CycleMove       Emote_Sit_Listen_01
	CycleMove       Emote_Sit_Listen_01_Break_01
	CycleMove       Emote_Sit_Listen_01_Break_02
	CycleMove       Emote_Sit_Listen_Trans_01

	Type		male
	  Anim          male\Sit_Listen 1 113
	TEnd

	Type		fem
	  Anim          fem\Sit_Listen 1 113
	TEnd

	Type		huge
	  Anim          huge\Sit_Listen 1 113
	TEnd
MEnd

Move Emote_Sit_Listen_01_Break_01
	Requires	ENCOUNTER SIT LISTEN MID
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Sit_Listen_01

	Type		male
	  Anim          male\Sit_Listen 114 183
	TEnd

	Type		fem
	  Anim          fem\Sit_Listen 114 183
	TEnd

	Type		huge
	  Anim          huge\Sit_Listen 114 183
	TEnd
MEnd

Move Emote_Sit_Listen_01_Break_02
	Requires	ENCOUNTER SIT LISTEN MID
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Sit_Listen_01

	Type		male
	  Anim          male\Sit_Listen 184 253
	TEnd

	Type		fem
	  Anim          fem\Sit_Listen 184 253
	TEnd

	Type		huge
	  Anim          huge\Sit_Listen 184 253
	TEnd
MEnd

Move Emote_Sit_Listen_Trans_01
	Requires	ENCOUNTER SIT LISTEN MID
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Sit_Listen_02

	Type		male
	  Anim          male\Sit_Listen 254 276
	TEnd

	Type		fem
	  Anim          fem\Sit_Listen 254 276
	TEnd

	Type		huge
	  Anim          huge\Sit_Listen 254 276
	TEnd
MEnd

Move Emote_Sit_Listen_02
	Requires	ENCOUNTER SIT LISTEN MID
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly FixArmReg
	CycleMove       Emote_Sit_Listen_02
	CycleMove       Emote_Sit_Listen_02_Break_01
	CycleMove       Emote_Sit_Listen_02_Break_02
	CycleMove       Emote_Sit_Listen_Trans_02

	Type		male
	  Anim          male\Sit_Listen 277 388
	TEnd

	Type		fem
	  Anim          fem\Sit_Listen 277 388
	TEnd

	Type		huge
	  Anim          huge\Sit_Listen 277 388
	TEnd
MEnd

Move Emote_Sit_Listen_02_Break_01
	Requires	ENCOUNTER SIT LISTEN MID
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Sit_Listen_02

	Type		male
	  Anim          male\Sit_Listen 389 443
	TEnd

	Type		fem
	  Anim          fem\Sit_Listen 389 443
	TEnd

	Type		huge
	  Anim          huge\Sit_Listen 389 443
	TEnd
MEnd

Move Emote_Sit_Listen_02_Break_02
	Requires	ENCOUNTER SIT LISTEN MID
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Sit_Listen_02

	Type		male
	  Anim          male\Sit_Listen 444 488
	TEnd

	Type		fem
	  Anim          fem\Sit_Listen 444 488
	TEnd

	Type		huge
	  Anim          huge\Sit_Listen 444 488
	TEnd
MEnd

Move Emote_Sit_Listen_Trans_02
	Requires	ENCOUNTER SIT LISTEN MID
	Priority	3
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Sit_Listen_01

	Type		male
	  Anim          male\Sit_Listen 489 524
	TEnd

	Type		fem
	  Anim          fem\Sit_Listen 489 524
	TEnd

	Type		huge
	  Anim          huge\Sit_Listen 489 524
	TEnd
MEnd

#############################################################
##  Sit Talk Left
#############################################################

Move Emote_Sit_Talk_Left_01
	Requires	ENCOUNTER SIT TALK LEFT
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly FixArmReg
	CycleMove       Emote_Sit_Talk_Left_01
	CycleMove       Emote_Sit_Talk_Left_01_Break_01
	CycleMove       Emote_Sit_Talk_Left_01_Break_02
	CycleMove       Emote_Sit_Talk_Left_Trans_01

	Type		male
	  Anim          male\Sit_Talk_Left 1 133
	TEnd

	Type		fem
	  Anim          fem\Sit_Talk_Left 1 133
	TEnd

	Type		huge
	  Anim          huge\Sit_Talk_Left 1 133
	TEnd
MEnd

Move Emote_Sit_Talk_Left_01_Break_01
	Requires	ENCOUNTER SIT TALK LEFT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Sit_Talk_Left_01

	Type		male
	  Anim          male\Sit_Talk_Left 134 203
	TEnd

	Type		fem
	  Anim          fem\Sit_Talk_Left 134 203
	TEnd

	Type		huge
	  Anim          huge\Sit_Talk_Left 134 203
	TEnd
MEnd

Move Emote_Sit_Talk_Left_01_Break_02
	Requires	ENCOUNTER SIT TALK LEFT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Sit_Talk_Left_01

	Type		male
	  Anim          male\Sit_Talk_Left 204 264
	TEnd

	Type		fem
	  Anim          fem\Sit_Talk_Left 204 264
	TEnd

	Type		huge
	  Anim          huge\Sit_Talk_Left 204 264
	TEnd
MEnd

Move Emote_Sit_Talk_Left_Trans_01
	Requires	ENCOUNTER SIT TALK LEFT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Sit_Talk_Left_02

	Type		male
	  Anim          male\Sit_Talk_Left 265 286
	TEnd

	Type		fem
	  Anim          fem\Sit_Talk_Left 265 286
	TEnd

	Type		huge
	  Anim          huge\Sit_Talk_Left 265 286
	TEnd
MEnd

Move Emote_Sit_Talk_Left_02
	Requires	ENCOUNTER SIT TALK LEFT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly FixArmReg
	CycleMove       Emote_Sit_Talk_Left_02
	CycleMove       Emote_Sit_Talk_Left_02_Break_01
	CycleMove       Emote_Sit_Talk_Left_02_Break_02
	CycleMove       Emote_Sit_Talk_Left_Trans_02

	Type		male
	  Anim          male\Sit_Talk_Left 287 423
	TEnd

	Type		fem
	  Anim          fem\Sit_Talk_Left 287 423
	TEnd

	Type		huge
	  Anim          huge\Sit_Talk_Left 287 423
	TEnd
MEnd

Move Emote_Sit_Talk_Left_02_Break_01
	Requires	ENCOUNTER SIT TALK LEFT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Sit_Talk_Left_02

	Type		male
	  Anim          male\Sit_Talk_Left 424 475
	TEnd

	Type		fem
	  Anim          fem\Sit_Talk_Left 424 475
	TEnd

	Type		huge
	  Anim          huge\Sit_Talk_Left 424 475
	TEnd
MEnd

Move Emote_Sit_Talk_Left_02_Break_02
	Requires	ENCOUNTER SIT TALK LEFT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Sit_Talk_Left_02

	Type		male
	  Anim          male\Sit_Talk_Left 476 520
	TEnd

	Type		fem
	  Anim          fem\Sit_Talk_Left 476 520
	TEnd

	Type		huge
	  Anim          huge\Sit_Talk_Left 476 520
	TEnd
MEnd

Move Emote_Sit_Talk_Left_Trans_02
	Requires	ENCOUNTER SIT TALK LEFT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Sit_Talk_Left_01

	Type		male
	  Anim          male\Sit_Talk_Left 521 556
	TEnd

	Type		fem
	  Anim          fem\Sit_Talk_Left 521 556
	TEnd

	Type		huge
	  Anim          huge\Sit_Talk_Left 521 556
	TEnd
MEnd

#############################################################
##  Sit Listen Left
#############################################################

Move Emote_Sit_Listen_Left_01
	Requires	ENCOUNTER SIT LISTEN LEFT
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly FixArmReg
	CycleMove       Emote_Sit_Listen_Left_01
	CycleMove       Emote_Sit_Listen_Left_01_Break_01
	CycleMove       Emote_Sit_Listen_Left_01_Break_02
	CycleMove       Emote_Sit_Listen_Left_Trans_01

	Type		male
	  Anim          male\Sit_Listen_Left 1 113
	TEnd

	Type		fem
	  Anim          fem\Sit_Listen_Left 1 113
	TEnd

	Type		huge
	  Anim          huge\Sit_Listen_Left 1 113
	TEnd
MEnd

Move Emote_Sit_Listen_Left_01_Break_01
	Requires	ENCOUNTER SIT LISTEN LEFT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Sit_Listen_Left_01

	Type		male
	  Anim          male\Sit_Listen_Left 114 183
	TEnd

	Type		fem
	  Anim          fem\Sit_Listen_Left 114 183
	TEnd

	Type		huge
	  Anim          huge\Sit_Listen_Left 114 183
	TEnd
MEnd

Move Emote_Sit_Listen_Left_01_Break_02
	Requires	ENCOUNTER SIT LISTEN LEFT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Sit_Listen_Left_01

	Type		male
	  Anim          male\Sit_Listen_Left 184 253
	TEnd

	Type		fem
	  Anim          fem\Sit_Listen_Left 184 253
	TEnd

	Type		huge
	  Anim          huge\Sit_Listen_Left 184 253
	TEnd
MEnd

Move Emote_Sit_Listen_Left_Trans_01
	Requires	ENCOUNTER SIT LISTEN LEFT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Sit_Listen_Left_02

	Type		male
	  Anim          male\Sit_Listen_Left 254 276
	TEnd

	Type		fem
	  Anim          fem\Sit_Listen_Left 254 276
	TEnd

	Type		huge
	  Anim          huge\Sit_Listen_Left 254 276
	TEnd
MEnd

Move Emote_Sit_Listen_Left_02
	Requires	ENCOUNTER SIT LISTEN LEFT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly FixArmReg
	CycleMove       Emote_Sit_Listen_Left_02
	CycleMove       Emote_Sit_Listen_Left_02_Break_01
	CycleMove       Emote_Sit_Listen_Left_02_Break_02
	CycleMove       Emote_Sit_Listen_Left_Trans_02

	Type		male
	  Anim          male\Sit_Listen_Left 277 388
	TEnd

	Type		fem
	  Anim          fem\Sit_Listen_Left 277 388
	TEnd

	Type		huge
	  Anim          huge\Sit_Listen_Left 277 388
	TEnd
MEnd

Move Emote_Sit_Listen_Left_02_Break_01
	Requires	ENCOUNTER SIT LISTEN LEFT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Sit_Listen_Left_02

	Type		male
	  Anim          male\Sit_Listen_Left 389 443
	TEnd

	Type		fem
	  Anim          fem\Sit_Listen_Left 389 443
	TEnd

	Type		huge
	  Anim          huge\Sit_Listen_Left 389 443
	TEnd
MEnd

Move Emote_Sit_Listen_Left_02_Break_02
	Requires	ENCOUNTER SIT LISTEN LEFT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Sit_Listen_Left_02

	Type		male
	  Anim          male\Sit_Listen_Left 444 488
	TEnd

	Type		fem
	  Anim          fem\Sit_Listen_Left 444 488
	TEnd

	Type		huge
	  Anim          huge\Sit_Listen_Left 444 488
	TEnd
MEnd

Move Emote_Sit_Listen_Left_Trans_02
	Requires	ENCOUNTER SIT LISTEN LEFT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Sit_Listen_Left_01

	Type		male
	  Anim          male\Sit_Listen_Left 489 524
	TEnd

	Type		fem
	  Anim          fem\Sit_Listen_Left 489 524
	TEnd

	Type		huge
	  Anim          huge\Sit_Listen_Left 489 524
	TEnd
MEnd

#############################################################
##  Sit Talk Right
#############################################################

Move Emote_Sit_Talk_Right_01
	Requires	ENCOUNTER SIT TALK RIGHT
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly FixArmReg
	CycleMove       Emote_Sit_Talk_Right_01
	CycleMove       Emote_Sit_Talk_Right_01_Break_01
	CycleMove       Emote_Sit_Talk_Right_01_Break_02
	CycleMove       Emote_Sit_Talk_Right_Trans_01

	Type		male
	  Anim          male\Sit_Talk_Right 1 133
	TEnd

	Type		fem
	  Anim          fem\Sit_Talk_Right 1 133
	TEnd

	Type		huge
	  Anim          huge\Sit_Talk_Right 1 133
	TEnd
MEnd

Move Emote_Sit_Talk_Right_01_Break_01
	Requires	ENCOUNTER SIT TALK RIGHT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Sit_Talk_Right_01

	Type		male
	  Anim          male\Sit_Talk_Right 134 203
	TEnd

	Type		fem
	  Anim          fem\Sit_Talk_Right 134 203
	TEnd

	Type		huge
	  Anim          huge\Sit_Talk_Right 134 203
	TEnd
MEnd

Move Emote_Sit_Talk_Right_01_Break_02
	Requires	ENCOUNTER SIT TALK RIGHT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Sit_Talk_Right_01

	Type		male
	  Anim          male\Sit_Talk_Right 204 264
	TEnd

	Type		fem
	  Anim          fem\Sit_Talk_Right 204 264
	TEnd

	Type		huge
	  Anim          huge\Sit_Talk_Right 204 264
	TEnd
MEnd

Move Emote_Sit_Talk_Right_Trans_01
	Requires	ENCOUNTER SIT TALK RIGHT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Sit_Talk_Right_02

	Type		male
	  Anim          male\Sit_Talk_Right 265 286
	TEnd

	Type		fem
	  Anim          fem\Sit_Talk_Right 265 286
	TEnd

	Type		huge
	  Anim          huge\Sit_Talk_Right 265 286
	TEnd
MEnd

Move Emote_Sit_Talk_Right_02
	Requires	ENCOUNTER SIT TALK RIGHT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly FixArmReg
	CycleMove       Emote_Sit_Talk_Right_02
	CycleMove       Emote_Sit_Talk_Right_02_Break_01
	CycleMove       Emote_Sit_Talk_Right_02_Break_02
	CycleMove       Emote_Sit_Talk_Right_Trans_02

	Type		male
	  Anim          male\Sit_Talk_Right 287 423
	TEnd

	Type		fem
	  Anim          fem\Sit_Talk_Right 287 423
	TEnd

	Type		huge
	  Anim          huge\Sit_Talk_Right 287 423
	TEnd
MEnd

Move Emote_Sit_Talk_Right_02_Break_01
	Requires	ENCOUNTER SIT TALK RIGHT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Sit_Talk_Right_02

	Type		male
	  Anim          male\Sit_Talk_Right 424 475
	TEnd

	Type		fem
	  Anim          fem\Sit_Talk_Right 424 475
	TEnd

	Type		huge
	  Anim          huge\Sit_Talk_Right 424 475
	TEnd
MEnd

Move Emote_Sit_Talk_Right_02_Break_02
	Requires	ENCOUNTER SIT TALK RIGHT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Sit_Talk_Right_02

	Type		male
	  Anim          male\Sit_Talk_Right 476 520
	TEnd

	Type		fem
	  Anim          fem\Sit_Talk_Right 476 520
	TEnd

	Type		huge
	  Anim          huge\Sit_Talk_Right 476 520
	TEnd
MEnd

Move Emote_Sit_Talk_Right_Trans_02
	Requires	ENCOUNTER SIT TALK RIGHT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Sit_Talk_Right_01

	Type		male
	  Anim          male\Sit_Talk_Right 521 556
	TEnd

	Type		fem
	  Anim          fem\Sit_Talk_Right 521 556
	TEnd

	Type		huge
	  Anim          huge\Sit_Talk_Right 521 556
	TEnd
MEnd


#############################################################
##  Sit Listen Right
#############################################################

Move Emote_Sit_Listen_Right_01
	Requires	ENCOUNTER SIT LISTEN RIGHT
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly FixArmReg
	CycleMove       Emote_Sit_Listen_Right_01
	CycleMove       Emote_Sit_Listen_Right_01_Break_01
	CycleMove       Emote_Sit_Listen_Right_01_Break_02
	CycleMove       Emote_Sit_Listen_Right_Trans_01

	Type		male
	  Anim          male\Sit_Listen_Right 1 113
	TEnd

	Type		fem
	  Anim          fem\Sit_Listen_Right 1 113
	TEnd

	Type		huge
	  Anim          huge\Sit_Listen_Right 1 113
	TEnd
MEnd

Move Emote_Sit_Listen_Right_01_Break_01
	Requires	ENCOUNTER SIT LISTEN RIGHT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Sit_Listen_Right_01

	Type		male
	  Anim          male\Sit_Listen_Right 114 183
	TEnd

	Type		fem
	  Anim          fem\Sit_Listen_Right 114 183
	TEnd

	Type		huge
	  Anim          huge\Sit_Listen_Right 114 183
	TEnd
MEnd

Move Emote_Sit_Listen_Right_01_Break_02
	Requires	ENCOUNTER SIT LISTEN RIGHT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Sit_Listen_Right_01

	Type		male
	  Anim          male\Sit_Listen_Right 184 253
	TEnd

	Type		fem
	  Anim          fem\Sit_Listen_Right 184 253
	TEnd

	Type		huge
	  Anim          huge\Sit_Listen_Right 184 253
	TEnd
MEnd

Move Emote_Sit_Listen_Right_Trans_01
	Requires	ENCOUNTER SIT LISTEN RIGHT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Sit_Listen_Right_02

	Type		male
	  Anim          male\Sit_Listen_Right 254 276
	TEnd

	Type		fem
	  Anim          fem\Sit_Listen_Right 254 276
	TEnd

	Type		huge
	  Anim          huge\Sit_Listen_Right 254 276
	TEnd
MEnd

Move Emote_Sit_Listen_Right_02
	Requires	ENCOUNTER SIT LISTEN RIGHT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale PickRandomly FixArmReg
	CycleMove       Emote_Sit_Listen_Right_02
	CycleMove       Emote_Sit_Listen_Right_02_Break_01
	CycleMove       Emote_Sit_Listen_Right_02_Break_02
	CycleMove       Emote_Sit_Listen_Right_Trans_02

	Type		male
	  Anim          male\Sit_Listen_Right 277 388
	TEnd

	Type		fem
	  Anim          fem\Sit_Listen_Right 277 388
	TEnd

	Type		huge
	  Anim          huge\Sit_Listen_Right 277 388
	TEnd
MEnd

Move Emote_Sit_Listen_Right_02_Break_01
	Requires	ENCOUNTER SIT LISTEN RIGHT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Sit_Listen_Right_02

	Type		male
	  Anim          male\Sit_Listen_Right 389 443
	TEnd

	Type		fem
	  Anim          fem\Sit_Listen_Right 389 443
	TEnd

	Type		huge
	  Anim          huge\Sit_Listen_Right 389 443
	TEnd
MEnd

Move Emote_Sit_Listen_Right_02_Break_02
	Requires	ENCOUNTER SIT LISTEN RIGHT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Sit_Listen_Right_02

	Type		male
	  Anim          male\Sit_Listen_Right 444 488
	TEnd

	Type		fem
	  Anim          fem\Sit_Listen_Right 444 488
	TEnd

	Type		huge
	  Anim          huge\Sit_Listen_Right 444 488
	TEnd
MEnd

Move Emote_Sit_Listen_Right_Trans_02
	Requires	ENCOUNTER SIT LISTEN RIGHT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Sit_Listen_Right_01

	Type		male
	  Anim          male\Sit_Listen_Right 489 524
	TEnd

	Type		fem
	  Anim          fem\Sit_Listen_Right 489 524
	TEnd

	Type		huge
	  Anim          huge\Sit_Listen_Right 489 524
	TEnd
MEnd

#############################################################
##  Recluse_Collapse
#############################################################

Move RecluseCollapse_In
	Requires	ENCOUNTER COLLAPSE
	Priority        2
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	Flags		NoSizeScale
	NextMove	RecluseCollapse_Cycle

	Type		male
	  Anim          male\Recluse_Cutscene01 1 75
	TEnd
MEnd

Move RecluseCollapse_Cycle
	Requires	ENCOUNTER COLLAPSE
	Priority        2
	Include sequencers\Members\Encounters_Scripted.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\Recluse_Cutscene02 1 105
	TEnd
MEnd

#############################################################
##  Recluse_LookUp
#############################################################

Move Recluse_LookUp_In
	Requires	ENCOUNTER LOOKUP
	Priority        2
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	Flags		NoSizeScale
	NextMove	Recluse_LookUp_Cycle

	Type		male
	  Anim          male\Recluse_Cutscene03 1 83
	TEnd
MEnd

Move Recluse_LookUp_Cycle
	Requires	ENCOUNTER LOOKUP
	Priority        2
	Include sequencers\Members\Encounters_Scripted.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\Recluse_Cutscene04 1 124
	TEnd
MEnd