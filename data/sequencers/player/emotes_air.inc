#############################################################
## Emotes_air.inc - Flying emotes
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

Move A_Teabag_Pre
	Requires	FLY EMOTE TEABAG
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> #<IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Teabag_cycle
	Flags		FIXARMREG

	Type		male
	  Anim          male\emote_teabag 1 10
	  PlayFX	generic\teabag.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\emote_teabag 1 10
	  PlayFX	generic\teabag.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\emote_teabag 1 10
	  PlayFX	generic\teabag.fx 0 constant
	TEnd
MEnd

Move A_Teabag
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Teabag_cycle
	Flags		FIXARMREG REQINPUTS

	Type		male
	  Anim          male\emote_teabag 10
	  PlayFX	generic\teabag.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\emote_teabag 10
	  PlayFX	generic\teabag.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\emote_teabag 10
	  PlayFX	generic\teabag.fx 0 constant
	TEnd
MEnd

Move A_Teabag_cycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE FIXARMREG REQINPUTS

	Type		male
	  Anim          male\emote_teabag 1
	  PlayFX	generic\teabag.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\emote_teabag 1
	  PlayFX	generic\teabag.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\emote_teabag 1
	  PlayFX	generic\teabag.fx 0 constant
	TEnd
MEnd

#############################################################
## Invention
#############################################################

Move A_Invention_Pre
	Requires	FLY EMOTE INVENT
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> #<IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Invention_Cycle
	Flags		FIXARMREG

	Type		male
	  Anim          male\invention_intro_hovering 15
	  PlayFX	generic\InventionWidget.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\invention_intro_hovering 15
	  PlayFX	generic\InventionWidget.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\invention_intro_hovering 15
	  PlayFX	generic\InventionWidget.fx 0 constant
	TEnd
MEnd

Move A_Invention_Cycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	A_Invention_Cycle
	CycleMove	A_Invention_Cycle
	CycleMove	A_Invention_Examine
	Flags		FIXARMREG CYCLE REQINPUTS

	Type		male
	  Anim          male\invention_loop_hovering 1
	  PlayFX	generic\InventionWidget.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\invention_loop_hovering 1
	  PlayFX	generic\InventionWidget.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\invention_loop_hovering 1
	  PlayFX	generic\InventionWidget.fx 0 constant
	TEnd
MEnd

Move A_Invention_Examine
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	CycleMove	A_Invention_Cycle
	CycleMove	A_Invention_Cycle
	CycleMove	A_Invention_Cycle
	Flags		FIXARMREG CYCLE REQINPUTS

	Type		male
	  Anim          male\invention_break1_hovering 1
	  PlayFX	generic\InventionWidget.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\invention_break1_hovering 1
	  PlayFX	generic\InventionWidget.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\invention_break1_hovering 1
	  PlayFX	generic\InventionWidget.fx 0 constant
	TEnd
MEnd


#############################################################
## Dance Base
#############################################################

Move A_Dance1
	Requires	FLY EMOTE DANCE ONE
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Dance1_cycle

	Type		male
	  Anim          male\dance_base 1
	TEnd
MEnd

Move A_Dance1_cycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS

	Type		male
	  Anim          male\dance_base 1
	TEnd
MEnd

#############################################################
## Dance Block
#############################################################

Move A_Dance2
	Requires	FLY EMOTE DANCE TWO
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Dance2_cycle

	Type		male
	  Anim          male\dance_block 1
	TEnd
MEnd

Move A_Dance2_cycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS

	Type		male
	  Anim          male\dance_block 1
	TEnd
MEnd

#############################################################
## Dance Washer
#############################################################

Move A_Dance3
	Requires	FLY EMOTE DANCE THREE
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Dance3_cycle

	Type		male
	  Anim          male\dance_washer 1
	TEnd
MEnd

Move A_Dance3_cycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS

	Type		male
	  Anim          male\dance_washer 1
	TEnd
MEnd

#############################################################
## Dance Armsup
#############################################################

Move A_Dance4
	Requires	FLY EMOTE DANCE FOUR
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Dance4_cycle

	Type		male
	  Anim          male\dance_armsup 1
	TEnd
MEnd

Move A_Dance4_cycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS

	Type		male
	  Anim          male\dance_armsup 1
	TEnd
MEnd

#############################################################
## Dance Bounce
#############################################################

Move A_Dance5
	Requires	FLY EMOTE DANCE FIVE
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Dance5_cycle

	Type		male
	  Anim          male\dance_bounce 1
	TEnd
MEnd

Move A_Dance5_cycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS

	Type		male
	  Anim          male\dance_bounce 1
	TEnd
MEnd

#############################################################
## Dance Monkey
#############################################################

Move A_Dance6
	Requires	FLY EMOTE DANCE SIX
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Dance6_cycle

	Type		male
	  Anim          male\dance_monkey 1
	TEnd
MEnd

Move A_Dance6_cycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS

	Type		male
	  Anim          male\dance_monkey 1
	TEnd
MEnd

#############################################################
## Sitting Poses
#############################################################

Move A_Sitting_Pose1_Pre
	Requires	FLY EMOTE SIT POSE ONE
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Sitting_Pose1_Cycle
	Priority	5
	Flags		FIXARMREG

	Type		male
	  Anim          male\male_sitting_poses 1 2
	TEnd

	Type		NPC_male
	  Anim          male\npc_sitting_poses 1 2
	TEnd

	Type		fem
	  Anim          fem\female_sitting_poses 1 2
	TEnd

	Type		huge
	  Anim          huge\huge_sitting_poses 1 2
	TEnd
MEnd

Move A_Sitting_Pose1_Cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	Flags		CYCLE FIXARMREG REQINPUTS

	Type		male
	  Anim          male\male_sitting_poses 1 2
	TEnd

	Type		NPC_male
	  Anim          male\npc_sitting_poses 1 2
	TEnd

	Type		fem
	  Anim          fem\female_sitting_poses 1 2
	TEnd

	Type		huge
	  Anim          huge\huge_sitting_poses 1 2
	TEnd
MEnd

#############################################################

Move A_Emote_Sitting_Pose2_Intro
	Requires	FLY EMOTE SIT POSE TWO
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Sitting_Pose2_Loop
	Priority	5
	Flags		FIXARMREG

	Type		male
	  Anim          male\male_sitting_poses 3 4
	TEnd

	Type		fem
	  Anim          fem\female_sitting_poses 3 4
	TEnd

	Type		huge
	  Anim          huge\huge_sitting_poses 3 4
	TEnd
MEnd

Move A_Emote_Sitting_Pose2_Loop
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	CycleMove	Emote_Sitting_Pose2_Loop
	Flags		CYCLE FIXARMREG REQINPUTS

	Type		male
	  Anim          male\male_sitting_poses 3 4
	TEnd

	Type		fem
	  Anim          fem\female_sitting_poses 3 4
	TEnd

	Type		huge
	  Anim          huge\huge_sitting_poses 3 4
	TEnd
MEnd

#############################################################

Move A_Emote_Sitting_Pose3_Intro
	Requires	FLY EMOTE SIT POSE THREE
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> #<IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Sitting_Pose3_Loop
	Priority	3
	Flags		FIXARMREG

	Type		male
	  Anim          male\male_sitting_poses 5 6
	TEnd

	Type		fem
	  Anim          fem\female_sitting_poses 5 6
	TEnd

	Type		huge
	  Anim          huge\huge_sitting_poses 5 6
	TEnd
MEnd

Move A_Emote_Sitting_Pose3_Loop
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	CycleMove	A_Emote_Sitting_Pose3_Loop
	Flags		CYCLE FIXARMREG REQINPUTS

	Type		male
	  Anim          male\male_sitting_poses 5 6
	TEnd

	Type		fem
	  Anim          fem\female_sitting_poses 5 6
	TEnd

	Type		huge
	  Anim          huge\huge_sitting_poses 5 6
	TEnd
MEnd

#############################################################

Move A_Emote_Sitting_Pose4_Intro
	Requires	FLY EMOTE SIT POSE FOUR
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> #<IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Sitting_Pose4_Loop
	Priority	5
	Flags		FIXARMREG

	Type		male
	  Anim          male\male_sitting_poses 7 8
	TEnd

	Type		fem
	  Anim          fem\female_sitting_poses 7 8
	TEnd

	Type		huge
	  Anim          huge\huge_sitting_poses 7 8
	TEnd
MEnd

Move A_Emote_Sitting_Pose4_Loop
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	Flags		CYCLE FIXARMREG REQINPUTS
	CycleMove	A_Emote_Sitting_Pose4_Loop

	Type		male
	  Anim          male\male_sitting_poses 7 8
	TEnd

	Type		fem
	  Anim          fem\female_sitting_poses 7 8
	TEnd

	Type		huge
	  Anim          huge\huge_sitting_poses 7 8
	TEnd
MEnd

#############################################################

Move A_Emote_Sitting_Pose5_Intro
	Requires	FLY EMOTE SIT POSE FIVE
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> #<IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Sitting_Pose5_Loop
	Priority	5
	Flags		FIXARMREG

	Type		male
	  Anim          male\male_sitting_poses 9 10
	TEnd

	Type		fem
	  Anim          fem\female_sitting_poses 9 10
	TEnd

	Type		huge
	  Anim          huge\huge_sitting_poses 9 10
	TEnd
MEnd

Move A_Emote_Sitting_Pose5_Loop
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	Flags		CYCLE FIXARMREG REQINPUTS
	CycleMove	A_Emote_Sitting_Pose5_Loop

	Type		male
	  Anim          male\male_sitting_poses 9 10
	TEnd

	Type		fem
	  Anim          fem\female_sitting_poses 9 10
	TEnd

	Type		huge
	  Anim          huge\huge_sitting_poses 9 10
	TEnd
MEnd

#############################################################

Move A_Emote_Sitting_Pose6_Intro
	Requires	FLY EMOTE SIT POSE SIX
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Sitting_Pose6_Loop
	Priority	5
	Flags		FIXARMREG

	Type		male
	  Anim          male\male_sitting_poses 11 12
	TEnd

	Type		fem
	  Anim          fem\female_sitting_poses 9 10
	TEnd

	Type		huge
	  Anim          huge\huge_sitting_poses 11 12
	TEnd
MEnd

Move A_Emote_Sitting_Pose6_Loop
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	Flags		CYCLE FIXARMREG REQINPUTS
	CycleMove	A_Emote_Sitting_Pose6_Loop

	Type		male
	  Anim          male\male_sitting_poses 11 12
	TEnd

	Type		fem
	  Anim          fem\female_sitting_poses 9 10
	TEnd

	Type		huge
	  Anim          huge\huge_sitting_poses 11 12
	TEnd
MEnd

#############################################################

Move A_Emote_Sitting_Pose7_Intro
	Requires	FLY EMOTE SIT POSE SEVEN
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Sitting_Pose7_Loop
	Priority	5
	Flags		FIXARMREG

	Type		male
	  Anim          male\male_sitting_poses 13 14
	TEnd

	Type		fem
	  Anim          fem\female_sitting_poses 9 10
	TEnd

	Type		huge
	  Anim          huge\huge_sitting_poses 13 14
	TEnd
MEnd

Move A_Emote_Sitting_Pose7_Loop
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	Flags		CYCLE FIXARMREG REQINPUTS
	CycleMove	A_Emote_Sitting_Pose7_Loop

	Type		male
	  Anim          male\male_sitting_poses 13 14
	TEnd

	Type		fem
	  Anim          fem\female_sitting_poses 9 10
	TEnd

	Type		huge
	  Anim          huge\huge_sitting_poses 13 14
	TEnd
MEnd

#############################################################

Move A_Emote_Sitting_Pose8_Intro
	Requires	FLY EMOTE SIT POSE EIGHT
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Sitting_Pose8_Loop
	Priority	5
	Flags		FIXARMREG

	Type		male
	  Anim          male\male_sitting_poses 15 16
	TEnd

	Type		fem
	  Anim          fem\female_sitting_poses 9 10
	TEnd

	Type		huge
	  Anim          huge\huge_sitting_poses 15 16
	TEnd
MEnd

Move A_Emote_Sitting_Pose8_Loop
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	Flags		CYCLE FIXARMREG REQINPUTS
	CycleMove	A_Emote_Sitting_Pose8_Loop

	Type		male
	  Anim          male\male_sitting_poses 15 16
	TEnd

	Type		fem
	  Anim          fem\female_sitting_poses 9 10
	TEnd

	Type		huge
	  Anim          huge\huge_sitting_poses 15 16
	TEnd
MEnd

#############################################################

Move A_Emote_Sitting_Pose9_Intro
	Requires	FLY EMOTE SIT POSE NINE
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Sitting_Pose9_Loop
	Priority	3
	Flags		FIXARMREG

	Type		male
	  Anim          male\male_sitting_poses 17 18
	TEnd

	Type		fem
	  Anim          fem\female_sitting_poses 11 12
	TEnd

	Type		huge
	  Anim          huge\huge_sitting_poses 17 18
	TEnd
MEnd

Move A_Emote_Sitting_Pose9_Loop
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	Flags		CYCLE FIXARMREG REQINPUTS
	CycleMove	A_Emote_Sitting_Pose9_Loop

	Type		male
	  Anim          male\male_sitting_poses 17 18
	TEnd

	Type		fem
	  Anim          fem\female_sitting_poses 11 12
	TEnd

	Type		huge
	  Anim          huge\huge_sitting_poses 17 18
	TEnd
MEnd

#############################################################

Move A_Emote_Sitting_Pose10_Intro
	Requires	FLY EMOTE SIT POSE TEN
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Sitting_Pose10_Loop
	Priority	5
	Flags		FIXARMREG

	Type		male
	  Anim          male\male_sitting_poses 19 20
	TEnd

	Type		fem
	  Anim          fem\female_sitting_poses 9 10
	TEnd

	Type		huge
	  Anim          huge\huge_sitting_poses 19 20
	TEnd
MEnd

Move A_Emote_Sitting_Pose10_Loop
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	CycleMove	A_Emote_Sitting_Pose10_Loop
	Flags		CYCLE FIXARMREG REQINPUTS

	Type		male
	  Anim          male\male_sitting_poses 19 20
	TEnd

	Type		fem
	  Anim          fem\female_sitting_poses 9 10
	TEnd

	Type		huge
	  Anim          huge\huge_sitting_poses 19 20
	TEnd
MEnd

#############################################################

Move A_Emote_Sitting_Pose11_Intro
	Requires	FLY EMOTE SIT POSE TEN ONE
	Priority	6
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Sitting_Pose11_Loop
	Flags		FIXARMREG

	Type		male
	  Anim          male\male_sitting_poses 21 22
	TEnd

	Type		fem
	  Anim          fem\female_sitting_poses 13 14
	TEnd

	Type		huge
	  Anim          huge\huge_sitting_poses 21 22
	TEnd
MEnd

Move A_Emote_Sitting_Pose11_Loop
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	Flags		CYCLE FIXARMREG REQINPUTS
	CycleMove	A_Emote_Sitting_Pose11_Loop

	Type		male
	  Anim          male\male_sitting_poses 21 22
	TEnd

	Type		fem
	  Anim          fem\female_sitting_poses 13 14
	TEnd

	Type		huge
	  Anim          huge\huge_sitting_poses 21 22
	TEnd
MEnd


#############################################################
## Lotus\Yoga Emote
#############################################################

Move A_Lotus_Pre
	Requires	FLY EMOTE SIT MEDITATE LOW
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Lotus_Cycle
	Priority	5
	Flags		NOSIZESCALE FIXARMREG
	Wind		SIT

	Type		male
	  Anim          fem\float_lotus 1
	TEnd

	Type		huge
	  Anim          huge\float_lotus 1
	TEnd
MEnd

Move A_Lotus_Cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	Flags		CYCLE NOSIZESCALE REQINPUTS FIXARMREG
	Wind		SIT

	Type		male
	  Anim          fem\float_lotus 1
	TEnd

	Type		huge
	  Anim          huge\float_lotus 1
	TEnd
MEnd


#############################################################
## Howl
#############################################################

Move A_Emote_Howl
	Requires	FLY EMOTE HOWL
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_NoSpam_Air.inc
	NextMove	Fake_C_flycycle
	Flags		NOSIZESCALE
	Scale		0.625

	Type		male
	  Anim          male\air_scream 1
	  PlayFX	Emotes\PlayerHowl.fx 10
	TEnd

	Type		fem
	  Anim          fem\air_scream 1
	  PlayFX	Emotes\PlayerHowl.fx 10
	TEnd

	Type		huge
	  Anim          huge\air_scream 1
	  PlayFX	Emotes\PlayerHowl.fx 10
	TEnd
MEnd

#############################################################
## Random Dance - Dance Base
#############################################################

Move A_D_Base
	Requires	FLY EMOTE DANCE LOW
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_D_Base_cycle

	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\dance_base 1
	TEnd
MEnd

Move A_D_Base_cycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS

	Type		male
	  Anim          male\dance_base 1
	TEnd
MEnd

#############################################################
## Random Dance - Dance Block
#############################################################

Move A_D_Block
	Requires	FLY EMOTE DANCE LOW
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_D_Block_cycle

	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\dance_block 1
	TEnd
MEnd

Move A_D_Block_cycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS

	Type		male
	  Anim          male\dance_block 1
	TEnd
MEnd


#############################################################
## Random Dance - Dance Washer
#############################################################

Move A_D_Washer
	Requires	FLY EMOTE DANCE LOW
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_D_Washer_cycle

	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\dance_washer 1
	TEnd
MEnd

Move A_D_Washer_cycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS

	Type		male
	  Anim          male\dance_washer 1
	TEnd
MEnd


#############################################################
## Random Dance - Dance Armsup
#############################################################

Move A_D_Armsup
	Requires	FLY EMOTE DANCE LOW
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_D_Armsup_cycle

	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\dance_armsup 1
	TEnd
MEnd

Move A_D_Armsup_cycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS

	Type		male
	  Anim          male\dance_armsup 1
	TEnd
MEnd


#############################################################
## Random Dance - Dance Bounce
#############################################################

Move A_D_Bounce
	Requires	FLY EMOTE DANCE LOW
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_D_Bounce_cycle

	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\dance_bounce 1
	TEnd
MEnd

