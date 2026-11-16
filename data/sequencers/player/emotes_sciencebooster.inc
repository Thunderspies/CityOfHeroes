#############################################################
## emotes_ScienceBooster.inc
#############################################################

## EXPERIMENT/MIX FORMULA ###########################################################

Move Emote_MixFormula
	Requires	EMOTE MIX FORMULA
	Priority        3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesShield> #<IRQEmotesRighthand> #<IRQEmotesLefthand> #<IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_MixFormula_Cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_experiment_ready 1 63
	  PlayFX	Emotes\TestTubes\TestTube_Continuing.fx 10 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_experiment_ready 1 63
	  PlayFX	Emotes\TestTubes\TestTube_Continuing.fx 10 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_experiment_ready 1 63
	  PlayFX	Emotes\TestTubes\TestTube_Continuing.fx 10 CONSTANT
	TEnd

	Type		5cw
	  Anim          5cw\skel_monster_cycle 1 63
	TEnd
MEnd

Move Emote_MixFormula_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NoSizeScale
	CycleMove	Emote_MixFormula_Break1
	CycleMove	Emote_MixFormula_Break2
	Scale		0.75

	Type		male
	  Anim          male\emote_experiment_cycle 1 214
	  PlayFX	Emotes\TestTubes\TestTube_Continuing.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_experiment_cycle 1 214
	  PlayFX	Emotes\TestTubes\TestTube_Continuing.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_experiment_cycle 1 214
	  PlayFX	Emotes\TestTubes\TestTube_Continuing.fx 0 CONSTANT
	TEnd

	Type		5cw
	  Anim          5cw\skel_monster_cycle 1 63
	TEnd
MEnd

Move Emote_MixFormula_Break1
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NoSizeScale
	CycleMove	Emote_MixFormula_Cycle
	CycleMove	Emote_MixFormula_Break2

	Type		male
	  Anim          male\emote_experiment_break1 1 254
	  PlayFX	Emotes\TestTubes\TestTube_Failure.fx 0
	  PlayFX	Emotes\TestTubes\BeakerShatter.fx 156
	TEnd

	Type		fem
	  Anim          fem\emote_experiment_break1 1 254
	  PlayFX	Emotes\TestTubes\TestTube_Failure.fx 0
	  PlayFX	Emotes\TestTubes\BeakerShatter.fx 156
	TEnd

	Type		huge
	  Anim          huge\emote_experiment_break1 1 254
	  PlayFX	Emotes\TestTubes\TestTube_Failure.fx 0
	  PlayFX	Emotes\TestTubes\BeakerShatter.fx 156
	TEnd

	Type		5cw
	  Anim          5cw\skel_monster_cycle 1 63
	TEnd
MEnd

Move Emote_MixFormula_Break2
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NoSizeScale
	CycleMove	Emote_MixFormula_Cycle

	Type		male
	  Anim          male\emote_experiment_break2 1 245
	  PlayFX	Emotes\TestTubes\TestTube_Success.fx 0
	TEnd

	Type		fem
	  Anim          fem\emote_experiment_break2 1 245
	  PlayFX	Emotes\TestTubes\TestTube_Success.fx 0
	TEnd

	Type		huge
	  Anim          huge\emote_experiment_break2 1 245
	  PlayFX	Emotes\TestTubes\TestTube_Success.fx 0
	TEnd

	Type		5cw
	  Anim          5cw\skel_monster_cycle 1 63
	TEnd
MEnd

#############################################################

Move A_Emote_MixFormula
	Requires	FLY EMOTE MIX FORMULA
	Priority        4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesShield> #<IRQEmotesRighthand> #<IRQEmotesLefthand> #<IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_MixFormula_Cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_experiment_ready_air 1 63
	  PlayFX	Emotes\TestTubes\TestTube_Continuing.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_experiment_ready_air 1 63
	  PlayFX	Emotes\TestTubes\TestTube_Continuing.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_experiment_ready_air 1 63
	  PlayFX	Emotes\TestTubes\TestTube_Continuing.fx 0 CONSTANT
	TEnd

	Type		5cw
	  Anim          5cw\mon_float 1
	TEnd
MEnd

