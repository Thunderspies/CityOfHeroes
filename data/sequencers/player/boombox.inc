#############################################################
## Boombox.inc - Boombox\Music Variants
#############################################################

#############################################################
## Place Boombox
#############################################################

Move PlaceBoombox_Pre
	Requires	EMOTE DANCE POSE BOX
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	PlaceBoombox
	Priority	4

	Type		male
	  Anim          male\Loot 1 2
	  PlayFx        GENERIC\Boombox_sound.FX  0 Constant
	TEnd
MEnd

Move PlaceBoombox
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	PlaceBoombox_cycle
	Priority	1

	Type		male
	  Anim          male\Loot 3
	  PlayFx        GENERIC\Boombox_sound.FX  0 Constant
	TEnd
MEnd

Move PlaceBoombox_cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Priority	1

	Type		male
	  Anim          male\dance_base 1
	  PlayFx        GENERIC\Boombox_sound.FX  0 Constant
	TEnd
MEnd


#############################################################
## Boombox 1
#############################################################

Move Boombox_1
	Requires	EMOTE DANCE POSE BOX ONE
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Boombox_cycle_1
	Priority	5
	Interpolate	8

	Type		male
	  Anim          male\Loot 3 50
	  PlayFx        GENERIC\Boombox_01.fx  0 Constant
	TEnd
MEnd

Move Boombox_cycle_1
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle

	Type		male
	  Anim          male\dance_base 1
	  PlayFx        GENERIC\Boombox_01.fx  0 Constant
	TEnd
MEnd


#############################################################
## Boombox 2
#############################################################

Move Boombox_2
	Requires	EMOTE DANCE POSE BOX TWO
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Boombox_cycle_2
	Priority	5
	Interpolate	8

	Type		male
	  Anim          male\Loot 3 50
	  PlayFx        GENERIC\Boombox_02.fx  0 Constant
	TEnd
MEnd

Move Boombox_cycle_2
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle

	Type		male
	  Anim          male\dance_base 1
	  PlayFx        GENERIC\Boombox_02.fx  0 Constant
	TEnd
MEnd


#############################################################
## Boombox 3
#############################################################

Move Boombox_3
	Requires	EMOTE DANCE POSE BOX THREE
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Boombox_cycle_3
	Priority	5
	Interpolate	8

	Type		male
	  Anim          male\Loot 3 50
	  PlayFx        GENERIC\Boombox_03.fx  0 Constant
	TEnd
MEnd

Move Boombox_cycle_3
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle

	Type		male
	  Anim          male\dance_base 1
	  PlayFx        GENERIC\Boombox_03.fx  0 Constant
	TEnd
MEnd


#############################################################
## Boombox 4
#############################################################

Move Boombox_4
	Requires	EMOTE DANCE POSE BOX FOUR
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Boombox_cycle_4
	Priority	5
	Interpolate	8

	Type		male
	  Anim          male\Loot 3 50
	  PlayFx        GENERIC\Boombox_04.fx  0 Constant
	TEnd
MEnd

Move Boombox_cycle_4
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle

	Type		male
	  Anim          male\dance_base 1
	  PlayFx        GENERIC\Boombox_04.fx  0 Constant
	TEnd
MEnd


#############################################################
## Boombox 5
#############################################################

Move Boombox_5
	Requires	EMOTE DANCE POSE BOX FIVE
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Boombox_cycle_5
	Priority	5
	Interpolate	8

	Type		male
	  Anim          male\Loot 3 50
	  PlayFx        GENERIC\Boombox_05.fx  0 Constant
	TEnd
MEnd

Move Boombox_cycle_5
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle

	Type		male
	  Anim          male\dance_base 1
	  PlayFx        GENERIC\Boombox_05.fx  0 Constant
	TEnd
MEnd


#############################################################
## Boombox 6
#############################################################

Move Boombox_6
	Requires	EMOTE DANCE POSE BOX SIX
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Boombox_cycle_6
	Priority	5
	Interpolate	8

	Type		male
	  Anim          male\Loot 3 50
	  PlayFx        GENERIC\Boombox_06.fx  0 Constant
	TEnd
MEnd

Move Boombox_cycle_6
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle

	Type		male
	  Anim          male\dance_base 1
	  PlayFx        GENERIC\Boombox_06.fx  0 Constant
	TEnd
MEnd


#############################################################
## Boombox 7
#############################################################

Move Boombox_7
	Requires	EMOTE DANCE POSE BOX SEVEN
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Boombox_cycle_7
	Priority	5
	Interpolate	8

	Type		male
	  Anim          male\Loot 3 50
	  PlayFx        GENERIC\Boombox_07.fx  0 Constant
	TEnd
MEnd

Move Boombox_cycle_7
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle

	Type		male
	  Anim          male\dance_base 1
	  PlayFx        GENERIC\Boombox_07.fx  0 Constant
	TEnd
MEnd


#############################################################
## Boombox 8
#############################################################

