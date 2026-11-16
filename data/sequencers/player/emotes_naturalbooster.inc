#############################################################
## emotes_NaturalBooster.inc
#############################################################

## PUSHUP ###########################################################

Move Emote_Pushup_Intro
	Requires	EMOTE TRAIN PUSHUP
	Priority        3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove	Emote_Pushup_Cycle1
	Flags		FIXWEAPONREG
	Interpolate	0

	Type		male
	  Anim          male\emote_pushup 1 46
	  PlayFX	Emotes\Pushup.fx
	TEnd

	Type		fem
	  Anim          fem\emote_pushup 1 46
	TEnd

	Type		huge
	  Anim          huge\emote_pushup 1 46
	TEnd

	Type		5cw
	  Anim          5cw\skel_monster_cycle 1 63
	TEnd
MEnd

Move Emote_Pushup_Cycle1
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXWEAPONREG
	CycleMove	Emote_Pushup_Cycle1
	CycleMove	Emote_Pushup_Cycle1
	CycleMove	Emote_Pushup_Cycle1
	CycleMove	Emote_Pushup_Break1
	Interpolate	0

	Type		male
	  Anim          male\emote_pushup 46 72
	  PlayFX	Emotes\Pushup.fx
	TEnd

	Type		fem
	  Anim          fem\emote_pushup 46 72
	TEnd

	Type		huge
	  Anim          huge\emote_pushup 46 72
	TEnd

	Type		5cw
	  Anim          5cw\skel_monster_cycle 1 63
	TEnd
MEnd

Move Emote_Pushup_Break1
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXWEAPONREG
	CycleMove	Emote_Pushup_Break1
	CycleMove	Emote_Pushup_Cycle1
	CycleMove	Emote_Pushup_Cycle1
	CycleMove	Emote_Pushup_Cycle2_Intro
	Interpolate	0

	Type		male
	  Anim          male\emote_pushup 100 123
	  PlayFX	Emotes\Pushup.fx
	  PlayFX	Emotes\Pushup_Clap.fx 10
	  PlayFX	Emotes\Pushup_Hand.fx 18
	TEnd

	Type		fem
	  Anim          fem\emote_pushup 100 123
	TEnd

	Type		huge
	  Anim          huge\emote_pushup 100 123
	TEnd

	Type		5cw
	  Anim          5cw\skel_monster_cycle 1 63
	TEnd
MEnd

Move Emote_Pushup_Cycle2_Intro
	Include sequencers\Members\Emotes_Idle.inc
	Flags		FIXWEAPONREG
	NextMove	Emote_Pushup_Cycle2
	Interpolate	0

	Type		male
	  Anim          male\emote_pushup 151 180
	  PlayFX	Emotes\Pushup.fx
	  PlayFX	Emotes\Pushup_Hand.fx 20
	TEnd

	Type		fem
	  Anim          fem\emote_pushup 151 180
	TEnd

	Type		huge
	  Anim          huge\emote_pushup 151 180
	TEnd

	Type		5cw
	  Anim          5cw\skel_monster_cycle 1 63
	TEnd
MEnd

Move Emote_Pushup_Cycle2
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXWEAPONREG
	CycleMove	Emote_Pushup_Cycle2
	CycleMove	Emote_Pushup_Cycle2
	CycleMove	Emote_Pushup_Cycle2
	CycleMove	Emote_Pushup_Cycle2_end
	Interpolate	0

	Type		male
	  Anim          male\emote_pushup 180 209
	  PlayFX	Emotes\Pushup.fx
	TEnd

	Type		fem
	  Anim          fem\emote_pushup 180 209
	TEnd

	Type		huge
	  Anim          huge\emote_pushup 180 209
	TEnd

	Type		5cw
	  Anim          5cw\skel_monster_cycle 1 63
	TEnd
MEnd

Move Emote_Pushup_Cycle2_end
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Pushup_Cycle1
	Interpolate	0

	Type		male
	  Anim          male\emote_pushup 209 240
	  PlayFX	Emotes\Pushup.fx
	  PlayFX	Emotes\Pushup_Hand.fx 22
	TEnd

	Type		fem
	  Anim          fem\emote_pushup 209 240
	TEnd

	Type		huge
	  Anim          huge\emote_pushup 209 240
	TEnd

	Type		5cw
	  Anim          5cw\skel_monster_cycle 1 63
	TEnd
