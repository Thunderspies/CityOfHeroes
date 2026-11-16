#############################################################
## Weapons_Hammer.inc
#############################################################

## DRAW EARTH HAMMER ###########################################################

Move Draw_EarthHammer
	Requires	COMBAT WEAPON CLUB SPAWNSIDE
	Priority        54
	Interrupts	<AttackIRQ> <ReactIRQ>
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Draw_EarthHammer_post
	Sets            CANTMOVE
	Flags		NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\ROChammer_pre 80 159
	TEnd

	Type		huge
	  Anim          huge\ROChammer_pre 80 159
	TEnd

	Type		fem
	  Anim          fem\ROChammer_pre 80 159
	TEnd
MEnd

Move Draw_EarthHammer_post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        HammerReady
	Scale           0.50
	Flags		FIXARMREG

	Type		male
	  Anim          male\2hclub_pre 8 34
	TEnd

	Type		huge
	  Anim          huge\2hclub_pre 8 34
	TEnd

	Type		fem
	  Anim          fem\2hclub_pre 8 34
	TEnd
MEnd

#############################################################

Move HammerReady_15
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        HammerReady
	Flags		FIXARMREG
	Scale           0.50

	Type		male
	  Anim          male\2hclub_cycle 15 31
	TEnd

	Type		huge
	  Anim          huge\2hclub_cycle 15 31
	TEnd

	Type		fem
	  Anim          fem\2hclub_cycle 15 31
	TEnd
MEnd

#############################################################

Move HammerReady
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        postReady_back
	Flags		CYCLE FIXARMREG
	Scale           0.50

	Type		male
	  Anim          male\2hclub_cycle 1 31
	TEnd

	Type		huge
	  Anim          huge\2hclub_cycle 1 31
	TEnd

	Type		fem
	  Anim          fem\2hclub_cycle 1 31
	TEnd
MEnd

#############################################################

Move A_HammerReady
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_Air.inc
	Scale           0.50
	Flags		Cycle FIXARMREG

	Type		male
	  Anim          male\air_2hclub_cycle 1 31
	TEnd

	Type		huge
	  Anim          huge\air_2hclub_cycle 1 31
	TEnd
MEnd

Move A_HammerReady_15
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_Air.inc
	NextMove        A_HammerReady
	Scale           0.50
	Flags		FIXARMREG

	Type		male
	  Anim          male\air_2hclub_cycle 15 31
	TEnd

	Type		huge
	  Anim          huge\air_2hclub_cycle 15 31
	TEnd
MEnd

#########################################################
## SledgeHammer Uppercut
#########################################################

Move Sledge_Upcut_Draw
	Requires	COMBAT WEAPON SLEDGE UPPERCUT SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ> <ReactIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Sledge_Upcut
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\draw_axe 1 30
	TEnd

	Type		huge
	  Anim          huge\draw_axe 1 30
	TEnd

	Type		fem
	  Anim          fem\draw_axe 1 30
	TEnd
MEnd

Move Sledge_Upcut
	Requires	COMBAT WEAPON SLEDGE UPPERCUT
	Priority        4
	Interrupts	<AttackIRQ> <ReactIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Sledge_Upcut_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\2hclub_upcut 1 30
	TEnd

	Type		huge
	  Anim          huge\2hclub_upcut 1 30
	TEnd

	Type		fem
	  Anim          fem\2hclub_upcut 1 30
	TEnd
MEnd

Move Sledge_Upcut_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        HammerReady_15
	Scale           0.50

	Type		male
	  Anim          male\2hclub_upcut 31
	TEnd

	Type		huge
	  Anim          huge\2hclub_upcut 31
	TEnd

	Type		fem
	  Anim          fem\2hclub_upcut 31
	TEnd
MEnd

#############################################################

Move A_Hammer_Upcut_Pre
	Requires	FLY COMBAT WEAPON CLUB UPPERCUT
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Hammer_Upcut
	Sets            CANTMOVE
	Flags		NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\air_2hclub_upcut 1 30
	TEnd

	Type		huge
	  Anim          huge\air_2hclub_upcut 1 30
	TEnd
MEnd

