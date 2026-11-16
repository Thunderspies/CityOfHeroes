#############################################################
## SuperStrength - Custom Animations 1
#############################################################

## SUPER STRENGTH - TIER 1 (Jab) ###########################################################

Move SS_Tier1_Alt1
	Requires	COMBAT GUTPUNCH ALTERNATE ONE
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        SS_Tier1_Alt1_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\SS_jab 1 25
	TEnd
MEnd

Move SS_Tier1_Alt1_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Lazy_CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\SS_jab 25
	TEnd
MEnd

#############################################################

Move A_SS_Tier1_Alt1
	Requires	FLY COMBAT GUTPUNCH ALTERNATE ONE
	Priority        15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_SS_Tier1_Alt1_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\SS_jab_air 1 25
	TEnd
MEnd

Move A_SS_Tier1_Alt1_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        Lazy_C_flycycle_Blend
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\SS_jab_air 25
	TEnd
MEnd

## SUPER STRENGTH - TIER 2 (Punch) ###########################################################

Move SS_Tier2_Alt1_Left
	Requires	COMBAT PUNCH SMASH ALTERNATE ONE
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        SS_Tier2_Alt1_Left_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\SS_punch 1 32
	TEnd
MEnd

Move SS_Tier2_Alt1_Left_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Lazy_CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\SS_punch 32 46
	TEnd
MEnd

#############################################################

Move SS_Tier2_Alt1_Right
	Requires	COMBAT PUNCH SMASH ALTERNATE ONE
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        SS_Tier2_Alt1_Right_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\SS_punch 46 82
	TEnd
MEnd

Move SS_Tier2_Alt1_Right_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Lazy_CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\SS_punch 82
	TEnd
MEnd

#############################################################

Move A_SS_Tier2_Alt1_Left
	Requires	FLY COMBAT PUNCH SMASH ALTERNATE ONE
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_SS_Tier2_Alt1_Left_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\SS_punch_air 1 32
	TEnd
MEnd

Move A_SS_Tier2_Alt1_Left_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        Lazy_C_flycycle_Blend
	Flags		NoSizeScale ReqInputs
	Scale		0.5

	Type		male
	  Anim          male\SS_punch_air 32 46
	TEnd
MEnd

#############################################################

Move A_SS_Tier2_Alt1_Right
	Requires	FLY COMBAT PUNCH SMASH ALTERNATE ONE
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_SS_Tier2_Alt1_Right_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\SS_punch_air 46 82
	TEnd
MEnd

Move A_SS_Tier2_Alt1_Right_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        Lazy_C_flycycle_Blend
	Flags		NoSizeScale ReqInputs
	Scale		0.5

	Type		male
	  Anim          male\SS_punch_air 82
	TEnd
MEnd

## SUPER STRENGTH - TIER 3 (Haymaker) ###########################################################

Move SS_Tier3_Alt1_Right
	Requires	COMBAT SMASH ALTERNATE ONE
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        SS_Tier3_Alt1_Right_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim		male\haymaker 1 48
	TEnd
MEnd

Move SS_Tier3_Alt1_Right_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Lazy_CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim		male\haymaker 48 58
	TEnd
MEnd

#############################################################

Move A_SS_Tier3_Alt1_Right
	Requires	FLY COMBAT SMASH ALTERNATE ONE
	Priority        15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_SS_Tier3_Alt1_Right_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\haymaker_air 1 48
	TEnd
MEnd

Move A_SS_Tier3_Alt1_Right_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        Lazy_C_flycycle_Blend
	Flags		NoSizeScale ReqInputs
	Scale		0.5

	Type		male
	  Anim          male\haymaker_air 48 58
	TEnd
MEnd

## SUPER STRENGTH - TIER 6 (Knockout Blow) ###########################################################

Move SS_Tier6_Alt1
	Requires	COMBAT WINDUPPERCUT ALTERNATE ONE
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        SS_Tier6_Alt1_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\SS_KnockoutBlow 1 66
	TEnd

	Type		fem
	  Anim          fem\SS_KnockoutBlow 1 66
	TEnd

	Type		huge
	  Anim          huge\SS_KnockoutBlow 1 66
	TEnd
MEnd

Move SS_Tier6_Alt1_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Lazy_CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\SS_KnockoutBlow 66 76
	TEnd

	Type		fem
	  Anim          fem\SS_KnockoutBlow 66 76
	TEnd

	Type		huge
	  Anim          huge\SS_KnockoutBlow 66 76
	TEnd

MEnd

#############################################################

Move A_SS_Tier6_Alt1
	Requires	FLY COMBAT WINDUPPERCUT ALTERNATE ONE
	Priority        15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        A_SS_Tier6_Alt1_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\SS_knockoutBlow_air 1 66
	TEnd

	Type		fem
	  Anim          fem\SS_knockoutBlow_air 1 66
	TEnd

	Type		huge
	  Anim          huge\SS_knockoutBlow_air 1 66
	TEnd
MEnd

Move A_SS_Tier6_Alt1_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        Lazy_C_flycycle_Blend
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\SS_knockoutBlow_air 66
	TEnd

	Type		fem
	  Anim          fem\SS_knockoutBlow_air 66
	TEnd

	Type		huge
	  Anim          huge\SS_knockoutblow_air 66
	TEnd
MEnd

## SUPER STRENGTH - TIER 9 (Foot Stomp) ###########################################################

Move SS_Tier9_Alt1
	Requires	COMBAT STOMP ALTERNATE ONE
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        SS_Tier9_Alt1_2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\SS_ground_punch 1 70
	TEnd

	Type		huge
	  Anim          male\SS_ground_punch 1 70
	TEnd

	Type		fem
	  Anim          male\SS_ground_punch 1 70
	TEnd

	Type		RockSuit
	  Anim          male\SS_ground_punch 1 70
	TEnd
MEnd

Move SS_Tier9_Alt1_2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\SS_ground_punch 70
	TEnd

	Type		huge
	  Anim          male\SS_ground_punch 70
	TEnd

	Type		fem
	  Anim          male\SS_ground_punch 70
	TEnd

	Type		RockSuit
	  Anim          male\SS_ground_punch 70
	TEnd
MEnd