Move A_D_Bounce_cycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS

	Type		male
	  Anim          male\dance_bounce 1
	TEnd
MEnd


#############################################################
## Random Dance - Dance Monkey
#############################################################

Move A_D_Monkey
	Requires	FLY EMOTE DANCE LOW
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_D_Monkey_cycle

	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\dance_monkey 1
	TEnd
MEnd

Move A_D_Monkey_cycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS

	Type		male
	  Anim          male\dance_monkey 1
	TEnd
MEnd


#############################################################
##  Eat Donut
#############################################################

Move A_Emote_Eat_Donut_Pre1
	Requires	FLY EMOTE EATDONUT
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Eat_Donut_Pre2
	Priority	3
	Scale		0.875
	Flags		NOSIZESCALE PICKRANDOMLY FIXARMREG

	Type		male
	  Anim          male\emote_eat_bite1 1 2
	  PlayFx        Generic\Eat_Donut.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\emote_eat_bite1 1 2
	  PlayFx        Generic\Eat_Donut.fx 0 Constant
	TEnd

	Type		huge
	  Anim          huge\emote_eat_bite1 1 2
	  PlayFx        Generic\Eat_Donut.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Eat_Donut_Pre2
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Eat_Donut_Loop1
	Priority	1
	Scale		0.875
	Flags		NOSIZESCALE REQINPUTS FIXARMREG

	Type		male
	  Anim          male\emote_eat_bite1 3
	  PlayFx        Generic\Eat_Donut.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\emote_eat_bite1 3
	  PlayFx        Generic\Eat_Donut.fx 0 Constant
	TEnd

	Type		huge
	  Anim          huge\emote_eat_bite1 3
	  PlayFx        Generic\Eat_Donut.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Eat_Donut_Loop1
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	Scale		0.875
	CycleMove	Emote_Eat_Donut_Loop1
	CycleMove	Emote_Eat_Donut_Loop2
	CycleMove	Emote_Eat_Donut_Bite1
	Flags		CYCLE NOSIZESCALE REQINPUTS FIXARMREG

	Type		male
	  Anim          male\emote_eat_loop 1
	  PlayFx        Generic\Eat_Donut.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\emote_eat_loop 1
	  PlayFx        Generic\Eat_Donut.fx 0 Constant
	TEnd

	Type		huge
	  Anim          huge\emote_eat_loop 1
	  PlayFx        Generic\Eat_Donut.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Eat_Donut_Loop2
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	Scale		0.75
	CycleMove	Emote_Eat_Donut_Bite1
	CycleMove	Emote_Eat_Donut_Bite1
	CycleMove	Emote_Eat_Donut_Loop1
	Flags		CYCLE NOSIZESCALE REQINPUTS FIXARMREG

	Type		male
	  Anim          male\emote_eat_loop2 1
	  PlayFx        Generic\Eat_Donut.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\emote_eat_loop2 1
	  PlayFx        Generic\Eat_Donut.fx 0 Constant
	TEnd

	Type		huge
	  Anim          huge\emote_eat_loop2 1
	  PlayFx        Generic\Eat_Donut.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Eat_Donut_Bite1
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE NOSIZESCALE REQINPUTS FIXARMREG
	Priority	1
	CycleMove	Emote_Eat_Donut_Wipe
	CycleMove	Emote_Eat_Donut_Wipe
	CycleMove	Emote_Eat_Donut_Bite1
	CycleMove	Emote_Eat_Donut_Loop1

	Type		male
	  Anim          male\emote_eat_bite1 1
	  PlayFx        Generic\Eat_Donut.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\emote_eat_bite1 1
	  PlayFx        Generic\Eat_Donut.fx 0 Constant
	TEnd

	Type		huge
	  Anim          huge\emote_eat_bite1 1
	  PlayFx        Generic\Eat_Donut.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Eat_Donut_Wipe
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE NOSIZESCALE REQINPUTS FIXARMREG
	Priority	1
	CycleMove	Emote_Eat_Donut_Loop1
	CycleMove	Emote_Eat_Donut_Bite1

	Type		male
	  Anim          male\emote_eat_wipe 1
	  PlayFx        Generic\Eat_Donut.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\emote_eat_wipe 1
	  PlayFx        Generic\Eat_Donut.fx 0 Constant
	TEnd

	Type		huge
	  Anim          huge\emote_eat_wipe 1
	  PlayFx        Generic\Eat_Donut.fx 0 Constant
	TEnd
MEnd


#############################################################
##  Eat Food
#############################################################

Move A_Emote_Eat_Food_Pre1
	Requires	FLY EMOTE EATFOOD
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Eat_Food_Pre2
	Priority	3
	Flags		NOSIZESCALE PICKRANDOMLY FIXARMREG

	Type		male
	  Anim          male\emote_eat_bite1 1 2
	  PlayFx        Generic\Eat_Food.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\emote_eat_bite1 1 2
	  PlayFx        Generic\Eat_Food.fx 0 Constant
	TEnd

	Type		huge
	  Anim          huge\emote_eat_bite1 1 2
	  PlayFx        Generic\Eat_Food.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Eat_Food_Pre2
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Eat_Food_Loop1
	Priority	1
	Flags		NOSIZESCALE REQINPUTS FIXARMREG

	Type		male
	  Anim          male\emote_eat_bite1 3
	  PlayFx        Generic\Eat_Food.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\emote_eat_bite1 3
	  PlayFx        Generic\Eat_Food.fx 0 Constant
	TEnd

	Type		huge
	  Anim          huge\emote_eat_bite1 3
	  PlayFx        Generic\Eat_Food.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Eat_Food_Loop1
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	CycleMove	Emote_Eat_Food_Loop1
	CycleMove	Emote_Eat_Food_Loop2
	CycleMove	Emote_Eat_Food_Bite1
	Flags		CYCLE NOSIZESCALE REQINPUTS FIXARMREG

	Type		male
	  Anim          male\emote_eat_loop 1
	  PlayFx        Generic\Eat_Food.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\emote_eat_loop 1
	  PlayFx        Generic\Eat_Food.fx 0 Constant
	TEnd

	Type		huge
	  Anim          huge\emote_eat_loop 1
	  PlayFx        Generic\Eat_Food.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Eat_Food_Loop2
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	Scale		0.9
	CycleMove	Emote_Eat_Food_Bite1
	CycleMove	Emote_Eat_Food_Bite1
	CycleMove	Emote_Eat_Food_Loop1
	Flags		CYCLE NOSIZESCALE REQINPUTS FIXARMREG

	Type		male
	  Anim          male\emote_eat_loop2 1
	  PlayFx        Generic\Eat_Food.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\emote_eat_loop2 1
	  PlayFx        Generic\Eat_Food.fx 0 Constant
	TEnd

	Type		huge
	  Anim          huge\emote_eat_bite1 1
	  PlayFx        Generic\Eat_Food.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Eat_Food_Bite1
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	CycleMove	Emote_Eat_Food_Wipe
	CycleMove	Emote_Eat_Food_Wipe
	CycleMove	Emote_Eat_Food_Bite1
	CycleMove	Emote_Eat_Food_Loop1
	Flags		CYCLE NOSIZESCALE REQINPUTS FIXARMREG

	Type		male
	  Anim          male\emote_eat_bite1 1
	  PlayFx        Generic\Eat_Food.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\emote_eat_bite1 1
	  PlayFx        Generic\Eat_Food.fx 0 Constant
	TEnd

	Type		huge
	  Anim          huge\emote_eat_bite1 1
	  PlayFx        Generic\Eat_Food.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Eat_Food_Wipe
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	CycleMove	Emote_Eat_Food_Loop1
	CycleMove	Emote_Eat_Food_Bite1
	Flags		CYCLE NOSIZESCALE REQINPUTS FIXARMREG

	Type		male
	  Anim          male\emote_eat_wipe 1
	  PlayFx        Generic\Eat_Food.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\emote_eat_wipe 1
	  PlayFx        Generic\Eat_Food.fx 0 Constant
	TEnd

	Type		huge
	  Anim          huge\emote_eat_wipe 1
	  PlayFx        Generic\Eat_Food.fx 0 Constant
	TEnd
MEnd


#############################################################
## Listening to Police Band
#############################################################

Move A_ListenPoliceBand_pre
	Requires	FLY LISTENPOLICEBAND
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_ListenPoliceBand
	Priority	2
	Flags		FIXARMREG

	Type		male
	  Anim          male\emote_ListenPoliceBand_intro 1 2
	  PlayFX	generic\PoliceBandScanner.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\emote_ListenPoliceBand_intro 1 2
	  PlayFX	generic\PoliceBandScanner.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\emote_ListenPoliceBand_intro 1 2
	  PlayFX	generic\PoliceBandScanner.fx 0 constant
	TEnd

#	Type		v_male
#	  Anim          male\newspaper_cycle 1 2
#	  PlayFx        GENERIC\FX_newspaper_rogue.FX 0 Constant
#	TEnd
#
#	Type		v_fem
#	  Anim          fem\fem_newspaper 1 2
#	  PlayFx        GENERIC\FX_newspaper_female_rogue.FX 0 Constant
#	TEnd
#
#	Type		v_huge
#	  Anim          huge\AFK_newspaper 1 2
#	  PlayFx        GENERIC\FX_newspaper_huge_rogue.FX 0 Constant
#	TEnd

	Type		beast
	  Anim          Beast\lookquick 1 2
	TEnd
MEnd

Move A_ListenPoliceBand
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_ListenPoliceBand_Cycle
	Priority	2
	Flags		FIXARMREG REQINPUTS

	Type		male
	  Anim          male\emote_ListenPoliceBand_intro 3
	  PlayFX	generic\PoliceBandScanner.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\emote_ListenPoliceBand_intro 3
	  PlayFX	generic\PoliceBandScanner.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\emote_ListenPoliceBand_intro 3
	  PlayFX	generic\PoliceBandScanner.fx 0 constant
	TEnd

#	Type		v_male
#	  Anim          male\newspaper_cycle 3 156
#	  PlayFx        GENERIC\FX_newspaper_rogue.FX 0 Constant
#	TEnd
#
#	Type		v_fem
#	  Anim          fem\fem_newspaper 3 156
#	  PlayFx        GENERIC\FX_newspaper_female_rogue.FX 0 Constant
#	TEnd
#
#	Type		v_huge
#	  Anim          huge\AFK_newspaper 3 156
#	  PlayFx        GENERIC\FX_newspaper_huge_rogue.FX 0 Constant
#	TEnd

	Type		beast
	  Anim          Beast\lookquick 3
	TEnd
MEnd

Move A_ListenPoliceBand_Cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	Flags		FIXARMREG CYCLE REQINPUTS

	Type		male
	  Anim          male\emote_ListenPoliceBand_loop 1
	  PlayFX	generic\PoliceBandScanner.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\emote_ListenPoliceBand_loop 1
	  PlayFX	generic\PoliceBandScanner.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\emote_ListenPoliceBand_loop 1
	  PlayFX	generic\PoliceBandScanner.fx 0 constant
	TEnd

#	Type		v_male
#	  Anim          male\newspaper_cycle 1 156
#	  PlayFx        GENERIC\FX_newspaper_rogue.FX 0 Constant
#	TEnd
#
#	Type		v_fem
#	  Anim          fem\fem_newspaper 1 156
#	  PlayFx        GENERIC\FX_newspaper_female_rogue.FX 0 Constant
#	TEnd
#
#	Type		v_huge
#	  Anim          huge\AFK_newspaper 1 156
#	  PlayFx        GENERIC\FX_newspaper_huge_rogue.FX 0 Constant
#	TEnd

	Type		beast
	  Anim          Beast\lookquick 1
	TEnd
MEnd


#############################################################
## Listening to Stolen Police Band
#############################################################

Move A_ListenStolenPoliceBand_Intro
	Requires	FLY EMOTE LISTENSTOLENPOLICEBAND
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_ListenStolenPoliceBand_Cycle
	Priority	3
	Flags		FIXARMREG

	Type		male
	  Anim          male\emote_ListenPoliceBand_intro 1
	  PlayFX	generic\PoliceBandScanner.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\emote_ListenPoliceBand_intro 1
	  PlayFX	generic\PoliceBandScanner.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\emote_ListenPoliceBand_intro 1
	  PlayFX	generic\PoliceBandScanner.fx 0 constant
	TEnd

	Type		Beast
	  Anim          male\MonComb_cycle 1
	TEnd
MEnd

Move A_ListenStolenPoliceBand_Cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	CycleMove	ListenStolenPoliceBand_Cycle
	Flags		FIXARMREG CYCLE REQINPUTS

	Type		male
	  Anim          male\emote_ListenPoliceBand_loop 1
	  PlayFX	generic\PoliceBandScanner.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\emote_ListenPoliceBand_loop 1
	  PlayFX	generic\PoliceBandScanner.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\emote_ListenPoliceBand_loop 1
	  PlayFX	generic\PoliceBandScanner.fx 0 constant
	TEnd

	Type		Beast
	  Anim          male\MonComb_cycle 1
	TEnd
MEnd


#############################################################
## Dojo Kneel
#############################################################

Move A_Pre_DojoKneel
	Requires	FLY EMOTE KNEEL KUNGFU SIT
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_DojoKneel_cycle
	Priority	5
	#Wind		SIT
	Flags		FIXARMREG

	Type		male
	  Anim          male\dojo_kneel 1 66
	TEnd

	Type		beast
	  Anim          beast\sit_comp 1 4
	TEnd
MEnd

Move A_DojoKneel_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	#Wind		SIT
	Flags		CYCLE REQINPUTS FIXARMREG

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

Move A_GroundSit
	Requires	FLY EMOTE SIT
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_GroundSit_cycle
	Priority	3
	#Wind		SIT

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

Move A_GroundSit_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS
	Priority	1
	#Wind		SIT

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
## Waiting
#############################################################

Move A_emote_Waiting_pre
	Requires	FLY EMOTE WAITING
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_emote_Waiting_cycle
	Priority	3
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

Move A_emote_Waiting_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	CycleMove	emote_Waiting_cycle
	CycleMove	emote_Waiting_cycle
	CycleMove	emote_Waiting_cycle
	CycleMove	emote_Waiting_break1
	Flags		FIXARMREG CYCLE REQINPUTS

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

Move A_emote_Waiting_break1
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	NextMove	A_emote_Waiting_post
	Flags		FIXARMREG REQINPUTS

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

Move A_emote_Waiting_post
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	NextMove	A_emote_Waiting_cycle
	Flags		FIXARMREG REQINPUTS

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


#############################################################
## Flipping Coin
#############################################################

Move A_Emote_FlippingCoin_pre
	Requires	FLY EMOTE FLIPPINGCOIN
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_FlippingCoin_cycle
	Priority	3
	Scale		0.85

	Type		male
	  Anim          male\emote_FlippingCoin 1
	  PlayFX	generic\FlippingCoin.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\emote_FlippingCoin 1
	  PlayFX	generic\FlippingCoin.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\emote_FlippingCoin 1
	  PlayFX	generic\FlippingCoin.fx 0 constant
	TEnd
MEnd

Move A_Emote_FlippingCoin_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS
	Priority	1
	Scale		0.85

	Type		male
	  Anim          male\emote_FlippingCoin 1
	  PlayFX	generic\FlippingCoin.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\emote_FlippingCoin 1
	  PlayFX	generic\FlippingCoin.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\emote_FlippingCoin 1
	  PlayFX	generic\FlippingCoin.fx 0 constant
	TEnd
MEnd

#############################################################
## Smartphone
#############################################################

Move A_Emote_Smartphone_Pre
	Requires	FLY EMOTE Smartphone
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Smartphone_Cycle
	Priority	3
	Scale		0.5
	Flags		FIXARMREG

	Type		male
	  Anim          male\air_emote_smartphone 1
	  PlayFx        GENERIC\FX_Smartphone.FX 0 Constant
	TEnd

	Type		huge
	  Anim          huge\air_emote_smartphone 1
	  PlayFx        GENERIC\FX_Smartphone.FX 0 Constant
	TEnd

	Type		fem
	  Anim          fem\air_emote_smartphone 1
	  PlayFx        GENERIC\FX_Smartphone.FX 0 Constant
	TEnd
MEnd

Move A_Emote_Smartphone_Cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	Scale		0.5
	Flags		FIXARMREG CYCLE REQINPUTS

	Type		male
	  Anim          male\air_emote_smartphone 1
	  PlayFx        GENERIC\FX_Smartphone.FX 0 Constant
	TEnd

	Type		huge
	  Anim          huge\air_emote_smartphone 1
	  PlayFx        GENERIC\FX_Smartphone.FX 0 Constant
	TEnd

	Type		fem
	  Anim          fem\air_emote_smartphone 1
	  PlayFx        GENERIC\FX_Smartphone.FX 0 Constant
	TEnd
MEnd

#############################################################
## Smartphone - Text
#############################################################

Move A_Emote_Smartphone_Text_Pre
	Requires	FLY EMOTE Smartphone TEXT
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Smartphone_Text_Cycle
	Priority	4
	Flags		FIXARMREG nosizescale

	Type		male
	  Anim          male\air_emote_smartphone_texting 1 39
	  PlayFx        GENERIC\FX_Smartphone.FX 0 Constant
	TEnd

	Type		huge
	  Anim          huge\air_emote_smartphone_texting 1 39
	  PlayFx        GENERIC\FX_Smartphone_huge.FX 0 Constant
	TEnd

	Type		fem
	  Anim          fem\air_emote_smartphone_texting 1 39
	  PlayFx        GENERIC\FX_Smartphone.FX 0 Constant
	TEnd
MEnd

Move A_Emote_Smartphone_Text_Cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	NextMove	A_Emote_Smartphone_Text_Cycle_Post
	Flags		FIXARMREG nosizescale CYCLE

	Type		male
	  Anim          male\air_emote_smartphone_texting 39 112
	  PlayFx        GENERIC\FX_Smartphone.FX 0 Constant
	TEnd

	Type		huge
	  Anim          huge\air_emote_smartphone_texting 39 112
	  PlayFx        GENERIC\FX_Smartphone_huge.FX 0 Constant
	TEnd

	Type		fem
	  Anim          fem\air_emote_smartphone_texting 39 112
	  PlayFx        GENERIC\FX_Smartphone.FX 0 Constant
	TEnd
