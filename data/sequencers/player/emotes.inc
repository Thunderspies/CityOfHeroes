#############################################################
## Emotes.inc - Emotes
#############################################################
## Interrupts
## <IRQEmotesALL> - Other emotes, basic readies
## <IRQEmotesCombat> - Combat cycles
## <IRQEmotesRighthand> - Right-Handed weapon combat cycles (Broadsword, Mace, etc)
## <IRQEmotesLefthand> - Left-Handed weapon combat cycles (Katana, Archery, etc)
## <IRQEmotesDualhand> - Dual Wield weapon combat cycles (Dual Blades, Dual pistols, Claws, etc)
## <IRQEmotesTwohand> - Two handed weapon combat cycles (Assault Rifle, Stone Mallet, etc)

#############################################################
## Teabag Emote
#############################################################

Move Teabag_Pre
	Requires	EMOTE TEABAG
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Teabag
	Flags		FIXARMREG

	Type		male
	  Anim          male\emote_teabag 1 10
	  PlayFX	generic\teabag.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_teabag 1 10
	  PlayFX	generic\teabag.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_teabag 1 10
	  PlayFX	generic\teabag.fx 0 CONSTANT
	TEnd
MEnd

Move Teabag
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Teabag_cycle
	Flags		FIXARMREG

	Type		male
	  Anim          male\emote_teabag 10
	  PlayFX	generic\teabag.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_teabag 10
	  PlayFX	generic\teabag.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_teabag 10
	  PlayFX	generic\teabag.fx 0 CONSTANT
	TEnd
MEnd

Move Teabag_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\emote_teabag 1
	  PlayFX	generic\teabag.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_teabag 1
	  PlayFX	generic\teabag.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_teabag 1
	  PlayFX	generic\teabag.fx 0 CONSTANT
	TEnd
MEnd


#############################################################
## Invention - Emote
#############################################################

Move Invention
	Requires	EMOTE INVENT
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Invention_cycle
	Flags		FIXARMREG

	Type		male
	  Anim          male\invention_intro 15
	  PlayFX	generic\InventionWidget.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\invention_intro 15
	  PlayFX	generic\InventionWidget.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\invention_intro 15
	  PlayFX	generic\InventionWidget.fx 0 CONSTANT
	TEnd
MEnd

Move Invention_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		FIXARMREG CYCLE
	CycleMove	Invention_cycle
	CycleMove	Invention_cycle
	CycleMove	Invention_examine
	CycleMove	Invention_ponder

	Type		male
	  Anim          male\invention_loop 1
	  PlayFX	generic\InventionWidget.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\invention_loop 1
	  PlayFX	generic\InventionWidget.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\invention_loop 1
	  PlayFX	generic\InventionWidget.fx 0 CONSTANT
	TEnd
MEnd

Move Invention_examine
	Include sequencers\Members\Emotes_Idle.inc
	Flags		FIXARMREG CYCLE
	CycleMove	Invention_cycle
	CycleMove	Invention_cycle
	CycleMove	Invention_cycle
	CycleMove	Invention_ponder

	Type		male
	  Anim          male\invention_break1 1
	  PlayFX	generic\InventionWidget.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\invention_break1 1
	  PlayFX	generic\InventionWidget.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\invention_break1 1
	  PlayFX	generic\InventionWidget.fx 0 CONSTANT
	TEnd
MEnd

Move Invention_ponder
	Include sequencers\Members\Emotes_Idle.inc
	Flags		FIXARMREG CYCLE
	CycleMove	Invention_cycle
	CycleMove	Invention_cycle
	CycleMove	Invention_cycle
	CycleMove	Invention_examine

	Type		male
	  Anim          male\invention_break2 1
	  PlayFX	generic\InventionWidget.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\invention_break2 1
	  PlayFX	generic\InventionWidget.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\invention_break2 1
	  PlayFX	generic\InventionWidget.fx 0 CONSTANT
	TEnd
MEnd


#############################################################
## Random Dance - Dance Base
#############################################################

Move D_Base
	Requires	EMOTE DANCE LOW
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	D_Base_cycle
	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\dance_base 1
	TEnd
MEnd

Move D_Base_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\dance_base 1
	TEnd
MEnd

#############################################################

Move Dance1
	Requires	EMOTE DANCE ONE
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesShield> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	D_Base_cycle

	Type		male
	  Anim          male\dance_base 1
	TEnd
MEnd

#############################################################
## Random Dance - Dance Block
#############################################################

Move D_Block
	Requires	EMOTE DANCE LOW
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	D_Block_cycle
	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\dance_block 1
	TEnd
MEnd

Move D_Block_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\dance_block 1
	TEnd
MEnd

#############################################################

Move Dance2
	Requires	EMOTE DANCE TWO
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	D_Block_cycle

	Type		male
	  Anim          male\dance_block 1
	TEnd
MEnd

#############################################################
## Random Dance - Dance Washer
#############################################################

Move D_Washer
	Requires	EMOTE DANCE LOW
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	D_Washer_cycle
	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\dance_washer 1
	TEnd
MEnd

Move D_Washer_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\dance_washer 1
	TEnd
MEnd

#############################################################

Move Dance3
	Requires	EMOTE DANCE THREE
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	D_Washer_cycle

	Type		male
	  Anim          male\dance_washer 1
	TEnd
MEnd

#############################################################
## Random Dance - Dance Armsup
#############################################################

Move D_Armsup
	Requires	EMOTE DANCE LOW
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	D_Armsup_cycle
	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\dance_armsup 1
	TEnd
MEnd

Move D_Armsup_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\dance_armsup 1
	TEnd
MEnd

#############################################################

Move Dance4
	Requires	EMOTE DANCE FOUR
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	D_Armsup_cycle

	Type		male
	  Anim          male\dance_armsup 1
	TEnd
MEnd

#############################################################
## Random Dance - Dance Bounce
#############################################################

Move D_Bounce
	Requires	EMOTE DANCE LOW
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	D_Bounce_cycle
	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\dance_bounce 1
	TEnd
MEnd

Move D_Bounce_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\dance_bounce 1
	TEnd
MEnd

#############################################################

Move Dance5
	Requires	EMOTE DANCE FIVE
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	D_Bounce_cycle

	Type		male
	  Anim          male\dance_bounce 1
	TEnd
MEnd

#############################################################
## Random Dance - Dance Monkey
#############################################################

Move D_Monkey
	Requires	EMOTE DANCE LOW
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	D_Monkey_cycle
	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\dance_monkey 1
	TEnd
MEnd

Move D_Monkey_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\dance_monkey 1
	TEnd
MEnd

#############################################################

Move Dance6
	Requires	EMOTE DANCE SIX
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	D_Monkey_cycle

	Type		male
	  Anim          male\dance_monkey 1
	TEnd
MEnd

#############################################################
## Eat Donut
#############################################################

Move Emote_Eat_Donut_Pre1
	Requires	EMOTE EATDONUT
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> #<IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NOSIZESCALE PickRandomly
	NextMove	Emote_Eat_Donut_Pre2
	Scale		0.875

	Type		male
	  Anim          male\emote_eat_bite1 1 2
	  PlayFx        Generic\Eat_Donut.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_eat_bite1 1 2
	  PlayFx        Generic\Eat_Donut.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_eat_bite1 1 2
	  PlayFx        Generic\Eat_Donut.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Eat_Donut_Pre2
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NOSIZESCALE PickRandomly
	NextMove	Emote_Eat_Donut_Loop1
	Scale		0.875

	Type		male
	  Anim          male\emote_eat_bite1 3
	  PlayFx        Generic\Eat_Donut.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_eat_bite1 3
	  PlayFx        Generic\Eat_Donut.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_eat_bite1 3
	  PlayFx        Generic\Eat_Donut.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Eat_Donut_Loop1
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NOSIZESCALE
	Scale		0.875
	CycleMove	Emote_Eat_Donut_Loop1
	CycleMove	Emote_Eat_Donut_Loop2
	CycleMove	Emote_Eat_Donut_Bite1

	Type		male
	  Anim          male\emote_eat_loop 1
	  PlayFx        Generic\Eat_Donut.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_eat_loop 1
	  PlayFx        Generic\Eat_Donut.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_eat_loop 1
	  PlayFx        Generic\Eat_Donut.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Eat_Donut_Loop2
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NOSIZESCALE #PickRandomly
	Scale		0.75
	CycleMove	Emote_Eat_Donut_Bite1
	CycleMove	Emote_Eat_Donut_Bite1
	CycleMove	Emote_Eat_Donut_Loop1

	Type		male
	  Anim          male\emote_eat_loop2 1
	  PlayFx        Generic\Eat_Donut.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_eat_loop2 1
	  PlayFx        Generic\Eat_Donut.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_eat_loop2 1
	  PlayFx        Generic\Eat_Donut.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Eat_Donut_Bite1
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NOSIZESCALE #PickRandomly
	CycleMove	Emote_Eat_Donut_Wipe
	CycleMove	Emote_Eat_Donut_Wipe
	CycleMove	Emote_Eat_Donut_Bite1
	CycleMove	Emote_Eat_Donut_Loop1

	Type		male
	  Anim          male\emote_eat_bite1 1
	  PlayFx        Generic\Eat_Donut.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_eat_bite1 1
	  PlayFx        Generic\Eat_Donut.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_eat_bite1 1
	  PlayFx        Generic\Eat_Donut.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Eat_Donut_Wipe
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NOSIZESCALE #PickRandomly
	CycleMove	Emote_Eat_Donut_Loop1
	CycleMove	Emote_Eat_Donut_Bite1

	Type		male
	  Anim          male\emote_eat_wipe 1
	  PlayFx        Generic\Eat_Donut.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_eat_wipe 1
	  PlayFx        Generic\Eat_Donut.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_eat_wipe 1
	  PlayFx        Generic\Eat_Donut.fx 0 CONSTANT
	TEnd
MEnd


#############################################################
##  Eat Food
#############################################################

Move Emote_Eat_Food_Pre1
	Requires	EMOTE EATFOOD
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> #<IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Eat_Food_Pre2

	Type		male
	  Anim          male\emote_eat_bite1 1 2
	  PlayFx        Generic\Eat_Food.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_eat_bite1 1 2
	  PlayFx        Generic\Eat_Food.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_eat_bite1 1 2
	  PlayFx        Generic\Eat_Food.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Eat_Food_Pre2
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Eat_Food_Loop1

	Type		male
	  Anim          male\emote_eat_bite1 3
	  PlayFx        Generic\Eat_Food.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_eat_bite1 3
	  PlayFx        Generic\Eat_Food.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_eat_bite1 3
	  PlayFx        Generic\Eat_Food.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Eat_Food_Loop1
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE
	CycleMove	Emote_Eat_Food_Loop1
	CycleMove	Emote_Eat_Food_Loop2
	CycleMove	Emote_Eat_Food_Bite1

	Type		male
	  Anim          male\emote_eat_loop 1
	  PlayFx        Generic\Eat_Food.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_eat_loop 1
	  PlayFx        Generic\Eat_Food.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_eat_loop 1
	  PlayFx        Generic\Eat_Food.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Eat_Food_Loop2
	Include sequencers\Members\Emotes_Idle.inc
	Scale		0.9
	Flags		CYCLE
	CycleMove	Emote_Eat_Food_Bite1
	CycleMove	Emote_Eat_Food_Bite1
	CycleMove	Emote_Eat_Food_Loop1

	Type		male
	  Anim          male\emote_eat_loop2 1
	  PlayFx        Generic\Eat_Food.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_eat_loop2 1
	  PlayFx        Generic\Eat_Food.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_eat_bite1 1
	  PlayFx        Generic\Eat_Food.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Eat_Food_Bite1
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE
	CycleMove	Emote_Eat_Food_Wipe
	CycleMove	Emote_Eat_Food_Wipe
	CycleMove	Emote_Eat_Food_Bite1
	CycleMove	Emote_Eat_Food_Loop1

	Type		male
	  Anim          male\emote_eat_bite1 1
	  PlayFx        Generic\Eat_Food.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_eat_bite1 1
	  PlayFx        Generic\Eat_Food.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_eat_bite1 1
	  PlayFx        Generic\Eat_Food.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Eat_Food_Wipe
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE
	CycleMove	Emote_Eat_Food_Loop1
	CycleMove	Emote_Eat_Food_Bite1

	Type		male
	  Anim          male\emote_eat_wipe 1
	  PlayFx        Generic\Eat_Food.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_eat_wipe 1
	  PlayFx        Generic\Eat_Food.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_eat_wipe 1
	  PlayFx        Generic\Eat_Food.fx 0 CONSTANT
	TEnd
MEnd


#############################################################
## Listening to Police Band
#############################################################

Move ListenPoliceBand_Intro
	Requires	EMOTE LISTENPOLICEBAND
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	ListenPoliceBand_Cycle
	Flags		FIXARMREG

	Type		male
	  Anim          male\emote_ListenPoliceBand_intro 1
	  PlayFX	generic\PoliceBandScanner.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_ListenPoliceBand_intro 1
	  PlayFX	generic\PoliceBandScanner.fx 0 CONSTANT
	TEnd
#
#	Type		huge
#	  Anim          huge\emote_ListenPoliceBand_intro 1
#	  PlayFX	generic\PoliceBandScanner.fx 0 CONSTANT
#	TEnd
#
#	Type		v_male
#	  Anim          male\newspaper_cycle 1 156
#	  PlayFx        GENERIC\FX_newspaper_rogue.FX 0 CONSTANT
#	TEnd
#
#	Type		v_fem
#	  Anim          fem\fem_newspaper 1 156
#	  PlayFx        GENERIC\FX_newspaper_female_rogue.FX 0 CONSTANT
#	TEnd
#
#	Type		v_huge
#	  Anim          huge\AFK_newspaper 1 156
#	  PlayFx        GENERIC\FX_newspaper_huge_rogue.FX 0 CONSTANT
#	TEnd

	Type		beast
	  Anim          Beast\lookquick 1
	TEnd
MEnd

Move ListenPoliceBand_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		FIXARMREG CYCLE
	CycleMove	ListenPoliceBand_Cycle

	Type		male
	  Anim          male\emote_ListenPoliceBand_loop 1
	  PlayFX	generic\PoliceBandScanner.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_ListenPoliceBand_loop 1
	  PlayFX	generic\PoliceBandScanner.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_ListenPoliceBand_loop 1
	  PlayFX	generic\PoliceBandScanner.fx 0 CONSTANT
	TEnd
#
#	Type		v_male
#	  Anim          male\newspaper_cycle 1 156
#	  PlayFx        GENERIC\FX_newspaper_rogue.FX 0 CONSTANT
#	TEnd
#
#	Type		v_fem
#	  Anim          fem\fem_newspaper 1 156
#	  PlayFx        GENERIC\FX_newspaper_female_rogue.FX 0 CONSTANT
#	TEnd
#
#	Type		v_huge
#	  Anim          huge\AFK_newspaper 1 156
#	  PlayFx        GENERIC\FX_newspaper_huge_rogue.FX 0 CONSTANT
#	TEnd

	Type		beast
	  Anim          Beast\lookquick 1
	TEnd
MEnd


#############################################################
## Listening to Stolen Police Band
#############################################################

Move ListenStolenPoliceBand_Intro
	Requires	EMOTE LISTENSTOLENPOLICEBAND
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> #<IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	ListenStolenPoliceBand_Cycle
	Flags		FIXARMREG

	Type		male
	  Anim          male\emote_ListenPoliceBand_intro 1
	  PlayFX	generic\PoliceBandScanner.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_ListenPoliceBand_intro 1
	  PlayFX	generic\PoliceBandScanner.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_ListenPoliceBand_intro 1
	  PlayFX	generic\PoliceBandScanner.fx 0 CONSTANT
	TEnd

	Type		Beast
	  Anim          male\MonComb_cycle 1
	TEnd
MEnd

Move ListenStolenPoliceBand_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		FIXARMREG CYCLE
	CycleMove	ListenStolenPoliceBand_Cycle
	NextMove	prefall

	Type		male
	  Anim          male\emote_ListenPoliceBand_loop 1
	  PlayFX	generic\PoliceBandScanner.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_ListenPoliceBand_loop 1
	  PlayFX	generic\PoliceBandScanner.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_ListenPoliceBand_loop 1
	  PlayFX	generic\PoliceBandScanner.fx 0 CONSTANT
	TEnd

	Type		Beast
	  Anim          male\MonComb_cycle 1
	TEnd
MEnd

#############################################################
## Listening to Police Band (for in game radio only)
#############################################################

Move InGameOnly_ListenPoliceBand_Intro
	Requires	EMOTE LISTENPOLICEBAND ALTERNATE ONE
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> #<IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	InGameOnly_ListenPoliceBand_Cycle
	Flags		FIXARMREG

	Type		male
	  Anim          male\emote_ListenPoliceBand_intro 1
	  PlayFX	generic\PoliceBandScanner.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_ListenPoliceBand_intro 1
	  PlayFX	generic\PoliceBandScanner.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_ListenPoliceBand_intro 1
	  PlayFX	generic\PoliceBandScanner.fx 0 CONSTANT
	TEnd
MEnd

Move InGameOnly_ListenPoliceBand_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		FIXARMREG CYCLE
	CycleMove	ListenPoliceBand_Cycle

	Type		male
	  Anim          male\emote_ListenPoliceBand_loop 1
	  PlayFX	generic\PoliceBandScanner.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_ListenPoliceBand_loop 1
	  PlayFX	generic\PoliceBandScanner.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_ListenPoliceBand_loop 1
	  PlayFX	generic\PoliceBandScanner.fx 0 CONSTANT
	TEnd

MEnd

#############################################################
## Sitting Poses
#############################################################

Move Emote_Sitting_Pose1_Pre
	Requires	EMOTE SIT POSE ONE
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		FIXARMREG
	NextMove	Emote_Sitting_Pose1_Cycle

	Type		male
	  Anim          male\emote_sitting_poses 1 2
	TEnd

	Type		fem
	  Anim          fem\emote_sitting_poses 1 2
	TEnd

	Type		huge
	  Anim          huge\emote_sitting_poses 1 2
	TEnd
MEnd

Move Emote_Sitting_Pose1_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\emote_sitting_poses 1 2
	TEnd

	Type		fem
	  Anim          fem\emote_sitting_poses 1 2
	TEnd

	Type		huge
	  Anim          huge\emote_sitting_poses 1 2
	TEnd
MEnd

#############################################################

Move Emote_Sitting_Pose2_Intro
	Requires	EMOTE SIT POSE TWO
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		FIXARMREG
	NextMove	Emote_Sitting_Pose2_Loop

	Type		male
	  Anim          male\emote_sitting_poses 3 4
	TEnd

	Type		fem
	  Anim          fem\emote_sitting_poses 3 4
	TEnd

	Type		huge
	  Anim          huge\emote_sitting_poses 3 4
	TEnd
MEnd

Move Emote_Sitting_Pose2_Loop
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG
	CycleMove	Emote_Sitting_Pose2_Loop

	Type		male
	  Anim          male\emote_sitting_poses 3 4
	TEnd

	Type		fem
	  Anim          fem\emote_sitting_poses 3 4
	TEnd

	Type		huge
	  Anim          huge\emote_sitting_poses 3 4
	TEnd
MEnd

#############################################################

Move Emote_Sitting_Pose3_Intro
	Requires	EMOTE SIT POSE THREE
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> #<IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		FIXARMREG
	NextMove	Emote_Sitting_Pose3_Loop

	Type		male
	  Anim          male\emote_sitting_poses 5 6
	TEnd

	Type		fem
	  Anim          fem\emote_sitting_poses 5 6
	TEnd

	Type		huge
	  Anim          huge\emote_sitting_poses 5 6
	TEnd
MEnd

Move Emote_Sitting_Pose3_Loop
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG
	CycleMove	Emote_Sitting_Pose3_Loop

	Type		male
	  Anim          male\emote_sitting_poses 5 6
	TEnd

	Type		fem
	  Anim          fem\emote_sitting_poses 5 6
	TEnd

	Type		huge
	  Anim          huge\emote_sitting_poses 5 6
	TEnd
MEnd

#############################################################

Move Emote_Sitting_Pose4_Intro
	Requires	EMOTE SIT POSE FOUR
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> #<IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		FIXARMREG
	NextMove	Emote_Sitting_Pose4_Loop

	Type		male
	  Anim          male\emote_sitting_poses 7 8
	TEnd

	Type		fem
	  Anim          fem\emote_sitting_poses 7 8
	TEnd

	Type		huge
	  Anim          huge\emote_sitting_poses 7 8
	TEnd
MEnd

Move Emote_Sitting_Pose4_Loop
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG
	CycleMove	Emote_Sitting_Pose4_Loop

	Type		male
	  Anim          male\emote_sitting_poses 7 8
	TEnd

	Type		fem
	  Anim          fem\emote_sitting_poses 7 8
	TEnd

	Type		huge
	  Anim          huge\emote_sitting_poses 7 8
	TEnd
MEnd

#############################################################

Move Emote_Sitting_Pose5_Intro
	Requires	EMOTE SIT POSE FIVE
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> #<IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		FIXARMREG
	NextMove	Emote_Sitting_Pose5_Loop

	Type		male
	  Anim          male\emote_sitting_poses 9 10
	TEnd

	Type		fem
	  Anim          fem\emote_sitting_poses 9 10
	TEnd

	Type		huge
	  Anim          huge\emote_sitting_poses 9 10
	TEnd
MEnd

Move Emote_Sitting_Pose5_Loop
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG
	CycleMove	Emote_Sitting_Pose5_Loop

	Type		male
	  Anim          male\emote_sitting_poses 9 10
	TEnd

	Type		fem
	  Anim          fem\emote_sitting_poses 9 10
	TEnd

	Type		huge
	  Anim          huge\emote_sitting_poses 9 10
	TEnd
MEnd

#############################################################

Move Emote_Sitting_Pose6_Intro
	Requires	EMOTE SIT POSE SIX
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		FIXARMREG
	NextMove	Emote_Sitting_Pose6_Loop

	Type		male
	  Anim          male\emote_sitting_poses 11 12
	TEnd

	Type		fem
	  Anim          fem\emote_sitting_poses 9 10
	TEnd

	Type		huge
	  Anim          huge\emote_sitting_poses 11 12
	TEnd
MEnd

Move Emote_Sitting_Pose6_Loop
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG
	CycleMove	Emote_Sitting_Pose6_Loop

	Type		male
	  Anim          male\emote_sitting_poses 11 12
	TEnd

	Type		fem
	  Anim          fem\emote_sitting_poses 9 10
	TEnd

	Type		huge
	  Anim          huge\emote_sitting_poses 11 12
	TEnd
MEnd

#############################################################

Move Emote_Sitting_Pose7_Intro
	Requires	EMOTE SIT POSE SEVEN
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		FIXARMREG
	NextMove	Emote_Sitting_Pose7_Loop

	Type		male
	  Anim          male\emote_sitting_poses 13 14
	TEnd

	Type		fem
	  Anim          fem\emote_sitting_poses 9 10
	TEnd

	Type		huge
	  Anim          huge\emote_sitting_poses 13 14
	TEnd
MEnd

Move Emote_Sitting_Pose7_Loop
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG
	CycleMove	Emote_Sitting_Pose7_Loop

	Type		male
	  Anim          male\emote_sitting_poses 13 14
	TEnd

	Type		fem
	  Anim          fem\emote_sitting_poses 9 10
	TEnd

	Type		huge
	  Anim          huge\emote_sitting_poses 13 14
	TEnd
MEnd

#############################################################

Move Emote_Sitting_Pose8_Intro
	Requires	EMOTE SIT POSE EIGHT
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		FIXARMREG
	NextMove	Emote_Sitting_Pose8_Loop

	Type		male
	  Anim          male\emote_sitting_poses 15 16
	TEnd

	Type		fem
	  Anim          fem\emote_sitting_poses 9 10
	TEnd

	Type		huge
	  Anim          huge\emote_sitting_poses 15 16
	TEnd
MEnd

