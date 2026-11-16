#############################################################
## emotes_MutantBooster.inc
#############################################################


## Dimension Shift ###########################################################

Move Emote_Dimension_Shift_Pre
	Requires	EMOTE CCCHANGE SHIFT
	Priority        3
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove	Emote_Dimension_Shift_Cycle
	Flags		NoSizeScale
	SticksOnChild	EMOTE SHIFT

	Type		male
	  Anim          male\flameaura 1 55
	playfx		"Emotes\DimensionShift\DimensionShift_Emote_Male.fx" 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\fireaura 1 55
	playfx		"Emotes\DimensionShift\DimensionShift_Emote_Huge.fx" 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\fireaura 1 55
	playfx		"Emotes\DimensionShift\DimensionShift_Emote_Female.fx" 0 CONSTANT
	TEnd

MEnd

Move Emote_Dimension_Shift_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Dimension_Shift_Post
	Flags		NoSizeScale
	Sets		CANTMOVE COSTUMECHANGE

	Type		male
	  Anim          male\emote_dimension_shift 1
	TEnd

MEnd

Move Emote_Dimension_Shift_Post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\flameaura 55
	playfx		"Emotes\DimensionShift\BodyGlow.fx" 0

	TEnd

	Type		huge
	  Anim          huge\fireaura 55
	playfx		"Emotes\DimensionShift\BodyGlow.fx" 0

	TEnd

	Type		fem
	  Anim          fem\fireaura 55
	playfx		"Emotes\DimensionShift\BodyGlow.fx" 0

	TEnd
MEnd

## AIR Dimension Shift ###########################################################

Move A_Emote_Dimension_Shift_Pre
	Requires	FLY EMOTE CCCHANGE SHIFT
	Priority        14
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove	A_Emote_Dimension_Shift_Cycle
	Flags		NoSizeScale
	SticksOnChild	FLY EMOTE SHIFT

	Type		male
	  Anim          male\air_fireaura 1 55
	playfx		"Emotes\DimensionShift\DimensionShift_Emote_Male.fx" 0 CONSTANT

	TEnd

	Type		huge
	  Anim          huge\air_fireaura 1 55
	playfx		"Emotes\DimensionShift\DimensionShift_Emote_Huge.fx" 0 CONSTANT

	TEnd

	Type		fem
	  Anim          fem\air_fireaura 1 55
	playfx		"Emotes\DimensionShift\DimensionShift_Emote_Female.fx" 0 CONSTANT

	TEnd

MEnd

Move A_Emote_Dimension_Shift_Cycle
	Requires	FLY
	Priority	1
	NextMove	A_Emote_Dimension_Shift_Post
	Flags		NoSizeScale
	Sets		CANTMOVE COSTUMECHANGE

	Type		male
	  Anim          male\emote_dimension_shift 1
	TEnd

MEnd

Move A_Emote_Dimension_Shift_Post
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove        FlyReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_fireaura 55
	playfx		"Emotes\DimensionShift\BodyGlow.fx" 0

	TEnd

	Type		huge
	  Anim          huge\air_fireaura 55
	playfx		"Emotes\DimensionShift\BodyGlow.fx" 0

	TEnd

	Type		fem
	  Anim          fem\air_fireaura 55
	playfx		"Emotes\DimensionShift\BodyGlow.fx" 0

	TEnd
MEnd

## Growl ###########################################################

Move Emote_Mutant_Growl_Pre
	Requires	EMOTE GROWL
	Priority        2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Mutant_Growl_Cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_growl 1 39
	  PlayFX		Emotes\Growl\Growl_Start.fx
	TEnd

	Type		fem
	  Anim          fem\emote_growl 1 39
	  PlayFX		Emotes\Growl\Growl_Start.fx
	TEnd

	Type		huge
	  Anim          huge\emote_growl 1 39
	  PlayFX		Emotes\Growl\Growl_Start.fx
	TEnd

	Type		5cw
	  Anim          5cw\skel_monster_cycle 1 63
	TEnd

MEnd

