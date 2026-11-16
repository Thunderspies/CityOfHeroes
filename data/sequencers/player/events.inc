#########################################################
## EVENTS.INC - Sequencers related to Event powers
#########################################################

#####################
####### Draw SillyBat  ######
######################

Move Draw_SillyBat
	Requires	COMBAT WEAPON DUAL REV SPAWNHIP KATANA HAPPY
	Interrupts	<Ready> <PostMoveCycle> <Combat> <WepCombat> <TwoHandCombat>  Predual PreWep Draw_Katana
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ>
	NextMove        Draw_SillyBat_post
	Priority        7
	Interpolate	8
	Sets            CANTMOVE
	Flags		FixArmReg NoSizeScale

	Type		male
	  Anim          male\ninjasword_draw 1 20
	TEnd
MEnd

Move Draw_SillyBat_post
	Requires	COMBAT WEAPON DUAL REV ###HAPPY
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <WepAttack> <AttackIRQ> <PostMoveCycle>
	NextMove        Katana_half  ###SillyBat_half
	Priority        4
	Flags		FixArmReg NoSizeScale

	Type		male
	  Anim          male\ninjasword_draw 20
	TEnd
MEnd


######################
####### SillyBat Slash  ######
######################

Move 1st_SillyBat_slash
	Requires	COMBAT WEAPON DUAL REV SPAWNHIP KATANA SLASH HAPPY
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        1st_SillyBat_slash2
	Priority        8
	Sets            CANTMOVE
	Flags		FixArmReg NoSizeScale

	Type		male
	  Anim          male\ninjasword_draw 1 20
	TEnd
MEnd

Move 1st_SillyBat_slash2
	Requires	COMBAT WEAPON DUAL REV HAPPY
	Member		<DeathIRQ>
	NextMove        1st_SillyBat_slash_post
	Priority        5
	Sets            CANTMOVE
	Flags		FixArmReg NoSizeScale

	Type		male
	  Anim          male\katana_2slash 2 30
	  PlayFx        FEET\Squeaky.fx  7
	  PlayFx        FEET\Squeaky.fx  15
	TEnd

	Type		fem
	  Anim          fem\katana_2slash 2 30
	  PlayFx        FEET\Squeaky.fx  7
	  PlayFx        FEET\Squeaky.fx  15
	TEnd

	Type		huge
	  Anim          huge\katana_2slash 2 30
	  PlayFx        FEET\Squeaky.fx  7
	  PlayFx        FEET\Squeaky.fx  15
	TEnd
MEnd

Move 1st_SillyBat_slash_post
	Requires	COMBAT WEAPON DUAL REV ##HAPPY
	Priority        4
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <WepAttack> <AttackIRQ> <PostMoveCycle>
	NextMove        Katana_half
	Flags		FixArmReg NoSizeScale

	Type		male
	  Anim          male\katana_2slash 30
	TEnd

	Type		fem
	  Anim          fem\katana_2slash 30
	TEnd

	Type		huge
	  Anim          huge\katana_2slash 30
	TEnd
MEnd


Move 2nd_SillyBat_slash
	Requires	COMBAT WEAPON DUAL REV KATANA SLASH HAPPY
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        1st_SillyBat_slash_post
	Priority        7
	Sets            CANTMOVE
	Flags		FixArmReg PickRandomly NoSizeScale

	Type		male
	  Anim          male\katana_2slash 2 30
	  PlayFx        FEET\Squeaky.fx  7
	  PlayFx        FEET\Squeaky.fx  15
	TEnd

	Type		fem
	  Anim          fem\katana_2slash 2 30
	  PlayFx        FEET\Squeaky.fx  7
	  PlayFx        FEET\Squeaky.fx  15
	TEnd

	Type		huge
	  Anim          huge\katana_2slash 2 30
	  PlayFx        FEET\Squeaky.fx  7
	  PlayFx        FEET\Squeaky.fx  15
	TEnd
MEnd


Move 3rd_SillyBat_parry
	Requires	COMBAT WEAPON DUAL REV KATANA SLASH HAPPY
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        3rd_SillyBat_parry_post
	Priority        7
	Interpolate	8
	Sets            CANTMOVE
	Flags		FixArmReg PickRandomly NoSizeScale

	Type		male
	  Anim          male\katana_parry 13 35
	  PlayFx        FEET\Squeaky.fx  15
	TEnd

	Type		fem
	  Anim          fem\katana_parry 13 35
	  PlayFx        FEET\Squeaky.fx  15
	TEnd

	Type		huge
	  Anim          huge\katana_parry 13 35
	  PlayFx        FEET\Squeaky.fx  15
	TEnd
MEnd

