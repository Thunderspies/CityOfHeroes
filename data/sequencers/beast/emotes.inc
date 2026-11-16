#############################################################
## Emotes_beast.inc - Emotes Specific to the 'beast' rig
#############################################################
## Interrupts
## <IRQEmotesALL> - Other emotes, basic readies
## <IRQEmotesCombat> - Combat cycles
## <IRQEmotesRighthand> - Right-Handed weapon combat cycles
## <IRQEmotesRighthand> - Left-Handed weapon combat cycles
## <IRQEmotesRighthand> - Dual Wield weapon combat cycles
##
## Members
## <DeathIRQ> - All emotes should be a member of this group
## <AttackIRQ> - All emotes should be a member of this group
## <AttackIRQ> - All emotes should be a member of this group
## <MoveIRQ> - All emotes should be a member of this group
## <ReactIRQ> - Emotes should be a member of this group unless we specifically don't want them interruptible by being hit
## <HitIRQ> - Emotes should be a member of this group unless we specifically don't want them interruptible by being hit
## <IRQEmotesALL> - All emotes should be a member of this group UNLESS we want to restrict spamming them (ie. Howls, slaps, etc)

#############################################################
## Emote Ledge Sit
#############################################################

Move Emote_Beast_Ledge_Sit
	Requires	EMOTE LEDGESIT
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Emote_Beast_Ledge_Sit_cycle
	Interpolate	8

	Type		Beast
	  Anim          male\MonComb_cycle 1
	TEnd
MEnd

Move Emote_Beast_Ledge_Sit_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle

	Type		Beast
	  Anim          male\MonComb_cycle 1
	TEnd
MEnd


#############################################################
## Sitting Poses
#############################################################

Move Emote_Beast_SittingPose
	Requires	EMOTE SIT POSE #TEN ONE
	Priority	2 #+2 FOR EXTRA BITS USED FOR VARIANTS
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		FixArmReg
	NextMove        Emote_Beast_SittingPose_Cycle

	Type		Beast
	  Anim          male\MonComb_cycle 1
	TEnd
MEnd

Move Emote_Beast_SittingPose_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle FixArmReg

	Type		Beast
	  Anim          male\MonComb_cycle 1
	TEnd
MEnd

#############################################################
## Eat Donut
#############################################################

Move Emote_Beast_EatDonut_Pre
	Requires	EMOTE EATDONUT
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NoSizeScale PickRandomly
	NextMove        Emote_Beast_EatDonut_Intro
	Scale		0.75

	Type		Beast
	  Anim          huge\emote_eat_loop 1 11
	  PlayFx        Generic\Eat_Donut.fx 0 Constant
	TEnd
MEnd

Move Emote_Beast_EatDonut_Intro
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NoSizeScale PickRandomly
	NextMove        Emote_Beast_EatDonut_Loop1
	Scale		0.75

	Type		Beast
	  Anim          huge\emote_eat_loop 11
	  PlayFx        Generic\Eat_Donut.fx 0 Constant
	TEnd
MEnd

Move Emote_Beast_EatDonut_Loop1
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
	Scale		0.75
	CycleMove       Emote_Beast_EatDonut_Loop1
	CycleMove       Emote_Beast_EatDonut_Loop2

	Type		Beast
	  Anim          huge\emote_eat_loop 1
	  PlayFx        Generic\Eat_Donut.fx 0 Constant
	TEnd
MEnd

Move Emote_Beast_EatDonut_Loop2
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale #PickRandomly
	Scale		0.625
	CycleMove       Emote_Beast_EatDonut_Loop1

	Type		Beast
	  Anim          huge\emote_eat_loop2 1
	  PlayFx        Generic\Eat_Donut.fx 0 Constant
	TEnd
MEnd


#############################################################
## Emote Drink
#############################################################

Move Emote_Beast_Drink_intro
	Requires	EMOTE DRINK
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Emote_Beast_Drink_cycle

	Type		Beast
	  Anim          male\PL_DrinkIdle 1 11
	  PlayFx        Generic\FX_DrinkingCup.fx 0 Constant
	TEnd
MEnd

Move Emote_Beast_Drink
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Emote_Beast_Drink_cycle

	Type		Beast
	  Anim          male\PL_DrinkIdle 11
	  PlayFx        Generic\FX_DrinkingCup.fx 0 Constant
	TEnd
MEnd

Move Emote_Beast_Drink_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	CycleMove	Emote_Beast_Drink_cycle
	NextMove        Ready_A

	Type		Beast
	  Anim          male\PL_DrinkIdle 1
	  PlayFx        Generic\FX_DrinkingCup.fx 0 Constant
	TEnd
MEnd


#############################################################
## Emote Eat food
#############################################################

Move Emote_Beast_eat_food_pre
	Requires	EMOTE EATFOOD
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Priority	3
	NextMove	Emote_Beast_eat_food_intro

	Type		Beast
	  Anim          Beast\eat 1 11
	  PlayFx        GENERIC\meatbone.fx 0 Constant
	TEnd
MEnd

Move Emote_Beast_eat_food_intro
	Include sequencers\Members\Emotes_Idle.inc
	Priority	3
	NextMove	Emote_Beast_eat_food

	Type		Beast
	  Anim          Beast\eat 11
	  PlayFx        GENERIC\meatbone.fx 0 Constant
	TEnd
MEnd

Move Emote_Beast_eat_food
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	CycleMove	Emote_Beast_nibble_food
	CycleMove       Emote_Beast_lookquick_food
	CycleMove       Emote_Beast_lookquick_slow_food

	Type		Beast
	  Anim          Beast\eat 1
	  PlayFx        GENERIC\meatbone.fx 0 Constant
	TEnd
MEnd

Move Emote_Beast_nibble_food
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	CycleMove       Emote_Beast_lookquick_food
	CycleMove       Emote_Beast_nibble_blend_food

	Type		Beast
	  Anim          Beast\nibble 1
	  PlayFx        GENERIC\meatbone.fx 0 Constant
	TEnd
MEnd

Move Emote_Beast_nibble_blend_food
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	CycleMove       Emote_Beast_lookquick_slow_food

	Type		Beast
	  Anim          Beast\nibble 11
	  PlayFx        GENERIC\meatbone.fx 0 Constant
	TEnd
MEnd

Move Emote_Beast_lookquick_food
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	CycleMove       Emote_Beast_eat_food

	Type		Beast
	  Anim          Beast\eat_lookquick 4
	  PlayFx        GENERIC\meatbone.fx 0 Constant
	TEnd
MEnd

Move Emote_Beast_lookquick_slow_food
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	CycleMove       Emote_Beast_eat_food
	Scale		0.6

	Type		Beast
	  Anim          Beast\eat_lookquick 4
	  PlayFx        GENERIC\meatbone.fx 0 Constant
	TEnd
MEnd


#############################################################
## Emote Grief
#############################################################

Move Emote_Beast_grief_beast
	Requires	EMOTE SAD POSE SHRUG
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Ready_H
	Priority	5
	Scale		0.5

	Type		Beast
	  Anim          Beast\howl 1
	  #PlayFX	EnemyFx\5thcolumn\NightWolf\nightwolfhowl_B.fx 16
	TEnd
MEnd


#############################################################
## Beast Read Newspaper
#############################################################

Move Emote_Beast_AFK_newspaper_pre
	Requires	EMOTE AFK
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Emote_Beast_AFK_newspaper_intro
	Priority	3
	Flags		FixArmReg NoSizeScale
	Scale		0.5

	Type		beast
	  Anim          Beast\lookquick 1 11
	TEnd
MEnd

Move	Emote_Beast_AFK_newspaper_intro
	Include sequencers\Members\Emotes_Idle.inc
	Flags		FixArmReg NoSizeScale
	Scale		0.5

	Type		beast
	  Anim          Beast\lookquick 11
	TEnd
MEnd

#############################################################
## Beast Listen Police Band
#############################################################

Move Emote_Beast_ListenPoliceBand_pre
	Requires	EMOTE LISTENPOLICEBAND
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Emote_Beast_ListenPoliceBand_intro
	Flags		FixArmReg NoSizeScale
	Scale		0.5

	Type		beast
	  Anim          Beast\lookquick 1 11
	TEnd
MEnd

Move Emote_Beast_ListenPoliceBand_intro
	Include sequencers\Members\Emotes_Idle.inc
	Flags		FixArmReg NoSizeScale
	Scale		0.5

	Type		beast
	  Anim          Beast\lookquick 11
	TEnd
MEnd

#############################################################
## Random Dance - Dance Base
#############################################################

Move D_Base
	Requires	EMOTE DANCE LOW
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	D_Base_cycle
	Flags		PICKRANDOMLY NoSizeScale

	Type		Beast
	  Anim          male\dance_base 1
	TEnd
MEnd

Move D_Base_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale

	Type		Beast
	  Anim          male\dance_base 1
	TEnd
MEnd

#############################################################
## Random Dance - Dance Block
#############################################################

Move D_Block
	Requires	EMOTE DANCE LOW
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	D_Block_cycle
	Flags		PICKRANDOMLY NoSizeScale

	Type		Beast
	  Anim          male\dance_block 1
	TEnd
MEnd

Move D_Block_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale

	Type		Beast
	  Anim          male\dance_block 1
	TEnd
MEnd


#############################################################
## Random Dance - Dance Washer
#############################################################

Move D_Washer
	Requires	EMOTE DANCE LOW
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	D_Washer_cycle
	Flags		PICKRANDOMLY NoSizeScale

	Type		Beast
	  Anim          male\dance_washer 1
	TEnd
MEnd

Move D_Washer_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale

	Type		Beast
	  Anim          male\dance_washer 1
	TEnd
MEnd


#############################################################
## Random Dance - Dance Armsup
#############################################################

Move D_Armsup
	Requires	EMOTE DANCE LOW
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	D_Armsup_cycle
	Flags		PICKRANDOMLY NoSizeScale

	Type		Beast
	  Anim          male\dance_armsup 1
	TEnd
MEnd

Move D_Armsup_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale

	Type		Beast
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
	Flags		PICKRANDOMLY NoSizeScale

	Type		Beast
	  Anim          male\dance_bounce 1
	TEnd
MEnd

Move D_Bounce_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale

	Type		Beast
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
	Flags		PICKRANDOMLY NoSizeScale

	Type		Beast
	  Anim          male\dance_monkey 1
	TEnd
MEnd

Move D_Monkey_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale

	Type		Beast
	  Anim          male\dance_monkey 1
	TEnd
MEnd


#############################################################
## Dance Base
#############################################################

Move Dance1
	Requires	EMOTE DANCE ONE
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	D_Base_cycle

	Type		Beast
	  Anim          male\dance_base 1
	TEnd
MEnd

Move Dance1_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale

	Type		Beast
	  Anim          male\dance_base 1
	TEnd
MEnd


#############################################################
## Dance Block
#############################################################

Move Dance2
	Requires	EMOTE DANCE TWO
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	D_Block_cycle

	Type		Beast
	  Anim          male\dance_block 1
	TEnd
MEnd

Move Dance2_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale

	Type		Beast
	  Anim          male\dance_block 1
	TEnd
MEnd


#############################################################
## Dance Washer
#############################################################

Move Dance3
	Requires	EMOTE DANCE THREE
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	D_Washer_cycle

	Type		Beast
	  Anim          male\dance_washer 1
	TEnd
MEnd

Move Dance3_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale

	Type		Beast
	  Anim          male\dance_washer 1
	TEnd
MEnd


#############################################################
## Dance Armsup
#############################################################

Move Dance4
	Requires	EMOTE DANCE FOUR
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	D_Armsup_cycle

	Type		Beast
	  Anim          male\dance_armsup 1
	TEnd
MEnd

Move Dance4_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale

	Type		Beast
	  Anim          male\dance_armsup 1
	TEnd
MEnd


#############################################################
## Dance Bounce
#############################################################

Move Dance5
	Requires	EMOTE DANCE FIVE
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	D_Bounce_cycle

	Type		Beast
	  Anim          male\dance_bounce 1
	TEnd
MEnd

Move Dance5_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale

	Type		Beast
	  Anim          male\dance_bounce 1
	TEnd
MEnd


#############################################################
## Dance Monkey
#############################################################

Move Dance6
	Requires	EMOTE DANCE SIX
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	D_Monkey_cycle

	Type		Beast
	  Anim          male\dance_monkey 1
	TEnd
MEnd

Move Dance6_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale

	Type		Beast
	  Anim          male\dance_monkey 1
	TEnd
MEnd

#############################################################
## Wounded
#############################################################

Move Emote_Wounded_Pre
	Requires	EMOTE WOUNDED
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Wounded
	Priority	2
	Scale           0.50

	Type		Beast
	  Anim          male\dizzy_cycle 1 31
	TEnd
MEnd

Move Emote_Wounded
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	1
	Scale           0.50

	Type		Beast
	  Anim          male\dizzy_cycle 1 31
	TEnd
MEnd

#############################################################
## Talker
#############################################################

Move Emote_Talk_Pre
	Requires	EMOTE TALK
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NoSizeScale
	NextMove	Emote_Talk_Cycle
	Priority	2
	Scale		0.75

	Type		Beast
	  Anim          male\PL_TalkIdle 1
	TEnd
MEnd

Move Emote_Talk_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
	NextMove	Ready
	Priority	2
	Scale		0.75

	CycleMove	Emote_Talk_Cycle
	CycleMove	Emote_Talk01
	CycleMove	Emote_Talk02
	CycleMove	Emote_Talk03

	Type		Beast
	  Anim          male\PL_TalkIdle 1
	TEnd
MEnd

Move Emote_Talk01
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
	NextMove	Ready
	Priority	1
	Scale		0.75

	CycleMove	Emote_Talk_Cycle
	CycleMove	Emote_Talk_Cycle
	CycleMove	Emote_Talk02
	CycleMove	Emote_Talk03

	Type		Beast
	  Anim          male\PL_Talk01 1
	TEnd
MEnd

Move Emote_Talk02
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
	NextMove	Ready
	Priority	1
	Scale		0.75

	CycleMove	Emote_Talk_Cycle
	CycleMove	Emote_Talk_Cycle
	CycleMove	Emote_Talk01
	CycleMove	Emote_Talk03

	Type		Beast
	  Anim          male\PL_Talk02 1
	TEnd
MEnd

Move Emote_Talk03
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
	NextMove	Ready
	Priority	1
	Scale		0.75

	CycleMove	Emote_Talk_Cycle
	CycleMove	Emote_Talk_Cycle
	CycleMove	Emote_Talk01
	CycleMove	Emote_Talk02

	Type		Beast
	  Anim          male\PL_Talk03 1
	TEnd
MEnd


#############################################################
## Howl
#############################################################

Move Emote_Howl
	Requires	EMOTE HOWL
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	CombatReady #Scream_Post
	Priority	2
	Flags		NoSizeScale
	Scale		0.625

	Type		Beast
	  Anim          huge\scream 1 #10 58
	  PlayFX	Emotes\PlayerHowl.fx 5
	TEnd
