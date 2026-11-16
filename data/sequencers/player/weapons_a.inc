####################################################################
## WEAPON_A.INC - Sequencers related to Weapon animations in flight
####################################################################
##
##  Fly combat moves get +10 priority to resolve conficts with hover and combat hover modes

## DRAW WEAPON ###########################################################

Move A_Weapon_Draw
	Requires	FLY COMBAT WEAPON SPAWNBACK
	Priority	64
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon_Air.inc
	NextMove        A_Weapon_Draw_Post
	Flags		NoSizeScale

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

Move A_Weapon_Draw_Post
	Requires	FLY COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon_Air.inc
	NextMove        Lazy_W_flycycle_HalfBlend
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_draw_axe 30
	TEnd

	Type		huge
	  Anim          huge\air_draw_axe 30
	TEnd

	Type		fem
	  Anim          fem\air_draw_axe 30
	TEnd
MEnd

## CREATE ELEMENTAL WEAPON ###########################################################

Move airCreatewep
	Requires	FLY COMBAT WEPSPAWN
	Priority	63
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	airCreateweppost
	Scale           1.5
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_createwep 1 75
	TEnd

	Type		huge
	  Anim          huge\air_createwep 1 75
	TEnd

	Type		fem
	  Anim          fem\air_createwep 1 75
	TEnd
MEnd

Move airCreateweppost
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_Air.inc
	NextMove	W_flyready
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_createwep 75
	TEnd

	Type		huge
	  Anim          huge\air_createwep 75
	TEnd

	Type		fem
	  Anim          fem\air_createwep 75
	TEnd
MEnd

## WEAPON BOOSTUP ###########################################################

Move A_W_Boostup
	Requires	COMBAT WEAPON FLY BOOSTUP
	Priority	14
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon_Air.inc
	NextMove        A_W_Boostuppost
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_boostup 1 2
	TEnd

	Type		huge
	  Anim          huge\air_boostup 1 2
	TEnd

	Type		fem
	  Anim          fem\air_boostup 1 2
	TEnd
MEnd

Move A_W_Boostuppost
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_Air.inc
	NextMove        W_flycycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_boostup 3
	TEnd

	Type		huge
	  Anim          huge\air_boostup 3
	TEnd

	Type		fem
	  Anim          fem\air_boostup 3
	TEnd
MEnd

#############################################
###### Air Mace Shoot (works with player mace) ######
############################################

Move A_MaceShoot_Draw
	Requires	FLY COMBAT WEAPON MACE SHOOT SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_MaceShoot
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_draw_axe 1 30
	TEnd
MEnd

Move A_MaceShoot
	Requires	FLY COMBAT WEAPON MACE SHOOT
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_MaceShoot_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\air_wep_mace_projectile 1 45
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\air_wep_mace_projectile 1 45
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\air_wep_mace_projectile 1 45
	  PitchAngle	0.5
	TEnd
MEnd

Move A_MaceShoot_Post
	Requires	FLY COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon_Air.inc
	NextMove        A_MaceShoot_PostCycle
	Flags		NOSIZESCALE ReqInputs PitchToTarget

	Type		male
	  Anim          male\air_wep_mace_projectile 45 109
	  PitchEnd	65
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\air_wep_mace_projectile 45 109
	  PitchEnd	65
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\air_wep_mace_projectile 45 109
	  PitchEnd	65
	  PitchAngle	0.5
	TEnd
MEnd

Move A_MaceShoot_PostCycle
	Requires	FLY COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon_Air.inc
	Flags		Cycle

	Type		male
	  Anim          male\air_wep_mace_projectile 112
	TEnd

	Type		huge
	  Anim          huge\air_wep_mace_projectile 112
	TEnd

	Type		fem
	  Anim          fem\air_wep_mace_projectile 112
	TEnd
MEnd


#############################################
###### Air Mace Summon (point mace in air) ######
############################################

Move A_Mace_Summon_Draw
	Requires	FLY COMBAT WEAPON MACESUMMON SPAWNBACK
	Priority	65
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Mace_Summon
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_draw_axe 1 30
	TEnd
MEnd

Move A_Mace_Summon
	Requires	FLY COMBAT WEAPON MACESUMMON
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Mace_Summon_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_wep_mace_summon 1 60
	TEnd
MEnd

Move A_Mace_Summon_Post
	Requires	FLY COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon_Air.inc
	NextMove        Lazy_WepReady
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_wep_mace_summon 60
	TEnd
MEnd

################################
###### Air Wep (Mace) Boostup  ######
################################

Move A_Wep_Boostup_Draw
	Requires	FLY COMBAT WEAPON WEPBOOSTUP SPAWNBACK
	Priority	65
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Wep_Boostup
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_draw_axe 1 30
	TEnd
MEnd

Move A_Wep_Boostup
	Requires	FLY COMBAT WEAPON WEPBOOSTUP
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Wep_Boostup_Post
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_boostup 1 2
	TEnd
MEnd

Move A_Wep_Boostup_Post
	Requires	FLY COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon_Air.inc
	NextMove        A_Wep_Boostup_PostCycle
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_boostup 2 60
	TEnd
MEnd

Move A_Wep_Boostup_PostCycle
	Requires	FLY COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon_Air.inc
	Flags		Cycle
	NextMove        postReady_back
	Scale		0.5
	Interpolate	15

	Type		male
	  Anim          male\air_wep_cycle 1 31
	TEnd
MEnd

#########################
##### Air MaceSniper Shoot  ######  ##(NOTE: make sure all shotgun readys have <ReactIRQ>)
##########################