MEnd

#############################################################

Move A_Emote_Pushup_Intro
	Requires	FLY EMOTE TRAIN PUSHUP
	Priority        4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesShield> #<IRQEmotesRighthand> #<IRQEmotesLefthand> #<IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Pushup_Cycle1

	Type		male
	  Anim          male\emote_pushup 1 46
	TEnd

	Type		fem
	  Anim          fem\emote_pushup 1 46
	TEnd

	Type		huge
	  Anim          huge\emote_pushup 1 46
	TEnd

	Type		5cw
	  Anim          5cw\skel_monster_cycle 1 63
	TEnd
MEnd

Move A_Emote_Pushup_Cycle1
	Requires	FLY
	Priority        1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE
	CycleMove	A_Emote_Pushup_Cycle1
	CycleMove	A_Emote_Pushup_Cycle1
	CycleMove	A_Emote_Pushup_Cycle1
	CycleMove	A_Emote_Pushup_Break1
	NextMove	PreFall

	Type		male
	  Anim          male\emote_pushup 46 72
	TEnd

	Type		fem
	  Anim          fem\emote_pushup 46 72
	TEnd

	Type		huge
	  Anim          huge\emote_pushup 46 72
	TEnd

	Type		5cw
	  Anim          5cw\skel_monster_cycle 1 63
	TEnd
MEnd

Move A_Emote_Pushup_Break1
	Requires	FLY
	Priority        1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE
	CycleMove	A_Emote_Pushup_Break1
	CycleMove	A_Emote_Pushup_Cycle1
	CycleMove	A_Emote_Pushup_Cycle1
	CycleMove	A_Emote_Pushup_Cycle2_Intro
	NextMove	PreFall

	Type		male
	  Anim          male\emote_pushup 100 123
	TEnd

	Type		fem
	  Anim          fem\emote_pushup 100 123
	TEnd

	Type		huge
	  Anim          huge\emote_pushup 100 123
	TEnd

	Type		5cw
	  Anim          5cw\skel_monster_cycle 1 63
	TEnd
MEnd

Move A_Emote_Pushup_Cycle2_Intro
	Requires	FLY
	Priority        1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE
	CycleMove	A_Emote_Pushup_Cycle2
	NextMove	PreFall

	Type		male
	  Anim          male\emote_pushup 151 180
	TEnd

	Type		fem
	  Anim          fem\emote_pushup 151 180
	TEnd

	Type		huge
	  Anim          huge\emote_pushup 151 180
	TEnd

	Type		5cw
	  Anim          5cw\skel_monster_cycle 1 63
	TEnd
MEnd

Move A_Emote_Pushup_Cycle2
	Requires	FLY
	Priority        1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE
	NextMove	PreFall

	Type		male
	  Anim          male\emote_pushup 180 209
	TEnd

	Type		fem
	  Anim          fem\emote_pushup 180 209
	TEnd

	Type		huge
	  Anim          huge\emote_pushup 180 209
	TEnd

	Type		5cw
	  Anim          5cw\skel_monster_cycle 1 63
	TEnd
MEnd

## BRING IT ###########################################################

Move Emote_Bring_It
	Requires	EMOTE BRINGIT
	Priority        2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Bring_It_Cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_bringit 1 106
	  PlayFX	Emotes\BringIt.fx
	TEnd
MEnd

Move Emote_Bring_It_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NoSizeScale
	CycleMove	Emote_Bring_It_Cycle

	Type		male
	  Anim          male\emote_bringit 106 165
	TEnd
MEnd

#############################################################

Move A_Emote_Bring_It
	Requires	FLY EMOTE BRINGIT
	Priority        13
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Bring_It_Cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_emote_bringit 1 106
	TEnd
MEnd

Move A_Emote_Bring_It_Cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE NoSizeScale
	NextMove	PreFall

	Type		male
	  Anim          male\air_emote_bringit 106 165
	TEnd
MEnd