Move Emote_Sitting_Pose8_Loop
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG
	CycleMove	Emote_Sitting_Pose8_Loop

	Type		male
	  Anim          male\emote_sitting_poses 15 16
	TEnd

	Type		fem
	  Anim          fem\emote_sitting_poses 9 10
	TEnd

	Type		huge
	  Anim          huge\emote_sitting_poses 15 16
	TEnd
MEnd

#############################################################

Move Emote_Sitting_Pose9_Intro
	Requires	EMOTE SIT POSE NINE
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		FIXARMREG
	NextMove	Emote_Sitting_Pose9_Loop

	Type		male
	  Anim          male\emote_sitting_poses 17 18
	TEnd

	Type		fem
	  Anim          fem\emote_sitting_poses 11 12
	TEnd

	Type		huge
	  Anim          huge\emote_sitting_poses 17 18
	TEnd
MEnd

Move Emote_Sitting_Pose9_Loop
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG
	CycleMove	Emote_Sitting_Pose9_Loop

	Type		male
	  Anim          male\emote_sitting_poses 17 18
	TEnd

	Type		fem
	  Anim          fem\emote_sitting_poses 11 12
	TEnd

	Type		huge
	  Anim          huge\emote_sitting_poses 17 18
	TEnd
MEnd

#############################################################

Move Emote_Sitting_Pose10_Intro
	Requires	EMOTE SIT POSE TEN
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		FIXARMREG
	NextMove	Emote_Sitting_Pose10_Loop

	Type		male
	  Anim          male\emote_sitting_poses 19 20
	TEnd

	Type		fem
	  Anim          fem\emote_sitting_poses 9 10
	TEnd

	Type		huge
	  Anim          huge\emote_sitting_poses 19 20
	TEnd
MEnd

Move Emote_Sitting_Pose10_Loop
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG
	CycleMove	Emote_Sitting_Pose10_Loop

	Type		male
	  Anim          male\emote_sitting_poses 19 20
	TEnd

	Type		fem
	  Anim          fem\emote_sitting_poses 9 10
	TEnd

	Type		huge
	  Anim          huge\emote_sitting_poses 19 20
	TEnd
MEnd

#############################################################

Move Emote_Sitting_Pose11_Intro
	Requires	EMOTE SIT POSE TEN ONE
	Priority	5
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		FIXARMREG
	NextMove	Emote_Sitting_Pose11_Loop

	Type		male
	  Anim          male\emote_sitting_poses 21 22
	TEnd

	Type		fem
	  Anim          fem\emote_sitting_poses 13 14
	TEnd

	Type		huge
	  Anim          huge\emote_sitting_poses 21 22
	TEnd
MEnd

Move Emote_Sitting_Pose11_Loop
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG
	CycleMove	Emote_Sitting_Pose11_Loop

	Type		male
	  Anim          male\emote_sitting_poses 21 22
	TEnd

	Type		fem
	  Anim          fem\emote_sitting_poses 13 14
	TEnd

	Type		huge
	  Anim          huge\emote_sitting_poses 21 22
	TEnd
MEnd

#############################################################
## Dojo Kneel
#############################################################

Move Pre_DojoKneel
	Requires	EMOTE KNEEL KUNGFU SIT
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> #<IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	DojoKneel_cycle
	Wind		SIT
	Flags		FIXARMREG

	Type		male
	  Anim          male\dojo_kneel 1 66
	TEnd

	Type		beast
	  Anim          beast\sit_comp 1 4
	TEnd
MEnd

Move DojoKneel_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Wind		SIT
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\dojo_kneel 67 70
	TEnd

	Type		beast
	  Anim          beast\sit_comp 1 4
	TEnd
MEnd

#############################################################
## Sit Ground
#############################################################

Move GroundSit
	Requires	EMOTE SIT
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> #<IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	GroundSit_cycle
	Wind		SIT

	Type		male
	  Anim          male\sit_ground 1 101
	TEnd

	Type		huge
	  Anim          huge\sit_ground 1 101
	TEnd

	Type		beast
	  Anim          beast\sit_comp 1 4
	TEnd
MEnd

Move GroundSit_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE
	Wind		SIT

	Type		male
	  Anim          male\sit_ground 102
	TEnd

	Type		huge
	  Anim          huge\sit_ground 102
	TEnd

	Type		beast
	  Anim          beast\sit_comp 1 4
	TEnd
MEnd


#############################################################
## Lotus\Yoga Emote
#############################################################

Move Lotus_Pre
	Requires	EMOTE SIT MEDITATE LOW
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> #<IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Lotus_Cycle
	Scale		0.7
	Wind		SIT
	Flags		FIXARMREG

	Type		male
	  Anim          male\astro 1 62
	TEnd

	Type		fem
	  Anim          fem\astro 1 62
	TEnd

	Type		huge
	  Anim          huge\astro 1 62
	TEnd

	Type		beast
	  Anim          beast\sit_comp 1 4
	TEnd
MEnd

Move Lotus_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Wind		SIT
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\astro 63 64
	TEnd

	Type		fem
	  Anim          fem\astro 63 64
	TEnd

	Type		huge
	  Anim          huge\astro 63 64
	TEnd

	Type		beast
	  Anim          beast\sit_comp 1 4
	TEnd
MEnd


#############################################################
## Ledge Sit
#############################################################

Move Emote_Ledge_Sit1
	Requires	EMOTE LEDGESIT
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> #<IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Ledge_Sit1_Cycle
	Flags		FIXARMREG PICKRANDOMLY

	Type		male
	  Anim          male\ledgesit_var1 1
	TEnd
MEnd

Move Emote_Ledge_Sit1_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\ledgesit_var1 1
	TEnd
MEnd

#############################################################

Move Emote_Ledge_Sit2
	Requires	EMOTE LEDGESIT
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> #<IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Ledge_Sit2_Cycle
	Flags		FIXARMREG PICKRANDOMLY

	Type		male
	  Anim          male\ledgesit_var2 1
	TEnd
MEnd

Move Emote_Ledge_Sit2_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\ledgesit_var2 1
	TEnd
MEnd

#############################################################

Move Emote_Ledge_Sit3
	Requires	EMOTE LEDGESIT
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> #<IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Ledge_Sit3_Cycle
	Flags		FIXARMREG PICKRANDOMLY

	Type		male
	  Anim          male\ledgesit_var3 1
	TEnd
MEnd

Move Emote_Ledge_Sit3_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\ledgesit_var3 1
	TEnd
MEnd

#############################################################

Move Emote_Ledge_Sit4
	Requires	EMOTE LEDGESIT
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> #<IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Ledge_Sit4_Cycle
	Flags		FIXARMREG PICKRANDOMLY

	Type		male
	  Anim          male\ledgesit_var4 1
	TEnd
MEnd

Move Emote_Ledge_Sit4_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\ledgesit_var4 1
	TEnd
MEnd

#############################################################
## Waiting
#############################################################

Move emote_Waiting_pre
	Requires	EMOTE WAITING
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	emote_Waiting_cycle
	Flags		FIXARMREG

	Type		male
	  Anim          male\emote_waiting_cycle 1
	TEnd

	Type		fem
	  Anim          fem\emote_waiting_cycle 1
	TEnd

	Type		huge
	  Anim          huge\emote_waiting_cycle 1
	TEnd

MEnd

Move emote_Waiting_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		FIXARMREG CYCLE
	CycleMove	emote_Waiting_cycle
	CycleMove	emote_Waiting_cycle
	CycleMove	emote_Waiting_cycle
	CycleMove	emote_Waiting_break1

	Type		male
	  Anim          male\emote_waiting_cycle 1
	TEnd

	Type		fem
	  Anim          fem\emote_waiting_cycle 1
	TEnd

	Type		huge
	  Anim          huge\emote_waiting_cycle 1
	TEnd
MEnd

Move emote_Waiting_break1
	Include sequencers\Members\Emotes_Idle.inc
	Flags		FIXARMREG
	NextMove	emote_Waiting_cycle

	Type		male
	  Anim          male\emote_waiting_break1 1
	TEnd

	Type		fem
	  Anim          fem\emote_waiting_break1 1
	TEnd

	Type		huge
	  Anim          huge\emote_waiting_break1 1
	TEnd
MEnd

#############################################################
## Flipping Coin
#############################################################

Move Emote_FlippingCoin_pre
	Requires	EMOTE FLIPPINGCOIN
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_FlippingCoin_cycle
	Scale		0.85
	Flags		NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\emote_FlippingCoin 1
	  PlayFX	generic\FlippingCoin.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_FlippingCoin 1
	  PlayFX	generic\FlippingCoin.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_FlippingCoin 1
	  PlayFX	generic\FlippingCoin.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_FlippingCoin_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NOSIZESCALE FIXARMREG
	Scale		0.85

	Type		male
	  Anim          male\emote_FlippingCoin 1
	  PlayFX	generic\FlippingCoin.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_FlippingCoin 1
	  PlayFX	generic\FlippingCoin.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_FlippingCoin 1
	  PlayFX	generic\FlippingCoin.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
## Smartphone
#############################################################

Move Emote_Smartphone_Pre
	Requires	EMOTE SMARTPHONE
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Smartphone_Cycle
	Scale		0.5
	Flags		FIXARMREG

	Type		male
	  Anim          male\emote_smartphone 1
	  PlayFx        GENERIC\FX_Smartphone.FX 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_smartphone 1
	  PlayFx        GENERIC\FX_Smartphone.FX 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_smartphone 1
	  PlayFx        GENERIC\FX_Smartphone.FX 0 CONSTANT
	TEnd
MEnd

Move Emote_Smartphone_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Interpolate	8
	Scale		0.5
	Flags		FIXARMREG CYCLE

	Type		male
	  Anim          male\emote_smartphone 1
	  PlayFx        GENERIC\FX_Smartphone.FX 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_smartphone 1
	  PlayFx        GENERIC\FX_Smartphone.FX 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_smartphone 1
	  PlayFx        GENERIC\FX_Smartphone.FX 0 CONSTANT
	TEnd
MEnd

#############################################################
## Smartphone - Texting
#############################################################

Move Emote_Smartphone_Text_Pre
	Requires	EMOTE SMARTPHONE TEXT
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Smartphone_Text_Cycle
	Flags		FIXARMREG nosizescale

	Type		male
	  Anim          male\emote_smartphone_texting 1 39
	  PlayFx        GENERIC\FX_Smartphone_Texting.FX 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_smartphone_texting 1 39
	  PlayFx        GENERIC\FX_Smartphone_Texting_huge.FX 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_smartphone_texting 1 39
	  PlayFx        GENERIC\FX_Smartphone_Texting_Fem.FX 0 CONSTANT
	TEnd
MEnd

Move Emote_Smartphone_Text_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Smartphone_Text_Cycle_Post
	Flags		FIXARMREG CYCLE nosizescale

	Type		male
	  Anim          male\emote_smartphone_texting 39 112
	  PlayFx        GENERIC\FX_Smartphone_Texting.FX 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_smartphone_texting 39 112
	  PlayFx        GENERIC\FX_Smartphone_Texting_huge.FX 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_smartphone_texting 39 112
	  PlayFx        GENERIC\FX_Smartphone_Texting_Fem.FX 0 CONSTANT
	TEnd
MEnd

Move Emote_Smartphone_Text_Cycle_Post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Ready
	Flags		FIXARMREG nosizescale

	Type		male
	  Anim          male\emote_smartphone_texting 112
	  PlayFx        GENERIC\FX_Smartphone.FX 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_smartphone_texting 112
	  PlayFx        GENERIC\FX_Smartphone_huge.FX 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_smartphone_texting 112
	  PlayFx        GENERIC\FX_Smartphone_Fem.FX 0 CONSTANT
	TEnd
MEnd
#############################################################
## Smartphone - Cameraphone
#############################################################

Move Emote_Smartphone_Cameraphone
	Requires	EMOTE SMARTPHONE PICTURE
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Smartphone_Cameraphone_Post
	#Scale		0.5
	Flags		FIXARMREG

	Type		male
	  Anim          male\emote_cameraphone 1 155
	  PlayFx        GENERIC\FX_CameraPhone.FX 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_cameraphone 1 155
	  PlayFx        GENERIC\FX_CameraPhone.FX 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_cameraphone 1 155
	  PlayFx        GENERIC\FX_CameraPhone.FX 0 CONSTANT
	TEnd
MEnd

Move Emote_Smartphone_Cameraphone_Post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Ready
	Interpolate	8
	#Scale		0.5
	Flags		FIXARMREG #CYCLE

	Type		male
	  Anim          male\emote_cameraphone 155
	  #PlayFx        GENERIC\FX_Smartphone.FX 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_cameraphone 155
	 # PlayFx        GENERIC\FX_Smartphone.FX 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_cameraphone 155
	 # PlayFx        GENERIC\FX_Smartphone.FX 0 CONSTANT
	TEnd
MEnd

#############################################################
## Cellphone - old
#############################################################

Move Emote_Cellphone_Pre
	Requires	EMOTE CELLPHONE
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Cellphone_Cycle
	Scale		0.5
	Flags		FIXARMREG

	Type		male
	  Anim          male\emote_cellphone 1
	  PlayFx        GENERIC\FX_cellphone.FX 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_cellphone 1
	  PlayFx        GENERIC\FX_cellphone.FX 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_cellphone 1
	  PlayFx        GENERIC\FX_cellphone.FX 0 CONSTANT
	TEnd
MEnd

Move Emote_Cellphone_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Interpolate	8
	Scale		0.5
	Flags		FIXARMREG CYCLE

	Type		male
	  Anim          male\emote_cellphone 1
	  PlayFx        GENERIC\FX_cellphone.FX 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_cellphone 1
	  PlayFx        GENERIC\FX_cellphone.FX 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_cellphone 1
	  PlayFx        GENERIC\FX_cellphone.FX 0 CONSTANT
	TEnd
MEnd

#############################################################
## Binoculars
#############################################################

Move Emote_Binoculars_Pre
	Requires	EMOTE BINOCULARS
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Binoculars
	Flags		FIXARMREG
	Scale		0.5

	Type		male
	  Anim          male\emote_binoculars 1
	  PlayFx        GENERIC\Binoculars.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_binoculars 1
	  PlayFx        GENERIC\Binoculars.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_binoculars 1
	  PlayFx        GENERIC\Binoculars.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Binoculars
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG
	Scale		0.5

	Type		male
	  Anim          male\emote_binoculars 1
	  PlayFx        GENERIC\Binoculars.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_binoculars 1
	  PlayFx        GENERIC\Binoculars.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_binoculars 1
	  PlayFx        GENERIC\Binoculars.fx 0 CONSTANT
	TEnd
MEnd


#############################################################
## Clipboard
#############################################################

Move Emote_Clipboard_Pre
	Requires	EMOTE CLIPBOARD
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Clipboard
	Scale		0.7
	Flags		FIXARMREG

	Type		male
	  Anim          male\clipboard_write 1
	  PlayFx        GENERIC\FX_clipboard.FX 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\fem_clipboard 1
	  PlayFx        GENERIC\FX_clipboard.FX 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_clipboard 1
	  PlayFx        GENERIC\FX_clipboard.FX 0 CONSTANT
	TEnd
MEnd

Move Emote_Clipboard
	Include sequencers\Members\Emotes_Idle.inc
	Scale		0.7
	Flags		FIXARMREG CYCLE

	Type		male
	  Anim          male\clipboard_write 1
	  PlayFx        GENERIC\FX_clipboard.FX 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\fem_clipboard 1
	  PlayFx        GENERIC\FX_clipboard.FX 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_clipboard 1
	  PlayFx        GENERIC\FX_clipboard.FX 0 CONSTANT
	TEnd
MEnd

#############################################################
## Mission Architect Accolade
#############################################################

Move Emote_MATablet_Pre
	Requires	EMOTE CLIPBOARD MISSIONARCHITECT
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_MATablet_Cycle
	Flags		FixArmReg

	Type		male
	  Anim          male\Bookie_Base 1
	  PlayFx	Emotes\MATablet\Male_MATablet.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\Bookie_Base 1
	  PlayFx	Emotes\MATablet\Fem_MATablet.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\Bookie_Base 1
	  PlayFx	Emotes\MATablet\Huge_MATablet.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_MATablet_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle FixArmReg

	Type		male
	  Anim          male\Bookie_Base 1
	  PlayFx	Emotes\MATablet\Male_MATablet.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\Bookie_Base 1
	  PlayFx	Emotes\MATablet\Fem_MATablet.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\Bookie_Base 1
	  PlayFx	Emotes\MATablet\Huge_MATablet.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
## Camera - Taking Pictures
#############################################################

Move Emote_Camera_Pre
	Requires	EMOTE CAMERA
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Camera
	Flags		FIXARMREG NOSIZESCALE

	Type		male
	  Anim          male\emote_camera 1
	  PlayFx        GENERIC\Camera_Big.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_camera 1
	  PlayFx        GENERIC\Camera_Big.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_camera 1
	  PlayFx        GENERIC\Camera_Big.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Camera
	Include sequencers\Members\Emotes_Idle.inc
	Flags		FIXARMREG CYCLE NOSIZESCALE

	Type		male
	  Anim          male\emote_camera 1
	  PlayFx        GENERIC\Camera_Big.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_camera 1
	  PlayFx        GENERIC\Camera_Big.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_camera 1
	  PlayFx        GENERIC\Camera_Big.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
## SLRCamera - Taking Pictures
#############################################################

Move Emote_SLRCamera_Pre
	Requires	EMOTE SLRCAMERA
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_SLRCamera
	Flags		FIXARMREG NOSIZESCALE

	Type		male
	  Anim          male\emote_slrcamera 1
	  PlayFx        GENERIC\Camera_SLR.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_slrcamera 1
	  PlayFx        GENERIC\Camera_SLR.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_slrcamera 1
	  PlayFx        GENERIC\Camera_SLR.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_SLRCamera
	Include sequencers\Members\Emotes_Idle.inc
	Flags		FIXARMREG CYCLE NOSIZESCALE

	Type		male
	  Anim          male\emote_slrcamera 1
	  PlayFx        GENERIC\Camera_SLR.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_slrcamera 1
	  PlayFx        GENERIC\Camera_SLR.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_slrcamera 1
	  PlayFx        GENERIC\Camera_SLR.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
## Wounded
#############################################################

Move Emote_Wounded_Pre
	Requires	EMOTE WOUNDED
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Wounded
	Scale          0.50

	Type		male
	  Anim          male\dizzy_cycle 1 31
	TEnd
MEnd

Move Emote_Wounded
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE
	Scale          0.50

	Type		male
	  Anim          male\dizzy_cycle 1 31
	TEnd
MEnd


#############################################################
## Fishing
#############################################################

Move Emote_Fishing_Pre
	Requires	EMOTE FISHING
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		FIXARMREG NOSIZESCALE
	NextMove	Emote_Fishing

	Type		male
	  Anim          male\emote_fishing 1
	  Playfx	Generic\FX_FishingRod.fx  5 CONSTANT
	TEnd

	Type		fem
	  Anim          male\emote_fishing 1
	  Playfx	Generic\FX_FishingRod.fx  5 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_fishing 1
	  Playfx	Generic\FX_FishingRod.fx  5 CONSTANT
	TEnd
MEnd

Move Emote_Fishing
	Include sequencers\Members\Emotes_Idle.inc
	Flags		FIXARMREG CYCLE NOSIZESCALE

	Type		male
	  Anim          male\emote_fishing 1
	  Playfx	Generic\FX_FishingRod.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          male\emote_fishing 1
	  Playfx	Generic\FX_FishingRod.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_fishing 1
	  Playfx	Generic\FX_FishingRod.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
## Howl
#############################################################

Move Emote_Howl
	Requires	EMOTE HOWL
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_NoSpam.inc
	NextMove	Emote_Howl_Post
	Flags		NOSIZESCALE
	Scale		0.625

	Type		male
	  Anim          male\scream 1
	  PlayFX	Emotes\PlayerHowl.fx 5
	TEnd

	Type		fem
	  Anim          fem\scream 1
	  PlayFX	Emotes\PlayerHowl.fx 5
	TEnd

	Type		huge
	  Anim          huge\scream 1
	  PlayFX	Emotes\PlayerHowl.fx 5
	TEnd
MEnd

Move Emote_Howl_Post
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Scale		0.50

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
## Stance - Hands on Hips
#############################################################

Move Emote_HandsOnHips
	Requires	EMOTE AKIMBO
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_HandsOnHips_cycle
	Flags		FIXARMREG

	Type		male
	  Anim          male\yes 1 90
	TEnd

	Type		huge
	  Anim          huge\yes 1 90
	TEnd

	Type		fem
	  Anim          fem\yes 1 69
	TEnd
MEnd

Move Emote_HandsOnHips_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\yes 91 95
	TEnd

	Type		huge
	  Anim          huge\yes 91 95
	TEnd

	Type		fem
	  Anim          fem\yes 70 75
	TEnd
MEnd

#############################################################
## Stance - Cross Arms
#############################################################

Move emote_XArms
	Requires	EMOTE CROSSARMS
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	emote_XArms_Cycle
	Scale		0.6
	Interpolate	15
	Flags		FIXARMREG

	Type		male
	  Anim          male\no 1 90
	TEnd

	Type		huge
	  Anim          huge\no 1 90
	TEnd

	Type		fem
	  Anim          fem\no 1 90
	TEnd
MEnd

Move emote_XArms_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\no 91 95
	TEnd

	Type		huge
	  Anim          huge\no 91 95
	TEnd

	Type		fem
	  Anim          fem\no 91 95
	TEnd
MEnd

#############################################################
## Whistle
#############################################################

Move Emote_Whistle
	Requires	EMOTE WHISTLE
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> #<IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Whistle_PostCycle
	Interpolate     7
	Flags		FIXARMREG

	Type		male
	  Anim          male\whistle 1 50
	  PlayFx        FEET\WHISTLE.FX  19
	TEnd
MEnd

Move Emote_Whistle_PostCycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle

	Type		male
	  Anim          male\threat_cycle 1
	TEnd
MEnd


#############################################################
## RaiseHand
#############################################################

Move Emote_RaiseHand
	Requires	EMOTE RAISEHAND
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield> #<IRQEmotesRighthand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_RaiseHand_cycle

	Type		male
	  Anim          male\raise_hand 1 50
	TEnd

	Type		huge
	  Anim          huge\raise_hand 1 50
	TEnd

	Type		fem
	  Anim          fem\raise_hand 1 50
	TEnd
MEnd

Move Emote_RaiseHand_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\raise_hand 50 55
	TEnd

	Type		huge
	  Anim          huge\raise_hand 50 55
	TEnd

	Type		fem
	  Anim          fem\raise_hand 50 55
	TEnd
MEnd


#############################################################
## "Point\Attack!"
#############################################################

Move Emote_Attack
	Requires	EMOTE ATTACK
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Attack2

	Type		male
	  Anim          male\attack 1 57
	TEnd

	Type		huge
	  Anim          huge\attack 1 57
	TEnd

	Type		fem
	  Anim          fem\attack 1 57
	TEnd
MEnd

Move Emote_Attack2
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_HandsOnHips_cycle

	Type		male
	  Anim          male\attack 56
	TEnd

	Type		huge
	  Anim          huge\attack 56
	TEnd

	Type		fem
	  Anim          fem\attack 56
	TEnd
MEnd


#############################################################
## "Don't Attack"
#############################################################

Move Emote_DontAttack
	Requires	EMOTE NO
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_DontAttack2

	Type		male
	  Anim          male\NoAttack 1 62
	TEnd

	Type		huge
	  Anim          huge\NoAttack 1 62
	TEnd

	Type		fem
	  Anim          fem\NoAttack 1 62
	TEnd
MEnd

Move Emote_DontAttack2
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	emote_XArms_Cycle

	Type		male
	  Anim          male\NoAttack 63
	TEnd

	Type		huge
	  Anim          huge\NoAttack 63
	TEnd

	Type		fem
	  Anim          fem\NoAttack 63
	TEnd
MEnd


#############################################################
## "Over Here"
#############################################################