MEnd

Move A_Emote_Smartphone_Text_Cycle_Post
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	FlyReady
	Flags		FIXARMREG nosizescale

	Type		male
	  Anim          male\air_emote_smartphone_texting 112
	  PlayFx        GENERIC\FX_Smartphone.FX 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\air_emote_smartphone_texting 112
	  PlayFx        GENERIC\FX_Smartphone_huge.FX 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\air_emote_smartphone_texting 112
	  PlayFx        GENERIC\FX_Smartphone.FX 0 CONSTANT
	TEnd
MEnd

#############################################################
## Smartphone - Camerapicture
#############################################################

Move A_Emote_Smartphone_Camerapicture
	Requires	FLY EMOTE Smartphone PICTURE
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Cellphone_Cameraphone_Post
	Priority	4
	#Scale		0.5
	Flags		FIXARMREG

	Type		male
	  Anim          male\air_emote_cameraphone 1 155
	  PlayFx        GENERIC\FX_Smartphone.FX 0 Constant
	TEnd

	Type		huge
	  Anim          huge\air_emote_cameraphone 1 155
	  PlayFx        GENERIC\FX_Smartphone.FX 0 Constant
	TEnd

	Type		fem
	  Anim          fem\air_emote_cameraphone 1 155
	  PlayFx        GENERIC\FX_Smartphone.FX 0 Constant
	TEnd
MEnd

Move A_Emote_Cellphone_Cameraphone_Post
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	NextMove	FlyReady
	Interpolate	8
	Flags		FIXARMREG REQINPUTS

	Type		male
	  Anim          male\air_emote_cameraphone 155
	 # PlayFx        GENERIC\FX_cellphone.FX 0 Constant
	TEnd

	Type		huge
	  Anim          huge\air_emote_cameraphone 155
	  #PlayFx        GENERIC\FX_cellphone.FX 0 Constant
	TEnd

	Type		fem
	  Anim          fem\air_emote_cameraphone 155
	  #PlayFx        GENERIC\FX_cellphone.FX 0 Constant
	TEnd
MEnd

#############################################################
## Cellphone - old
#############################################################

Move A_Emote_Cellphone_Pre
	Requires	FLY EMOTE CELLPHONE
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Cellphone_Cycle
	Priority	3
	Scale		0.5
	Flags		FIXARMREG

	Type		male
	  Anim          male\air_emote_cellphone 1
	  PlayFx        GENERIC\FX_cellphone.FX 0 Constant
	TEnd

	Type		huge
	  Anim          huge\air_emote_cellphone 1
	  PlayFx        GENERIC\FX_cellphone.FX 0 Constant
	TEnd

	Type		fem
	  Anim          fem\air_emote_cellphone 1
	  PlayFx        GENERIC\FX_cellphone.FX 0 Constant
	TEnd
MEnd

Move A_Emote_Cellphone_Cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	Scale		0.5
	Flags		FIXARMREG CYCLE REQINPUTS

	Type		male
	  Anim          male\air_emote_cellphone 1
	  PlayFx        GENERIC\FX_cellphone.FX 0 Constant
	TEnd

	Type		huge
	  Anim          huge\air_emote_cellphone 1
	  PlayFx        GENERIC\FX_cellphone.FX 0 Constant
	TEnd

	Type		fem
	  Anim          fem\air_emote_cellphone 1
	  PlayFx        GENERIC\FX_cellphone.FX 0 Constant
	TEnd
MEnd

#############################################################
## Binoculars
#############################################################

Move A_Emote_Binoculars_Pre
	Requires	FLY EMOTE BINOCULARS
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Binoculars
	Priority	3
	Scale		0.5
	Flags		FIXARMREG

	Type		male
	  Anim          male\emote_binoculars 1
	  PlayFx        GENERIC\Binoculars.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\emote_binoculars 1
	  PlayFx        GENERIC\Binoculars.fx 0 Constant
	TEnd

	Type		huge
	  Anim          huge\emote_binoculars 1
	  PlayFx        GENERIC\Binoculars.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Binoculars
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	Scale		0.5
	Flags		CYCLE FIXARMREG REQINPUTS

	Type		male
	  Anim          male\emote_binoculars 1
	  PlayFx        GENERIC\Binoculars.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\emote_binoculars 1
	  PlayFx        GENERIC\Binoculars.fx 0 Constant
	TEnd

	Type		huge
	  Anim          huge\emote_binoculars 1
	  PlayFx        GENERIC\Binoculars.fx 0 Constant
	TEnd
MEnd


#############################################################
## Clipboard
#############################################################

Move A_Emote_Clipboard_Pre
	Requires	FLY EMOTE CLIPBOARD
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Clipboard
	Priority	3
	Scale		0.7
	Flags		FIXARMREG

	Type		male
	  Anim          male\clipboard_write 1
	  PlayFx        GENERIC\FX_clipboard.FX 0 Constant
	TEnd

	Type		fem
	  Anim          fem\fem_clipboard 1
	  PlayFx        GENERIC\FX_clipboard.FX 0 Constant
	TEnd

	Type		huge
	  Anim          huge\emote_clipboard 1
	  PlayFx        GENERIC\FX_clipboard.FX 0 Constant
	TEnd
MEnd

Move A_Emote_Clipboard
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	Scale		0.7
	Flags		FIXARMREG CYCLE REQINPUTS

	Type		male
	  Anim          male\clipboard_write 1
	  PlayFx        GENERIC\FX_clipboard.FX 0 Constant
	TEnd

	Type		fem
	  Anim          fem\fem_clipboard 1
	  PlayFx        GENERIC\FX_clipboard.FX 0 Constant
	TEnd

	Type		huge
	  Anim          huge\emote_clipboard 1
	  PlayFx        GENERIC\FX_clipboard.FX 0 Constant
	TEnd
MEnd


#############################################################
## Mission Architect Accolade
#############################################################

Move A_Emote_MATablet_Pre
	Requires	FLY EMOTE CLIPBOARD MISSIONARCHITECT
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_MATablet
	Flags		FixArmReg

	Type		male
	  Anim          male\air_Bookie_Base 1 15
	  PlayFx	Emotes\MATablet\Male_MATablet.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\air_Bookie_Base 1 15
	  PlayFx	Emotes\MATablet\Fem_MATablet.fx 0 Constant
	TEnd

	Type		huge
	  Anim          huge\air_Bookie_Base 1 15
	  PlayFx	Emotes\MATablet\Huge_MATablet.fx 0 Constant
	TEnd
MEnd

Move A_Emote_MATablet
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	PreFall
	CycleMove	A_Emote_MATablet_Cycle
	Flags		Cycle FixArmReg

	Type		male
	  Anim          male\air_Bookie_Base 15
	  PlayFx	Emotes\MATablet\Male_MATablet.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\air_Bookie_Base 15
	  PlayFx	Emotes\MATablet\Fem_MATablet.fx 0 Constant
	TEnd

	Type		huge
	  Anim          huge\air_Bookie_Base 15
	  PlayFx	Emotes\MATablet\Huge_MATablet.fx 0 Constant
	TEnd
MEnd

Move A_Emote_MATablet_Cycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	PreFall
	Flags		Cycle FixArmReg

	Type		male
	  Anim          male\air_Bookie_Base 1
	  PlayFx	Emotes\MATablet\Male_MATablet.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\air_Bookie_Base 1
	  PlayFx	Emotes\MATablet\Fem_MATablet.fx 0 Constant
	TEnd

	Type		huge
	  Anim          huge\air_Bookie_Base 1
	  PlayFx	Emotes\MATablet\Huge_MATablet.fx 0 Constant
	TEnd
MEnd


#############################################################
## Camera - Taking Pictures
#############################################################

Move A_Emote_Camera_Pre
	Requires	FLY EMOTE CAMERA
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Camera
	Flags		FIXARMREG NOSIZESCALE
	Priority	3

	Type		male
	  Anim          male\air_emote_camera 1
	  PlayFx        GENERIC\Camera_Big.fx 0 Constant
	TEnd

	Type		huge
	  Anim          huge\air_emote_camera 1
	  PlayFx        GENERIC\Camera_Big.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\air_emote_camera 1
	  PlayFx        GENERIC\Camera_Big.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Camera
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	Flags		FIXARMREG CYCLE NOSIZESCALE REQINPUTS

	Type		male
	  Anim          male\air_emote_camera 1
	  PlayFx        GENERIC\Camera_Big.fx 0 Constant
	TEnd

	Type		huge
	  Anim          huge\air_emote_camera 1
	  PlayFx        GENERIC\Camera_Big.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\air_emote_camera 1
	  PlayFx        GENERIC\Camera_Big.fx 0 Constant
	TEnd
MEnd

#############################################################
## SLRCamera - Taking Pictures
#############################################################

Move A_Emote_SLRCamera_Pre
	Requires	FLY EMOTE SLRCAMERA
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_SLRCamera
	Flags		FIXARMREG NOSIZESCALE
	Priority	3

	Type		male
	  Anim          male\air_emote_slrcamera 1
	  PlayFx        GENERIC\Camera_SLR.fx 0 Constant
	TEnd

	Type		huge
	  Anim          huge\air_emote_slrcamera 1
	  PlayFx        GENERIC\Camera_SLR.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\air_emote_slrcamera 1
	  PlayFx        GENERIC\Camera_SLR.fx 0 Constant
	TEnd
MEnd

Move A_Emote_SLRCamera
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	Flags		FIXARMREG CYCLE NOSIZESCALE REQINPUTS

	Type		male
	  Anim          male\air_emote_slrcamera 1
	  PlayFx        GENERIC\Camera_SLR.fx 0 Constant
	TEnd

	Type		huge
	  Anim          huge\air_emote_slrcamera 1
	  PlayFx        GENERIC\Camera_SLR.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\air_emote_slrcamera 1
	  PlayFx        GENERIC\Camera_SLR.fx 0 Constant
	TEnd
MEnd


#############################################################
## Fishing
#############################################################

Move A_Emote_Fishing_Pre
	Requires	FLY EMOTE FISHING
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Fishing
	Priority	3
	Flags		FIXARMREG NOSIZESCALE

	Type		male
	  Anim          male\emote_fishing 1
	  Playfx	Generic\FX_FishingRod.fx 5 Constant
	TEnd

	Type		fem
	  Anim          male\emote_fishing 1
	  Playfx	Generic\FX_FishingRod.fx 5 Constant
	TEnd

	Type		huge
	  Anim          huge\emote_fishing 1
	  Playfx	Generic\FX_FishingRod.fx 5 Constant
	TEnd
MEnd

Move A_Emote_Fishing
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	Flags		FIXARMREG CYCLE NOSIZESCALE REQINPUTS

	Type		male
	  Anim          male\emote_fishing 1
	  Playfx	Generic\FX_FishingRod.fx 0 Constant
	TEnd

	Type		fem
	  Anim          male\emote_fishing 1
	  Playfx	Generic\FX_FishingRod.fx 0 Constant
	TEnd

	Type		huge
	  Anim          huge\emote_fishing 1
	  Playfx	Generic\FX_FishingRod.fx 0 Constant
	TEnd
MEnd


#############################################################
## Talker
#############################################################

Move A_Emote_Talk_Pre
	Requires	FLY EMOTE TALK
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Talk_Cycle
	Priority	3
	Scale		0.75
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\PL_TalkIdle 1
	TEnd
MEnd

Move A_Emote_Talk_Cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	2
	CycleMove	A_Emote_Talk_Cycle
	CycleMove	A_Emote_Talk01
	CycleMove	A_Emote_Talk02
	CycleMove	A_Emote_Talk03
	Scale		0.75
	Flags		CYCLE NOSIZESCALE REQINPUTS

	Type		male
	  Anim          male\PL_TalkIdle 1
	TEnd
MEnd

Move A_Emote_Talk01
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	CycleMove	A_Emote_Talk_Cycle
	CycleMove	A_Emote_Talk_Cycle
	CycleMove	A_Emote_Talk02
	CycleMove	A_Emote_Talk03
	Scale		0.75
	Flags		CYCLE NOSIZESCALE REQINPUTS

	Type		male
	  Anim          male\PL_Talk01 1
	TEnd
MEnd

Move A_Emote_Talk02
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	CycleMove	A_Emote_Talk_Cycle
	CycleMove	A_Emote_Talk_Cycle
	CycleMove	A_Emote_Talk01
	CycleMove	A_Emote_Talk03
	Scale		0.75
	Flags		CYCLE NOSIZESCALE REQINPUTS

	Type		male
	  Anim          male\PL_Talk02 1
	TEnd
MEnd

Move A_Emote_Talk03
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	CycleMove	A_Emote_Talk_Cycle
	CycleMove	A_Emote_Talk_Cycle
	CycleMove	A_Emote_Talk01
	CycleMove	A_Emote_Talk02
	Scale		0.75
	Flags		CYCLE NOSIZESCALE REQINPUTS

	Type		male
	  Anim          male\PL_Talk03 1
	TEnd
MEnd

#############################################################
## Whistle
#############################################################

Move A_Emote_Whistle
	Requires	FLY EMOTE WHISTLE
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	3
	Interpolate     7
	Flags		FIXARMREG

	Type		male
	  Anim          male\whistle 1 50
	  PlayFx        FEET\WHISTLE.FX  19
	TEnd

	Type		huge
	  Anim          huge\whistle 1 50
	  PlayFx        FEET\WHISTLE.FX  19
	TEnd

	Type		fem
	  Anim          fem\whistle 1 50
	  PlayFx        FEET\WHISTLE.FX  19
	TEnd
MEnd


#############################################################
## RaiseHand
#############################################################

Move A_Emote_RaiseHand
	Requires	FLY EMOTE RAISEHAND
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_RaiseHand_cycle

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

Move A_Emote_RaiseHand_cycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS

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

Move A_Emote_Attack
	Requires	FLY EMOTE ATTACK
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Attack2

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

Move A_Emote_Attack2
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		REQINPUTS

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

Move A_Emote_DontAttack
	Requires	FLY EMOTE NO
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_DontAttack2

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

Move A_Emote_DontAttack2
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	Xarms_Cycle
	Flags		REQINPUTS

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

Move A_Emote_OverHere
	Requires	FLY EMOTE BIGWAVE
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_OverHere2

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

Move A_Emote_OverHere2
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		REQINPUTS

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

Move A_Emote_Hi
	Requires	FLY EMOTE WAVE
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Hi2
	Priority	3

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

Move A_Emote_Hi2
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		REQINPUTS

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

Move A_Emote_ThumbsUp
	Requires	FLY EMOTE YES
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_ThumbsUp2
	Priority	3

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

Move A_Emote_ThumbsUp2
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	Flags		REQINPUTS

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

Move A_RPS_Rock_pre
	Requires	FLY EMOTE ROCK
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_RPS_Rock_cast
	Priority	3
	Flags		FIXARMREG

	Type		male
	  Anim          male\RPS_pre 15 150
	  PlayFx        GENERIC\RPS_pre.FX 0 Constant
	TEnd
MEnd

Move A_RPS_Rock_cast
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_RPS_Rock
	Flags		FIXARMREG REQINPUTS

	Type		male
	  Anim          male\RPS_rock 28 73
	  PlayFx        GENERIC\RPS_pre.FX 0 Constant
	TEnd
MEnd

Move A_RPS_Rock
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_RPS_Rock_cycle
	Priority	1
	Flags		FIXARMREG REQINPUTS

	Type		male
	  Anim          male\RPS_rock 74 104
	  PlayFx        GENERIC\RPS_rock.FX 0 Constant
	TEnd
MEnd

Move A_RPS_Rock_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE NOSIZESCALE REQINPUTS
	Priority	1
	Flags		FIXARMREG REQINPUTS

	Type		male
	  Anim          male\RPS_rock 105
	  PlayFx        GENERIC\RPS_rock.FX 0 Constant
	TEnd
MEnd


#############################################################
## Rock\Paper\Scissors - "Paper"
#############################################################

Move A_RPS_paper_pre
	Requires	FLY EMOTE PAPER
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_RPS_paper_cast
	Priority	3
	Flags		FIXARMREG

	Type		male
	  Anim          male\RPS_pre 15 150
	  PlayFx        GENERIC\RPS_pre.FX 0 Constant
	TEnd
MEnd


Move A_RPS_paper_cast
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_RPS_paper
	Priority	1
	Flags		FIXARMREG REQINPUTS

	Type		male
	  Anim          male\RPS_paper 28 73
	  PlayFx        GENERIC\RPS_pre.FX 0 Constant
	TEnd
MEnd

Move A_RPS_paper
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_RPS_paper_cycle
	Priority	1
	Flags		FIXARMREG REQINPUTS

	Type		male
	  Anim          male\RPS_paper 74 104
	  PlayFx        GENERIC\RPS_paper.FX 0 Constant
	TEnd
MEnd

Move A_RPS_paper_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE NOSIZESCALE REQINPUTS
	Priority	1
	Flags		FIXARMREG REQINPUTS

	Type		male
	  Anim          male\RPS_paper 105
	  PlayFx        GENERIC\RPS_paper.FX 0 Constant
	TEnd
MEnd


#############################################################
## Rock\Paper\Scissors - "Scissors"
#############################################################

Move A_RPS_scissor_pre
	Requires	FLY EMOTE SCISSORS
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_RPS_scissor_cast
	Priority	4
	Flags		FIXARMREG

	Type		male
	  Anim          male\RPS_pre 15 150
	  PlayFx        GENERIC\RPS_pre.FX 0 Constant
	TEnd
MEnd

Move A_RPS_scissor_cast
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_RPS_scissor
	Priority	1
	Flags		FIXARMREG REQINPUTS

	Type		male
	  Anim          male\RPS_scissor 28 73
	  PlayFx        GENERIC\RPS_pre.FX 0 Constant
	TEnd
MEnd

Move A_RPS_scissor
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_RPS_scissor_cycle
	Priority	1
	Flags		FIXARMREG REQINPUTS

	Type		male
	  Anim          male\RPS_scissor 74 104
	  PlayFx        GENERIC\RPS_scissor.FX 0 Constant
	TEnd
MEnd

Move A_RPS_scissor_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE NOSIZESCALE REQINPUTS
	Priority	1
	Flags		FIXARMREG REQINPUTS

	Type		male
	  Anim          male\RPS_scissor 105
	  PlayFx        GENERIC\RPS_scissor.FX 0 Constant
	TEnd