Move A_Emote_MixFormula_Cycle
	Requires	FLY
	Priority        1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE NoSizeScale
	CycleMove	A_Emote_MixFormula_Break1
	CycleMove	A_Emote_MixFormula_Break2
	NextMove	PreFall
	Scale		0.75

	Type		male
	  Anim          male\emote_experiment_cycle_air 1 214
	  PlayFX	Emotes\TestTubes\TestTube_Continuing.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_experiment_cycle_air 1 214
	  PlayFX	Emotes\TestTubes\TestTube_Continuing.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_experiment_cycle_air 1 214
	  PlayFX	Emotes\TestTubes\TestTube_Continuing.fx 0 CONSTANT
	TEnd

	Type		5cw
	  Anim          5cw\mon_float 1
	TEnd
MEnd

Move A_Emote_MixFormula_Break1
	Requires	FLY
	Priority        1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE NoSizeScale
	CycleMove	A_Emote_MixFormula_Cycle
	CycleMove	A_Emote_MixFormula_Break2
	NextMove	PreFall

	Type		male
	  Anim          male\emote_experiment_break1_air 1 254
	  PlayFX	Emotes\TestTubes\TestTube_Failure.fx 0
	  PlayFX	Emotes\TestTubes\BeakerThrow.fx 155
	TEnd

	Type		fem
	  Anim          fem\emote_experiment_break1_air 1 254
	  PlayFX	Emotes\TestTubes\TestTube_Failure.fx 0
	  PlayFX	Emotes\TestTubes\BeakerThrow.fx 155
	TEnd

	Type		huge
	  Anim          huge\emote_experiment_break1_air 1 254
	  PlayFX	Emotes\TestTubes\TestTube_Failure.fx 0
	  PlayFX	Emotes\TestTubes\BeakerThrow.fx 155
	TEnd

	Type		5cw
	  Anim          5cw\mon_float 1
	TEnd
MEnd

Move A_Emote_MixFormula_Break2
	Requires	FLY
	Priority        1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE NoSizeScale
	CycleMove	A_Emote_MixFormula_Cycle
	NextMove	PreFall

	Type		male
	  Anim          male\emote_experiment_break2_air 1 245
	  PlayFX	Emotes\TestTubes\TestTube_Success.fx 0
	TEnd

	Type		fem
	  Anim          fem\emote_experiment_break2_air 1 245
	  PlayFX	Emotes\TestTubes\TestTube_Success.fx 0
	TEnd

	Type		huge
	  Anim          huge\emote_experiment_break2_air 1 245
	  PlayFX	Emotes\TestTubes\TestTube_Success.fx 0
	TEnd

	Type		5cw
	  Anim          5cw\mon_float 1
	TEnd
MEnd

## CHALKBOARD ###########################################################

Move Emote_Chalkboard
	Requires	EMOTE CALCULATE
	Priority        2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesShield> #<IRQEmotesRighthand> #<IRQEmotesLefthand> #<IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Chalkboard_Cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_chalkboard_ready 1 46
	  PlayFX	Emotes\Thinking\Thinking.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_chalkboard_ready 1 46
	  PlayFX	Emotes\Thinking\Thinking.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Chalkboard_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NoSizeScale
	CycleMove	Emote_Chalkboard_Cycle
	CycleMove	Emote_Chalkboard_Cycle
	CycleMove	Emote_Chalkboard_Break1
	CycleMove	Emote_Chalkboard_Break2

	Type		male
	  Anim          male\emote_chalkboard_cycle 1 40
	  PlayFX	Emotes\Thinking\Thinking.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_chalkboard_cycle 1 40
	  PlayFX	Emotes\Thinking\Thinking.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Chalkboard_Break1
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NoSizeScale
	CycleMove	Emote_Chalkboard_Cycle

	Type		male
	  Anim          male\emote_chalkboard_break1 1 179
	  PlayFX	Emotes\Thinking\Thinking.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_chalkboard_break1 1 179
	  PlayFX	Emotes\Thinking\Thinking.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Chalkboard_Break2
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NoSizeScale
	CycleMove	Emote_Chalkboard_Cycle

	Type		male
	  Anim          male\emote_chalkboard_break2 1 154
	  PlayFX	Emotes\Thinking\Thinking.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_chalkboard_break2 1 154
	  PlayFX	Emotes\Thinking\Thinking.fx 0 CONSTANT
	TEnd
MEnd

#############################################################