Move Emote_OverHere
	Requires	EMOTE BIGWAVE
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_OverHere2

	Type		male
	  Anim          male\over_here 1 88
	TEnd

	Type		huge
	  Anim          huge\over_here 1 88
	TEnd

	Type		fem
	  Anim          fem\over_here 1 88
	TEnd
MEnd

Move Emote_OverHere2
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_HandsOnHips_cycle

	Type		male
	  Anim          male\over_here 89
	TEnd

	Type		huge
	  Anim          huge\over_here 89
	TEnd

	Type		fem
	  Anim          fem\over_here 89
	TEnd
MEnd


#############################################################
## "Hi"
#############################################################

Move Emote_Hi
	Requires	EMOTE WAVE
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Hi2

	Type		male
	  Anim          male\hi 1 45
	TEnd

	Type		huge
	  Anim          huge\hi 1 45
	TEnd

	Type		fem
	  Anim          fem\hi 1 45
	TEnd
MEnd

Move Emote_Hi2
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_HandsOnHips_cycle

	Type		male
	  Anim          male\hi 46
	TEnd

	Type		huge
	  Anim          huge\hi 46
	TEnd

	Type		fem
	  Anim          fem\hi 46
	TEnd
MEnd


#############################################################
## ThumbsUp
#############################################################

Move Emote_ThumbsUp
	Requires	EMOTE YES
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_ThumbsUp2

	Type		male
	  Anim          male\thumbsup 1 60
	TEnd

	Type		huge
	  Anim          huge\thumbsup 1 60
	TEnd

	Type		fem
	  Anim          fem\thumbsup 1 60
	TEnd
MEnd

Move Emote_ThumbsUp2
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_HandsOnHips_cycle

	Type		male
	  Anim          male\thumbsup 61
	TEnd

	Type		huge
	  Anim          huge\thumbsup 61
	TEnd

	Type		fem
	  Anim          fem\thumbsup 61
	TEnd
MEnd


#############################################################
## Rock\Paper\Scissors - "Rock"
#############################################################

Move RPS_Rock_pre
	Requires	EMOTE ROCK
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	RPS_Rock_cast
	Flags		FIXARMREG

	Type		male
	  Anim          male\RPS_pre 15 150
	  PlayFx        GENERIC\RPS_pre.FX 0 CONSTANT
	TEnd
MEnd

Move RPS_Rock_cast
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	RPS_Rock
	Flags		FIXARMREG

	Type		male
	  Anim          male\RPS_rock 28 73
	  PlayFx        GENERIC\RPS_pre.FX 0 CONSTANT
	TEnd
MEnd

Move RPS_Rock
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	RPS_Rock_cycle
	Flags		FIXARMREG

	Type		male
	  Anim          male\RPS_rock 74 104
	  PlayFx        GENERIC\RPS_rock.FX 0 CONSTANT
	TEnd
MEnd

Move RPS_Rock_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\RPS_rock 105
	  PlayFx        GENERIC\RPS_rock.FX 0 CONSTANT
	TEnd
MEnd


#############################################################
## Rock\Paper\Scissors - "Paper"
#############################################################

Move RPS_paper_pre
	Requires	EMOTE PAPER
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	RPS_paper_cast
	Flags		FIXARMREG

	Type		male
	  Anim          male\RPS_pre 15 150
	  PlayFx        GENERIC\RPS_pre.FX 0 CONSTANT
	TEnd
MEnd


Move RPS_paper_cast
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	RPS_paper
	Flags		FIXARMREG

	Type		male
	  Anim          male\RPS_paper 28 73
	  PlayFx        GENERIC\RPS_pre.FX 0 CONSTANT
	TEnd
MEnd

Move RPS_paper
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	RPS_paper_cycle
	Flags		FIXARMREG

	Type		male
	  Anim          male\RPS_paper 74 104
	  PlayFx        GENERIC\RPS_paper.FX 0 CONSTANT
	TEnd
MEnd

Move RPS_paper_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\RPS_paper 105
	  PlayFx        GENERIC\RPS_paper.FX 0 CONSTANT
	TEnd
MEnd


#############################################################
## Rock\Paper\Scissors - "Scissors"
#############################################################

Move RPS_scissor_pre
	Requires	EMOTE SCISSORS
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	RPS_scissor_cast
	Flags		FIXARMREG

	Type		male
	  Anim          male\RPS_pre 15 150
	  PlayFx        GENERIC\RPS_pre.FX 0 CONSTANT
	TEnd
MEnd

Move RPS_scissor_cast
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	RPS_scissor
	Flags		FIXARMREG

	Type		male
	  Anim          male\RPS_scissor 28 73
	  PlayFx        GENERIC\RPS_pre.FX 0 CONSTANT
	TEnd
MEnd

Move RPS_scissor
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	RPS_scissor_cycle
	Flags		FIXARMREG

	Type		male
	  Anim          male\RPS_scissor 74 104
	  PlayFx        GENERIC\RPS_scissor.FX 0 CONSTANT
	TEnd
MEnd

Move RPS_scissor_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\RPS_scissor 105
	  PlayFx        GENERIC\RPS_scissor.FX 0 CONSTANT
	TEnd
MEnd

#############################################################
## Roll Dice #1
#############################################################

Move 1_RollDice_pre
	Requires	EMOTE ROLLDICE HIGH
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield> #<IRQEmotesRighthand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	1_RollDice_cast
	Scale		1.5
	Interpolate	12
	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\roll_dice 125 201  ##209
	TEnd
MEnd

Move 1_RollDice_cast
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	postReady_back
	Interpolate	8

	Type		male
	  Anim          male\roll_dice 1 120
	  PlayFx        GENERIC\Dice_1.FX  80 CONSTANT
	  PlayFx        GENERIC\Prop_Dice.FX  69
	TEnd
MEnd

#############################################################
##  Roll Dice #2
#############################################################

Move 2_RollDice_pre
	Requires	EMOTE ROLLDICE HIGH
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield> #<IRQEmotesRighthand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	2_RollDice_cast
	Scale		1.5
	Interpolate	12
	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\roll_dice 125 201  ##209
	TEnd
MEnd

Move 2_RollDice_cast
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	postReady_back
	Interpolate	8

	Type		male
	  Anim          male\roll_dice 1 120
	  PlayFx        GENERIC\Dice_2.FX  80 CONSTANT
	  PlayFx        GENERIC\Prop_Dice.FX  69
	TEnd
MEnd

#############################################################
## Roll Dice #3
#############################################################

Move 3_RollDice_pre
	Requires	EMOTE ROLLDICE HIGH
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield> #<IRQEmotesRighthand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	3_RollDice_cast
	Scale		1.5
	Interpolate	12
	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\roll_dice 125 201  ##209
	TEnd
MEnd

Move 3_RollDice_cast
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	postReady_back
	Interpolate	8

	Type		male
	  Anim          male\roll_dice 1 120
	  PlayFx        GENERIC\Dice_3.FX  80 CONSTANT
	  PlayFx        GENERIC\Prop_Dice.FX  69
	TEnd
MEnd

#############################################################
## Roll Dice #4
#############################################################

Move 4_RollDice_pre
	Requires	EMOTE ROLLDICE HIGH
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield> #<IRQEmotesRighthand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	4_RollDice_cast
	Scale		1.5
	Interpolate	12
	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\roll_dice 125 201  ##209
	TEnd
MEnd

Move 4_RollDice_cast
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	postReady_back
	Interpolate	8

	Type		male
	  Anim          male\roll_dice 1 120
	  PlayFx        GENERIC\Dice_4.FX  80 CONSTANT
	  PlayFx        GENERIC\Prop_Dice.FX  69
	TEnd
MEnd

#############################################################
##  Roll Dice #5
#############################################################

Move 5_RollDice_pre
	Requires	EMOTE ROLLDICE HIGH
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield> #<IRQEmotesRighthand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	5_RollDice_cast
	Scale		1.5
	Interpolate	12
	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\roll_dice 125 201  ##209
	TEnd
MEnd

Move 5_RollDice_cast
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	postReady_back
	Interpolate	8

	Type		male
	  Anim          male\roll_dice 1 120
	  PlayFx        GENERIC\Dice_5.FX  80 CONSTANT
	  PlayFx        GENERIC\Prop_Dice.FX  69
	TEnd
MEnd

#############################################################
## Roll Dice #6
#############################################################

Move 6_RollDice_pre
	Requires	EMOTE ROLLDICE HIGH
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield> #<IRQEmotesRighthand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	6_RollDice_cast
	Scale		1.5
	Interpolate	12
	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\roll_dice 125 201  ##209
	TEnd
MEnd

Move 6_RollDice_cast
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	postReady_back
	Interpolate	8

	Type		male
	  Anim          male\roll_dice 1 120
	  PlayFx        GENERIC\Dice_6.FX  80 CONSTANT
	  PlayFx        GENERIC\Prop_Dice.FX 69
	TEnd
MEnd

#############################################################
## Roll Dice #7
#############################################################

Move 7_RollDice_pre
	Requires	EMOTE DICE HIGH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield> #<IRQEmotesRighthand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	7_RollDice_cast
	Scale		1.5
	Interpolate	12

	Type		male
	  Anim          male\roll_dice 125 201  ##209
	TEnd
MEnd

Move 7_RollDice_cast
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	postReady_back
	Interpolate	8

	Type		male
	  Anim          male\roll_dice 1 120
	  PlayFx        GENERIC\Dice_7.FX  80 CONSTANT
	  PlayFx        GENERIC\Prop_Dice.FX 69
	TEnd
MEnd

#############################################################
## Flip Coin (heads A)
#############################################################

Move Flipcoin_heads
	Requires	EMOTE ROLLDICE CAST LOW
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Flipcoin_heads_cycle
	Flags		PICKRANDOMLY FIXARMREG


	Type		male
	  Anim          male\flipcoin 1 140
	  PlayFx        GENERIC\Coin_A.FX  113 CONSTANT
	  PlayFx        GENERIC\Prop_Coin.FX  56
	TEnd
MEnd

Move Flipcoin_heads_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\flipcoin 144
	  PlayFx        GENERIC\Coin_A.FX 0 CONSTANT
	TEnd
MEnd


#############################################################
## Flip Coin (tails B)
#############################################################

Move Flipcoin_tail
	Requires	EMOTE ROLLDICE CAST LOW
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Flipcoin_tail_cycle
	Flags		PICKRANDOMLY FIXARMREG

	Type		male
	  Anim          male\flipcoin 1 140
	  PlayFx        GENERIC\Coin_B.FX  113 CONSTANT
	  PlayFx        GENERIC\Prop_Coin.FX  56
	TEnd
MEnd

Move Flipcoin_tail_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\flipcoin 144
	  PlayFx        GENERIC\Coin_B.FX 0 CONSTANT
	TEnd
MEnd


#############################################################
## Flex A
#############################################################

Move Flex_A
	Requires	EMOTE FLEX HIGH
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Flex_A_cycle

	Type		male
	  Anim          male\flex_A 5 54
	TEnd
MEnd

Move Flex_A_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NOSIZESCALE

	Type		male
	  Anim          male\flex_A 55
	TEnd
MEnd


#############################################################
## Flex B
#############################################################

Move Flex_B
	Requires	EMOTE FLEX ONE
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesShield> #<IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Flex_B_cycle

	Type		male
	  Anim          male\flex_B 6 54
	TEnd
MEnd

Move Flex_B_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NOSIZESCALE

	Type		male
	  Anim          male\flex_B 55
	TEnd
MEnd


#############################################################
## Flex C
#############################################################

Move Flex_C
	Requires	EMOTE FLEX LOW
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Flex_C_cycle


	Type		male
	  Anim          male\flex_C 11 119
	TEnd
MEnd

Move Flex_C_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NOSIZESCALE

	Type		male
	  Anim          male\flex_C 120
	TEnd
MEnd


#############################################################
## Heroic Laugh
#############################################################

Move Emote_Laugh
	Requires	EMOTE LAUGH LOW
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Laugh_cycle
	Flags		FIXARMREG

	Type		male
	  Anim          male\heroic_laugh 1
	TEnd
MEnd

Move Emote_Laugh_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NOSIZESCALE

	Type		male
	  Anim          male\thumbsup 99 100
	TEnd
MEnd


#############################################################
## Somewhat Mad
#############################################################

Move Emote_Mad
	Requires	EMOTE ANGRY POSE LOW
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Mad_cycle
	Interpolate	8

	Type		male
	  Anim          male\mad 1 55
	TEnd
MEnd

Move Emote_Mad_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\mad 55 60
	TEnd
MEnd

#############################################################
## "What?"
#############################################################

Move Emote_What
	Requires	EMOTE QUESTION LOW
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_What_cycle
	Interpolate	8

	Type		male
	  Anim          male\what 1
	TEnd
MEnd

Move Emote_What_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE
	Interpolate     8

	Type		male
	  Anim          male\centerstance_cycle 1
	TEnd
MEnd

#############################################################
## Point
#############################################################

Move Emote_Point
	Requires	EMOTE POINT
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Point_cycle
	Interpolate	8

	Type		male
	  Anim          male\point 1
	TEnd
MEnd

Move Emote_Point_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\thumbsup 99 100
	TEnd
MEnd

#############################################################
## Salute
#############################################################

Move Emote_Salute
	Requires	EMOTE SALUTE ONE
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Salute_cycle
	Interpolate	8
	Flags		FIXARMREG

	Type		male
	  Anim          male\salute 1
	TEnd
MEnd

Move Emote_Salute_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\thumbsup 99 100
	TEnd
MEnd

#############################################################
## Argue - Lecture
#############################################################

Move Argue
	Requires	EMOTE ARGUE HIGH
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Argue_cycle
	Interpolate	8

	Type		male
	  Anim          male\recruit_nobox 129 800
	TEnd
MEnd

Move Argue_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\thumbsup 99 100
	TEnd
MEnd


#############################################################
## Argue - Disagree
#############################################################

Move Argue_Disagree
	Requires	EMOTE ARGUE MID
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Argue_Disagree_cycle
	Interpolate	8

	Type		male
	  Anim          male\deal_A 1 238
	TEnd
MEnd

Move Argue_Disagree_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\no 110 115
	TEnd
MEnd


#############################################################
## Argue - Counterpoint
#############################################################

Move Argue_Reason
	Requires	EMOTE ARGUE LOW
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Argue_Reason2
	Interpolate	8

	Type		male
	  Anim          male\deal_B 92 238
	TEnd
MEnd

Move Argue_Reason2
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Argue_Reason_cycle
	Interpolate	8

	Type		male
	  Anim          male\deal_B 1 83
	TEnd
MEnd

Move Argue_Reason_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\thumbsup 99 100
	TEnd
MEnd


#############################################################
## KungfuTrain
#############################################################

Move Emote_KungfuTrain
	Requires	EMOTE KUNGFU TRAIN STRONG
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_KungfuTrain_cycle

	Type		male
	  Anim          male\train_kungfu_A 1
	  PlayFx        FEET\SUPERJUMP1.FX 17
	  PlayFx        FEET\SUPERJUMP1.FX 74
	  PlayFx        FEET\SUPERJUMP1.FX 127
	  PlayFx        FEET\SUPERJUMP1.FX 194
	TEnd

	Type		fem
	  Anim          male\train_kungfu_A 1
	  PlayFx        FEET\FEMJUMP1.FX 17
	  PlayFx        FEET\FEMJUMP1.FX 74
	  PlayFx        FEET\FEMJUMP1.FX 127
	  PlayFx        FEET\FEMJUMP1.FX 194
	TEnd

	Type		huge
	  Anim          male\train_kungfu_A 1
	  PlayFx        FEET\HUGEJUMP2.FX 17
	  PlayFx        FEET\HUGEJUMP2.FX 74
	  PlayFx        FEET\HUGEJUMP2.FX 127
	  PlayFx        FEET\HUGEJUMP2.FX 194
	TEnd
MEnd

Move Emote_KungfuTrain_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\train_kungfu_A 1
	  PlayFx        FEET\SUPERJUMP1.FX 17
	  PlayFx        FEET\SUPERJUMP1.FX 74
	  PlayFx        FEET\SUPERJUMP1.FX 127
	  PlayFx        FEET\SUPERJUMP1.FX 194
	TEnd

	Type		fem
	  Anim          male\train_kungfu_A 1
	  PlayFx        FEET\FEMJUMP1.FX 17
	  PlayFx        FEET\FEMJUMP1.FX 74
	  PlayFx        FEET\FEMJUMP1.FX 127
	  PlayFx        FEET\FEMJUMP1.FX 194
	TEnd

	Type		huge
	  Anim          male\train_kungfu_A 1
	  PlayFx        FEET\HUGEJUMP2.FX 17
	  PlayFx        FEET\HUGEJUMP2.FX 74
	  PlayFx        FEET\HUGEJUMP2.FX 127
	  PlayFx        FEET\HUGEJUMP2.FX 194
	TEnd
MEnd


#############################################################
## Jumping Jacks
#############################################################

Move Emote_JumpingJacks
	Requires	EMOTE TRAIN LOW STRONG
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_JumpingJacks_cycle

	Type		male
	  Anim          male\jumpingjax 1
	  PlayFx        FEET\SUPERJUMP1.FX 15
	TEnd

	Type		huge
	  Anim          male\jumpingjax 1
	  PlayFx        FEET\HUGEJUMP2.FX 15
	TEnd

	Type		fem
	  Anim          male\jumpingjax 1
	  PlayFx        FEET\FEMJUMP1.FX 15
	TEnd
MEnd

Move Emote_JumpingJacks_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\jumpingjax 1
	  PlayFx        FEET\SUPERJUMP1.FX 15
	TEnd

	Type		huge
	  Anim          male\jumpingjax 1
	  PlayFx        FEET\HUGEJUMP2.FX 15
	TEnd

	Type		fem
	  Anim          male\jumpingjax 1
	  PlayFx        FEET\FEMJUMP1.FX 15
	TEnd
MEnd


#############################################################
## Beatchest
#############################################################

Move Emote_Beatchest
	Requires	EMOTE COMBAT TAUNTA ANGRY
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Beatchest_cycle
	Flags		NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\comb_beatchest 1 91
	  PlayFx        FEET\MALETAUNTA.FX
	TEnd

	Type		huge
	  Anim          huge\comb_beatchest 1 91
	  PlayFx        FEET\HUGETAUNTA.FX
	TEnd

	Type		fem
	  Anim          fem\comb_beatchest 1 91
	  PlayFx        FEET\FEMALETAUNTA.FX
	TEnd
MEnd

Move Beatchest_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NOSIZESCALE FIXARMREG

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
## "Come On!"
#############################################################

Move Emote_ComeOn
	Requires	EMOTE COMBAT TAUNTB ANGRY
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_ComeOn2
	Interpolate     8
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\combat_comeon 9 79
	  PlayFX	feet\MaleTaunt12.fx
	TEnd

	Type		huge
	  Anim          huge\combat_comeon 9 79
	  PlayFX	feet\mALETaunt6.fx
	TEnd

	Type		fem
	  Anim          fem\combat_comeon 9 79
	  PlayFX	feet\fEMTaunt9.fx
	TEnd
MEnd

Move Emote_ComeOn2
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_ComeOn_cycle
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\combat_comeon 80 118
	  PlayFX	feet\handpunch1.fx
	TEnd

	Type		huge
	  Anim          huge\combat_comeon 80 118
	  PlayFX	feet\handpunch1.fx
	TEnd

	Type		fem
	  Anim          fem\combat_comeon 80 118
	  PlayFX	feet\handpunch1.fx
	TEnd
MEnd

Move Emote_ComeOn_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NOSIZESCALE
	NextMove	postReady_back

	Type		male
	  Anim          male\combat_comeon 119 156
	  PlayFX	feet\handpunch1.fx
	TEnd

	Type		huge
	  Anim          huge\combat_comeon 119 156
	  PlayFX	feet\handpunch1.fx
	TEnd

	Type		fem
	  Anim          fem\combat_comeon 119 156
	  PlayFX	feet\handpunch1.fx
	TEnd
MEnd


#############################################################
## "Come Here!"
#############################################################

Move Emote_ComeHere
	Requires	EMOTE COMBAT TAUNTC ANGRY
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Fake_CombatCycle_Half

	Type		male
	  Anim          male\combat_comehere 5
  	  PlayFX	feet\MaleTaunt13.fx
	TEnd

	Type		huge
	  Anim          huge\combat_comehere 5
	  PlayFX	feet\MaleTaunt8.fx
	TEnd

	Type		fem
	  Anim          fem\combat_comehere 5
	  PlayFX	feet\FemTaunt10.fx
	TEnd
MEnd

#############################################################
## Angery Growl (ROAR)
#############################################################

Move Emote_Growl
	Requires	EMOTE ANGRY HIGH FIERCE
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Growl_PostCycle
	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\Growl 1
	PlayFX		Generic\Roar.fx
	TEnd
MEnd

#############################################################

Move Emote_Growl_2
	Requires	EMOTE ANGRY HIGH FIERCE
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Growl_PostCycle
	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\growl_pawR 1
	PlayFX		Generic\Roar.fx
	TEnd
MEnd

#############################################################

Move Emote_Growl_3
	Requires	EMOTE ANGRY HIGH FIERCE
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Growl_PostCycle
	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\growl_pawL
	PlayFX		Generic\Roar.fx
	TEnd
MEnd

Move Emote_Growl_PostCycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\growl_cycle 1
	TEnd
MEnd


#############################################################
## Burp
#############################################################

Move Emote_Burp
	Requires	EMOTE RUDE LOW
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Growl_PostCycle

	Flags		PICKRANDOMLY NOSIZESCALE

	Type		male
	  Anim          male\Growl_burp 1
	PlayFX		Generic\Burp.fx
	TEnd
MEnd

#############################################################
## AFK (newspaper)
#############################################################

Move AFK_newspaper
	Requires	EMOTE AFK
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	AFK_newspaper_cycle
	Flags		FIXARMREG NOSIZESCALE

	Type		male
	  Anim          male\AFK_newspaper 1 156
	  PlayFx        GENERIC\FX_newspaper_male.FX 0 CONSTANT
	TEnd

	Type		v_male
	  Anim          male\newspaper_cycle 1 156
	  PlayFx        GENERIC\FX_newspaper_rogue.FX 0 CONSTANT
	TEnd

	Type		P_male
	  Anim          male\AFK_newspaper 1 156
	  PlayFx        GENERIC\FX_Newspaper_Praetorian_Male.FX 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\AFK_newspaper 1 156
	  PlayFx        GENERIC\FX_newspaper_huge.FX 0 CONSTANT
	TEnd

	Type		V_huge
	  Anim          huge\AFK_newspaper 1 156
	  PlayFx        GENERIC\FX_newspaper_huge_rogue.FX 0 CONSTANT
	TEnd

	Type		P_huge
	  Anim          huge\AFK_newspaper 1 156
	  PlayFx        GENERIC\FX_Newspaper_Praetorian_Huge.FX 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\fem_newspaper 1 156
	  PlayFx        GENERIC\FX_newspaper_female.FX 0 CONSTANT
	TEnd

	Type		V_fem
	  Anim          fem\fem_newspaper 1 156
	  PlayFx        GENERIC\FX_newspaper_female_rogue.FX 0 CONSTANT
	TEnd

	Type		P_fem
	  Anim          fem\fem_newspaper 1 156
	  PlayFx        GENERIC\FX_Newspaper_Praetorian_Female.FX 0 CONSTANT
	TEnd

	Type		male_pushable
	  Anim          male\newspaper_cycle 1 156
	  PlayFx        GENERIC\FX_newspaper_rogue.FX 0 CONSTANT
	TEnd

	Type		ninja
	  Anim          male\newspaper_cycle 1 156
	  PlayFx        GENERIC\FX_newspaper_rogue.FX 0 CONSTANT
	TEnd

	Type		mastermind_skeleton
	  Anim          male\newspaper_cycle 1 156
	  PlayFx        GENERIC\FX_newspaper_rogue.FX 0 CONSTANT
	TEnd

	Type		GhostSoul
	  Anim          male\newspaper_cycle 1 156
	  PlayFx        GENERIC\FX_newspaper_rogue.FX 0 CONSTANT
	TEnd

	Type		5cw
	  Anim          5cw\skel_monster_cycle 1 63
	TEnd

	Type		soldier
	  Anim          male\newspaper_cycle 1 156
	  PlayFx        GENERIC\FX_newspaper_rogue.FX 0 CONSTANT
	TEnd

	Type		Parachute
	  Anim          male\newspaper_cycle 1 156
	  PlayFx        GENERIC\FX_newspaper_rogue.FX 0 CONSTANT
	TEnd

	Type		Rappel
	  Anim          male\newspaper_cycle 1 156
	  PlayFx        GENERIC\FX_newspaper_rogue.FX 0 CONSTANT
	TEnd