MEnd


#############################################################
## Stance - Hands on Hips
#############################################################

Move Emote_HandsOnHips
	Requires	EMOTE AKIMBO
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_HandsOnHips_cycle
	Priority	2

	Type		Beast
	  Anim          huge\yes 1 90
	TEnd
MEnd

Move Emote_HandsOnHips_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0
	Interpolate     0

	Type		Beast
	  Anim          huge\yes 91 95
	TEnd
MEnd


#############################################################
## Stance - Cross Arms
#############################################################

Move emote_XArms
	Requires	EMOTE CROSSARMS
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	emote_XArms_Cycle
	Priority	2
	Scale		0.6
	Interpolate	15

	Type		Beast
	  Anim          huge\no 1 90
	TEnd
MEnd

Move emote_XArms_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Interpolate     0

	Type		Beast
	  Anim          huge\no 91 95
	TEnd
MEnd


#############################################################
## Whistle
#############################################################

Move Emote_Whistle
	Requires	EMOTE WHISTLE
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Threat_Cycle
	Priority	2
	Interpolate     7

	Type		Beast
	  Anim          huge\whistle 1 50
	  PlayFx        FEET\WHISTLE.FX  19
	TEnd
MEnd


#############################################################
## RaiseHand
#############################################################

Move Emote_RaiseHand
	Requires	EMOTE RAISEHAND
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_RaiseHand_cycle
	Priority	2

	Type		Beast
	  Anim          huge\raise_hand 1 50
	TEnd
MEnd

Move Emote_RaiseHand_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0
	Interpolate     0

	Type		Beast
	  Anim          huge\raise_hand 50 55
	TEnd
MEnd


#############################################################
## "Point\Attack!"
#############################################################

Move Emote_Attack
	Requires	EMOTE ATTACK
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Attack2
	Priority	2

	Type		Beast
	  Anim          huge\attack 1 57
	TEnd
MEnd

Move Emote_Attack2
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_HandsOnHips_cycle
	Priority	0
	Interpolate     0

	Type		Beast
	  Anim          huge\attack 56
	TEnd
MEnd


#############################################################
## "Don't Attack"
#############################################################

Move Emote_DontAttack
	Requires	EMOTE NO
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_DontAttack2
	Priority	2

	Type		Beast
	  Anim          huge\NoAttack 1 62
	TEnd
MEnd

Move Emote_DontAttack2
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Xarms_Cycle
	Priority	0
	Interpolate     0

	Type		Beast
	  Anim          huge\NoAttack 63
	TEnd
MEnd


#############################################################
## "Over Here"
#############################################################

Move Emote_OverHere
	Requires	EMOTE BIGWAVE
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_OverHere2
	Priority	2

	Type		Beast
	  Anim          huge\over_here 1 88
	TEnd
MEnd

Move Emote_OverHere2
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_HandsOnHips_cycle
	Priority	0
	Interpolate     0

	Type		Beast
	  Anim          huge\over_here 89
	TEnd
MEnd


#############################################################
## "Hi"
#############################################################

Move Emote_Hi
	Requires	EMOTE WAVE
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Hi2
	Priority	2

	Type		Beast
	  Anim          huge\hi 1 45
	TEnd
MEnd

Move Emote_Hi2
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_HandsOnHips_cycle
	Priority	0
	Interpolate     0

	Type		Beast
	  Anim          huge\hi 46
	TEnd
MEnd


#############################################################
## ThumbsUp
#############################################################

Move Emote_ThumbsUp
	Requires	EMOTE YES
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_ThumbsUp2
	Priority	2

	Type		Beast
	  Anim          huge\thumbsup 1 60
	TEnd
MEnd

Move Emote_ThumbsUp2
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_HandsOnHips_cycle
	Priority	0
	Interpolate     0

	Type		Beast
	  Anim          huge\thumbsup 61
	TEnd
MEnd


#############################################################
## Rock\Paper\Scissors - "Rock"
#############################################################

Move RPS_Rock_pre
	Requires	EMOTE ROCK
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	RPS_Rock_cast
	Priority	2
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\RPS_pre 15 150
	  PlayFx        GENERIC\RPS_pre.FX 0 Constant
	TEnd
MEnd

Move RPS_Rock_cast
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	RPS_Rock
	Priority	1
	Interpolate	0
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\RPS_rock 28 73
	  PlayFx        GENERIC\RPS_pre.FX 0 Constant
	TEnd
MEnd

Move RPS_Rock
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	RPS_Rock_cycle
	Priority	1
	Interpolate	0
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\RPS_rock 74 104
	  PlayFx        GENERIC\RPS_rock.FX  0 Constant
	TEnd
MEnd

Move RPS_Rock_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Interpolate	0
	Priority	0
	Flags		Cycle NoSizeScale

	Type		Beast
	  Anim          male\RPS_rock 105
	  PlayFx        GENERIC\RPS_rock.FX  0 Constant
	TEnd
MEnd


#############################################################
## Rock\Paper\Scissors - "Paper"
#############################################################

Move RPS_paper_pre
	Requires	EMOTE PAPER
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	RPS_paper_cast
	Priority	2
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\RPS_pre 15 150
	  PlayFx        GENERIC\RPS_pre.FX 0 Constant
	TEnd
MEnd


Move RPS_paper_cast
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	RPS_paper
	Priority	1
	Interpolate	0
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\RPS_paper 28 73
	  PlayFx        GENERIC\RPS_pre.FX 0 Constant
	TEnd
MEnd

Move RPS_paper
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	RPS_paper_cycle
	Priority	1
	Interpolate	0
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\RPS_paper 74 104
	  PlayFx        GENERIC\RPS_paper.FX  0 Constant
	TEnd
MEnd

Move RPS_paper_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Interpolate	0
	Priority	0
	Flags		Cycle NoSizeScale

	Type		Beast
	  Anim          male\RPS_paper 105
	  PlayFx        GENERIC\RPS_paper.FX  0 Constant
	TEnd
MEnd


#############################################################
## Rock\Paper\Scissors - "Scissors"
#############################################################

Move RPS_scissor_pre
	Requires	EMOTE SCISSORS
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	RPS_scissor_cast
	Priority	2
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\RPS_pre 15 150
	  PlayFx        GENERIC\RPS_pre.FX 0 Constant
	TEnd
MEnd

Move RPS_scissor_cast
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	RPS_scissor
	Priority	1
	Interpolate	0
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\RPS_scissor 28 73
	  PlayFx        GENERIC\RPS_pre.FX 0 Constant
	TEnd
MEnd

Move RPS_scissor
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	RPS_scissor_cycle
	Priority	1
	Interpolate	0
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\RPS_scissor 74 104
	  PlayFx        GENERIC\RPS_scissor.FX  0 Constant
	TEnd
MEnd

Move RPS_scissor_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Priority	1
	Flags		Cycle NoSizeScale

	Type		Beast
	  Anim          male\RPS_scissor 105
	  PlayFx        GENERIC\RPS_scissor.FX  0 Constant
	TEnd
MEnd


#############################################################
## Roll Dice #1
#############################################################

Move 1_RollDice_pre
	Requires	EMOTE ROLLDICE HIGH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	1_RollDice_cast
	Priority	3
	Scale		1.5
	Interpolate	12
	Flags		PICKRANDOMLY NoSizeScale

	Type		Beast
	  Anim          male\roll_dice 125 201  ##209
	TEnd
MEnd

Move 1_RollDice_cast
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	postReady_back
	Priority	1
	Interpolate	8
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\roll_dice 1 120
	  PlayFx        GENERIC\Dice_1.FX  80 Constant
	  PlayFx        GENERIC\Prop_Dice.FX  69
	TEnd
MEnd


#############################################################
##  Roll Dice #2
#############################################################

Move 2_RollDice_pre
	Requires	EMOTE ROLLDICE HIGH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	2_RollDice_cast
	Priority	3
	Scale		1.5
	Interpolate	12
	Flags		PICKRANDOMLY NoSizeScale

	Type		Beast
	  Anim          male\roll_dice 125 201  ##209
	TEnd
MEnd

Move 2_RollDice_cast
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	postReady_back
	Priority	1
	Interpolate	8
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\roll_dice 1 120
	  PlayFx        GENERIC\Dice_2.FX  80 Constant
	  PlayFx        GENERIC\Prop_Dice.FX  69
	TEnd
MEnd


#############################################################
## Roll Dice #3
#############################################################

Move 3_RollDice_pre
	Requires	EMOTE ROLLDICE HIGH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	3_RollDice_cast
	Priority	3
	Scale		1.5
	Interpolate	12
	Flags		PICKRANDOMLY NoSizeScale

	Type		Beast
	  Anim          male\roll_dice 125 201  ##209
	TEnd
MEnd


Move 3_RollDice_cast
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	postReady_back
	Priority	1
	Interpolate	8
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\roll_dice 1 120
	  PlayFx        GENERIC\Dice_3.FX  80 Constant
	  PlayFx        GENERIC\Prop_Dice.FX  69
	TEnd
MEnd


#############################################################
## Roll Dice #4
#############################################################

Move 4_RollDice_pre
	Requires	EMOTE ROLLDICE HIGH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	4_RollDice_cast
	Priority	3
	Scale		1.5
	Interpolate	12
	Flags		PICKRANDOMLY NoSizeScale

	Type		Beast
	  Anim          male\roll_dice 125 201  ##209
	TEnd
MEnd

Move 4_RollDice_cast
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	postReady_back
	Priority	1
	Interpolate	8
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\roll_dice 1 120
	  PlayFx        GENERIC\Dice_4.FX  80 Constant
	  PlayFx        GENERIC\Prop_Dice.FX  69
	TEnd
MEnd


#############################################################
##  Roll Dice #5
#############################################################

Move 5_RollDice_pre
	Requires	EMOTE ROLLDICE HIGH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	5_RollDice_cast
	Priority	3
	Scale		1.5
	Interpolate	12
	Flags		PICKRANDOMLY NoSizeScale

	Type		Beast
	  Anim          male\roll_dice 125 201  ##209
	TEnd
MEnd

Move 5_RollDice_cast
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	postReady_back
	Priority	1
	Interpolate	8
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\roll_dice 1 120
	  PlayFx        GENERIC\Dice_5.FX  80 Constant
	  PlayFx        GENERIC\Prop_Dice.FX  69
	TEnd
MEnd


#############################################################
## Roll Dice #6
#############################################################

Move 6_RollDice_pre
	Requires	EMOTE ROLLDICE HIGH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	6_RollDice_cast
	Priority	3
	Scale		1.5
	Interpolate	12
	Flags		PICKRANDOMLY NoSizeScale

	Type		Beast
	  Anim          male\roll_dice 125 201  ##209
	TEnd
MEnd

Move 6_RollDice_cast
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	postReady_back
	Priority	1
	Interpolate	8
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\roll_dice 1 120
	  PlayFx        GENERIC\Dice_6.FX  80 Constant
	  PlayFx        GENERIC\Prop_Dice.FX 69
	TEnd
MEnd


#############################################################
## Roll Dice #7
#############################################################

Move 7_RollDice_pre
	Requires	EMOTE DICE HIGH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	7_RollDice_cast
	Priority	1
	Scale		1.5
	Interpolate	12
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\roll_dice 125 201  ##209
	TEnd
MEnd

Move 7_RollDice_cast
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	postReady_back
	Priority	1
	Interpolate	8
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\roll_dice 1 120
	  PlayFx        GENERIC\Dice_7.FX  80 Constant
	  PlayFx        GENERIC\Prop_Dice.FX 69
	TEnd
MEnd


#############################################################
## Flip Coin (heads A)
#############################################################

Move Flipcoin_heads
	Requires	EMOTE ROLLDICE CAST LOW
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Flipcoin_heads_cycle
	Priority	4
	Flags		PICKRANDOMLY

	Type		Beast
	  Anim          male\flipcoin 1 140
	  PlayFx        GENERIC\Coin_A.FX  113 Constant
	  PlayFx        GENERIC\Prop_Coin.FX  56
	TEnd
MEnd

Move Flipcoin_heads_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	1
	Interpolate	0

	Type		Beast
	  Anim          male\flipcoin 144
	  PlayFx        GENERIC\Coin_A.FX  0 Constant
	TEnd
MEnd


#############################################################
## Flip Coin (tails B)
#############################################################

Move Flipcoin_tail
	Requires	EMOTE ROLLDICE CAST LOW
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Flipcoin_tail_cycle
	Priority	4
	Flags		PICKRANDOMLY

	Type		Beast
	  Anim          male\flipcoin 1 140
	  PlayFx        GENERIC\Coin_B.FX  113 Constant
	  PlayFx        GENERIC\Prop_Coin.FX  56
	TEnd
MEnd

Move Flipcoin_tail_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	1
	Interpolate	0

	Type		Beast
	  Anim          male\flipcoin 144
	  PlayFx        GENERIC\Coin_B.FX  0 Constant
	TEnd
MEnd


#############################################################
## Flex A
#############################################################

Move Flex_A
	Requires	EMOTE FLEX HIGH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Flex_A_cycle
	Priority	3

	Type		Beast
	  Anim          male\flex_A 5 54
	TEnd
MEnd

Move Flex_A_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
	Interpolate	0
	Priority	0

	Type		Beast
	  Anim          male\flex_A 55
	TEnd
MEnd


#############################################################
## Flex B
#############################################################

Move Flex_B
	Requires	EMOTE FLEX MID
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Flex_B_cycle
	Priority	3

	Type		Beast
	  Anim          male\flex_B 6 54
	TEnd
MEnd

Move Flex_B_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
	Interpolate	0
	Priority	0

	Type		Beast
	  Anim          male\flex_B 55
	TEnd
MEnd


#############################################################
## Flex C
#############################################################

Move Flex_C
	Requires	EMOTE FLEX LOW
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Flex_C_cycle
	Priority	3

	Type		Beast
	  Anim          male\flex_C 11 119
	TEnd
MEnd

Move Flex_C_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
	Interpolate	0
	Priority	0

	Type		Beast
	  Anim          male\flex_C 120
	TEnd
MEnd


#############################################################
## Heroic Laugh
#############################################################

Move Emote_Laugh
	Requires	EMOTE LAUGH LOW
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Laugh_cycle
	Priority	3

	Type		Beast
	  Anim          male\heroic_laugh 1
	TEnd
MEnd

Move Emote_Laugh_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
	Interpolate	0
	Priority	0

	Type		Beast
	  Anim          male\thumbsup 99 100
	TEnd
MEnd


#############################################################
## Somewhat Mad
#############################################################

Move Emote_Mad
	Requires	EMOTE ANGRY POSE LOW
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Mad_cycle
	Priority	4
	Interpolate	8

	Type		Beast
	  Anim          male\mad 1 55
	TEnd
MEnd