Move A_Emote_Chalkboard
	Requires	FLY EMOTE CALCULATE
	Priority        3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesShield> #<IRQEmotesRighthand> #<IRQEmotesLefthand> #<IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Chalkboard_Cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_chalkboard_ready_air 1 46
	  PlayFX	Emotes\Thinking\Thinking.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_chalkboard_ready_air 1 46
	  PlayFX	Emotes\Thinking\Thinking.fx 0 CONSTANT
	TEnd
MEnd

Move A_Emote_Chalkboard_Cycle
	Requires 	FLY
	Priority        1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE NoSizeScale
	CycleMove	A_Emote_Chalkboard_Cycle
	CycleMove	A_Emote_Chalkboard_Cycle
	CycleMove	A_Emote_Chalkboard_Break1
	CycleMove	A_Emote_Chalkboard_Break2
	NextMove	PreFall

	Type		male
	  Anim          male\emote_chalkboard_cycle_air 1 40
	  PlayFX	Emotes\Thinking\Thinking.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_chalkboard_cycle_air 1 40
	  PlayFX	Emotes\Thinking\Thinking.fx 0 CONSTANT
	TEnd
MEnd

Move A_Emote_Chalkboard_Break1
	Requires 	FLY
	Priority        1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE NoSizeScale
	CycleMove	A_Emote_Chalkboard_Cycle
	NextMove	PreFall

	Type		male
	  Anim          male\emote_chalkboard_break1_air 1 179
	  PlayFX	Emotes\Thinking\Thinking.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_chalkboard_break1_air 1 179
	  PlayFX	Emotes\Thinking\Thinking.fx 0 CONSTANT
	TEnd
MEnd

Move A_Emote_Chalkboard_Break2
	Requires 	FLY
	Priority        1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE NoSizeScale
	CycleMove	A_Emote_Chalkboard_Cycle
	NextMove	PreFall

	Type		male
	  Anim          male\emote_chalkboard_break2_air 1 154
	  PlayFX	Emotes\Thinking\Thinking.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_chalkboard_break2_air 1 154
	  PlayFX	Emotes\Thinking\Thinking.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
## Drink Formula
#############################################################

Move Emote_CostumeChange_DrinkFormula_Pre
	Requires	EMOTE CCCHANGE DRINK FORMULA ONE
	Priority        5
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> #<IRQEmotesShield>
	NextMove        Emote_CostumeChange_DrinkFormula
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_DrinkFormula 1 190
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\Beaker.fx 0 CONSTANT
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\SmokeBombChem.fx 185 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_DrinkFormula 1 190
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\Beaker.fx 0 CONSTANT
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\SmokeBombChem.fx 185 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_DrinkFormula 1 190
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\Beaker.fx 0 CONSTANT
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\SmokeBombChem.fx 185 CONSTANT
	TEnd
MEnd

Move Emote_CostumeChange_DrinkFormula
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Emote_CostumeChange_DrinkFormula_Post
	Sets            CANTMOVE COSTUMECHANGE
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_DrinkFormula 190 195
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\SmokeBombChem.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_DrinkFormula 190 195
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\SmokeBombChem.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_DrinkFormula 190 195
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\SmokeBombChem.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_CostumeChange_DrinkFormula_Post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Ready
	Flags		NoSizeScale
	Scale		0.5

	Type		male
	  Anim          male\emote_DrinkFormula 195
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\SmokeBombChem.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_DrinkFormula 195
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\SmokeBombChem.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_DrinkFormula 195
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\SmokeBombChem.fx 0 CONSTANT
	TEnd
MEnd

#############################################################

Move A_Emote_CostumeChange_DrinkFormula_Pre
	Requires	FLY EMOTE CCCHANGE DRINK FORMULA ONE
	Priority        6
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> #<IRQEmotesShield>
	NextMove        A_Emote_CostumeChange_DrinkFormula_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_drinkformula_air 1 190
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\Beaker.fx 0 CONSTANT
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\SmokeBombChem.fx 185 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_drinkformula_air 1 190
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\Beaker.fx 0 CONSTANT
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\SmokeBombChem.fx 185 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_drinkformula_air 1 190
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\Beaker.fx 0 CONSTANT
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\SmokeBombChem.fx 185 CONSTANT
	TEnd
MEnd

Move A_Emote_CostumeChange_DrinkFormula_Post
	Requires	FLY
	Priority        1
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove        A_Emote_CostumeChange_DrinkFormula_PostCycle
	Sets		CANTMOVE COSTUMECHANGE
	Flags		NoSizeScale
	Scale		0.5

	Type		male
	  Anim          male\emote_drinkformula_air 190 221
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\SmokeBombChem.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_drinkformula_air 190 221
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\SmokeBombChem.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_drinkformula_air 190 221
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\SmokeBombChem.fx 0 CONSTANT
	TEnd