MEnd


#############################################################
## Flip Coin (heads A)
#############################################################

Move A_Flipcoin_heads
	Requires	FLY EMOTE ROLLDICE CAST LOW
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Flipcoin_heads_cycle
	Priority	5
	Flags		FIXARMREG PICKRANDOMLY

	Type		male
	  Anim          male\flipcoin 1 140
	  PlayFx        GENERIC\Coin_A.FX  113 Constant
	  PlayFx        GENERIC\Prop_Coin.FX  56
	TEnd
MEnd

Move A_Flipcoin_heads_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	Flags		CYCLE REQINPUTS FIXARMREG

	Type		male
	  Anim          male\flipcoin 144
	  PlayFx        GENERIC\Coin_A.FX 0 Constant
	TEnd
MEnd


#############################################################
## Flip Coin (tails B)
#############################################################

Move A_Flipcoin_tail
	Requires	FLY EMOTE ROLLDICE CAST LOW
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Flipcoin_tail_cycle
	Priority	5
	Flags		PICKRANDOMLY FIXARMREG

	Type		male
	  Anim          male\flipcoin 1 140
	  PlayFx        GENERIC\Coin_B.FX  113 Constant
	  PlayFx        GENERIC\Prop_Coin.FX  56
	TEnd
MEnd

Move A_Flipcoin_tail_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	Flags		CYCLE REQINPUTS FIXARMREG

	Type		male
	  Anim          male\flipcoin 144
	  PlayFx        GENERIC\Coin_B.FX 0 Constant
	TEnd
MEnd


#############################################################
## Flex A
#############################################################

Move A_Flex_A
	Requires	FLY EMOTE FLEX HIGH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Flex_A_cycle
	Priority	4

	Type		male
	  Anim          male\flex_A 5 54
	TEnd
MEnd

Move A_Flex_A_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE NOSIZESCALE REQINPUTS
	Priority	1

	Type		male
	  Anim          male\flex_A 55
	TEnd
MEnd


#############################################################
## Flex B
#############################################################

Move A_Flex_B
	Requires	FLY EMOTE FLEX ONE
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Flex_B_cycle
	Priority	4

	Type		male
	  Anim          male\flex_B 6 54
	TEnd
MEnd

Move A_Flex_B_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE NOSIZESCALE REQINPUTS
	Priority	1

	Type		male
	  Anim          male\flex_B 55
	TEnd
MEnd


#############################################################
## Flex C
#############################################################

Move A_Flex_C
	Requires	FLY EMOTE FLEX LOW
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Flex_C_cycle
	Priority	4

	Type		male
	  Anim          male\flex_C 11 119
	TEnd
MEnd

Move A_Flex_C_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE NOSIZESCALE REQINPUTS
	Priority	1

	Type		male
	  Anim          male\flex_C 120
	TEnd
MEnd


#############################################################
## Heroic Laugh
#############################################################

Move A_Emote_Laugh
	Requires	FLY EMOTE LAUGH LOW
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	#NextMove	A_Emote_Laugh_cycle
	Priority	4
	Flags		FIXARMREG

	Type		male
	  Anim          male\heroic_laugh 1
	TEnd
MEnd

#############################################################
## Somewhat Mad
#############################################################

Move A_Emote_Mad
	Requires	FLY EMOTE ANGRY POSE LOW
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Mad_cycle
	Priority	5

	Type		male
	  Anim          male\mad 1 55
	TEnd
MEnd

Move A_Emote_Mad_cycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS

	Type		male
	  Anim          male\mad 55 60
	TEnd
MEnd


#############################################################
## "What?"
#############################################################

Move A_Emote_What
	Requires	FLY EMOTE QUESTION LOW
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	#NextMove	A_Emote_What_cycle
	Priority	4

	Type		male
	  Anim          male\what 1
	TEnd
MEnd

#############################################################
## Point
#############################################################

Move A_Emote_Point
	Requires	FLY EMOTE POINT
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	#NextMove	A_Emote_Point_cycle
	Priority	3

	Type		male
	  Anim          male\point 1
	TEnd
MEnd

#############################################################
## Salute
#############################################################

Move A_Emote_Salute
	Requires	FLY EMOTE SALUTE ONE
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Salute_cycle
	Priority	4
	Flags		FIXARMREG

	Type		male
	  Anim          male\salute 1
	TEnd
MEnd

Move A_Emote_Salute_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\thumbsup 99 100
	TEnd
MEnd


#############################################################
## Argue - Lecture
#############################################################

Move A_Argue
	Requires	FLY EMOTE ARGUE HIGH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Argue_cycle
	Priority	4

	Type		male
	  Anim          male\recruit_nobox 129 800
	TEnd
MEnd

Move A_Argue_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS
	Priority	1

	Type		male
	  Anim          male\thumbsup 99 100
	TEnd
MEnd


#############################################################
## Argue - Disagree
#############################################################

Move A_Argue_Disagree
	Requires	FLY EMOTE ARGUE MID
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Argue_Disagree_cycle
	Priority	4

	Type		male
	  Anim          male\deal_A 1 238
	TEnd
MEnd

Move A_Argue_Disagree_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS
	Priority	1

	Type		male
	  Anim          male\no 110 115
	TEnd
MEnd


#############################################################
## Argue - Counterpoint
#############################################################

Move A_Argue_Reason
	Requires	FLY EMOTE ARGUE LOW
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Argue_Reason2
	Priority	4

	Type		male
	  Anim          male\deal_B 92 238
	TEnd
MEnd

Move A_Argue_Reason2
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Argue_Reason_cycle
	Priority	1

	Flags		REQINPUTS

	Type		male
	  Anim          male\deal_B 1 83
	TEnd
MEnd

Move A_Argue_Reason_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS
	Priority	1
	Flags		REQINPUTS

	Type		male
	  Anim          male\thumbsup 99 100
	TEnd
MEnd


#############################################################
## KungfuTrain
#############################################################

Move A_Emote_KungfuTrain
	Requires	FLY EMOTE KUNGFU TRAIN STRONG
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_KungfuTrain_cycle
	Priority	5

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

Move A_Emote_KungfuTrain_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS
	Priority	1

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

Move A_Emote_JumpingJacks
	Requires	FLY EMOTE TRAIN LOW STRONG
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_JumpingJacks_cycle
	Priority	5

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

Move A_Emote_JumpingJacks_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS
	Priority	1

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

Move A_Emote_Beatchest
	Requires	FLY EMOTE COMBAT TAUNTA ANGRY
	Priority	5
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	Fake_C_flycycle
	Flags		NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\air_comb_beatchest 1 106
	  PlayFx        FEET\MALETAUNTA.FX
	TEnd

	Type		huge
	  Anim          huge\air_comb_beatchest 1 106
	  PlayFx        FEET\HUGETAUNTA.FX
	TEnd

	Type		fem
	  Anim          fem\air_comb_beatchest 1 106
	  PlayFx        FEET\FEMALETAUNTA.FX
	TEnd
MEnd

#############################################################
## "Come On!"
#############################################################

Move A_Emote_ComeOn
	Requires	FLY EMOTE COMBAT TAUNTB ANGRY
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_ComeOn2
	Priority	5
	Interpolate     8
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_combat_comeon 9 54
	  PlayFX	feet\MaleTaunt12.fx
	TEnd

	Type		huge
	  Anim          huge\air_combat_comeon 9 54
	  PlayFX	feet\mALETaunt6.fx
	TEnd

	Type		fem
	  Anim          fem\air_combat_comeon 9 54
	  PlayFX	feet\fEMTaunt9.fx
	TEnd
MEnd

Move A_Emote_ComeOn2
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_ComeOn_cycle
	Priority	1
	Flags		NOSIZESCALE REQINPUTS

	Type		male
	  Anim          male\air_combat_comeon 55 118
	  PlayFX	feet\handpunch1.fx
	TEnd

	Type		huge
	  Anim          huge\air_combat_comeon 55 118
	  PlayFX	feet\handpunch1.fx
	TEnd

	Type		fem
	  Anim          fem\air_combat_comeon 55 118
	  PlayFX	feet\handpunch1.fx
	TEnd
MEnd

Move A_Emote_ComeOn_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE NOSIZESCALE REQINPUTS
	NextMove	postReady_back
	Priority	1

	Type		male
	  Anim          male\air_combat_comeon 119 156
	  PlayFX	feet\handpunch1.fx
	TEnd

	Type		huge
	  Anim          huge\air_combat_comeon 119 156
	  PlayFX	feet\handpunch1.fx
	TEnd

	Type		fem
	  Anim          fem\air_combat_comeon 119 156
	  PlayFX	feet\handpunch1.fx
	TEnd
MEnd


#############################################################
## "Come Here!"
#############################################################

Move A_Emote_ComeHere
	Requires	FLY EMOTE COMBAT TAUNTC ANGRY
	Priority	5
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_ComeHere2

	Type		male
	  Anim          male\air_combat_comehere 1 2
	  PlayFX	feet\maletaunt4.fx
	TEnd

	Type		huge
	  Anim          huge\air_combat_comehere 1 2
	  PlayFX	FEET\HUGETAUNTA.FX
	TEnd

	Type		fem
	  Anim          fem\air_combat_comehere 1 2
	  PlayFX	FEET\femtaunt10.FX
	TEnd
MEnd

Move A_Emote_ComeHere2
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	Fake_C_flycycle_Half
	Flags		REQINPUTS

	Type		male
	  Anim          male\air_combat_comehere 2
	TEnd

	Type		huge
	  Anim          huge\air_combat_comehere 2
	TEnd

	Type		fem
	  Anim          fem\air_combat_comehere 2
	TEnd
MEnd

#############################################################
## AFK (newspaper)
#############################################################

Move A_AFK_newspaper_PRE
	Requires	FLY EMOTE AFK
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_AFK_newspaper_cycle
	Flags		FIXARMREG NOSIZESCALE

	Type		male
	  Anim          male\AFK_newspaper 1 156
	  PlayFx        GENERIC\FX_newspaper_male.FX 0 Constant
	TEnd

	Type		v_male
	  Anim          male\newspaper_cycle 1 156
	  PlayFx        GENERIC\FX_newspaper_rogue.FX 0 Constant
	TEnd

	Type		P_male
	  Anim          male\AFK_newspaper 1 156
	  PlayFx        GENERIC\FX_Newspaper_Praetorian_Male.FX 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\AFK_newspaper 1 156
	  PlayFx        GENERIC\FX_newspaper_huge.FX 0 Constant
	TEnd

	Type		V_huge
	  Anim          huge\AFK_newspaper 1 156
	  PlayFx        GENERIC\FX_newspaper_huge_rogue.FX 0 Constant
	TEnd

	Type		p_huge
	  Anim          huge\AFK_newspaper 1 156
	  PlayFx        GENERIC\FX_Newspaper_Praetorian_Huge.FX 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\fem_newspaper 1 156
	  PlayFx        GENERIC\FX_newspaper_female.FX 0 Constant
	TEnd

	Type		V_fem
	  Anim          fem\fem_newspaper 1 156
	  PlayFx        GENERIC\FX_newspaper_female_rogue.FX 0 Constant
	TEnd

	Type		p_fem
	  Anim          fem\fem_newspaper 1 156
	  PlayFx        GENERIC\FX_Newspaper_Praetorian_Female.FX 0 CONSTANT
	TEnd
MEnd

Move A_AFK_newspaper_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE FIXARMREG NOSIZESCALE REQINPUTS

	Type		male
	  Anim          male\AFK_newspaper 1 156
	  PlayFx        GENERIC\FX_newspaper_male.FX 0 Constant
	TEnd

	Type		v_male
	  Anim          male\newspaper_cycle 1 156
	  PlayFx        GENERIC\FX_newspaper_rogue.FX 0 Constant
	TEnd

	Type		P_male
	  Anim          male\AFK_newspaper 1 156
	  PlayFx        GENERIC\FX_Newspaper_Praetorian_Male.FX 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\AFK_newspaper 1 156
	  PlayFx        GENERIC\FX_newspaper_huge.FX 0 Constant
	TEnd

	Type		V_huge
	  Anim          huge\AFK_newspaper 1 156
	  PlayFx        GENERIC\FX_newspaper_huge_rogue.FX 0 Constant
	TEnd

	Type		p_huge
	  Anim          huge\AFK_newspaper 1 156
	  PlayFx        GENERIC\FX_Newspaper_Praetorian_Huge.FX 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\fem_newspaper 1 156
	  PlayFx        GENERIC\FX_newspaper_female.FX 0 Constant
	TEnd

	Type		V_fem
	  Anim          fem\fem_newspaper 1 156
	  PlayFx        GENERIC\FX_newspaper_female_rogue.FX 0 Constant
	TEnd

	Type		p_fem
	  Anim          fem\fem_newspaper 1 156
	  PlayFx        GENERIC\FX_Newspaper_Praetorian_Female.FX 0 CONSTANT
	TEnd
MEnd


#############################################################
##  Praise_Bow
#############################################################

Move A_Emote_Praise_Bow
	Requires	FLY EMOTE BOWING LOW
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Praise_Bow_cycle
	Priority	4
	Flags		FIXARMREG

	Type		male
	  Anim          male\kneeling_bow 1 63
	TEnd
MEnd

Move A_Emote_Praise_Bow_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	Flags		CYCLE REQINPUTS FIXARMREG

	Type		male
	  Anim          male\kneeling_bow 65 136
	TEnd
MEnd


#############################################################
## Cheer (chant)
#############################################################

Move A_Cheerchant_A
	Requires	FLY EMOTE CHEER HIGH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Cheerchant_A_cycle
	Priority	4
	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\chant_high 1
	TEnd
MEnd

Move A_Cheerchant_A_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE
	Priority	1

	Type		male
	  Anim          male\chant_high 1
	TEnd
MEnd

#############################################################

Move A_Cheerchant_B
	Requires	FLY EMOTE CHEER HIGH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Cheerchant_B_cycle
	Priority	4
	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\chant_1arm 1
	TEnd
MEnd

Move A_Cheerchant_B_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	Flags		CYCLE

	Type		male
	  Anim          male\chant_1arm 1
	TEnd
MEnd

#############################################################

Move A_Cheerchant_C
	Requires	FLY EMOTE CHEER HIGH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Cheerchant_C_cycle
	Priority	4
	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\chant_low 1
	TEnd
MEnd

Move A_Cheerchant_C_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	Flags		CYCLE REQINPUTS

	Type		male
	  Anim          male\chant_low 1
	TEnd
MEnd


#############################################################
## Rooting  (woof woof)
#############################################################

Move A_Rooting
	Requires	FLY EMOTE CHEER MID
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	#NextMove	A_Rooting_2
	Priority	4
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

#############################################################
## Clapping
#############################################################

Move A_Clapping
	Requires	FLY EMOTE CHEER LOW
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	#NextMove	A_Clapping_2
	Priority	4
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

#############################################################
## Reverant Bow
#############################################################

Move A_Bow_Reverant
	Requires	FLY EMOTE BOW GREETING LOW
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Bow_Reverant_cycle
	Priority	6
	Flags		FIXARMREG REQINPUTS

	Type		male
	  Anim          male\reverant_bow2 1 100
	TEnd

	Type		huge
	  Anim          huge\reverant_bow2 1 100
	TEnd
MEnd

Move A_Bow_Reverant_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	Interpolate	15
	Flags		REQINPUTS

	Type		male
	  Anim		male\postback_right 28
	TEnd

	Type		huge
	  Anim		huge\postback_right 28
	TEnd

	Type		fem
	  Anim		male\postback_right 28
	TEnd
MEnd


#############################################################
##  Military Salute
#############################################################

Move A_MilitarySalute
	Requires	FLY EMOTE SALUTE TWO
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_MilitarySalute_cycle
	Priority	4
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

Move A_MilitarySalute_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS FIXARMREG
	Priority	1

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

Move A_AtEase_FromMilitarySalute
	Requires	FLY EMOTE SALUTE HIGH LOW
	Priority	5
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_AtEase_cycle
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

Move A_AtEase
	Requires	FLY EMOTE SALUTE HIGH LOW
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_AtEase_cycle
	Priority	5
	Flags		FIXARMREG

	Type		male
	  Anim          male\at_ease 22 60
	TEnd

	Type		huge
	  Anim          huge\at_ease 22 60
	TEnd
MEnd

Move A_AtEase_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS FIXARMREG
	Priority	1

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

Move A_Emote_Nod
	Requires	FLY EMOTE YES LOW
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	4

	Type		male
	  Anim          male\nod 1 70
	TEnd
MEnd

#############################################################
## "Welcome"
#############################################################

Move A_Emote_Welcome
	Requires	FLY EMOTE GREETING HIGH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	4

	Type		male
	  Anim          male\welcome 1 135
	TEnd
MEnd

#############################################################
## Fancy_Bow
#############################################################

Move A_Emote_Fancy_Bow
	Requires	FLY EMOTE BOW GREETING HIGH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	#NextMove	A_Emote_Fancy_Bow_cycle
	Priority	4

	Type		male
	  Anim          male\fancy_bow 1
	TEnd
MEnd

#############################################################
## Victory
#############################################################

Move A_Emote_Victory
	Requires	FLY EMOTE CHEER FIERCE HIGH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	#NextMove	A_Emote_Victory_cycle
	Priority	5
	Interpolate	5

	Type		male
	  Anim          male\victory 1
	TEnd
MEnd

#############################################################
## "The Champ"
#############################################################

Move A_Emote_Champion
	Requires	FLY EMOTE CHEER FLEX
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	#NextMove	A_Emote_Champion_cycle
	Priority	4
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

#############################################################
## "The Wave"
#############################################################

Move A_Emote_TheWave
	Requires	FLY EMOTE CHEER WAVE HIGH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	#NextMove	A_Emote_TheWave_cycle
	Priority	5

	Type		male
	  Anim          male\the_wave 1
	TEnd
MEnd

#############################################################
## Frustrated
#############################################################

Move A_Emote_Frustrated
	Requires	FLY EMOTE ANGRY SAD MID
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Frustrated2
	Priority	5

	Type		male
	  Anim          male\idiots 1 2
	TEnd
MEnd