Move 3rd_SillyBat_parry_post
	Requires	COMBAT WEAPON DUAL REV ##HAPPY
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <WepAttack> <AttackIRQ> <PostMoveCycle>
	NextMove        Katana_half
	Priority        4
	Interpolate     0
	Flags		FixArmReg NoSizeScale

	Type		male
	  Anim          male\katana_parry 36 61
	TEnd

	Type		fem
	  Anim          fem\katana_parry 36 60
	TEnd

	Type		huge
	  Anim          huge\katana_parry 36 60
	TEnd
MEnd


#####################
####### Air Draw SillyBat  ######
######################

Move A_Draw_SillyBat
	Requires	Fly COMBAT WEAPON DUAL REV SPAWNHIP KATANA HAPPY
	Interrupts	<FlyReady> <FlyCombat> <FlyWepCombat> <FlyTwoHandCombat> <PostFLYCycle> <FlyWepAttack> A_Draw_Katana
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ>
	NextMove        A_Draw_SillyBat_post
	Priority        8
	Interpolate	8
	Sets            CANTMOVE
	Flags		FixArmReg NoSizeScale

	Type		male
	  Anim          male\air_ninjasword_draw 1 20
	TEnd
MEnd

Move A_Draw_SillyBat_post
	Requires	Fly COMBAT WEAPON DUAL REV ###HAPPY
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <FlyWepAttack>
	NextMove        A_Katana_half
	Priority        5
	Flags		FixArmReg NoSizeScale

	Type		male
	  Anim          male\air_ninjasword_draw 20
	TEnd
MEnd


######################
####### SillyBat Slash  ######
######################

Move A_1st_SillyBat_slash
	Requires	Fly COMBAT WEAPON DUAL REV SPAWNHIP KATANA SLASH HAPPY
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        A_1st_SillyBat_slash2
	Priority        9
	Sets            CANTMOVE
	Flags		FixArmReg NoSizeScale

	Type		male
	  Anim          male\air_ninjasword_draw 1 20
	TEnd
MEnd

Move A_1st_SillyBat_slash2
	Requires	FLY COMBAT WEAPON DUAL REV HAPPY
	Priority        6
	Member		<DeathIRQ>
	NextMove        A_1st_SillyBat_slash_post
	Sets            CANTMOVE
	Flags		FixArmReg NoSizeScale

	Type		male
	  Anim          male\air_katana_slash 2 30
	  PlayFx        FEET\Squeaky.fx  7
	  PlayFx        FEET\Squeaky.fx  15
	TEnd

	Type		fem
	  Anim          fem\air_katana_slash 2 30
	  PlayFx        FEET\Squeaky.fx  7
	  PlayFx        FEET\Squeaky.fx  15
	TEnd

	Type		huge
	  Anim          huge\air_katana_slash 2 30
	  PlayFx        FEET\Squeaky.fx  7
	  PlayFx        FEET\Squeaky.fx  15
	TEnd
MEnd

Move A_1st_SillyBat_slash_post
	Requires	FLY COMBAT WEAPON DUAL REV ##HAPPY
	Priority        5
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <FlyWepAttack>
	NextMove        A_Katana_half
	Interpolate     0
	Flags		FixArmReg NoSizeScale

	Type		male
	  Anim          male\air_katana_slash 30
	TEnd

	Type		fem
	  Anim          fem\air_katana_slash 30
	TEnd

	Type		huge
	  Anim          huge\air_katana_slash 30
	TEnd
MEnd


Move A_2nd_SillyBat_slash
	Requires	FLY COMBAT WEAPON DUAL REV KATANA SLASH HAPPY
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        A_1st_SillyBat_slash_post
	Priority        8
	Sets            CANTMOVE
	Flags		FixArmReg NoSizeScale PickRandomly

	Type		male
	  Anim          male\air_katana_slash 2 30
	  PlayFx        FEET\Squeaky.fx  7
	  PlayFx        FEET\Squeaky.fx  15
	TEnd

	Type		fem
	  Anim          fem\air_katana_slash 2 30
	  PlayFx        FEET\Squeaky.fx  7
	  PlayFx        FEET\Squeaky.fx  15
	TEnd

	Type		huge
	  Anim          huge\air_katana_slash 2 30
	  PlayFx        FEET\Squeaky.fx  7
	  PlayFx        FEET\Squeaky.fx  15
	TEnd
MEnd


Move A_3rd_SillyBat_parry
	Requires	Fly COMBAT WEAPON DUAL REV KATANA SLASH HAPPY
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        A_3rd_SillyBat_parry_post
	Priority        8
	Interpolate	8
	Sets            CANTMOVE
	Flags		FixArmReg NoSizeScale PickRandomly

	Type		male
	  Anim          male\air_katana_parry 13 35
	  PlayFx        FEET\Squeaky.fx  15
	TEnd

	Type		fem
	  Anim          fem\air_katana_parry 13 35
	  PlayFx        FEET\Squeaky.fx  15
	TEnd

	Type		huge
	  Anim          huge\air_katana_parry 13 35
	  PlayFx        FEET\Squeaky.fx  15
	TEnd