Move A_Mace_Sniper_Windup_Draw
	Requires	FLY COMBAT WEAPON MACE MACESNIPERWINDUP SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ>
	NextMove        A_Mace_Sniper_Windup
	Flags		NOSIZESCALE FixArmReg

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

Move A_Mace_Sniper_Windup
	Requires	FLY COMBAT WEAPON MACESNIPERWINDUP
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ>
	NextMove        A_Mace_Sniper_WindupCycle
	Flags		NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\air_wep_mace_sniper 1 73
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\air_wep_mace_sniper 1 73
	  PitchAngle	0.5
	 TEnd

	Type		fem
	  Anim          fem\air_wep_mace_sniper 1 73
	  PitchAngle	0.5
	TEnd
MEnd

Move A_Mace_Sniper_WindupCycle
	Requires	FLY COMBAT WEAPON MACESNIPERWINDUP
	Priority	14
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ>
	NextMove	A_Mace_Sniper_Shoot
	Flags		Cycle PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\air_wep_mace_sniper 74 108
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\air_wep_mace_sniper 74 108
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\air_wep_mace_sniper 74 108
	  PitchAngle	0.5
	TEnd
MEnd

#############################################################

Move A_Mace_Sniper_Shoot
	Requires	FLY COMBAT WEAPON MACE MACESNIPER
	Priority	15
	Interrupts	<AttackIRQ> A_Mace_Sniper_Windup A_Mace_Sniper_WindupCycle
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Mace_Sniper_Shoot_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\air_wep_mace_sniper 109 157
	TEnd

	Type		huge
	  Anim          huge\air_wep_mace_sniper 109 157
	TEnd

	Type		fem
	  Anim          fem\air_wep_mace_sniper 109 157
	TEnd
MEnd

Move A_Mace_Sniper_Shoot_Post
	Requires	FLY COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon_Air.inc
	NextMove        A_Mace_Sniper_Shoot_PostCycle
	Flags		FixArmReg NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_wep_mace_sniper 157 173
	TEnd

	Type		huge
	  Anim          huge\air_wep_mace_sniper 157 173
	TEnd

	Type		fem
	  Anim          fem\air_wep_mace_sniper 157 173
	TEnd
MEnd

Move A_Mace_Sniper_Shoot_PostCycle
	Requires	FLY COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon_Air.inc
	Flags		Cycle FixArmReg NOSIZESCALE
	NextMove        postReady_back

	Type		male
	  Anim          male\air_wep_mace_sniper 174 233
	TEnd

	Type		huge
	  Anim          huge\air_wep_mace_sniper 174 233
	TEnd

	Type		fem
	  Anim          fem\air_wep_mace_sniper 174 233
	TEnd
MEnd

#########################
###### Air Left Wristshoot  ######
########################

Move A_LeftWristShoot
	Requires	FLY COMBAT CAST HAND CROSSBOW LOW
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_LeftWristShoot_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\air_comb_wristshoot_L 1 43
	  PitchStart	12
	  PitchAngle	-0.5
	TEnd
MEnd

Move A_LeftWristShoot_Post
	Requires	FLY COMBAT
	Priority	12
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove	Lazy_C_flycycle
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_comb_wristshoot_L 46
	TEnd
MEnd

## MAGIC STAFF - DRAW ###########################################################

Move A_Draw_MagicStaff
	Requires	FLY COMBAT WEAPON CLUB SPAWNSIDE STAFF
	Priority	66
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        W_flycycle
	Flags		NOSIZESCALE

	Type		male
	  Anim          fem\air_firesword_draw 1
	TEnd
MEnd

## MAGIC STAFF - CAST ###########################################################

Move A_MagicStaff_Cast_Draw
	Requires	FLY COMBAT WEAPON CLUB STAFF CAST SPAWNSIDE
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_MagicStaff_Cast
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          fem\air_firesword_draw 1 25
	TEnd
MEnd

Move A_MagicStaff_Cast
	Requires	FLY COMBAT WEAPON CLUB STAFF CAST
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_MagicStaff_Cast_Post
	Interpolate     8
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          fem\air_staff_cast_med 1 54
	  PitchStart	10
	  PitchEnd	35
	TEnd
MEnd

Move A_MagicStaff_Cast_Post
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        W_flycycle_half
	Flags		NOSIZESCALE

	Type		male
	  Anim          fem\air_staff_cast_med 55
	TEnd
MEnd

###################
###### Air knife attack  ######
###################

Move A_Draw_Knife
	Requires	FLY COMBAT WEAPON KNIFE SPAWNSIDE
	Priority	65
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        A_Draw_Knife_post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_draw_saber 1 21
	TEnd

	Type		huge
	  Anim          huge\air_draw_saber 1 21
	TEnd

	Type		fem
	  Anim          fem\air_draw_saber 1 21
	TEnd
MEnd

Move A_Draw_Knife_post
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        W_flycycle
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_draw_saber 22
	TEnd

	Type		huge
	  Anim          huge\air_draw_saber 22
	TEnd

	Type		fem
	  Anim          fem\air_draw_saber 22
	TEnd
MEnd

#############################################################

Move A_Knife_Atack_Draw
	Requires	FLY COMBAT WEAPON ATTACK KNIFE SPAWNSIDE
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Knife_Atack
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_draw_saber 1 21
	TEnd

	Type		huge
	  Anim          huge\air_draw_saber 1 21
	TEnd

	Type		fem
	  Anim          fem\air_draw_saber 1 21
	TEnd
MEnd