Move A_Emote_Frustrated2
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Frustrated_cycle
	Priority	1
	Flags		REQINPUTS

	Type		male
	  Anim          male\idiots 3
	TEnd
MEnd

Move A_Emote_Frustrated_cycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS

	Type		male
	  Anim          male\frustrated_cycle 1
	TEnd
MEnd


#############################################################
## Drat
#############################################################

Move A_Emote_Drat
	Requires	FLY EMOTE ANGRY RUDE SHRUG
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Drat2
	Priority	5

	Type		male
	  Anim          male\aw_crap 1 2
	TEnd
MEnd

Move A_Emote_Drat2
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Drat_cycle
	Priority	1
	Flags		REQINPUTS

	Type		male
	  Anim          male\aw_crap 3
	TEnd
MEnd

Move A_Emote_Drat_cycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS

	Type		male
	  Anim          male\frustrated_cycle 1
	TEnd
MEnd


#############################################################
## "Grief"
#############################################################

Move A_Emote_Grief
	Requires	FLY EMOTE SAD POSE SHRUG
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Grief_cycle
	Priority	5
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

Move A_Emote_Grief_cycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
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
## "You're Welcome"
#############################################################

Move A_Emote_YourWelcome
	Requires	FLY EMOTE THANK LOW POSE REPAIR
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	#NextMove	A_Emote_YourWelcome_cycle
	Priority	6
	Scale		1.2

	Type		male
	  Anim          male\your_welcome_sm 1
	TEnd
MEnd

#############################################################
## "Thank You" - Small
#############################################################

Move A_Emote_Thankyou_SM
	Requires	FLY EMOTE THANK LOW POSE
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	#NextMove	A_Emote_Thankyou_SM_cycle
	Priority	5

	Type		male
	  Anim          male\thankyou_sm 1
	TEnd
MEnd

#############################################################
## Judge Score = 1
#############################################################

Move A_Emote_Judge_1
	Requires	FLY EMOTE SCORE ONE
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Judge_1_cycle
	Priority	4

	Type		male
	  Anim          male\judge 1 41
	  PlayFx        GENERIC\Score_1.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 1 41
	  PlayFx        GENERIC\Score_1.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Judge_1_cycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS

	Type		male
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_1.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 41
	  PlayFx        GENERIC\Score_1.fx 0 Constant
	TEnd
MEnd


#############################################################
## Judge Score = 2
#############################################################

Move A_Emote_Judge_2
	Requires	FLY EMOTE SCORE TWO
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Judge_2_cycle
	Priority	4

	Type		male
	  Anim          male\judge 1 41
	  PlayFx        GENERIC\Score_2.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 1 41
	  PlayFx        GENERIC\Score_2.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Judge_2_cycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE

	Type		male
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_2.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 41
	  PlayFx        GENERIC\Score_2.fx 0 Constant
	TEnd
MEnd


#############################################################
## Judge Score = 3
#############################################################

Move A_Emote_Judge_3
	Requires	FLY EMOTE SCORE THREE
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Judge_3_cycle
	Priority	4

	Type		male
	  Anim          male\judge 1 41
	  PlayFx        GENERIC\Score_3.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 1 41
	  PlayFx        GENERIC\Score_3.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Judge_3_cycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE

	Type		male
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_3.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 41
	  PlayFx        GENERIC\Score_3.fx 0 Constant
	TEnd
MEnd


#############################################################
## Judge Score = 4
#############################################################

Move A_Emote_Judge_4
	Requires	FLY EMOTE SCORE FOUR
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Judge_4_cycle
	Priority	4

	Type		male
	  Anim          male\judge 1 41
	  PlayFx        GENERIC\Score_4.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 1 41
	  PlayFx        GENERIC\Score_4.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Judge_4_cycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE

	Type		male
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_4.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 41
	  PlayFx        GENERIC\Score_4.fx 0 Constant
	TEnd
MEnd


#############################################################
## Judge Score = 5
#############################################################

Move A_Emote_Judge_5
	Requires	FLY EMOTE SCORE FIVE
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Judge_5_cycle
	Priority	4

	Type		male
	  Anim          male\judge 1 41
	  PlayFx        GENERIC\Score_5.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 1 41
	  PlayFx        GENERIC\Score_5.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Judge_5_cycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE

	Type		male
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_5.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 41
	  PlayFx        GENERIC\Score_5.fx 0 Constant
	TEnd
MEnd


#############################################################
## Judge Score = 6
#############################################################

Move A_Emote_Judge_6
	Requires	FLY EMOTE SCORE SIX
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Judge_6_cycle
	Priority	4

	Type		male
	  Anim          male\judge 1 41
	  PlayFx        GENERIC\Score_6.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 1 41
	  PlayFx        GENERIC\Score_6.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Judge_6_cycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE

	Type		male
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_6.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 41
	  PlayFx        GENERIC\Score_6.fx 0 Constant
	TEnd
MEnd


#############################################################
## Judge Score = 7
#############################################################

Move A_Emote_Judge_7
	Requires	FLY EMOTE SCORE SEVEN
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Judge_7_cycle
	Priority	4

	Type		male
	  Anim          male\judge 1 41
	  PlayFx        GENERIC\Score_7.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 1 41
	  PlayFx        GENERIC\Score_7.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Judge_7_cycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE

	Type		male
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_7.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 41
	  PlayFx        GENERIC\Score_7.fx 0 Constant
	TEnd
MEnd


#############################################################
## Judge Score = 8
#############################################################

Move A_Emote_Judge_8
	Requires	FLY EMOTE SCORE EIGHT
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Judge_8_cycle
	Priority	4

	Type		male
	  Anim          male\judge 1 41
	  PlayFx        GENERIC\Score_8.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 1 41
	  PlayFx        GENERIC\Score_8.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Judge_8_cycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE

	Type		male
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_8.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 41
	  PlayFx        GENERIC\Score_8.fx 0 Constant
	TEnd
MEnd


#############################################################
## Judge Score = 9
#############################################################

Move A_Emote_Judge_9
	Requires	FLY EMOTE SCORE NINE
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Judge_9_cycle
	Priority	4

	Type		male
	  Anim          male\judge 1 41
	  PlayFx        GENERIC\Score_9.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 1 41
	  PlayFx        GENERIC\Score_9.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Judge_9_cycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE

	Type		male
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_9.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 41
	  PlayFx        GENERIC\Score_9.fx 0 Constant
	TEnd
MEnd


#############################################################
##  Judge Score = 10
#############################################################

Move A_Emote_Judge_10
	Requires	FLY EMOTE SCORE TEN
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Judge_10_cycle
	Priority	4

	Type		male
	  Anim          male\judge 1 41
	  PlayFx        GENERIC\Score_10.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 1 41
	  PlayFx        GENERIC\Score_10.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Judge_10_cycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE

	Type		male
	  Anim          male\judge 41
	  PlayFx        GENERIC\Score_10.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\judge 41
	  PlayFx        GENERIC\Score_10.fx 0 Constant
	TEnd
MEnd


#############################################################
## "Shucks!"
#############################################################

Move A_Emote_Shucks
	Requires	FLY EMOTE ANGRY RUDE SHRUG LOW
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	#NextMove	A_Emote_Shucks_cycle
	Priority	6

	Type		male
	  Anim          male\shucks 1
	TEnd
MEnd

#############################################################
## Panhandle
#############################################################

Move A_Emote_Panhandle
	Requires	FLY EMOTE PANHANDELING
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Panhandle_cycle_1
	Priority	2
	Flags		FIXARMREG

	Type		male
	  Anim          male\bum_cup_cycle 1
  	  PlayFx	GENERIC\cup_tin_r.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Panhandle_cycle_1
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	CycleMove	A_Emote_Panhandle_cycle_2
	Flags		FIXARMREG CYCLE REQINPUTS

	Type		male
	  Anim          male\bum_cup_beg_l 1 30
  	  PlayFx	GENERIC\cup_tin_r.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Panhandle_cycle_2
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	A_Emote_Panhandle_cycle_3
	CycleMove	A_Emote_Panhandle_cycle_5
	Priority	1
	Interpolate     13
	Flags		CYCLE REQINPUTS FIXARMREG

	Type		male
	  Anim          male\bum_cup_beg_l 35 80 ##85
  	  PlayFx	GENERIC\cup_tin_r.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Panhandle_cycle_3
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	A_Emote_Panhandle_cycle_7
	CycleMove	A_Emote_Panhandle_cycle_1
	CycleMove	A_Emote_Panhandle_cycle_4
	CycleMove	A_Emote_Panhandle
	Priority	1
	Flags		CYCLE REQINPUTS FIXARMREG

	Type		male
	  Anim          male\bum_cup_beg_l 85
  	  PlayFx	GENERIC\cup_tin_r.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Panhandle_cycle_4
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	A_Emote_Panhandle_cycle_5
	Priority	1
	Interpolate     15
	Flags		CYCLE REQINPUTS FIXARMREG

	Type		male
	  Anim          male\bum_cup_beg_r 1 30
  	  PlayFx	GENERIC\cup_tin_r.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Panhandle_cycle_5
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	A_Emote_Panhandle_cycle_6
	CycleMove	A_Emote_Panhandle_cycle_2
	Priority	1
	Interpolate     13
	Flags		CYCLE REQINPUTS FIXARMREG

	Type		male
	  Anim          male\bum_cup_beg_r 35 80 #85
  	  PlayFx	GENERIC\cup_tin_r.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Panhandle_cycle_6
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	A_Emote_Panhandle_cycle_7
	CycleMove	A_Emote_Panhandle_cycle_1
	CycleMove	A_Emote_Panhandle
	CycleMove	A_Emote_Panhandle_cycle_4
	Priority	1
	Flags		CYCLE REQINPUTS FIXARMREG

	Type		male
	  Anim          male\bum_cup_beg_r 85
  	  PlayFx	GENERIC\cup_tin_r.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Panhandle_cycle_7
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	A_Emote_Panhandle
	Priority	1
	Flags		CYCLE REQINPUTS FIXARMREG

	Type		male
	  Anim          male\bum_cup_jingle 1
  	  PlayFx	GENERIC\cup_tin_r.fx 0 Constant
	TEnd
MEnd


#############################################################
## Wall Lean
#############################################################

Move A_Emote_Wall_Lean_1
	Requires	FLY EMOTE LEANWALL
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Wall_Lean_cycle_1
	Priority	2
	Flags		FIXARMREG

	Type		male
	  Anim          male\wall_lean1 1
	TEnd
MEnd

Move A_Emote_Wall_Lean_cycle_1
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS FIXARMREG

	Type		male
	  Anim          male\wall_lean1 1
	TEnd
MEnd

#############################################################

Move A_Emote_Wall_Lean_3
	Requires	FLY EMOTE LEANWALL
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Wall_Lean_cycle_3
	Priority	2
	Flags		FIXARMREG

	Type		male
	  Anim          male\wall_lean3 1
	TEnd
MEnd

Move A_Emote_Wall_Lean_cycle_3
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	Flags		CYCLE REQINPUTS FIXARMREG

	Type		male
	  Anim          male\wall_lean3 1
	TEnd
MEnd


#############################################################
## HandsUp
#############################################################

Move A_Emote_HandsUp
	Requires	FLY EMOTE HANDSUP
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_HandsUp_cycle_1
	Priority	2

	Type		male
	  Anim          male\handsup 1
	TEnd
MEnd

Move A_Emote_HandsUp_cycle_1
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS
	CycleMove	A_Emote_HandsUp_cycle_1
	CycleMove	A_Emote_HandsUp_cycle_2
	CycleMove	A_Emote_HandsUp_cycle_1
	Priority	1

	Type		male
	  Anim          male\handsup 1
	TEnd
MEnd

Move A_Emote_HandsUp_cycle_2
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS
	NextMove	A_Emote_HandsUp_cycle_1
	Priority	1

	Type		male
	  Anim          male\handsup_var 1
	TEnd
MEnd


#############################################################
## HandsUp Kneeling
#############################################################

Move A_Emote_HandsUp_Kneel
	Requires	FLY EMOTE HANDSUP
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_HandsUp_Kneel_cycle_1
	Priority	2

	Type		male
	  Anim          male\handsup_knees 1
	TEnd
MEnd

Move A_Emote_HandsUp_Kneel_cycle_1
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	A_Emote_HandsUp_Kneel_cycle_1
	CycleMove	A_Emote_HandsUp_Kneel_cycle_1
	CycleMove	A_Emote_HandsUp_Kneel_cycle_2
	Priority	1
	Interpolate	20
	Flags		CYCLE REQINPUTS

	Type		male
	  Anim          male\handsup_knees 1
	TEnd
MEnd

Move A_Emote_HandsUp_Kneel_cycle_2
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_HandsUp_Kneel_cycle_1
	Priority	1
	Interpolate	25
	Flags		CYCLE REQINPUTS

	Type		male
	  Anim          male\handsup_knees_var 1
	TEnd
MEnd


#############################################################
## FlashLight
#############################################################

Move A_Emote_FlashLight_1
	Requires	FLY EMOTE FLASHLIGHT
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_FlashLight_cycle_1
	Priority	3

	Type		male
	  Anim          male\Flashlight_Up 1
	  PlayFX	Generic\Flashlight_NoSpot.fx 0 Constant
	TEnd
MEnd

Move A_Emote_FlashLight_cycle_1
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS

	Type		male
	  Anim          male\Flashlight_Up 1
	  PlayFX	Generic\Flashlight_NoSpot.fx 0 Constant
	TEnd
MEnd

#############################################################

Move A_Emote_FlashLight_2
	Requires	FLY EMOTE FLASHLIGHT
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_FlashLight_cycle_2
	Priority	3

	Type		male
	  Anim          male\Flashlight_Down 1
	  PlayFX	Generic\Flashlight_NoSpot.fx 0 Constant
	TEnd
MEnd

Move A_Emote_FlashLight_cycle_2
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS

	Type		male
	  Anim          male\Flashlight_Down 1
	  PlayFX	Generic\Flashlight_NoSpot.fx 0 Constant
	TEnd
MEnd

#############################################################
## FlashLight - UP
#############################################################

Move A_Emote_FlashLight_Up_Pre
	Requires	FLY EMOTE FLASHLIGHT UP
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_FlashLight_Up_Cycle
	Priority	4

	Type		male
	  Anim          male\Flashlight_Up 1
	  PlayFX	Generic\Flashlight_NoSpot.fx 0 Constant
	TEnd
MEnd

Move A_Emote_FlashLight_Up_Cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS
	Priority	1

	Type		male
	  Anim          male\Flashlight_Up 1
	  PlayFX	Generic\Flashlight_NoSpot.fx 0 Constant
	TEnd
MEnd


#############################################################
## FlashLight - DOWN
#############################################################

Move A_Emote_FlashLight_Down_Pre
	Requires	FLY EMOTE FLASHLIGHT DOWN
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_FlashLight_cycle_2
	Priority	4

	Type		male
	  Anim          male\Flashlight_Down 1
	  PlayFX	Generic\Flashlight_NoSpot.fx 0 Constant
	TEnd
MEnd

Move A_Emote_FlashLight_Down_Cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS
	Priority	1

	Type		male
	  Anim          male\Flashlight_Down 1
	  PlayFX	Generic\Flashlight_NoSpot.fx 0 Constant
	TEnd
MEnd

#############################################################
## Book research
#############################################################

Move A_Emote_Book_research
	Requires	FLY EMOTE BOOKRESEARCH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Book_research_cycle
	Priority	2
	Scale		0.5
	Flags		FIXARMREG

	Type		male
	  Anim          male\blackboard 1
	  PlayFx        GENERIC\book_left.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Book_research_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	Scale		0.5
	Flags		CYCLE REQINPUTS FIXARMREG

	Type		male
	  Anim          male\blackboard 1
	  PlayFx        GENERIC\book_left.fx 0 Constant
	TEnd
MEnd


#############################################################
## Book Research Low
#############################################################

Move A_Emote_Book_research_Low
	Requires	FLY EMOTE BOOKRESEARCH LOW
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Book_research_cycle_low
	Priority	3
	Scale		0.5
	Flags		FIXARMREG

	Type		male
	  Anim          male\blackboard_low 1
	  PlayFx        GENERIC\book_left.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Book_research_cycle_low
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS FIXARMREG
	Priority	1
	Scale		0.5

	Type		male
	  Anim          male\blackboard_low 1
	  PlayFx        GENERIC\book_left.fx 0 Constant
	TEnd
MEnd


#############################################################
## Float Books
#############################################################

Move A_Emote_Float_Books
	Requires	FLY EMOTE FLOATBOOKS
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Float_Books_cycle
	Priority	2
	Scale		0.5
	Flags		FIXARMREG

	Type		male
	  Anim          male\idle_2hips_look 1
	  PlayFx        GENERIC\cot_books.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Float_Books_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	Scale		0.5
	Flags		CYCLE REQINPUTS FIXARMREG

	Type		male
	  Anim          male\idle_2hips_look 1
	  PlayFx        GENERIC\cot_books.fx 0 Constant
	TEnd
MEnd


#############################################################
## Hold Torch
#############################################################

Move A_Emote_Hold_Torch
	Requires	FLY EMOTE HOLDTORCH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Hold_Torch_cycle
	Priority	2
	Scale		0.5
	Interpolate	15

	Type		male
	  Anim          male\holdtorch 1
	  PlayFx        Generic\Torch_left.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Hold_Torch_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS
	Priority	1
	Scale		0.5
	Interpolate	15

	Type		male
	  Anim          male\holdtorch 1
	  PlayFx        Generic\Torch_left.fx 0 Constant
	TEnd
MEnd


#############################################################
## Drum
#############################################################

