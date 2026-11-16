#############################################################
## Weapons_LongStaff.inc
#############################################################

## Draw Long Staff Air ###########################################################

Move A_Staff_draw
	Requires	FLY COMBAT WEAPON 2HLARGE STAFF SPAWNBACK
	Priority        66
	Interrupts	<AttackIRQ> <ReactIRQ>
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_Draw_2hlarge_Post
	Flags		FixArmReg NOSIZESCALE

	Type		male
	  Anim          male\air_2hlarge_draw_basic 1 2
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		huge
	  Anim          huge\air_2hlarge_draw_basic 1 2
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		fem
	  Anim          fem\air_2hlarge_draw_basic 1 2
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

MEnd

Move A_Staff_draw_Post ##Post move is set up like this to allow combat mode to reset properly between similiar weapon powers (prevents doubling of 'rifle' models)
	Requires	FLY COMBAT WEAPON 2HLARGE STAFF
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_2hlargeReady
	Flags		FixArmReg NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_2hlarge_draw_basic 2
	TEnd

	Type		huge
	  Anim          huge\air_2hlarge_draw_basic 2
	TEnd

	Type		fem
	  Anim          fem\air_2hlarge_draw_basic 2
	TEnd

MEnd

## Mercurial_blow A Air ###########################################################

Move A_Mercurial_blow_A_Draw
	Requires	FLY COMBAT WEAPON 2hlarge STAFF QUICK SPAWNBACK
	Priority        77
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Mercurial_blow_A
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\air_2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		fem
	  Anim		fem\air_2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		huge
	  Anim		huge\air_2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd
MEnd

Move A_Mercurial_blow_A
	Requires	FLY COMBAT WEAPON 2hlarge STAFF QUICK
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Mercurial_blow_A_post
	Flags		NoSizeScale Pitchtotarget
	Sets            CANTMOVE
	Interpolate	0

	Type		male
	  Anim		male\staff_mercurial_blow_A_air 1 30
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim		fem\staff_mercurial_blow_A_air 1 30
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim		huge\staff_mercurial_blow_A_air 1 30
	  PitchAngle	0.3
	TEnd

MEnd

Move A_Mercurial_blow_A_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_2hlargeReady
	Flags		ReqInputs
	Scale		0.5

	Type		male
	  Anim          male\staff_mercurial_blow_A_air 31 64
	TEnd

	Type		fem
	  Anim          fem\staff_mercurial_blow_A_air 31 64
	TEnd

	Type		huge
	  Anim          huge\staff_mercurial_blow_A_air 31 64
	TEnd

MEnd

### Mercurial_blow B Air ###########################################################

Move A_Mercurial_blow_B
	Requires	FLY COMBAT WEAPON 2hlarge STAFF QUICK
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Mercurial_blow_B_post
	Flags		NoSizeScale Pitchtotarget
	Sets            CANTMOVE
	Interpolate	0

	Type		male
	  Anim		male\staff_mercurial_blow_B_air 1 30
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim		fem\staff_mercurial_blow_B_air 1 30
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim		huge\staff_mercurial_blow_B_air 1 30
	  PitchAngle	0.3
	TEnd

MEnd

Move A_Mercurial_blow_B_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_2hlargeReady
	Flags		ReqInputs
	Scale		0.5

	Type		male
	  Anim          male\staff_mercurial_blow_B_air 31 64
	TEnd

	Type		fem
	  Anim          fem\staff_mercurial_blow_B_air 31 64
	TEnd

	Type		huge
	  Anim          huge\staff_mercurial_blow_B_air 31 64
	TEnd

MEnd

### Mercurial_blow C Air ###########################################################

Move A_Mercurial_blow_C
	Requires	FLY COMBAT WEAPON 2hlarge STAFF QUICK
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Mercurial_blow_C_post
	Flags		NoSizeScale Pitchtotarget
	Sets            CANTMOVE
	Interpolate	0

	Type		male
	  Anim		male\staff_mercurial_blow_C_air 1 30
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim		fem\staff_mercurial_blow_C_air 1 30
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim		huge\staff_mercurial_blow_C_air 1 30
	  PitchAngle	0.3
	TEnd
MEnd

