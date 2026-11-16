#############################################################
## Weapons_LongStaff.inc
#############################################################

## Draw Long Staff ###########################################################

Move Staff_draw
	Requires	COMBAT WEAPON 2HLARGE STAFF SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        Staff_draw_Post
	Flags		FixArmReg NOSIZESCALE

	Type		male
	  Anim          male\2hlarge_draw_basic 1 2
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		huge
	  Anim          huge\2hlarge_draw_basic 1 2
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		fem
	  Anim          fem\2hlarge_draw_basic 1 2
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd


MEnd

Move Staff_draw_Post ##Post move is set up like this to allow combat mode to reset properly between similiar weapon powers (prevents doubling of 'rifle' models)
	Requires	COMBAT WEAPON 2HLARGE STAFF
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        2hlargeReady
	Flags		FixArmReg NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\2hlarge_draw_basic 2 30
	TEnd

	Type		huge
	  Anim          huge\2hlarge_draw_basic 2 30
	TEnd

	Type		fem
	  Anim          fem\2hlarge_draw_basic 2 30
	TEnd



MEnd

## Mercurial_blow A ###########################################################

Move Mercurial_blow_A_Draw
	Requires	COMBAT WEAPON 2hlarge STAFF QUICK SPAWNBACK
	Priority        66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Mercurial_blow_A
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		fem
	  Anim		fem\2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		huge
	  Anim		huge\2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd
MEnd

Move Mercurial_blow_A
	Requires	COMBAT WEAPON 2hlarge STAFF QUICK
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Mercurial_blow_A_post
	Flags		NoSizeScale pitchtotarget
	Sets            CANTMOVE
	Interpolate	0
	STICKSONCHILD	PUNCH RIGHT

	Type		male
	  Anim		male\staff_mercurial_blow_A 1 30
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
	  PitchAngle    0.3
	TEnd

	Type		fem
	  Anim		fem\staff_mercurial_blow_A 1 30
	  PitchAngle    0.3
	TEnd

	Type		huge
	  Anim		huge\staff_mercurial_blow_A 1 30
	  PitchAngle    0.3
	TEnd

MEnd

Move Mercurial_blow_A_post
	Requires	COMBAT 2hlarge
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        2hlargeReady
	Flags		ReqInputs
	Scale		0.5

	Type		male
	  Anim          male\staff_mercurial_blow_A 31 67
	TEnd

	Type		fem
	  Anim          fem\staff_mercurial_blow_A 31 67
	TEnd

	Type		huge
	  Anim          huge\staff_mercurial_blow_A 31 67
	TEnd

MEnd

## Mercurial_blow B ###########################################################
#
Move Mercurial_blow_B
	Requires	COMBAT WEAPON 2hlarge STAFF QUICK
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Mercurial_blow_B_post
	Flags		NoSizeScale pitchtotarget
	Sets            CANTMOVE
	Interpolate	0
	STICKSONCHILD	PUNCH LEFT

	Type		male
	  Anim		male\staff_mercurial_blow_B 1 30
	  PitchAngle    0.3
	TEnd

	Type		fem
	  Anim		fem\staff_mercurial_blow_B 1 30
	  PitchAngle    0.3
	TEnd

	Type		huge
	  Anim		huge\staff_mercurial_blow_B 1 30
	  PitchAngle    0.3
	TEnd


MEnd

Move Mercurial_blow_B_post
	Requires	COMBAT 2hlarge
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        2hlargeReady
	Flags		ReqInputs
	Scale		0.7

	Type		male
	  Anim          male\staff_mercurial_blow_B 31 67
	TEnd

	Type		fem
	  Anim          fem\staff_mercurial_blow_B 31 67
	TEnd

	Type		huge
	  Anim          huge\staff_mercurial_blow_B 31 67
	TEnd

MEnd

## Mercurial_blow C ###########################################################

Move Mercurial_blow_C
	Requires	COMBAT WEAPON 2hlarge STAFF QUICK
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Mercurial_blow_C_post
	Flags		NoSizeScale pitchtotarget
	Sets            CANTMOVE
	Interpolate	0
	STICKSONCHILD	PUNCH UP
	Type		male
	  Anim		male\staff_mercurial_blow_C 1 30
	  PitchAngle    0.3
	TEnd

	Type		fem
	  Anim		fem\staff_mercurial_blow_C 1 30
	  PitchAngle    0.3
	TEnd

	Type		huge
	  Anim		huge\staff_mercurial_blow_C 1 30
	  PitchAngle    0.3
	TEnd