Move A_Emote_Drum
	Requires	FLY EMOTE DRUM
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Drum_cycle
	Priority	2
	Flags		FIXARMREG

	Type		male
	  Anim          male\drum_high 1
	  PlayFx        GENERIC\drum_big.fx 0 Constant
	  PlayFx        Emotes\Drum_Large.fx 0 Constant
	TEnd

	Type		huge
	  Anim          male\drum_high 1
	  PlayFx        GENERIC\huge_drum_big.fx 0 Constant
	  PlayFx        Emotes\Drum_Large.fx 0 Constant
	TEnd

	Type		fem
	  Anim          male\drum_high 1
	  PlayFx        GENERIC\fem_drum_big.fx 0 Constant
	  PlayFx        Emotes\Drum_Large.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Drum_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	Flags		CYCLE REQINPUTS FIXARMREG

	Type		male
	  Anim          male\drum_high 1
	  PlayFx        GENERIC\drum_big.fx 0 Constant
	  PlayFx        Emotes\Drum_Large.fx 0 Constant
	TEnd

	Type		huge
	  Anim          male\drum_high 1
	  PlayFx        GENERIC\huge_drum_big.fx 0 Constant
	  PlayFx        Emotes\Drum_Large.fx 0 Constant
	TEnd

	Type		fem
	  Anim          male\drum_high 1
	  PlayFx        GENERIC\fem_drum_big.fx 0 Constant
	  PlayFx        Emotes\Drum_Large.fx 0 Constant
	TEnd
MEnd


#############################################################
## Drum Low
#############################################################

Move A_Emote_Drum_Low
	Requires	FLY EMOTE DRUM LOW
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Drum_Low_cycle
	Priority	3
	Flags		FIXARMREG

	Type		male
	  Anim          male\drum_low 1
	  PlayFx        Emotes\Drum_Small.fx 0 Constant
	TEnd

	Type		huge
	  Anim          male\drum_low 1
	  PlayFx        Emotes\Drum_Small_Huge.fx 0 Constant
	TEnd

	Type		fem
	  Anim          male\drum_low 1
	  PlayFx        Emotes\Drum_Small_Fem.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Drum_Low_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	Interpolate	15
	Flags		CYCLE REQINPUTS FIXARMREG

	Type		male
	  Anim          male\drum_low 1
	  PlayFx        Emotes\Drum_Small.fx 0 Constant
	TEnd

	Type		huge
	  Anim          male\drum_low 1
	  PlayFx        Emotes\Drum_Small_Huge.fx 0 Constant
	TEnd

	Type		fem
	  Anim          male\drum_low 1
	  PlayFx        Emotes\Drum_Small_Fem.fx 0 Constant
	TEnd
MEnd


#############################################################
## Tribal Dance
#############################################################

Move A_Emote_Tribal_Dance
	Requires	FLY EMOTE TRIBALDANCE
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Tribal_Dance_cycle_1
	Priority	2

	Type		male
	  Anim          male\tribal_dance 1
	TEnd
MEnd

Move A_Emote_Tribal_Dance_cycle_1
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS
	CycleMove	A_Emote_Tribal_Dance
	CycleMove	A_Emote_Tribal_Dance_cycle_1
	Priority	1

	Type		male
	  Anim          male\tribal_dance_alt 1
	TEnd
MEnd


#############################################################
## Assume Position
#############################################################

Move A_Emote_Assume_Position
	Requires	FLY EMOTE ASSUMEPOSITION
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Assume_Position_cycle
	Priority	2

	Type		male
	  Anim          male\AssumePosition_2 1
	TEnd
MEnd

Move A_Emote_Assume_Position_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS
	Priority	1

	Type		male
	  Anim          male\AssumePosition_2 1
	TEnd
MEnd


#############################################################
## Peer In
#############################################################

Move A_Emote_Peer_In
	Requires	FLY EMOTE PEERIN
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Peer_In_cycle_1
	Priority	2
	Flags		FIXARMREG

	Type		male
	  Anim          male\PeerIn 1 20
	TEnd
MEnd

Move A_Emote_Peer_In_cycle_1
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	A_Emote_Peer_In_cycle_2
	CycleMove	A_Emote_Peer_In_cycle_1
	CycleMove	A_Emote_Peer_In_cycle_1
	Priority	1
	Flags		CYCLE REQINPUTS FIXARMREG

	Type		male
	  Anim          male\PeerIn 20
	TEnd
MEnd

Move A_Emote_Peer_In_cycle_2
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	A_Emote_Peer_In
	Priority	1
	Flags		CYCLE REQINPUTS FIXARMREG

	Type		male
	  Anim          male\lookquick 1
	TEnd
MEnd


#############################################################
## "Bat Smash"
#############################################################

Move A_Emote_Bat_smash
	Requires	FLY EMOTE BATSMASH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Bat_smash_cycle
	Priority	1
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\bat_smash_simple 1 47
	  PlayFx        WEAPONS\BASEBALL_BAT.FX 0 Constant
	TEnd

	Type		beast
	  Anim          male\bat_smash_simple 1 47
	  PlayFx        GENERIC\meatbone.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Bat_smash_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE NOSIZESCALE REQINPUTS
	Priority	1

	Type		male
	  Anim          male\bat_smash_simple 1 47
	  PlayFx        WEAPONS\BASEBALL_BAT.FX 0 Constant
	TEnd

	Type		beast
	  Anim          male\bat_smash_simple 1 47
	  PlayFx        GENERIC\meatbone.fx 0 Constant
	TEnd
MEnd


#############################################################
## "Bat Smash" React
#############################################################

Move A_Emote_Bat_smash_react
	Requires	FLY EMOTE BATSMASHREACT
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Bat_smash_react_cycle
	Priority	2
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\bat_smash_react 1 47
	TEnd
MEnd

Move A_Emote_Bat_smash_react_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE NOSIZESCALE REQINPUTS
	Priority	1

	Type		male
	  Anim          male\bat_smash_react 1 47
	TEnd
MEnd


#############################################################
## Protest
#############################################################

Move A_Emote_Protest
	Requires	FLY EMOTE PROTEST
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Protest_cycle
	Priority	2
	Flags		NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\sign_chant 1
	  PlayFx        GENERIC\Protest_sign_A.fx 0 Constant
	TEnd

	Type		huge
	  Anim          male\sign_onearm_chant 1
	  PlayFx        GENERIC\Protest_sign_A.fx 0 Constant
	TEnd

	Type		5cw
	  Anim          male\sign_onearm_chant 1
	  PlayFx        GENERIC\Protest_sign_A.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Protest_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	Flags		CYCLE NOSIZESCALE REQINPUTS FIXARMREG

	Type		male
	  Anim          male\sign_chant 1
	  PlayFx        GENERIC\Protest_sign_A.fx 0 Constant
	TEnd

	Type		huge
	  Anim          male\sign_onearm_chant 1
	  PlayFx        GENERIC\Protest_sign_A.fx 0 Constant
	TEnd

	Type		5cw
	  Anim          male\sign_onearm_chant 1
	  PlayFx        GENERIC\Protest_sign_A.fx 0 Constant
	TEnd
MEnd


#############################################################
## Fear
#############################################################

Move A_Emote_Fear
	Requires	FLY EMOTE FEAR
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Fear_cycle
	Priority	3

	Type		male
	  Anim          male\air_fear_stun 1
	TEnd
MEnd

Move A_Emote_Fear_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS
	Priority	1

	Type		male
	  Anim          male\air_fear_stun 25 60
	TEnd
MEnd


#############################################################
## Slap React
#############################################################

Move A_Emote_Slap_React
	Requires	FLY EMOTE SLAPREACT
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Slap_React_2
	Priority	2
	Interpolate	15
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\combat_block 1 22
	TEnd
MEnd

Move A_Emote_Slap_React_2
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		NOSIZESCALE REQINPUTS
	NextMove	A_Emote_Slap_React_cycle
	Priority	1

	Type		male
	  Anim          male\stun_hit 2 96
	TEnd
MEnd

Move A_Emote_Slap_React_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS
	Priority	1
	Scale          0.5

	Type		male
	  Anim          male\stun_cycle 1 31
	TEnd
MEnd

#############################################################
## "Ala Kazam!"
#############################################################

Move A_Emote_AlaKazam
	Requires	FLY EMOTE ALAKAZAM
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	Interpolate	15
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\magic_L 1 ##30
	TEnd
MEnd


#############################################################
## "Ala Kazam!" Reaction
#############################################################

Move A_Emote_AlaKazam_React
	Requires	FLY EMOTE ALAKAZAMREACT
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_AlaKazam_React_hidden

	Type		male
	  Anim          male\freezestun 1 20
	  PlayFX	Emotes\CostumeChanges\PrestoChango\PrestoChango.fx 5
	  PlayFX	GENERIC\AlakazamReact.fx 8 CONSTANT
	  PlayFx	Generic\entityVanish.fx 6 CONSTANT
	TEnd
MEnd

Move A_Emote_AlaKazam_React_hidden
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_AlaKazam_React_cycle
	Flags		REQINPUTS
	Scale		0.5

	Type		male
	  Anim          male\freezestun 20
	  PlayFX	GENERIC\AlakazamReact.fx 0 CONSTANT
	  PlayFx	Generic\entityVanish.fx 0 CONSTANT
	TEnd
MEnd

Move A_Emote_AlaKazam_React_cycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_What
	Flags		REQINPUTS

	Type		male
	  Anim          male\lookquick 1
	  PlayFX	Emotes\CostumeChanges\PrestoChango\PrestoChango.fx 0
	TEnd
MEnd


#############################################################
## Drink
#############################################################

Move A_Emote_Drink
	Requires	FLY EMOTE DRINK
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Drink_cycle_A
	Priority	3
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

Move A_Emote_Drink_cycle_A
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	A_Emote_Drink_cycle_A
	CycleMove	A_Emote_Drink_cycle_B
	CycleMove	A_Emote_Drink_cycle_C
	CycleMove	A_Emote_Drink_cycle_A
	Priority	1
	Flags		CYCLE REQINPUTS FIXARMREG

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

Move A_Emote_Drink_cycle_B
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	A_Emote_Drink_cycle_A
	Priority	1
	Flags		CYCLE REQINPUTS FIXARMREG

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

Move A_Emote_Drink_cycle_C
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	A_Emote_Drink_cycle_A
	Priority	1
	Flags		CYCLE REQINPUTS FIXARMREG

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
## Talker
#############################################################

Move A_Emote_Talker
	Requires	FLY EMOTE GESTICULATE
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Talker_cycle_A
	Priority	3

	Type		male
	  Anim          male\PL_Talk01 1
	TEnd
MEnd

Move A_Emote_Talker_cycle_A
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	A_Emote_Talker_cycle_A
	CycleMove	A_Emote_Talker_cycle_B
	CycleMove	A_Emote_Talker_cycle_C
	CycleMove	A_Emote_Talker_cycle_A
	Priority	1
	Flags		CYCLE REQINPUTS

	Type		male
	  Anim          male\PL_TalkIdle 1
	TEnd
MEnd

Move A_Emote_Talker_cycle_B
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	A_Emote_Talker_cycle_C
	CycleMove	A_Emote_Talker_cycle_D
	CycleMove	A_Emote_Talker_cycle_A
	Priority	1
	Flags		CYCLE REQINPUTS

	Type		male
	  Anim          male\PL_Talk01 1
	TEnd
MEnd

Move A_Emote_Talker_cycle_C
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	A_Emote_Talker_cycle_B
	CycleMove	A_Emote_Talker_cycle_A
	CycleMove	A_Emote_Talker_cycle_D
	Priority	1
	Flags		CYCLE REQINPUTS

	Type		male
	  Anim          male\PL_Talk02 1
	TEnd
MEnd

Move A_Emote_Talker_cycle_D
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	A_Emote_Talker_cycle_A
	CycleMove	A_Emote_Talker_cycle_C
	CycleMove	A_Emote_Talker_cycle_B
	Priority	1
	Flags		CYCLE REQINPUTS

	Type		male
	  Anim          male\PL_Talk03 1
	TEnd
MEnd


#############################################################
## Yata Yata
#############################################################

Move A_Emote_YataYata
	Requires	FLY EMOTE YATA
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	3
	Scale		1.1

	Type		male
	  Anim          male\YataYata 1
	TEnd
MEnd

#############################################################
## Laugh Two
#############################################################

Move A_Emote_LAUGHTWO
	Requires	FLY EMOTE LAUGHTWO
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	3
	Interpolate	3

	Type		male
	  Anim          male\emote_Laugh2 1
	TEnd
MEnd

#############################################################
## Villain Laugh
#############################################################

Move A_Emote_VillainLaugh
	Requires	FLY EMOTE VILLAINLAUGH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	3
	Interpolate	3

	Type		male
	  Anim          male\emote_VillainLaugh 1
	TEnd
MEnd

#############################################################
## Slash Throat
#############################################################

Move A_Emote_SlashThroat
	Requires	FLY EMOTE SLASHTHROAT
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	3
	Flags		FIXARMREG

	Type		male
	  Anim          male\emote_SlashThroat 1
	TEnd
MEnd

#############################################################
## Curse You
#############################################################

Move A_Emote_CurseYou
	Requires	FLY EMOTE CURSEYOU
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	3
	Interpolate	4

	Type		male
	  Anim          male\emote_CurseYou 1 80
	TEnd
MEnd


#############################################################
## Sleep
#############################################################

Move A_Emote_Sleep
	Requires	FLY EMOTE ASLEEP
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_SleepLoop
	Priority	3
	Interpolate	7
	scale		0.9

	Type		male
	  Anim          male\emote_sleep 15 95
	TEnd
MEnd

Move A_Emote_SleepLoop
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE REQINPUTS
	Priority	1
	Interpolate     4

	Type		male
	  Anim          male\emote_Sleep 96 191
	  PlayFX	V_COV\Items\Emote_SleepZ.fx 0 Constant
	TEnd

MEnd

#############################################################
## Dust Off
#############################################################

Move A_Emote_DustOff
	Requires	FLY EMOTE DUSTOFF
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_DustOff2
	Priority	3
	scale		1.1
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

Move A_Emote_DustOff2
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	scale		1.1
	Flags		FIXARMREG

	Type		male
	  Anim          male\emote_DustOff 70
	TEnd

	Type		huge
	  Anim          huge\emote_DustOff 70
	TEnd

	Type		fem
	  Anim          fem\emote_DustOff 70
	TEnd
MEnd


#############################################################
##  Plotting
#############################################################

Move A_Emote_Plotting
	Requires	FLY EMOTE PLOTTING
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	3
	Flags		FIXARMREG
	NextMove	A_Emote_Plotting_Cycle

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


Move A_Emote_Plotting_Cycle
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
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

Move A_Emote_Chicken
	Requires	FLY EMOTE CHICKEN
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	#NextMove	A_Ready_Chicken
	Priority	3
	Interpolate	5
	Flags		FIXARMREG

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

#############################################################
## Laptop
#############################################################

Move A_Emote_Laptop
	Requires	FLY EMOTE LAPTOP
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Laptop_cycle_1
	Priority	3
	Flags		FIXARMREG

	Type		male
	  Anim          male\Portable_Laptop 1
	  PlayFX	Generic\Stand_LaptopTable.fx 0 Constant
	TEnd

	Type		huge
	  Anim          male\Portable_Laptop_Rest 1
	  PlayFX	Generic\huge_Stand_LaptopTable.fx 0 Constant
	TEnd

	Type		fem
	  Anim          male\Portable_Laptop_Rest 1
	  PlayFX	Generic\fem_Stand_LaptopTable.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Laptop_cycle_1
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	A_Emote_Laptop
	CycleMove	A_Emote_Laptop_cycle_1
	CycleMove	A_Emote_Laptop_cycle_2
	Flags		CYCLE REQINPUTS FIXARMREG

	Type		male
	  Anim          male\Portable_Laptop_Rest 1
	  PlayFX	Generic\Stand_LaptopTable.fx 0 Constant
	TEnd

	Type		huge
	  Anim          male\Portable_Laptop_Rest 1
	  PlayFX	Generic\huge_Stand_LaptopTable.fx 0 Constant
	TEnd

	Type		fem
	  Anim          male\Portable_Laptop_Rest 1
	  PlayFX	Generic\fem_Stand_LaptopTable.fx 0 Constant
	TEnd
MEnd

Move A_Emote_Laptop_cycle_2
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	A_Emote_Laptop
	CycleMove	A_Emote_Laptop_cycle_1
	Flags		CYCLE REQINPUTS FIXARMREG

	Type		male
	  Anim          male\Portable_Laptop_Delete 1
	  PlayFX	Generic\Stand_LaptopTable.fx 0 Constant
	TEnd

	Type		huge
	  Anim          male\Portable_Laptop_Rest 1
	  PlayFX	Generic\huge_Stand_LaptopTable.fx 0 Constant
	TEnd

	Type		fem
	  Anim          male\Portable_Laptop_Rest 1
	  PlayFX	Generic\fem_Stand_LaptopTable.fx 0 Constant
	TEnd
MEnd

#############################################################
## Touch Screen
#############################################################

Move A_Emote_ScreenIdle
	Requires	FLY EMOTE SCREEN
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_ScreenIdleLoop
	Priority	3
	Interpolate	15
	Flags		FIXARMREG

	Type		male
	  Anim          male\emote_TouchScreen 1 117
	TEnd
MEnd

Move A_Emote_ScreenIdleLoop
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	A_Emote_TouchScreenHit
	CycleMove	A_Emote_TouchScreen
	Priority	3
	Flags		CYCLE REQINPUTS FIXARMREG

	Type		male
	  Anim          male\emote_TouchScreen 110
	TEnd
MEnd

Move A_Emote_TouchScreen
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	A_Emote_ScreenIdleLoop
	CycleMove	A_Emote_ScreenIdleLoop
	CycleMove	A_Emote_TouchScreenHit
	Priority	3
	Flags		CYCLE REQINPUTS FIXARMREG

	Type		male
	  Anim          male\emote_TouchScreen 1 117
	TEnd
MEnd

Move A_Emote_TouchScreenHit
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	A_Emote_ScreenIdleLoop
	CycleMove	A_Emote_TouchScreen
	Priority	3
	Flags		CYCLE REQINPUTS FIXARMREG

	Type		male
	  Anim          male\emote_TouchScreenHit 1
	TEnd
MEnd


#############################################################
## Go Away
#############################################################

Move A_Emote_GoAway
	Requires	FLY EMOTE GOAWAY
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	Nextmove	Ready_2Hips ## looks good enough on fem
	Priority	3

	Type		male
	  Anim          male\Emote_GoAway 1
	TEnd
MEnd


#############################################################
## Talk to the hand
#############################################################