Move Emote_Mad_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0
	Interpolate     0

	Type		Beast
	  Anim          male\mad 55 60
	TEnd
MEnd


#############################################################
## "What?"
#############################################################

Move Emote_What
	Requires	EMOTE QUESTION LOW
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_What_cycle
	Priority	3
	Interpolate	8

	Type		Beast
	  Anim          male\what 1
	TEnd
MEnd

Move Emote_What_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0
	Interpolate     8

	Type		Beast
	  Anim          male\centerstance_cycle 1
	TEnd
MEnd


#############################################################
## Point
#############################################################

Move Emote_Point
	Requires	EMOTE POINT
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Point_cycle
	Priority	2
	Interpolate	8

	Type		Beast
	  Anim          male\point 1
	TEnd
MEnd

Move Emote_Point_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0
	Interpolate     0

	Type		Beast
	  Anim          male\thumbsup 99 100
	TEnd
MEnd


#############################################################
## Salute
#############################################################

Move Emote_Salute
	Requires	EMOTE SALUTE ONE
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Salute_cycle
	Priority	3
	Interpolate	8

	Type		Beast
	  Anim          male\salute 1
	TEnd
MEnd

Move Emote_Salute_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0
	Interpolate     0

	Type		Beast
	  Anim          male\thumbsup 99 100
	TEnd
MEnd


#############################################################
## Argue - Lecture
#############################################################

Move Argue
	Requires	EMOTE ARGUE HIGH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Argue_cycle
	Priority	3
	Interpolate	8

	Type		Beast
	  Anim          male\recruit_nobox 129 800
	TEnd
MEnd

Move Argue_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Interpolate	0
	Priority	0

	Type		Beast
	  Anim          male\thumbsup 99 100
	TEnd
MEnd


#############################################################
## Argue - Disagree
#############################################################

Move Argue_Disagree
	Requires	EMOTE ARGUE MID
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Argue_Disagree_cycle
	Priority	3
	Interpolate	8

	Type		Beast
	  Anim          male\deal_A 1 238
	TEnd
MEnd

Move Argue_Disagree_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Interpolate	0
	Priority	0

	Type		Beast
	  Anim          male\no 110 115
	TEnd
MEnd


#############################################################
## Argue - Counterpoint
#############################################################

Move Argue_Reason
	Requires	EMOTE ARGUE LOW
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Argue_Reason2
	Priority	3
	Interpolate	8

	Type		Beast
	  Anim          male\deal_B 92 238
	TEnd
MEnd

Move Argue_Reason2
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Argue_Reason_cycle
	Priority	1
	Interpolate	8

	Type		Beast
	  Anim          male\deal_B 1 83
	TEnd
MEnd

Move Argue_Reason_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Interpolate	0
	Priority	0

	Type		Beast
	  Anim          male\thumbsup 99 100
	TEnd
MEnd


#############################################################
## KungfuTrain
#############################################################

Move Emote_KungfuTrain
	Requires	EMOTE KUNGFU TRAIN STRONG
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_KungfuTrain_cycle
	Priority	4

	Type		Beast
	  Anim          male\train_kungfu_A 1
	  PlayFx        FEET\HUGEJUMP2.FX 17
	  PlayFx        FEET\HUGEJUMP2.FX 74
	  PlayFx        FEET\HUGEJUMP2.FX 127
	  PlayFx        FEET\HUGEJUMP2.FX 194
	TEnd
MEnd

Move Emote_KungfuTrain_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Interpolate	0
	Priority	0

	Type		Beast
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
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_JumpingJacks_cycle
	Priority	4
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\jumpingjax 1
	  PlayFx        FEET\HUGEJUMP2.FX 15
	TEnd
MEnd

Move Emote_JumpingJacks_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
	Interpolate	0
	Priority	0

	Type		Beast
	  Anim          male\jumpingjax 1
	  PlayFx        FEET\HUGEJUMP2.FX 15
	TEnd
MEnd


#############################################################
## Beatchest
#############################################################

Move Emote_Beatchest
	Requires	EMOTE COMBAT TAUNTA ANGRY
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Beatchest_cycle
	Priority	4
	Flags		NoSizeScale

	Type		Beast
	  Anim          huge\comb_beatchest 1 91
	  PlayFx        FEET\HUGETAUNTA.FX
	TEnd
MEnd

Move Beatchest_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
	Priority	1

	Type		Beast
	  Anim          huge\comb_beatchest 95 144
	TEnd
MEnd


#############################################################
## "Come On!"
#############################################################

Move Emote_ComeOn
	Requires	EMOTE COMBAT TAUNTB ANGRY
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	postReady_back #Emote_ComeOn2
	Priority	4
	Interpolate     8
	Flags		NoSizeScale

	Type		Beast
	  Anim          huge\combat_comeon 9 79
	  PlayFX	feet\mALETaunt6.fx
	TEnd
MEnd


#############################################################
## "Come Here!"
#############################################################

Move Emote_ComeHere
	Requires	EMOTE COMBAT TAUNTC ANGRY
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_ComeHere2
	Priority	4

	Type		Beast
	  Anim          huge\combat_comehere 5
	  PlayFX	feet\MaleTaunt8.fx
	TEnd
MEnd

Move Emote_ComeHere2
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_ComeHere_cycle
	Priority	0
	Scale           0.5

	Type		Beast
	  Anim          huge\combat_cycle 15 31
	TEnd
MEnd

Move Emote_ComeHere_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0
	Scale           0.5

	Type		Beast
	  Anim          huge\combat_cycle 1 31
	TEnd
MEnd

#############################################################
## Angery Growl (ROAR)
#############################################################

Move Emote_Growl
	Requires	EMOTE ANGRY HIGH FIERCE
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Growl_cycle
	Priority	4
	Flags		PICKRANDOMLY

	Type		Beast
	  Anim          male\Growl 1
	PlayFX		Generic\Roar.fx
	TEnd
MEnd

#############################################################

Move Emote_Growl_2
	Requires	EMOTE ANGRY HIGH FIERCE
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Growl_cycle
	Priority	4
	Flags		PICKRANDOMLY

	Type		Beast
	  Anim          male\growl_pawR 1
	PlayFX		Generic\Roar.fx
	TEnd
MEnd

#############################################################

Move Emote_Growl_3
	Requires	EMOTE ANGRY HIGH FIERCE
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Growl_cycle
	Priority	4
	Flags		PICKRANDOMLY

	Type		Beast
	  Anim          male\growl_pawL
	PlayFX		Generic\Roar.fx
	TEnd
MEnd

Move Emote_Growl_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle

	Type		Beast
	  Anim          male\growl_cycle 1
	TEnd
MEnd

#############################################################
## Burp
#############################################################

Move Emote_Burp
	Requires	EMOTE RUDE LOW
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Growl_cycle
	Priority	4
	Flags		PICKRANDOMLY NoSizeScale

	Type		Beast
	  Anim          male\Growl_burp 1
	PlayFX		Generic\Burp.fx
	TEnd
MEnd

#############################################################
## Cheer (chant)
#############################################################

Move Cheerchant_A
	Requires	EMOTE CHEER HIGH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Cheerchant_A_cycle
	Priority	3
	Interpolate	8
	Flags		PICKRANDOMLY NoSizeScale

	Type		Beast
	  Anim          male\chant_high 1
	TEnd
MEnd

Move Cheerchant_A_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
	Priority	0
	Interpolate     0

	Type		Beast
	  Anim          male\chant_high 1
	TEnd
MEnd

#############################################################

Move Cheerchant_B
	Requires	EMOTE CHEER HIGH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Cheerchant_B_cycle
	Priority	3
	Interpolate	8
	Flags		PICKRANDOMLY NoSizeScale

	Type		Beast
	  Anim          male\chant_1arm 1
	TEnd
MEnd

Move Cheerchant_B_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
	Priority	0
	Interpolate     0

	Type		Beast
	  Anim          male\chant_1arm 1
	TEnd
MEnd

#############################################################

Move Cheerchant_C
	Requires	EMOTE CHEER HIGH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Cheerchant_C_cycle
	Priority	3
	Interpolate	8
	Flags		PICKRANDOMLY NoSizeScale

	Type		Beast
	  Anim          male\chant_low 1
	TEnd
MEnd

Move Cheerchant_C_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale

	Priority	0
	Interpolate     0

	Type		Beast
	  Anim          male\chant_low 1
	TEnd
MEnd

#############################################################
## Military "At Ease"
#############################################################

Move AtEase
	Requires	EMOTE SALUTE HIGH LOW
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	AtEase_cycle
	Priority	4
	Interpolate	8

	Type		Beast
	  Anim          huge\at_ease 22 60
	TEnd
MEnd

Move AtEase_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0

	Type		Beast
	  Anim          huge\at_ease 61 65
	TEnd
MEnd

#############################################################

Move AtEase_FromMilitarySalute
	Requires	EMOTE SALUTE HIGH LOW
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	AtEase_cycle
	Priority	5

	Type		Beast
	  Anim          male\at_ease 1 60
	TEnd

	Type		Beast
	  Anim          huge\at_ease 1 60
	TEnd
MEnd


#############################################################
## Nod - Cycle
#############################################################

Move Emote_Nod
	Requires	EMOTE YES LOW
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Nod_cycle
	Priority	3

	Type		Beast
	  Anim          male\nod 1 70
	TEnd
MEnd

Move Emote_Nod_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0
	Interpolate     0

	Type		Beast
	  Anim          male\nod 71 75
	TEnd
MEnd


#############################################################
## "Welcome"
#############################################################

Move Emote_Welcome
	Requires	EMOTE GREETING HIGH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Welcome_cycle
	Priority	3
	Interpolate	8

	Type		Beast
	  Anim          male\welcome 1 135
	TEnd
MEnd

Move Emote_Welcome_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0
	Interpolate     0

	Type		Beast
	  Anim          male\welcome 136 140
	TEnd
MEnd


#############################################################
## Fancy_Bow
#############################################################

Move Emote_Fancy_Bow
	Requires	EMOTE BOW GREETING HIGH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Fancy_Bow_cycle
	Priority	4
	Interpolate	8

	Type		Beast
	  Anim          male\fancy_bow 1
	TEnd
MEnd

Move Emote_Fancy_Bow_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0
	Interpolate     0

	Type		Beast
	  Anim          male\centerstance_cycle 1
	TEnd
MEnd


#############################################################
## Victory
#############################################################

Move Emote_Victory
	Requires	EMOTE CHEER FIERCE HIGH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Victory_cycle
	Priority	4
	Interpolate	5

	Type		Beast
	  Anim          male\victory 1
	TEnd
MEnd

Move Emote_Victory_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0
	Interpolate     0

	Type		Beast
	  Anim          male\centerstance_cycle 1
	TEnd
MEnd


#############################################################
## "The Champ"
#############################################################

Move Emote_Champion
	Requires	EMOTE CHEER FLEX
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Champion_cycle
	Priority	3
	Interpolate	8
	Flags		FixArmReg

	Type		Beast
	  Anim          huge\champion 1 150
	TEnd
MEnd

Move Emote_Champion_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0
	Interpolate     0
	Flags		FixArmReg

	Type		Beast
	  Anim          huge\champion 151 152
	TEnd
MEnd


#############################################################
## "The Wave"
#############################################################

Move Emote_TheWave
	Requires	EMOTE CHEER WAVE HIGH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_TheWave_cycle
	Priority	4
	Interpolate	8

	Type		Beast
	  Anim          male\the_wave 1
	TEnd
MEnd

Move Emote_TheWave_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0
	Interpolate     0

	Type		Beast
	  Anim          male\centerstance_cycle 1
	TEnd
MEnd


#############################################################
## Frustrated
#############################################################

Move Emote_Frustrated
	Requires	EMOTE ANGRY SAD MID
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Frustrated2
	Priority	4
	Interpolate	8

	Type		Beast
	  Anim          male\idiots 1 2
	TEnd
MEnd

Move Emote_Frustrated2
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Frustrated_cycle
	Priority	0

	Type		Beast
	  Anim          male\idiots 3
	TEnd
MEnd

Move Emote_Frustrated_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0
	Interpolate     0

	Type		Beast
	  Anim          male\frustrated_cycle 1
	TEnd
MEnd

#############################################################
## Drat
#############################################################

Move Emote_Drat
	Requires	EMOTE ANGRY RUDE SHRUG
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Drat2
	Priority	4
	Interpolate	8

	Type		Beast
	  Anim          male\aw_crap 1 2
	TEnd
MEnd

Move Emote_Drat2
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Drat_cycle
	Priority	0


	Type		Beast
	  Anim          male\aw_crap 3
	TEnd
MEnd

Move Emote_Drat_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0
	Interpolate     0

	Type		Beast
	  Anim          male\frustrated_cycle 1
	TEnd
MEnd


#############################################################
## "You're Welcome"
#############################################################

Move Emote_YourWelcome
	Requires	EMOTE THANK LOW POSE REPAIR
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_YourWelcome_cycle
	Priority	5
	Scale		1.2

	Type		Beast
	  Anim          male\your_welcome_sm 1
	TEnd
MEnd

Move Emote_YourWelcome_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0
	Interpolate     0

	Type		Beast
	  Anim          male\centerstance_cycle 1
	TEnd
MEnd


#############################################################
## "Thank You" - Small
#############################################################

Move Emote_Thankyou_SM
	Requires	EMOTE THANK LOW POSE
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Thankyou_SM_cycle
	Priority	4

	Type		Beast
	  Anim          male\thankyou_sm 1
	TEnd
MEnd

Move Emote_Thankyou_SM_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0
	Interpolate     0

	Type		Beast
	  Anim          male\centerstance_cycle 1
	TEnd
MEnd


#############################################################
## Judge Score = 1
#############################################################

Move Emote_Judge_1
	Requires	EMOTE SCORE ONE
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Judge_1_cycle
	Priority	3


	Type		Beast
	  Anim          male\judge 1 40
	  PlayFx        GENERIC\Score_1.fx  0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 1 40
	  PlayFx        GENERIC\Score_1.fx  0 Constant
	TEnd
MEnd

Move Emote_Judge_1_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0
	Interpolate     0

	Type		Beast
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_1.fx  0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 41
	  PlayFx        GENERIC\Score_1.fx  0 Constant
	TEnd
MEnd


#############################################################
## Judge Score = 2
#############################################################

Move Emote_Judge_2
	Requires	EMOTE SCORE TWO
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Judge_2_cycle
	Priority	3

	Type		Beast
	  Anim          male\judge 1 40
	  PlayFx        GENERIC\Score_2.fx  0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 1 40
	  PlayFx        GENERIC\Score_2.fx  0 Constant
	TEnd
MEnd

Move Emote_Judge_2_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0
	Interpolate     0

	Type		Beast
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_2.fx  0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 41
	  PlayFx        GENERIC\Score_2.fx  0 Constant
	TEnd
MEnd


#############################################################
## Judge Score = 3
#############################################################