MEnd

Move Mercurial_blow_C_post
	Requires	COMBAT 2hlarge
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        2hlargeReady
	Flags		ReqInputs
	Scale		0.7

	Type		male
	  Anim          male\staff_mercurial_blow_C 31 67
	TEnd

	Type		fem
	  Anim          fem\staff_mercurial_blow_C 31 67
	TEnd

	Type		huge
	  Anim          huge\staff_mercurial_blow_C 31 67
	TEnd

MEnd

## Mercurial_blow D ###########################################################

Move Mercurial_blow_D
	Requires	COMBAT WEAPON 2hlarge STAFF QUICK
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Mercurial_blow_D_post
	Flags		NoSizeScale pitchtotarget
	Sets            CANTMOVE
	Interpolate	0
	STICKSONCHILD	PUNCH DOWN

	Type		male
	  Anim		male\staff_mercurial_blow_D 1 30
	  PitchAngle    0.3
	TEnd

	Type		fem
	  Anim		fem\staff_mercurial_blow_D 1 30
	  PitchAngle    0.3
	TEnd

	Type		huge
	  Anim		huge\staff_mercurial_blow_D 1 30
	  PitchAngle    0.3
	TEnd

MEnd

Move Mercurial_blow_D_post
	Requires	COMBAT 2hlarge
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        2hlargeReady
	Flags		ReqInputs
	Scale		0.7

	Type		male
	  Anim          male\staff_mercurial_blow_D 31 67
	TEnd

	Type		fem
	  Anim          fem\staff_mercurial_blow_D 31 67
	TEnd

	Type		huge
	  Anim          huge\staff_mercurial_blow_D 31 67
	TEnd

MEnd

###### Precise_strike ##############################################################

Move Precise_strike_Draw
	Requires	COMBAT WEAPON 2hlarge STAFF STRONG SPAWNBACK
	Priority        66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Precise_strike
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		fem
	  Anim		fem\2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		huge
	  Anim		huge\2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd
MEnd

Move Precise_strike
	Requires	COMBAT WEAPON 2hlarge STAFF STRONG
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Precise_strike_post
	Flags		NoSizeScale pitchtotarget
	Sets            CANTMOVE
	Interpolate	4

	Type		male
	  Anim		male\staff_precise_strike 1 34
	  PitchAngle    0.3
	TEnd

	Type		fem
	  Anim		fem\staff_precise_strike 1 34
	  PitchAngle    0.3
	TEnd

	Type		huge
	  Anim		huge\staff_precise_strike 1 34
	  PitchAngle    0.3
	TEnd

MEnd

Move Precise_strike_post
	Requires	COMBAT 2hlarge
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        2hlargeReady
	Flags		ReqInputs
	Scale 		0.7

	Type		male
	  Anim          male\staff_precise_strike 35 45
	TEnd

	Type		fem
	  Anim          fem\staff_precise_strike 35 45
	TEnd

	Type		huge
	  Anim          huge\staff_precise_strike 35 45
	TEnd

MEnd

####### Guard_spin #############################################################

Move Guard_spin_Draw
	Requires	COMBAT WEAPON 2hlarge STAFF SPIN SPAWNBACK
	Priority        66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Guard_spin
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		fem
	  Anim		fem\2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		huge
	  Anim		huge\2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

MEnd

Move Guard_spin
	Requires	COMBAT WEAPON 2hlarge STAFF SPIN
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Guard_spin_post
	Sets            CANTMOVE
	Flags		NoSizeScale pitchtotarget
	Interpolate	0

	Type		male
	  Anim		male\staff_guard_spin 1 55
	  PitchAngle    0.3
	TEnd

	Type		fem
	  Anim		fem\staff_guard_spin 1 55
	  PitchAngle    0.3
	TEnd

	Type		huge
	  Anim		huge\staff_guard_spin 1 55
	  PitchAngle    0.3
	TEnd

MEnd

Move Guard_spin_post
	Requires	COMBAT 2hlarge
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        2hlargeReady
	Flags		ReqInputs
	Scale		0.7

	Type		male
	  Anim          male\staff_guard_spin 56 70
	TEnd

	Type		fem
	  Anim          fem\staff_guard_spin 56 70
	TEnd

	Type		huge
	  Anim          huge\staff_guard_spin 56 70
	TEnd