Move A_Mercurial_blow_C_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_2hlargeReady
	Flags		ReqInputs
	Scale		0.5

	Type		male
	  Anim          male\staff_mercurial_blow_C_air 31 64
	TEnd

	Type		fem
	  Anim          fem\staff_mercurial_blow_C_air 31 64
	TEnd

	Type		huge
	  Anim          huge\staff_mercurial_blow_C_air 31 64
	TEnd

MEnd

### Mercurial_blow D Air ###########################################################

Move A_Mercurial_blow_D
	Requires	FLY COMBAT WEAPON 2hlarge STAFF QUICK
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Mercurial_blow_D_post
	Flags		NoSizeScale Pitchtotarget
	Sets            CANTMOVE
	Interpolate	0

	Type		male
	  Anim		male\staff_mercurial_blow_D_air 1 30
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim		fem\staff_mercurial_blow_D_air 1 30
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim		huge\staff_mercurial_blow_D_air 1 30
	  PitchAngle	0.3
	TEnd

MEnd

Move A_Mercurial_blow_D_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_2hlargeReady
	Flags		ReqInputs
	Scale		0.5

	Type		male
	  Anim          male\staff_mercurial_blow_D_air 31 64
	TEnd

	Type		fem
	  Anim          fem\staff_mercurial_blow_D_air 31 64
	TEnd

	Type		huge
	  Anim          huge\staff_mercurial_blow_D_air 31 64
	TEnd

MEnd


####### Precise_strike ##############################################################

Move A_Precise_strike_Draw
	Requires	FLY COMBAT WEAPON 2hlarge STAFF STRONG SPAWNBACK
	Priority        17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Precise_strike
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\air_2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		fem
	  Anim		fem\air_2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		huge
	  Anim		huge\air_2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd
MEnd

Move A_Precise_strike
	Requires	FLY COMBAT WEAPON 2hlarge STAFF STRONG
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Precise_strike_post
	Flags		NoSizeScale Pitchtotarget
	Sets            CANTMOVE
	Interpolate	0

	Type		male
	  Anim		male\staff_precise_strike_air 1 34
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim		fem\staff_precise_strike_air 1 34
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim		huge\staff_precise_strike_air 1 34
	  PitchAngle	0.3
	TEnd

MEnd

Move A_Precise_strike_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_2hlargeReady
	Flags		ReqInputs
	Scale		0.7

	Type		male
	  Anim          male\staff_precise_strike_air 35 75
	TEnd

	Type		fem
	  Anim          fem\staff_precise_strike_air 35 75
	TEnd

	Type		huge
	  Anim          huge\staff_precise_strike_air 35 75
	TEnd

MEnd

####### Guard_spin ##############################################################

Move A_Guard_spin_Draw
	Requires	FLY COMBAT WEAPON 2hlarge STAFF SPIN SPAWNBACK
	Priority        17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Guard_spin
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\air_2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		fem
	  Anim		fem\air_2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		huge
	  Anim		huge\air_2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd
MEnd

Move A_Guard_spin
	Requires	FLY COMBAT WEAPON 2hlarge STAFF SPIN
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Guard_spin_post
	Flags		NoSizeScale Pitchtotarget
	Sets            CANTMOVE
	Interpolate	0

	Type		male
	  Anim		male\staff_Guard_spin_air 1 55
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim		fem\staff_Guard_spin_air 1 55
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim		huge\staff_Guard_spin_air 1 55
	  PitchAngle	0.3
	TEnd

MEnd

Move A_Guard_spin_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_2hlargeReady
	Flags		ReqInputs
	Scale		0.7

	Type		male
	  Anim          male\staff_Guard_spin_air 56 70
	TEnd

	Type		fem
	  Anim          fem\staff_Guard_spin_air 56 70
	TEnd

	Type		huge
	  Anim          huge\staff_Guard_spin_air 56 70
	TEnd

MEnd

####### Eye_of_Storm Air ##############################################################

Move A_Eye_of_Storm_Draw
	Requires	FLY COMBAT WEAPON 2hlarge STAFF WHIRLWIND SPAWNBACK
	Priority        78
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Eye_of_Storm
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\air_2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim		fem\air_2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim		huge\air_2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	  PitchAngle	0.3
	TEnd
MEnd