Move A_Emote_TalktoHand
	Requires	FLY EMOTE TALKTOHAND
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	Nextmove	Ready_2Hips ## looks good enough on fem
	Priority	3
	Scale		0.8

	Type		male
	  Anim          male\Emote_TalktoHand 1
	TEnd
MEnd


#############################################################
## Kneel before ZOD
#############################################################

Move A_Emote_Bow_Down
	Requires	FLY EMOTE BOWDOWN
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	Nextmove	Ready_2Hips ## looks good enough on fem
	Priority	3
	Scale		0.8

	Type		male
	  Anim		male\Emote_BowDown 1
	TEnd
MEnd


#############################################################
## Forward slap
#############################################################

Move A_Emote_Slap
	Requires	FLY EMOTE SLAP
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_NoSpam_Air.inc
	Nextmove	Ready_2Hips ## looks good enough on fem
	Priority	3

	Type		male
	  Anim          male\Emote_Slap 1
	  Playfx	generic\Slap1.fx
	TEnd
MEnd


#############################################################
## Backhand Smack
#############################################################

Move A_Emote_Smack
	Requires	FLY EMOTE SMACK
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_NoSpam_Air.inc
	Nextmove	Ready_2Hips ## looks good enough on fem
	Priority	3

	Type		male
	  Anim          male\Emote_Smack 3
	  Playfx	generic\Smack1.fx
	TEnd
MEnd


#############################################################
## Pimphand threaten
#############################################################

Move A_Emote_SmackThreat
	Requires	FLY EMOTE STRONGHAND
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	Nextmove	Ready_2Hips ## looks good enough on fem

	Type		male
	  Anim          male\Emote_Stronghand 1
	TEnd
MEnd


#############################################################
## Scheme
#############################################################

Move A_Scheme
	Requires	FLY EMOTE SCHEME
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	Nextmove	A_Scheme_cycle
	Priority	3
	Scale		1.5
	Flags		FIXARMREG

	Type		male
	  Anim          male\emote_scheme 1 49
	TEnd

	Type		fem
	  Anim          fem\emote_scheme 1 49
	TEnd

	Type		huge
	  Anim          huge\emote_scheme 1 49
	TEnd
MEnd

Move A_Scheme_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	Interpolate     2
	Flags		CYCLE FIXARMREG REQINPUTS

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

Move A_Emote_KissMyButt
	Requires	FLY EMOTE KISSMYBUTT
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	Nextmove	Ready_2Hips ## looks good enough on fem
	Priority	3
	Flags		FIXARMREG

	Type		male
	  Anim          male\Emote_KissMyButt 1
	TEnd
MEnd


#############################################################
## Crack Knuckles
#############################################################

Move A_Emote_CrackKnuckles
	Requires	FLY EMOTE KNUCKLECRACK
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	Nextmove	Ready_2Hips
	Priority	3
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
## Roll Dice #1
#############################################################

Move A_1_RollDice_pre
	Requires	FLY EMOTE ROLLDICE HIGH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_1_RollDice_cast
	Priority	4
	Scale		1.5
	Interpolate	12
	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\roll_dice 125 201  ##209
	TEnd
MEnd

Move A_1_RollDice_cast
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	postReady_back
	Priority	1
	Flags		REQINPUTS

	Type		male
	  Anim          male\roll_dice 1 120
	  PlayFx        GENERIC\Dice_1.FX  80 Constant
	  PlayFx        GENERIC\Prop_Dice.FX  69
	TEnd
MEnd


#############################################################
##  Roll Dice #2
#############################################################

Move A_2_RollDice_pre
	Requires	FLY EMOTE ROLLDICE HIGH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_2_RollDice_cast
	Priority	4
	Scale		1.5
	Interpolate	12
	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\roll_dice 125 201  ##209
	TEnd
MEnd

Move A_2_RollDice_cast
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	postReady_back
	Priority	1

	Type		male
	  Anim          male\roll_dice 1 120
	  PlayFx        GENERIC\Dice_2.FX  80 Constant
	  PlayFx        GENERIC\Prop_Dice.FX  69
	TEnd
MEnd


#############################################################
## Roll Dice #3
#############################################################

Move A_3_RollDice_pre
	Requires	FLY EMOTE ROLLDICE HIGH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_3_RollDice_cast
	Priority	4
	Scale		1.5
	Interpolate	12
	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\roll_dice 125 201  ##209
	TEnd
MEnd


Move A_3_RollDice_cast
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	postReady_back
	Priority	1
	Flags		REQINPUTS

	Type		male
	  Anim          male\roll_dice 1 120
	  PlayFx        GENERIC\Dice_3.FX  80 Constant
	  PlayFx        GENERIC\Prop_Dice.FX  69
	TEnd
MEnd


#############################################################
## Roll Dice #4
#############################################################

Move A_4_RollDice_pre
	Requires	FLY EMOTE ROLLDICE HIGH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_4_RollDice_cast
	Priority	4
	Scale		1.5
	Interpolate	12
	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\roll_dice 125 201  ##209
	TEnd
MEnd

Move A_4_RollDice_cast
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	postReady_back
	Priority	1
	Flags		REQINPUTS

	Type		male
	  Anim          male\roll_dice 1 120
	  PlayFx        GENERIC\Dice_4.FX  80 Constant
	  PlayFx        GENERIC\Prop_Dice.FX  69
	TEnd
MEnd


#############################################################
##  Roll Dice #5
#############################################################

Move A_5_RollDice_pre
	Requires	FLY EMOTE ROLLDICE HIGH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_5_RollDice_cast
	Priority	4
	Scale		1.5
	Interpolate	12
	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\roll_dice 125 201  ##209
	TEnd
MEnd

Move A_5_RollDice_cast
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	postReady_back
	Priority	1
	Flags		REQINPUTS

	Type		male
	  Anim          male\roll_dice 1 120
	  PlayFx        GENERIC\Dice_5.FX  80 Constant
	  PlayFx        GENERIC\Prop_Dice.FX  69
	TEnd
MEnd


#############################################################
## Roll Dice #6
#############################################################

Move A_6_RollDice_pre
	Requires	FLY EMOTE ROLLDICE HIGH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_6_RollDice_cast
	Priority	4
	Scale		1.5
	Interpolate	12
	Flags		PICKRANDOMLY

	Type		male
	  Anim          male\roll_dice 125 201  ##209
	TEnd
MEnd

Move A_6_RollDice_cast
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	postReady_back
	Priority	1
	Flags		REQINPUTS

	Type		male
	  Anim          male\roll_dice 1 120
	  PlayFx        GENERIC\Dice_6.FX  80 Constant
	  PlayFx        GENERIC\Prop_Dice.FX 69
	TEnd
MEnd


#############################################################
## Roll Dice #7
#############################################################

Move A_7_RollDice_pre
	Requires	FLY EMOTE DICE HIGH
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_7_RollDice_cast
	Priority	4
	Scale		1.5
	Interpolate	12

	Type		male
	  Anim          male\roll_dice 125 201  ##209
	TEnd
MEnd

Move A_7_RollDice_cast
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	postReady_back
	Priority	1
	Flags		REQINPUTS

	Type		male
	  Anim          male\roll_dice 1 120
	  PlayFx        GENERIC\Dice_7.FX  80 Constant
	  PlayFx        GENERIC\Prop_Dice.FX 69
	TEnd
MEnd


#############################################################
## Wedding - Marriage Proposal
#############################################################

Move A_Emote_MarriageProposal_Pre
	Requires	FLY EMOTE PROPOSE
	Priority        3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_MarriageProposal_Cycle

	Type		male
	  Anim          male\emote_MarriageProposal 25 119
	TEnd
MEnd

Move A_Emote_MarriageProposal_Cycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE
	NextMove	postReady_back

	Type		male
	  Anim          male\emote_MarriageProposal 120 179
	TEnd
MEnd


#############################################################
## Wedding - Toss Rice
#############################################################

Move A_Emote_ThrowRice
	Requires	FLY EMOTE UNDERHANDTOSS ONE
	Priority        4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	FlyReady

	Type		male
	  Anim          male\air_emote_UnderhandToss 2
	  PlayFx        Emotes\ThrowRice.fx 30 Constant
	TEnd
MEnd


#############################################################
## Wedding - Throw Confetti
#############################################################

Move A_Emote_ThrowConfetti
	Requires	FLY EMOTE UNDERHANDTOSS TWO
	Priority        4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	FlyReady

	Type		male
	  Anim          male\air_emote_UnderhandToss 2
	  PlayFx        Emotes\ThrowConfetti.fx 31 Constant
	TEnd
MEnd

#############################################################
## Wedding - Throw Rose Petals
#############################################################

Move A_Emote_ThrowRosePetals
	Requires	FLY EMOTE UNDERHANDTOSS THREE
	Priority        4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	FlyReady

	Type		male
	  Anim          male\air_emote_UnderhandToss 2
	  PlayFx        Emotes\ThrowRosePetals.fx 30 Constant
	TEnd
MEnd

#############################################################
## Robot Dance
#############################################################

Move A_DanceRobot_Pre
	Requires	FLY EMOTE DANCE LOW CYBORGPACK
	Priority	5
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_DanceRobot_Cycle

	Type		male
	  Anim          male\RoboDance 1
	TEnd
MEnd

Move A_DanceRobot_Cycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE

	Type		male
	  Anim          male\RoboDance 1
	TEnd
MEnd


#############################################################
## Robot Power Down
#############################################################

Move A_Emote_RoboPowerDown
	Requires	FLY EMOTE SLEEP DOWN CYBORGPACK
	Priority        5
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_RoboPowerDown_PostCycle

	Type		male
	  Anim          male\RoboPowerDown 1
	TEnd
MEnd

Move A_Emote_RoboPowerDown_PostCycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE

	Type		male
	  Anim          male\RoboPowerDown 120
	TEnd
MEnd


#############################################################
## Robot Power Up
#############################################################

Move A_Emote_RoboPowerUp
	Requires	FLY EMOTE SLEEP UP CYBORGPACK
	Priority        5
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_RoboPowerUp_PostCycle

	Type		male
	  Anim          male\RoboPowerUp 1
	TEnd
MEnd

Move A_Emote_RoboPowerUp_PostCycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Interpolate	15
	Flags		CYCLE
	Scale		0.5

	Type		male
	  Anim          male\shield_Ready 1 31
	TEnd
MEnd

#############################################################
## Magic Cards
#############################################################

Move A_Emote_MagicCards_Pre
	Requires	FLY EMOTE MAGIC CARDS TRICK
	Priority        5
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_MagicCards
	Flags		NoSizeScale FixArmReg

	Type		male
	  Anim          male\air_Magic_Cards 1 10
	  ##PlayFX	Emotes\CardTrick\CardTrick_Male.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\air_Magic_Cards 1 10
	  ##PlayFX	Emotes\CardTrick\CardTrick_Huge.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\air_Magic_Cards 1 10
	  ##PlayFX	Emotes\CardTrick\CardTrick_Fem.fx 0 CONSTANT
	TEnd
MEnd

Move A_Emote_MagicCards
	Requires	FLY
	Priority        1
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_MagicCards_Toss
	Flags		NoSizeScale FixArmReg ReqInputs

	Type		male
	  Anim          male\air_Magic_Cards 10 252
	  PlayFX	Emotes\CardTrick\CardTrick_Male.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\air_Magic_Cards 10 252
	  PlayFX	Emotes\CardTrick\CardTrick_Huge.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\air_Magic_Cards 10 252
	  PlayFX	Emotes\CardTrick\CardTrick_Fem.fx 0 CONSTANT
	TEnd
MEnd


Move A_Emote_MagicCards_Toss
	Requires	FLY
	Priority        1
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_MagicCards_PostCycle
	Flags		NoSizeScale FixArmReg ReqInputs

	Type		male
	  Anim          male\air_Magic_Cards 252
	  PlayFX	Emotes\CardTrick\CardFountain.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\air_Magic_Cards 252
	  PlayFX	Emotes\CardTrick\CardFountain.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\air_Magic_Cards 252
	  PlayFX	Emotes\CardTrick\CardFountain.fx 0 CONSTANT
	TEnd
MEnd

Move A_Emote_MagicCards_PostCycle
	Requires	FLY
	Priority        1
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	PreFall
	Flags		Cycle

	Type		male
	  Anim		male\float2 1
	  PlayFX	Emotes\CardTrick\CardFountain.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim		huge\float 1
	  PlayFX	Emotes\CardTrick\CardFountain.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim		fem\fly_ready 1
	  PlayFX	Emotes\CardTrick\CardFountain.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
## Backflip
#############################################################

Move A_Emote_Backflip_Pre
	Requires	FLY EMOTE BACKFLIP
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> #<IRQEmotesShield>
	Priority	3
	Include sequencers\Members\Emotes_Idle_Air.inc
	Nextmove	A_Emote_Backflip_PostCycle

	Type		male
	  Anim          male\emote_backflip_air 1 63
	TEnd
MEnd

Move A_Emote_Backflip_PostCycle
	Requires	FLY
	Priority	1
	Flags		CYCLE
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	PreFall

	Type		male
	  Anim          male\emote_backflip_air_cycle 1 56
	TEnd
MEnd

#############################################################

Move A_Emote_Shield_Backflip
	Requires	FLY EMOTE BACKFLIP SHIELD
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	Nextmove	A_Emote_Shield_Backflip_PostCycle
	Flags 		FIXARMREG

	Type		male
	  Anim          male\emote_backflip_shield_air 1 63
	TEnd
MEnd

Move A_Emote_Shield_Backflip_PostCycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	PreFall
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\emote_backflip_shield_air_cycle 1 56
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
	  PlayFX	NPCFX\FacePalm_SFX.fx 12
	TEnd

	Type		huge
	  Anim          huge\air_emote_facepalm 1 20
	  PlayFX	NPCFX\FacePalm_SFX.fx 12
	TEnd

	Type		fem
	  Anim          fem\air_emote_facepalm 1 20
	  PlayFX	NPCFX\FacePalm_SFX.fx 12
	TEnd

MEnd

Move A_Emote_Facepalm_Post
	Include sequencers\Members\Emotes_Idle_Air.inc
	Interpolate	8
	Flags		NOSIZESCALE
	NextMove	FlyReady
	Requires	FLY

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
	  PlayFX	NPCFX\FacePalmDouble_SFX.fx 12
	TEnd

	Type		huge
	  Anim          huge\air_emote_double_facepalm 1 20
	  PlayFX	NPCFX\FacePalmDouble_SFX.fx 12
	TEnd

	Type		fem
	  Anim          fem\air_emote_double_facepalm 1 20
	  PlayFX	NPCFX\FacePalmDouble_SFX.fx 12
	TEnd

MEnd

Move A_Emote_Double_Facepalm_Post
	Requires	FLY
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

#############################################################
##	Emote_Ghoul Flex Emote
#############################################################

Move A_Emote_Ghoul_Flex_Cycle_Pre
	Requires	FLY EMOTE YELL FLEX ONE
	Priority	5
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Ghoul_Flex_Cycle_A

	Type		male
	  Anim          male\air_ghouls_ready_A 1
	#  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 1
	#  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 20
	TEnd

MEnd

Move A_Emote_Ghoul_Flex_Cycle_A
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		Cycle
	CycleMove	A_Emote_Ghoul_Flex_Cycle_A
	CycleMove	A_Emote_Ghoul_Flex_Cycle_A
	CycleMove	A_Emote_Ghoul_Flex_Cycle_break_A

	Type		male
	  Anim          male\air_ghouls_ready_A 1
	#  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 1
	#  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 20
	TEnd

MEnd

Move A_Emote_Ghoul_Flex_Cycle_break_A
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		Cycle
	CycleMove	A_Emote_Ghoul_Flex_Cycle_B

	Type		male
	  Anim          male\air_ghouls_idle_break_A 1
	#  PlayFX	ENEMYFX\Ghouls\Ghoul_BreakA_SFX.fx
	#  PlayFX	ENEMYFX\Ghouls\Ghoul_Run_SFX.fx 50
	#  PlayFX	ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 105
	TEnd

MEnd

Move A_Emote_Ghoul_Flex_Cycle_B
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		Cycle
	CycleMove	A_Emote_Ghoul_Flex_Cycle_B
	CycleMove	A_Emote_Ghoul_Flex_Cycle_B
	CycleMove	A_Emote_Ghoul_Flex_Cycle_break_B
	CycleMove	A_Emote_Ghoul_Flex_Cycle_break_C

	Type		male
	  Anim          male\air_ghouls_ready_B 1
#	  PlayFX	ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 1
	#  PlayFX	ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 20
	TEnd

MEnd

Move A_Emote_Ghoul_Flex_Cycle_break_B
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		Cycle
	CycleMove	A_Emote_Ghoul_Flex_Cycle_B

	Type		male
	  Anim          male\air_ghouls_idle_break_B 1
	#  PlayFX	ENEMYFX\Ghouls\Ghoul_BreakB_SFX.fx
	#  PlayFX	ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 50
	TEnd

MEnd

Move A_Emote_Ghoul_Flex_Cycle_break_C
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		Cycle
	CycleMove	A_Emote_Ghoul_Flex_Cycle_A

	Type		male
	  Anim          male\air_ghouls_idle_break_C 1
	#  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 1
	#  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 20
	#  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 50
	#  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 70
	#  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 100
	#  PlayFX		ENEMYFX\Ghouls\Ghoul_Ready_SFX.fx 120
	TEnd

MEnd

##############################
###   Reward Emotes
##############################
Move A_Emote_Ultimate_Power
	Requires	FLY EMOTE YELL REWARD
	Priority	13
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Ultimate_Power_Post
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_emote_ultimate_power 1 63
	  PlayFX	Emotes\CostumeChanges\Lightning\LightningBlast.fx 15
	TEnd

MEnd

Move A_Emote_Ultimate_Power_Post
	NextMove        FlyReady
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_emote_ultimate_power 64
	TEnd

MEnd
############################################################
Move A_Emote_Catch_Breath
	Requires	FLY EMOTE POSE REWARD
	Priority	13
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Catch_Breath_Post
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_emote_catch_breath 1 30
	TEnd

	Type		huge
	  Anim          huge\air_emote_catch_breath 1 30
	TEnd

	Type		fem
	  Anim          fem\air_emote_catch_breath 1 30
	TEnd

MEnd