MEnd

Move AFK_newspaper_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG NOSIZESCALE
	Interpolate	5

	Type		male
	  Anim          male\AFK_newspaper 1 156
	  PlayFx        GENERIC\FX_newspaper_male.FX 0 CONSTANT
	TEnd

	Type		v_male
	  Anim          male\newspaper_cycle 1 156
	  PlayFx        GENERIC\FX_newspaper_rogue.FX 0 CONSTANT
	TEnd

	Type		p_male
	  Anim          male\AFK_newspaper 1 156
	  PlayFx        GENERIC\FX_Newspaper_Praetorian_Male.FX 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\AFK_newspaper 1 156
	  PlayFx        GENERIC\FX_newspaper_huge.FX 0 CONSTANT
	TEnd

	Type		V_huge
	  Anim          huge\AFK_newspaper 1 156
	  PlayFx        GENERIC\FX_newspaper_huge_rogue.FX 0 CONSTANT
	TEnd

	Type		p_huge
	  Anim          huge\AFK_newspaper 1 156
	  PlayFx        GENERIC\FX_Newspaper_Praetorian_Huge.FX 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\fem_newspaper 1 156
	  PlayFx        GENERIC\FX_newspaper_female.FX 0 CONSTANT
	TEnd

	Type		V_fem
	  Anim          fem\fem_newspaper 1 156
	  PlayFx        GENERIC\FX_newspaper_female_rogue.FX 0 CONSTANT
	TEnd

	Type		p_fem
	  Anim          fem\fem_newspaper 1 156
	  PlayFx        GENERIC\FX_Newspaper_Praetorian_Female.FX 0 CONSTANT
	TEnd

	Type		male_pushable
	  Anim          male\newspaper_cycle 1 156
	  PlayFx        GENERIC\FX_newspaper_rogue.FX 0 CONSTANT
	TEnd

	Type		ninja
	  Anim          male\newspaper_cycle 1 156
	  PlayFx        GENERIC\FX_newspaper_rogue.FX 0 CONSTANT
	TEnd

	Type		mastermind_skeleton
	  Anim          male\newspaper_cycle 1 156
	  PlayFx        GENERIC\FX_newspaper_rogue.FX 0 CONSTANT
	TEnd

	Type		GhostSoul
	  Anim          male\newspaper_cycle 1 156
	  PlayFx        GENERIC\FX_newspaper_rogue.FX 0 CONSTANT
	TEnd

	Type		5cw
	  Anim          5cw\skel_monster_cycle 1 63
	TEnd

	Type		soldier
	  Anim          male\newspaper_cycle 1 156
	  PlayFx        GENERIC\FX_newspaper_rogue.FX 0 CONSTANT
	TEnd

	Type		Parachute
	  Anim          male\newspaper_cycle 1 156
	  PlayFx        GENERIC\FX_newspaper_rogue.FX 0 CONSTANT
	TEnd

	Type		Rappel
	  Anim          male\newspaper_cycle 1 156
	  PlayFx        GENERIC\FX_newspaper_rogue.FX 0 CONSTANT
	TEnd


MEnd


#############################################################
##  Praise_Bow
#############################################################

Move Emote_Praise_Bow
	Requires	EMOTE BOWING LOW
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Praise_Bow_cycle
	Interpolate	8

	Type		male
	  Anim          male\kneeling_bow 1 63
	TEnd
MEnd

Move Emote_Praise_Bow_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\kneeling_bow 65 136
	TEnd
MEnd


#############################################################
## Cheer (chant)
#############################################################

Move Cheerchant_A
	Requires	EMOTE CHEER HIGH
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Cheerchant_A_cycle
	Interpolate	8
	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\chant_high 1
	TEnd
MEnd

Move Cheerchant_A_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\chant_high 1
	TEnd
MEnd

#############################################################

Move Cheerchant_B
	Requires	EMOTE CHEER HIGH
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Cheerchant_B_cycle
	Interpolate	8
	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\chant_1arm 1
	TEnd
MEnd

Move Cheerchant_B_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\chant_1arm 1
	TEnd
MEnd

#############################################################

Move Cheerchant_C
	Requires	EMOTE CHEER HIGH
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Cheerchant_C_cycle
	Interpolate	8
	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\chant_low 1
	TEnd
MEnd

Move Cheerchant_C_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\chant_low 1
	TEnd
MEnd


#############################################################
## Rooting  (woof woof)
#############################################################

Move Rooting
	Requires	EMOTE CHEER MID
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Rooting_2
	Interpolate	8
	Flags		NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\listen_1 1 260
	  PlayFX	feet\handclap.fx 127
	  PlayFX	feet\handclap.fx 134
	  PlayFX	feet\handclap.fx 142
	  PlayFX	feet\handclap.fx 150
	  PlayFX	feet\handclap.fx 158
	  PlayFX	feet\handclap.fx 166
	  PlayFX	feet\handclap.fx 174
	  PlayFX	feet\handclap.fx 182
	  PlayFX	feet\handclap.fx 190
	TEnd

	Type		huge
	  Anim          huge\listen_1 1 190
	  PlayFX	feet\handclap.fx 127
	  PlayFX	feet\handclap.fx 134
	  PlayFX	feet\handclap.fx 142
	  PlayFX	feet\handclap.fx 150
	  PlayFX	feet\handclap.fx 158
	  PlayFX	feet\handclap.fx 166
	  PlayFX	feet\handclap.fx 174
	  PlayFX	feet\handclap.fx 182
	  PlayFX	feet\handclap.fx 190
	TEnd
MEnd

Move Rooting_2
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Rooting_cycle
	Interpolate	8

	Type		male
	  Anim          male\no 110 115
	TEnd

	Type		huge
	  Anim          huge\yes 11 90
	  Scale		0.5
	TEnd
MEnd

Move Rooting_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\no 110 115
	TEnd

	Type		huge
	  Anim          huge\yes 91 95
	TEnd
MEnd


#############################################################
## Clapping
#############################################################

Move Clapping
	Requires	EMOTE CHEER LOW
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Clapping_2

	Flags		NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\listen_1 111 260
	  PlayFX	feet\handclap.fx 16
	  PlayFX	feet\handclap.fx 23
	  PlayFX	feet\handclap.fx 30
	  PlayFX	feet\handclap.fx 37
	  PlayFX	feet\handclap.fx 44
	  PlayFX	feet\handclap.fx 51
	  PlayFX	feet\handclap.fx 58
	  PlayFX	feet\handclap.fx 65
	  PlayFX	feet\handclap.fx 72
	TEnd

	Type		huge
	  Anim          huge\listen_1 111 190
	  PlayFX	feet\handclap.fx 16
	  PlayFX	feet\handclap.fx 23
	  PlayFX	feet\handclap.fx 30
	  PlayFX	feet\handclap.fx 37
	  PlayFX	feet\handclap.fx 44
	  PlayFX	feet\handclap.fx 51
	  PlayFX	feet\handclap.fx 58
	  PlayFX	feet\handclap.fx 65
	  PlayFX	feet\handclap.fx 72
	TEnd
MEnd

Move Clapping_2
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Clapping_cycle
	Interpolate	8

	Type		male
	  Anim          male\no 110 115
	TEnd

	Type		huge
	  Anim          huge\yes 11 90
	  Scale		0.5
	TEnd
MEnd

Move Clapping_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\no 110 115
	TEnd

	Type		huge
	  Anim          huge\yes 91 95
	TEnd
MEnd


#############################################################
## Reverant Bow
#############################################################

Move Bow_Reverant
	Requires	EMOTE BOW GREETING LOW
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Bow_Reverant_post
	Priority	6
	Flags		FIXWEAPONREG #FIXARMREG

	Type		male
	  Anim          male\reverant_bow2 1 100
	TEnd

	Type		huge
	  Anim          huge\reverant_bow2 1 100
	TEnd
MEnd

Move Bow_Reverant_post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Idle1_Cycle
	Interpolate	15
	Flags		FIXWEAPONREG  #FIXARMREG

	Type		male
	  Anim          male\postback_right 28
	TEnd

	Type		huge
	  Anim          huge\postback_right 28
	TEnd

	Type		fem
	  Anim		fem\post_ready 28
	TEnd
MEnd


#############################################################
##  Military Salute
#############################################################

Move MilitarySalute
	Requires	EMOTE SALUTE TWO
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	MilitarySalute_cycle
	Flags		FIXARMREG

	Type		male
	  Anim          male\formal_salute 1 40
	TEnd

	Type		fem
	  Anim          fem\formal_salute 1 40
	TEnd

	Type		huge
	  Anim          huge\formal_salute 1 40
	TEnd
MEnd

Move MilitarySalute_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\formal_salute 41 45
	TEnd

	Type		fem
	  Anim          fem\formal_salute 41 45
	TEnd

	Type		huge
	  Anim          huge\formal_salute 41 45
	TEnd
MEnd

#############################################################

Move AtEase_FromMilitarySalute
	Requires	EMOTE SALUTE HIGH LOW
	Priority	5
	Interrupts	MilitarySalute MilitarySalute_cycle
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	AtEase_cycle
	Flags		FIXARMREG

	Type		male
	  Anim          male\at_ease 1 60
	TEnd

	Type		huge
	  Anim          huge\at_ease 1 60
	TEnd
MEnd

#############################################################
## Military "At Ease"
#############################################################

Move AtEase
	Requires	EMOTE SALUTE HIGH LOW
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	AtEase_cycle
	Flags		FIXARMREG

	Type		male
	  Anim          male\at_ease 22 60
	TEnd

	Type		huge
	  Anim          huge\at_ease 22 60
	TEnd
MEnd

Move AtEase_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\at_ease 61 65
	TEnd

	Type		huge
	  Anim          huge\at_ease 61 65
	TEnd
MEnd


#############################################################
## Nod - CYCLE
#############################################################

Move Emote_Nod
	Requires	EMOTE YES LOW
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Nod_cycle

	Type		male
	  Anim          male\nod 1 70
	TEnd
MEnd

Move Emote_Nod_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\nod 71 75
	TEnd
MEnd

#############################################################
## Fancy_Bow
#############################################################

Move Emote_Fancy_Bow
	Requires	EMOTE BOW GREETING HIGH
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Fancy_Bow_cycle
	Interpolate	8

	Type		male
	  Anim          male\fancy_bow 1
	TEnd
MEnd

Move Emote_Fancy_Bow_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\centerstance_cycle 1
	TEnd
MEnd


#############################################################
## Victory
#############################################################

Move Emote_Victory
	Requires	EMOTE CHEER FIERCE HIGH
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Victory_cycle
	Interpolate	5

	Type		male
	  Anim          male\victory 1
	TEnd
MEnd

Move Emote_Victory_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\centerstance_cycle 1
	TEnd
MEnd


#############################################################
## "The Champ"
#############################################################

Move Emote_Champion
	Requires	EMOTE CHEER FLEX
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Champion_cycle
	Interpolate	8
	Flags		FIXARMREG

	Type		male
	  Anim          male\champion 1 150
	TEnd

	Type		huge
	  Anim          huge\champion 1 150
	TEnd

	Type		fem
	  Anim          fem\champion 1 150
	TEnd
MEnd

Move Emote_Champion_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\centerstance_cycle 1
	TEnd

	Type		huge
	  Anim          huge\champion 151 152
	TEnd

	Type		fem
	  Anim          fem\champion 151 152
	TEnd
MEnd


#############################################################
## "The Wave"
#############################################################

Move Emote_TheWave
	Requires	EMOTE CHEER WAVE HIGH
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_TheWave_cycle
	Interpolate	8

	Type		male
	  Anim          male\the_wave 1
	TEnd
MEnd

Move Emote_TheWave_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\centerstance_cycle 1
	TEnd
MEnd


#############################################################
## Frustrated
#############################################################

Move Emote_Frustrated
	Requires	EMOTE ANGRY SAD MID
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Frustrated2
	Interpolate	8

	Type		male
	  Anim          male\idiots 1 2
	TEnd
MEnd

Move Emote_Frustrated2
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Frustrated_cycle

	Type		male
	  Anim          male\idiots 3
	TEnd
MEnd

Move Emote_Frustrated_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\frustrated_cycle 1
	TEnd
MEnd


#############################################################
## Drat
#############################################################

Move Emote_Drat
	Requires	EMOTE ANGRY RUDE SHRUG
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Drat2
	Interpolate	8

	Type		male
	  Anim          male\aw_crap 1 2
	TEnd
MEnd

Move Emote_Drat2
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Drat_cycle


	Type		male
	  Anim          male\aw_crap 3
	TEnd
MEnd

Move Emote_Drat_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\frustrated_cycle 1
	TEnd
MEnd


#############################################################
## "Grief"
#############################################################

Move Emote_Grief
	Requires	EMOTE SAD POSE SHRUG
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Grief_cycle
	Interpolate	8
	Scale		0.8
	Flags		FIXARMREG

	Type		male
	  Anim          male\grief 1 150
	TEnd

	Type		huge
	  Anim          huge\grief 1 150
	TEnd

	Type		fem
	  Anim          fem\grief 1 150
	TEnd
MEnd


Move Emote_Grief_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\grief 150 155
	TEnd

	Type		huge
	  Anim          huge\grief 150 155
	TEnd

	Type		fem
	  Anim          fem\grief 150 155
	TEnd
MEnd

#############################################################
## "Thank You" - Small
#############################################################

Move Emote_Thankyou_SM
	Requires	EMOTE THANK LOW POSE
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Thankyou_SM_cycle

	Type		male
	  Anim          male\thankyou_sm 1
	TEnd
MEnd

Move Emote_Thankyou_SM_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\centerstance_cycle 1
	TEnd
MEnd

#############################################################
## "You're Welcome"
#############################################################

Move Emote_YourWelcome
	Requires	EMOTE THANK LOW POSE REPAIR
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesLefthand> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_YourWelcome_cycle
	Priority	5
	Scale		1.2

	Type		male
	  Anim          male\your_welcome_sm 1
	TEnd
MEnd

Move Emote_YourWelcome_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\centerstance_cycle 1
	TEnd
MEnd

#############################################################
## "Welcome"
#############################################################

Move Emote_Welcome
	Requires	EMOTE GREETING HIGH
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield> #<IRQEmotesRighthand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Welcome_cycle
	Interpolate	8

	Type		male
	  Anim          male\welcome 1 135
	TEnd
MEnd

Move Emote_Welcome_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\welcome 136 140
	TEnd
MEnd

#############################################################
## Judge Score = 1
#############################################################

Move Emote_Judge_1
	Requires	EMOTE SCORE ONE
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> #<IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Judge_1_cycle

	Type		male
	  Anim          male\judge 1 40
	  PlayFx        GENERIC\Score_1.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\judge 1 40
	  PlayFx        GENERIC\Score_1.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Judge_1_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_1.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\judge 41
	  PlayFx        GENERIC\Score_1.fx 0 CONSTANT
	TEnd
MEnd


#############################################################
## Judge Score = 2
#############################################################

Move Emote_Judge_2
	Requires	EMOTE SCORE TWO
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> #<IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Judge_2_cycle

	Type		male
	  Anim          male\judge 1 40
	  PlayFx        GENERIC\Score_2.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\judge 1 40
	  PlayFx        GENERIC\Score_2.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Judge_2_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_2.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\judge 41
	  PlayFx        GENERIC\Score_2.fx 0 CONSTANT
	TEnd
MEnd


#############################################################
## Judge Score = 3
#############################################################

Move Emote_Judge_3
	Requires	EMOTE SCORE THREE
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> #<IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Judge_3_cycle

	Type		male
	  Anim          male\judge 1 40
	  PlayFx        GENERIC\Score_3.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\judge 1 40
	  PlayFx        GENERIC\Score_3.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Judge_3_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_3.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\judge 41
	  PlayFx        GENERIC\Score_3.fx 0 CONSTANT
	TEnd
MEnd


#############################################################
## Judge Score = 4
#############################################################

Move Emote_Judge_4
	Requires	EMOTE SCORE FOUR
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> #<IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Judge_4_cycle

	Type		male
	  Anim          male\judge 1 40
	  PlayFx        GENERIC\Score_4.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\judge 1 40
	  PlayFx        GENERIC\Score_4.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Judge_4_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_4.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\judge 41
	  PlayFx        GENERIC\Score_4.fx 0 CONSTANT
	TEnd
MEnd


#############################################################
## Judge Score = 5
#############################################################

Move Emote_Judge_5
	Requires	EMOTE SCORE FIVE
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> #<IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Judge_5_cycle

	Type		male
	  Anim          male\judge 1 40
	  PlayFx        GENERIC\Score_5.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\judge 1 40
	  PlayFx        GENERIC\Score_5.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Judge_5_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_5.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\judge 41
	  PlayFx        GENERIC\Score_5.fx 0 CONSTANT
	TEnd
MEnd


#############################################################
## Judge Score = 6
#############################################################

Move Emote_Judge_6
	Requires	EMOTE SCORE SIX
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> #<IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Judge_6_cycle

	Type		male
	  Anim          male\judge 1 40
	  PlayFx        GENERIC\Score_6.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\judge 1 40
	  PlayFx        GENERIC\Score_6.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Judge_6_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_6.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\judge 41
	  PlayFx        GENERIC\Score_6.fx 0 CONSTANT
	TEnd
MEnd


#############################################################
## Judge Score = 7
#############################################################

Move Emote_Judge_7
	Requires	EMOTE SCORE SEVEN
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> #<IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Judge_7_cycle

	Type		male
	  Anim          male\judge 1 40
	  PlayFx        GENERIC\Score_7.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\judge 1 40
	  PlayFx        GENERIC\Score_7.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Judge_7_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_7.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\judge 41
	  PlayFx        GENERIC\Score_7.fx 0 CONSTANT
	TEnd
MEnd


#############################################################
## Judge Score = 8
#############################################################

Move Emote_Judge_8
	Requires	EMOTE SCORE EIGHT
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> #<IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Judge_8_cycle

	Type		male
	  Anim          male\judge 1 40
	  PlayFx        GENERIC\Score_8.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\judge 1 40
	  PlayFx        GENERIC\Score_8.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Judge_8_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_8.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\judge 41
	  PlayFx        GENERIC\Score_8.fx 0 CONSTANT
	TEnd
MEnd


#############################################################
## Judge Score = 9
#############################################################

Move Emote_Judge_9
	Requires	EMOTE SCORE NINE
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> #<IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Judge_9_cycle

	Type		male
	  Anim          male\judge 1 40
	  PlayFx        GENERIC\Score_9.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\judge 1 40
	  PlayFx        GENERIC\Score_9.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Judge_9_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_9.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\judge 41
	  PlayFx        GENERIC\Score_9.fx 0 CONSTANT
	TEnd
MEnd


#############################################################
##  Judge Score = 10
#############################################################

Move Emote_Judge_10
	Requires	EMOTE SCORE TEN
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> #<IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Judge_10_cycle

	Type		male
	  Anim          male\judge 1 40
	  PlayFx        GENERIC\Score_10.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\judge 1 40
	  PlayFx        GENERIC\Score_10.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Judge_10_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_10.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\judge 41
	  PlayFx        GENERIC\Score_10.fx 0 CONSTANT
	TEnd
MEnd


#############################################################
## "Shucks!"
#############################################################

Move Emote_Shucks
	Requires	EMOTE ANGRY RUDE SHRUG LOW
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesLefthand> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Shucks_cycle
	Priority	5

	Type		male
	  Anim          male\shucks 1
	TEnd
MEnd

Move Emote_Shucks_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\centerstance_cycle 1
	TEnd
MEnd


#############################################################
## Panhandle
#############################################################

Move Emote_Panhandle
	Requires	EMOTE PANHANDELING
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Panhandle_cycle_1
	Interpolate	8
	Flags		FIXARMREG

	Type		male
	  Anim          male\bum_cup_cycle 1
  	  PlayFx	GENERIC\cup_tin_r.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Panhandle_cycle_1
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Panhandle_cycle_2
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\bum_cup_beg_l 1 30
  	  PlayFx	GENERIC\cup_tin_r.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Panhandle_cycle_2
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Panhandle_cycle_3
	CycleMove	Emote_Panhandle_cycle_5
	Interpolate     13
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\bum_cup_beg_l 35 80 ##85
  	  PlayFx	GENERIC\cup_tin_r.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Panhandle_cycle_3
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Panhandle_cycle_7
	CycleMove	Emote_Panhandle_cycle_1
	CycleMove	Emote_Panhandle_cycle_4
	CycleMove	Emote_Panhandle
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\bum_cup_beg_l 85
  	  PlayFx	GENERIC\cup_tin_r.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Panhandle_cycle_4
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Panhandle_cycle_5
	Interpolate     15
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\bum_cup_beg_r 1 30
  	  PlayFx	GENERIC\cup_tin_r.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Panhandle_cycle_5
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Panhandle_cycle_6
	CycleMove	Emote_Panhandle_cycle_2
	Interpolate     13
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\bum_cup_beg_r 35 80 #85
  	  PlayFx	GENERIC\cup_tin_r.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Panhandle_cycle_6
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Panhandle_cycle_7
	CycleMove	Emote_Panhandle_cycle_1
	CycleMove	Emote_Panhandle
	CycleMove	Emote_Panhandle_cycle_4
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\bum_cup_beg_r 85
  	  PlayFx	GENERIC\cup_tin_r.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Panhandle_cycle_7
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Panhandle
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\bum_cup_jingle 1
  	  PlayFx	GENERIC\cup_tin_r.fx 0 CONSTANT
	TEnd
MEnd


#############################################################
## Wall Lean
#############################################################

Move Emote_Wall_Lean_1
	Requires	EMOTE LEANWALL
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Wall_Lean_cycle_1
	Flags		FIXARMREG

	Type		male
	  Anim          male\wall_lean1 1
	TEnd
MEnd

Move Emote_Wall_Lean_cycle_1
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\wall_lean1 1
	TEnd
MEnd

#############################################################

Move Emote_Wall_Lean_3
	Requires	EMOTE LEANWALL
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Wall_Lean_cycle_3
	Flags		FIXARMREG

	Type		male
	  Anim          male\wall_lean3 1
	TEnd
MEnd

Move Emote_Wall_Lean_cycle_3
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\wall_lean3 1
	TEnd
MEnd

#############################################################
## HandsUp
#############################################################

Move Emote_HandsUp
	Requires	EMOTE HANDSUP
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_HandsUp_cycle_1
	Interpolate	8

	Type		male
	  Anim          male\handsup 1
	TEnd
MEnd

Move Emote_HandsUp_cycle_1
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE
	CycleMove	Emote_HandsUp_cycle_1
	CycleMove	Emote_HandsUp_cycle_2
	CycleMove	Emote_HandsUp_cycle_1

	Type		male
	  Anim          male\handsup 1
	TEnd
MEnd

Move Emote_HandsUp_cycle_2
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE
	CycleMove	Emote_HandsUp_cycle_1

	Type		male
	  Anim          male\handsup_var 1
	TEnd
MEnd

#############################################################
## HandsUp Kneeling
#############################################################

Move Emote_HandsUp_Kneel
	Requires	EMOTE HANDSUP
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_HandsUp_Kneel_cycle_1
	Interpolate	8

	Type		male
	  Anim          male\handsup_knees 1
	TEnd
MEnd

Move Emote_HandsUp_Kneel_cycle_1
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE
	CycleMove	Emote_HandsUp_Kneel_cycle_1
	CycleMove	Emote_HandsUp_Kneel_cycle_2
	CycleMove	Emote_HandsUp_Kneel_cycle_1
	interpolate	20

	Type		male
	  Anim          male\handsup_knees 1
	TEnd