Move Emote_Judge_3
	Requires	EMOTE SCORE THREE
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Judge_3_cycle
	Priority	3

	Type		Beast
	  Anim          male\judge 1 40
	  PlayFx        GENERIC\Score_3.fx  0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 1 40
	  PlayFx        GENERIC\Score_3.fx  0 Constant
	TEnd
MEnd

Move Emote_Judge_3_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0
	Interpolate     0

	Type		Beast
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_3.fx  0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 41
	  PlayFx        GENERIC\Score_3.fx  0 Constant
	TEnd
MEnd


#############################################################
## Judge Score = 4
#############################################################

Move Emote_Judge_4
	Requires	EMOTE SCORE FOUR
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Judge_4_cycle
	Priority	3

	Type		Beast
	  Anim          male\judge 1 40
	  PlayFx        GENERIC\Score_4.fx  0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 1 40
	  PlayFx        GENERIC\Score_4.fx  0 Constant
	TEnd
MEnd

Move Emote_Judge_4_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0
	Interpolate     0

	Type		Beast
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_4.fx  0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 41
	  PlayFx        GENERIC\Score_4.fx  0 Constant
	TEnd
MEnd


#############################################################
## Judge Score = 5
#############################################################

Move Emote_Judge_5
	Requires	EMOTE SCORE FIVE
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Judge_5_cycle
	Priority	3

	Type		Beast
	  Anim          male\judge 1 40
	  PlayFx        GENERIC\Score_5.fx  0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 1 40
	  PlayFx        GENERIC\Score_5.fx  0 Constant
	TEnd
MEnd

Move Emote_Judge_5_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0
	Interpolate     0

	Type		Beast
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_5.fx  0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 41
	  PlayFx        GENERIC\Score_5.fx  0 Constant
	TEnd
MEnd


#############################################################
## Judge Score = 6
#############################################################

Move Emote_Judge_6
	Requires	EMOTE SCORE SIX
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Judge_6_cycle
	Priority	3

	Type		Beast
	  Anim          male\judge 1 40
	  PlayFx        GENERIC\Score_6.fx  0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 1 40
	  PlayFx        GENERIC\Score_6.fx  0 Constant
	TEnd
MEnd

Move Emote_Judge_6_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0
	Interpolate     0

	Type		Beast
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_6.fx  0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 41
	  PlayFx        GENERIC\Score_6.fx  0 Constant
	TEnd
MEnd


#############################################################
## Judge Score = 7
#############################################################

Move Emote_Judge_7
	Requires	EMOTE SCORE SEVEN
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Judge_7_cycle
	Priority	3

	Type		Beast
	  Anim          male\judge 1 40
	  PlayFx        GENERIC\Score_7.fx  0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 1 40
	  PlayFx        GENERIC\Score_7.fx  0 Constant
	TEnd
MEnd

Move Emote_Judge_7_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0
	Interpolate     0

	Type		Beast
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_7.fx  0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 41
	  PlayFx        GENERIC\Score_7.fx  0 Constant
	TEnd
MEnd


#############################################################
## Judge Score = 8
#############################################################

Move Emote_Judge_8
	Requires	EMOTE SCORE EIGHT
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Judge_8_cycle
	Priority	3

	Type		Beast
	  Anim          male\judge 1 40
	  PlayFx        GENERIC\Score_8.fx  0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 1 40
	  PlayFx        GENERIC\Score_8.fx  0 Constant
	TEnd
MEnd

Move Emote_Judge_8_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0
	Interpolate     0

	Type		Beast
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_8.fx  0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 41
	  PlayFx        GENERIC\Score_8.fx  0 Constant
	TEnd
MEnd


#############################################################
## Judge Score = 9
#############################################################

Move Emote_Judge_9
	Requires	EMOTE SCORE NINE
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Judge_9_cycle
	Priority	3

	Type		Beast
	  Anim          male\judge 1 40
	  PlayFx        GENERIC\Score_9.fx  0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 1 40
	  PlayFx        GENERIC\Score_9.fx  0 Constant
	TEnd
MEnd

Move Emote_Judge_9_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0
	Interpolate     0

	Type		Beast
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_9.fx  0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 41
	  PlayFx        GENERIC\Score_9.fx  0 Constant
	TEnd
MEnd


#############################################################
##  Judge Score = 10
#############################################################

Move Emote_Judge_10
	Requires	EMOTE SCORE TEN
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Judge_10_cycle
	Priority	3

	Type		Beast
	  Anim          male\judge 1 40
	  PlayFx        GENERIC\Score_10.fx  0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 1 40
	  PlayFx        GENERIC\Score_10.fx  0 Constant
	TEnd
MEnd

Move Emote_Judge_10_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0
	Interpolate     0

	Type		Beast
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_10.fx  0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 41
	  PlayFx        GENERIC\Score_10.fx  0 Constant
	TEnd
MEnd


#############################################################
## "Shucks!"
#############################################################

Move Emote_Shucks
	Requires	EMOTE ANGRY RUDE SHRUG LOW
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Shucks_cycle
	Priority	5

	Type		Beast
	  Anim          male\shucks 1
	TEnd
MEnd

Move Emote_Shucks_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle

	Type		Beast
	  Anim          male\centerstance_cycle 1
	TEnd
MEnd


#############################################################
## Wall Lean
#############################################################

Move Emote_Wall_Lean_1
	Requires	EMOTE LEANWALL
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Wall_Lean_cycle_1
	Priority	1

	Type		Beast
	  Anim          male\wall_lean1 1
	TEnd
MEnd

Move Emote_Wall_Lean_cycle_1
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0

	Type		Beast
	  Anim          male\wall_lean1 1
	TEnd
MEnd

#############################################################

Move Emote_Wall_Lean_3
	Requires	EMOTE LEANWALL
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Wall_Lean_cycle_3
	Priority	1

	Type		Beast
	  Anim          male\wall_lean3 1
	TEnd
MEnd

Move Emote_Wall_Lean_cycle_3
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0

	Type		Beast
	  Anim          male\wall_lean3 1
	TEnd
MEnd


#############################################################
## HandsUp
#############################################################

Move Emote_HandsUp
	Requires	EMOTE HANDSUP
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_HandsUp_cycle_1
	Priority	1
	Interpolate	8

	Type		Beast
	  Anim          male\handsup 1
	TEnd
MEnd

Move Emote_HandsUp_cycle_1
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	CycleMove	Emote_HandsUp_cycle_1
	CycleMove	Emote_HandsUp_cycle_2
	CycleMove	Emote_HandsUp_cycle_1
	Priority	0

	Type		Beast
	  Anim          male\handsup 1
	TEnd
MEnd

Move Emote_HandsUp_cycle_2
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	CycleMove	Emote_HandsUp_cycle_1
	Priority	0

	Type		Beast
	  Anim          male\handsup_var 1
	TEnd
MEnd


#############################################################
## FlashLight
#############################################################

Move Emote_FlashLight_1
	Requires	EMOTE FLASHLIGHT
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_FlashLight_cycle_1
	Priority	2
	Interpolate	8

	Type		Beast
	  Anim          male\Flashlight_Up 1
	  PlayFX	Generic\Flashlight_NoSpot.fx 0 Constant
	TEnd
MEnd

Move Emote_FlashLight_cycle_1
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0

	Type		Beast
	  Anim          male\Flashlight_Up 1
	  PlayFX	Generic\Flashlight_NoSpot.fx 0 Constant
	TEnd
MEnd

#############################################################

Move Emote_FlashLight_2
	Requires	EMOTE FLASHLIGHT
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_FlashLight_cycle_2
	Priority	2
	Interpolate	8

	Type		Beast
	  Anim          male\Flashlight_Down 1
	  PlayFX	Generic\Flashlight_NoSpot.fx 0 Constant
	TEnd
MEnd

Move Emote_FlashLight_cycle_2
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0

	Type		Beast
	  Anim          male\Flashlight_Down 1
	  PlayFX	Generic\Flashlight_NoSpot.fx 0 Constant
	TEnd
MEnd

#############################################################
## FlashLight - UP
#############################################################

Move Emote_FlashLight_Up_Pre
	Requires	EMOTE FLASHLIGHT UP
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_FlashLight_Up_Cycle
	Priority	3
	Interpolate	8

	Type		Beast
	  Anim          male\Flashlight_Up 1
	  PlayFX	Generic\Flashlight_NoSpot.fx 0 Constant
	TEnd

MEnd

Move Emote_FlashLight_Up_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0

	Type		Beast
	  Anim          male\Flashlight_Up 1
	  PlayFX	Generic\Flashlight_NoSpot.fx 0 Constant
	TEnd
MEnd


#############################################################
## FlashLight - DOWN
#############################################################

Move Emote_FlashLight_Down_Pre
	Requires	EMOTE FLASHLIGHT DOWN
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_FlashLight_cycle_2
	Priority	3
	Interpolate	8

	Type		Beast
	  Anim          male\Flashlight_Down 1
	  PlayFX	Generic\Flashlight_NoSpot.fx 0 Constant
	TEnd
MEnd

Move Emote_FlashLight_Down_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0

	Type		Beast
	  Anim          male\Flashlight_Down 1
	  PlayFX	Generic\Flashlight_NoSpot.fx 0 Constant
	TEnd
MEnd


#############################################################
## Book research
#############################################################

Move Emote_Book_research
	Requires	EMOTE BOOKRESEARCH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Book_research_cycle
	Priority	1
	Interpolate	8
	Scale		0.5

	Type		Beast
	  Anim          male\blackboard 1
	  PlayFx        GENERIC\book_left.fx  0 Constant
	TEnd
MEnd

Move Emote_Book_research_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0
	Scale		0.5

	Type		Beast
	  Anim          male\blackboard 1
	  PlayFx        GENERIC\book_left.fx  0 Constant
	TEnd
MEnd


#############################################################
## Book Research Low
#############################################################

Move Emote_Book_research_Low
	Requires	EMOTE BOOKRESEARCH LOW
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Book_research_cycle_low
	Priority	2
	Interpolate	8
	Scale		0.5

	Type		Beast
	  Anim          male\blackboard_low 1
	  PlayFx        GENERIC\book_left.fx  0 Constant
	TEnd
MEnd

Move Emote_Book_research_cycle_low
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0
	Scale		0.5

	Type		Beast
	  Anim          male\blackboard_low 1
	  PlayFx        GENERIC\book_left.fx  0 Constant
	TEnd
MEnd


#############################################################
## Float Books
#############################################################

Move Emote_Float_Books
	Requires	EMOTE FLOATBOOKS
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Float_Books_cycle
	Priority	2
	Scale		0.5

	Type		Beast
	  Anim          male\idle_2hips_look 1
	  PlayFx        GENERIC\cot_books.fx  0 Constant
	TEnd

MEnd

Move Emote_Float_Books_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Scale		0.5

	Type		Beast
	  Anim          male\idle_2hips_look 1
	  PlayFx        GENERIC\cot_books.fx  0 Constant
	TEnd
MEnd


#############################################################
## Hold Torch
#############################################################

Move Emote_Hold_Torch
	Requires	EMOTE HOLDTORCH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Hold_Torch_cycle
	Priority	2
	Scale		0.5

	Type		Beast
	  Anim          male\holdtorch 1
	  PlayFx        Generic\Torch_left.fx  0 Constant
	TEnd
MEnd

Move Emote_Hold_Torch_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Scale		0.5

	Type		Beast
	  Anim          male\holdtorch 1
	  PlayFx        Generic\Torch_left.fx  0 Constant
	TEnd
MEnd


#############################################################
## Drum
#############################################################

Move Emote_Drum
	Requires	EMOTE DRUM
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Drum_cycle
	Priority	2
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\drum_high 1
	  PlayFx        GENERIC\huge_drum_big.fx  0 Constant
	  PlayFx        GENERIC\drum_sticks_bones.fx  0 Constant
	  PlayFx        FEET\Lowdrum.fx  1
	  PlayFx        FEET\Lowdrum.fx  30
	TEnd
MEnd

Move Emote_Drum_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
	Priority	0

	Type		Beast
	  Anim          male\drum_high 1
	  PlayFx        GENERIC\huge_drum_big.fx  0 Constant
	  PlayFx        GENERIC\drum_sticks_bones.fx  0 Constant
	  PlayFx        FEET\Lowdrum.fx  1
	  PlayFx        FEET\Lowdrum.fx  30
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
	Priority	1
	Interpolate	8
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\tribal_dance 1
	TEnd
MEnd

Move Emote_Tribal_Dance_cycle_1
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
	CycleMove	Emote_Tribal_Dance
	CycleMove	Emote_Tribal_Dance_cycle_1
	Priority	0

	Type		Beast
	  Anim          male\tribal_dance_alt 1
	TEnd
MEnd


#############################################################
## Assume Position
#############################################################

Move Emote_Assume_Position
	Requires	EMOTE ASSUMEPOSITION
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Assume_Position_cycle
	Priority	2

	Type		Beast
	  Anim          male\AssumePosition_2 1
	TEnd
MEnd

Move Emote_Assume_Position_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0

	Type		Beast
	  Anim          male\AssumePosition_2 1
	TEnd
MEnd

#############################################################
## "Bat Smash"
#############################################################

Move Emote_Bat_smash
	Requires	EMOTE BATSMASH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Bat_smash_cycle
	Priority	1
	Flags		NoSizeScale

	Type		beast
	  Anim          male\bat_smash_simple 1 47
	  PlayFx        GENERIC\meatbone.fx 0 Constant
	TEnd
MEnd

Move Emote_Bat_smash_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
	Priority	0

	Type		beast
	  Anim          male\bat_smash_simple 1 47
	  PlayFx        GENERIC\meatbone.fx 0 Constant
	TEnd
MEnd


#############################################################
## "Bat Smash" React
#############################################################

Move Emote_Bat_smash_react
	Requires	EMOTE BATSMASHREACT
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Bat_smash_react_cycle
	Priority	1
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\bat_smash_react 1 47
	TEnd
MEnd

Move Emote_Bat_smash_react_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
	Priority	0

	Type		Beast
	  Anim          male\bat_smash_react 1 47
	TEnd
MEnd


#############################################################
## Protest
#############################################################

Move Emote_Protest
	Requires	EMOTE PROTEST
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Protest_cycle
	Priority	1
	Interpolate	8
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\sign_onearm_chant 1
	  PlayFx        GENERIC\Protest_sign_A.fx  0 Constant
	TEnd
MEnd

Move Emote_Protest_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
	Priority	0

	Type		Beast
	  Anim          male\sign_onearm_chant 1
	  PlayFx        GENERIC\Protest_sign_A.fx  0 Constant
	TEnd
MEnd


#############################################################
## Fear
#############################################################

Move Emote_Fear
	Requires	EMOTE FEAR
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Fear_cycle
	Priority	1

	Type		Beast
	  Anim          male\fear_stun 1
	TEnd
MEnd