Move A_Knife_Atack
	Requires	FLY COMBAT WEAPON ATTACK KNIFE
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Knife_Atack_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_wm_slash 1 16
	TEnd

	Type		huge
	  Anim          huge\air_wm_slash 1 16
	TEnd

	Type		fem
	  Anim          fem\air_wm_slash 1 16
	TEnd
MEnd

Move A_Knife_Atack_Post
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        W_flycycle_half
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_wm_slash 17
	TEnd

	Type		huge
	  Anim          huge\air_wm_slash 17
	TEnd

	Type		fem
	  Anim          fem\air_wm_slash 17
	TEnd
MEnd

########################
###### Air knife Stab  ######
#######################

Move A_Knife_Stab_Draw
	Requires	FLY COMBAT WEAPON STAB KNIFE SPAWNSIDE
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Knife_Stab
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_draw_saber 1 21
	TEnd
MEnd

Move A_Knife_Stab
	Requires	FLY COMBAT WEAPON STAB KNIFE
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Knife_Stab_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_wm_stab 1 16
	TEnd
MEnd

Move A_Knife_Stab_Post
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        W_flycycle_half
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_wm_Stab 17
	TEnd
MEnd

###################
###### Air Knife Parry  ######
###################

Move A_Knife_Parry_Draw
	Requires	FLY COMBAT WEAPON KNIFE PARRY SPAWNSIDE
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Knife_Parry
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_draw_saber 1 21
	TEnd

	Type		huge
	  Anim          huge\air_draw_saber 1 21
	TEnd

	Type		fem
	  Anim          fem\air_draw_saber 1 21
	TEnd
MEnd

Move A_Knife_Parry
	Requires	FLY COMBAT WEAPON KNIFE PARRY
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Knife_Parry_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_sword_parry 1 54
	TEnd

	Type		huge
	  Anim          huge\air_sword_parry 1 54
	TEnd

	Type		fem
	  Anim          fem\air_sword_parry 1 54
	TEnd
MEnd

Move A_Knife_Parry_Post
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        W_flycycle_half
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_sword_parry 55
	TEnd

	Type		huge
	  Anim          huge\air_sword_parry 55
	TEnd

	Type		fem
	  Anim          fem\air_sword_parry 55
	TEnd
MEnd

## SWORD ATTACK ###########################################################

Move A_Sword_Slash_Draw
	Requires	FLY COMBAT WEAPON AXE SLASH SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Sword_Slash
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

Move A_Sword_Slash
	Requires	FLY COMBAT WEAPON AXE SLASH
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Sword_Slash_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_sword_1st_slash 1 54
	TEnd

	Type		huge
	  Anim          huge\air_sword_1st_slash 1 54
	TEnd

	Type		fem
	  Anim          fem\air_sword_1st_slash 1 54
	TEnd
MEnd

Move A_Sword_Slash_Post
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        W_flycycle_half
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_sword_1st_slash 55
	TEnd

	Type		huge
	  Anim          huge\air_sword_1st_slash 55
	TEnd

	Type		fem
	  Anim          fem\air_sword_1st_slash 55
	TEnd
MEnd

#################################
###### Air sword Slash (Horizontal) ######
#################################

Move A_Sword_MidSlash_Draw
	Requires	FLY COMBAT WEAPON AXE SLASH MID SPAWNBACK
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Sword_MidSlash
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

Move A_Sword_MidSlash
	Requires	FLY COMBAT WEAPON AXE SLASH MID
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Sword_MidSlash_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_wm_slash 1 41
	TEnd

	Type		huge
	  Anim          huge\air_wm_slash 1 41
	TEnd

	Type		fem
	  Anim          fem\air_wm_slash 1 41
	TEnd
MEnd

Move A_Sword_MidSlash_Post
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        W_flycycle_half
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_wm_slash 42
	TEnd

	Type		huge
	  Anim          huge\air_wm_slash 42
	TEnd

	Type		fem
	  Anim          fem\air_wm_slash 42
	TEnd
MEnd

## AIR SWORD PARRY ###########################################################

Move A_Parry_Draw
	Requires	FLY COMBAT WEAPON AXE PARRY SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Parry
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

Move A_Parry
	Requires	FLY COMBAT WEAPON AXE PARRY
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Parry_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_sword_parry 1 40
	TEnd

	Type		huge
	  Anim          huge\air_sword_parry 1 40
	TEnd

	Type		fem
	  Anim          fem\air_sword_parry 1 40
	TEnd
MEnd

Move A_Parry_Post
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        W_flycycle_half
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_sword_parry 41
	TEnd

	Type		huge
	  Anim          huge\air_sword_parry 41
	TEnd

	Type		fem
	  Anim          fem\air_sword_parry 41
	TEnd
MEnd


###################
###### Air sword Twirl  ######
###################

Move A_Twirl_Draw
	Requires	FLY COMBAT WEAPON AXE SPIN SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Twirl
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

Move A_Twirl
	Requires	FLY COMBAT WEAPON AXE SPIN
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Twirl_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_firesword_jumpspin 1 80
	TEnd

	Type		huge
	  Anim          huge\air_firesword_jumpspin 1 80
	TEnd

	Type		fem
	  Anim          fem\air_firesword_jumpspin 1 80
	TEnd
MEnd

Move A_Twirl_Post
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        W_flycycle_half
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_firesword_jumpspin 81
	TEnd

	Type		huge
	  Anim          huge\air_firesword_jumpspin 81
	TEnd

	Type		fem
	  Anim          fem\air_firesword_jumpspin 81
	TEnd