MEnd

Move Emote_HandsUp_Kneel_cycle_2
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE
	CycleMove	Emote_HandsUp_Kneel_cycle_1
	interpolate	25

	Type		male
	  Anim          male\handsup_knees_var 1
	TEnd
MEnd


#############################################################
## FlashLight
#############################################################

Move Emote_FlashLight_1
	Requires	EMOTE FLASHLIGHT
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_FlashLight_cycle_1
	Interpolate	8

	Type		male
	  Anim          male\Flashlight_Up 1
	  PlayFX	Generic\Flashlight_NoSpot.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_FlashLight_cycle_1
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\Flashlight_Up 1
	  PlayFX	Generic\Flashlight_NoSpot.fx 0 CONSTANT
	TEnd
MEnd

#############################################################

Move Emote_FlashLight_2
	Requires	EMOTE FLASHLIGHT
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_FlashLight_cycle_2
	Interpolate	8

	Type		male
	  Anim          male\Flashlight_Down 1
	  PlayFX	Generic\Flashlight_NoSpot.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_FlashLight_cycle_2
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\Flashlight_Down 1
	  PlayFX	Generic\Flashlight_NoSpot.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
## FlashLight - UP
#############################################################

Move Emote_FlashLight_Up_Pre
	Requires	EMOTE FLASHLIGHT UP
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_FlashLight_Up_Cycle
	Interpolate	8

	Type		male
	  Anim          male\Flashlight_Up 1
	  PlayFX	Generic\Flashlight_NoSpot.fx 0 CONSTANT
	TEnd

MEnd

Move Emote_FlashLight_Up_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\Flashlight_Up 1
	  PlayFX	Generic\Flashlight_NoSpot.fx 0 CONSTANT
	TEnd
MEnd


#############################################################
## FlashLight - DOWN
#############################################################

Move Emote_FlashLight_Down_Pre
	Requires	EMOTE FLASHLIGHT DOWN
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_FlashLight_cycle_2
	Interpolate	8

	Type		male
	  Anim          male\Flashlight_Down 1
	  PlayFX	Generic\Flashlight_NoSpot.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_FlashLight_Down_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\Flashlight_Down 1
	  PlayFX	Generic\Flashlight_NoSpot.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
## Book research
#############################################################

Move Emote_Book_research
	Requires	EMOTE BOOKRESEARCH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Book_research_cycle
	Interpolate	8
	Scale		0.5
	Flags		FIXARMREG

	Type		male
	  Anim          male\blackboard 1
	  PlayFx        GENERIC\book_left.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Book_research_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Scale		0.5
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\blackboard 1
	  PlayFx        GENERIC\book_left.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
## Book Research Low
#############################################################

Move Emote_Book_research_Low
	Requires	EMOTE BOOKRESEARCH LOW
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Book_research_cycle_low
	Interpolate	8
	Scale		0.5
	Flags		FIXARMREG

	Type		male
	  Anim          male\blackboard_low 1
	  PlayFx        GENERIC\book_left.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Book_research_cycle_low
	Include sequencers\Members\Emotes_Idle.inc
	Scale		0.5
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\blackboard_low 1
	  PlayFx        GENERIC\book_left.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
## Float Books
#############################################################

Move Emote_Float_Books
	Requires	EMOTE FLOATBOOKS
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Float_Books_cycle
	Interpolate	8
	Scale		0.5
	Flags		FIXARMREG

	Type		male
	  Anim          male\idle_2hips_look 1
	  PlayFx        GENERIC\cot_books.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Float_Books_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Scale		0.5
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\idle_2hips_look 1
	  PlayFx        GENERIC\cot_books.fx 0 CONSTANT
	TEnd
MEnd


#############################################################
## Hold Torch
#############################################################

Move Emote_Hold_Torch
	Requires	EMOTE HOLDTORCH
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> #<IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Hold_Torch_cycle
	Scale		0.5
	Interpolate	15

	Type		male
	  Anim          male\holdtorch 1
	  PlayFx        Generic\Torch_left.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Hold_Torch_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE
	Scale		0.5
	Interpolate	15

	Type		male
	  Anim          male\holdtorch 1
	  PlayFx        Generic\Torch_left.fx 0 CONSTANT
	TEnd
MEnd


#############################################################
## Drum
#############################################################

Move Emote_Drum
	Requires	EMOTE DRUM
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Drum_cycle
	Flags		NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\drum_high 1
	  PlayFx        GENERIC\drum_big.fx 0 CONSTANT
	  PlayFx        Emotes\Drum_Large.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          male\drum_high 1
	  PlayFx        GENERIC\huge_drum_big.fx 0 CONSTANT
	  PlayFx        Emotes\Drum_Large.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          male\drum_high 1
	  PlayFx        GENERIC\fem_drum_big.fx 0 CONSTANT
	  PlayFx        Emotes\Drum_Large.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Drum_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\drum_high 1
	  PlayFx        GENERIC\drum_big.fx 0 CONSTANT
	  PlayFx        Emotes\Drum_Large.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          male\drum_high 1
	  PlayFx        GENERIC\huge_drum_big.fx 0 CONSTANT
	  PlayFx        Emotes\Drum_Large.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          male\drum_high 1
	  PlayFx        GENERIC\fem_drum_big.fx 0 CONSTANT
	  PlayFx        Emotes\Drum_Large.fx 0 CONSTANT
	TEnd
MEnd


#############################################################
## Drum Low
#############################################################

Move Emote_Drum_Low
	Requires	EMOTE DRUM LOW
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Drum_Low_cycle
	Flags		NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\drum_low 1
	  PlayFx        Emotes\Drum_Small.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          male\drum_low 1
	  PlayFx        Emotes\Drum_Small_Huge.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          male\drum_low 1
	  PlayFx        Emotes\Drum_Small_Fem.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Drum_Low_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Interpolate	15
	Flags		CYCLE NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\drum_low 1
	  PlayFx        Emotes\Drum_Small.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          male\drum_low 1
	  PlayFx        Emotes\Drum_Small_Huge.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          male\drum_low 1
	  PlayFx        Emotes\Drum_Small_Fem.fx 0 CONSTANT
	TEnd
MEnd


#############################################################
## Tribal Dance
#############################################################

Move Emote_Tribal_Dance
	Requires	EMOTE TRIBALDANCE
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Tribal_Dance_cycle_1
	Interpolate	8
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\tribal_dance 1
	TEnd
MEnd

Move Emote_Tribal_Dance_cycle_1
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NOSIZESCALE
	CycleMove	Emote_Tribal_Dance
	CycleMove	Emote_Tribal_Dance_cycle_1

	Type		male
	  Anim          male\tribal_dance_alt 1
	TEnd
MEnd


#############################################################
## Assume Position
#############################################################

Move Emote_Assume_Position1
	Requires	EMOTE ASSUMEPOSITION
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Assume_Position1_cycle
	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\AssumePosition_1 1
	TEnd
MEnd

Move Emote_Assume_Position1_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\AssumePosition_1 1
	TEnd
MEnd

#############################################################

Move Emote_Assume_Position2
	Requires	EMOTE ASSUMEPOSITION
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Assume_Position2_cycle
	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\AssumePosition_2 1
	TEnd
MEnd

Move Emote_Assume_Position2_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\AssumePosition_2 1
	TEnd
MEnd

#############################################################

Move Emote_Assume_Position3
	Requires	EMOTE ASSUMEPOSITION
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Assume_Position3_cycle
	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\AssumePosition_3 1
	TEnd
MEnd

Move Emote_Assume_Position3_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\AssumePosition_3 1
	TEnd
MEnd

#############################################################

Move Emote_Assume_Position4
	Requires	EMOTE ASSUMEPOSITION
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Assume_Position4_cycle
	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\AssumePosition_4 1
	TEnd
MEnd

Move Emote_Assume_Position4_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\AssumePosition_4 1
	TEnd
MEnd

#############################################################
## Peer In
#############################################################

Move Emote_Peer_In
	Requires	EMOTE PEERIN
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Peer_In_cycle_1
	Flags		FIXARMREG

	Type		male
	  Anim          male\PeerIn 1 20
	TEnd
MEnd

Move Emote_Peer_In_cycle_1
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Peer_In_cycle_2
	CycleMove	Emote_Peer_In_cycle_1
	CycleMove	Emote_Peer_In_cycle_1
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\PeerIn 20
	TEnd
MEnd

Move Emote_Peer_In_cycle_2
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Peer_In
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\lookquick 1
	TEnd
MEnd

#############################################################
## "Bat Smash"
#############################################################

Move Emote_Bat_smash
	Requires	EMOTE BATSMASH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Bat_smash_cycle
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\bat_smash_simple 1 47
	  PlayFx        WEAPONS\BASEBALL_BAT.FX 0 CONSTANT
	TEnd

	Type		beast
	  Anim          male\bat_smash_simple 1 47
	  PlayFx        GENERIC\meatbone.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Bat_smash_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NOSIZESCALE

	Type		male
	  Anim          male\bat_smash_simple 1 47
	  PlayFx        WEAPONS\BASEBALL_BAT.FX 0 CONSTANT
	TEnd

	Type		beast
	  Anim          male\bat_smash_simple 1 47
	  PlayFx        GENERIC\meatbone.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
## "Bat Smash" React
#############################################################

Move Emote_Bat_smash_react
	Requires	EMOTE BATSMASHREACT
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Bat_smash_react_cycle
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\bat_smash_react 1 47
	TEnd
MEnd

Move Emote_Bat_smash_react_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NOSIZESCALE

	Type		male
	  Anim          male\bat_smash_react 1 47
	TEnd
MEnd

#############################################################
## Protest
#############################################################

Move Emote_Protest
	Requires	EMOTE PROTEST
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Protest_cycle
	Interpolate	8
	Flags		NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\sign_chant 1
	  PlayFx        GENERIC\Protest_sign_A.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          male\sign_onearm_chant 1
	  PlayFx        GENERIC\Protest_sign_A.fx 0 CONSTANT
	TEnd

	Type		5cw
	  Anim          male\sign_onearm_chant 1
	  PlayFx        GENERIC\Protest_sign_A.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Protest_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\sign_chant 1
	  PlayFx        GENERIC\Protest_sign_A.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          male\sign_onearm_chant 1
	  PlayFx        GENERIC\Protest_sign_A.fx 0 CONSTANT
	TEnd

	Type		5cw
	  Anim          male\sign_onearm_chant 1
	  PlayFx        GENERIC\Protest_sign_A.fx 0 CONSTANT
	TEnd
MEnd


#############################################################
## Fear
#############################################################

Move Emote_Fear
	Requires	EMOTE FEAR
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Fear_cycle

	Type		male
	  Anim          male\fear_stun 1
	TEnd
MEnd

Move Emote_Fear_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\fear_stun 25 60
	TEnd
MEnd


#############################################################
## Slap React
#############################################################

Move Emote_Slap_React
	Requires	EMOTE SLAPREACT
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Slap_React_2
	Interpolate	15
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\combat_block 1 22
	TEnd
MEnd

Move Emote_Slap_React_2
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NOSIZESCALE
	NextMove	Emote_Slap_React_cycle

	Type		male
	  Anim          male\stun_hit 2 96
	TEnd
MEnd

Move Emote_Slap_React_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE
	Scale		0.5

	Type		male
	  Anim          male\stun_cycle 1 31
	TEnd
MEnd

#############################################################
## "Ala Kazam!"
#############################################################

Move Emote_AlaKazam
	Requires	EMOTE ALAKAZAM
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	postReady_back
	Interpolate	15
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\magic_L 1 ##30
	TEnd
MEnd


#############################################################
## "Ala Kazam!" Reaction
#############################################################

Move Emote_AlaKazam_React
	Requires	EMOTE ALAKAZAMREACT
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Member		<DeathIRQ>
	NextMove	Emote_AlaKazam_React_hidden
	Sets		CANTMOVE

	Type		male
	  Anim          male\freezestun 1 20
	  PlayFX	Emotes\CostumeChanges\PrestoChango\PrestoChango.fx 5
	  PlayFX	GENERIC\AlakazamReact.fx 8 CONSTANT
	  PlayFx	Generic\entityVanish.fx 6 CONSTANT
	TEnd
MEnd

Move Emote_AlaKazam_React_hidden
	Member		<DeathIRQ>
	NextMove	Emote_AlaKazam_React_cycle
	Scale		0.5
	Sets		CANTMOVE

	Type		male
	  Anim          male\freezestun 20
	  PlayFX	GENERIC\AlakazamReact.fx 0 CONSTANT
	  PlayFx	Generic\entityVanish.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_AlaKazam_React_cycle
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_What

	Type		male
	  Anim          male\lookquick 1
	  PlayFX	Emotes\CostumeChanges\PrestoChango\PrestoChango.fx 0
	TEnd
MEnd


#############################################################
## Drink
#############################################################

Move Emote_Drink
	Requires	EMOTE DRINK ONE
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Drink_cycle_A
	Flags		FIXARMREG

	Type		male
	  Anim          male\PL_DrinkSip 1
	  PlayFx        Generic\FX_DrinkingCup.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\PL_Drink_New 1 111
	  PlayFx        Generic\FX_DrinkingCup.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\PL_DrinkSip 1
	  PlayFx        Generic\FX_DrinkingCup.fx 0 Constant
	TEnd
MEnd

Move Emote_Drink_cycle_A
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Drink_cycle_A
	CycleMove	Emote_Drink_cycle_B
	CycleMove	Emote_Drink_cycle_C
	CycleMove	Emote_Drink_cycle_A
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\PL_DrinkIdle 1
	  PlayFx        Generic\FX_DrinkingCup.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\PL_Drink_New 112 282
	  PlayFx        Generic\FX_DrinkingCup.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\PL_DrinkIdle 1
	  PlayFx        Generic\FX_DrinkingCup.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Drink_cycle_B
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Drink_cycle_A
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\PL_DrinkSip 1
	  PlayFx        Generic\FX_DrinkingCup.fx 0 CONSTANT
	TEnd


	Type		fem
	  Anim          fem\PL_Drink_New 1 111
	  PlayFx        Generic\FX_DrinkingCup.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\PL_DrinkSip 1
	  PlayFx        Generic\FX_DrinkingCup.fx 0 CONSTANT
	TEnd

MEnd

Move Emote_Drink_cycle_C
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Drink_cycle_A
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\PL_DrinkSipBig 1
	  PlayFx        Generic\FX_DrinkingCup.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\PL_Drink_New 283 393
	  PlayFx        Generic\FX_DrinkingCup.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\PL_DrinkSipBig 1
	  PlayFx        Generic\FX_DrinkingCup.fx 0 CONSTANT
	TEnd
MEnd

#############################################################

Move Emote_Drink_ClayMug
	Requires	EMOTE DRINK TWO
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Drink_ClayMug_CycleA
	Flags		FIXARMREG

	Type		male
	  Anim          male\PL_DrinkSip 1
	  PlayFx        Generic\FX_DrinkingClayMug.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\PL_Drink_New 1 111
	  PlayFx        Generic\FX_DrinkingCup.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\PL_DrinkSip 1
	  PlayFx        Generic\FX_DrinkingCup.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Drink_ClayMug_CycleA
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Drink_ClayMug_CycleA
	CycleMove	Emote_Drink_ClayMug_CycleB
	CycleMove	Emote_Drink_ClayMug_CycleC
	CycleMove	Emote_Drink_ClayMug_CycleA
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\PL_DrinkIdle 1
	  PlayFx        Generic\FX_DrinkingClayMug.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\PL_Drink_New 112 282
	  PlayFx        Generic\FX_DrinkingCup.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\PL_DrinkIdle 1
	  PlayFx        Generic\FX_DrinkingCup.fx 0 CONSTANT
	TEnd

MEnd

Move Emote_Drink_ClayMug_CycleB
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Drink_ClayMug_CycleA
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\PL_DrinkSip 1
	  PlayFx        Generic\FX_DrinkingClayMug.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\PL_Drink_New 1 111
	  PlayFx        Generic\FX_DrinkingCup.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\PL_DrinkSip 1
	  PlayFx        Generic\FX_DrinkingCup.fx 0 CONSTANT
	TEnd

MEnd

Move Emote_Drink_ClayMug_CycleC
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Drink_ClayMug_CycleA
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\PL_DrinkSipBig 1
	  PlayFx        Generic\FX_DrinkingClayMug.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\PL_Drink_New 283 393
	  PlayFx        Generic\FX_DrinkingCup.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\PL_DrinkSipBig 1
	  PlayFx        Generic\FX_DrinkingCup.fx 0 CONSTANT
	TEnd

MEnd

#############################################################
## Talker
#############################################################

Move Emote_Talk_Pre
	Requires	EMOTE TALK
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NOSIZESCALE
	NextMove	Emote_Talk_Cycle
	Scale		0.75

	Type		male
	  Anim          male\PL_TalkIdle 1
	TEnd
MEnd

Move Emote_Talk_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NOSIZESCALE
	Scale		0.75

	CycleMove	Emote_Talk_Cycle
	CycleMove	Emote_Talk01
	CycleMove	Emote_Talk02
	CycleMove	Emote_Talk03

	Type		male
	  Anim          male\PL_TalkIdle 1
	TEnd
MEnd

Move Emote_Talk01
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NOSIZESCALE
	NextMove	Emote_Idle1_Cycle
	Scale		0.75

	CycleMove	Emote_Talk_Cycle
	CycleMove	Emote_Talk_Cycle
	CycleMove	Emote_Talk02
	CycleMove	Emote_Talk03

	Type		male
	  Anim          male\PL_Talk01 1
	TEnd
MEnd

Move Emote_Talk02
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NOSIZESCALE
	NextMove	Emote_Idle1_Cycle
	Scale		0.75

	CycleMove	Emote_Talk_Cycle
	CycleMove	Emote_Talk_Cycle
	CycleMove	Emote_Talk01
	CycleMove	Emote_Talk03

	Type		male
	  Anim          male\PL_Talk02 1
	TEnd
MEnd

Move Emote_Talk03
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NOSIZESCALE
	NextMove	Emote_Idle1_Cycle
	Scale		0.75

	CycleMove	Emote_Talk_Cycle
	CycleMove	Emote_Talk_Cycle
	CycleMove	Emote_Talk01
	CycleMove	Emote_Talk02

	Type		male
	  Anim          male\PL_Talk03 1
	TEnd
MEnd

#############################################################
## Yata Yata
#############################################################

Move Emote_YataYata
	Requires	EMOTE YATA
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Idle1_Cycle
	Scale		1.1

	Type		male
	  Anim          male\YataYata 1
	TEnd
MEnd

#############################################################
## Laugh Two
#############################################################

Move Emote_LAUGHTWO
	Requires	EMOTE LAUGHTWO
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Idle1_Cycle
	Interpolate	3

	Type		male
	  Anim          male\emote_Laugh2 1
	TEnd
MEnd

#############################################################
## Villain Laugh
#############################################################

Move Emote_VillainLaugh
	Requires	EMOTE VILLAINLAUGH
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Idle2_Cycle
	Interpolate	3

	Type		male
	  Anim          male\emote_VillainLaugh 1
	TEnd
MEnd

#############################################################
## Slash Throat
#############################################################

Move Emote_SlashThroat
	Requires	EMOTE SLASHTHROAT
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Idle1_Cycle
	Flags		FIXARMREG

	Type		male
	  Anim          male\emote_SlashThroat 1
	TEnd
MEnd

#############################################################
## Curse You
#############################################################

Move Emote_CurseYou
	Requires	EMOTE CURSEYOU
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Idle1_Cycle
	Interpolate	4

	Type		male
	  Anim          male\emote_CurseYou 1 80
	TEnd
MEnd


#############################################################
## Sleep
#############################################################

Move Emote_Sleep
	Requires	EMOTE ASLEEP
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_SleepLoop
	Interpolate	7
	scale		0.9

	Type		male
	  Anim          male\emote_sleep 15 95
	TEnd
MEnd

Move Emote_SleepLoop
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE
	NextMove	Emote_Idle1_Cycle
	Interpolate     4

	Type		male
	  Anim          male\emote_Sleep 96 191
	  PlayFX	V_COV\Items\Emote_SleepZ.fx 0 CONSTANT
	TEnd

MEnd

#############################################################
## Dust Off
#############################################################

Move Emote_DustOff
	Requires	EMOTE DUSTOFF
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_DustOff2
	Scale		1.1
	Flags		FIXARMREG

	Type		male
	  Anim          male\emote_DustOff 1 70
	TEnd

	Type		huge
	  Anim          huge\emote_DustOff 1 70
	TEnd

	Type		fem
	  Anim          fem\emote_DustOff 1 70
	TEnd
MEnd

Move Emote_DustOff2
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Idle1_Cycle
	Scale		1.1
	Flags		FIXARMREG

	Type		male
	  Anim          male\emote_DustOff 71
	TEnd

	Type		huge
	  Anim          huge\emote_DustOff 71
	TEnd

	Type		fem
	  Anim          fem\emote_DustOff 71
	TEnd
MEnd


#############################################################
##  Plotting
#############################################################

Move Emote_Plotting
	Requires	EMOTE PLOTTING
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Plotting_Cycle
	Flags		FIXARMREG
	Interpolate	5

	Type		male
	  Anim          male\contemplate 1 20
	TEnd

	Type		huge
	  Anim          huge\contemplate 1 20
	TEnd

	Type		fem
	  Anim          fem\contemplate 1 20
	TEnd
MEnd

Move Emote_Plotting_Cycle
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Idle1_Cycle
	Flags		FIXARMREG Cycle
	Interpolate	5

	Type		male
	  Anim          male\contemplate 20 80
	TEnd

	Type		huge
	  Anim          huge\contemplate 20 80
	TEnd

	Type		fem
	  Anim          fem\contemplate 20 80
	TEnd

MEnd


#############################################################
## Chicken
#############################################################

Move Emote_Chicken
	Requires	EMOTE CHICKEN
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Ready_Chicken
	Interpolate	5

	Type		male
	  Anim          male\emote_Chicken 1
	TEnd

	Type		fem
	  Anim          fem\emote_Chicken 1
	TEnd

	Type		huge
	  Anim          huge\emote_Chicken 1
	TEnd
MEnd

Move Ready_Chicken
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE
	Scale		0.5
	Interpolate	15

	Type		male
	  Anim          male\stand_2Hhips 40 64
	TEnd

	Type		huge
	  Anim          huge\stand_2Hhips 40 64
	TEnd

	Type		fem
	  Anim          fem\stand_2Hhips 40 64
	TEnd
MEnd


#############################################################
## Laptop
#############################################################

Move Emote_Laptop
	Requires	EMOTE LAPTOP
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Laptop_cycle_1
	Interpolate	8
	Flags		FIXARMREG

	Type		male
	  Anim          male\Portable_Laptop 1
	  PlayFX	Generic\Stand_LaptopTable.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          male\Portable_Laptop_Rest 1
	  PlayFX	Generic\huge_Stand_LaptopTable.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          male\Portable_Laptop_Rest 1
	  PlayFX	Generic\fem_Stand_LaptopTable.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Laptop_cycle_1
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Laptop
	CycleMove	Emote_Laptop_cycle_1
	CycleMove	Emote_Laptop_cycle_2
	Flags		CYCLE FIXARMREG


	Type		male
	  Anim          male\Portable_Laptop_Rest 1
	  PlayFX	Generic\Stand_LaptopTable.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          male\Portable_Laptop_Rest 1
	  PlayFX	Generic\huge_Stand_LaptopTable.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          male\Portable_Laptop_Rest 1
	  PlayFX	Generic\fem_Stand_LaptopTable.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Laptop_cycle_2
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG
	CycleMove	Emote_Laptop
	CycleMove	Emote_Laptop_cycle_1


	Type		male
	  Anim          male\Portable_Laptop_Delete 1
	  PlayFX	Generic\Stand_LaptopTable.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          male\Portable_Laptop_Rest 1
	  PlayFX	Generic\huge_Stand_LaptopTable.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          male\Portable_Laptop_Rest 1
	  PlayFX	Generic\fem_Stand_LaptopTable.fx 0 CONSTANT
	TEnd