Move Emote_Mutant_Growl_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE
	CycleMove	Emote_Mutant_Growl_Cycle
	CycleMove	Emote_Mutant_Growl_Cycle
	CycleMove	Emote_Mutant_Growl_Cycle
	CycleMove	Emote_Mutant_Growl_Break
	Interpolate	0

	Type		male
	  Anim          male\emote_growl 40 54
	  PlayFX		Emotes\Growl\Growl_Breath.fx 1 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_growl 40 54
	  PlayFX		Emotes\Growl\Growl_Breath.fx 1 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_growl 40 54
	  PlayFX		Emotes\Growl\Growl_Breath.fx 1 CONSTANT
	TEnd

	Type		5cw
	  Anim          5cw\skel_monster_cycle 1 63
	TEnd

MEnd


Move Emote_Mutant_Growl_Break
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	CycleMove	Emote_Mutant_Growl_Cycle

	Type		male
	  Anim          male\emote_growl 55 85
	  PlayFX		Emotes\Growl\Growl_Swipe.fx 5
	TEnd

	Type		fem
	  Anim          fem\emote_growl 55 85
	  PlayFX		Emotes\Growl\Growl_Swipe.fx 5
	TEnd

	Type		huge
	  Anim          huge\emote_growl 55 85
	  PlayFX		Emotes\Growl\Growl_Swipe.fx 5
	TEnd

	Type		5cw
	  Anim          5cw\skel_monster_cycle 1 63
	TEnd

MEnd

#############################################################
## Costume Change - Pod Person
#############################################################

Move Emote_CostumeChange_Pod_Person_Pre
	Requires	EMOTE CCCHANGE MORPH
	Priority        4
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove        Emote_CostumeChange_Pod_Person_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_pod_person 1 52
	PlayFX		Emotes\CostumeChanges\Pod\PodPerson.fx 0
	TEnd

	Type		fem
	  Anim          fem\emote_pod_person 1 52
	PlayFX		Emotes\CostumeChanges\Pod\PodPerson.fx 0
	TEnd

MEnd

Move Emote_CostumeChange_Pod_Person_Post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Ready
	Flags		NoSizeScale
	Sets            COSTUMECHANGE

	Type		male
	  Anim          male\emote_pod_person 52
	TEnd

	Type		fem
	  Anim          fem\emote_pod_person 52
	TEnd


MEnd

#############################################################

Move A_Emote_CostumeChange_Pod_Person_Pre
	Requires	FLY EMOTE CCCHANGE MORPH
	Priority        14
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove        A_Emote_CostumeChange_Pod_Person_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_emote_pod_person 1 52
	PlayFX		Emotes\CostumeChanges\Pod\PodPerson_Fly.fx 0 CONSTANT
	TEnd

MEnd

Move A_Emote_CostumeChange_Pod_Person_Post
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove        FlyReady
	Flags		NoSizeScale
	Sets            COSTUMECHANGE

	Type		male
	  Anim          male\air_emote_pod_person 53
	PlayFX		Emotes\CostumeChanges\Pod\PodPerson_Fly.fx 0 CONSTANT
	TEnd

MEnd

#############################################################
## Costume Change - Rapid Boil
#############################################################

Move Emote_CostumeChange_RapidBoil_Pre
	Requires	EMOTE CCCHANGE TRANSFORM
	Priority        3
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove        Emote_CostumeChange_RapidBoil
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_Rapidboil 1 65
	  PlayFX	Emotes\CostumeChanges\Tumors\Tumors_CostumeChange.fx 0
	TEnd

	Type		fem
	  Anim          fem\emote_Rapidboil 1 65
	  PlayFX	Emotes\CostumeChanges\Tumors\Tumors_CostumeChange_Female.fx 0
	TEnd

	Type		huge
	  Anim          fem\emote_Rapidboil 1 65
	  PlayFX	Emotes\CostumeChanges\Tumors\Tumors_CostumeChange_Huge.fx 0
	TEnd
MEnd

Move Emote_CostumeChange_RapidBoil
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Emote_CostumeChange_RapidBoil_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_Rapidboil 66 70
	TEnd

	Type		fem
	  Anim          fem\emote_Rapidboil 66 70
	TEnd

	Type		huge
	  Anim          fem\emote_Rapidboil 66 70
	TEnd
