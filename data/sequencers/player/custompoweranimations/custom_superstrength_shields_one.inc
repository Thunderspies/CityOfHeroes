#############################################################
## SuperStrength - Custom Shield Animations 1
#############################################################

## SUPER STRENGTH - TIER 1 (Jab) ###########################################################

Move SS_Tier1_Shield_Alt1
	Requires	COMBAT GUTPUNCH ALTERNATE ONE SHIELD
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        SS_Tier1_Shield_Alt1_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\SS_jab_shield 1 25
	TEnd
MEnd

Move SS_Tier1_Shield_Alt1_Post
	Requires	COMBAT SHIELD
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <CombatAttack> <AttackIRQ> <MoveIRQ>
	NextMove        Shield_CombatReady_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\SS_jab_shield 25
	TEnd
MEnd

#############################################################

Move A_SS_Tier1_Shield_Alt1
	Requires	FLY COMBAT GUTPUNCH ALTERNATE ONE SHIELD
	Priority        27
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_SS_Tier1_Shield_Alt1_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\SS_jab_shield_air 1 25
	TEnd
MEnd

Move A_SS_Tier1_Shield_Alt1_Post
	Requires	FLY COMBAT SHIELD
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <FlyCombatAttack> <BlockIRQ> <AttackIRQ> <MoveIRQ>
	NextMove        Shield_CombatFly_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\SS_jab_shield_air 25
	TEnd
MEnd

## SUPER STRENGTH - TIER 2 (Punch) ###########################################################

Move SS_Tier2_Shield_Alt1_Left
	Requires	COMBAT PUNCH SMASH ALTERNATE ONE SHIELD
	Priority        17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        SS_Tier2_Shield_Alt1_Left_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\SS_punch_shield 1 32
	TEnd
MEnd

Move SS_Tier2_Shield_Alt1_Left_post
	Requires	COMBAT SHIELD
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <BlockIRQ> <MoveIRQ>
	NextMove        Shield_CombatReady_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\SS_punch_shield 32 46
	TEnd
MEnd

Move SS_Tier2_Shield_Alt1_Right
	Requires	COMBAT PUNCH SMASH ALTERNATE ONE SHIELD
	Priority        17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        SS_Tier2_Shield_Alt1_Right_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\SS_punch_shield 46 82
	TEnd
MEnd

Move SS_Tier2_Shield_Alt1_Right_post
	Requires	COMBAT SHIELD
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <BlockIRQ> <MoveIRQ>
	NextMove        Shield_CombatReady_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\SS_punch_shield 83
	TEnd
MEnd

#############################################################

Move A_SS_Tier2_Shield_Alt1_Left
	Requires	FLY COMBAT PUNCH SMASH ALTERNATE ONE SHIELD
	Priority        28
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_SS_Tier2_Shield_Alt1_Left_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\SS_punch_shield_air 1 32
	TEnd
MEnd

Move A_SS_Tier2_Shield_Alt1_Left_post
	Requires	FLY COMBAT SHIELD
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <FlyCombatAttack> <AttackIRQ> <BlockIRQ> <MoveIRQ>
	NextMove        Shield_CombatFly_Blend
	Interpolate     0
	Flags		NoSizeScale
	Scale		0.5

	Type		male
	  Anim          male\SS_punch_shield_air 32 46
	TEnd
MEnd

Move A_SS_Tier2_Shield_Alt1_Right
	Requires	FLY COMBAT PUNCH SMASH ALTERNATE ONE SHIELD
	Priority        28
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_SS_Tier2_Shield_Alt1_Right_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\SS_punch_shield_air 46 82
	TEnd
MEnd

Move A_SS_Tier2_Shield_Alt1_Right_post
	Requires	FLY COMBAT SHIELD
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <FlyCombatAttack> <AttackIRQ> <BlockIRQ> <MoveIRQ>
	NextMove        Shield_CombatFly_Blend
	Flags		NoSizeScale
	Scale		0.5

	Type		male
	  Anim          male\SS_punch_shield_air 83
	TEnd
MEnd

## SUPER STRENGTH - TIER 3 (Haymaker) ###########################################################