Move Emote_Fear_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0

	Type		Beast
	  Anim          male\fear_stun 25 60
	TEnd
MEnd


#############################################################
## Slap React
#############################################################

Move Emote_Slap_React
	Requires	EMOTE SLAPREACT
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Slap_React_2
	Priority	1
	Interpolate	15
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\combat_block 1 22
	TEnd
MEnd

Move Emote_Slap_React_2
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NoSizeScale
	NextMove	Emote_Slap_React_cycle
	Priority	0

	Type		Beast
	  Anim          male\stun_hit 2 96
	TEnd
MEnd

Move Emote_Slap_React_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	0
	Scale           0.5

	Type		Beast
	  Anim          male\stun_cycle 1 31
	TEnd
MEnd

#############################################################
## "Ala Kazam!"
#############################################################

Move Emote_AlaKazam
	Requires	EMOTE ALAKAZAM
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	postReady_back
	Priority	2
	Interpolate	15
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\magic_L 1 ##30
	TEnd
MEnd


#############################################################
## "Ala Kazam!" Reaction
#############################################################

Move Emote_AlaKazam_React
	Requires	EMOTE ALAKAZAMREACT
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_AlaKazam_React_hidden
	Priority	1
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\freezestun 1 20
	  PlayFX	Emotes\CostumeChanges\PrestoChango\PrestoChango.fx 5
	  PlayFX	GENERIC\AlakazamReact.fx 8 CONSTANT
	  PlayFx	Generic\entityVanish.fx 6 CONSTANT
	TEnd
MEnd

Move Emote_AlaKazam_React_hidden
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_AlaKazam_React_cycle
	Priority	0
	Scale		0.5
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\freezestun 20
	  PlayFX	GENERIC\AlakazamReact.fx 0 CONSTANT
	  PlayFx	Generic\entityVanish.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_AlaKazam_React_cycle
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_What
	Priority	0
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\lookquick 1
	  PlayFX	Emotes\CostumeChanges\PrestoChango\PrestoChango.fx 0
	TEnd
MEnd


#############################################################
## Talker
#############################################################

Move Emote_Talker
	Requires	EMOTE GESTICULATE
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Talker_cycle_A
	Priority	2

	Type		Beast
	  Anim          male\PL_Talk01 1
	TEnd
MEnd

Move Emote_Talker_cycle_A
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	CycleMove	Emote_Talker_cycle_A
	CycleMove	Emote_Talker_cycle_B
	CycleMove	Emote_Talker_cycle_C
	CycleMove	Emote_Talker_cycle_A
	Priority	0

	Type		Beast
	  Anim          male\PL_TalkIdle 1
	TEnd
MEnd

Move Emote_Talker_cycle_B
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	CycleMove	Emote_Talker_cycle_C
	CycleMove	Emote_Talker_cycle_D
	CycleMove	Emote_Talker_cycle_A
	Priority	0

	Type		Beast
	  Anim          male\PL_Talk01 1
	TEnd
MEnd

Move Emote_Talker_cycle_C
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	CycleMove	Emote_Talker_cycle_B
	CycleMove	Emote_Talker_cycle_A
	CycleMove	Emote_Talker_cycle_D
	Priority	0

	Type		Beast
	  Anim          male\PL_Talk02 1
	TEnd
MEnd

Move Emote_Talker_cycle_D
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	CycleMove	Emote_Talker_cycle_A
	CycleMove	Emote_Talker_cycle_C
	CycleMove	Emote_Talker_cycle_B
	Priority	0

	Type		Beast
	  Anim          male\PL_Talk03 1
	TEnd
MEnd


#############################################################
## Yata Yata
#############################################################

Move Emote_YataYata
	Requires	EMOTE YATA
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Ready
	Priority	3
	Scale		1.1

	Type		Beast
	  Anim          male\YataYata 1
	TEnd
MEnd

#############################################################
## Laugh Two
#############################################################

Move Emote_LAUGHTWO
	Requires	EMOTE LAUGHTWO
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Ready
	Interpolate	3

	Type		Beast
	  Anim          male\emote_Laugh2 1
	TEnd
MEnd

#############################################################
## Villain Laugh
#############################################################

Move Emote_VillainLaugh
	Requires	EMOTE VILLAINLAUGH
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Ready
	Interpolate	3

	Type		Beast
	  Anim          male\emote_VillainLaugh 1
	TEnd
MEnd

#############################################################
## Curse You
#############################################################

Move Emote_CurseYou
	Requires	EMOTE CURSEYOU
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Ready
	Interpolate	4

	Type		Beast
	  Anim          male\emote_CurseYou 1 80
	TEnd
MEnd


#############################################################
## Sleep
#############################################################

Move Emote_Sleep
	Requires	EMOTE ASLEEP
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_SleepLoop
	Priority	2
	Interpolate	7
	scale		0.9

	Type		Beast
	  Anim          male\emote_sleep 15 95
	TEnd
MEnd

Move Emote_SleepLoop
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Interpolate     4

	Type		Beast
	  Anim          male\emote_Sleep 96 191
	  PlayFX	V_COV\Items\Emote_SleepZ.fx 0 Constant
	TEnd
MEnd

#############################################################
## Dust Off
#############################################################

Move Emote_DustOff
	Requires	EMOTE DUSTOFF
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_DustOff2
	Priority	2
	Flags		FixArmReg
	Scale		1.1

	Type		Beast
	  Anim          huge\emote_DustOff 1 70
	TEnd
MEnd

Move Emote_DustOff2
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Ready
	Scale		1.1

	Type		Beast
	  Anim          huge\emote_DustOff 71
	TEnd
MEnd


#############################################################
##  Plotting
#############################################################

Move Emote_Plotting
	Requires	EMOTE PLOTTING
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Plotting_Cycle
	Flags		FixArmReg

	Type		Beast
	  Anim          huge\contemplate 1 20
	TEnd
MEnd


Move Emote_Plotting_Cycle
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Idle1_Cycle
	Flags		FIXARMREG Cycle
	Interpolate	5

	Type		Beast
	  Anim          huge\contemplate 20 80
	TEnd

MEnd

#############################################################
## Chicken
#############################################################

Move Emote_Chicken
	Requires	EMOTE CHICKEN
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Ready_Chicken
	Priority	2
	Interpolate	5

	Type		Beast
	  Anim          huge\emote_Chicken 1
	TEnd
MEnd

Move Ready_Chicken
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	NextMove	Ready_2Hips_look
	Priority	1
	Scale		0.5
	Interpolate	15

	Type		Beast
	  Anim          huge\stand_2Hhips 40 64
	TEnd
MEnd


#############################################################
## Laptop
#############################################################

Move Emote_Laptop
	Requires	EMOTE LAPTOP
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Laptop_cycle_1
	Priority	2
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\Portable_Laptop_Rest 1
	  PlayFX	Generic\huge_Stand_LaptopTable.fx 0 Constant
	TEnd
MEnd

Move Emote_Laptop_cycle_1
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
	CycleMove	Emote_Laptop
	CycleMove	Emote_Laptop_cycle_1
	CycleMove	Emote_Laptop_cycle_2

	Type		Beast
	  Anim          male\Portable_Laptop_Rest 1
	  PlayFX	Generic\huge_Stand_LaptopTable.fx 0 Constant
	TEnd
MEnd

Move Emote_Laptop_cycle_2
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
	CycleMove	Emote_Laptop
	CycleMove	Emote_Laptop_cycle_1

	Type		Beast
	  Anim          male\Portable_Laptop_Rest 1
	  PlayFX	Generic\huge_Stand_LaptopTable.fx 0 Constant
	TEnd
MEnd


#############################################################
## Typing - No FX geometry, just animation
#############################################################

Move Emote_LaptopType
	Requires	EMOTE TYPING
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_LaptopType02
	Priority	2
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\emote_LaptopType 1
	TEnd
MEnd

Move Emote_LaptopType02
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle FixArmReg NoSizeScale
	CycleMove	Emote_LaptopRest
	CycleMove	Emote_LaptopRest
	CycleMove	Emote_LaptopType02
	Priority	3
	Interpolate	0

	Type		Beast
	  Anim          male\emote_LaptopType 1
	TEnd
MEnd

Move Emote_LaptopRest
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
	CycleMove	Emote_LaptopRest
	CycleMove	Emote_LaptopType02
	CycleMove	Emote_LaptopType02
	CycleMove	Emote_LaptopDelete
	Priority	3
	Interpolate	0

	Type		Beast
	  Anim          male\emote_LaptopRest 1
	TEnd
MEnd

Move Emote_LaptopDelete
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
	CycleMove	Emote_LaptopRest
	CycleMove	Emote_LaptopRest
	CycleMove	Emote_LaptopType02
	Priority	3
	Interpolate	0

	Type		Beast
	  Anim          male\emote_LaptopDelete 1
	TEnd
MEnd

#############################################################
## Touch Screen
#############################################################

Move Emote_ScreenIdle
	Requires	EMOTE SCREEN
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_ScreenIdleLoop
	Priority	2
	Interpolate	15
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\emote_TouchScreen 1 117
	TEnd
MEnd

Move Emote_ScreenIdleLoop
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
	CycleMove	Emote_TouchScreenHit
	CycleMove	Emote_TouchScreen
	Priority	3
	Interpolate	0

	Type		Beast
	  Anim          male\emote_TouchScreen 110
	TEnd
MEnd

Move Emote_TouchScreen
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
	CycleMove	Emote_ScreenIdleLoop
	CycleMove	Emote_ScreenIdleLoop
	CycleMove	Emote_TouchScreenHit
	Priority	3
	Interpolate	0

	Type		Beast
	  Anim          male\emote_TouchScreen 1 117
	TEnd
MEnd

Move Emote_TouchScreenHit
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
	CycleMove	Emote_ScreenIdleLoop
	CycleMove	Emote_TouchScreen
	Priority	3
	Interpolate	0

	Type		Beast
	  Anim          male\emote_TouchScreenHit 1
	TEnd
MEnd


#############################################################
## Go Away
#############################################################

Move Emote_GoAway
	Requires	EMOTE GOAWAY
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Nextmove	Ready_2Hips ## looks good enough on fem
	Priority	2

	Type		Beast
	  Anim          male\Emote_GoAway 1
	TEnd
MEnd


#############################################################
## Talk to the hand
#############################################################

Move Emote_TalktoHand
	Requires	EMOTE TALKTOHAND
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Nextmove	Ready_2Hips ## looks good enough on fem
	Priority	2
	Scale		0.8

	Type		Beast
	  Anim          male\Emote_TalktoHand 1
	TEnd
MEnd


#############################################################
## Kneel before ZOD
#############################################################

Move Emote_Bow_Down
	Requires	EMOTE BOWDOWN
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Nextmove	Ready_2Hips ## looks good enough on fem
	Priority	2
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
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Nextmove	Ready_2Hips ## looks good enough on fem
	Priority	2

	Type		Beast
	  Anim          male\Emote_Slap 1
	  Playfx	generic\Slap1.fx
	TEnd
MEnd


#############################################################
## Backhand Smack
#############################################################

Move Emote_Smack
	Requires	EMOTE SMACK
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Nextmove	Ready_2Hips ## looks good enough on fem
	Priority	2

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
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Nextmove	Ready_2Hips ## looks good enough on fem
	Priority	2

	Type		Beast
	  Anim          male\Emote_Stronghand 1
	TEnd
MEnd


#############################################################
## Backflip
#############################################################

Move Emote_Backflip
	Requires	EMOTE BACKFLIP
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Nextmove	Ready_2Hips ## looks good enough on fem
	Priority	2
	Interpolate	5

	Type		Beast
	  Anim          male\Emote_Backflip 1
	TEnd
MEnd


#############################################################
## Kiss my Butt
#############################################################

Move Emote_KissMyButt
	Requires	EMOTE KISSMYBUTT
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Nextmove	Ready_2Hips ## looks good enough on fem
	Priority	2

	Type		Beast
	  Anim          male\Emote_KissMyButt 1
	TEnd
MEnd


#############################################################
## Crack Knuckles
#############################################################

Move Emote_CrackKnuckles
	Requires	EMOTE KNUCKLECRACK
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Nextmove	Ready_2Hips
	Flags		FixArmReg
	Priority	2

	Type		Beast
	  Anim          huge\emote_knuckle_crack 1  80
	  playfx	V_COV\Items\Emote_knuckles.fx
	TEnd
MEnd

#############################################################
## Wedding - Marriage Proposal
#############################################################

Move Emote_MarriageProposal_Pre
	Requires	EMOTE PROPOSE
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
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
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Ready

	Type		male
	  Anim          male\emote_UnderhandToss 2
	  PlayFx        Emotes\ThrowRice.fx 30 Constant
	TEnd

	Type		fem
	  Anim          fem\emote_UnderhandToss 2
	  PlayFx        Emotes\ThrowRice.fx 28 Constant
	TEnd
MEnd


#############################################################
## Wedding - Throw Confetti
#############################################################

Move Emote_ThrowConfetti
	Requires	EMOTE UNDERHANDTOSS TWO
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Ready

	Type		male
	  Anim          male\emote_UnderhandToss 2
	  PlayFx        Emotes\ThrowConfetti.fx 31 Constant
	TEnd

	Type		fem
	  Anim          fem\emote_UnderhandToss 2
	  PlayFx        Emotes\ThrowConfetti.fx 28 Constant
	TEnd
MEnd


#############################################################
## Wedding - Throw Rose Petals
#############################################################

Move Emote_ThrowRosePetals
	Requires	EMOTE UNDERHANDTOSS THREE
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Ready

	Type		male
	  Anim          male\emote_UnderhandToss 2
	  PlayFx        Emotes\ThrowRosePetals.fx 30 Constant
	TEnd

	Type		fem
	  Anim          fem\emote_UnderhandToss 2
	  PlayFx        Emotes\ThrowRosePetals.fx 28 Constant
	TEnd
MEnd


#############################################################
## Robot Dance
#############################################################

Move DanceRobot_Pre
	Requires	EMOTE DANCE LOW CYBORGPACK
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
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
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
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
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
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
	Flags		FIXARMREG NoSizeScale

	Type		male
	  Anim          male\PraetorianSalute 1 65
	TEnd
MEnd

Move Emote_PratorianSalute_PostCycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG NoSizeScale
	Scale		0.75

	Type		male
	  Anim          male\PraetorianSalute 65 125
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
	Flags		NoSizeScale

	Type		male
	  Anim          male\spray_lookquick 1
	  PlayFX	Generic\spraycan.fx 0 Constant
	TEnd
MEnd

Move Emote_Spraypaint_LookQuick
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
	CycleMove       Emote_Spraypaint_ShakeCan

	Type		male
	  Anim          male\spray_lookquick 1
	  PlayFX	Generic\spraycan.fx 0 Constant
	TEnd
MEnd