MEnd

Move A_Emote_CostumeChange_DrinkFormula_PostCycle
	Include sequencers\Members\Emotes_Idle_Air.inc
	Requires	FLY
	Flags		CYCLE
	NextMove        Prefall

	Type		male
	  Anim          male\emote_drinkformula_air_cycle 1 79
	TEnd

	Type		huge
	  Anim          huge\emote_drinkformula_air_cycle 1 79
	TEnd

	Type		fem
	  Anim          fem\emote_drinkformula_air_cycle 1 79
	TEnd
MEnd

#############################################################

Move Emote_Shield_CostumeChange_DrinkFormula_Pre
	Requires	EMOTE SHIELD CCCHANGE DRINK FORMULA ONE
	Priority        6
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove        Emote_Shield_CostumeChange_DrinkFormula
	Sets            CANTMOVE
	Flags		NoSizeScale FIXARMREG

	Type		male
	  Anim          male\emote_drinkformula_shield 1 190
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\Beaker.fx 0 CONSTANT
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\SmokeBombChem.fx 185 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_drinkformula_shield 1 190
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\Beaker.fx 0 CONSTANT
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\SmokeBombChem.fx 185 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_DrinkFormula 1 190
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\Beaker.fx 0 CONSTANT
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\SmokeBombChem.fx 185 CONSTANT
	TEnd
MEnd

Move Emote_Shield_CostumeChange_DrinkFormula
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Emote_Shield_CostumeChange_DrinkFormula_Post
	Sets            CANTMOVE COSTUMECHANGE
	Flags		NoSizeScale FIXARMREG

	Type		male
	  Anim          male\emote_drinkformula_shield 190 195
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\SmokeBombChem.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_drinkformula_shield 190 195
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\SmokeBombChem.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_drinkformula_shield 190 195
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\SmokeBombChem.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Shield_CostumeChange_DrinkFormula_Post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Ready
	Flags		NoSizeScale FIXARMREG
	Scale		0.5

	Type		male
	  Anim          male\emote_drinkformula_shield 195
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\SmokeBombChem.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_drinkformula_shield 195
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\SmokeBombChem.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_drinkformula_shield 195
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\SmokeBombChem.fx 0 CONSTANT
	TEnd
MEnd

#############################################################

Move A_Emote_Shield_CostumeChange_DrinkFormula_Pre
	Requires	FLY EMOTE SHIELD CCCHANGE DRINK FORMULA ONE
	Priority        7
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove        A_Emote_Shield_CostumeChange_DrinkFormula_Post
	Sets            CANTMOVE
	Flags		NoSizeScale FIXARMREG

	Type		male
	  Anim          male\emote_drinkformula_shield_air 1 190
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\Beaker.fx 0 CONSTANT
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\SmokeBombChem.fx 185 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_drinkformula_shield_air 1 190
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\Beaker.fx 0 CONSTANT
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\SmokeBombChem.fx 185 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_drinkformula_shield_air 1 190
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\Beaker.fx 0 CONSTANT
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\SmokeBombChem.fx 185 CONSTANT
	TEnd
MEnd

Move A_Emote_Shield_CostumeChange_DrinkFormula_Post
	Requires	FLY SHIELD
	Priority        2
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove        A_Emote_Shield_CostumeChange_DrinkFormula_PostCycle
	Sets		CANTMOVE COSTUMECHANGE
	Flags		NoSizeScale FIXARMREG
	Scale		0.5

	Type		male
	  Anim          male\emote_drinkformula_shield_air 190 221
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\SmokeBombChem.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_drinkformula_shield_air 190 221
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\SmokeBombChem.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_drinkformula_shield_air 190 221
	  PlayFX	Emotes\CostumeChanges\ChemExplosion\SmokeBombChem.fx 0 CONSTANT
	TEnd
MEnd

Move A_Emote_Shield_CostumeChange_DrinkFormula_PostCycle
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\emote_drinkformula_shield_air_cycle 1 79
	TEnd

	Type		huge
	  Anim          huge\emote_drinkformula_shield_air_cycle 1 79
	TEnd

	Type		fem
	  Anim          fem\emote_drinkformula_shield_air_cycle 1 79
	TEnd