Move SS_Tier3_Shield_Alt1_Right
	Requires	COMBAT SMASH ALTERNATE ONE SHIELD
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        SS_Tier3_Shield_Alt1_Right_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim		male\haymaker_shield 1 48
	TEnd
MEnd

Move SS_Tier3_Shield_Alt1_Right_Post
	Requires	COMBAT SHIELD
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <CombatAttack> <AttackIRQ> <MoveIRQ>
	NextMove        Shield_CombatReady_Blend
	Flags		NoSizeScale

	Type		male
	  Anim		male\haymaker_shield 48 58
	TEnd
MEnd

#############################################################

Move A_SS_Tier3_Shield_Alt1_Right
	Requires	FLY COMBAT SMASH ALTERNATE ONE SHIELD
	Priority        27
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_SS_Tier3_Shield_Alt1_Right_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\haymaker_shield_air 1 48
	TEnd
MEnd

Move A_SS_Tier3_Shield_Alt1_Right_Post
	Requires	FLY COMBAT SHIELD
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <FlyCombat> <AttackIRQ> <MoveIRQ>
	NextMove        Shield_CombatFly_Blend
	Flags		NoSizeScale
	Scale		0.5

	Type		male
	  Anim          male\haymaker_shield_air 48 58
	TEnd
MEnd