Move A_Hammer_Upcut
	Requires	FLY COMBAT WEAPON
	Priority	13
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Hammer_Upcut_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE FIXARMREG
	Scale           0.50

	Type		male
	  Anim          male\air_2hclub_upcut 31 41
	TEnd

	Type		huge
	  Anim          huge\air_2hclub_upcut 31 41
	TEnd
MEnd

Move A_Hammer_Upcut_Post
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_Air.inc
	NextMove        A_HammerReady_15
	Flags		FIXARMREG
	Scale           0.50

	Type		male
	  Anim          male\air_2hclub_upcut 42
	TEnd

	Type		huge
	  Anim          huge\air_2hclub_upcut 42
	TEnd
MEnd

#########################################################
## SledgeHammer Smash
#########################################################

Move Sledge_Smash_Draw
	Requires	COMBAT WEAPON SLEDGE SMASH SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Sledge_Smash
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\draw_axe 1 30
	TEnd

	Type		huge
	  Anim          huge\draw_axe 1 30
	TEnd

	Type		fem
	  Anim          fem\draw_axe 1 30
	TEnd
MEnd

Move Sledge_Smash
	Requires	COMBAT WEAPON SLEDGE SMASH
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Sledge_Smash_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\2hclub_slam 6 44
	  PlayFX		NPCFX\NPC_WhooshBig_SFX.fx 15
	TEnd

	Type		huge
	  Anim          huge\2hclub_slam 6 44
	  PlayFX		NPCFX\NPC_WhooshBig_SFX.fx 15
	TEnd

	Type		fem
	  Anim          fem\2hclub_slam 6 44
	  PlayFX		NPCFX\NPC_WhooshBig_SFX.fx 15
	TEnd
MEnd

Move Sledge_Smash_Post
	Requires	COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        HammerReady_15
	Scale           0.50

	Type		male
	  Anim          male\2hclub_slam 45
	TEnd

	Type		huge
	  Anim          huge\2hclub_slam 45
	TEnd

	Type		fem
	  Anim          fem\2hclub_slam 45
	TEnd
MEnd

#############################################################

Move A_Sledge_Smash_Draw
	Requires	FLY COMBAT WEAPON SLEDGE SMASH SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Sledge_Smash
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_draw_axe 1 30
	TEnd

	Type		huge
	  Anim          huge\air_draw_axe 1 30
	TEnd

	Type		fem
	  Anim          fem\air_draw_axe 1 30
	TEnd
MEnd

Move A_Sledge_Smash
	Requires	FLY COMBAT WEAPON SLEDGE SMASH
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Sledge_Smash_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_2hclub_smash 6 44
	TEnd

	Type		huge
	  Anim          huge\air_2hclub_smash 6 44
	TEnd

#	Type		fem
#	  Anim          fem\air_2hclub_smash 6 44
#	TEnd
MEnd

Move A_Sledge_Smash_Post
	Requires	FLY COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon_Air.inc
	NextMove        A_HammerReady_15
	Scale           0.50

	Type		male
	  Anim          male\air_2hclub_smash 45
	TEnd

	Type		huge
	  Anim          huge\air_2hclub_smash 45
	TEnd

#	Type		fem
#	  Anim          fem\air_2hclub_smash 45
#	TEnd
MEnd

#############################################################
## HAMMER UPPERCUT
#############################################################

Move Hammer_Uppercut_Draw
	Requires	COMBAT WEAPON CLUB UPPERCUT SPAWNSIDE
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Hammer_Uppercut_Pre
	Sets            CANTMOVE
	Flags		NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\ROChammer_pre 80 155
	TEnd

	Type		huge
	  Anim          huge\ROChammer_pre 80 155
	TEnd

	Type		fem
	  Anim          fem\ROChammer_pre 80 155
	TEnd
MEnd

Move Hammer_Uppercut_Pre
	Requires	COMBAT WEAPON CLUB UPPERCUT
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Hammer_Uppercut
	Sets            CANTMOVE
	Flags		NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\2hclub_upcut 1 30
	TEnd

	Type		huge
	  Anim          huge\2hclub_upcut 1 30
	TEnd

	Type		fem
	  Anim          fem\2hclub_upcut 1 30
	TEnd