MEnd

####### Eye Of Storm #############################################################

Move Eye_of_Storm_Draw
	Requires	COMBAT WEAPON 2hlarge STAFF WHIRLWIND SPAWNBACK
	Priority        66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Eye_of_Storm
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		fem
	  Anim		fem\2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		huge
	  Anim		huge\2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd
MEnd

Move Eye_of_Storm
	Requires	COMBAT WEAPON 2hlarge STAFF WHIRLWIND
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Eye_of_Storm_post
	Flags		NoSizeScale
	Sets            CANTMOVE
	Interpolate	0

	Type		male
	  Anim		male\Staff_eye_of_storm 1 77
	TEnd

	Type		fem
	  Anim		fem\Staff_eye_of_storm 1 77
	TEnd

	Type		huge
	  Anim		huge\Staff_eye_of_storm 1 77
	TEnd

MEnd

Move Eye_of_Storm_post
	Requires	COMBAT 2hlarge
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        2hlargeReady
	#Flags		ReqInputs
	Scale		0.7

	Type		male
	  Anim          male\Staff_eye_of_storm 78 90
	TEnd

	Type		fem
	  Anim          fem\Staff_eye_of_storm 78 90
	TEnd

	Type		huge
	  Anim          huge\Staff_eye_of_storm 78 90
	TEnd

MEnd



####### Staff Mastery #############################################################

#Move StaffMastery_Draw
#	Requires	COMBAT WEAPON 2hlarge STAFF BOAST SPAWNBACK
#	Priority        66
#	Interrupts	<AttackIRQ>
#	Include sequencers\Members\Combat.inc
#	NextMove	StaffMastery
##	Sets            CANTMOVE
#	Flags		NOSIZESCALE
#
#	Type		male
#	  Anim		male\2hlarge_draw_basic 2 30
#	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
#	TEnd
#
#	Type		fem
#	  Anim		fem\2hlarge_draw_basic 2 30
#	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
#	TEnd
#
#	Type		huge
#	  Anim		huge\2hlarge_draw_basic 2 30
#	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
#	TEnd
#
#
#
#MEnd

Move StaffMastery_BODY
	Requires	COMBAT WEAPON 2hlarge STAFF BOAST FORMOFTHEBODY
	Priority        6
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        StaffMastery_BODY_Post
#	Flags		NoSizeScale
	Interpolate	0

	Type		male
	  Anim		male\Staff_mastery 1 60
	TEnd

	Type		fem
	  Anim		fem\Staff_mastery 1 60
	TEnd

	Type		huge
	  Anim		huge\Staff_mastery 1 60
	TEnd

MEnd


Move StaffMastery_BODY_Post
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        2hlargeReady
#	Flags		NoSizeScale
	Interpolate	0

	Type		male
	  Anim		male\Staff_mastery 61
	TEnd

	Type		fem
	  Anim		fem\Staff_mastery 61
	TEnd

	Type		huge
	  Anim		huge\Staff_mastery 61
	TEnd

MEnd

Move StaffMastery_MIND
	Requires	COMBAT WEAPON 2hlarge STAFF BOAST FORMOFTHEMIND
	Priority        6
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        StaffMastery_MIND_Post
#	Flags		NoSizeScale
	Interpolate	0

	Type		male
	  Anim		male\Staff_mastery 1 60
	TEnd

	Type		fem
	  Anim		fem\Staff_mastery 1 60
	TEnd

	Type		huge
	  Anim		huge\Staff_mastery 1 60
	TEnd

MEnd

Move StaffMastery_MIND_Post
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        2hlargeReady
#	Flags		NoSizeScale
	Interpolate	0

	Type		male
	  Anim		male\Staff_mastery 61
	TEnd

	Type		fem
	  Anim		fem\Staff_mastery 61
	TEnd

	Type		huge
	  Anim		huge\Staff_mastery 61
	TEnd

MEnd

Move StaffMastery_SOUL
	Requires	COMBAT WEAPON 2hlarge STAFF BOAST FORMOFTHESOUL
	Priority        6
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        StaffMastery_SOUL_Post
#	Flags		NoSizeScale
	Interpolate	0

	Type		male
	  Anim		male\Staff_mastery 1 60
	TEnd

	Type		fem
	  Anim		fem\Staff_mastery 1 60
	TEnd

	Type		huge
	  Anim		huge\Staff_mastery 1 60
	TEnd