Move A_Emote_Catch_Breath_Post
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		NoSizeScale Cycle
	NextMove        FlyReady
	Requires	FLY
	CycleMove	A_Emote_Catch_Breath_Post

	Type		male
	  Anim          male\air_emote_catch_breath 30
	TEnd

	Type		huge
	  Anim          huge\air_emote_catch_breath 30
	TEnd

	Type		fem
	  Anim          fem\air_emote_catch_breath 30
	TEnd

MEnd

#############################################################
## Hero Loyalty A_Emote
#############################################################

Move A_Emote_Hero_Loyalty
	Requires	FLY EMOTE LOYALTY ONE
	Priority	14
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Hero_Loyalty_Pre_Pre
	Flags		FIXARMREG

	Type		male
	  Anim          male\a_yes 1 30
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd

	Type		huge
	  Anim          huge\a_yes_hips 1 30
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd

	Type		fem
	  Anim          fem\a_yes_hips 1 30
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd
MEnd

Move A_Emote_Hero_Loyalty_Pre_Pre
	Include sequencers\Members\Emotes_Idle_Air.inc
	Requires	FLY
	Priority	1
	Flags		FIXARMREG
	NextMove	A_Emote_Hero_Loyalty_cycle

	Type		male
	  Anim          male\a_yes 30 90
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd

	Type		huge
	  Anim          huge\a_yes_hips 30 90
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd

	Type		fem
	  Anim          fem\a_yes_hips 30 69
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd
MEnd

Move A_Emote_Hero_Loyalty_cycle
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE FIXARMREG
	Requires	FLY
	Priority	1
	CycleMove	A_Emote_Hero_Loyalty_Nod
	CycleMove	A_Emote_Hero_Loyalty_cycle
	CycleMove	A_Emote_Hero_Loyalty_Wave
	CycleMove	A_Emote_Hero_Loyalty_cycle


	Type		male
	  Anim          male\a_yes 91 95
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd

	Type		huge
	  Anim          huge\a_yes_hips 91 95
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd

	Type		fem
	  Anim          fem\a_yes_hips 70 75
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd
MEnd

Move A_Emote_Hero_Loyalty_Nod
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE FIXARMREG
	CycleMove	A_Emote_Hero_Loyalty_Nod
	CycleMove	A_Emote_Hero_Loyalty_Nod
	CycleMove	A_Emote_Hero_Loyalty_Wave
	CycleMove	A_Emote_Hero_Loyalty_Wave_Right


	Type		male
	  Anim          male\a_yes_nod 1
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd

	Type		huge
	  Anim          male\a_yes_nod 1
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd

	Type		fem
	  Anim          male\a_yes_nod 1
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd
MEnd

Move A_Emote_Hero_Loyalty_Wave
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE FIXARMREG
	CycleMove	A_Emote_Hero_Loyalty_Nod
	CycleMove	A_Emote_Hero_Loyalty_Wave
	CycleMove	A_Emote_Hero_Loyalty_Wave_Left
	CycleMove	A_Emote_Hero_Loyalty_Nod


	Type		male
	  Anim          male\a_yes_wave 1
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd

	Type		huge
	  Anim          male\a_yes_wave 1
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd

	Type		fem
	  Anim          male\a_yes_wave 1
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd
MEnd

Move A_Emote_Hero_Loyalty_Wave_Right
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE FIXARMREG
	CycleMove	A_Emote_Hero_Loyalty_Wave_Left
	CycleMove	A_Emote_Hero_Loyalty_Wave
	CycleMove	A_Emote_Hero_Loyalty_Nod
	CycleMove	A_Emote_Hero_Loyalty_Nod


	Type		male
	  Anim          male\a_yes_wave_right_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd

	Type		huge
	  Anim          male\a_yes_wave_right_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd

	Type		fem
	  Anim          male\a_yes_wave_right_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd
MEnd


Move A_Emote_Hero_Loyalty_Wave_Left
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE FIXARMREG
	CycleMove	A_Emote_Hero_Loyalty_Wave_Left
	CycleMove	A_Emote_Hero_Loyalty_Nod
	CycleMove	A_Emote_Hero_Loyalty_Wave_Right
	CycleMove	A_Emote_Hero_Loyalty_Nod


	Type		male
	  Anim          male\a_yes_wave_left_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd

	Type		huge
	  Anim          male\a_yes_wave_left_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd

	Type		fem
	  Anim          male\a_yes_wave_left_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Hero.fx" 0 constant
	TEnd
MEnd

#############################################################
## Villain Loyalty A_Emote
#############################################################

Move A_Emote_Villain_Loyalty
	Requires	FLY EMOTE LOYALTY TWO
	Priority	14
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Villain_Loyalty_Pre_Pre
	Flags		FIXARMREG

	Type		male
	  Anim          male\a_yes 1 30
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd

	Type		huge
	  Anim          huge\a_yes_hips 1 30
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd

	Type		fem
	  Anim          fem\a_yes_hips 1 30
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd
MEnd

Move A_Emote_Villain_Loyalty_Pre_Pre
	Include sequencers\Members\Emotes_Idle_Air.inc
	Requires	FLY
	Flags		REQINPUTS FIXARMREG
	NextMove	A_Emote_Villain_Loyalty_cycle

	Type		male
	  Anim          male\a_yes 30 90
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd

	Type		huge
	  Anim          huge\a_yes_hips 30 90
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd

	Type		fem
	  Anim          fem\a_yes_hips 30 69
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd
MEnd

Move A_Emote_Villain_Loyalty_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE FIXARMREG
	CycleMove	A_Emote_Villain_Loyalty_Nod
	CycleMove	A_Emote_Villain_Loyalty_cycle
	CycleMove	A_Emote_Villain_Loyalty_Wave
	CycleMove	A_Emote_Villain_Loyalty_cycle


	Type		male
	  Anim          male\a_yes 91 95
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd

	Type		huge
	  Anim          huge\a_yes_hips 91 95
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd

	Type		fem
	  Anim          fem\a_yes_hips 70 75
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd
MEnd

Move A_Emote_Villain_Loyalty_Nod
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE FIXARMREG
	CycleMove	A_Emote_Villain_Loyalty_Nod
	CycleMove	A_Emote_Villain_Loyalty_Nod
	CycleMove	A_Emote_Villain_Loyalty_Wave
	CycleMove	A_Emote_Villain_Loyalty_Wave_Right


	Type		male
	  Anim          male\a_yes_nod 1
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd

	Type		huge
	  Anim          male\a_yes_nod 1
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd

	Type		fem
	  Anim          male\a_yes_nod 1
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd
MEnd

Move A_Emote_Villain_Loyalty_Wave
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE FIXARMREG
	CycleMove	A_Emote_Villain_Loyalty_Nod
	CycleMove	A_Emote_Villain_Loyalty_Nod
	CycleMove	A_Emote_Villain_Loyalty_Wave_Left
	CycleMove	A_Emote_Villain_Loyalty_Wave


	Type		male
	  Anim          male\a_yes_wave 1
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd

	Type		huge
	  Anim          male\a_yes_wave 1
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd

	Type		fem
	  Anim          male\a_yes_wave 1
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd
MEnd

Move A_Emote_Villain_Loyalty_Wave_Right
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE FIXARMREG
	CycleMove	A_Emote_Villain_Loyalty_Wave_Left
	CycleMove	A_Emote_Villain_Loyalty_Wave
	CycleMove	A_Emote_Villain_Loyalty_Nod
	CycleMove	A_Emote_Villain_Loyalty_Nod


	Type		male
	  Anim          male\a_yes_wave_right_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd

	Type		huge
	  Anim          male\a_yes_wave_right_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd

	Type		fem
	  Anim          male\a_yes_wave_right_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd
MEnd


Move A_Emote_Villain_Loyalty_Wave_Left
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE FIXARMREG
	CycleMove	A_Emote_Villain_Loyalty_Wave_Left
	CycleMove	A_Emote_Villain_Loyalty_Nod
	CycleMove	A_Emote_Villain_Loyalty_Wave_Right
	CycleMove	A_Emote_Villain_Loyalty_Nod


	Type		male
	  Anim          male\a_yes_wave_left_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd

	Type		huge
	  Anim          male\a_yes_wave_left_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd

	Type		fem
	  Anim          male\a_yes_wave_left_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Villain.fx" 0 constant
	TEnd
MEnd

#############################################################
## Vigilante Loyalty A_Emote
#############################################################

Move A_Emote_Vigilante_Loyalty
	Requires	FLY EMOTE LOYALTY FOUR
	Priority	14
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Vigilante_Loyalty_Pre_Pre
	Flags		FIXARMREG

	Type		male
	  Anim          male\a_yes 1 30
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd

	Type		huge
	  Anim          huge\a_yes_hips 1 30
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd

	Type		fem
	  Anim          fem\a_yes_hips 1 30
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd
MEnd

Move A_Emote_Vigilante_Loyalty_Pre_Pre
	Include sequencers\Members\Emotes_Idle_Air.inc
	Requires	FLY
	Flags		REQINPUTS FIXARMREG
	NextMove	A_Emote_Vigilante_Loyalty_cycle

	Type		male
	  Anim          male\a_yes 30 90
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd

	Type		huge
	  Anim          huge\a_yes_hips 30 90
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd

	Type		fem
	  Anim          fem\a_yes_hips 30 69
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd
MEnd

Move A_Emote_Vigilante_Loyalty_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE FIXARMREG
	CycleMove	A_Emote_Vigilante_Loyalty_Nod
	CycleMove	A_Emote_Vigilante_Loyalty_cycle
	CycleMove	A_Emote_Vigilante_Loyalty_Wave
	CycleMove	A_Emote_Vigilante_Loyalty_cycle


	Type		male
	  Anim          male\a_yes 91 95
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd

	Type		huge
	  Anim          huge\a_yes_hips 91 95
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd

	Type		fem
	  Anim          fem\a_yes_hips 70 75
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd
MEnd

Move A_Emote_Vigilante_Loyalty_Nod
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE FIXARMREG
	CycleMove	A_Emote_Vigilante_Loyalty_Nod
	CycleMove	A_Emote_Vigilante_Loyalty_Nod
	CycleMove	A_Emote_Vigilante_Loyalty_Wave
	CycleMove	A_Emote_Vigilante_Loyalty_Wave_Right


	Type		male
	  Anim          male\a_yes_nod 1
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd

	Type		huge
	  Anim          male\a_yes_nod 1
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd

	Type		fem
	  Anim          male\a_yes_nod 1
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd
MEnd

Move A_Emote_Vigilante_Loyalty_Wave
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE FIXARMREG
	CycleMove	A_Emote_Vigilante_Loyalty_Nod
	CycleMove	A_Emote_Vigilante_Loyalty_Nod
	CycleMove	A_Emote_Vigilante_Loyalty_Wave_Left
	CycleMove	A_Emote_Vigilante_Loyalty_Wave


	Type		male
	  Anim          male\a_yes_wave 1
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd

	Type		huge
	  Anim          male\a_yes_wave 1
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd

	Type		fem
	  Anim          male\a_yes_wave 1
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd
MEnd

Move A_Emote_Vigilante_Loyalty_Wave_Right
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE FIXARMREG
	CycleMove	A_Emote_Vigilante_Loyalty_Wave_Left
	CycleMove	A_Emote_Vigilante_Loyalty_Wave
	CycleMove	A_Emote_Vigilante_Loyalty_Nod
	CycleMove	A_Emote_Vigilante_Loyalty_Nod


	Type		male
	  Anim          male\a_yes_wave_right_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd

	Type		huge
	  Anim          male\a_yes_wave_right_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd

	Type		fem
	  Anim          male\a_yes_wave_right_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd
MEnd


Move A_Emote_Vigilante_Loyalty_Wave_Left
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE FIXARMREG
	CycleMove	A_Emote_Vigilante_Loyalty_Wave_Left
	CycleMove	A_Emote_Vigilante_Loyalty_Nod
	CycleMove	A_Emote_Vigilante_Loyalty_Wave_Right
	CycleMove	A_Emote_Vigilante_Loyalty_Nod


	Type		male
	  Anim          male\a_yes_wave_left_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd

	Type		huge
	  Anim          male\a_yes_wave_left_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd

	Type		fem
	  Anim          male\a_yes_wave_left_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Vigilante.fx" 0 constant
	TEnd
MEnd

#############################################################
## Rogue Loyalty A_Emote
#############################################################

Move A_Emote_Rogue_Loyalty
	Requires	FLY EMOTE LOYALTY THREE
	Priority	14
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Rogue_Loyalty_Pre_Pre
	Flags		FIXARMREG

	Type		male
	  Anim          male\a_yes 1 30
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd

	Type		huge
	  Anim          huge\a_yes_hips 1 30
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd

	Type		fem
	  Anim          fem\a_yes_hips 1 30
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd
MEnd

Move A_Emote_Rogue_Loyalty_Pre_Pre
	Include sequencers\Members\Emotes_Idle_Air.inc
	Requires	FLY
	Flags		REQINPUTS FIXARMREG
	NextMove	A_Emote_Rogue_Loyalty_cycle

	Type		male
	  Anim          male\a_yes 30 90
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd

	Type		huge
	  Anim          huge\a_yes_hips 30 90
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd

	Type		fem
	  Anim          fem\a_yes_hips 30 69
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd
MEnd

Move A_Emote_Rogue_Loyalty_cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE FIXARMREG
	CycleMove	A_Emote_Rogue_Loyalty_Nod
	CycleMove	A_Emote_Rogue_Loyalty_cycle
	CycleMove	A_Emote_Rogue_Loyalty_Wave
	CycleMove	A_Emote_Rogue_Loyalty_cycle


	Type		male
	  Anim          male\a_yes 91 95
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd

	Type		huge
	  Anim          huge\a_yes_hips 91 95
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd

	Type		fem
	  Anim          fem\a_yes_hips 70 75
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd
MEnd

Move A_Emote_Rogue_Loyalty_Nod
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE FIXARMREG
	CycleMove	A_Emote_Rogue_Loyalty_Nod
	CycleMove	A_Emote_Rogue_Loyalty_Nod
	CycleMove	A_Emote_Rogue_Loyalty_Wave
	CycleMove	A_Emote_Rogue_Loyalty_Wave_Right


	Type		male
	  Anim          male\a_yes_nod 1
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd

	Type		huge
	  Anim          male\a_yes_nod 1
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd

	Type		fem
	  Anim          male\a_yes_nod 1
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd
MEnd

Move A_Emote_Rogue_Loyalty_Wave
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE FIXARMREG
	CycleMove	A_Emote_Rogue_Loyalty_Nod
	CycleMove	A_Emote_Rogue_Loyalty_Nod
	CycleMove	A_Emote_Rogue_Loyalty_Wave_Left
	CycleMove	A_Emote_Rogue_Loyalty_Wave


	Type		male
	  Anim          male\a_yes_wave 1
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd

	Type		huge
	  Anim          male\a_yes_wave 1
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd

	Type		fem
	  Anim          male\a_yes_wave 1
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd
MEnd

Move A_Emote_Rogue_Loyalty_Wave_Right
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE FIXARMREG
	CycleMove	A_Emote_Rogue_Loyalty_Wave_Left
	CycleMove	A_Emote_Rogue_Loyalty_Wave
	CycleMove	A_Emote_Rogue_Loyalty_Nod
	CycleMove	A_Emote_Rogue_Loyalty_Nod


	Type		male
	  Anim          male\a_yes_wave_right_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd

	Type		huge
	  Anim          male\a_yes_wave_right_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd

	Type		fem
	  Anim          male\a_yes_wave_right_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd
MEnd


Move A_Emote_Rogue_Loyalty_Wave_Left
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE FIXARMREG
	CycleMove	A_Emote_Rogue_Loyalty_Wave_Left
	CycleMove	A_Emote_Rogue_Loyalty_Nod
	CycleMove	A_Emote_Rogue_Loyalty_Wave_Right
	CycleMove	A_Emote_Rogue_Loyalty_Nod


	Type		male
	  Anim          male\a_yes_wave_left_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd

	Type		huge
	  Anim          male\a_yes_wave_left_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd

	Type		fem
	  Anim          male\a_yes_wave_left_only 1
	Playfx		"Emotes\Loyalty\Loyalty_Rogue.fx" 0 constant
	TEnd
MEnd

#############################################################
## "Firework Rocket Flare"
#############################################################

Move A_Emote_firework_rocket
	Requires	FLY EMOTE FIRE LAUNCH HIGH
	Priority	15
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_firework_rocket_post

	Type		male
	  Anim          male\air_emote_firework_rocket 1 115
	  PlayFX	"Emotes\Fireworks\Fireworks_Rocket.fx" 1 CONSTANT
	TEnd
MEnd

Move A_Emote_firework_rocket_post
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		NoSizeScale
	NextMove        FlyReady

	Type		male
	  Anim          male\air_emote_firework_rocket 116
	TEnd
MEnd

#############################################################
## "Firework Bloom"
#############################################################

Move A_Emote_firework_bloom
	Requires	FLY EMOTE FIRE LAUNCH MID
	Priority	15
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_air.inc
	NextMove	A_Emote_firework_bloom_post

	Type		male
	  Anim          male\air_emote_firework_bloom 1 65
	  PlayFX	"Emotes\Fireworks\Fireworks_Bloom.fx" 1 CONSTANT
	TEnd
MEnd

Move A_Emote_firework_bloom_post
	Include sequencers\Members\Emotes_Idle_air.inc
	Flags		NoSizeScale
	NextMove        FlyReady

	Type		male
	  Anim          male\air_emote_firework_bloom 70
	TEnd
MEnd

#############################################################
## "Firework Sparkle"
#############################################################

Move A_Emote_firework_sparkle
	Requires	FLY EMOTE FIRE LAUNCH LOW
	Priority	15
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_air.inc
	NextMove	A_Emote_firework_sparkle_post

	Type		male
	  Anim          male\air_emote_firework_sparkle 1 65
	  PlayFX	"Emotes\Fireworks\Fireworks_Sparkle.fx" 1 CONSTANT
	TEnd
MEnd

Move A_Emote_firework_sparkle_post
	Include sequencers\Members\Emotes_Idle_air.inc
	Flags		NoSizeScale
	NextMove        FlyReady

	Type		male
	  Anim          male\air_emote_firework_sparkle 70
	TEnd
MEnd

#############################################################