### SUPER STRENGTH - TIER 4 (Taunt) ###########################################################
#
#Move SS_Tier4_Alt1_Pre
#	Requires	COMBAT TAUNT VARIATIONB ALTERNATE ONE
#	Priority        5
#	Interrupts	<AttackIRQ>
#	Member		<DeathIRQ> <AttackIRQ>
#	NextMove        SS_Tier4_Alt1
#	Sets            CANTMOVE
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim          male\combat_comeon 9 54
#	  PlayFX	feet\MaleTaunt12.fx
#	TEnd
#
#	Type		huge
#	  Anim          huge\combat_comeon 9 54
#	  PlayFX	feet\mALETaunt6.fx
#	TEnd
#
#	Type		fem
#	  Anim          fem\combat_comeon 9 54
#	  PlayFX	feet\fEMTaunt9.fx
#	TEnd
#MEnd
#
#Move SS_Tier4_Alt1
#	Requires	COMBAT
#	Priority        1
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ>
#	NextMove        SS_Tier4_Alt1_Cycle
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim          male\combat_comeon 55 118
#	  PlayFX	feet\handpunch1.fx
#	TEnd
#
#	Type		huge
#	  Anim          huge\combat_comeon 55 118
#	  PlayFX	feet\handpunch1.fx
#	TEnd
#
#	Type		fem
#	  Anim          fem\combat_comeon 55 118
#	  PlayFX	feet\handpunch1.fx
#	TEnd
#MEnd
#
#Move SS_Tier4_Alt1_Cycle
#	Requires	COMBAT
#	Priority        1
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ>
#	Flags		Cycle
#	NextMove        postReady_back
#
#	Type		male
#	  Anim          male\combat_comeon 119 156
#	  PlayFX	feet\handpunch1.fx
#	TEnd
#
#	Type		huge
#	  Anim          huge\combat_comeon 119 156
#	  PlayFX	feet\handpunch1.fx
#	TEnd
#
#	Type		fem
#	  Anim          fem\combat_comeon 119 156
#	  PlayFX	feet\handpunch1.fx
#	TEnd
#MEnd
#
#
##############################################################
#
#Move A_SS_Tier4_Alt1_Pre
#	Requires	FLY COMBAT TAUNT VARIATIONB ALTERNATE ONE
#	Priority        6
#	Interrupts	<FLYAttackAll>
#	Member		<DeathIRQ> <FLYAttackAll>
#	NextMove        A_SS_Tier4_Alt1
#	Sets            CANTMOVE
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim          male\air_combat_comeon 9 54
#	  PlayFX	feet\MaleTaunt12.fx
#	TEnd
#
#	Type		huge
#	  Anim          huge\air_combat_comeon 9 54
#	  PlayFX	feet\mALETaunt6.fx
#	TEnd
#
#	Type		fem
#	  Anim          fem\air_combat_comeon 9 54
#	  PlayFX	feet\fEMTaunt9.fx
#	TEnd
#MEnd
#
#Move A_SS_Tier4_Alt1
#	Requires	FLY COMBAT
#	Priority        2
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <FLYAttackAll> <MoveIRQ>
#	NextMove        A_SS_Tier4_Alt1_Cycle
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim          male\air_combat_comeon 55 118
#	  PlayFX	feet\handpunch1.fx
#	TEnd
#
#	Type		huge
#	  Anim          huge\air_combat_comeon 55 118
#	  PlayFX	feet\handpunch1.fx
#	TEnd
#
#	Type		fem
#	  Anim          fem\air_combat_comeon 55 118
#	  PlayFX	feet\handpunch1.fx
#	TEnd
#MEnd
#
#Move A_SS_Tier4_Alt1_Cycle
#	Requires	FLY COMBAT
#	Priority        2
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <FLYAttackAll> <MoveIRQ>
#	Flags		Cycle
#	NextMove        C_flycycle_half
#
#	Type		male
#	  Anim          male\air_combat_comeon 119 156
#	  PlayFX	feet\handpunch1.fx
#	TEnd
#
#	Type		huge
#	  Anim          huge\air_combat_comeon 119 156
#	  PlayFX	feet\handpunch1.fx
#	TEnd
#
#	Type		fem
#	  Anim          fem\air_combat_comeon 119 156
#	  PlayFX	feet\handpunch1.fx
#	TEnd
#MEnd
#
#
### SUPER STRENGTH - TIER 5 (Hand Clap) ###########################################################
#
#Move SS_Tier5_Alt1
#	Requires	COMBAT CAGE ALTERNATE ONE
#	Priority        4
#	Interrupts	<AttackIRQ>
#	Member		<DeathIRQ> <AttackIRQ>
#	NextMove        SS_Tier5_Alt1_Post
#	Sets            CANTMOVE
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim          male\hand_clap 1 35
#	TEnd
#MEnd
#
#Move SS_Tier5_Alt1_Post
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <BlockIRQ> <MoveIRQ>
#	NextMove        Combat_half
#	Scale           0.50
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim          male\hand_clap 36
#	TEnd
#MEnd
#
##############################################################
#
#Move A_SS_Tier5_Alt1
#	Requires	FLY COMBAT CAGE ALTERNATE ONE
#	Priority        5
#	Interrupts	<FLYAttackAll>
#	Member		<DeathIRQ> <FLYAttackAll>
#	NextMove        A_SS_Tier5_Alt1_Post
#	Sets            CANTMOVE
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim          male\air_cage 1 30
#	TEnd
#
#	Type		huge
#	  Anim          huge\air_cage 1 30
#	TEnd
#
#	Type		fem
#	  Anim          fem\air_cage 1 30
#	TEnd
#
#	Type		Rularuu_male
#	  Anim          male\mystic_handclap 1 30
#	TEnd
#MEnd
#
#Move A_SS_Tier5_Alt1_Post
#	Requires	FLY COMBAT
#	Priority        2
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <FlyCombatAttack> <FLYAttackAll> <BlockIRQ>
#	NextMove        C_flyready
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim          male\air_cage 31
#	TEnd
#
#	Type		huge
#	  Anim          huge\air_cage 31
#	TEnd
#
#	Type		fem
#	  Anim          fem\air_cage 31
#	TEnd
#
#	Type		Rularuu_male
#	  Anim          male\mystic_handclap 31
#	TEnd
#MEnd

## SUPER STRENGTH - TIER 6 (Knockout Blow) ###########################################################

Move SS_Tier6_Shield_Alt1
	Requires	COMBAT WINDUPPERCUT ALTERNATE ONE SHIELD
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        SS_Tier6_Shield_Alt1_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\SS_KnockoutBlow_shield 1 66
	TEnd

	Type		fem
	  Anim          fem\SS_KnockoutBlow_shield 1 66
	TEnd

	Type		huge
	  Anim          huge\SS_KnockoutBlow_shield 1 66
	TEnd
MEnd

Move SS_Tier6_Shield_Alt1_post
	Requires	COMBAT SHIELD
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <CombatAttack> <AttackIRQ> <MoveIRQ>
	NextMove        Shield_CombatReady_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\SS_KnockoutBlow_shield 67 76
	TEnd

	Type		fem
	  Anim          fem\SS_KnockoutBlow_shield 67 76
	TEnd

	Type		huge
	  Anim          huge\SS_KnockoutBlow_shield 67 76
	TEnd