MEnd

#############################################################

Move A_Axe_Chop_Draw
	Requires	FLY COMBAT WEAPON SPAWNBACK CHOP AXE
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Axe_Chop
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_draw_axe 1 30
	TEnd
MEnd

Move A_Axe_Chop
	Requires	FLY COMBAT WEAPON CHOP AXE
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Axe_Chop_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_sword_1st_slash 2 38
	TEnd
MEnd

Move A_Axe_Chop_Post
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        W_flycycle_half
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_sword_1st_slash 39
	TEnd
MEnd


###################
###### Air Axe Swoop  ######
###################

Move A_Axe_Swoop_Draw
	Requires	FLY COMBAT WEAPON UPPERCUT AXE SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Axe_Swoop
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

Move A_Axe_Swoop
	Requires	FLY COMBAT WEAPON UPPERCUT AXE
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Axe_Swoop_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_axe_swoop 1 55
	TEnd

	Type		huge
	  Anim          huge\air_axe_swoop 1 55
	TEnd

	Type		fem
	  Anim          fem\air_axe_swoop 1 55
	TEnd
MEnd

Move A_Axe_Swoop_Post
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        W_flycycle_half
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_axe_swoop 56
	TEnd

	Type		huge
	  Anim          huge\air_axe_swoop 56
	TEnd

	Type		fem
	  Anim          fem\air_axe_swoop 56
	TEnd
MEnd

###################
###### Air Axe Reverse Sweep  ######
###################

Move A_Axe_RSweep_Draw
	Requires	FLY COMBAT WEAPON AXE SWIPE SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Axe_RSweep
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

Move A_Axe_RSweep
	Requires	FLY COMBAT WEAPON AXE SWIPE
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Axe_RSweep_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_axe_reverse_sweep 1 60
	TEnd

	Type		huge
	  Anim          huge\air_axe_reverse_sweep 1 60
	TEnd

	Type		fem
	  Anim          fem\air_axe_reverse_sweep 1 60
	TEnd
MEnd

Move A_Axe_RSweep_Post
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        W_flycycle_half
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_axe_reverse_sweep 61
	TEnd

	Type		huge
	  Anim          huge\air_axe_reverse_sweep 61
	TEnd

	Type		fem
	  Anim          fem\air_axe_reverse_sweep 61
	TEnd
MEnd


## MACE ATTACK ###########################################################

Move A_Mace_Slash_Draw
	Requires	FLY COMBAT WEAPON SWORD SLASH SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Mace_Slash
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

Move A_Mace_Slash
	Requires	FLY COMBAT WEAPON SWORD SLASH
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Mace_Slash_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_sword_1st_slash 1 54
	TEnd

	Type		huge
	  Anim          huge\air_sword_1st_slash 1 54
	TEnd

	Type		fem
	  Anim          fem\air_sword_1st_slash 1 54
	TEnd
MEnd

Move A_Mace_Slash_Post
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        W_flycycle_half
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_sword_1st_slash 55
	TEnd

	Type		huge
	  Anim          huge\air_sword_1st_slash 55
	TEnd

	Type		fem
	  Anim          fem\air_sword_1st_slash 55
	TEnd
MEnd

###################
###### Air mace uppercut\smash  ######
###################

Move A_Mace_Upcut_Draw
	Requires	FLY COMBAT WEAPON UPPERCUT MACE SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Mace_Upcut
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

Move A_Mace_Upcut
	Requires	FLY COMBAT WEAPON UPPERCUT MACE
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Mace_Upcut_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_wm_upcut 1 50
	TEnd

	Type		huge
	  Anim          huge\air_wm_upcut 1 50
	TEnd

	Type		fem
	  Anim          fem\air_wm_upcut 1 50
	TEnd
MEnd

Move A_Mace_Upcut_Post
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        W_flycycle_half
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_wm_upcut 51
	TEnd

	Type		huge
	  Anim          huge\air_wm_upcut 51
	TEnd

	Type		fem
	  Anim          fem\air_wm_upcut 51
	TEnd
MEnd

###################
###### Air mace clobber\smash  ######
###################

Move A_Mace_Clobber_Draw
	Requires	FLY COMBAT WEAPON MACE SMASH SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Mace_Clobber
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

Move A_Mace_Clobber
	Requires	FLY COMBAT WEAPON MACE SMASH
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Mace_Clobber_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_mace_bigsmash 1 71
	TEnd

	Type		huge
	  Anim          huge\air_mace_bigsmash 1 71
	TEnd

	Type		fem
	  Anim          fem\air_mace_bigsmash 1 71
	TEnd
MEnd

Move A_Mace_Clobber_Post
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        W_flycycle_half
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_mace_bigsmash 72
	TEnd

	Type		huge
	  Anim          huge\air_mace_bigsmash 72
	TEnd

	Type		fem
	  Anim          fem\air_mace_bigsmash 72
	TEnd
MEnd

###############################
###### Air fire sword attack  ######
###############################

Move A_Draw_firesword
	Requires	FLY COMBAT WEAPON WEPSPAWN2
	Priority	64
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        A_Draw_firesword_post
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_firesword_draw 1 21
	TEnd

	Type		huge
	  Anim          huge\air_firesword_draw 1 21
	TEnd

	Type		fem
	  Anim          fem\air_firesword_draw 1 21
	TEnd
MEnd

Move A_Draw_firesword_post
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        W_flycycle
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_firesword_draw 22
	TEnd

	Type		huge
	  Anim          huge\air_firesword_draw 22
	TEnd

	Type		fem
	  Anim          fem\air_firesword_draw 22
	TEnd