Move A_Eye_of_Storm
	Requires	FLY COMBAT WEAPON 2hlarge STAFF WHIRLWIND
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Eye_of_Storm_post
	Flags		NoSizeScale Pitchtotarget
	Sets            CANTMOVE
	Interpolate	0

	Type		male
	  Anim		male\Staff_eye_of_storm_air 1 77
	TEnd

	Type		fem
	  Anim		fem\Staff_eye_of_storm_air 1 77
	TEnd

	Type		huge
	  Anim		huge\Staff_eye_of_storm_air 1 77
	TEnd

MEnd

Move A_Eye_of_Storm_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_2hlargeReady
	Flags		ReqInputs
	Scale		0.7

	Type		male
	  Anim          male\Staff_eye_of_storm_air 78 106
	TEnd

	Type		fem
	  Anim          fem\Staff_eye_of_storm_air 78 106
	TEnd

	Type		huge
	  Anim          huge\Staff_eye_of_storm_air 78 106
	TEnd

MEnd

####### Staff Mastery #############################################################

Move A_StaffMastery_BODY
	Requires	FLY COMBAT WEAPON 2hlarge STAFF BOAST FORMOFTHEBODY
	Priority        17
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_StaffMastery_BODY_Post
#	Flags		NoSizeScale
	Interpolate	0

	Type		male
	  Anim		male\Staff_mastery_air 1 60
	TEnd

	Type		fem
	  Anim		fem\Staff_mastery_air 1 60
	TEnd

	Type		huge
	  Anim		huge\Staff_mastery_air 1 60
	TEnd

MEnd


Move A_StaffMastery_BODY_Post
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_2hlargeReady
#	Flags		NoSizeScale
	Interpolate	0

	Type		male
	  Anim		male\Staff_mastery_air 61
	TEnd

	Type		fem
	  Anim		fem\Staff_mastery_air 61
	TEnd

	Type		huge
	  Anim		huge\Staff_mastery_air 61
	TEnd

MEnd

Move A_StaffMastery_MIND
	Requires	FLY COMBAT WEAPON 2hlarge STAFF BOAST FORMOFTHEMIND
	Priority        17
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_StaffMastery_MIND_Post
#	Flags		NoSizeScale
	Interpolate	0

	Type		male
	  Anim		male\Staff_mastery_air 1 60
	TEnd

	Type		fem
	  Anim		fem\Staff_mastery_air 1 60
	TEnd

	Type		huge
	  Anim		huge\Staff_mastery_air 1 60
	TEnd

MEnd

Move A_StaffMastery_MIND_Post
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_2hlargeReady
#	Flags		NoSizeScale
	Interpolate	0

	Type		male
	  Anim		male\Staff_mastery_air 61
	TEnd

	Type		fem
	  Anim		fem\Staff_mastery_air 61
	TEnd

	Type		huge
	  Anim		huge\Staff_mastery_air 61
	TEnd

MEnd

Move A_StaffMastery_SOUL
	Requires	FLY COMBAT WEAPON 2hlarge STAFF BOAST FORMOFTHESOUL
	Priority        17
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_StaffMastery_SOUL_Post
#	Flags		NoSizeScale
	Interpolate	0

	Type		male
	  Anim		male\Staff_mastery_air 1 60
	TEnd

	Type		fem
	  Anim		fem\Staff_mastery_air 1 60
	TEnd

	Type		huge
	  Anim		huge\Staff_mastery_air 1 60
	TEnd

MEnd

Move A_StaffMastery_SOUL_Post
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_2hlargeReady
#	Flags		NoSizeScale
	Interpolate	0

	Type		male
	  Anim		male\Staff_mastery_air 61
	TEnd

	Type		fem
	  Anim		fem\Staff_mastery_air 61
	TEnd

	Type		huge
	  Anim		huge\Staff_mastery_air 61
	TEnd

MEnd

####### Staff_Taunt Air ##############################################################

Move A_Staff_Taunt_Draw
	Requires	FLY COMBAT WEAPON 2hlarge STAFF TAUNT SPAWNBACK
	Priority        17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Staff_Taunt
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\air_2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		fem
	  Anim		fem\air_2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		huge
	  Anim		huge\air_2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd
MEnd