MEnd

#############################################################

Move A_SS_Tier6_Shield_Alt1
	Requires	FLY COMBAT WINDUPPERCUT ALTERNATE ONE SHIELD
	Priority        27
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        A_SS_Tier6_Shield_Alt1_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\SS_knockoutBlow_shield_air 1 66
	TEnd

	Type		fem
	  Anim          male\SS_knockoutBlow_shield_air 1 66
	TEnd

	Type		huge
	  Anim          huge\SS_knockoutBlow_shield_air 1 66
	TEnd
MEnd

Move A_SS_Tier6_Shield_Alt1_Post
	Requires	FLY COMBAT SHIELD
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <FlyCombatAttack> <AttackIRQ> <BlockIRQ> <MoveIRQ>
	NextMove        Shield_CombatFly_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\SS_knockoutBlow_shield_air 67
	TEnd

	Type		fem
	  Anim          male\SS_knockoutBlow_shield_air 67
	TEnd

	Type		huge
	  Anim          huge\SS_knockoutBlow_shield_air 67
	TEnd

MEnd


### SUPER STRENGTH - TIER 7 (Rage)###########################################################
#
#Move SS_Tier7_Alt1
#	Requires	BOOSTDOWN ALTERNATE ONE
#	Priority        3
#	Interrupts	<ReactIRQ> <HitIRQ> <AttackIRQ>
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <BlockIRQ> <MoveIRQ>
#	NextMove        SS_Tier7_Alt1_Post
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim          male\boost 1
#	TEnd
#
#	Type		huge
#	  Anim          huge\boostdown 1
#	TEnd
#
#	Type		fem
#	  Anim          fem\boostdown 1
#	TEnd
#MEnd
#
#Move SS_Tier7_Alt1_Post
#	Member		<Ready> <ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <PostMoveCycle>
#	Flags		Cycle NoSizeScale
#
#	Type		male
#	  Anim          male\Skel_ready2 1 60
#	TEnd
#
#	Type		huge
#	  Anim          huge\mon_ready 1 60
#	TEnd
#
#	Type		fem
#	  Anim          fem\Skel_ready2 1 60
#	TEnd
#MEnd
#
##############################################################
#
#Move A_SS_Tier7_Alt1
#	Requires	BOOSTDOWN ALTERNATE ONE
#	Priority        3
#	Interrupts	<ReactIRQ> <HitIRQ> <AttackIRQ>
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <BlockIRQ> <MoveIRQ>
#	NextMove        A_SS_Tier7_Alt1_Post
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim          male\boost 1
#	TEnd
#
#	Type		huge
#	  Anim          huge\boostdown 1
#	TEnd
#
#	Type		fem
#	  Anim          fem\boostdown 1
#	TEnd
#MEnd
#
#Move A_SS_Tier7_Alt1_Post
#	Member		<Ready> <ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <PostMoveCycle>
#	Flags		Cycle NoSizeScale
#
#	Type		male
#	  Anim          male\Skel_ready2 1 60
#	TEnd
#
#	Type		huge
#	  Anim          huge\mon_ready 1 60
#	TEnd
#
#	Type		fem
#	  Anim          fem\Skel_ready2 1 60
#	TEnd
#MEnd