MEnd

#############################################################

Move A_Firesword_DrawNSlash
	Requires	FLY COMBAT WEAPON SWORD ATTACK WEPSPAWN2
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Firesword_DrawNSlash_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_firesword_drawNslash 1 80
	TEnd

	Type		huge
	  Anim          huge\air_firesword_drawNslash 1 80
	TEnd

	Type		fem
	  Anim          fem\air_firesword_drawNslash 1 80
	TEnd
MEnd

Move A_Firesword_DrawNSlash_Post
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        W_flycycle_half
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_firesword_drawNslash 81
	TEnd

	Type		huge
	  Anim          huge\air_firesword_drawNslash 81
	TEnd

	Type		fem
	  Anim          fem\air_firesword_drawNslash 81
	TEnd
MEnd

#############################################################

Move A_Firesword_Slash
	Requires	FLY COMBAT WEAPON SWORD ATTACK
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Firesword_Slash_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_wm_slash 1 30
	TEnd

	Type		huge
	  Anim          huge\air_wm_slash 1 30
	TEnd

	Type		fem
	  Anim          fem\air_wm_slash 1 30
	TEnd
MEnd

Move A_Firesword_Slash_Post
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        W_flycycle_half
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_wm_slash 31
	TEnd

	Type		huge
	  Anim          huge\air_wm_slash 31
	TEnd

	Type		fem
	  Anim          fem\air_wm_slash 31
	TEnd
MEnd


#########################################
###### Air Greater Firesword (old 2Hsword)  ######
#########################################

Move A_Firesword_DrawNUpcut
	Requires	FLY COMBAT WEAPON SWORD 2HAND ATTACK WEPSPAWN2
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Firesword_DrawNUpcut_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_firesword_drawNupcut 1 89
	TEnd

	Type		huge
	  Anim          huge\air_firesword_drawNupcut 1 89
	TEnd

	Type		fem
	  Anim          fem\air_firesword_drawNupcut 1 89
	TEnd
MEnd

Move A_Firesword_DrawNUpcut_Post
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        Wep_half
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_firesword_drawNupcut 90
	TEnd

	Type		huge
	  Anim          huge\air_firesword_drawNupcut 90
	TEnd

	Type		fem
	  Anim          fem\air_firesword_drawNupcut 90
	TEnd
MEnd

#############################################################

Move A_Firesword_Upcut
	Requires	FLY COMBAT WEAPON SWORD 2HAND ATTACK
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Firesword_Upcut_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_wm_upcut 1 25
	TEnd

	Type		huge
	  Anim          huge\air_wm_upcut 1 25
	TEnd

	Type		fem
	  Anim          fem\air_wm_upcut 1 25
	TEnd
MEnd

Move A_Firesword_Upcut_Post
	Requires	FLY COMBAT
	Priority	12
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        Fake_WepFlyCycle_Half
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_wm_upcut 26
	TEnd

	Type		huge
	  Anim          huge\air_wm_upcut 26
	TEnd

	Type		fem
	  Anim          fem\air_wm_upcut 26
	TEnd
MEnd


## DRAW HAMMER ###########################################################

Move A_Draw_SledgeHammer
	Requires	FLY COMBAT WEAPON SLEDGE SPAWNBACK
	Priority	65
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        W_flycycle
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

#############################
###### Air sword jumpspin   ######
############################

Move A_Sword_DrawnNSpin
	Requires	FLY COMBAT WEAPON FIERCE SWORD ATTACK WEPSPAWN2
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Sword_DrawnNSpin_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_firesword_drawNspin 1 124
	TEnd

	Type		huge
	  Anim          huge\air_firesword_drawNspin 1 124
	TEnd

	Type		fem
	  Anim          fem\air_firesword_drawNspin 1 124
	TEnd
MEnd

Move A_Sword_DrawnNSpin_Post
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        W_flycycle_half
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_firesword_drawNspin 125
	TEnd

	Type		huge
	  Anim          huge\air_firesword_drawNspin 125
	TEnd

	Type		fem
	  Anim          fem\air_firesword_drawNspin 125
	TEnd
MEnd

#############################################################

Move A_Sword_JumpSpin
	Requires	FLY COMBAT WEAPON FIERCE SWORD ATTACK
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Sword_JumpSpin_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_firesword_jumpspin 1 40
	TEnd

	Type		huge
	  Anim          huge\air_firesword_jumpspin 1 40
	TEnd

	Type		fem
	  Anim          fem\air_firesword_jumpspin 1 40
	TEnd
MEnd

Move A_Sword_JumpSpin_Post
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        W_flycycle_half
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_firesword_jumpspin 41
	TEnd

	Type		huge
	  Anim          huge\air_firesword_jumpspin 41
	TEnd

	Type		fem
	  Anim          fem\air_firesword_jumpspin 41
	TEnd
MEnd

## DRAW PISTOL ###########################################################

Move A_Draw_Pistol
	Requires	FLY COMBAT WEAPON HANDGUN SPAWNSIDE
	Priority	65
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon_Air.inc
	NextMove        A_Draw_Pistol_post
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\air_draw_pistol 1 29
	TEnd

	Type		huge
	  Anim          huge\air_draw_pistol 1 29
	TEnd

	Type		fem
	  Anim          fem\air_draw_pistol 1 29
	TEnd
MEnd