Move  Emote_Spraypaint_ShakeCan
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
	CycleMove       Emote_Spraypaint_Start

	Type		male
	  Anim          male\spray_shakecan 1
	  PlayFX	Generic\spraycan.fx 0 Constant
	  PlayFX	Generic\spraycan_shake_SFX.fx 5
	TEnd
MEnd

Move  Emote_Spraypaint_Start
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NoSizeScale
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
	Flags		Cycle NoSizeScale
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

Move Emote_Boast
	Requires	EMOTE BOAST
	Priority	2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Ready

	Type		male
	  Anim          male\emote_boast 1
	TEnd
MEnd

Move A_Emote_Boast
	Requires	FLY EMOTE BOAST
	Priority	13
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> #<IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	FlyReady

	Type		male
	  Anim          male\air_emote_boast 1
	TEnd
MEnd

## Growl ###########################################################

Move Emote_Mutant_Growl_Pre
	Requires	EMOTE GROWL
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	CombatReady #Scream_Post
	Priority	2
	Flags		NoSizeScale
	Scale		0.625

	Type		Beast
	  Anim          huge\scream 1 #10 58
	  PlayFX	Emotes\PlayerHowl.fx 5
	TEnd

MEnd

#############################################################
## Stance - Heroic 1
#############################################################

Move Emote_Stance_Heroic01
	Requires	EMOTE POSE GOOD ONE
	Priority	4
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Member		<DeathIRQ> <AttackIRQ>  <Jumps> <MoveIRQ> <ReactIRQ> <HitIRQ> <IRQEmotesALL>
	NextMove	Emote_Stance_Heroic01_Cycle
	Scale		0.5
	Interpolate	5
	Flags		FIXARMREG

	Type		male
	  Anim          male\Stance_Heroic01 1
	TEnd

	Type		fem
	  Anim          fem\Stance_Heroic01 1
	TEnd
MEnd

Move Emote_Stance_Heroic01_Cycle
	Member		<DeathIRQ> <AttackIRQ>  <Jumps> <MoveIRQ> <ReactIRQ> <HitIRQ> <IRQEmotesALL>
	Scale		0.5
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\Stance_Heroic01 1
	TEnd

	Type		fem
	  Anim          fem\Stance_Heroic01 1
	TEnd
MEnd

#############################################################

Move A_Emote_Stance_Heroic01
	Requires	FLY EMOTE POSE GOOD ONE
	Priority	5
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Member		<IRQEmotesALL> <DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ> #<FlyReady> #<Block>
	NextMove	A_Emote_Stance_Heroic01_Cycle
	Scale		0.5
	Interpolate	5
	Flags		FIXARMREG

	Type		male
	  Anim          male\air_Stance_Heroic01 1
	TEnd

	Type		fem
	  Anim          fem\air_Stance_Heroic01 1
	TEnd

	Type		huge
	  Anim          huge\air_Stance_Heroic01 1
	TEnd
MEnd

Move A_Emote_Stance_Heroic01_Cycle
	Requires	FLY
	Priority        1
	Member		<IRQEmotesALL> <DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ> #<FlyReady> #<Block>
	NextMove	PreFall
	Scale		0.5
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\air_Stance_Heroic01 1
	TEnd

	Type		fem
	  Anim          fem\air_Stance_Heroic01 1
	TEnd

	Type		huge
	  Anim          huge\air_Stance_Heroic01 1
	TEnd
MEnd

#############################################################
## Stance - Heroic 2
#############################################################

Move Emote_Stance_Heroic02
	Requires	EMOTE POSE GOOD TWO
	Priority	4
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Member		<DeathIRQ> <AttackIRQ>  <Jumps> <MoveIRQ> <ReactIRQ> <HitIRQ> <IRQEmotesALL>
	NextMove	Emote_Stance_Heroic02_Cycle
	Scale		0.5
	Interpolate	5
	Flags		FIXARMREG

	Type		male
	  Anim          male\Stance_Heroic02 1
	TEnd

	Type		fem
	  Anim          fem\Stance_Heroic02 1
	TEnd
MEnd

Move Emote_Stance_Heroic02_Cycle
	Member		<DeathIRQ> <AttackIRQ>  <Jumps> <MoveIRQ> <ReactIRQ> <HitIRQ> <IRQEmotesALL>
	Scale		0.5
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\Stance_Heroic02 1
	TEnd

	Type		fem
	  Anim          fem\Stance_Heroic02 1
	TEnd
MEnd

#############################################################

Move A_Emote_Stance_Heroic02
	Requires	FLY EMOTE POSE GOOD TWO
	Priority	5
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Member		<IRQEmotesALL> <DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ> #<FlyReady> #<Block>
	NextMove	A_Emote_Stance_Heroic02_Cycle
	Scale		0.5
	Interpolate	5
	Flags		FIXARMREG

	Type		male
	  Anim          male\air_Stance_Heroic02 1
	TEnd
MEnd

Move A_Emote_Stance_Heroic02_Cycle
	Requires	FLY
	Priority        1
	Member		<IRQEmotesALL> <DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ> #<FlyReady> #<Block>
	NextMove	PreFall
	Scale		0.5
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\air_Stance_Heroic02 1
	TEnd
MEnd


#############################################################
## Stance - Villainous 1
#############################################################

Move Emote_Stance_Villainous01
	Requires	EMOTE POSE EVIL ONE
	Priority	4
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Member		<DeathIRQ> <AttackIRQ>  <Jumps> <MoveIRQ> <ReactIRQ> <HitIRQ> <IRQEmotesALL>
	NextMove	Emote_Stance_Villainous01_Cycle
	Scale		0.75
	Interpolate	5
	Flags		FIXARMREG

	Type		male
	  Anim          male\Stance_Villainous01 1
	TEnd
MEnd

Move Emote_Stance_Villainous01_Cycle
	Member		<DeathIRQ> <AttackIRQ>  <Jumps> <MoveIRQ> <ReactIRQ> <HitIRQ> <IRQEmotesALL>
	Scale		0.75
	NextMove	PreFall
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\Stance_Villainous01 1
	TEnd
MEnd

#############################################################

Move A_Emote_Stance_Omega
	Requires	FLY EMOTE POSE EVIL ONE
	Priority	5
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Member		<IRQEmotesALL> <DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ> #<FlyReady> #<Block>
	NextMove	A_Emote_Stance_Omega_Cycle
	Scale		0.625
	Interpolate	5
	Flags		FIXARMREG

	Type		male
	  Anim          male\air_Stance_Villainous01 1
	TEnd
MEnd

Move A_Emote_Stance_Omega_Cycle
	Requires	FLY
	Priority        1
	Member		<IRQEmotesALL> <DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ> #<FlyReady> #<Block>
	NextMove	PreFall
	Scale		0.625
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\air_Stance_Villainous01 1
	TEnd
MEnd

#############################################################
## Stance - Villainous 2
#############################################################

Move Emote_Stance_Villainous02
	Requires	EMOTE POSE EVIL TWO
	Priority	4
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Member		<DeathIRQ> <AttackIRQ>  <Jumps> <MoveIRQ> <ReactIRQ> <HitIRQ> <IRQEmotesALL>
	NextMove	Emote_Stance_Villainous02_Cycle
	Scale		0.5
	Interpolate	5
	Flags		FIXARMREG

	Type		male
	  Anim          male\Stance_Villainous02 1
	TEnd

	Type		fem
	  Anim          fem\Stance_Villainous02 1
	TEnd

	Type		huge
	  Anim          huge\Stance_Villainous02 1
	TEnd
MEnd

Move Emote_Stance_Villainous02_Cycle
	Member		<DeathIRQ> <AttackIRQ>  <Jumps> <MoveIRQ> <ReactIRQ> <HitIRQ> <IRQEmotesALL>
	Scale		0.5
	NextMove	PreFall
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\Stance_Villainous02 1
	TEnd

	Type		fem
	  Anim          fem\Stance_Villainous02 1
	TEnd

	Type		huge
	  Anim          huge\Stance_Villainous02 1
	TEnd
MEnd

#############################################################

Move A_Emote_Stance_Villainous02
	Requires	FLY EMOTE POSE EVIL TWO
	Priority	5
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Member		<IRQEmotesALL> <DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ> #<FlyReady> #<Block>
	NextMove	A_Emote_Stance_Villainous02_Cycle
	Scale		0.625
	Interpolate	5
	Flags		FIXARMREG

	Type		male
	  Anim          male\air_Stance_Villainous02 1
	TEnd

	Type		fem
	  Anim          fem\air_Stance_Villainous02 1
	TEnd

	Type		huge
	  Anim          huge\air_Stance_Villainous02 1
	TEnd
MEnd

Move A_Emote_Stance_Villainous02_Cycle
	Requires	FLY
	Priority        1
	Member		<IRQEmotesALL> <DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ> #<FlyReady> #<Block>
	NextMove	PreFall
	Scale		0.625
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\air_Stance_Villainous02 1
	TEnd

	Type		fem
	  Anim          fem\air_Stance_Villainous02 1
	TEnd

	Type		huge
	  Anim          huge\air_Stance_Villainous02 1
	TEnd
MEnd

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

## Karate Dance ###########################################################

Move Emote_Karate_Dance_Pre
	Requires	EMOTE DANCE EIGHT
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	Nextmove	Emote_Karate_Dance_one
	Flags		NoSizeScale pickrandomly
	Sets            CANTMOVE

	Type		male
	  Anim          male\emote_karate_dance 1 11
	TEnd
MEnd

Move Emote_Karate_Dance_one
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Karate_Dance_one
	CycleMove	Emote_Karate_Dance_one
	CycleMove	Emote_Karate_Dance_cycle_post
	NextMove	Emote_Karate_Dance_cycle_post
	Flags		NoSizeScale Cycle


	Type		male
	  Anim          male\emote_karate_dance 11 50
	  # PlayFX		NPCFX\Dance_ClothShort_SFX.fx 11
	  # PlayFX		NPCFX\Dance_ClothLong_SFX.fx 22
	  # PlayFX		NPCFX\Dance_ClothShort_SFX.fx 47
	  # PlayFX		NPCFX\Dance_ClothLong_SFX.fx 61
	  # PlayFX		NPCFX\Dance_ClothShort_SFX.fx 82
	  # PlayFX		NPCFX\Dance_ClothLong_SFX.fx 96
	  # PlayFX		NPCFX\Dance_ClothShort_SFX.fx 117
	TEnd
MEnd

Move Emote_Karate_Dance_cycle_post
	Include sequencers\Members\Emotes_Idle.inc
	Cyclemove	Emote_Karate_Dance_cycle_two
	Nextmove	Emote_Karate_Dance_cycle_two
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_karate_dance 50 156
	TEnd
MEnd

Move Emote_Karate_Dance_cycle_two
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Karate_Dance_cycle_two
	CycleMove	Emote_Karate_Dance_post
	NextMove	Emote_Karate_Dance_post
	Flags		NoSizeScale Cycle


	Type		male
	  Anim          male\emote_karate_dance 156 215
	TEnd
MEnd

Move Emote_Karate_Dance_post
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Karate_Dance_one
	CycleMove	Emote_Karate_Dance_Post_kick
	NextMove	Emote_Karate_Dance_Post_kick
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_karate_dance 215 225
	TEnd
MEnd

Move Emote_Karate_Dance_Post_kick
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Karate_Dance_one
	CycleMove	Emote_Karate_Dance_Post_kick
	NextMove	Emote_Karate_Dance_one
	Flags		NoSizeScale
	scale		1.2

	Type		male
	  Anim          male\emote_karate_dance 225 294
	TEnd
MEnd

## Karate Dance ###########################################################

Move A_Emote_Karate_Dance_Pre
	Requires	FLY EMOTE DANCE EIGHT
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	Nextmove	A_Emote_Karate_Dance_one
	Flags		NoSizeScale pickrandomly
	Sets            CANTMOVE

	Type		male
	  Anim          male\emote_karate_dance 1 11
	TEnd
MEnd

Move A_Emote_Karate_Dance_one
	Requires	FLY
	Priority	1
	CycleMove	A_Emote_Karate_Dance_one
	CycleMove	A_Emote_Karate_Dance_one
	CycleMove	A_Emote_Karate_Dance_cycle_post
	NextMove	A_Emote_Karate_Dance_cycle_post
	Flags		NoSizeScale Cycle


	Type		male
	  Anim          male\emote_karate_dance 11 50
	  # PlayFX		NPCFX\Dance_ClothShort_SFX.fx 11
	  # PlayFX		NPCFX\Dance_ClothLong_SFX.fx 22
	  # PlayFX		NPCFX\Dance_ClothShort_SFX.fx 47
	  # PlayFX		NPCFX\Dance_ClothLong_SFX.fx 61
	  # PlayFX		NPCFX\Dance_ClothShort_SFX.fx 82
	  # PlayFX		NPCFX\Dance_ClothLong_SFX.fx 96
	  # PlayFX		NPCFX\Dance_ClothShort_SFX.fx 117
	TEnd
MEnd

Move A_Emote_Karate_Dance_cycle_post
	Requires	FLY
	Priority	1
	Cyclemove	A_Emote_Karate_Dance_cycle_two
	Nextmove	A_Emote_Karate_Dance_cycle_two
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_karate_dance 50 156
	TEnd
MEnd

Move A_Emote_Karate_Dance_cycle_two
	Requires	FLY
	Priority	1
	CycleMove	A_Emote_Karate_Dance_cycle_two
	CycleMove	A_Emote_Karate_Dance_post
	NextMove	A_Emote_Karate_Dance_post
	Flags		NoSizeScale Cycle


	Type		male
	  Anim          male\emote_karate_dance 156 215
	TEnd
MEnd

Move A_Emote_Karate_Dance_post
	Requires	FLY
	Priority	1
	CycleMove	A_Emote_Karate_Dance_one
	CycleMove	A_Emote_Karate_Dance_Post_kick
	NextMove	A_Emote_Karate_Dance_Post_kick
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_karate_dance 215 225
	TEnd
MEnd

Move A_Emote_Karate_Dance_Post_kick
	Requires	FLY
	Priority	1
	CycleMove	A_Emote_Karate_Dance_one
	CycleMove	A_Emote_Karate_Dance_Post_kick
	NextMove	A_Emote_Karate_Dance_one
	Flags		NoSizeScale
	scale		1.2

	Type		male
	  Anim          male\emote_karate_dance 225 294
	TEnd
MEnd

#############################################################
## Billy Jeans Dance ###########################################################

Move Emote_Dance_BillyJeans_Pre
	Requires	EMOTE DANCE NINE
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	Nextmove	Emote_Dance_BillyJeans
	Flags		NoSizeScale pickrandomly
	Sets            CANTMOVE

	Type		male
	  Anim          male\emote_dance_billyjeans 1 2
	TEnd

	Type		fem
	  Anim          fem\emote_dance_billyjeans 1 2
	TEnd

	Type		Huge
	  Anim          huge\emote_dance_billyjeans 1 2
	TEnd
MEnd