Move A_Staff_Taunt
	Requires	FLY COMBAT WEAPON 2hlarge STAFF TAUNT
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Staff_Taunt_post
	Flags		NoSizeScale Pitchtotarget
	Sets            CANTMOVE
	Interpolate	0

	Type		male
	  Anim		male\staff_Taunt_air 1 75
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim		fem\staff_Taunt_air 1 75
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim		huge\staff_Taunt_air 1 75
	  PitchAngle	0.3
	TEnd

MEnd

Move A_Staff_Taunt_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_2hlargeReady
	Flags		ReqInputs
	Scale		0.7

	Type		male
	  Anim          male\staff_Taunt_air 75 106
	TEnd

	Type		fem
	  Anim          fem\staff_Taunt_air 75 106
	TEnd

	Type		huge
	  Anim          huge\staff_Taunt_air 75 106
	TEnd

MEnd

####### Serpent Reach #############################################################

Move A_Serpent_Reach_Draw
	Requires	 FLY COMBAT WEAPON 2hlarge STAFF LAUNCH SPAWNBACK
	Priority        78
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Serpent_Reach
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\air_2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		fem
	  Anim		fem\air_2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		huge
	  Anim		huge\air_2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd
MEnd

Move A_Serpent_Reach
	Requires	FLY COMBAT WEAPON 2hlarge STAFF LAUNCH
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Serpent_Reach_post
	Flags		NoSizeScale pitchtotarget
	Sets            CANTMOVE
	Interpolate	0

	Type		male
	  Anim		male\Staff_Serpent_Reach_air 1 53
	  PitchAngle	-0.9
	  #PitchStart	28
	TEnd

	Type		fem
	  Anim		fem\Staff_Serpent_Reach_air 1 53
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim		huge\Staff_Serpent_Reach_air 1 53
	  PitchAngle	0.3
	TEnd

MEnd


Move A_Serpent_Reach_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_2hlargeReady
	Flags		ReqInputs
	Scale		0.8

	Type		male
	  Anim          male\Staff_Serpent_Reach_air 54 69
	TEnd

	Type		fem
	  Anim          fem\Staff_Serpent_Reach_air 54 69
	TEnd

	Type		huge
	  Anim          huge\Staff_Serpent_Reach_air 54 69
	TEnd

MEnd

####### Innocuous_strike #############################################################

Move A_Innocuous_strike_Draw
	Requires	 FLY COMBAT WEAPON 2hlarge STAFF CONE SPAWNBACK
	Priority        78
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Innocuous_strike
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\air_2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		fem
	  Anim		fem\air_2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		huge
	  Anim		huge\air_2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd
MEnd

Move A_Innocuous_strike
	Requires	FLY COMBAT WEAPON 2hlarge STAFF CONE
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Innocuous_strike_post
	Flags		NoSizeScale pitchtotarget
	Sets            CANTMOVE
	Interpolate	0

	Type		male
	  Anim		male\Staff_Innocuous_strike_air 1 65
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim		fem\Staff_Innocuous_strike_air 1 65
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim		huge\Staff_Innocuous_strike_air 1 65
	  PitchAngle	0.3
	TEnd

MEnd

Move A_Innocuous_strike_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_2hlargeReady
	Flags		ReqInputs
	Scale		0.7

	Type		male
	  Anim          male\Staff_Innocuous_strike_air 66 100
	TEnd

	Type		fem
	  Anim          fem\Staff_Innocuous_strike_air 66 71
	TEnd

	Type		huge
	  Anim          huge\Staff_Innocuous_strike_air 66 71
	TEnd

MEnd

####### Sky Splitter #############################################################

Move A_Sky_Splitter_Draw
	Requires	 FLY COMBAT WEAPON 2hlarge STAFF SMASH SPAWNBACK
	Priority        78
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Sky_Splitter
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\air_2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		fem
	  Anim		fem\air_2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		huge
	  Anim		huge\air_2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd
MEnd

Move A_Sky_Splitter
	Requires	FLY COMBAT WEAPON 2hlarge STAFF SMASH
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Sky_Splitter_post
	Flags		NoSizeScale pitchtotarget
	Sets            CANTMOVE
	Interpolate	0

	Type		male
	  Anim		male\Staff_Sky_Splitter_air 1 85
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim		fem\Staff_Sky_Splitter_air 1 85
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim		huge\Staff_Sky_Splitter_air 1 85
	  PitchAngle	0.3
	TEnd