Move Boombox_8
	Requires	EMOTE DANCE POSE BOX EIGHT
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Boombox_cycle_8
	Priority	5
	Interpolate	8

	Type		male
	  Anim          male\Loot 3 50
	  PlayFx        GENERIC\Boombox_08.fx  0 Constant
	TEnd
MEnd

Move Boombox_cycle_8
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle

	Type		male
	  Anim          male\dance_base 1
	  PlayFx        GENERIC\Boombox_08.fx  0 Constant
	TEnd
MEnd


#############################################################
## Boombox 9
#############################################################

Move Boombox_9
	Requires	EMOTE DANCE POSE BOX NINE
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Boombox_cycle_9
	Priority	5
	Interpolate	8

	Type		male
	  Anim          male\Loot 3 50
	  PlayFx        GENERIC\Boombox_09.fx  0 Constant
	TEnd
MEnd

Move Boombox_cycle_9
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle

	Type		male
	  Anim          male\dance_base 1
	  PlayFx        GENERIC\Boombox_09.fx  0 Constant
	TEnd
MEnd


#############################################################
## Boombox 10
#############################################################

Move Boombox_10
	Requires	EMOTE DANCE POSE BOX TEN
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Boombox_cycle_10
	Priority	5
	Interpolate	8

	Type		male
	  Anim          male\Loot 3 50
	  PlayFx        GENERIC\Boombox_10.fx  0 Constant
	TEnd
MEnd

Move Boombox_cycle_10
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle

	Type		male
	  Anim          male\dance_base 1
	  PlayFx        GENERIC\Boombox_10.fx  0 Constant
	TEnd
MEnd


#############################################################
## Boombox 11
#############################################################

Move Boombox_11
	Requires	EMOTE DANCE POSE BOX ONE TEN
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Boombox_cycle_11
	Priority	6
	Interpolate	8

	Type		male
	  Anim          male\Loot 3 50
	  PlayFx        GENERIC\Boombox_11.fx  0 Constant
	TEnd
MEnd

Move Boombox_cycle_11
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle

	Type		male
	  Anim          male\dance_base 1
	  PlayFx        GENERIC\Boombox_11.fx  0 Constant
	TEnd
MEnd


#############################################################
## Boombox 12
#############################################################

Move Boombox_12
	Requires	EMOTE DANCE POSE BOX TWO TEN
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Boombox_cycle_12
	Priority	6
	Interpolate	8

	Type		male
	  Anim          male\Loot 3 50
	  PlayFx        GENERIC\Boombox_12.fx  0 Constant
	TEnd
MEnd

Move Boombox_cycle_12
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle

	Type		male
	  Anim          male\dance_base 1
	  PlayFx        GENERIC\Boombox_12.fx  0 Constant
	TEnd
MEnd

#############################################################
## Boombox 13
#############################################################

Move Boombox_13
	Requires	EMOTE DANCE POSE BOX THREE TEN
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Boombox_cycle_13
	Priority	6
	Interpolate	8

	Type		male
	  Anim          male\Loot 3 50
	  PlayFx        GENERIC\Boombox_13.fx  0 Constant
	TEnd
MEnd

Move Boombox_cycle_13
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle

	Type		male
	  Anim          male\dance_base 1
	  PlayFx        GENERIC\Boombox_13.fx  0 Constant
	TEnd
MEnd


#############################################################
## Boombox 14
#############################################################

Move Boombox_14
	Requires	EMOTE DANCE POSE BOX FOUR TEN
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Boombox_cycle_14
	Priority	6
	Interpolate	8

	Type		male
	  Anim          male\Loot 3 50
	  PlayFx        GENERIC\Boombox_14.fx  0 Constant
	TEnd
MEnd

Move Boombox_cycle_14
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle

	Type		male
	  Anim          male\dance_base 1
	  PlayFx        GENERIC\Boombox_14.fx  0 Constant
	TEnd
MEnd


#############################################################
## Boombox 15
#############################################################

Move Boombox_15
	Requires	EMOTE DANCE POSE BOX FIVE TEN
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Boombox_cycle_15
	Priority	6
	Interpolate	8

	Type		male
	  Anim          male\Loot 3 50
	  PlayFx        GENERIC\Boombox_15.fx  0 Constant
	TEnd
MEnd

Move Boombox_cycle_15
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle

	Type		male
	  Anim          male\dance_base 1
	  PlayFx        GENERIC\Boombox_15.fx  0 Constant
	TEnd
MEnd


#############################################################
## Boombox 16
#############################################################

Move Boombox_16
	Requires	EMOTE DANCE POSE BOX SIX TEN
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Boombox_cycle_16
	Priority	6
	Interpolate	8

	Type		male
	  Anim          male\Loot 3 50
	  PlayFx        GENERIC\Boombox_16.fx  0 Constant
	TEnd
MEnd

Move Boombox_cycle_16
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle

	Type		male
	  Anim          male\dance_base 1
	  PlayFx        GENERIC\Boombox_16.fx  0 Constant
	TEnd
MEnd


#############################################################
## Boombox 17
#############################################################