Move Emote_Dance_BillyJeans
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Dance_BillyJeans
	Flags		NoSizeScale Cycle

	Type		male
	  Anim          male\emote_dance_billyjeans 2
	  PlayFX		NPCFX\Dance_Cloth.fx 3
	  PlayFX		NPCFX\Dance_Feet.fx 14
	  PlayFX		NPCFX\Dance_Feet.fx 19
	  PlayFX		NPCFX\Dance_ClothLong_SFX.fx 41
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 52
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 74
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 100
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 124
	  PlayFX		NPCFX\Dance_Scuff.fx 145
	  PlayFX		NPCFX\Dance_Cloth.fx 161
	  PlayFX		NPCFX\Dance_Scuff.fx 177
	  PlayFX		NPCFX\Dance_Cloth.fx 219
	  PlayFX		NPCFX\Dance_Feet.fx 244
	  PlayFX		NPCFX\Dance_Feet.fx 249
	  PlayFX		NPCFX\Dance_Cloth.fx 268
	  PlayFX		NPCFX\Dance_Cloth.fx 271
	  PlayFX		NPCFX\Dance_Feet.fx 279
	  PlayFX		NPCFX\Dance_Feet.fx 281
	  PlayFX		NPCFX\Dance_Feet.fx 293
	  PlayFX		NPCFX\Dance_Feet.fx 295
	  PlayFX		NPCFX\Dance_Scuff.fx 328
	  PlayFX		NPCFX\Dance_Scuff.fx 351
	  PlayFX		NPCFX\Dance_Scuff.fx 367
	  PlayFX		NPCFX\Dance_Scuff.fx 387
	  PlayFX		NPCFX\Dance_Scuff.fx 406
	  PlayFX		NPCFX\Dance_Cloth.fx 407
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 423
	  PlayFX		NPCFX\Dance_Feet.fx 430
	  PlayFX		NPCFX\Dance_Feet.fx 432
	TEnd

	Type		fem
	  Anim          fem\emote_dance_billyjeans 2
	  PlayFX		NPCFX\Dance_Cloth.fx 3
	  PlayFX		NPCFX\Dance_Feet.fx 14
	  PlayFX		NPCFX\Dance_Feet.fx 19
	  PlayFX		NPCFX\Dance_ClothLong_SFX.fx 41
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 52
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 74
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 100
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 124
	  PlayFX		NPCFX\Dance_Scuff.fx 145
	  PlayFX		NPCFX\Dance_Cloth.fx 161
	  PlayFX		NPCFX\Dance_Scuff.fx 177
	  PlayFX		NPCFX\Dance_Cloth.fx 219
	  PlayFX		NPCFX\Dance_Feet.fx 244
	  PlayFX		NPCFX\Dance_Feet.fx 249
	  PlayFX		NPCFX\Dance_Cloth.fx 268
	  PlayFX		NPCFX\Dance_Cloth.fx 271
	  PlayFX		NPCFX\Dance_Feet.fx 279
	  PlayFX		NPCFX\Dance_Feet.fx 281
	  PlayFX		NPCFX\Dance_Feet.fx 293
	  PlayFX		NPCFX\Dance_Feet.fx 295
	  PlayFX		NPCFX\Dance_Scuff.fx 328
	  PlayFX		NPCFX\Dance_Scuff.fx 351
	  PlayFX		NPCFX\Dance_Scuff.fx 367
	  PlayFX		NPCFX\Dance_Scuff.fx 387
	  PlayFX		NPCFX\Dance_Scuff.fx 406
	  PlayFX		NPCFX\Dance_Cloth.fx 407
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 423
	  PlayFX		NPCFX\Dance_Feet.fx 430
	  PlayFX		NPCFX\Dance_Feet.fx 432
	TEnd

	Type		Huge
	  Anim          huge\emote_dance_billyjeans 2
	  PlayFX		NPCFX\Dance_Cloth.fx 3
	  PlayFX		NPCFX\Dance_Feet.fx 14
	  PlayFX		NPCFX\Dance_Feet.fx 19
	  PlayFX		NPCFX\Dance_ClothLong_SFX.fx 41
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 52
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 74
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 100
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 124
	  PlayFX		NPCFX\Dance_Scuff.fx 145
	  PlayFX		NPCFX\Dance_Cloth.fx 161
	  PlayFX		NPCFX\Dance_Scuff.fx 177
	  PlayFX		NPCFX\Dance_Cloth.fx 219
	  PlayFX		NPCFX\Dance_Feet.fx 244
	  PlayFX		NPCFX\Dance_Feet.fx 249
	  PlayFX		NPCFX\Dance_Cloth.fx 268
	  PlayFX		NPCFX\Dance_Cloth.fx 271
	  PlayFX		NPCFX\Dance_Feet.fx 279
	  PlayFX		NPCFX\Dance_Feet.fx 281
	  PlayFX		NPCFX\Dance_Feet.fx 293
	  PlayFX		NPCFX\Dance_Feet.fx 295
	  PlayFX		NPCFX\Dance_Scuff.fx 328
	  PlayFX		NPCFX\Dance_Scuff.fx 351
	  PlayFX		NPCFX\Dance_Scuff.fx 367
	  PlayFX		NPCFX\Dance_Scuff.fx 387
	  PlayFX		NPCFX\Dance_Scuff.fx 406
	  PlayFX		NPCFX\Dance_Cloth.fx 407
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 423
	  PlayFX		NPCFX\Dance_Feet.fx 430
	  PlayFX		NPCFX\Dance_Feet.fx 432
	TEnd
MEnd

#############################################################
## Billy Jeans Dance ###########################################################

Move A_Emote_Dance_BillyJeans_Pre
	Requires	FLY EMOTE DANCE NINE
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	Nextmove	A_Emote_Dance_BillyJeans
	Flags		NoSizeScale pickrandomly
	Sets            CANTMOVE

	Type		male
	  Anim          male\emote_dance_billyjeans 1 2
	TEnd

	Type		fem
	  Anim          fem\emote_dance_billyjeans 1 2
	TEnd

	Type		Huge
	  Anim          huge\emote_dance_billyjeans 1 2
	TEnd
MEnd

Move A_Emote_Dance_BillyJeans
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	A_Emote_Dance_BillyJeans
	Flags		NoSizeScale Cycle

	Type		male
	  Anim          male\emote_dance_billyjeans 2
	  PlayFX		NPCFX\Dance_Cloth.fx 3
	  PlayFX		NPCFX\Dance_Feet.fx 14
	  PlayFX		NPCFX\Dance_Feet.fx 19
	  PlayFX		NPCFX\Dance_ClothLong_SFX.fx 41
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 52
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 74
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 100
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 124
	  PlayFX		NPCFX\Dance_Scuff.fx 145
	  PlayFX		NPCFX\Dance_Cloth.fx 161
	  PlayFX		NPCFX\Dance_Scuff.fx 177
	  PlayFX		NPCFX\Dance_Cloth.fx 219
	  PlayFX		NPCFX\Dance_Feet.fx 244
	  PlayFX		NPCFX\Dance_Feet.fx 249
	  PlayFX		NPCFX\Dance_Cloth.fx 268
	  PlayFX		NPCFX\Dance_Cloth.fx 271
	  PlayFX		NPCFX\Dance_Feet.fx 279
	  PlayFX		NPCFX\Dance_Feet.fx 281
	  PlayFX		NPCFX\Dance_Feet.fx 293
	  PlayFX		NPCFX\Dance_Feet.fx 295
	  PlayFX		NPCFX\Dance_Scuff.fx 328
	  PlayFX		NPCFX\Dance_Scuff.fx 351
	  PlayFX		NPCFX\Dance_Scuff.fx 367
	  PlayFX		NPCFX\Dance_Scuff.fx 387
	  PlayFX		NPCFX\Dance_Scuff.fx 406
	  PlayFX		NPCFX\Dance_Cloth.fx 407
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 423
	  PlayFX		NPCFX\Dance_Feet.fx 430
	  PlayFX		NPCFX\Dance_Feet.fx 432
	TEnd

	Type		fem
	  Anim          fem\emote_dance_billyjeans 2
	  PlayFX		NPCFX\Dance_Cloth.fx 3
	  PlayFX		NPCFX\Dance_Feet.fx 14
	  PlayFX		NPCFX\Dance_Feet.fx 19
	  PlayFX		NPCFX\Dance_ClothLong_SFX.fx 41
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 52
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 74
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 100
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 124
	  PlayFX		NPCFX\Dance_Scuff.fx 145
	  PlayFX		NPCFX\Dance_Cloth.fx 161
	  PlayFX		NPCFX\Dance_Scuff.fx 177
	  PlayFX		NPCFX\Dance_Cloth.fx 219
	  PlayFX		NPCFX\Dance_Feet.fx 244
	  PlayFX		NPCFX\Dance_Feet.fx 249
	  PlayFX		NPCFX\Dance_Cloth.fx 268
	  PlayFX		NPCFX\Dance_Cloth.fx 271
	  PlayFX		NPCFX\Dance_Feet.fx 279
	  PlayFX		NPCFX\Dance_Feet.fx 281
	  PlayFX		NPCFX\Dance_Feet.fx 293
	  PlayFX		NPCFX\Dance_Feet.fx 295
	  PlayFX		NPCFX\Dance_Scuff.fx 328
	  PlayFX		NPCFX\Dance_Scuff.fx 351
	  PlayFX		NPCFX\Dance_Scuff.fx 367
	  PlayFX		NPCFX\Dance_Scuff.fx 387
	  PlayFX		NPCFX\Dance_Scuff.fx 406
	  PlayFX		NPCFX\Dance_Cloth.fx 407
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 423
	  PlayFX		NPCFX\Dance_Feet.fx 430
	  PlayFX		NPCFX\Dance_Feet.fx 432
	TEnd

	Type		Huge
	  Anim          huge\emote_dance_billyjeans 2
	  PlayFX		NPCFX\Dance_Cloth.fx 3
	  PlayFX		NPCFX\Dance_Feet.fx 14
	  PlayFX		NPCFX\Dance_Feet.fx 19
	  PlayFX		NPCFX\Dance_ClothLong_SFX.fx 41
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 52
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 74
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 100
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 124
	  PlayFX		NPCFX\Dance_Scuff.fx 145
	  PlayFX		NPCFX\Dance_Cloth.fx 161
	  PlayFX		NPCFX\Dance_Scuff.fx 177
	  PlayFX		NPCFX\Dance_Cloth.fx 219
	  PlayFX		NPCFX\Dance_Feet.fx 244
	  PlayFX		NPCFX\Dance_Feet.fx 249
	  PlayFX		NPCFX\Dance_Cloth.fx 268
	  PlayFX		NPCFX\Dance_Cloth.fx 271
	  PlayFX		NPCFX\Dance_Feet.fx 279
	  PlayFX		NPCFX\Dance_Feet.fx 281
	  PlayFX		NPCFX\Dance_Feet.fx 293
	  PlayFX		NPCFX\Dance_Feet.fx 295
	  PlayFX		NPCFX\Dance_Scuff.fx 328
	  PlayFX		NPCFX\Dance_Scuff.fx 351
	  PlayFX		NPCFX\Dance_Scuff.fx 367
	  PlayFX		NPCFX\Dance_Scuff.fx 387
	  PlayFX		NPCFX\Dance_Scuff.fx 406
	  PlayFX		NPCFX\Dance_Cloth.fx 407
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 423
	  PlayFX		NPCFX\Dance_Feet.fx 430
	  PlayFX		NPCFX\Dance_Feet.fx 432
	TEnd
MEnd


###########################################################
## Disco Dance Emote ###########################################################
 ###########################################################

Move Emote_Disco_Dance_Pre
	Requires	EMOTE DANCE ELEVEN
	Priority        3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale
	NextMove	Emote_Disco_Dance

	Type		male
	  Anim          male\emote_disco_dance 1 7
	TEnd

	Type		fem
	  Anim          fem\emote_disco_dance 1 7
	TEnd

MEnd

Move Emote_Disco_Dance
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Disco_Dance_Cycle
	Flags		nosizescale

	Type		male
	  Anim          male\emote_disco_dance 8 192
	TEnd

	Type		fem
	  Anim          fem\emote_disco_dance 8 192
	TEnd

MEnd

Move Emote_Disco_Dance_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale Cycle
	Interpolate	0

	Type		male
	  Anim          male\emote_disco_dance 7 192
	TEnd

	Type		fem
	  Anim          fem\emote_disco_dance 7 192
	TEnd

MEnd

## AirGuitar Emote ###########################################################

Move Emote_Air_Guitar_Pre
	Requires	EMOTE DANCE TEN
	Priority        3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale
	NextMove	Emote_Air_Guitar_Cycle

	Type		male
	  Anim          male\emote_air_guitar_dance 1 22
	TEnd

MEnd

Move Emote_Air_Guitar_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale Cycle
	CycleMove	Emote_Air_Guitar_Cycle
	CycleMove	Emote_Air_Guitar_Cycle
	CycleMove	Emote_Air_Guitar_Cycle
	CycleMove	Emote_Air_Guitar_Cycle_HeadSwing

	Type		male
	  Anim          male\emote_air_guitar_dance 22 88
	TEnd

MEnd

Move Emote_Air_Guitar_Cycle_HeadSwing
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale Cycle
	Scale		1.1
	CycleMove	Emote_Air_Guitar_Cycle
	CycleMove	Emote_Air_Guitar_Cycle
	CycleMove	Emote_Air_Guitar_Cycle
	CycleMove	Emote_Air_Guitar_Cycle_HeadSwing



	Type		male
	  Anim          male\emote_air_guitar_dance 88 124
	TEnd

MEnd

 ###########################################################
## AIR Disco Dance Emote ###########################################################
 ###########################################################

Move A_Emote_Disco_Dance_Pre
	Requires	FLY EMOTE DANCE ELEVEN
	Priority        4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		nosizescale
	NextMove	A_Emote_Disco_Dance

	Type		male
	  Anim          male\emote_disco_dance 1 7
	TEnd

	Type		fem
	  Anim          fem\emote_disco_dance 1 7
	TEnd

MEnd

Move A_Emote_Disco_Dance
	Requires	FLY
	Priority        1
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Disco_Dance_Cycle
	Flags		nosizescale

	Type		male
	  Anim          male\emote_disco_dance 8 192
	TEnd

	Type		fem
	  Anim          fem\emote_disco_dance 8 192
	TEnd

MEnd

Move A_Emote_Disco_Dance_Cycle
	Requires	FLY
	Priority        1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		nosizescale Cycle
	Interpolate	0

	Type		male
	  Anim          male\emote_disco_dance 7 192
	TEnd

	Type		fem
	  Anim          fem\emote_disco_dance 7 192
	TEnd

MEnd

## Air AirGuitar Emote ###########################################################

Move A_Emote_Air_Guitar_Pre
	Requires	FLY EMOTE DANCE TEN
	Priority        4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		nosizescale
	NextMove	A_Emote_Air_Guitar_Cycle

	Type		male
	  Anim          male\emote_air_guitar_dance 1 22
	TEnd