#############################################################
## Costume Change - Ninja Jump
#############################################################

Move Emote_CostumeChange_Ninja_Jump
	Requires	EMOTE CCCHANGE NINJAJUMP
	Priority	3
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Nextmove	Emote_CostumeChange_Ninja_Jump_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  PlayFX	Emotes\CostumeChanges\NinjaJump\NinjaJump_FadeOut.fx 10
	  Anim          male\emote_ninjajump 1 19
	TEnd
MEnd

Move Emote_CostumeChange_Ninja_Jump_Post
	Include sequencers\Members\Emotes_Idle.inc
	Nextmove	Emote_CostumeChange_Ninja_Jump_PostCycle
	Sets		CANTMOVE COSTUMECHANGE
	Flags		NoSizeScale

	Type		male
	  PlayFX	Emotes\CostumeChanges\NinjaJump\NinjaJump_FadeIn.fx 19
	  Anim          male\emote_ninjajump 19 40
	TEnd
MEnd

Move Emote_CostumeChange_Ninja_Jump_PostCycle
	Include sequencers\Members\Emotes_Idle.inc
	Nextmove	Ready_2Hips
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_ninjajump 40 112
	TEnd
MEnd

#############################################################

Move A_Emote_CostumeChange_Ninja_Jump
	Requires	FLY EMOTE CCCHANGE NINJAJUMP
	Priority	14
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Nextmove	A_Emote_CostumeChange_Ninja_Jump_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_emote_ninjajump 1 21
	  PlayFX	Emotes\CostumeChanges\NinjaJump\NinjaJump_FadeOut.fx 10
	TEnd
MEnd

Move A_Emote_CostumeChange_Ninja_Jump_Post
	Include sequencers\Members\Emotes_Idle_Air.inc
	Nextmove	A_Emote_CostumeChange_Ninja_Jump_PostCycle
	Sets		CANTMOVE COSTUMECHANGE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_emote_ninjajump 21 40
	  PlayFX	Emotes\CostumeChanges\NinjaJump\NinjaJump_FadeIn.fx 19
	TEnd
MEnd

Move A_Emote_CostumeChange_Ninja_Jump_PostCycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		NoSizeScale
	NextMove        A_Emote_CostumeChange_Ninja_Jump_PostCycle2

	Type		male
	  Anim          male\air_emote_ninjajump 40 90
	TEnd
MEnd

Move A_Emote_CostumeChange_Ninja_Jump_PostCycle2
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		NoSizeScale reqinputs CYCLE
	NextMove	PreFall

	Type		male
	  Anim          male\air_emote_ninjajump 90
	TEnd
MEnd

#############################################################
## Costume Change - Smoke Flash
#############################################################

Move Emote_CostumeChange_SmokeFlash
	Requires	EMOTE CCCHANGE SMOKEBOMB
	Priority        3
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove        Emote_CostumeChange_SmokeFlash_Post
	Interpolate	8
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\smokeflash 1 50
	  PlayFX	Emotes\CostumeChanges\SmokeBomb\SmokeBomb.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_CostumeChange_SmokeFlash_Post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Fake_CombatCycle
	Sets		CANTMOVE COSTUMECHANGE
	Flags		NoSizeScale

	Type		male
	  Anim          male\smokeflash 50
	  PlayFX	Emotes\CostumeChanges\SmokeBomb\SmokeBomb.fx 0 CONSTANT
	TEnd
MEnd

#############################################################

Move A_Emote_CostumeChange_SmokeFlash
	Requires	FLY EMOTE CCCHANGE SMOKEBOMB
	Priority	14
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove	A_Emote_CostumeChange_SmokeFlash_Post
	Interpolate	8
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_smokeflash 1 51
	  PlayFX	Emotes\CostumeChanges\SmokeBomb\SmokeBomb.fx 45 CONSTANT
	TEnd
MEnd

Move A_Emote_CostumeChange_SmokeFlash_Post
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	Fake_C_flycycle
	Sets		COSTUMECHANGE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_smokeflash 51
	  PlayFX	Emotes\CostumeChanges\SmokeBomb\SmokeBomb.fx 0 CONSTANT
	TEnd
MEnd

#############################################################