MEnd

Move Emote_CostumeChange_RapidBoil_Post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Ready
	Flags		NoSizeScale
	Sets            COSTUMECHANGE
	Scale		0.5

	Type		male
	  Anim          male\emote_Rapidboil 71
	TEnd

	Type		fem
	  Anim          fem\emote_Rapidboil 71
	TEnd

	Type		huge
	  Anim          fem\emote_Rapidboil 71
	TEnd
MEnd

#############################################################

Move A_Emote_CostumeChange_RapidBoil_Pre
	Requires	FLY EMOTE CCCHANGE TRANSFORM
	Priority        14
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove        A_Emote_CostumeChange_RapidBoil_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_emote_Rapidboil 1 65
	  PlayFX	Emotes\CostumeChanges\Tumors\Tumors_CostumeChange_Fly.fx 0
	TEnd

	Type		fem
	  Anim          male\air_emote_Rapidboil 1 65
	  PlayFX	Emotes\CostumeChanges\Tumors\Tumors_CostumeChange_Female_Fly.fx 0
	TEnd

	Type		huge
	  Anim          male\air_emote_Rapidboil 1 65
	  PlayFX	Emotes\CostumeChanges\Tumors\Tumors_CostumeChange_Huge_Fly.fx 0
	TEnd
MEnd

Move A_Emote_CostumeChange_RapidBoil_Post
	Requires	FLY
	Priority        1
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove        A_Emote_CostumeChange_RapidBoil_PostCycle
	Sets		CANTMOVE
	Flags		NoSizeScale
	Scale		0.5

	Type		male
	  Anim          male\air_emote_Rapidboil 66 70
	TEnd

	Type		fem
	  Anim          male\air_emote_Rapidboil 66 70
	TEnd

	Type		huge
	  Anim          male\air_emote_Rapidboil 66 70
	TEnd
MEnd

Move A_Emote_CostumeChange_RapidBoil_PostCycle
	Include sequencers\Members\Emotes_Idle_Air.inc
	Requires	FLY
	Flags		NoSizeScale
	Sets		COSTUMECHANGE
	NextMove        Prefall

	Type		male
	  Anim          male\air_emote_Rapidboil 71
	TEnd
MEnd

###############################################

Move Emote_Boast
	Requires	EMOTE BOAST
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Ready
	Scale		0.85

	Type		male
	  Anim          male\emote_boast 1
	TEnd
MEnd

Move A_Emote_Boast
	Requires	FLY EMOTE BOAST
	Priority	13
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	FlyReady
	Scale		0.85

	Type		male
	  Anim          male\air_emote_boast 1
	TEnd
MEnd

##################################
###### Secondary Mutant Boost_up#####
##################################

Move Mutation_Boostup
	Requires	MUTATION BOOSTUP
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Mutation_Boostuppost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\absorb 1 10
	TEnd

	Type		huge
	  Anim          huge\boostup 1 10
	TEnd

	Type		fem
	  Anim          fem\boostup 1 10
	TEnd

MEnd

Move Mutation_Boostuppost
	Member		<DeathIRQ> <AttackIRQ> <BlockIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove        Mutation_BoostupSuperPost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\absorb 11 40
	TEnd

	Type		huge
	  Anim          huge\boostup 11 40
	TEnd

	Type		fem
	  Anim          fem\boostup 11 40
	TEnd

MEnd

Move Mutation_BoostupSuperPost
	Member		<DeathIRQ> <AttackIRQ> <BlockIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove        Mutation_BoostupReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\absorb 40
	TEnd

	Type		huge
	  Anim          huge\boostup 40
	TEnd

	Type		fem
	  Anim          fem\boostup 40
	TEnd

MEnd


Move Mutation_BoostupReady
	Include sequencers\Members\Readies.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\Skel_ready2 1 60
	TEnd

	Type		huge
	  Anim          huge\mon_ready 1 60
	TEnd

	Type		fem
	  Anim          fem\Skel_ready2 1 60
	TEnd

MEnd