MEnd

Move StaffMastery_SOUL_Post
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        2hlargeReady
#	Flags		NoSizeScale
	Interpolate	0

	Type		male
	  Anim		male\Staff_mastery 61
	TEnd

	Type		fem
	  Anim		fem\Staff_mastery 61
	TEnd

	Type		huge
	  Anim		huge\Staff_mastery 61
	TEnd

MEnd

#Move StaffMastery_post
#	Requires	COMBAT
#	Include sequencers\Members\Combat_Weapon_TwoHand.inc
#	NextMove        2hlargeReady
##	Flags		NoSizeScale
#	Interpolate	0
#
#	Type		male
#	  Anim		male\Staff_mastery 3 144
#	TEnd
#
#	Type		fem
#	  Anim		fem\Staff_mastery 3 144
#	TEnd
#
#	Type		huge
#	  Anim		huge\Staff_mastery 3 144
#	TEnd
#
#MEnd

#Move FormOfBody_post
#	Requires	COMBAT
#	Include sequencers\Members\Combat_Weapon_TwoHand.inc
#	NextMove        2hlargeReady
##	Flags		ReqInputs
#	Scale		0.7
#
#	Type		male
#	  Anim          male\Staff_mastery 51 144
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#	Type		fem
#	  Anim          fem\Staff_mastery 51 144
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#	Type		huge
#	  Anim          huge\Staff_mastery 51 146
#	Playfx	"fx\WEAPONS\Custom_Staff\Male_teststaff_01.fx" 0
#	TEnd
#
#MEnd

## TAUNT ###########################################################

Move Staff_Taunt_Draw
	Requires	COMBAT WEAPON 2hlarge STAFF TAUNT SPAWNBACK
	Priority        66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Staff_Taunt
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		fem
	  Anim		fem\2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		huge
	  Anim		huge\2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

MEnd

Move Staff_Taunt
	Requires	COMBAT WEAPON 2hlarge STAFF TAUNT
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Staff_Taunt_post
	Sets            CANTMOVE
	Flags		NOSIZESCALE pitchtotarget


	Type		male
	  Anim		male\staff_taunt 1 75
	  PitchAngle    0.3
	TEnd

	Type		fem
	  Anim		fem\staff_taunt 1 75
	  PitchAngle    0.3
	TEnd

	Type		huge
	  Anim		huge\staff_taunt 1 75
	  PitchAngle    0.3
	TEnd

MEnd

Move Staff_Taunt_post
	Requires	COMBAT 2hlarge
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        2hlargeReady
	Flags		ReqInputs
	Scale		0.7

	Type		male
	  Anim          male\staff_taunt 75 108
	TEnd

	Type		fem
	  Anim          fem\staff_taunt 75 108
	TEnd

	Type		huge
	  Anim          huge\staff_taunt 75 108
	TEnd
MEnd

####### Serpent's Reach #############################################################
Move Serpent_Reach_Draw
	Requires	COMBAT WEAPON 2hlarge STAFF LAUNCH SPAWNBACK
	Priority        66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Serpent_Reach
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		fem
	  Anim		fem\2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		huge
	  Anim		huge\2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd
MEnd

Move Serpent_Reach
	Requires	COMBAT WEAPON 2hlarge STAFF LAUNCH
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Serpent_Reach_post
	Flags		NoSizeScale pitchtotarget
	Sets            CANTMOVE
	Interpolate	0

	Type		male
	  Anim		male\Staff_serpent_reach 1 53
	  PitchAngle    -0.9
	  PitchStart	27
	TEnd

	Type		fem
	  Anim		fem\Staff_serpent_reach 1 53
	  PitchAngle    0.3
	TEnd

	Type		huge
	  Anim		huge\Staff_serpent_reach 1 53
	  PitchAngle    0.3
	TEnd

MEnd

Move Serpent_Reach_post
	Requires	COMBAT 2hlarge
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        2hlargeReady
	Flags		ReqInputs
	Scale		0.7

	Type		male
	  Anim          male\Staff_serpent_reach 54 65
	TEnd

	Type		fem
	  Anim          fem\Staff_serpent_reach 54 65
	TEnd

	Type		huge
	  Anim          huge\Staff_serpent_reach 54 65
	TEnd

MEnd

####### Innocuous Strike #############################################################