MEnd

#############################################################
## Injection Costume Change
#############################################################

Move Emote_CostumeChange_InjectSerum_Pre
	Requires	EMOTE CCCHANGE SERUM ONE
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove        Emote_CostumeChange_InjectSerum
	Sets		CANTMOVE
	Flags		NoSizeScale FixArmReg
	Interpolate	15

	Type		male
	  Anim		male\Paramilitary_CatchBoost 10 70
	  PlayFX	Emotes\CostumeChanges\Injection\Injection_Veins_NoShield.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim		fem\Paramilitary_CatchBoost 10 70
	  PlayFX	Emotes\CostumeChanges\Injection\Injection_Veins_NoShield_Female.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim		male\Paramilitary_CatchBoost 10 70
	  PlayFX	Emotes\CostumeChanges\Injection\Injection_Veins_NoShield_Huge.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_CostumeChange_InjectSerum
	NextMove        Emote_CostumeChange_InjectSerum_Change
	Sets		CANTMOVE
	Flags		NoSizeScale FixArmReg
	Scale		0.625

	Type		male
	  Anim		male\Paramilitary_CatchBoost 70 130
	  PlayFX	Emotes\CostumeChanges\Injection\Injection_Veins_NoShield.fx 0 CONSTANT
	  PlayFX	Emotes\CostumeChanges\Injection\BodyGlow2.fx 45 CONSTANT
	TEnd

	Type		fem
	  Anim		fem\Paramilitary_CatchBoost 70 130
	  PlayFX	Emotes\CostumeChanges\Injection\Injection_Veins_NoShield_Female.fx 0 CONSTANT
	  PlayFX	Emotes\CostumeChanges\Injection\BodyGlow2_Female.fx 45 CONSTANT
	TEnd

	Type		huge
	  Anim		male\Paramilitary_CatchBoost 70 130
	  PlayFX	Emotes\CostumeChanges\Injection\Injection_Veins_NoShield_Huge.fx 0 CONSTANT
	  PlayFX	Emotes\CostumeChanges\Injection\BodyGlow2_Huge.fx 45 CONSTANT
	TEnd
MEnd

Move Emote_CostumeChange_InjectSerum_Change
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_CostumeChange_InjectSerum_Post
	Sets		CANTMOVE COSTUMECHANGE
	Flags		NoSizeScale FixArmReg

	Type		male
	  Anim          male\comb_beatchest 1 95
	  PlayFX	Emotes\CostumeChanges\Injection\Injection_Veins_NoShield.fx 0 CONSTANT
	  PlayFX	Emotes\CostumeChanges\Injection\BodyGlow2.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\comb_beatchest 1 95
	  PlayFX	Emotes\CostumeChanges\Injection\Injection_Veins_NoShield_Female.fx 0 CONSTANT
	  PlayFX	Emotes\CostumeChanges\Injection\BodyGlow2_Female.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\comb_beatchest 1 95
	  PlayFX	Emotes\CostumeChanges\Injection\Injection_Veins_NoShield_Huge.fx 0 CONSTANT
	  PlayFX	Emotes\CostumeChanges\Injection\BodyGlow2_Huge.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_CostumeChange_InjectSerum_Post
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <BlockIRQ> <AttackIRQ> <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	Flags		Cycle
	NextMove        postReady_back

	Type		male
	  Anim          male\comb_beatchest 95 144
	TEnd

	Type		huge
	  Anim          huge\comb_beatchest 95 144
	TEnd

	Type		fem
	  Anim          fem\comb_beatchest 95 144
	TEnd
MEnd

#############################################################

Move A_Emote_CostumeChange_InjectSerum_Pre
	Requires	FLY EMOTE CCCHANGE SERUM ONE
	Priority	5
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> #<IRQEmotesShield>
	NextMove	A_Emote_CostumeChange_InjectSerum
	Sets		CANTMOVE
	Flags		NoSizeScale FixArmReg

	Type		male
	  Anim		male\Paramilitary_CatchBoost_Air 1 70
	  PlayFX	Emotes\CostumeChanges\Injection\Injection_Veins_NoShield.fx 0 CONSTANT
	TEnd

	Type		female
	  Anim		fem\Paramilitary_CatchBoost_Air 1 70
	  PlayFX	Emotes\CostumeChanges\Injection\Injection_Veins_NoShield_Female.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim		male\Paramilitary_CatchBoost_Air 1 70
	  PlayFX	Emotes\CostumeChanges\Injection\Injection_Veins_NoShield_Huge.fx 0 CONSTANT
	TEnd