MEnd

Move A_Emote_Air_Guitar_Cycle
	Requires	FLY
	Priority        1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		nosizescale Cycle
	CycleMove	A_Emote_Air_Guitar_Cycle
	CycleMove	A_Emote_Air_Guitar_Cycle
	CycleMove	A_Emote_Air_Guitar_Cycle
	CycleMove	A_Emote_Air_Guitar_Cycle_HeadSwing

	Type		male
	  Anim          male\emote_air_guitar_dance 22 88
	TEnd

MEnd

Move A_Emote_Air_Guitar_Cycle_HeadSwing
	Requires	FLY
	Priority        1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		nosizescale Cycle
	Scale		1.1
	CycleMove	A_Emote_Air_Guitar_Cycle
	CycleMove	A_Emote_Air_Guitar_Cycle
	CycleMove	A_Emote_Air_Guitar_Cycle
	CycleMove	A_Emote_Air_Guitar_Cycle_HeadSwing

	Type		male
	  Anim          male\emote_air_guitar_dance 88 124
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

MEnd

Move Emote_Vomit_Post_Two
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Vomit_To_Ready_Two
	Flags		FixArmReg NoSizeScale

	Type		male
	  Anim          male\barf 60 139
	TEnd

MEnd

Move Emote_Vomit_To_Ready_Two
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle FixArmReg NoSizeScale
	NextMove        postReady_back

	Type		male
	  Anim          male\barf_cycle 1 50
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

MEnd

Move Emote_Vomit_Post_Three
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Vomit_To_Ready_Two
	Flags		FixArmReg NoSizeScale

	Type		male
	  Anim          male\quickbarf 60
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

##################################################
Move A_Emote_Double_Facepalm
	Requires	FLY EMOTE FACEPALM ALTERNATE
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	Interpolate	8
	Flags		NOSIZESCALE
	NextMove	A_Emote_Double_Facepalm_Post

	Type		male
	  Anim          male\air_emote_double_facepalm 1 20
	TEnd

	Type		huge
	  Anim          huge\air_emote_double_facepalm 1 20
	TEnd

	Type		fem
	  Anim          fem\air_emote_double_facepalm 1 20
	TEnd

MEnd

Move A_Emote_Double_Facepalm_Post
	Include sequencers\Members\Emotes_Idle_Air.inc
	Interpolate	8
	Flags		NOSIZESCALE
	NextMove	FlyReady

	Type		male
	  Anim          male\air_emote_double_facepalm 21
	TEnd

	Type		huge
	  Anim          huge\air_emote_double_facepalm 21
	TEnd

	Type		fem
	  Anim          fem\air_emote_double_facepalm 21
	TEnd

MEnd

##################################################
Move A_Emote_Facepalm
	Requires	FLY EMOTE FACEPALM
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	Interpolate	8
	Flags		NOSIZESCALE
	NextMove	A_Emote_Facepalm_Post

	Type		male
	  Anim          male\air_emote_facepalm 1 20
	TEnd

	Type		huge
	  Anim          huge\air_emote_facepalm 1 20
	TEnd

	Type		fem
	  Anim          fem\air_emote_facepalm 1 20
	TEnd

MEnd

Move A_Emote_Facepalm_Post
	Include sequencers\Members\Emotes_Idle_Air.inc
	Interpolate	8
	Flags		NOSIZESCALE
	NextMove	FlyReady

	Type		male
	  Anim          male\air_emote_facepalm 21
	TEnd

	Type		huge
	  Anim          huge\air_emote_facepalm 21
	TEnd

	Type		fem
	  Anim          fem\air_emote_facepalm 21
	TEnd

MEnd

########################
## Open Holiday Gift  ##
########################

Move Emote_Open_Holiday_Gift
	Requires	EMOTE STAND GIFT LOW
	Priority        4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat>
	Flags		NoSizeScale
	NextMove        Ready

	Type		male
	  Anim		male\open_holiday_gift 1
	  PlayFx	Events\Winter\Open_Gift1.fx 1 Constant
	TEnd

	Type		fem
	  Anim          fem\open_holiday_gift 1
	  PlayFx        Events\Winter\Open_Gift1.fx 1 Constant
	TEnd

	Type		huge
	  Anim		huge\open_holiday_gift 1
	  PlayFx        Events\Winter\Open_Gift1.fx 1 Constant
	TEnd
MEnd


## Sniff Emote ###########################################################

Move Emote_Sniff
	Requires	EMOTE ANIMAL YELL ONE
	Priority        4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale
	NextMove	Emote_Sniff_Post

	Type		male
	  Anim          male\emote_sniff 1 17
	TEnd

	Type		fem
	  Anim          fem\emote_sniff 1 17
	TEnd

MEnd

Move Emote_Sniff_Post
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale
	NextMove	Ready

	Type		male
	  Anim          male\emote_sniff 17
	TEnd

	Type		fem
	  Anim          fem\emote_sniff 17
	TEnd

MEnd


## Air Sniff Emote ###########################################################

Move A_Emote_Sniff
	Requires	FLY EMOTE ANIMAL YELL ONE
	Priority        5
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat>
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		nosizescale
	NextMove	A_Emote_Sniff_Post

	Type		male
	  Anim          male\air_emote_sniff 1 17
	TEnd

	Type		fem
	  Anim          fem\air_emote_sniff 1 17
	TEnd

MEnd

Move A_Emote_Sniff_Post
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		nosizescale
	NextMove	FlyReady

	Type		male
	  Anim          male\air_emote_sniff 17
	TEnd

	Type		fem
	  Anim          fem\air_emote_sniff 17
	TEnd

MEnd

## Hiss Emote ###########################################################

Move Emote_Hiss
	Requires	EMOTE ANIMAL ALTERNATE ONE
	Priority        5
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale
	NextMove	Emote_Hiss_Post

	Type		male
	  Anim          male\emote_hiss 1 27
	TEnd

	Type		fem
	  Anim          fem\emote_hiss 1 27
	TEnd

MEnd

Move Emote_Hiss_Post
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale
	NextMove	Ready

	Type		male
	  Anim          male\emote_hiss 27
	TEnd

	Type		fem
	  Anim          fem\emote_hiss 27
	TEnd

MEnd

## Air Hiss Emote ###########################################################

Move A_Emote_Hiss
	Requires	FLY EMOTE ANIMAL ALTERNATE ONE
	Priority        6
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat>
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		NoSizeScale
	NextMove	A_Emote_Hiss_Post


	Type		male
	  Anim          male\air_emote_hiss 1 27
	TEnd

MEnd

Move A_Emote_Hiss_Post
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		NoSizeScale
	NextMove	FlyReady


	Type		male
	  Anim          male\air_emote_hiss 27
	TEnd

MEnd

## Monkey Emote ###########################################################

Move Emote_Monkey_Dance
	Requires	EMOTE ANIMAL DANCE ALTERNATE
	Priority        4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale
	NextMove	Emote_Monkey_Dance_Post

	Type		male
	  Anim          male\emote_monkey 1 34
	TEnd

	Type		huge
	  Anim          huge\emote_monkey 1 34
	TEnd

	Type		fem
	  Anim          fem\emote_monkey 1 34
	TEnd

MEnd

Move Emote_Monkey_Dance_Post
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale
	NextMove	Emote_Monkey_Dance_Cycle

	Type		male
	  Anim          male\emote_monkey 34 160
	TEnd

	Type		huge
	  Anim          huge\emote_monkey 34 160
	TEnd

	Type		fem
	  Anim          fem\emote_monkey 34 160
	TEnd

MEnd

Move Emote_Monkey_Dance_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale Cycle
	CycleMove	Emote_Monkey_Dance_Cycle
	NextMove	Emote_Monkey_Dance_Cycle

	Type		male
	  Anim          male\emote_monkey 160 220
	TEnd

	Type		huge
	  Anim          huge\emote_monkey 160 220
	TEnd

	Type		fem
	  Anim          fem\emote_monkey 160 220
	TEnd

MEnd

## Emote Offer Gift ###########################################################

Move Emote_Offer_Gift_Pre
	Requires	EMOTE GIFT ONE
	Priority        3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Offer_Gift_One
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_offergift 1 14
	Playfx		Events\Winter\Offer_Gift1.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\emote_offergift 1 14
	Playfx		Events\Winter\Offer_Gift1.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\emote_offergift 1 14
	Playfx		Events\Winter\Offer_Gift1.fx 0 constant
	TEnd

MEnd

Move Emote_Offer_Gift_One
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NoSizeScale
	NextMove	Emote_Offer_Gift_Two

	Type		male
	  Anim          male\emote_offergift 14 37
	Playfx		Events\Winter\Offer_Gift1.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\emote_offergift 14 37
	Playfx		Events\Winter\Offer_Gift1.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\emote_offergift 14 37
	Playfx		Events\Winter\Offer_Gift1.fx 0 constant
	TEnd

MEnd

Move Emote_Offer_Gift_Two
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NoSizeScale
	NextMove	Emote_Offer_Gift_Post

	Type		male
	  Anim          male\emote_offergift 37 52
	Playfx		Events\Winter\Offer_Gift1.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\emote_offergift 37 52
	Playfx		Events\Winter\Offer_Gift1.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\emote_offergift 37 52
	Playfx		Events\Winter\Offer_Gift1.fx 0 constant
	TEnd

MEnd

Move Emote_Offer_Gift_Post
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NoSizeScale
	NextMove	Ready

	Type		male
	  Anim          male\emote_offergift 52 78
	TEnd

	Type		huge
	  Anim          huge\emote_offergift 52 78
	TEnd

	Type		fem
	  Anim          fem\emote_offergift 52 78
	TEnd
MEnd

## AIR Emote Offer Gift ###########################################################

Move A_Emote_Offer_Gift_Pre
	Requires	FLY EMOTE GIFT ONE
	Priority        4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Offer_Gift_One
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_emote_offergift 1 14
	Playfx		Events\Winter\Offer_Gift1.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\air_emote_offergift 1 14
	Playfx		Events\Winter\Offer_Gift1.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\air_emote_offergift 1 14
	Playfx		Events\Winter\Offer_Gift1.fx 0 constant
	TEnd

MEnd

Move A_Emote_Offer_Gift_One
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		NoSizeScale
	NextMove	A_Emote_Offer_Gift_Two

	Type		male
	  Anim          male\air_emote_offergift 14 37
	Playfx		Events\Winter\Offer_Gift1.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\air_emote_offergift 14 37
	Playfx		Events\Winter\Offer_Gift1.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\air_emote_offergift 14 37
	Playfx		Events\Winter\Offer_Gift1.fx 0 constant
	TEnd

MEnd

Move A_Emote_Offer_Gift_Two
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		NoSizeScale
	NextMove	A_Emote_Offer_Gift_Post

	Type		male
	  Anim          male\air_emote_offergift 37 52
	Playfx		Events\Winter\Offer_Gift1.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\air_emote_offergift 37 52
	Playfx		Events\Winter\Offer_Gift1.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\air_emote_offergift 37 52
	Playfx		Events\Winter\Offer_Gift1.fx 0 constant
	TEnd

MEnd

Move A_Emote_Offer_Gift_Post
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		NoSizeScale
	NextMove	FlyReady

	Type		male
	  Anim          male\air_emote_offergift 52 78
	TEnd

	Type		huge
	  Anim          huge\air_emote_offergift 52 78
	TEnd

	Type		fem
	  Anim          fem\air_emote_offergift 52 78
	TEnd

MEnd

## Emote Receive Gift ###########################################################

Move Emote_Receive_Gift_Pre
	Requires	EMOTE GIFT TWO
	Priority        3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Receive_Gift
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_receivegift 1 40
	TEnd

	Type		huge
	  Anim          huge\emote_receivegift 1 40
	TEnd

	Type		fem
	  Anim          fem\emote_receivegift 1 40
	TEnd


MEnd

Move Emote_Receive_Gift
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NoSizeScale
	NextMove	Emote_Receive_Gift_Post

	Type		male
	  Anim          male\emote_receivegift 40 57
	Playfx		Events\Winter\Receive_Gift1.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\emote_receivegift 40 57
	Playfx		Events\Winter\Receive_Gift1.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\emote_receivegift 40 57
	Playfx		Events\Winter\Receive_Gift1.fx 0 constant
	TEnd

MEnd

Move Emote_Receive_Gift_Post
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NoSizeScale
	NextMove	Emote_Receive_Gift_Post_One

	Type		male
	  Anim          male\emote_receivegift 57 81
	Playfx		Events\Winter\Receive_Gift1.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\emote_receivegift 57 81
	Playfx		Events\Winter\Receive_Gift1.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\emote_receivegift 57 81
	Playfx		Events\Winter\Receive_Gift1.fx 0 constant
	TEnd

MEnd

Move Emote_Receive_Gift_Post_One
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NoSizeScale
	NextMove	Ready

	Type		male
	  Anim          male\emote_receivegift 81
	TEnd

	Type		huge
	  Anim          huge\emote_receivegift 81
	TEnd

	Type		fem
	  Anim          fem\emote_receivegift 81
	TEnd

MEnd

## AIR Emote Receive Gift ###########################################################

Move A_Emote_Receive_Gift_Pre
	Requires	FLY EMOTE GIFT TWO
	Priority        4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Receive_Gift
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_emote_receivegift 1 40
	TEnd

	Type		huge
	  Anim          huge\air_emote_receivegift 1 40
	TEnd

	Type		fem
	  Anim          fem\air_emote_receivegift 1 40
	TEnd

MEnd

Move A_Emote_Receive_Gift
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		NoSizeScale
	NextMove	A_Emote_Receive_Gift_Post

	Type		male
	  Anim          male\air_emote_receivegift 40 57
	Playfx		Events\Winter\Receive_Gift1.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\air_emote_receivegift 40 57
	Playfx		Events\Winter\Receive_Gift1.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\air_emote_receivegift 40 57
	Playfx		Events\Winter\Receive_Gift1.fx 0 constant
	TEnd

MEnd

Move A_Emote_Receive_Gift_Post
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		NoSizeScale
	NextMove	A_Emote_Receive_Gift_Post_One

	Type		male
	  Anim          male\air_emote_receivegift 57 81
	Playfx		Events\Winter\Receive_Gift1.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\air_emote_receivegift 57 81
	Playfx		Events\Winter\Receive_Gift1.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\air_emote_receivegift 57 81
	Playfx		Events\Winter\Receive_Gift1.fx 0 constant
	TEnd

MEnd

Move A_Emote_Receive_Gift_Post_One
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		NoSizeScale
	NextMove	FlyReady

	Type		male
	  Anim          male\air_emote_receivegift 81
	TEnd

	Type		huge
	  Anim          huge\air_emote_receivegift 81
	TEnd

	Type		fem
	  Anim          fem\air_emote_receivegift 81
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