Move Innocuous_strike_Draw
	Requires	COMBAT WEAPON 2hlarge STAFF CONE SPAWNBACK
	Priority        66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Innocuous_strike
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		fem
	  Anim		fem\2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		huge
	  Anim		huge\2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd
MEnd

Move Innocuous_strike
	Requires	COMBAT WEAPON 2hlarge STAFF CONE
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Innocuous_strike_post
	Flags		NoSizeScale
	Sets            CANTMOVE
	Interpolate	0

	Type		male
	  Anim		male\staff_innocuous_strike 1 65
	TEnd

	Type		fem
	  Anim		fem\staff_innocuous_strike 1 65
	TEnd

	Type		huge
	  Anim		huge\staff_innocuous_strike 1 65
	TEnd

MEnd

Move Innocuous_strike_post
	Requires	COMBAT 2hlarge
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        2hlargeReady
	Flags		ReqInputs
	Scale		0.7

	Type		male
	  Anim          male\staff_innocuous_strike 66 102
	TEnd

	Type		fem
	  Anim          fem\staff_innocuous_strike 66 102
	TEnd

	Type		huge
	  Anim          huge\staff_innocuous_strike 66 71
	TEnd

MEnd

####### Sky Splitter #############################################################

Move Sky_Splitter_Draw
	Requires	COMBAT WEAPON 2hlarge STAFF SMASH SPAWNBACK
	Priority        66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Sky_Splitter
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		fem
	  Anim		fem\2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		huge
	  Anim		huge\2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd
MEnd

Move Sky_Splitter
	Requires	COMBAT WEAPON 2hlarge STAFF SMASH
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Sky_Splitter_post
	Flags		NoSizeScale
	Sets            CANTMOVE
	Interpolate	0

	Type		male
	  Anim		male\Staff_sky_splitter 1 85
	TEnd

	Type		fem
	  Anim		fem\Staff_sky_splitter 1 85
	TEnd

	Type		huge
	  Anim		huge\Staff_sky_splitter 1 85
	TEnd

MEnd

Move Sky_Splitter_post
	Requires	COMBAT 2hlarge
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        2hlargeReady
	Flags		ReqInputs

	Type		male
	  Anim          male\Staff_sky_splitter 85 128
	TEnd

	Type		fem
	  Anim          fem\Staff_sky_splitter 85 93
	TEnd

	Type		huge
	  Anim          huge\Staff_sky_splitter 85 93
	TEnd

MEnd

## Staff Assassins Strike #############################################

Move Staff_Assassins_Draw
	Requires	COMBAT WEAPON 2HLARGE STAFF WINDUPLONGSTAFF SPAWNBACK
	Priority        66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ> <IRQEmotesDualhand>
	NextMove	Staff_Assassins_Windup
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		fem
	  Anim		fem\2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		huge
	  Anim		huge\2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

MEnd

Move Staff_Assassins_Windup
	Requires	COMBAT WINDUPLONGSTAFF
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <MoveIRQ> <IRQEmotesDualhand>
	Flags		NoSizeScale PitchToTarget
	NextMove	Staff_Assassins_Windup_Cycle

	Type		male
	  Anim		male\staff_assassins_strike 1 29
	TEnd
MEnd

Move Staff_Assassins_Windup_Cycle
	Requires	COMBAT WINDUPLONGSTAFF
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <MoveIRQ> <IRQEmotesDualhand>
	Flags		Cycle NoSizeScale PitchToTarget
	NextMove	Staff_Assassins_Strike

	Type		male
	  Anim		male\staff_assassins_strike 30 54
	TEnd
MEnd

#############################################################

Move Staff_Assassins_Quick_Draw
	Requires	COMBAT WEAPON 2hlarge STAFF CHARGE SPAWNBACK
	Priority        66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Staff_Assassins_Strike
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		fem
	  Anim		fem\2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		huge
	  Anim		huge\2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

MEnd

Move Staff_Assassins_Strike
	Requires	COMBAT WEAPON 2hlarge STAFF CHARGE
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Staff_Assassins_Strike_Post
	Sets		CANTMOVE
	Flags		NoSizeScale
	#Interpolate	0

	Type		male
	  Anim		male\staff_assassins_strike 50 80
	  PitchAngle    0.3
	TEnd
MEnd

Move Staff_Assassins_Strike_Post
	Requires	COMBAT 2hlarge
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove	2hlargeReady
	Flags		ReqInputs

	Type		male
	  Anim		male\staff_assassins_strike 81
	TEnd