MEnd

Move A_Emote_CostumeChange_InjectSerum
	Requires	FLY
	Priority	1
	NextMove	A_Emote_CostumeChange_InjectSerum_Change
	Sets		CANTMOVE
	Flags		NoSizeScale FixArmReg
	Scale		0.625

	Type		male
	  Anim		male\Paramilitary_CatchBoost_Air 70 130
	  PlayFX	Emotes\CostumeChanges\Injection\Injection_Veins_NoShield.fx 0 CONSTANT
	  PlayFX	Emotes\CostumeChanges\Injection\BodyGlow2.fx 45 CONSTANT
	TEnd

	Type		female
	  Anim		fem\Paramilitary_CatchBoost_Air 70 130
	  PlayFX	Emotes\CostumeChanges\Injection\Injection_Veins_NoShield_Female.fx 0 CONSTANT
	  PlayFX	Emotes\CostumeChanges\Injection\BodyGlow2_Female.fx 45 CONSTANT
	TEnd

	Type		huge
	  Anim		male\Paramilitary_CatchBoost_Air 70 130
	  PlayFX	Emotes\CostumeChanges\Injection\Injection_Veins_NoShield_Huge.fx 0 CONSTANT
	  PlayFX	Emotes\CostumeChanges\Injection\BodyGlow2_Huge.fx 45 CONSTANT
	TEnd
MEnd

Move A_Emote_CostumeChange_InjectSerum_Change
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Sets		CANTMOVE COSTUMECHANGE
	Flags		NoSizeScale FixArmReg
	NextMove        A_Emote_CustomeChange_InjectSerum_Post
	Interpolate	15

	Type		male
	  Anim		male\Paramilitary_CatchBoost_Air 130 166
	  PlayFX	Emotes\CostumeChanges\Injection\Injection_Veins_NoShield.fx 0 CONSTANT
	  PlayFX	Emotes\CostumeChanges\Injection\BodyGlow2.fx 0 CONSTANT
	TEnd

	Type		female
	  Anim		fem\Paramilitary_CatchBoost_Air 130 166
	  PlayFX	Emotes\CostumeChanges\Injection\Injection_Veins_NoShield_Female.fx 0 CONSTANT
	  PlayFX	Emotes\CostumeChanges\Injection\BodyGlow2_Female.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim		male\Paramilitary_CatchBoost_Air 130 166
	  PlayFX	Emotes\CostumeChanges\Injection\Injection_Veins_NoShield_Huge.fx 0 CONSTANT
	  PlayFX	Emotes\CostumeChanges\Injection\BodyGlow2_Huge.fx 0 CONSTANT
	TEnd
MEnd

Move A_Emote_CustomeChange_InjectSerum_Post
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove        Fake_C_flycycle
	Sets            CANTMOVE
	Flags		NoSizeScale FixArmReg

	Type		male
	  Anim          male\air_comb_beatchest 1 106
	  PlayFX	Emotes\CostumeChanges\Injection\Injection_Veins_NoShield.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\air_comb_beatchest 1 106
	  PlayFX	Emotes\CostumeChanges\Injection\Injection_Veins_NoShield_Huge.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\air_comb_beatchest 1 106
	  PlayFX	Emotes\CostumeChanges\Injection\Injection_Veins_NoShield_Female.fx 0 CONSTANT
	TEnd
MEnd

#############################################################

Move Emote_Shield_CostumeChange_InjectSerum
	Requires	EMOTE SHIELD CCCHANGE SERUM ONE
	Priority        5
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove        Emote_Shield_CostumeChange_InjectSerum_Post
	Sets            CANTMOVE
	Flags		NoSizeScale FixArmReg

	Type		male
	  Anim          male\emote_injection_shield 1 160
	  PlayFX	Emotes\CostumeChanges\Injection\Injection_Veins.fx 0 CONSTANT
	  PlayFX	Emotes\CostumeChanges\Injection\BodyGlow2.fx 120 CONSTANT
	TEnd

	Type		fem
	  Anim          male\emote_injection_shield 1 160
	  PlayFX	Emotes\CostumeChanges\Injection\Injection_Veins_Female.fx 0 CONSTANT
	  PlayFX	Emotes\CostumeChanges\Injection\BodyGlow2_Female.fx 120 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_injection_shield 1 160
	  PlayFX	Emotes\CostumeChanges\Injection\Injection_Veins_Huge.fx 0 CONSTANT
	  PlayFX	Emotes\CostumeChanges\Injection\BodyGlow2_Huge.fx 120 CONSTANT
	TEnd