MEnd


#############################################################
## Typing - No FX geometry, just animation
#############################################################

Move Emote_LaptopType
	Requires	EMOTE TYPING
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_LaptopType02
	Flags		FIXARMREG

	Type		male
	  Anim          male\emote_LaptopType 1
	TEnd
MEnd

Move Emote_LaptopType02
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG
	CycleMove	Emote_LaptopRest
	CycleMove	Emote_LaptopRest
	CycleMove	Emote_LaptopType02

	Type		male
	  Anim          male\emote_LaptopType 1
	TEnd
MEnd

Move Emote_LaptopRest
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG
	CycleMove	Emote_LaptopRest
	CycleMove	Emote_LaptopType02
	CycleMove	Emote_LaptopType02
	CycleMove	Emote_LaptopDelete

	Type		male
	  Anim          male\emote_LaptopRest 1
	TEnd
MEnd

Move Emote_LaptopDelete
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG
	CycleMove	Emote_LaptopRest
	CycleMove	Emote_LaptopRest
	CycleMove	Emote_LaptopType02

	Type		male
	  Anim          male\emote_LaptopDelete 1
	TEnd
MEnd

#############################################################
## Touch Screen
#############################################################

Move Emote_ScreenIdle
	Requires	EMOTE SCREEN
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_ScreenIdleLoop
	Interpolate	15

	Type		male
	  Anim          male\emote_TouchScreen 1 117
	TEnd
MEnd

Move Emote_ScreenIdleLoop
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE
	CycleMove	Emote_TouchScreenHit
	CycleMove	Emote_TouchScreen

	Type		male
	  Anim          male\emote_TouchScreen 110
	TEnd
MEnd

Move Emote_TouchScreen
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE
	CycleMove	Emote_ScreenIdleLoop
	CycleMove	Emote_ScreenIdleLoop
	CycleMove	Emote_TouchScreenHit

	Type		male
	  Anim          male\emote_TouchScreen 1 117
	TEnd
MEnd

Move Emote_TouchScreenHit
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE
	CycleMove	Emote_ScreenIdleLoop
	CycleMove	Emote_TouchScreen

	Type		male
	  Anim          male\emote_TouchScreenHit 1
	TEnd
MEnd


#############################################################
## Go Away
#############################################################

Move Emote_GoAway
	Requires	EMOTE GOAWAY
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Nextmove	Emote_Idle1_Cycle

	Type		male
	  Anim          male\Emote_GoAway 1
	TEnd
MEnd


#############################################################
## Talk to the hand
#############################################################

Move Emote_TalktoHand
	Requires	EMOTE TALKTOHAND
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Nextmove	Emote_Idle1_Cycle
	Scale		0.8

	Type		male
	  Anim          male\Emote_TalktoHand 1
	TEnd
MEnd


#############################################################
## Kneel before ZOD
#############################################################

Move Emote_Bow_Down
	Requires	EMOTE BOWDOWN
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesLefthand> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	Nextmove	Emote_Idle1_Cycle
	Scale		0.8

	Type		male
	  Anim		male\Emote_BowDown 1
	TEnd

MEnd

#############################################################
## Forward slap
#############################################################

Move Emote_Slap
	Requires	EMOTE SLAP
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_NoSpam.inc
	Nextmove	Emote_Idle1_Cycle

	Type		male
	  Anim          male\Emote_Slap 1
	  Playfx	generic\Slap1.fx
	TEnd
MEnd


#############################################################
## Backhand Smack
#############################################################

Move Emote_Smack
	Requires	EMOTE SMACK
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_NoSpam.inc
	Nextmove	Emote_Idle1_Cycle

	Type		male
	  Anim          male\Emote_Smack 3
	  Playfx	generic\Smack1.fx
	TEnd

MEnd

#############################################################
## Pimphand threaten
#############################################################

Move Emote_SmackThreat
	Requires	EMOTE STRONGHAND
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesLefthand> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	Nextmove	Emote_Idle1_Cycle

	Type		male
	  Anim          male\Emote_Stronghand 1
	TEnd
MEnd


#############################################################
## Backflip
#############################################################

Move Emote_Backflip
	Requires	EMOTE BACKFLIP
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> #<IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Nextmove	Emote_Idle1_Cycle
	Interpolate	5

	Type		male
	  Anim          male\Emote_Backflip 1 63
	TEnd
MEnd

#############################################################

Move Emote_Backflip_Shield
	Requires	EMOTE SHIELD BACKFLIP
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Nextmove	Emote_Idle1_Cycle
	Interpolate	5

	Type		male
	  Anim          male\emote_backflip_shield 1 63
	TEnd
MEnd

#############################################################
## Scheme
#############################################################

Move Scheme
	Requires	EMOTE SCHEME
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Nextmove	Scheme_cycle
	Scale		1.5
	Flags		FIXARMREG

	Type		male
	  Anim          male\emote_scheme 1 50
	TEnd

	Type		fem
	  Anim          fem\emote_scheme 1 50
	TEnd

	Type		huge
	  Anim          huge\emote_scheme 1 50
	TEnd
MEnd

Move Scheme_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\emote_scheme 50
	TEnd

	Type		fem
	  Anim          fem\emote_scheme 50
	TEnd

	Type		huge
	  Anim          huge\emote_scheme 50
	TEnd
MEnd


#############################################################
## Kiss my Butt
#############################################################

Move Emote_KissMyButt
	Requires	EMOTE KISSMYBUTT
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesLefthand> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	Nextmove	Emote_Idle1_Cycle
	Flags		FIXARMREG

	Type		male
	  Anim          male\Emote_KissMyButt 1
	TEnd
MEnd


#############################################################
## Crack Knuckles
#############################################################

Move Emote_CrackKnuckles
	Requires	EMOTE KNUCKLECRACK
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Nextmove	Emote_Idle1_Cycle
	Flags		FIXARMREG

	Type		male
	  Anim          male\emote_knuckle_crack 1  80
	  playfx	V_COV\Items\Emote_knuckles.fx
	TEnd

	Type		fem
	  Anim          fem\emote_knuckle_crack 1  80
	  playfx	V_COV\Items\Emote_knuckles.fx
	TEnd

	Type		huge
	  Anim          huge\emote_knuckle_crack 1  80
	  playfx	V_COV\Items\Emote_knuckles.fx
	TEnd
MEnd


#############################################################
## Vampire Crouch
#############################################################

Move Emote_Crouch_Pre
	Requires	EMOTE POSE CROUCH
	Priority        3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Crouch
	Scale		0.5

	Type		male
	  Anim          male\crouch_look 1 2
	TEnd
MEnd

Move Emote_Crouch
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE
	Scale		0.5

	Type		male
	  Anim          male\crouch_look 3
	TEnd
MEnd


#############################################################
## Wedding - Marriage Proposal
#############################################################

Move Emote_MarriageProposal_Pre
	Requires	EMOTE PROPOSE
	Priority        2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_MarriageProposal_Cycle

	Type		male
	  Anim          male\emote_MarriageProposal 25 119
	TEnd
MEnd

Move Emote_MarriageProposal_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\emote_MarriageProposal 120 179
	TEnd
MEnd

#############################################################
## Wedding - Toss Rice
#############################################################

Move Emote_ThrowRice
	Requires	EMOTE UNDERHANDTOSS ONE
	Priority        3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesLefthand> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Idle1_Cycle

	Type		male
	  Anim          male\emote_UnderhandToss 2
	  PlayFx        Emotes\ThrowRice.fx 30 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_UnderhandToss 2
	  PlayFx        Emotes\ThrowRice.fx 28 CONSTANT
	TEnd
MEnd


#############################################################
## Wedding - Throw Confetti
#############################################################

Move Emote_ThrowConfetti
	Requires	EMOTE UNDERHANDTOSS TWO
	Priority        3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesLefthand> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Idle1_Cycle

	Type		male
	  Anim          male\emote_UnderhandToss 2
	  PlayFx        Emotes\ThrowConfetti.fx 31 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_UnderhandToss 2
	  PlayFx        Emotes\ThrowConfetti.fx 28 CONSTANT
	TEnd
MEnd


#############################################################
## Wedding - Throw Rose Petals
#############################################################

Move Emote_ThrowRosePetals
	Requires	EMOTE UNDERHANDTOSS THREE
	Priority        3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesLefthand> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Idle1_Cycle

	Type		male
	  Anim          male\emote_UnderhandToss 2
	  PlayFx        Emotes\ThrowRosePetals.fx 30 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_UnderhandToss 2
	  PlayFx        Emotes\ThrowRosePetals.fx 28 CONSTANT
	TEnd
MEnd

#############################################################
## Idle 1 - Default Ready
#############################################################

Move Emote_Idle1
	Requires	EMOTE POSE IDLE ONE
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Idle1_Cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\Skel_ready2 1
	TEnd

	Type		huge
	  Anim          huge\Skel_ready 1 60
	TEnd

	Type		fem
	  Anim          fem\Skel_ready2 1
	TEnd
MEnd

Move Emote_Idle1_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NoSizeScale

	Type		male
	  Anim          male\Skel_ready2 1
	TEnd

	Type		huge
	  Anim          huge\Skel_ready 1 60
	TEnd

	Type		fem
	  Anim          fem\Skel_ready2 1
	TEnd
MEnd


#############################################################
## Idle 2 - Center Stance
#############################################################

Move Emote_Idle2
	Requires	EMOTE POSE IDLE TWO
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Idle2_Cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\centerstance_cycle 1
	TEnd
MEnd

Move Emote_Idle2_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NoSizeScale

	Type		male
	  Anim          male\centerstance_cycle 1
	TEnd
MEnd


#############################################################
## VANDALIZE
#############################################################

Move Emote_Vandalize
	Requires	EMOTE VANDALIZE
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> #<IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Vandalize_Cycle

	Type		male
	  Anim          male\kneel_pound 1
	  PlayFx        GENERIC\VANDAL_HAMMER.FX 0 CONSTANT
	  PlayFx        GENERIC\VANDAL_HAMMER2.FX 9
	TEnd
MEnd

Move Emote_Vandalize_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle

	Type		male
	  Anim          male\kneel_pound 1
	  PlayFx        GENERIC\VANDAL_HAMMER.FX 0 CONSTANT
	  PlayFx        GENERIC\VANDAL_HAMMER2.FX 9
	TEnd
MEnd

#############################################################
##  ReadBook
#############################################################

Move Emote_ReadBook01_Intro
	Requires	EMOTE READBOOK
	Priority        2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NoSizeScale FixArmReg
	NextMove	Emote_ReadBookTrans01
	Scale		0.5

	Type		male
	  Anim          male\ReadBook01 1 122
	  PlayFX	Emotes\ReadBook_Hero.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\ReadBook01 1 121
	  PlayFX	Emotes\ReadBook_Hero.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\ReadBook01 1 121
	  PlayFX	Emotes\ReadBook_Hero.fx 0 CONSTANT
	TEnd

	Type		v_male
	  Anim          male\ReadBook01 1 122
	  PlayFX	Emotes\ReadBook_Villain.fx 0 CONSTANT
	TEnd

	Type		v_fem
	  Anim          fem\ReadBook01 1 121
	  PlayFX	Emotes\ReadBook_Villain.fx 0 CONSTANT
	TEnd

	Type		v_huge
	  Anim          huge\ReadBook01 1 121
	  PlayFX	Emotes\ReadBook_Villain.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_ReadBook01
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale PickRandomly FixArmReg
	CycleMove       Emote_ReadBookTrans01
	CycleMove       Emote_ReadBookTrans01
	CycleMove       Emote_ReadBookTransAlt01
	Scale		0.5

	Type		male
	  Anim          male\ReadBook01 1 122
	  PlayFX	Emotes\ReadBook_Hero.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\ReadBook01 1 121
	  PlayFX	Emotes\ReadBook_Hero.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\ReadBook01 1 121
	  PlayFX	Emotes\ReadBook_Hero.fx 0 CONSTANT
	TEnd

	Type		v_male
	  Anim          male\ReadBook01 1 122
	  PlayFX	Emotes\ReadBook_Villain.fx 0 CONSTANT
	TEnd

	Type		v_fem
	  Anim          fem\ReadBook01 1 121
	  PlayFX	Emotes\ReadBook_Villain.fx 0 CONSTANT
	TEnd

	Type		v_huge
	  Anim          huge\ReadBook01 1 121
	  PlayFX	Emotes\ReadBook_Villain.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_ReadBookTrans01
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_ReadBook02

	Type		male
	  Anim          male\ReadBook01 122 140
	  PlayFX	Emotes\ReadBook_Hero.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\ReadBook01 121 139
	  PlayFX	Emotes\ReadBook_Hero.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\ReadBook01 121 139
	  PlayFX	Emotes\ReadBook_Hero.fx 0 CONSTANT
	TEnd

	Type		v_male
	  Anim          male\ReadBook01 122 140
	  PlayFX	Emotes\ReadBook_Villain.fx 0 CONSTANT
	TEnd

	Type		v_fem
	  Anim          fem\ReadBook01 121 139
	  PlayFX	Emotes\ReadBook_Villain.fx 0 CONSTANT
	TEnd

	Type		v_huge
	  Anim          huge\ReadBook01 121 139
	  PlayFX	Emotes\ReadBook_Villain.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_ReadBook02
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale PickRandomly FixArmReg
	CycleMove       Emote_ReadBookTrans02
	CycleMove       Emote_ReadBookTrans02
	CycleMove       Emote_ReadBookTransAlt02
	Scale		0.5

	Type		male
	  Anim          male\ReadBook01 139 260
	  PlayFX	Emotes\ReadBook_Hero.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\ReadBook01 139 259
	  PlayFX	Emotes\ReadBook_Hero.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\ReadBook01 139 259
	  PlayFX	Emotes\ReadBook_Hero.fx 0 CONSTANT
	TEnd

	Type		v_male
	  Anim          male\ReadBook01 139 260
	  PlayFX	Emotes\ReadBook_Villain.fx 0 CONSTANT
	TEnd

	Type		v_fem
	  Anim          fem\ReadBook01 139 259
	  PlayFX	Emotes\ReadBook_Villain.fx 0 CONSTANT
	TEnd

	Type		v_huge
	  Anim          huge\ReadBook01 139 259
	  PlayFX	Emotes\ReadBook_Villain.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_ReadBookTrans02
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_ReadBook01

	Type		male
	  Anim          male\ReadBook01 260 278
	  PlayFX	Emotes\ReadBook_Hero.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\ReadBook01 259 277
	  PlayFX	Emotes\ReadBook_Hero.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\ReadBook01 259 277
	  PlayFX	Emotes\ReadBook_Hero.fx 0 CONSTANT
	TEnd

	Type		v_male
	  Anim          male\ReadBook01 260 278
	  PlayFX	Emotes\ReadBook_Villain.fx 0 CONSTANT
	TEnd

	Type		v_fem
	  Anim          fem\ReadBook01 259 277
	  PlayFX	Emotes\ReadBook_Villain.fx 0 CONSTANT
	TEnd

	Type		v_huge
	  Anim          huge\ReadBook01 259 277
	  PlayFX	Emotes\ReadBook_Villain.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_ReadBookTransAlt01
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_ReadBook02

	Type		male
	  Anim          male\ReadBook01 279 339
	  PlayFX	Emotes\ReadBook_Hero.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\ReadBook01 278 338
	  PlayFX	Emotes\ReadBook_Hero.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\ReadBook01 278 338
	  PlayFX	Emotes\ReadBook_Hero.fx 0 CONSTANT
	TEnd

	Type		v_male
	  Anim          male\ReadBook01 279 339
	  PlayFX	Emotes\ReadBook_Villain.fx 0 CONSTANT
	TEnd

	Type		v_fem
	  Anim          fem\ReadBook01 278 338
	  PlayFX	Emotes\ReadBook_Villain.fx 0 CONSTANT
	TEnd

	Type		v_huge
	  Anim          huge\ReadBook01 278 338
	  PlayFX	Emotes\ReadBook_Villain.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_ReadBookTransAlt02
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NoSizeScale PickRandomly FixArmReg
	NextMove        Emote_Readbook01

	Type		male
	  Anim          male\ReadBook01 340 376
	  PlayFX	Emotes\ReadBook_Hero.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\ReadBook01 339 375
	  PlayFX	Emotes\ReadBook_Hero.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\ReadBook01 339 375
	  PlayFX	Emotes\ReadBook_Hero.fx 0 CONSTANT
	TEnd

	Type		v_male
	  Anim          male\ReadBook01 340 376
	  PlayFX	Emotes\ReadBook_Villain.fx 0 CONSTANT
	TEnd

	Type		v_fem
	  Anim          fem\ReadBook01 339 375
	  PlayFX	Emotes\ReadBook_Villain.fx 0 CONSTANT
	TEnd

	Type		v_huge
	  Anim          huge\ReadBook01 339 375
	  PlayFX	Emotes\ReadBook_Villain.fx 0 CONSTANT
	TEnd
MEnd


#############################################################
## Robot Dance
#############################################################

Move DanceRobot_Pre
	Requires	EMOTE DANCE LOW CYBORGPACK
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	DanceRobot_Cycle

	Type		male
	  Anim          male\RoboDance 1
	TEnd
MEnd

Move DanceRobot_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\RoboDance 1
	TEnd
MEnd


#############################################################
## Robot Power Down
#############################################################

Move Emote_RoboPowerDown
	Requires	EMOTE SLEEP DOWN CYBORGPACK
	Priority        4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_RoboPowerDown_PostCycle

	Type		male
	  Anim          male\RoboPowerDown 1
	TEnd
MEnd

Move Emote_RoboPowerDown_PostCycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE

	Type		male
	  Anim          male\RoboPowerDown 120
	TEnd
MEnd


#############################################################
## Robot Power Up
#############################################################

Move Emote_RoboPowerUp
	Requires	EMOTE SLEEP UP CYBORGPACK
	Priority        4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_RoboPowerUp_PostCycle

	Type		male
	  Anim          male\RoboPowerUp 1
	TEnd
MEnd

Move Emote_RoboPowerUp_PostCycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE
	Interpolate	15
	Scale		0.5

	Type		male
	  Anim		male\shield_Ready 1 31
	TEnd
MEnd

#############################################################
## Magic Cards
#############################################################

Move Emote_MagicCards_Pre
	Requires	EMOTE MAGIC CARDS TRICK
	Priority        4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_MagicCards
	Flags		NoSizeScale FixArmReg NOCOLLISION

	Type		male
	  Anim          male\Magic_Cards 1 10
	  ##PlayFX	Emotes\CardTrick\CardTrick_Male.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\Magic_Cards 1 10
	  ##PlayFX	Emotes\CardTrick\CardTrick_Huge.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\Magic_Cards 1 10
	  ##PlayFX	Emotes\CardTrick\CardTrick_Fem.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_MagicCards
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_MagicCards_Toss
	Flags		NoSizeScale FixArmReg NOCOLLISION

	Type		male
	  Anim          male\Magic_Cards 10 252
	  PlayFX	Emotes\CardTrick\CardTrick_Male.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\Magic_Cards 10 252
	  PlayFX	Emotes\CardTrick\CardTrick_Huge.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\Magic_Cards 10 252
	  PlayFX	Emotes\CardTrick\CardTrick_Fem.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_MagicCards_Toss
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_MagicCards_PostCycle
	Flags		NoSizeScale FixArmReg NOCOLLISION

	Type		male
	  Anim          male\Magic_Cards 252
	  PlayFX	Emotes\CardTrick\CardFountain.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\Magic_Cards 252
	  PlayFX	Emotes\CardTrick\CardFountain.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\Magic_Cards 252
	  PlayFX	Emotes\CardTrick\CardFountain.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_MagicCards_PostCycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle

	Type		male
	  Anim          male\Skel_ready2 1
	  PlayFX	Emotes\CardTrick\CardFountain.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          male\Skel_ready2 1
	  PlayFX	Emotes\CardTrick\CardFountain.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\Skel_ready2 1
	  PlayFX	Emotes\CardTrick\CardFountain.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
## Praetorian Salute
#############################################################

Move Emote_PratorianSalute
	Requires	EMOTE SALUTE THREE
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesLefthand> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_PratorianSalute_PostCycle
	Interpolate	5
	Flags		FIXARMREG

	Type		male
	  Anim          male\PraetorianSalute 1 65
	TEnd
MEnd

Move Emote_PratorianSalute_PostCycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG
	Scale		0.75

	Type		male
	  Anim          male\PraetorianSalute 65 125
	TEnd
MEnd

#############################################################

Move PratorianSalute_AtEase
	Requires	EMOTE SALUTE HIGH LOW
	Priority	5
	Interrupts	Emote_PratorianSalute Emote_PratorianSalute_PostCycle
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	PratorianSalute_AtEase_PostCycle
	Flags		FIXARMREG

	Type		male
	  Anim          male\PraetorianSalute 125
	TEnd
MEnd

Move PratorianSalute_AtEase_PostCycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\PraetorianSalute 175 175
	TEnd
MEnd

## BAT LOOKOUT ###########################################################

Move Emote_BatLookout_Pre
	Requires	EMOTE WEAPON BLUNT LOOKOUT
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesLefthand> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_BatLookout_Cycle
	Scale           0.50

	Type		male
	  Anim          male\bat_suspicious 1
	  PlayFx        WEAPONS\BASEBALL_BAT.FX 0 Constant
	TEnd

	Type		fem
	  Anim          fem\bat_suspicious 1
	  PlayFx        WEAPONS\BASEBALL_BAT.FX 0 Constant
	TEnd

	Type		huge
	  Anim          huge\bat_suspicious 1
	  PlayFx        WEAPONS\BASEBALL_BAT.FX 0 Constant
	TEnd
MEnd

Move Emote_BatLookout_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Scale           0.50

	Type		male
	  Anim          male\bat_suspicious 1
	  PlayFx        WEAPONS\BASEBALL_BAT.FX 0 Constant
	TEnd

	Type		fem
	  Anim          fem\bat_suspicious 1
	  PlayFx        WEAPONS\BASEBALL_BAT.FX 0 Constant
	TEnd

	Type		huge
	  Anim          huge\bat_suspicious 1
	  PlayFx        WEAPONS\BASEBALL_BAT.FX 0 Constant
	TEnd

MEnd

#############################################################

Move Emote_BatLookout_Alt2_Pre
	Requires	EMOTE WEAPON BLUNT LOOKOUT
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesLefthand> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_BatLookout_Left
	Scale           0.70

	Type		male
	  Anim          male\bathold_lookright 1
	  PlayFx        WEAPONS\BASEBALL_BAT.FX 0 Constant
	TEnd

	Type		fem
	  Anim          fem\bathold_lookright 1
	  PlayFx        WEAPONS\BASEBALL_BAT.FX 0 Constant
	TEnd

	Type		huge
	  Anim          huge\bathold_lookright 1
	  PlayFx        WEAPONS\BASEBALL_BAT.FX 0 Constant
	TEnd

MEnd

Move Emote_BatLookout_Right
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	CycleMove	Emote_BatLookout_Right
	CycleMove	Emote_BatLookout_Left
	Scale           0.70

	Type		male
	  Anim          male\bathold_lookright 1
	  PlayFx        WEAPONS\BASEBALL_BAT.FX 0 Constant
	TEnd

	Type		fem
	  Anim          fem\bathold_lookright 1
	  PlayFx        WEAPONS\BASEBALL_BAT.FX 0 Constant
	TEnd

	Type		huge
	  Anim          huge\bathold_lookright 1
	  PlayFx        WEAPONS\BASEBALL_BAT.FX 0 Constant
	TEnd

MEnd

Move Emote_BatLookout_Left
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	CycleMove	Emote_BatLookout_Right
	CycleMove	Emote_BatLookout_Left
	Scale           0.90

	Type		male
	  Anim          male\bathold_lookleft 1
	  PlayFx        WEAPONS\BASEBALL_BAT.FX 0 Constant
	TEnd

	Type		fem
	  Anim          fem\bathold_lookleft 1
	  PlayFx        WEAPONS\BASEBALL_BAT.FX 0 Constant
	TEnd

	Type		huge
	  Anim          huge\bathold_lookleft 1
	  PlayFx        WEAPONS\BASEBALL_BAT.FX 0 Constant
	TEnd