MEnd

### ASSASSISN'S STRIKE ###########################################################
#
#Move DualWield_AssassinsStrike_Draw
#	Requires	COMBAT WEAPON DUAL WINDUPDUALBLADEBACKSTAB SPAWNBACK
#	Priority        55
#	Interrupts	<AttackIRQ>
#	Member		<DeathIRQ> <HitIRQ> <MoveIRQ> <IRQEmotesDualhand>
#	NextMove	DualWield_AssassinsStrike_Windup
#	##Sets            CANTMOVE FLAG MUST NOT BE USED
#	Flags		NOSIZESCALE
#	Scale		1.25
#
#	Type		male
#	  Anim		male\DualBlade_Draw 5 31
#	TEnd
#MEnd
#
#Move DualWield_AssassinsStrike_Windup
#	Requires	COMBAT WEAPON DUAL WINDUPDUALBLADEBACKSTAB
#	Priority        4
#	Member		<DeathIRQ> <HitIRQ> <MoveIRQ> <IRQEmotesDualhand>
#	NextMove        DualWield_AssassinsStrike
#	##Sets            CANTMOVE FLAG MUST NOT BE USED
#	Flags		NOSIZESCALE
#	Scale		1.6
#
#	Type		male
#	  Anim          male\DualBlade_Attack_AssassinsStrike 2 60
#	TEnd
#MEnd
#
#Move DualWield_AssassinsStrike_Windup2
#	Requires	COMBAT WEAPON DUAL WINDUPDUALBLADEBACKSTAB
#	Priority        4
#	Interrupts	<AttackIRQ>
#	Member		<DeathIRQ> <HitIRQ> <MoveIRQ> <IRQEmotesDualhand>
#	NextMove        DualWield_AssassinsStrike
#	##Sets            CANTMOVE FLAG MUST NOT BE USED
#	Flags		NOSIZESCALE
#
#	Type		male
#	  Anim          male\DualBlade_Attack_AssassinsStrike 2 60
#	TEnd
#MEnd
#
##############################################################
#
#Move DualWield_AssassinsStrike
#	Requires	COMBAT WEAPON DUAL ATTACK DUALBLADEBACKSTAB
#	Priority        5
#	Interrupts	<AttackIRQ> DualWield_AssassinsStrike_Draw DualWield_AssassinsStrike_Windup DualWield_AssassinsStrike_Windup2
#	Member		<DeathIRQ>
#	NextMove        DualWield_AssassinsStrike_Post
#	Sets            CANTMOVE
#	Flags		NOSIZESCALE
#
#	Type		male
#	  Anim          male\DualBlade_Attack_AssassinsStrike 61 91
#	TEnd
#MEnd
#
#Move DualWield_AssassinsStrike_Post
#	Requires	COMBAT
#	Include sequencers\Members\Combat_DualWeapon.inc
#	NextMove        DualWield_Ready
#	Flags		NOSIZESCALE ReqInputs
#
#	Type		male
#	  Anim          male\DualBlade_Attack_AssassinsStrike 92
#	TEnd
#MEnd
################################
###### Two Handed Boostup  ######
################################

Move Staff_2hlarge_Boostup_Draw
	Requires	COMBAT WEAPON 2hlarge STAFF WEPBOOSTUP HIGH SPAWNBACK
	Priority        66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        2hlarge_Boostup
	Sets            CANTMOVE
	Flags		NOSIZESCALE FixArmReg

	Type		male
	  Anim		male\2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		fem
	  Anim		fem\2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		huge
	  Anim		huge\2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd
MEnd

##################
##### Staff Placate ####
##################

Move Staff_Placate_Draw
	Requires	COMBAT WEAPON 2hlarge STAFF PLACATE SPAWNBACK
	Priority        66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Staff_Placate
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		fem
	  Anim		fem\2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd

	Type		huge
	  Anim		huge\2hlarge_draw_basic 2 30
	  PlayFX	CustomizeablePowers\StaffFighting\StaffDraw.fx
	TEnd
MEnd

Move Staff_Placate
	Requires	COMBAT WEAPON 2hlarge STAFF PLACATE
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Staff_Placate2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\Staff_placate 1 40
	TEnd
MEnd

Move Staff_Placate2
	Requires	COMBAT 2hlarge
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        2hlargeReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\Staff_placate 40
	TEnd
MEnd