MEnd

Move Hammer_Uppercut
	Requires	COMBAT WEAPON
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Hammer_Uppercut_Post
	Scale           0.50
	Sets            CANTMOVE
	Flags		NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\2hclub_upcut 31 41
	TEnd

	Type		huge
	  Anim          huge\2hclub_upcut 31 41
	TEnd

	Type		fem
	  Anim          fem\2hclub_upcut 31 41
	TEnd
MEnd

Move Hammer_Uppercut_Post
	Requires	COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        HammerReady_15
	Scale           0.50
	Flags		FIXARMREG

	Type		male
	  Anim          male\2hclub_upcut 42
	TEnd

	Type		huge
	  Anim          huge\2hclub_upcut 42
	TEnd

	Type		fem
	  Anim          fem\2hclub_upcut 42
	TEnd
MEnd

#######################################
###### EarthHammer ATTACK #2 (smash)  ######
#######################################

Move HammerSmash_Draw
	Requires	COMBAT WEAPON CLUB SMASH SPAWNSIDE
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        HammerSmash_Pre
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\ROChammer_pre 80 155
	TEnd

	Type		huge
	  Anim          huge\ROChammer_pre 80 155
	TEnd

	Type		fem
	  Anim          fem\ROChammer_pre 80 155
	TEnd
MEnd

Move HammerSmash_Pre
	Requires	COMBAT WEAPON CLUB SMASH
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        HammerSmash
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\2hclub_slam 6 44
	TEnd

	Type		huge
	  Anim          huge\2hclub_slam 6 44
	TEnd

	Type		fem
	  Anim          fem\2hclub_slam 6 44
	TEnd
MEnd

Move HammerSmash
	Requires	COMBAT WEAPON
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        HammerSmash_post
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale           0.50

	Type		male
	  Anim          male\2hclub_slam 45 50
	TEnd

	Type		huge
	  Anim          huge\2hclub_slam 45 50
	TEnd

	Type		fem
	  Anim          fem\2hclub_slam 45 50
	TEnd
MEnd

Move HammerSmash_post
	Requires	COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        HammerReady_15
	Scale           0.50

	Type		male
	  Anim          male\2hclub_slam 51
	TEnd

	Type		huge
	  Anim          huge\2hclub_slam 51
	TEnd

	Type		fem
	  Anim          fem\2hclub_slam 51
	TEnd
MEnd

#############################################################

Move A_HammerSmash_Draw
	Requires	FLY COMBAT WEAPON CLUB SMASH SPAWNSIDE
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_HammerSmash_Pre
	Sets            CANTMOVE
	Flags		NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\air_draw_axe 1 30
	TEnd

	Type		huge
	  Anim          huge\air_draw_axe 1 30
	TEnd

	Type		fem
	  Anim          fem\air_draw_axe 1 30
	TEnd
MEnd

Move A_HammerSmash_Pre
	Requires	FLY COMBAT WEAPON CLUB SMASH
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_HammerSmash
	Sets            CANTMOVE
	Flags		NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\air_2hclub_smash 6 44
	TEnd

	Type		huge
	  Anim          huge\air_2hclub_smash 6 44
	TEnd

#	Type		fem
#	  Anim          fem\air_2hclub_smash 6 44
#	TEnd
MEnd

Move A_HammerSmash
	Requires	FLY COMBAT WEAPON
	Priority	13
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_HammerSmash_post
	Scale           0.50
	Sets            CANTMOVE
	Flags		NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\air_2hclub_smash 45 50
	TEnd

	Type		huge
	  Anim          huge\air_2hclub_smash 45 50
	TEnd

#	Type		fem
#	  Anim          fem\air_2hclub_smash 45 50
#	TEnd
MEnd

Move A_HammerSmash_post
	Requires	FLY COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon_Air.inc
	NextMove        A_HammerReady_15
	Scale           0.50
	Flags		FIXARMREG

	Type		male
	  Anim          male\air_2hclub_smash 51
	TEnd

	Type		huge
	  Anim          huge\air_2hclub_smash 51
	TEnd

#	Type		fem
#	  Anim          fem\air_2hclub_smash 51
#	TEnd
MEnd