MEnd

Move Emote_Shield_CostumeChange_InjectSerum_Post
	NextMove        Shield_CombatReady
	Sets            CANTMOVE COSTUMECHANGE
	Flags		NoSizeScale FixArmReg

	Type		male
	  Anim          male\emote_injection_shield 160
	  PlayFX	Emotes\CostumeChanges\Injection\Injection_Veins.fx 0 CONSTANT
	  PlayFX	Emotes\CostumeChanges\Injection\BodyGlow2.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          male\emote_injection_shield 160
	  PlayFX	Emotes\CostumeChanges\Injection\Injection_Veins_Female.fx 0 CONSTANT
	  PlayFX	Emotes\CostumeChanges\Injection\BodyGlow2_Female.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_injection_shield 160
	  PlayFX	Emotes\CostumeChanges\Injection\Injection_Veins_Huge.fx 0 CONSTANT
	  PlayFX	Emotes\CostumeChanges\Injection\BodyGlow2_Huge.fx 0 CONSTANT
	TEnd
MEnd

#############################################################

Move A_Emote_Shield_CostumeChange_InjectSerum
	Requires	FLY EMOTE CCCHANGE SERUM ONE SHIELD
	Priority	6
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove        A_Emote_Shield_CostumeChange_InjectSerum_Change
	Sets            CANTMOVE
	Flags		NoSizeScale FIXARMREG

	Type		male
	  Anim          male\emote_injection_shield_air 1 160
	  PlayFX	Emotes\CostumeChanges\Injection\Injection_Veins.fx 0 CONSTANT
	  PlayFX	Emotes\CostumeChanges\Injection\BodyGlow2.fx 120 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_injection_shield_air 1 160
	  PlayFX	Emotes\CostumeChanges\Injection\Injection_Veins_Huge.fx 0 CONSTANT
	  PlayFX	Emotes\CostumeChanges\Injection\BodyGlow2_Female.fx 120 CONSTANT
	TEnd

	Type		female
	  Anim          male\emote_injection_shield_air 1 160
	  PlayFX	Emotes\CostumeChanges\Injection\Injection_Veins_Female.fx 0 CONSTANT
	  PlayFX	Emotes\CostumeChanges\Injection\BodyGlow2_Huge.fx 120 CONSTANT
	TEnd
MEnd

Move A_Emote_Shield_CostumeChange_InjectSerum_Change
	Requires	FLY SHIELD
	Priority	2
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove        A_Emote_Shield_CostumeChange_InjectSerum_Cycle
	Sets            CANTMOVE COSTUMECHANGE
	Flags		NoSizeScale FIXARMREG

	Type		male
	  Anim          male\emote_injection_shield_air 160
	  PlayFX	Emotes\CostumeChanges\Injection\Injection_Veins.fx 0 CONSTANT
	  PlayFX	Emotes\CostumeChanges\Injection\BodyGlow2.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_injection_shield_air 160
	  PlayFX	Emotes\CostumeChanges\Injection\Injection_Veins_Huge.fx 0 CONSTANT
	  PlayFX	Emotes\CostumeChanges\Injection\BodyGlow2_Female.fx 0 CONSTANT
	TEnd

	Type		female
	  Anim          male\emote_injection_shield_air 160
	  PlayFX	Emotes\CostumeChanges\Injection\Injection_Veins_Female.fx 0 CONSTANT
	  PlayFX	Emotes\CostumeChanges\Injection\BodyGlow2_Huge.fx 0 CONSTANT
	TEnd
MEnd

Move A_Emote_Shield_CostumeChange_InjectSerum_Cycle
	Requires	FLY SHIELD
	Priority	2
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		NoSizeScale FIXARMREG Cycle
	Interpolate	15

	Type		male
	  Anim          male\emote_injection_shield_air_cycle 1 61
	TEnd

	Type		huge
	  Anim          male\emote_injection_shield_air_cycle 1 61
	TEnd

	Type		female
	  Anim          male\emote_injection_shield_air_cycle 1 61
	TEnd
MEnd