MEnd

## HOLD CRATE ###########################################################

Move Emote_HoldCrate_Pre
	Requires	EMOTE CARRY BOX LOITER
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesLefthand> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_HoldCrate_Cycle

	Type		male
	  Anim          male\crate_walk_post 45
	  PlayFx        GENERIC\crate.fx 0 Constant
	TEnd
MEnd

Move Emote_HoldCrate_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle

	Type		male
	  Anim          male\crate_walk_post 45
	  PlayFx        GENERIC\crate.fx 0 Constant
	TEnd
MEnd

## GREEDY/DICE OBSERVE ###########################################################

Move Emote_Greedy
	Requires	EMOTE OBSERVE LOW
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesLefthand> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove		Emote_Greedy_Cycle

	Type		male
	  Anim          male\greedy 1
	TEnd

	Type		fem
	  Anim          fem\greedy 1
	TEnd

	Type		huge
	  Anim          huge\greedy 1
	TEnd

MEnd

Move Emote_Greedy_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle

	Type		male
	  Anim          male\greedy 1
	TEnd

	Type		fem
	  Anim          fem\greedy 1
	TEnd

	Type		huge
	  Anim          huge\greedy 1
	TEnd

MEnd

## SHOVEL DIG ###########################################################

Move Emote_DigHole_Pre
	Requires	EMOTE WORK GROUNDSUMMON LOW MID
	Priority	5
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesLefthand> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NoSizeScale FixWeaponReg
	NextMove	Emote_DigHole_Cycle

	Type		male
	  Anim          male\dig_hole 1
	  PlayFx        GENERIC\Shovel.fx 0 CONSTANT
	  PlayFx        GENERIC\shovel_dirt.fx 37
	  PlayFx        GENERIC\shovel_dirt.fx 69
	  PlayFx        NPCFX\ShovelDirt_throw.fx
	  PlayFx        NPCFX\ShovelDirt.fx 37
	  PlayFx        NPCFX\ShovelDirt_throw.fx 50
	TEnd

	Type		fem
	  Anim          fem\dig_hole 1
	  PlayFx        GENERIC\Shovel.fx 0 CONSTANT
	  PlayFx        GENERIC\shovel_dirt.fx 37
	  PlayFx        GENERIC\shovel_dirt.fx 69
	  PlayFx        NPCFX\ShovelDirt_throw.fx
	  PlayFx        NPCFX\ShovelDirt.fx 37
	  PlayFx        NPCFX\ShovelDirt_throw.fx 50
	TEnd

	Type		huge
	  Anim          huge\dig_hole 1
	  PlayFx        GENERIC\Shovel.fx 0 CONSTANT
	  PlayFx        GENERIC\shovel_dirt.fx 37
	  PlayFx        GENERIC\shovel_dirt.fx 69
	  PlayFx        NPCFX\ShovelDirt_throw.fx
	  PlayFx        NPCFX\ShovelDirt.fx 37
	  PlayFx        NPCFX\ShovelDirt_throw.fx 50
	TEnd

MEnd

Move Emote_DigHole_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale FixWeaponReg

	Type		male
	  Anim          male\dig_hole 1
	  PlayFx        GENERIC\Shovel.fx 0 CONSTANT
	  PlayFx        GENERIC\shovel_dirt.fx 37
	  PlayFx        GENERIC\shovel_dirt.fx 69
	  PlayFx        NPCFX\ShovelDirt.fx 37
	  PlayFx        NPCFX\ShovelDirt_throw.fx 50
	TEnd

	Type		fem
	  Anim          fem\dig_hole 1
	  PlayFx        GENERIC\Shovel.fx 0 CONSTANT
	  PlayFx        GENERIC\shovel_dirt.fx 37
	  PlayFx        GENERIC\shovel_dirt.fx 69
	  PlayFx        NPCFX\ShovelDirt.fx 37
	  PlayFx        NPCFX\ShovelDirt_throw.fx 50
	TEnd

	Type		huge
	  Anim          huge\dig_hole 1
	  PlayFx        GENERIC\Shovel.fx 0 CONSTANT
	  PlayFx        GENERIC\shovel_dirt.fx 37
	  PlayFx        GENERIC\shovel_dirt.fx 69
	  PlayFx        NPCFX\ShovelDirt.fx 37
	  PlayFx        NPCFX\ShovelDirt_throw.fx 50
	TEnd

MEnd

## JACKHAMMER ###########################################################

Move Emote_Jackhammer
	Requires	EMOTE PROP BUSY WORK LOW
	Priority        5
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesLefthand> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NoSizeScale
	NextMove	Emote_Jackhammer_Cycle

	Type		male
	  Anim          male\JackHammer_Vertical 1
	  PlayFX	Generic\JackHammer.fx 0 CONSTANT
	  PlayFX	Generic\JackHammer_audio.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\JackHammer_Vertical 1
	  PlayFX	Generic\JackHammer.fx 0 CONSTANT
	  PlayFX	Generic\JackHammer_audio.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\JackHammer_Vertical 1
	  PlayFX	Generic\JackHammer.fx 0 CONSTANT
	  PlayFX	Generic\JackHammer_audio.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Jackhammer_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
	CycleMove       Emote_Jackhammer_Cycle
	CycleMove       Emote_Jackhammer_Cycle
	CycleMove       Emote_Jackhammer_Wipe

	Type		male
	  Anim          male\JackHammer_Vertical 1
	  PlayFX	Generic\JackHammer.fx 0 CONSTANT
	  PlayFX	Generic\JackHammer_audio.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\JackHammer_Vertical 1
	  PlayFX	Generic\JackHammer.fx 0 CONSTANT
	  PlayFX	Generic\JackHammer_audio.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\JackHammer_Vertical 1
	  PlayFX	Generic\JackHammer.fx 0 CONSTANT
	  PlayFX	Generic\JackHammer_audio.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Jackhammer_Wipe
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
	CycleMove       Emote_Jackhammer_Cycle

	Type		male
	  Anim          male\JackHammer_Wipe 1
	  PlayFX	Generic\JackHammer.fx 0 CONSTANT
	  PlayFX	Generic\JackHammer_audio2.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\JackHammer_Wipe 1
	  PlayFX	Generic\JackHammer.fx 0 CONSTANT
	  PlayFX	Generic\JackHammer_audio2_Female.fx 0
	TEnd

	Type		huge
	  Anim          huge\JackHammer_Wipe 1
	  PlayFX	Generic\JackHammer.fx 0 CONSTANT
	  PlayFX	Generic\JackHammer_audio2.fx 0 CONSTANT
	TEnd
MEnd

## SPRAYPAINT ###########################################################

Move Emote_Spraypaint_Pre
	Requires	EMOTE SPRAYCAN
	Priority        2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesLefthand> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Spraypaint_ShakeCan

	Type		male
	  Anim          male\spray_lookquick 1
	  PlayFX	Generic\spraycan.fx 0 Constant
	TEnd
MEnd

Move Emote_Spraypaint_LookQuick
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	CycleMove       Emote_Spraypaint_ShakeCan

	Type		male
	  Anim          male\spray_lookquick 1
	  PlayFX	Generic\spraycan.fx 0 Constant
	TEnd
MEnd

Move  Emote_Spraypaint_ShakeCan
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	CycleMove       Emote_Spraypaint_Start

	Type		male
	  Anim          male\spray_shakecan 1
	  PlayFX	Generic\spraycan.fx 0 Constant
	  PlayFX	Generic\spraycan_shake_SFX.fx 5
	TEnd
MEnd

Move  Emote_Spraypaint_Start
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	CycleMove       Emote_Spraypaint_Cycle
	CycleMove       Emote_Spraypaint_Cycle
	CycleMove       Emote_Spraypaint_LookQuick
	CycleMove       Emote_Spraypaint_ShakeCan

	Type		male
	  Anim          male\spray_cycle 1
	  PlayFX	Generic\spraycan.fx 0 Constant
	  PlayFX	Generic\spraycan_spray_SFX.fx 0	Constant
	TEnd
MEnd

Move Emote_Spraypaint_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	CycleMove       Emote_Spraypaint_Cycle
	CycleMove       Emote_Spraypaint_Cycle
	CycleMove       Emote_Spraypaint_LookQuick
	CycleMove       Emote_Spraypaint_ShakeCan

	Type		male
	  Anim          male\spray_cycle 15
	  PlayFX	Generic\spraycan.fx 0 Constant
	  PlayFX	Generic\spraycan_spray_SFX.fx 0	Constant
	TEnd
MEnd

##############################################################

Move Emote_Spraypaint_Kneel_Pre
	Requires	EMOTE SPRAYCAN
	Priority        2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesLefthand> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NoSizeScale
	NextMove	Emote_Spraypaint_Kneel

	Type		male
	  Anim          male\spray_lookquick 1
	  PlayFX	Generic\spraycan.fx 0	Constant
	TEnd
MEnd

Move Emote_Spraypaint_Kneel
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
	CycleMove       Emote_Spraypaint_Kneel_ShakeCan

	Type		male
	  Anim          male\praetorian_spraypaint_kneel 1 40
	  PlayFX	Generic\spraycan.fx 0 Constant
	TEnd
MEnd

Move Emote_Spraypaint_Kneel_ShakeCan
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
	CycleMove       Emote_Spraypaint_Kneel_Start

	Type		male
	  Anim          male\praetorian_spraypaint_kneel 40 137
	  PlayFX	Generic\spraycan.fx 0 Constant
	  PlayFX	Generic\spraycan_shake_SFX.fx 5
	TEnd
MEnd

Move Emote_Spraypaint_Kneel_Start
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
	CycleMove       Emote_Spraypaint_Kneel_Cycle
	CycleMove       Emote_Spraypaint_Kneel_Cycle
	CycleMove       Emote_Spraypaint_Kneel_ShakeCan

	Type		male
	  Anim          male\praetorian_spraypaint_kneel 137 218
	  PlayFX	Generic\spraycan.fx 0 Constant
  	  PlayFX	Generic\spraycan_spray_SFX.fx 0	Constant
	TEnd
MEnd

Move Emote_Spraypaint_Kneel_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
	CycleMove       Emote_Spraypaint_Kneel_Cycle
	CycleMove       Emote_Spraypaint_Kneel_Cycle
	CycleMove       Emote_Spraypaint_Kneel_ShakeCan

	Type		male
	  Anim          male\praetorian_spraypaint_kneel 152 218
	  PlayFX	Generic\spraycan.fx 0 Constant
	  PlayFX	Generic\spraycan_spray_SFX.fx 0	Constant
	TEnd
MEnd

## PAMPLETS ###########################################################

Move Emote_Pamphlet_Pre
	Requires	EMOTE PROP LOITER BECKON
	Priority        4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesLefthand> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NoSizeScale
	NextMove	Emote_Pamphlet_Cycle

	Type		male
	  Anim          male\Pamphlet 1
	  Playfx	GENERIC\FX_Pamphlet.fx 0 Constant
	TEnd

	Type		p_male
	  Anim          male\Pamphlet 1
	  Playfx	GENERIC\FX_Praetorian_Pamphlet.fx 0 Constant
	TEnd

MEnd

Move Emote_Pamphlet_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\Pamphlet 1
	  Playfx	GENERIC\FX_Pamphlet.fx 0 Constant
	TEnd

	Type		p_male
	  Anim          male\Pamphlet 1
	  Playfx	GENERIC\FX_Praetorian_Pamphlet.fx 0 Constant
	TEnd
MEnd

#############################################################

Move Emote_Vendor_Pre
	Requires	EMOTE NOCARTVENDOR
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesLefthand> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesDualhand>
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale
	NextMove	Emote_Vendor_Point

	Type		male
	  Anim          male\VendorNew_YellRight 1
	TEnd
MEnd

Move Emote_Vendor_YellRight
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
	CycleMove       Emote_Vendor_Point
	CycleMove       Emote_Vendor_Sweep

	Type		male
	  Anim          male\VendorNew_YellRight 1
	TEnd
MEnd

Move Emote_Vendor_Point
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
	CycleMove       Emote_Vendor_YellRight
	CycleMove       Emote_Vendor_Sweep

	Type		male
	  Anim          male\VendorNew_Point 1
	TEnd
MEnd

Move Emote_Vendor_Sweep
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
	CycleMove       Emote_Vendor_Point
	CycleMove       Emote_Vendor_YellRight

	Type		male
	  Anim          male\VendorNew_Sweep 1
	TEnd
MEnd

## WARM HANDS ###########################################################

Move Emote_WarmHands_Pre
	Requires	EMOTE HAND LOITER
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesLefthand> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_WarmHands_Shuffle

	Type		male
	  Anim          male\loiter_warmhands 1
	TEnd

	Type		fem
	  Anim          fem\loiter_warmhands 1
	TEnd

	Type		huge
	  Anim          huge\loiter_warmhands 1
	TEnd

	Type		5cw
	  Anim          5cw\skel_monster_cycle 1 63
	TEnd
MEnd

Move Emote_WarmHands_Rub
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	CycleMove       Emote_WarmHands_Shuffle

	Type		male
	  Anim          male\loiter_warmhands 1
	TEnd

	Type		fem
	  Anim          fem\loiter_warmhands 1
	TEnd

	Type		huge
	  Anim          huge\loiter_warmhands 1
	TEnd

	Type		5cw
	  Anim          5cw\skel_monster_cycle 1 63
	TEnd

MEnd

Move Emote_WarmHands_Shuffle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	CycleMove       Emote_WarmHands_Blow

	Type		male
	  Anim          male\loiter_warmdance 1
	TEnd

	Type		fem
	  Anim          fem\loiter_warmhands 147 304
	TEnd

	Type		huge
	  Anim          huge\loiter_warmhands 147 304
	TEnd

	Type		5cw
	  Anim          5cw\skel_monster_cycle 1 63
	TEnd

MEnd

Move Emote_WarmHands_Blow
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	CycleMove       Emote_WarmHands_Rub

	Type		male
	  Anim          male\loiter_blowhands 1
	TEnd

	Type		fem
	  Anim          fem\loiter_warmhands 304
	TEnd

	Type		huge
	  Anim          huge\loiter_warmhands 304
	TEnd

	Type		5cw
	  Anim          5cw\skel_monster_cycle 1 63
	TEnd

MEnd

#############################################################
## Enriche Drink
#############################################################

Move Emote_Praetorian_Enriche_Drink
	Requires	EMOTE DRINK ALTERNATE
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Praetorian_Enriche_Drink_cycle_A
	Flags		FIXARMREG

	Type		male
	  Anim          male\emote_drink_enriche_sip 1
	  PlayFx        WORLD\Objects\PraetorianBottle\PraetorianBottle.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_drink_enriche_sip 1
	  PlayFx        WORLD\Objects\PraetorianBottle\PraetorianBottle.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_drink_enriche_sip 1
	  PlayFx        WORLD\Objects\PraetorianBottle\PraetorianBottle.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Praetorian_Enriche_Drink_cycle_A
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Praetorian_Enriche_Drink_cycle_A
	CycleMove	Emote_Praetorian_Enriche_Drink_cycle_B
	CycleMove	Emote_Praetorian_Enriche_Drink_cycle_C
	CycleMove	Emote_Praetorian_Enriche_Drink_cycle_A
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\emote_drink_enriche_idle 1
	  PlayFx        WORLD\Objects\PraetorianBottle\PraetorianBottle.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_drink_enriche_idle 1
	  PlayFx        WORLD\Objects\PraetorianBottle\PraetorianBottle.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_drink_enriche_idle 1
	  PlayFx        WORLD\Objects\PraetorianBottle\PraetorianBottle.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Praetorian_Enriche_Drink_cycle_B
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Praetorian_Enriche_Drink_cycle_A
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\emote_drink_enriche_sip 1
	  PlayFx        WORLD\Objects\PraetorianBottle\PraetorianBottle.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_drink_enriche_sip 1
	  PlayFx        WORLD\Objects\PraetorianBottle\PraetorianBottle.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_drink_enriche_sip 1
	  PlayFx        WORLD\Objects\PraetorianBottle\PraetorianBottle.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Praetorian_Enriche_Drink_cycle_C
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Praetorian_Enriche_Drink_cycle_A
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\emote_drink_enriche_bigsip 1
	  PlayFx        WORLD\Objects\PraetorianBottle\PraetorianBottle.fx 0 CONSTANT
	TEnd


	Type		fem
	  Anim          fem\emote_drink_enriche_bigsip 1
	  PlayFx        WORLD\Objects\PraetorianBottle\PraetorianBottle.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\emote_drink_enriche_bigsip 1
	  PlayFx        WORLD\Objects\PraetorianBottle\PraetorianBottle.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
## Protest Loyalist
#############################################################

Move Emote_Protest_Loyalist
	Requires	EMOTE LOYALIST
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Protest_Loyalist_cycle
	Interpolate	8
	Flags		NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\sign_chant 1
	  PlayFx        GENERIC\ProtestSign_Loyalist.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          male\sign_onearm_chant 1
	  PlayFx        GENERIC\ProtestSign_Loyalist.fx 0 CONSTANT
	TEnd

	Type		5cw
	  Anim          male\sign_onearm_chant 1
	  PlayFx        GENERIC\ProtestSign_Loyalist.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Protest_Loyalist_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\sign_chant 1
	  PlayFx        GENERIC\ProtestSign_Loyalist.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          male\sign_onearm_chant 1
	  PlayFx        GENERIC\ProtestSign_Loyalist.fx 0 CONSTANT
	TEnd

	Type		5cw
	  Anim          male\sign_onearm_chant 1
	  PlayFx        GENERIC\ProtestSign_Loyalist.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
## Protest Resistance
#############################################################

Move Emote_Protest_Resistance
	Requires	EMOTE RESISTANCE
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Protest_Resistance_cycle
	Interpolate	8
	Flags		NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\sign_chant 1
	  PlayFx        GENERIC\ProtestSign_Resistance.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          male\sign_onearm_chant 1
	  PlayFx        GENERIC\ProtestSign_Resistance.fx 0 CONSTANT
	TEnd

	Type		5cw
	  Anim          male\sign_onearm_chant 1
	  PlayFx        GENERIC\ProtestSign_Resistance.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Protest_Resistance_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\sign_chant 1
	  PlayFx        GENERIC\ProtestSign_Resistance.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          male\sign_onearm_chant 1
	  PlayFx        GENERIC\ProtestSign_Resistance.fx 0 CONSTANT
	TEnd

	Type		5cw
	  Anim          male\sign_onearm_chant 1
	  PlayFx        GENERIC\ProtestSign_Resistance.fx 0 CONSTANT
	TEnd
MEnd

###############################################

Move Emote_Facepalm
	Requires	EMOTE FACEPALM
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Facepalm_Post

	Type		male
	  Anim          male\emote_facepalm 1 20
	  PlayFX	NPCFX\FacePalm_SFX.fx 12
	TEnd

	Type		huge
	  Anim          huge\emote_facepalm 1 20
	  PlayFX	NPCFX\FacePalm_SFX.fx 12
	TEnd

	Type		fem
	  Anim          fem\emote_facepalm 1 20
	  PlayFX	NPCFX\FacePalm_SFX.fx 12
	TEnd
MEnd

Move Emote_Facepalm_Post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Ready

	Type		male
	  Anim          male\emote_facepalm 21
	TEnd

	Type		huge
	  Anim          huge\emote_facepalm 21
	TEnd

	Type		fem
	  Anim          fem\emote_facepalm 21
	TEnd
MEnd
###############################################

Move Emote_Facepalm_Shield
	Requires	EMOTE SHIELD FACEPALM
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Facepalm_Shield_Post

	Type		male
	  Anim          male\emote_facepalm_shield 1 20
	TEnd

	Type		huge
	  Anim          huge\emote_facepalm_shield 1 20
	TEnd

	Type		fem
	  Anim          fem\emote_facepalm_shield 1 20
	TEnd
MEnd

Move Emote_Facepalm_Shield_Post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	shield_combatready

	Type		male
	  Anim          male\emote_facepalm_shield 21
	TEnd

	Type		huge
	  Anim          huge\emote_facepalm_shield 21
	TEnd

	Type		fem
	  Anim          fem\emote_facepalm_shield 21
	TEnd
MEnd
###############################################

Move Emote_Double_Facepalm
	Requires	EMOTE FACEPALM ALTERNATE
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Double_Facepalm_Post

	Type		male
	  Anim          male\emote_double_facepalm 1 20
	  PlayFX	NPCFX\FacePalmDouble_SFX.fx 12
	TEnd

	Type		huge
	  Anim          huge\emote_double_facepalm 1 20
	  PlayFX	NPCFX\FacePalmDouble_SFX.fx 12
	TEnd

	Type		fem
	  Anim          fem\emote_double_facepalm 1 20
	  PlayFX	NPCFX\FacePalmDouble_SFX.fx 12
	TEnd
MEnd

Move Emote_Double_Facepalm_Post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Ready

	Type		male
	  Anim          male\emote_double_facepalm 21
	TEnd

	Type		huge
	  Anim          huge\emote_double_facepalm 21
	TEnd

	Type		fem
	  Anim          fem\emote_double_facepalm 21
	TEnd
MEnd
#######################################
### Vomit  #####
#######################################

Move Emote_Vomit_One
	Requires	EMOTE YELL BARFSTUN ONE
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Vomit_Post_One

	Type		male
	  Anim          male\drill_yell 1 100
	  PlayFX	Generic\Vomit.fx 40
	TEnd
MEnd

Move Emote_Vomit_Post_One
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Vomit_To_Ready_One

	Type		male
	  Anim          male\drill_yell 100 225
	  PlayFX	Generic\Vomit.fx 38
	TEnd
MEnd

Move Emote_Vomit_To_Ready_One
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Ready

	Type		male
	  Anim          male\emote_vomit_to_ready 1
	TEnd
MEnd
###############################################
Move Emote_Vomit_Two
	Requires	EMOTE YELL BARFSTUN ONE
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Vomit_Post_Two
	Flags		FixArmReg NoSizeScale
	Interpolate	15

	Type		male
	  Anim          male\barf 1 60
	  PlayFX	Generic\Vomit.fx 25
	TEnd

	Type		fem
	  Anim          male\barf 1 60
	  PlayFX	Generic\Vomit_female.fx 25
	TEnd

	Type		huge
	  Anim          huge\quickbarf 1 60
	  PlayFX	Generic\Vomit.fx 25
	TEnd
MEnd

Move Emote_Vomit_Post_Two
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Vomit_To_Ready_Two
	Flags		FixArmReg NoSizeScale

	Type		male
	  Anim          male\barf 60 139
	TEnd

	Type		huge
	  Anim          huge\quickbarf 60
	TEnd
MEnd

Move Emote_Vomit_To_Ready_Two
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle FixArmReg NoSizeScale
	NextMove        postReady_back

	Type		male
	  Anim          male\barf_cycle 1 50
	TEnd

	Type		huge
	  Anim          huge\barf_cycle 1 50
	TEnd
MEnd

###############################################
Move Emote_Vomit_Three
	Requires	EMOTE YELL BARFSTUN ONE
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Vomit_Post_Three
	Flags		FixArmReg NoSizeScale
	Interpolate	0

	Type		male
	  Anim          male\quickbarf 1 60
	  PlayFX	Generic\Vomit.fx 25
	TEnd

	Type		fem
	  Anim          male\quickbarf 1 60
	  PlayFX	Generic\Vomit_female.fx 25
	TEnd

	Type		huge
	  Anim          huge\quickbarf 1 60
	  PlayFX	Generic\Vomit.fx 25
	TEnd
MEnd

Move Emote_Vomit_Post_Three
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Vomit_To_Ready_Two
	Flags		FixArmReg NoSizeScale

	Type		male
	  Anim          male\quickbarf 60
	TEnd

	Type		huge
	  Anim          huge\quickbarf 60
	TEnd
MEnd