Move A_Draw_Pistol_post
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_air.inc
	Flags		Cycle NOSIZESCALE

	Type		male
	  Anim          male\air_draw_pistol 29
	TEnd

	Type		huge
	  Anim          huge\air_draw_pistol 29
	TEnd

	Type		fem
	  Anim          fem\air_draw_pistol 29
	TEnd
MEnd

## PISTOL SHOOT ###########################################################

Move A_Pistol_HipShoot_Draw
	Requires	FLY COMBAT WEAPON ATTACK HANDGUN SPAWNSIDE
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Pistol_HipShoot
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_draw_pistol 1 29
	TEnd

	Type		huge
	  Anim          huge\air_draw_pistol 1 29
	TEnd

	Type		fem
	  Anim          fem\air_draw_pistol 1 29
	TEnd
MEnd

Move A_Pistol_HipShoot
	Requires	FLY COMBAT WEAPON ATTACK HANDGUN
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Pistol_HipShoot_Post
	Interpolate     8
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\air_pistol_hipshoot 1 35
	TEnd

	Type		huge
	  Anim          huge\air_pistol_hipshoot 1 35
	TEnd

	Type		fem
	  Anim          fem\air_pistol_hipshoot 1 35
	TEnd
MEnd

Move A_Pistol_HipShoot_Post
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        W_flycycle
	Scale           0.50
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_pistol_hipshoot 36
	TEnd

	Type		huge
	  Anim          huge\air_pistol_hipshoot 36
	TEnd

	Type		fem
	  Anim          fem\air_pistol_hipshoot 36
	TEnd
MEnd

##########################
###### Air gun 3 shot burst  ######
##########################

Move A_Pistol_3Burst_Draw
	Requires	FLY COMBAT WEAPON HANDGUN 3SHOTBURST SPAWNSIDE
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Pistol_3Burst
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_draw_pistol 1 29
	TEnd
MEnd

Move A_Pistol_3Burst
	Requires	FLY COMBAT WEAPON HANDGUN 3SHOTBURST
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Pistol_3Burst_Post
	Interpolate     8
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\air_dual_guns_3_shot 1 51
	TEnd
MEnd

Move A_Pistol_3Burst_Post
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        A_Pistol_3Burst_PostCycle
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\air_dual_guns_3_shot 52
	  PitchEnd	58
	TEnd
MEnd

Move A_Pistol_3Burst_PostCycle
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_air.inc
	Flags		Cycle NOSIZESCALE
	Scale           0.50

	Type		male
	  Anim          male\air_dual_cycle 1 31
	TEnd
MEnd

################################
####### Air Wep Energy Torrent Cast ######
#################################

Move A_Wep_EnergyTorrent_Draw
	Requires	FLY COMBAT WEAPON ENERGYTORRENT SPAWNBACK
	Priority	65
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Wep_EnergyTorrent
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_draw_axe 1 30
	TEnd
MEnd

Move A_Wep_EnergyTorrent
	Requires	FLY COMBAT WEAPON ENERGYTORRENT
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Wep_EnergyTorrent_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_wep_energy_torrent 1 64
	TEnd
MEnd

Move A_Wep_EnergyTorrent_Post
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        Lazy_W_flycycle

	Type		male
	  Anim          male\air_wep_energy_torrent 65
	TEnd
MEnd

#####################
####### Air Wep Stop Cast ######
######################

Move A_Wep_StopCast_Draw
	Requires	FLY COMBAT WEAPON STOPCAST SPAWNBACK
	Priority	65
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Wep_StopCast
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_draw_axe 1 30
	TEnd
MEnd

Move A_Wep_StopCast
	Requires	FLY COMBAT WEAPON STOPCAST
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Wep_StopCast_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_wep_stop 1 60
	TEnd
MEnd

Move A_Wep_StopCast_Post
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        Lazy_W_flycycle

	Type		male
	  Anim          male\air_wep_stop 61
	TEnd
MEnd

############################
###### Air Wep Magic Cast ######
###########################

Move A_Wep_MagicCast_Draw
	Requires	FLY COMBAT WEAPON MAGIC SPAWNBACK
	Priority	65
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Wep_MagicCast
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_draw_axe 1 30
	TEnd
MEnd

Move A_Wep_MagicCast
	Requires	FLY COMBAT WEAPON MAGIC
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Wep_MagicCast_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_wep_magic_L 1 60
	TEnd
MEnd

Move A_Wep_MagicCast_Post
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        Lazy_W_flycycle

	Type		male
	  Anim          male\air_wep_magic_L 61
	TEnd
MEnd

#############################
###### Air Wep Frisbee Cast ######
###########################

Move A_Wep_FrisbeeCast_Draw
	Requires	FLY COMBAT WEAPON MAGICTHROW SPAWNBACK
	Priority	65
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Wep_FrisbeeCast
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_draw_axe 1 30
	TEnd
MEnd

Move A_Wep_FrisbeeCast
	Requires	FLY COMBAT WEAPON MAGICTHROW
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Wep_FrisbeeCast_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_wep_frisbee 1 30
	TEnd
MEnd

Move A_Wep_FrisbeeCast_Post
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        Lazy_W_flycycle

	Type		male
	  Anim          male\air_wep_frisbee 31
	TEnd
MEnd

###############################
####### Air Wep Telekinesis Cast ###
##############################

Move A_Wep_Telekinesis_Draw
	Requires	FLY COMBAT WEAPON TELEKINESIS ATTACK SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Wep_Telekinesis
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_draw_axe 1 30
	TEnd
MEnd

Move A_Wep_Telekinesis
	Requires	FLY COMBAT WEAPON TELEKINESIS ATTACK
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Wep_Telekinesis_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_wep_telekinesis 2 35
	TEnd