### SUPER STRENGTH - TIER 8 (Hurl) ###########################################################
#
#Move SS_Tier8_Alt1_Squat
#	Requires	COMBAT SHORTTOSS ALTERNATE ONE
#	Priority        4
#	Interrupts	<AttackIRQ>
#	Member		<DeathIRQ> <AttackIRQ>
#	NextMove        SS_Tier8_Alt1_Lift
#	Sets            CANTMOVE
#	Flags		NoSizeScale FixArmReg
#	Scale           1.5
#
#	Type		male
#	  Anim          male\short_toss 1 30
#	TEnd
#
#	Type		huge
#	  Anim          huge\short_toss 1 30
#	TEnd
#
#	Type		fem
#	  Anim          fem\short_toss 1 30
#	TEnd
#MEnd
#
#Move SS_Tier8_Alt1_Lift
#	Member		<DeathIRQ> <AttackIRQ>
#	NextMove        SS_Tier8_Alt1_Toss
#	Sets            CANTMOVE
#	Flags		NoSizeScale FixArmReg
#	Scale           1.625
#
#	Type		male
#	  Anim          male\short_toss 31 85
#	TEnd
#
#	Type		huge
#	  Anim          huge\short_toss 31 85
#	TEnd
#
#	Type		fem
#	  Anim          fem\short_toss 31 85
#	TEnd
#MEnd
#
#Move SS_Tier8_Alt1_Toss
#	Member		<DeathIRQ> <AttackIRQ>
#	NextMove        SS_Tier8_Alt1_Toss_post
#	Sets            CANTMOVE
#	Flags		NoSizeScale FixArmReg
#	Scale           1.3
#
#	Type		male
#	  Anim          male\short_toss 86 115
#	TEnd
#
#	Type		huge
#	  Anim          huge\short_toss 86 115
#	TEnd
#
#	Type		fem
#	  Anim          fem\short_toss 86 115
#	TEnd
#MEnd
#
#Move SS_Tier8_Alt1_Toss_post
#	Requires	COMBAT
#	Priority        1
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <CombatAttack> <AttackIRQ> <MoveIRQ>
#	NextMove        Combat_half
#	Scale           0.50
#	Flags		NoSizeScale FixArmReg
#
#	Type		male
#	  Anim          male\short_toss 116
#	TEnd
#
#	Type		huge
#	  Anim          huge\short_toss 116
#	TEnd
#
#	Type		fem
#	  Anim          fem\short_toss 116
#	TEnd
#MEnd
#
#############################################################
#
#Move A_SS_Tier8_Alt1
#	Requires	FLY COMBAT KUNGFU KUNGFUAREA ALTERNATE ONE
#	Priority        6
#	Interrupts	<FLYAttackAll>
#	Member		<DeathIRQ> <FLYAttackAll>
#	NextMove        A_SS_Tier8_Alt1_Post
#	Sets            CANTMOVE
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim          male\air_MA_sweepkick 40 85
#	TEnd
#MEnd
#
#Move A_SS_Tier8_Alt1_Post
#	Requires	FLY COMBAT
#	Priority        2
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <FlyCombat> <FLYAttackAll>
#	NextMove        C_flycycle_half
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim          male\air_MA_sweepkick 86
#	TEnd
#MEnd


### SUPER STRENGTH - TIER 9 (Foot Stomp) ###########################################################

Move SS_Tier9_Shield_Alt1
	Requires	COMBAT STOMP ALTERNATE ONE SHIELD
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        SS_Tier9_Shield_Alt1_2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\SS_ground_punch_shield 1 70
	TEnd

	Type		huge
	  Anim          male\SS_ground_punch_shield 1 70
	TEnd

	Type		fem
	  Anim          male\SS_ground_punch_shield 1 70
	TEnd

	Type		RockSuit
	  Anim          male\SS_ground_punch_shield 1 70
	TEnd
MEnd

Move SS_Tier9_Shield_Alt1_2
	Requires	COMBAT SHIELD
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <BlockIRQ> <MoveIRQ>
	NextMove        Shield_CombatReady_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\SS_ground_punch_shield 70
	TEnd

	Type		huge
	  Anim          male\SS_ground_punch_shield 70
	TEnd

	Type		fem
	  Anim          male\SS_ground_punch_shield 70
	TEnd

	Type		RockSuit
	  Anim          male\SS_ground_punch_shield 70
	TEnd
MEnd

#############################################################
#
#Move A_SS_Tier9_Alt1
#	Requires	FLY COMBAT KUNGFU KICK KUNGFUSTUN ALTERNATE ONE
#	Priority        7
#	Interrupts	<FLYAttackAll>
#	Member		<DeathIRQ> <FLYAttackAll>
#	NextMove        A_SS_Tier9_Alt1_Post
#	Sets            CANTMOVE
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim          male\MA_air_dropkick 1 90
#	TEnd
#MEnd
#
#Move A_SS_Tier9_Alt1_Post
#	Requires	FLY COMBAT
#	Priority        2
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <FlyCombat> <FLYAttackAll>
#	NextMove        Lazy_C_flycycle_Blend
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim          male\MA_air_dropkick 91
#	TEnd
#MEnd