MEnd

Move A_3rd_SillyBat_parry_post
	Requires	Fly COMBAT WEAPON DUAL REV ##HAPPY
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <FlyWepAttack>
	NextMove        A_Katana_half
	Priority        5
	Flags		FixArmReg NoSizeScale

	Type		male
	  Anim          male\air_katana_parry 35 61
	TEnd

	Type		fem
	  Anim          fem\air_katana_parry 35 60
	TEnd

	Type		huge
	  Anim          huge\air_katana_parry 35 60
	TEnd
MEnd

########################
## Open Holiday Gift  ##
########################

Move Open_Holiday_Gift1
	Requires	OPENHOLIDAYGIFT
	Priority        1
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Ready #NonCombat_Power_post
	Flags		CANTMOVE PICKRANDOMLY NoSizeScale

	Type		male
	  Anim		male\open_holiday_gift 1
	  PlayFx	Events\Winter\Open_Holiday_Gift1.fx 1 Constant
	TEnd

	Type		fem
	  Anim          fem\open_holiday_gift 1
	  PlayFx        Events\Winter\Open_Holiday_Gift1.fx 1 Constant
	TEnd

	Type		huge
	  Anim		huge\open_holiday_gift 1
	  PlayFx        Events\Winter\Open_Holiday_Gift1.fx 1 Constant
	TEnd
MEnd


#############################################################
## Winter Event 08 - Throw SnowFlakes
#############################################################

Move Emote_ThrowSnowflakes
	Requires	EMOTE UNDERHANDTOSS FOUR
	Priority        3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesLefthand> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Idle1_Cycle

	Type		male
	  Anim          male\emote_UnderhandToss 2
	  PlayFx        Emotes\ThrowSnowflakes.fx 30 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_UnderhandToss 2
	  PlayFx        Emotes\ThrowSnowflakes.fx 28 CONSTANT
	TEnd
MEnd

#############################################################

Move A_Emote_ThrowSnowflakes
	Requires	FLY EMOTE UNDERHANDTOSS FOUR
	Priority        4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	FlyReady

	Type		male
	  Anim          male\air_emote_UnderhandToss 2
	  PlayFx        Emotes\ThrowSnowflakes.fx 30 Constant
	TEnd
MEnd

########################
## Instant Snow Storm  ##
########################

Move Instant_SnowStorm
	Requires	OPENHOLIDAYGIFT SKYSUMMON
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Ready #NonCombat_Power_post
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim		male\sky_summon 1
	TEnd

#	Type		fem
#	  Anim          fem\open_holiday_gift 1
#	  PlayFx        Events\Winter\Open_Holiday_Gift1.fx 1 Constant
#	TEnd
#
#	Type		huge
#	  Anim		huge\open_holiday_gift 1
#	  PlayFx        Events\Winter\Open_Holiday_Gift1.fx 1 Constant
#	TEnd
MEnd

#############################################################
## DON'T KNOW WHY THERE'S 3 VERSIONS OF THIS SEQUENCE
#############################################################

#Move Open_Holiday_Gift2
#	Requires	OPENHOLIDAYGIFT
#	Priority        1
#	Interrupts	<AttackIRQ>
#	Member		<DeathIRQ>
#	NextMove        Ready #NonCombat_Power_post
#	Flags		CANTMOVE PICKRANDOMLY NoSizeScale
#
#	Type		male
#	  Anim          male\open_holiday_gift 1
#	  PlayFx        Events\Winter\Open_Holiday_Gift1.fx 1 Constant
#	TEnd
#
#	Type		fem
#	  Anim          fem\open_holiday_gift 1
#	  PlayFx        Events\Winter\Open_Holiday_Gift1.fx 1 Constant
#	TEnd
#
#	Type		huge
#	  Anim          huge\open_holiday_gift 1
#	  PlayFx        Events\Winter\Open_Holiday_Gift1.fx 1 Constant
#	TEnd
#MEnd
#
#Move Open_Holiday_Gift3
#	Requires	OPENHOLIDAYGIFT
#	Priority        1
#	Interrupts	<AttackIRQ>
#	Member		<DeathIRQ>
#	NextMove        Ready #NonCombat_Power_post
#	Flags		CANTMOVE PICKRANDOMLY NoSizeScale
#
#	Type		male
#	  Anim          male\open_holiday_gift 1
#	  PlayFx        Events\Winter\Open_Holiday_Gift1.fx 1 Constant
#	TEnd
#
#	Type		fem
#	  Anim          fem\open_holiday_gift 1
#	  PlayFx        Events\Winter\Open_Holiday_Gift1.fx 1 Constant
#	TEnd
#
#	Type		huge
#	  Anim          huge\open_holiday_gift 1
#	  PlayFx        Events\Winter\Open_Holiday_Gift1.fx 1 Constant
#	TEnd
#MEnd