MEnd

Move A_Wep_Telekinesis_Post
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        Lazy_W_flycycle

	Type		male
	  Anim          male\air_wep_telekinesis 36
	TEnd
MEnd

## UNDEAD SLAYING AXE ##################################################################

Move A_GhostAxe_Draw
	Requires	FLY COMBAT WEAPON GHOSTAXE UPPERCUT SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_GhostAxe
	Sets            CANTMOVE
	Flags		NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\air_draw_axe 1 30
	TEnd

	Type		huge
	  Anim          huge\air_draw_axe 1 30
	TEnd

	Type		fem
	  Anim		fem\air_draw_axe 1 30
	TEnd
MEnd

Move A_GhostAxe
	Requires	FLY COMBAT WEAPON GHOSTAXE UPPERCUT
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_GhostAxe_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\air_UndeadAxe_Uppercut 1 30
	TEnd

	Type		huge
	  Anim          huge\air_UndeadAxe_Uppercut 1 30
	TEnd

	Type		fem
	  Anim		fem\air_UndeadAxe_Uppercut 1 30
	TEnd
MEnd

Move A_GhostAxe_Post
	Requires	FLY COMBAT WEAPON #GHOSTAXE
	Priority	13
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        A_GhostAxe_Ready_15
	Scale           0.50
	Flags		NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\air_UndeadAxe_Uppercut 31
	TEnd

	Type		huge
	  Anim          huge\air_UndeadAxe_Uppercut 31
	TEnd

	Type		fem
	  Anim          fem\air_UndeadAxe_Uppercut 31
	TEnd
MEnd

#############################################################

Move A_GhostAxe_Ready_15
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        A_GhostAxe_Ready
	Scale           0.50
	Flags		NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\Air_UndeadAxe_Cycle 15 31
	TEnd

	Type		huge
	  Anim          huge\Air_UndeadAxe_Cycle 15 31
	TEnd

	Type		fem
	  Anim          fem\Air_UndeadAxe_Cycle 15 31
	TEnd
MEnd

Move A_GhostAxe_Ready
	Requires	FLY COMBAT #WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_air.inc
	Scale           0.50
	Flags		CYCLE NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\Air_UndeadAxe_Cycle 1 31
	TEnd

	Type		huge
	  Anim          huge\Air_UndeadAxe_Cycle 1 31
	TEnd

	Type		fem
	  Anim          fem\Air_UndeadAxe_Cycle 1 31
	TEnd
MEnd

####################################################################
## Air Bat Attack
####################################################################

Move A_Draw_Bat
	Requires	FLY COMBAT WEAPON BLUNT SPAWNBACK
	Priority	65
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        W_flycycle
	#Sets            CANTMOVE
	Flags		NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\air_draw_axe 1 30
	TEnd
MEnd

####################################################################

Move A_Bat
	Requires	FLY COMBAT WEAPON ATTACK BLUNT SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Bat_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE FIXARMREG
	Scale		1.25

	Type		male
	  Anim          male\air_sword_1st_slash 1 54
	TEnd

	Type		huge
	  Anim          huge\air_sword_1st_slash 1 54
	TEnd

	Type		fem
	  Anim          fem\air_sword_1st_slash 1 54
	TEnd
MEnd

Move A_Bat_Post
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        W_flycycle_half
	Flags		NOSIZESCALE FIXARMREG
	Scale		1.25

	Type		male
	  Anim          male\air_sword_1st_slash 55
	TEnd

	Type		huge
	  Anim          huge\air_sword_1st_slash 55
	TEnd

	Type		fem
	  Anim          fem\air_sword_1st_slash 55
	TEnd
MEnd

## AIR WEAPON PLACATE ###########################################################

Move A_Weapon_Placate_Draw
	Requires	FLY COMBAT WEAPON PLACATE SPAWNBACK
	Priority	65
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Weapon_Placate
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_draw_axe 1 30
	TEnd
MEnd

Move A_Weapon_Placate
	Requires	FLY COMBAT WEAPON PLACATE
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Weapon_Placate_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_placate 1 40
	TEnd
MEnd

Move A_Weapon_Placate_Post
	Requires	FLY COMBAT
	Priority	12
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        W_flycycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_placate 41
	TEnd
MEnd

## AIR BROADSWORD ASSASSIN'S STRIKE - DRAW AND WINDUP ###########################################################

Move A_AssassinsStrike_Broadsword_Draw
	Requires	FLY COMBAT WEAPON WINDUPBROADSWORDBACKSTAB SPAWNBACK
	Priority	65
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <MoveIRQ> # <AttackIRQ>
	NextMove	A_AssassinsStrike_Broadsword_Windup
	Flags		NOSIZESCALE PitchToTarget
	##Sets		CANTMOVE FLAG MUST NOT BE USED

	Type		male
	  Anim          male\air_draw_axe 1 30
	TEnd
MEnd

Move A_AssassinsStrike_Broadsword_Windup
	Requires	FLY COMBAT WEAPON WINDUPBROADSWORDBACKSTAB
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <MoveIRQ> # <AttackIRQ>
	NextMove        A_AssassinsStrike_Broadsword_WindupCycle
	Flags		NOSIZESCALE PitchToTarget
	##Sets		CANTMOVE FLAG MUST NOT BE USED

	Type		male
	  Anim          male\assassinstrike_broadsword_air 1 61
	TEnd
MEnd