#############################################################
##	Emote_Ghoul Flex Emote
#############################################################

Move Emote_Ghoul_Flex_Cycle_Pre
	Requires	EMOTE YELL FLEX ONE
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Ghoul_Flex_Cycle_A

	Type		male
	  Anim          male\ghouls_ready_A 1
	#  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 1
	#  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 20
	TEnd

MEnd

Move Emote_Ghoul_Flex_Cycle_A
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	CycleMove	Emote_Ghoul_Flex_Cycle_A
	CycleMove	Emote_Ghoul_Flex_Cycle_A
	CycleMove	Emote_Ghoul_Flex_Cycle_break_A

	Type		male
	  Anim          male\ghouls_ready_A 1
	#  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 1
	#  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 20
	TEnd

MEnd

Move Emote_Ghoul_Flex_Cycle_break_A
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	CycleMove	Emote_Ghoul_Flex_Cycle_B

	Type		male
	  Anim          male\ghouls_idle_break_A 1
	#  PlayFX	ENEMYFX\Ghouls\Ghoul_BreakA_SFX.fx
	#  PlayFX	ENEMYFX\Ghouls\Ghoul_Run_SFX.fx 50
	#  PlayFX	ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 105
	TEnd

MEnd

Move Emote_Ghoul_Flex_Cycle_B
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	CycleMove	Emote_Ghoul_Flex_Cycle_B
	CycleMove	Emote_Ghoul_Flex_Cycle_B
	CycleMove	Emote_Ghoul_Flex_Cycle_break_B
	CycleMove	Emote_Ghoul_Flex_Cycle_break_C

	Type		male
	  Anim          male\ghouls_ready_B 1
#	  PlayFX	ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 1
	#  PlayFX	ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 20
	TEnd

MEnd

Move Emote_Ghoul_Flex_Cycle_break_B
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	CycleMove	Emote_Ghoul_Flex_Cycle_B

	Type		male
	  Anim          male\ghouls_idle_break_B 1
	#  PlayFX	ENEMYFX\Ghouls\Ghoul_BreakB_SFX.fx
	#  PlayFX	ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 50
	TEnd

MEnd

Move Emote_Ghoul_Flex_Cycle_break_C
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	CycleMove	Emote_Ghoul_Flex_Cycle_A

	Type		male
	  Anim          male\ghouls_idle_break_C 1
	#  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 1
	#  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 20
	#  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 50
	#  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 70
	#  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 100
	#  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 120
	TEnd

MEnd

###################
###### Emote Temper Tantrum   ######
###################

Move Emote_Ghoul_Temper_Tantrum_Pre
	Requires	EMOTE SCREAM FLEX TWO
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Emote_Ghoul_Temper_Tantrum
	Flags		NoSizeScale

	Type		male
	  Anim          male\ghouls_death_frenzy 1 30
  	#  PlayFX	ENEMYFX\Ghouls\Ghoul_Death_SFX.fx
	TEnd

MEnd

Move Emote_Ghoul_Temper_Tantrum
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Ghoul_Temper_Tantrum_Post
	Flags		NoSizeScale

	Type		male
	  Anim          male\ghouls_death_frenzy 30 150
	TEnd

MEnd

Move Emote_Ghoul_Temper_Tantrum_Post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Ghoul_Temper_Tantrum_getup_back
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_ghoul_tantrum 154 163
	TEnd

MEnd

Move Emote_Ghoul_Temper_Tantrum_getup_back
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Emote_Ghoul_Temper_Tantrum_back2
	Flags		NoSizeScale

	Type		male
	  Anim          male\Quick_getup 11 25
	#  PlayFx        FEET\JUMP1.FX 3
	TEnd

	Type		huge
	  Anim          huge\Quick_getup 11 25
	#  PlayFx        FEET\HUGEJUMP2.FX 3
	TEnd

	Type		fem
	  Anim          fem\Quick_getup 11 25
	#  PlayFx        FEET\FEMJUMP1.FX 3
	TEnd
MEnd

Move Emote_Ghoul_Temper_Tantrum_back2
	NextMove        Ready
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NoSizeScale

	Type		male
	  Anim          male\comb_postjump 2
	  PlayFx        FEET\LAND1.FX  3
	TEnd

	Type		huge
	  Anim          huge\combat_postjump 2
	  PlayFx	FEET\HUGELAND1.FX:LAND	3
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT	12
	TEnd

	Type		fem
	  Anim          fem\combat_postjump 2
	  PlayFx        FEET\LAND1.FX  3
	TEnd
MEnd

##############################
###   Reward Emotes
##############################

Move Emote_Ultimate_Power
	Requires	EMOTE YELL REWARD
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesShield> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	NextMove	Emote_Ultimate_Power_Post
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_ultimate_power 1 63
	  PlayFX	Emotes\CostumeChanges\Lightning\LightningBlast.fx 15
	TEnd

MEnd

Move Emote_Ultimate_Power_Post
	NextMove        Ready
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_ultimate_power 64
	TEnd

MEnd
############################################################

Move Emote_Catch_Breath
	Requires	EMOTE POSE REWARD
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesShield> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	NextMove	Emote_Catch_Breath_Post
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_catch_breath 1 30
	TEnd

	Type		huge
	  Anim          huge\emote_catch_breath 1 30
	TEnd

	Type		fem
	  Anim          fem\emote_catch_breath 1 30
	TEnd

MEnd

Move Emote_Catch_Breath_Post
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NoSizeScale Cycle
	NextMove        Ready
	CycleMove	Emote_Catch_Breath_Post

	Type		male
	  Anim          male\emote_catch_breath 30
	TEnd

	Type		huge
	  Anim          huge\emote_catch_breath 30
	TEnd

	Type		fem
	  Anim          fem\emote_catch_breath 30
	TEnd

MEnd

############################################################

Move Emote_Collapse_Pre
	Requires	EMOTE REWARD ONE
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesShield> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Collapse
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_collapse 1 30
	  PlayFX	Feet\MaleLeft.fx 15
	TEnd

	Type		huge
	  Anim          huge\emote_collapse 1 30
	  PlayFX	Feet\HugeLeft.fx 15
	TEnd

	Type		fem
	  Anim          fem\emote_collapse 1 30
	  PlayFX	Feet\FemLeft.fx 15
	TEnd
MEnd

Move Emote_Collapse
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NoSizeScale
	NextMove        Emote_Collapse_Post

	Type		male
	  Anim          male\emote_collapse 31 99
	  PlayFX	Feet\MaleRight.fx 1
	  PlayFX	Feet\MaleLeft.fx 15
	  PlayFX	Emotes\Reward\Collapse.fx 60
	TEnd

	Type		huge
	  Anim          huge\emote_collapse 31 99
	  PlayFX	Feet\HugeRight.fx 1
	  PlayFX	Feet\HugeLeft.fx 15
	  PlayFX	Emotes\Reward\Collapse.fx 60

	TEnd

	Type		fem
	  Anim          fem\emote_collapse 31 99
	  PlayFX	Feet\FemRight.fx 1
	  PlayFX	Feet\FemLeft.fx 15
	  PlayFX	Emotes\Reward\Collapse.fx 60
	TEnd

MEnd

Move Emote_Collapse_Post
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NoSizeScale Cycle
	NextMove        Ready
	CycleMove	Emote_Collapse_Post

	Type		male
	  Anim          male\emote_collapse 100
	TEnd

	Type		huge
	  Anim          huge\emote_collapse 100
	TEnd

	Type		fem
	  Anim          fem\emote_collapse 100
	TEnd
MEnd

############################################################

Move Emote_Swoon_Pre
	Requires	EMOTE AFK ALTERNATE REWARD
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Swoon
	Flags		NoSizeScale
	SticksonChild	EMOTE AFK ALTERNATE REWARD

	Type		male
	  Anim          male\emote_faint 1 30
	  PlayFX	Feet\MaleLeft.fx 25
	TEnd

	Type		fem
	  Anim          fem\emote_faint 1 30
	  PlayFX	Feet\FemLeft.fx 25
	TEnd

MEnd

Move Emote_Swoon
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NoSizeScale
	NextMove        Emote_Swoon_Post

	Type		male
	  Anim          male\emote_faint 30 75
	  PlayFX	Feet\MaleRight.fx 5
	  PlayFX	Emotes\Reward\Swoon.fx 35
	TEnd

	Type		fem
	  Anim          fem\emote_faint 30 75
	  PlayFX	Feet\FemRight.fx 5
	  PlayFX	Emotes\Reward\Swoon.fx 35
	TEnd

MEnd

Move Emote_Swoon_Post
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NoSizeScale Cycle
	NextMove        Ready
	CycleMove	Emote_Swoon_Post

	Type		male
	  Anim          male\emote_faint 75
	TEnd

	Type		fem
	  Anim          fem\emote_faint 75
	TEnd

MEnd

#############################################################
## Hero Loyalty Emote
#############################################################

Move Emote_Hero_Loyalty
	Requires	EMOTE LOYALTY ONE
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Hero_Loyalty_cycle
	Flags		FIXARMREG

	Type		male
	  Anim          male\yes 1 90
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd

	Type		huge
	  Anim          huge\yes_hips 1 90
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd

	Type		fem
	  Anim          fem\yes_hips 1 69
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd
MEnd

Move Emote_Hero_Loyalty_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG
	CycleMove	Emote_Hero_Loyalty_Nod
	CycleMove	Emote_Hero_Loyalty_cycle
	CycleMove	Emote_Hero_Loyalty_Wave
	CycleMove	Emote_Hero_Loyalty_cycle


	Type		male
	  Anim          male\yes 91 95
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd

	Type		huge
	  Anim          huge\yes_hips 91 95
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd

	Type		fem
	  Anim          fem\yes_hips 70 75
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd
MEnd

Move Emote_Hero_Loyalty_Nod
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG
	CycleMove	Emote_Hero_Loyalty_Nod
	CycleMove	Emote_Hero_Loyalty_Nod
	CycleMove	Emote_Hero_Loyalty_Wave
	CycleMove	Emote_Hero_Loyalty_Wave_Right


	Type		male
	  Anim          male\yes_nod 1
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd

	Type		huge
	  Anim          male\yes_nod 1
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd

	Type		fem
	  Anim          male\yes_nod 1
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd
MEnd

Move Emote_Hero_Loyalty_Wave
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG
	CycleMove	Emote_Hero_Loyalty_Nod
	CycleMove	Emote_Hero_Loyalty_Wave
	CycleMove	Emote_Hero_Loyalty_Wave_Left
	CycleMove	Emote_Hero_Loyalty_Nod


	Type		male
	  Anim          male\yes_wave 1
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd

	Type		huge
	  Anim          male\yes_wave 1
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd

	Type		fem
	  Anim          male\yes_wave 1
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd
MEnd

Move Emote_Hero_Loyalty_Wave_Right
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG
	CycleMove	Emote_Hero_Loyalty_Wave_Left
	CycleMove	Emote_Hero_Loyalty_Wave
	CycleMove	Emote_Hero_Loyalty_Nod
	CycleMove	Emote_Hero_Loyalty_Nod


	Type		male
	  Anim          male\yes_wave_right_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd

	Type		huge
	  Anim          male\yes_wave_right_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd

	Type		fem
	  Anim          male\yes_wave_right_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd
MEnd


Move Emote_Hero_Loyalty_Wave_Left
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG
	CycleMove	Emote_Hero_Loyalty_Wave_Left
	CycleMove	Emote_Hero_Loyalty_Nod
	CycleMove	Emote_Hero_Loyalty_Wave_Right
	CycleMove	Emote_Hero_Loyalty_Nod


	Type		male
	  Anim          male\yes_wave_left_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd

	Type		huge
	  Anim          male\yes_wave_left_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd

	Type		fem
	  Anim          male\yes_wave_left_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd
MEnd

#############################################################
## Villain Loyalty Emote
#############################################################

Move Emote_Villain_Loyalty
	Requires	EMOTE LOYALTY TWO
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Villain_Loyalty_cycle
	Flags		FIXARMREG

	Type		male
	  Anim          male\yes 1 90
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd

	Type		huge
	  Anim          huge\yes_hips 1 90
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd

	Type		fem
	  Anim          fem\yes_hips 1 69
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd
MEnd

Move Emote_Villain_Loyalty_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG
	CycleMove	Emote_Villain_Loyalty_Nod
	CycleMove	Emote_Villain_Loyalty_cycle
	CycleMove	Emote_Villain_Loyalty_Wave
	CycleMove	Emote_Villain_Loyalty_cycle


	Type		male
	  Anim          male\yes 91 95
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd

	Type		huge
	  Anim          huge\yes_hips 91 95
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd

	Type		fem
	  Anim          fem\yes_hips 70 75
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd
MEnd

Move Emote_Villain_Loyalty_Nod
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG
	CycleMove	Emote_Villain_Loyalty_Nod
	CycleMove	Emote_Villain_Loyalty_Nod
	CycleMove	Emote_Villain_Loyalty_Wave
	CycleMove	Emote_Villain_Loyalty_Wave_Right


	Type		male
	  Anim          male\yes_nod 1
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd

	Type		huge
	  Anim          male\yes_nod 1
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd

	Type		fem
	  Anim          male\yes_nod 1
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd
MEnd

Move Emote_Villain_Loyalty_Wave
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG
	CycleMove	Emote_Villain_Loyalty_Nod
	CycleMove	Emote_Villain_Loyalty_Nod
	CycleMove	Emote_Villain_Loyalty_Wave_Left
	CycleMove	Emote_Villain_Loyalty_Wave


	Type		male
	  Anim          male\yes_wave 1
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd

	Type		huge
	  Anim          male\yes_wave 1
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd

	Type		fem
	  Anim          male\yes_wave 1
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd
MEnd

Move Emote_Villain_Loyalty_Wave_Right
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG
	CycleMove	Emote_Villain_Loyalty_Wave_Left
	CycleMove	Emote_Villain_Loyalty_Wave
	CycleMove	Emote_Villain_Loyalty_Nod
	CycleMove	Emote_Villain_Loyalty_Nod


	Type		male
	  Anim          male\yes_wave_right_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd

	Type		huge
	  Anim          male\yes_wave_right_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd

	Type		fem
	  Anim          male\yes_wave_right_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd
MEnd


Move Emote_Villain_Loyalty_Wave_Left
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG
	CycleMove	Emote_Villain_Loyalty_Wave_Left
	CycleMove	Emote_Villain_Loyalty_Nod
	CycleMove	Emote_Villain_Loyalty_Wave_Right
	CycleMove	Emote_Villain_Loyalty_Nod


	Type		male
	  Anim          male\yes_wave_left_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd

	Type		huge
	  Anim          male\yes_wave_left_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd

	Type		fem
	  Anim          male\yes_wave_left_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd
MEnd

#############################################################
## Vigilante Loyalty Emote
#############################################################

Move Emote_Vigilante_Loyalty
	Requires	EMOTE LOYALTY FOUR
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Vigilante_Loyalty_cycle
	Flags		FIXARMREG

	Type		male
	  Anim          male\yes 1 90
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd

	Type		huge
	  Anim          huge\yes_hips 1 90
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd

	Type		fem
	  Anim          fem\yes_hips 1 69
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd
MEnd

Move Emote_Vigilante_Loyalty_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG
	CycleMove	Emote_Vigilante_Loyalty_Nod
	CycleMove	Emote_Vigilante_Loyalty_cycle
	CycleMove	Emote_Vigilante_Loyalty_Wave
	CycleMove	Emote_Vigilante_Loyalty_cycle


	Type		male
	  Anim          male\yes 91 95
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd

	Type		huge
	  Anim          huge\yes_hips 91 95
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd

	Type		fem
	  Anim          fem\yes_hips 70 75
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd
MEnd

Move Emote_Vigilante_Loyalty_Nod
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG
	CycleMove	Emote_Vigilante_Loyalty_Nod
	CycleMove	Emote_Vigilante_Loyalty_Nod
	CycleMove	Emote_Vigilante_Loyalty_Wave
	CycleMove	Emote_Vigilante_Loyalty_Wave_Right


	Type		male
	  Anim          male\yes_nod 1
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd

	Type		huge
	  Anim          male\yes_nod 1
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd

	Type		fem
	  Anim          male\yes_nod 1
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd
MEnd

Move Emote_Vigilante_Loyalty_Wave
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG
	CycleMove	Emote_Vigilante_Loyalty_Nod
	CycleMove	Emote_Vigilante_Loyalty_Nod
	CycleMove	Emote_Vigilante_Loyalty_Wave_Left
	CycleMove	Emote_Vigilante_Loyalty_Wave


	Type		male
	  Anim          male\yes_wave 1
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd

	Type		huge
	  Anim          male\yes_wave 1
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd

	Type		fem
	  Anim          male\yes_wave 1
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd
MEnd

Move Emote_Vigilante_Loyalty_Wave_Right
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG
	CycleMove	Emote_Vigilante_Loyalty_Wave_Left
	CycleMove	Emote_Vigilante_Loyalty_Wave
	CycleMove	Emote_Vigilante_Loyalty_Nod
	CycleMove	Emote_Vigilante_Loyalty_Nod


	Type		male
	  Anim          male\yes_wave_right_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd

	Type		huge
	  Anim          male\yes_wave_right_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd

	Type		fem
	  Anim          male\yes_wave_right_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd
MEnd


Move Emote_Vigilante_Loyalty_Wave_Left
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG
	CycleMove	Emote_Vigilante_Loyalty_Wave_Left
	CycleMove	Emote_Vigilante_Loyalty_Nod
	CycleMove	Emote_Vigilante_Loyalty_Wave_Right
	CycleMove	Emote_Vigilante_Loyalty_Nod


	Type		male
	  Anim          male\yes_wave_left_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd

	Type		huge
	  Anim          male\yes_wave_left_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd

	Type		fem
	  Anim          male\yes_wave_left_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd
MEnd

#############################################################
## Rogue Loyalty Emote
#############################################################

Move Emote_Rogue_Loyalty
	Requires	EMOTE LOYALTY THREE
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Rogue_Loyalty_cycle
	Flags		FIXARMREG

	Type		male
	  Anim          male\yes 1 90
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd

	Type		huge
	  Anim          huge\yes_hips 1 90
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd

	Type		fem
	  Anim          fem\yes_hips 1 69
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd
MEnd

Move Emote_Rogue_Loyalty_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG
	CycleMove	Emote_Rogue_Loyalty_Nod
	CycleMove	Emote_Rogue_Loyalty_cycle
	CycleMove	Emote_Rogue_Loyalty_Wave
	CycleMove	Emote_Rogue_Loyalty_cycle


	Type		male
	  Anim          male\yes 91 95
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd

	Type		huge
	  Anim          huge\yes_hips 91 95
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd

	Type		fem
	  Anim          fem\yes_hips 70 75
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd
MEnd

Move Emote_Rogue_Loyalty_Nod
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG
	CycleMove	Emote_Rogue_Loyalty_Nod
	CycleMove	Emote_Rogue_Loyalty_Nod
	CycleMove	Emote_Rogue_Loyalty_Wave
	CycleMove	Emote_Rogue_Loyalty_Wave_Right


	Type		male
	  Anim          male\yes_nod 1
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd

	Type		huge
	  Anim          male\yes_nod 1
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd

	Type		fem
	  Anim          male\yes_nod 1
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd
MEnd

Move Emote_Rogue_Loyalty_Wave
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG
	CycleMove	Emote_Rogue_Loyalty_Nod
	CycleMove	Emote_Rogue_Loyalty_Nod
	CycleMove	Emote_Rogue_Loyalty_Wave_Left
	CycleMove	Emote_Rogue_Loyalty_Wave


	Type		male
	  Anim          male\yes_wave 1
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd

	Type		huge
	  Anim          male\yes_wave 1
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd

	Type		fem
	  Anim          male\yes_wave 1
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd
MEnd

Move Emote_Rogue_Loyalty_Wave_Right
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG
	CycleMove	Emote_Rogue_Loyalty_Wave_Left
	CycleMove	Emote_Rogue_Loyalty_Wave
	CycleMove	Emote_Rogue_Loyalty_Nod
	CycleMove	Emote_Rogue_Loyalty_Nod


	Type		male
	  Anim          male\yes_wave_right_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd

	Type		huge
	  Anim          male\yes_wave_right_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd

	Type		fem
	  Anim          male\yes_wave_right_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd
MEnd


Move Emote_Rogue_Loyalty_Wave_Left
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG
	CycleMove	Emote_Rogue_Loyalty_Wave_Left
	CycleMove	Emote_Rogue_Loyalty_Nod
	CycleMove	Emote_Rogue_Loyalty_Wave_Right
	CycleMove	Emote_Rogue_Loyalty_Nod


	Type		male
	  Anim          male\yes_wave_left_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd

	Type		huge
	  Anim          male\yes_wave_left_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd

	Type		fem
	  Anim          male\yes_wave_left_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd
MEnd

#############################################################
## "Give gift"
#############################################################

Move Emote_Gift_Give
	Requires	EMOTE THANK LOW SHORTTOSS
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Gift_Give_post

	Type		male
	  Anim          male\gift_giving 1 30
	TEnd
MEnd

Move Emote_Gift_Give_post
	Include sequencers\Members\Emotes_Idle.inc

	Type		male
	  Anim          male\gift_giving 31
	TEnd
MEnd

#############################################################
## "Firework Rocket Flare"
#############################################################

Move Emote_firework_rocket
	Requires	EMOTE FIRE LAUNCH HIGH
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_firework_rocket_post

	Type		male
	  Anim          male\emote_firework_rocket 1 115
	  PlayFX	"Emotes\Fireworks\Fireworks_Rocket.fx" 1 CONSTANT
	TEnd
MEnd

Move Emote_firework_rocket_post
	Include sequencers\Members\Emotes_Idle.inc

	Type		male
	  Anim          male\emote_firework_rocket 116
	TEnd
MEnd

#############################################################
## "Firework Bloom"
#############################################################

Move Emote_firework_bloom
	Requires	EMOTE FIRE LAUNCH MID
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_firework_bloom_post

	Type		male
	  Anim          male\emote_firework_bloom 1 65
	  PlayFX	"Emotes\Fireworks\Fireworks_Bloom.fx" 1 CONSTANT
	TEnd
MEnd

Move Emote_firework_bloom_post
	Include sequencers\Members\Emotes_Idle.inc

	Type		male
	  Anim          male\emote_firework_bloom 70
	TEnd
MEnd

#############################################################
## "Firework Sparkle"
#############################################################

Move Emote_firework_sparkle
	Requires	EMOTE FIRE LAUNCH LOW
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_firework_sparkle_post

	Type		male
	  Anim          male\emote_firework_sparkle 1 65
	  PlayFX	"Emotes\Fireworks\Fireworks_Sparkle.fx" 1 CONSTANT
	TEnd
MEnd

Move Emote_firework_sparkle_post
	Include sequencers\Members\Emotes_Idle.inc

	Type		male
	  Anim          male\emote_firework_sparkle 70
	TEnd
MEnd

##############################################################
### "Receive gift"
##############################################################
#
#Move Emote_Gift_Receive
#	Requires	BOX HAPPY
#	Priority	2
#	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
#	Include sequencers\Members\Emotes_Idle.inc
#	NextMove	Emote_Gift_Receive_post
#
#	Type		male
#	  Anim          male\gift_receiving 1 30
#	TEnd
#MEnd
#
#Move Emote_Gift_Receive_post
#	Include sequencers\Members\Emotes_Idle.inc
#
#	Type		male
#	  Anim          male\gift_receiving 31
#	TEnd
#MEnd

###############################################
Include sequencers\player\emotes_Juggle.inc
Include sequencers\player\emotes_DevOnly.inc
Include sequencers\player\emotes_ScienceBooster.inc
Include sequencers\player\emotes_NaturalBooster.inc
Include sequencers\player\emotes_MutantBooster.inc
Include sequencers\player\emotes_RecreationBooster.inc
Include sequencers\player\emotes_DanceBooster.inc
Include sequencers\player\emotes_AnimalBooster.inc
Include sequencers\player\emotes_HolidayBooster.inc
Include sequencers\player\emotes_SteampunkBooster.inc