MEnd

Move A_Sky_Splitter_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	NextMove        A_2hlargeReady
	Flags		ReqInputs
	Scale		0.7

	Type		male
	  Anim          male\Staff_Sky_Splitter_air 86 94
	TEnd

	Type		fem
	  Anim          fem\Staff_Sky_Splitter_air 86 94
	TEnd

	Type		huge
	  Anim          huge\Staff_Sky_Splitter_air 86 98
	TEnd

MEnd

## Staff Assassins Strike #############################################

Move A_Staff_Assassins_Draw
	Requires	FLY COMBAT WEAPON 2HLARGE STAFF WINDUPLONGSTAFF SPAWNBACK
	Priority        77
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ> <IRQEmotesDualhand>
	NextMove	A_Staff_Assassins_Windup
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\air_2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		fem
	  Anim		fem\air_2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		huge
	  Anim		huge\air_2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

MEnd

Move A_Staff_Assassins_Windup
	Requires	FLY COMBAT WINDUPLONGSTAFF
	Priority	13
	Interrupts	<AttackIRQ> <DeathIRQ> <HitIRQ> <MoveIRQ> <IRQEmotesDualhand>
	Include sequencers\Members\Windups_Air.inc
	Flags		NoSizeScale PitchToTarget
	NextMove	A_Staff_Assassins_Windup_Cycle

	Type		male
	  Anim		male\air_staff_assassins_strike 1 29
	TEnd
MEnd

Move A_Staff_Assassins_Windup_Cycle
	Requires	FLY COMBAT WINDUPLONGSTAFF
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ> <IRQEmotesDualhand>
	Include sequencers\Members\Windups_Air.inc
	Flags		Cycle NoSizeScale PitchToTarget
	NextMove	A_Staff_Assassins_Strike

	Type		male
	  Anim		male\air_staff_assassins_strike 30 54
	TEnd
MEnd

#############################################################

Move A_Staff_Assassins_Quick_Draw
	Requires	FLY COMBAT WEAPON 2HLARGE STAFF CHARGE SPAWNBACK
	Priority        77
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ> <IRQEmotesDualhand>
	NextMove	A_Staff_Assassins_Strike
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\air_2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		fem
	  Anim		fem\air_2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		huge
	  Anim		huge\air_2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

MEnd

Move A_Staff_Assassins_Strike
	Requires	FLY COMBAT WEAPON 2hlarge STAFF CHARGE
	Priority	27
	Interrupts	<AttackIRQ> <Windups>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Staff_Assassins_Strike_Post
	Sets		CANTMOVE
	Flags		NoSizeScale Pitchtotarget


	Type		male
	  Anim		male\air_staff_assassins_strike 50 80
	  PitchAngle	0.3
	TEnd
MEnd

Move A_Staff_Assassins_Strike_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand_Air.inc
	#Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove	A_2hlargeReady
	Flags		ReqInputs

	Type		male
	  Anim		male\air_staff_assassins_strike 81
	TEnd
MEnd

################################
###### Two Handed Boostup  ######
################################

Move A_Staff_2hlarge_Boostup_Draw
	Requires	FLY COMBAT WEAPON 2hlarge STAFF WEPBOOSTUP HIGH SPAWNBACK
	Priority        88
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_2hlarge_Boostup
	Sets            CANTMOVE
	Flags		NOSIZESCALE FixArmReg

	Type		male
	  Anim		male\air_2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		fem
	  Anim		fem\air_2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		huge
	  Anim		huge\air_2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd
MEnd

##################
#####Air Staff Placate ####
##################

Move A_Staff_Placate_Draw
	Requires	FLY COMBAT WEAPON 2hlarge STAFF PLACATE SPAWNBACK
	Priority        77
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Staff_Placate
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\air_2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		fem
	  Anim		fem\air_2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		huge
	  Anim		huge\air_2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd
MEnd

Move A_Staff_Placate
	Requires	FLY COMBAT WEAPON 2hlarge STAFF PLACATE
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Staff_Placate2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_staff_placate 1 40
	TEnd
MEnd

Move A_Staff_Placate2
	Requires	FLY COMBAT
	#Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        A_2hlargeReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_staff_placate 40
	TEnd
MEnd