Move A_AssassinsStrike_Broadsword_WindupCycle
	Requires	COMBAT WEAPON WINDUPBROADSWORDBACKSTAB
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <MoveIRQ> # <AttackIRQ>
	NextMove        A_AssassinsStrike_Broadsword
	Flags		Cycle NOSIZESCALE PitchToTarget
	##Sets		CANTMOVE FLAG MUST NOT BE USED

	Type		male
	  Anim          male\assassinstrike_broadsword_air 61 61
	TEnd
MEnd

## AIR BROADSWORD ASSASSIN'S STRIKE - QUICK ###########################################################

Move A_AssassinsStrike_Broadsword_Quick_Draw
	Requires	FLY COMBAT WEAPON BROADSWORDBACKSTAB SPAWNBACK
	Priority	65
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <MoveIRQ> # <AttackIRQ>
	NextMove	A_AssassinsStrike_Broadsword
	Flags		NOSIZESCALE PitchToTarget
	##Sets		CANTMOVE FLAG MUST NOT BE USED

	Type		male
	  Anim          male\air_draw_axe 1 30
	TEnd
MEnd

Move A_AssassinsStrike_Broadsword
	Requires	FLY COMBAT WEAPON BROADSWORDBACKSTAB
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <MoveIRQ> # <AttackIRQ>
	NextMove        A_AssassinsStrike_Broadsword_Post
	Flags		NOSIZESCALE PitchToTarget
	##Sets		CANTMOVE FLAG MUST NOT BE USED

	Type		male
	  Anim          male\assassinstrike_broadsword_air 61 98
	TEnd
MEnd

Move A_AssassinsStrike_Broadsword_Post
	Requires	COMBAT WEAPON BROADSWORDBACKSTAB
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <MoveIRQ> # <AttackIRQ>
	NextMove        W_flycycle
	Flags		Cycle NOSIZESCALE PitchToTarget
	##Sets		CANTMOVE FLAG MUST NOT BE USED

	Type		male
	  Anim          male\assassinstrike_broadsword_air 98
	TEnd
MEnd

## AIR BROADSWORD PLACATE ###########################################################

Move A_Weapon_Placate__Broadsword_Draw
	Requires	FLY COMBAT WEAPON MACE PLACATE SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Weapon_Placate_Broadsword
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_draw_axe 1 30
	TEnd
MEnd

Move A_Weapon_Placate_Broadsword
	Requires	FLY COMBAT WEAPON MACE PLACATE
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Weapon_Placate_Broadsword_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\placate_broadsword_air 1 45
	TEnd
MEnd

Move A_Weapon_Placate_Broadsword_Post
	Requires	FLY COMBAT
	Priority	12
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        W_flycycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\placate_broadsword_air 45
	TEnd
MEnd

## BOOMERANG ###########################################################

Move A_Boomerang
	Requires	FLY COMBAT WEAPON BOOMERANG
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Boomerangpost
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_boomerang 1 55
	TEnd

	Type		huge
	  Anim          huge\air_boomerang 1 55
	TEnd

	Type		fem
	  Anim          fem\air_boomerang 1 55
	TEnd
MEnd

Move A_Boomerangpost
	Requires	FLY COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon_Air.inc
	NextMove	W_flyready
 	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_boomerang 55
	TEnd

	Type		huge
	  Anim          huge\air_boomerang 55
	TEnd

	Type		fem
	  Anim          fem\air_boomerang 55
	TEnd
MEnd

## GRENADE LOB ###########################################################

Move A_Grenadelob
	Requires	FLY COMBAT WEAPON GRENADE
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Grenadelobpost
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_grenade_lob 1 40
	TEnd

	Type		huge
	  Anim          huge\air_grenade_lob 1 40
	TEnd

	Type		fem
	  Anim          fem\air_grenade_lob 1 40
	TEnd
MEnd

Move A_Grenadelobpost
	Requires	FLY COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove	W_flyready
 	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_grenade_lob 40
	TEnd

	Type		huge
	  Anim          huge\air_grenade_lob 40
	TEnd

	Type		fem
	  Anim          fem\air_grenade_lob 40
	TEnd
MEnd

#############################################################
## BRAWLS
#############################################################

## MACE BRAWL ###########################################################

Move A_Mace_Punch
	Requires	FLY COMBAT WEAPON PUNCH MACE
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Mace_Punch2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_wm_slash 1 25
	TEnd

	Type		huge
	  Anim          huge\air_wm_slash 1 25
	TEnd

	Type		fem
	  Anim          fem\air_wm_slash 1 25
	TEnd
MEnd

Move A_Mace_Punch2
	Requires	FLY COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove	W_flycycle_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_wm_slash 25
	TEnd

	Type		huge
	  Anim          huge\air_wm_slash 25
	TEnd

	Type		fem
	  Anim          fem\air_wm_slash 25
	TEnd
MEnd

## PISTOL BRAWL ###########################################################

Move A_SingleGun_PistolWhip
	Requires	FLY COMBAT WEAPON HANDGUN PUNCH BRAWL
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_SingleGun_PistolWhip2
	Sets            CANTMOVE
	SticksOnChild   BACKTOREADY
	Flags		NoSizeScale

	Type		male
	  Anim          male\Thug_air_PistolWhipR 1 20
	TEnd
MEnd

Move A_SingleGun_PistolWhip2
	Requires	FLY COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove	A_PISTOLS_Ready
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\Thug_air_PistolWhipR 20
	TEnd
MEnd


#############################################################
Include sequencers\player\WEAPONS\Weapons_Whip_a.inc
Include sequencers\player\WEAPONS\Weapons_TwoHanded_a.inc