Move Boombox_17
	Requires	EMOTE DANCE POSE BOX SEVEN TEN
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Boombox_cycle_17
	Priority	6
	Interpolate	8

	Type		male
	  Anim          male\Loot 3 50
	  PlayFx        GENERIC\Boombox_17.fx  0 Constant
	TEnd
MEnd

Move Boombox_cycle_17
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle

	Type		male
	  Anim          male\dance_base 1
	  PlayFx        GENERIC\Boombox_17.fx  0 Constant
	TEnd
MEnd


#############################################################
## Boombox 18
#############################################################

Move Boombox_18
	Requires	EMOTE DANCE POSE BOX EIGHT TEN
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Boombox_cycle_18
	Priority	6
	Interpolate	8

	Type		male
	  Anim          male\Loot 3 50
	  PlayFx        GENERIC\Boombox_18.fx  0 Constant
	TEnd
MEnd

Move Boombox_cycle_18
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle

	Type		male
	  Anim          male\dance_base 1
	  PlayFx        GENERIC\Boombox_18.fx  0 Constant
	TEnd
MEnd


#############################################################
## Boombox 19
#############################################################

Move Boombox_19
	Requires	EMOTE DANCE POSE BOX NINE TEN
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Boombox_cycle_19
	Priority	6
	Interpolate	8

	Type		male
	  Anim          male\Loot 3 50
	  PlayFx        GENERIC\Boombox_19.fx  0 Constant
	TEnd
MEnd

Move Boombox_cycle_19
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle

	Type		male
	  Anim          male\dance_base 1
	  PlayFx        GENERIC\Boombox_19.fx  0 Constant
	TEnd
MEnd


#############################################################
## Boombox 20
#############################################################

Move Boombox_20
	Requires	EMOTE DANCE POSE BOX TWENTY
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Boombox_cycle_20
	Priority	5
	Interpolate	8

	Type		male
	  Anim          male\Loot 3 50
	  PlayFx        GENERIC\Boombox_20.fx  0 Constant
	TEnd
MEnd

Move Boombox_cycle_20
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle

	Type		male
	  Anim          male\dance_base 1
	  PlayFx        GENERIC\Boombox_20.fx  0 Constant
	TEnd
MEnd


#############################################################
## Boombox 21
#############################################################

Move Boombox_21
	Requires	EMOTE DANCE POSE BOX ONE TWENTY
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Boombox_cycle_21
	Priority	6
	Interpolate	8

	Type		male
	  Anim          male\Loot 3 50
	  PlayFx        GENERIC\Boombox_21.fx  0 Constant
	TEnd
MEnd

Move Boombox_cycle_21
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle

	Type		male
	  Anim          male\dance_base 1
	  PlayFx        GENERIC\Boombox_21.fx  0 Constant
	TEnd
MEnd


#############################################################
## Boombox 22
#############################################################

Move Boombox_22
	Requires	EMOTE DANCE POSE BOX TWO TWENTY
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Boombox_cycle_22
	Priority	6
	Interpolate	8

	Type		male
	  Anim          male\Loot 3 50
	  PlayFx        GENERIC\Boombox_22.fx  0 Constant
	TEnd
MEnd

Move Boombox_cycle_22
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle

	Type		male
	  Anim          male\dance_base 1
	  PlayFx        GENERIC\Boombox_22.fx  0 Constant
	TEnd
MEnd


#############################################################
## Boombox 23
#############################################################

Move Boombox_23
	Requires	EMOTE DANCE POSE BOX THREE TWENTY
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Boombox_cycle_23
	Priority	6
	Interpolate	8

	Type		male
	  Anim          male\Loot 3 50
	  PlayFx        GENERIC\Boombox_23.fx  0 Constant
	TEnd
MEnd

Move Boombox_cycle_23
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle

	Type		male
	  Anim          male\dance_base 1
	  PlayFx        GENERIC\Boombox_23.fx  0 Constant
	TEnd
MEnd


#############################################################
## Boombox 24
#############################################################

Move Boombox_24
	Requires	EMOTE DANCE POSE BOX FOUR TWENTY
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Boombox_cycle_24
	Priority	6
	Interpolate	8

	Type		male
	  Anim          male\Loot 3 50
	  PlayFx        GENERIC\Boombox_24.fx  0 Constant
	TEnd
MEnd

Move Boombox_cycle_24
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle

	Type		male
	  Anim          male\dance_base 1
	  PlayFx        GENERIC\Boombox_24.fx  0 Constant
	TEnd
MEnd


#############################################################
## Boombox 25
#############################################################

Move Boombox_25
	Requires	EMOTE DANCE POSE BOX FIVE TWENTY
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Boombox_cycle_25
	Priority	6
	Interpolate	8

	Type		male
	  Anim          male\Loot 3 50
	  PlayFx        GENERIC\Boombox_25.fx  0 Constant
	TEnd
MEnd

Move Boombox_cycle_25
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle

	Type		male
	  Anim          male\dance_base 1
	  PlayFx        GENERIC\Boombox_25.fx  0 Constant
	TEnd
MEnd