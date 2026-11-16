#########################################################
## WEAPON.INC - Sequencers related to WEAPON animations
#########################################################

#############################################################
## Draw Bat
#############################################################

Move Draw_WeaponBack
	Requires	COMBAT WEAPON SPAWNBACK
	Priority        53
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        WepReady
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\draw_axe 1 30
	TEnd
MEnd

## DRAW SABER ###########################################################

Move Draw_Saber
	Requires	COMBAT WEAPON KNIFE SPAWNSIDE
	Priority        54
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        WepReady
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\draw_saber 1
	TEnd

	Type		huge
	  Anim          huge\draw_saber 1
	TEnd

	Type		fem
	  Anim          fem\draw_saber 1
	TEnd
MEnd

## DRAW MAGICSTAFF ###########################################################

Move Draw_MagicStaff
	Requires	COMBAT WEAPON CLUB STAFF SPAWNSIDE
	Priority        55
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        WepReady
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\firesword_draw 1
	TEnd
MEnd

## DRAW PISTOL ###########################################################

Move Draw_Pistol
	Requires	COMBAT WEAPON HANDGUN SPAWNSIDE
	Priority        54
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Draw_Pistol_Post
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\draw_pistol 1
	TEnd

	Type		huge
	  Anim          male\draw_pistol 1
	TEnd

	Type		fem
	  Anim          male\draw_pistol 1
	TEnd
MEnd

Move Draw_Pistol_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        WepReady

	Type		male
	  Anim          male\wep_cycle 1 31
	TEnd

	Type		huge
	  Anim          huge\wep_cycle 1 31
	TEnd

	Type		fem
	  Anim          fem\wep_cycle 1 31
	TEnd
MEnd

## DRAW GUNSLINGER ###########################################################

Move Draw_Gunslinger
	Requires	COMBAT WEAPON HANDGUN SPAWNHIP
	Priority        54
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Draw_Gunslinger_post
	Interpolate	8
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\gunslinger_shoot 1 31  ##35
	  PitchStart	14
	TEnd
MEnd

Move Draw_Gunslinger_post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	Flags		CYCLE PitchToTarget

	Type		male
	  Anim          male\gunslinger_shoot 70 95  ##35 36
	TEnd
MEnd

## GUNSLINGER DRAW CONE ###########################################################

Move Draw_GunslingerCone
	Requires	COMBAT WEAPON HANDGUN CONE SPAWNHIP
	Priority        55
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Draw_GunslingerCone_Post
	Interpolate	8
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\gunslinger_cone 1 24
	  PitchStart	12
	TEnd
MEnd

Move Draw_GunslingerCone_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	Flags		CYCLE PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\gunslinger_cone 80 105
	TEnd
MEnd

#############################################################
## WEAPON BRAWL - KICK
#############################################################

Move W_KickBrawl_R
	Requires	COMBAT WEAPON KICKBRAWL
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        W_KickBrawl_R_post
	Flags		CANTMOVE

	Type		male
	  Anim          male\kickbrawl_R 1 20
	TEnd
MEnd

Move W_KickBrawl_R_post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        W_KickBrawl_cycle

	Type		male
	  Anim          male\kickbrawl_R 21 37
	TEnd
MEnd

#############################################################

Move W_KickBrawl_L
	Requires	COMBAT WEAPON KICKBRAWL
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        W_KickBrawl_L_post
	Flags		CANTMOVE

	Type		male
	  Anim          male\kickbrawl_L 1 20
	TEnd
MEnd

Move W_KickBrawl_L_post
	Requires	COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        W_KickBrawl_cycle
	Priority        2

	Type		male
	  Anim          male\kickbrawl_L 21 37
	TEnd
MEnd

Move W_KickBrawl_cycle
	Requires	COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon.inc
	Flags		Cycle
	NextMove        postReady_back
	Priority        2
	Interpolate	8
	Scale           0.50

	Type		male
	  Anim          male\kick_cycle_wep 1
	TEnd
MEnd

## WEAPON PLACATE ###########################################################

Move Weapon_Placate_Draw
	Requires	COMBAT WEAPON PLACATE SPAWNBACK
	Priority        54
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Weapon_Placate
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\draw_axe 1 30
	TEnd
MEnd

Move Weapon_Placate
	Requires	COMBAT WEAPON PLACATE
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Weapon_Placate_post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\placate 1 40
	TEnd
MEnd

Move Weapon_Placate_post
	Requires	COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Lazy_CombatReady
	Priority        2

	Type		male
	  Anim          male\placate 41
	TEnd
MEnd

#######################################
# Club smash (can be used for swords) ##
######################################

Move Club_Smash_Draw
	Requires	COMBAT WEAPON AXE CLUBSMASH SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Club_Smash
	Sets            CANTMOVE

	Type		male
	  Anim          male\draw_axe 1 30
	TEnd
MEnd

Move Club_Smash
	Requires	COMBAT WEAPON AXE CLUBSMASH
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Club_Smash_Post
	Sets            CANTMOVE

	Type		male
	  Anim          huge\giant_club_smash 1 40
	TEnd
MEnd

Move Club_Smash_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Club_Swing_PostCycle
	Flags		NOSIZESCALE

	Type		male
	  Anim          huge\giant_club_smash 41
	TEnd
MEnd


#######################################
# Club Swing (can be used for swords) ##
######################################

Move Club_Swing_Draw
	Requires	COMBAT WEAPON AXE CLUBSWING SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Club_Swing
	Sets            CANTMOVE

	Type		male
	  Anim          male\draw_axe 1 30
	TEnd
MEnd

Move Club_Swing
	Requires	COMBAT WEAPON AXE CLUBSWING
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Club_Swing_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          huge\giant_club_bigswing 1 40
	TEnd
MEnd

Move Club_Swing_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Club_Swing_PostCycle
	Flags		NOSIZESCALE

	Type		male
	  Anim          huge\giant_club_bigswing 41
	TEnd
MEnd

Move Club_Swing_PostCycle
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	Flags		Cycle
	NextMove        postReady_back

	Type		male
	  Anim          huge\giant_club_cycle 1 60
	TEnd
MEnd

#############################################################
## KNIFE ATTACK
#############################################################

Move Knife_Attack_Draw
	Requires	COMBAT WEAPON ATTACK KNIFE SPAWNSIDE
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Knife_Attack
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\draw_saber 1 21
	TEnd

	Type		huge
	  Anim          huge\draw_saber 1 21
	TEnd

	Type		fem
	  Anim          fem\draw_saber 1 21
	TEnd
MEnd

Move Knife_Attack
	Requires	COMBAT WEAPON ATTACK KNIFE
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Knife_Attack_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\wm_slash 1 16

	TEnd

	Type		huge
	  Anim          huge\wm_slash 1 16
	TEnd

	Type		fem
	  Anim          fem\wm_slash 1 16
	TEnd
MEnd

Move Knife_Attack_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Wep_half

	Type		male
	  Anim          male\wm_slash 17
	  PlayFX		NPCFX\NPC_Whoosh_SFX.fx
	TEnd

	Type		huge
	  Anim          huge\wm_slash 17
	  PlayFX		NPCFX\NPC_Whoosh_SFX.fx
	TEnd

	Type		fem
	  Anim          fem\wm_slash 17
	  PlayFX		NPCFX\NPC_Whoosh_SFX.fx
	TEnd
MEnd


###################
###### knife Stab ######
###################

Move Knife_Stab_Draw
	Requires	COMBAT WEAPON STAB KNIFE SPAWNSIDE
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Knife_Stab
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\draw_saber 1 21
	TEnd

	Type		huge
	  Anim          huge\draw_saber 1 21
	TEnd

	Type		fem
	  Anim          fem\draw_saber 1 21
	TEnd
MEnd

Move Knife_Stab
	Requires	COMBAT WEAPON STAB KNIFE
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Knife_Stab_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\wm_stab 1 16
	TEnd

	Type		huge
	  Anim          huge\wm_stab 1 16
	TEnd

	Type		fem
	  Anim          fem\wm_stab 1 16
	TEnd
MEnd

Move Knife_Stab_Post
	Requires	COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Wep_half
	Priority        2

	Type		male
	  Anim          male\wm_stab 17
	TEnd

	Type		huge
	  Anim          huge\wm_stab 17
	TEnd

	Type		fem
	  Anim          fem\wm_stab 17
	TEnd
MEnd

###################
###### Knife Parry  ######
###################

Move Knife_Parry_Draw
	Requires	COMBAT WEAPON KNIFE PARRY SPAWNSIDE
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Knife_Parry
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\draw_saber 1 21
	TEnd

	Type		huge
	  Anim          huge\draw_saber 1 21
	TEnd

	Type		fem
	  Anim          fem\draw_saber 1 21
	TEnd
MEnd

Move Knife_Parry
	Requires	COMBAT WEAPON KNIFE PARRY
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Knife_Parry_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\sword_parry 1 54
	TEnd

	Type		huge
	  Anim          huge\sword_parry 1 54
	TEnd

	Type		fem
	  Anim          fem\sword_parry 1 54
	TEnd
MEnd

Move Knife_Parry_Post
	Requires	COMBAT WEAPON
	Priority        2
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ> <IRQEmotesRighthand>
	NextMove        Wep_half

	Type		male
	  Anim          male\sword_parry 55
	TEnd

	Type		huge
	  Anim          huge\sword_parry 55
	TEnd

	Type		fem
	  Anim          fem\sword_parry 55
	TEnd
MEnd

##########################
###### Ringblade Slash   ######
##########################

Move RingBlade_Slash_Draw
	Requires	COMBAT WEAPON SLASH KNIFE SPAWNSIDE
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        RingBlade_Slash1
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          fem\draw_saber 1 21
	TEnd
MEnd

Move RingBlade_Slash1
	Requires	COMBAT WEAPON SLASH KNIFE
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        RingBlade_Slash1_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE PickRandomly

	Type		male
	  Anim          fem\wm_slash 1 16
	TEnd
MEnd

Move RingBlade_Slash1_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Wep_half

	Type		male
	  Anim          fem\wm_slash 17
	TEnd
MEnd

Move RingBlade_Slash2
	Requires	COMBAT WEAPON SLASH KNIFE
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        RingBlade_Slash2_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE PickRandomly

	Type		male
	  Anim          fem\wm_upcut 1 16
	TEnd
MEnd

Move RingBlade_Slash2_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Wep_half

	Type		male
	  Anim          fem\wm_upcut 17
	TEnd
MEnd

##########################
###### Ringblade Toss   ######
##########################

Move RingBlade_Toss_Draw
	Requires	COMBAT WEAPON SHORTTOSS KNIFE SPAWNSIDE
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        RingBlade_Toss
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          fem\draw_saber 1 21
	TEnd
MEnd

Move RingBlade_Toss
	Requires	COMBAT WEAPON SHORTTOSS KNIFE
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        RingBlade_Toss_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          fem\slicer_toss 1 25
	TEnd
MEnd

Move RingBlade_Toss_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Wep_half

	Type		male
	  Anim          fem\slicer_toss 26
	TEnd
MEnd

## BATON ATTACK ###########################################################

Move Baton_Attack_Draw
	Requires	COMBAT WEAPON ATTACK KNIFE CLUB SPAWNSIDE
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Baton_Attack
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\draw_saber 1 21
	TEnd

	Type		huge
	  Anim          huge\draw_saber 1 21
	TEnd

	Type		fem
	  Anim          fem\draw_saber 1 21
	TEnd
MEnd

Move Baton_Attack
	Requires	COMBAT WEAPON KNIFE CLUB ATTACK
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Baton_Attack_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\wm_chop 1 45
	TEnd

	Type		huge
	  Anim          huge\wm_chop 1 45
	TEnd

	Type		fem
	  Anim          fem\wm_chop 1 45
	TEnd
MEnd

Move Baton_Attack_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Wep_half

	Type		male
	  Anim          male\wm_chop 46
	TEnd

	Type		huge
	  Anim          huge\wm_chop 46
	TEnd

	Type		fem
	  Anim          fem\wm_chop 46
	TEnd
MEnd

#############################################################
## Bat Attack
#############################################################

Move Bat_Attack_Draw
	Requires	COMBAT WEAPON ATTACK BLUNT SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Bat_Attack1
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\draw_axe 1 30
	TEnd
MEnd

Move Bat_Attack1
	Requires	COMBAT WEAPON ATTACK BLUNT
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Bat_Attack1_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\villain_bat_2Hupcut 1 44
	  PlayFX		NPCFX\NPC_WhooshBig_SFX.fx 5
	TEnd
MEnd

Move Bat_Attack1_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Bat_Attack1_PostCycle
	Flags		NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\villain_bat_2Hupcut 45
	TEnd
MEnd

Move Bat_Attack1_PostCycle
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	Flags		CYCLE NOSIZESCALE FIXARMREG
	NextMove        postReady
	Scale           0.50

	Type		male
	  Anim          male\sword_cycle 1 31
	TEnd
MEnd

#############################################################

Move Bat_Attack2
	Requires	COMBAT WEAPON ATTACK BLUNT
	Priority        4
	Interrupts	Bat_Attack1_PostCycle
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Bat_Attack2_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\2sword_mid 51 75
	  PlayFX		NPCFX\NPC_WhooshBig_SFX.fx
	TEnd
MEnd

Move Bat_Attack2_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Bat_Attack1_PostCycle
	Flags		NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\2sword_mid 76
	TEnd
MEnd

#############################################################

Move Bat_Attack3
	Requires	COMBAT WEAPON ATTACK BLUNT
	Priority        4
	Interrupts	Bat_Attack1_PostCycle
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Bat_Attack3_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\2sword_mid 1 16
	  PlayFX		NPCFX\NPC_WhooshBig_SFX.fx
	TEnd
MEnd

Move Bat_Attack3_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Bat_Attack1_PostCycle
	Flags		NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\2sword_mid 17 49
	TEnd
MEnd

###################
###### Bat Shoot  ######
###################

Move Bat_Shoot_Draw
	Requires	COMBAT WEAPON SHOOT BLUNT SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Bat_Shoot
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\draw_axe 1 30
	TEnd
MEnd

Move Bat_Shoot
	Requires	COMBAT WEAPON SHOOT BLUNT
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Bat_Shoot_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\mace_shoot 1 45
	  PitchAngle	0.5
	TEnd
MEnd

Move Bat_Shoot_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Bat_Shoot_PostCycle
	Flags		PitchToTarget

	Type		male
	  Anim          male\mace_shoot 46
	  PitchEnd	65
	  PitchAngle	0.5
	TEnd
MEnd

Move Bat_Shoot_PostCycle
	Requires	COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon.inc
	Flags		Cycle
	NextMove        postReady
	Priority        2
	Scale           0.50

	Type		male
	  Anim          male\mace_shoot_cycle 1 31
	TEnd
MEnd

#############################################
###### Mace Shoot (works with player mace) ######
############################################

Move Mace_Shoot_Draw
	Requires	COMBAT WEAPON MACE SHOOT SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Mace_Shoot
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\draw_axe 1 30
	TEnd
MEnd

Move Mace_Shoot
	Requires	COMBAT WEAPON MACE SHOOT
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Mace_Shoot_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\wep_mace_projectile 1 45
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\wep_mace_projectile 1 45
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\wep_mace_projectile 1 45
	  PitchAngle	0.5
	TEnd
MEnd

Move Mace_Shoot_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Mace_Shoot_PostCycle
	Flags		PitchToTarget

	Type		male
	  Anim          male\wep_mace_projectile 46 109
	  PitchEnd	65
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\wep_mace_projectile 46 109
	  PitchEnd	65
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\wep_mace_projectile 46 109
	  PitchEnd	65
	  PitchAngle	0.5
	TEnd
MEnd

Move Mace_Shoot_PostCycle
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	Flags		Cycle
	NextMove        postReady

	Type		male
	  Anim          male\wep_mace_projectile 112
	TEnd

	Type		huge
	  Anim          huge\wep_mace_projectile 112
	TEnd

	Type		fem
	  Anim          fem\wep_mace_projectile 112
	TEnd
MEnd

#############################################
###### Generic NPC Enemy Shoot ######
############################################

Move Generic_Shoot_Draw
	Requires	COMBAT WEAPON SHOOT SPAWNBACK
	Priority        54
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Generic_Shoot
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\draw_axe 1 30
	TEnd
MEnd

Move Generic_Shoot
	Requires	COMBAT WEAPON SHOOT
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Generic_Shoot_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\wep_mace_projectile 1 45
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\wep_mace_projectile 1 45
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\wep_mace_projectile 1 45
	  PitchAngle	0.5
	TEnd
MEnd

Move Generic_Shoot_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Generic_Shoot_PostCycle
	Flags		PitchToTarget

	Type		male
	  Anim          male\wep_mace_projectile 46 109
	  PitchEnd	65
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\wep_mace_projectile 46 109
	  PitchEnd	65
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\wep_mace_projectile 46 109
	  PitchEnd	65
	  PitchAngle	0.5
	TEnd
MEnd

Move Generic_Shoot_PostCycle
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	Flags		Cycle
	NextMove        postReady

	Type		male
	  Anim          male\wep_mace_projectile 112
	TEnd

	Type		huge
	  Anim          huge\wep_mace_projectile 112
	TEnd

	Type		fem
	  Anim          fem\wep_mace_projectile 112
	TEnd
MEnd


#############################################
###### Mace Summon (point mace in air) ######
############################################

Move Mace_Summon_Draw
	Requires	COMBAT WEAPON MACESUMMON SPAWNBACK
	Priority        54
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Mace_Summon
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\draw_axe 1 30
	TEnd
MEnd

Move Mace_Summon
	Requires	COMBAT WEAPON MACESUMMON
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Mace_Summon_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\wep_mace_summon 1 60
	TEnd
MEnd

Move Mace_Summon_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Lazy_WepReady
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\wep_mace_summon 61
	TEnd
MEnd

################################
###### Wep (Mace) Boostup  ######
################################

Move Wep_Boostup_Draw
	Requires	COMBAT WEAPON WEPBOOSTUP SPAWNBACK
	Priority        54
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Wep_Boostup
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\draw_axe 1 30
	TEnd
MEnd

Move Wep_Boostup
	Requires	COMBAT WEAPON WEPBOOSTUP
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Wep_Boostup_Post
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\absorb 1 10
	TEnd
MEnd

Move Wep_Boostup_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Wep_Boostup_PostCycle
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\absorb 11 60
	TEnd
MEnd

Move Wep_Boostup_PostCycle
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	Flags		Cycle
	NextMove        postReady_back
	Scale		0.5
	Interpolate	15

	Type		male
	  Anim          male\wep_cycle 1 31
	TEnd
MEnd

#########################
##### MaceSniper Shoot  ######  ##(NOTE: make sure all shotgun readys have <ReactIRQ>)
##########################

Move Mace_SniperWindup_Draw
	Requires	COMBAT WEAPON MACE MACESNIPERWINDUP SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <MoveIRQ> #<ReactIRQ> <HitIRQ> <AttackIRQ>
	NextMove        Mace_SniperWindup
	Flags		NOSIZESCALE FixArmReg

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

Move Mace_SniperWindup
	Requires	COMBAT WEAPON MACESNIPERWINDUP
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ>
	Flags		PitchToTarget NOSIZESCALE FixArmReg
	NextMove        Mace_SniperWindup_Cycle

	Type		male
	  Anim          male\wep_mace_sniper 1 73
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\wep_mace_sniper 1 73
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\wep_mace_sniper 1 73
	  PitchAngle	0.5
	TEnd
MEnd

Move Mace_SniperWindup_Cycle
	Requires	COMBAT WEAPON MACESNIPERWINDUP
	Priority        3
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ>
	Flags		Cycle PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\wep_mace_sniper 74 108
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\wep_mace_sniper 74 108
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\wep_mace_sniper 74 108
	  PitchAngle	0.5
	TEnd
MEnd

#############################################################

Move Mace_SniperShot
	Requires	COMBAT WEAPON MACE MACESNIPER
	Priority        4
	Interrupts	<AttackIRQ> <HitIRQ> Mace_SniperWindup Mace_SniperWindup_Cycle
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Mace_SniperShot_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\wep_mace_sniper 109 157
	TEnd

	Type		huge
	  Anim          huge\wep_mace_sniper 109 157
	TEnd

	Type		fem
	  Anim          fem\wep_mace_sniper 109 157
	TEnd
MEnd

Move Mace_SniperShot_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Mace_SniperShot_PostCycle
	Flags		FixArmReg NOSIZESCALE

	Type		male
	  Anim          male\wep_mace_sniper 158 173
	TEnd

	Type		huge
	  Anim          huge\wep_mace_sniper 158 173
	TEnd

	Type		fem
	  Anim          fem\wep_mace_sniper 158 173
	TEnd
MEnd

Move Mace_SniperShot_PostCycle
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	Flags		Cycle FixArmReg
	NextMove        postReady_back

	Type		male
	  Anim          male\wep_mace_sniper 174 233
	TEnd

	Type		huge
	  Anim          huge\wep_mace_sniper 174 233
	TEnd

	Type		fem
	  Anim          fem\wep_mace_sniper 174 233
	TEnd
MEnd

#################################
###### Bat Pummel ######
#################################

Move Bat_Pummel_Draw
	Requires	COMBAT WEAPON PUMMEL BLUNT SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Bat_Pummel
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\draw_axe 1 30
	TEnd
MEnd

Move Bat_Pummel
	Requires	COMBAT WEAPON PUMMEL BLUNT
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Bat_Pummel_Post
	Sets            CANTMOVE
	Flags		FixArmReg NOSIZESCALE

	Type		male
	  Anim          male\katana_pummel 1 15
	TEnd

	Type		fem
	  Anim          fem\katana_pummel 1 15
	TEnd

	Type		huge
	  Anim          huge\katana_pummel 1 15
	TEnd
MEnd

Move Bat_Pummel_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Bat_Pummel_PostCycleBlend
	Flags		FixArmReg NOSIZESCALE

	Type		male
	  Anim          male\katana_pummel 16 49
	TEnd

	Type		fem
	  Anim          fem\katana_pummel 16 47
	TEnd

	Type		huge
	  Anim          huge\katana_pummel 16 47
	TEnd
MEnd

Move Bat_Pummel_PostCycleBlend
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Bat_Pummel_PostCycle
	Scale           0.50
	Flags		FixArmReg NOSIZESCALE

	Type		male
	  Anim          male\katana_cycle 15 31
	TEnd

	Type		fem
	  Anim          fem\katana_cycle 15 31
	TEnd

	Type		huge
	  Anim          huge\katana_cycle 15 31
	TEnd
MEnd

Move Bat_Pummel_PostCycle
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	Flags		Cycle FixArmReg NOSIZESCALE
	NextMove        postReady_back
	Scale           0.50

	Type		male
	  Anim          male\katana_cycle 1 31
	TEnd

	Type		fem
	  Anim          fem\katana_cycle 1 31
	TEnd

	Type		huge
	  Anim          huge\katana_cycle 1 31
	TEnd
MEnd


#################################
###### sword Spear throw Left ######
#################################

Move Spear_Throw_L_Draw
	Requires	COMBAT WEAPON AXE SPEARTHROWLEFT SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Spear_Throw_L
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\draw_axe 1 30
	TEnd
MEnd

Move Spear_Throw_L
	Requires	COMBAT WEAPON AXE SPEARTHROWLEFT
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Spear_Throw_L_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\SpearThrow_L 1 55
	TEnd
MEnd

Move Spear_Throw_L_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Blend_WepReady

	Type		male
	  Anim          male\SpearThrow_L 56 85
	TEnd
MEnd

#############################################################
## Sword Attack
#############################################################

Move Sword_Attack_Draw
	Requires	COMBAT WEAPON AXE SLASH SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Sword_Attack
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

Move Sword_Attack
	Requires	COMBAT WEAPON AXE SLASH
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Sword_Attack_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\sword_1st_slash 1 54
	  PlayFX	POWERS\SwordWhoosh.fx 15
	TEnd

	Type		huge
	  Anim          huge\sword_1st_slash 1 54
	  PlayFX	POWERS\SwordWhoosh.fx 15
	TEnd

	Type		fem
	  Anim          fem\sword_1st_slash 1 54
	  PlayFX	POWERS\SwordWhoosh.fx 15
	TEnd
MEnd

Move Sword_Attack_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Wep_half

	Type		male
	  Anim          male\sword_1st_slash 55
	TEnd

	Type		huge
	  Anim          huge\sword_1st_slash 55
	TEnd

	Type		fem
	  Anim          fem\sword_1st_slash 55
	TEnd
MEnd

#################################
###### sword Slash (Horizontal) ######
#################################

Move Sword_Slash_Draw
	Requires	COMBAT WEAPON AXE SLASH MID SPAWNBACK
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Sword_Slash
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

Move Sword_Slash
	Requires	COMBAT WEAPON AXE SLASH MID
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Sword_Slash_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\wm_slash 1 41
	TEnd

	Type		huge
	  Anim          huge\wm_slash 1 41
	TEnd

	Type		fem
	  Anim          fem\wm_slash 1 41
	TEnd
MEnd

Move Sword_Slash_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Wep_half

	Type		male
	  Anim          male\wm_slash 42
	TEnd

	Type		huge
	  Anim          huge\wm_slash 42
	TEnd

	Type		fem
	  Anim          fem\wm_slash 42
	TEnd
MEnd

## PARRY ###########################################################

Move Parry_Draw
	Requires	COMBAT WEAPON AXE PARRY SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Parry
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

Move Parry
	Requires	COMBAT WEAPON AXE PARRY
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Parry_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\sword_parry 1 40
	TEnd

	Type		huge
	  Anim          huge\sword_parry 1 40
	TEnd

	Type		fem
	  Anim          fem\sword_parry 1 40
	TEnd
MEnd

Move Parry_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Wep_half

	Type		male
	  Anim          male\sword_parry 41
	TEnd

	Type		huge
	  Anim          huge\sword_parry 41
	TEnd

	Type		fem
	  Anim          fem\sword_parry 41
	TEnd
MEnd


###################
###### sword Twirl  ######
###################

Move Twirl_Draw
	Requires	COMBAT WEAPON AXE SPIN SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ> <HitIRQ> <ReactIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Twirl
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

Move Twirl
	Requires	COMBAT WEAPON AXE SPIN
	Priority        4
	Interrupts	<AttackIRQ> <HitIRQ> <ReactIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Twirl_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\firesword_jumpspin 1 80
	TEnd

	Type		huge
	  Anim          huge\firesword_jumpspin 1 80
	TEnd

	Type		fem
	  Anim          fem\firesword_jumpspin 1 80
	TEnd

	Type		Rocksuit
	  Anim          beast\firesword_jumpspin 1 80
	TEnd
MEnd

Move Twirl_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Wep_half

	Type		male
	  Anim          male\firesword_jumpspin 81
	TEnd

	Type		huge
	  Anim          huge\firesword_jumpspin 81
	TEnd

	Type		fem
	  Anim          fem\firesword_jumpspin 81
	TEnd

	Type		Rocksuit
	  Anim          beast\firesword_jumpspin 81
	TEnd
MEnd


###################
###### Axe BASIC ATTACK  ######
###################

Move Axe_Chop_Draw
	Requires	COMBAT WEAPON SPAWNBACK CHOP AXE
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Axe_Chop
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\draw_axe 1 30
	TEnd
MEnd

Move Axe_Chop
	Requires	COMBAT WEAPON CHOP AXE
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Axe_Chop_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\wm_chop 2 38
	TEnd
MEnd

Move Axe_Chop_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Wep_half

	Type		male
	  Anim          male\wm_chop 39
	TEnd
MEnd


###################
###### Axe Swoop  ######
###################

Move Axe_Swoop_Draw
	Requires	COMBAT WEAPON UPPERCUT AXE SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Axe_Swoop
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

Move Axe_Swoop
	Requires	COMBAT WEAPON UPPERCUT AXE
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Axe_Swoop_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\axe_swoop 1 55
	TEnd

	Type		huge
	  Anim          huge\axe_swoop 1 55
	TEnd

	Type		fem
	  Anim          fem\axe_swoop 1 55
	TEnd
MEnd

Move Axe_Swoop_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Wep_half

	Type		male
	  Anim          male\axe_swoop 56
	TEnd

	Type		huge
	  Anim          huge\axe_swoop 56
	TEnd

	Type		fem
	  Anim          fem\axe_swoop 56
	TEnd
MEnd


###################
###### Axe Reverse Sweep  ######
###################

Move Axe_RSweep_Draw
	Requires	COMBAT WEAPON AXE SWIPE SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Axe_RSweep
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

Move Axe_RSweep
	Requires	COMBAT WEAPON AXE SWIPE
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Axe_RSweep_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\axe_reverse_sweep 1 60
	TEnd

	Type		huge
	  Anim          huge\axe_reverse_sweep 1 60
	TEnd

	Type		fem
	  Anim          fem\axe_reverse_sweep 1 60
	TEnd
MEnd

Move Axe_RSweep_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Wep_half

	Type		male
	  Anim          male\axe_reverse_sweep 61
	TEnd

	Type		huge
	  Anim          huge\axe_reverse_sweep 61
	TEnd

	Type		fem
	  Anim          fem\axe_reverse_sweep 61
	TEnd
MEnd


###################
###### mace BASIC ATTACK  ######
###################

Move Mace_Attack_Draw
	Requires	COMBAT WEAPON SWORD SPAWNBACK SLASH
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Mace_Attack
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

Move Mace_Attack
	Requires	COMBAT WEAPON SWORD SLASH
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Mace_Attack_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\sword_1st_slash 1 49
	TEnd

	Type		huge
	  Anim          huge\sword_1st_slash 1 49
	TEnd

	Type		fem
	  Anim          fem\sword_1st_slash 1 49
	TEnd
MEnd

Move Mace_Attack_Post
	Requires	COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Wep_half
	Priority        2

	Type		male
	  Anim          male\sword_1st_slash 50
	TEnd

	Type		huge
	  Anim          huge\sword_1st_slash 50
	TEnd

	Type		fem
	  Anim          fem\sword_1st_slash 50
	TEnd
MEnd

###################
###### mace uppercut\smash  ######
###################

Move Mace_Upcut_Draw
	Requires	COMBAT WEAPON UPPERCUT MACE SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Mace_Upcut
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

Move Mace_Upcut
	Requires	COMBAT WEAPON UPPERCUT MACE
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Mace_Upcut_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\wm_upcut 1 45
	TEnd

	Type		huge
	  Anim          huge\wm_upcut 1 45
	TEnd

	Type		fem
	  Anim          fem\wm_upcut 1 45
	TEnd
MEnd

Move Mace_Upcut_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Wep_half

	Type		male
	  Anim          male\wm_upcut 46
	TEnd

	Type		huge
	  Anim          huge\wm_upcut 46
	TEnd

	Type		fem
	  Anim          fem\wm_upcut 46
	TEnd
MEnd

###################
###### mace clobber\smash  ######
###################

Move Mace_Clobber_Draw
	Requires	COMBAT WEAPON MACE SMASH SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Mace_Clobber
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

Move Mace_Clobber
	Requires	COMBAT WEAPON MACE SMASH
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Mace_Clobber_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\mace_bigsmash 1 71
	TEnd

	Type		huge
	  Anim          huge\mace_bigsmash 1 71
	TEnd

	Type		fem
	  Anim          fem\mace_bigsmash 1 71
	TEnd
MEnd

Move Mace_Clobber_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Wep_half

	Type		male
	  Anim          male\mace_bigsmash 72
	TEnd

	Type		huge
	  Anim          huge\mace_bigsmash 72
	TEnd

	Type		fem
	  Anim          fem\mace_bigsmash 72
	TEnd
MEnd

###################
###### fire sword attack  ######
###################

Move Draw_firesword
	Requires	COMBAT WEAPON WEPSPAWN2
	Priority        53
	Interrupts	<Ready> <Combat> <WepCombat> <TwoHandCombat> <PostMoveCycle>
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ>
	NextMove        Draw_firesword_post

	Type		male
	  Anim          male\firesword_draw 1 21
	TEnd

	Type		huge
	  Anim          huge\firesword_draw 1 21
	TEnd

	Type		fem
	  Anim          fem\firesword_draw 1 21
	TEnd
MEnd

Move Draw_firesword_post
	Requires	COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        WepReady
	Priority        2

	Type		male
	  Anim          male\firesword_draw 22
	TEnd

	Type		huge
	  Anim          huge\firesword_draw 22
	TEnd

	Type		fem
	  Anim          fem\firesword_draw 22
	TEnd
MEnd

#############################################################
## Firesword Drawn and Slash
#############################################################

Move Firesword_DrawNSlash
	Requires	COMBAT WEAPON SWORD ATTACK WEPSPAWN2
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Firesword_DrawNSlash_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\firesword_drawNslash 1 80
	TEnd

	Type		huge
	  Anim          huge\firesword_drawNslash 1 80
	TEnd

	Type		fem
	  Anim          fem\firesword_drawNslash 1 80
	TEnd
MEnd

Move Firesword_DrawNSlash_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Wep_half

	Type		male
	  Anim          male\firesword_drawNslash 81
	TEnd

	Type		huge
	  Anim          huge\firesword_drawNslash 81
	TEnd

	Type		fem
	  Anim          fem\firesword_drawNslash 81
	TEnd
MEnd

#############################################################
## Firesword Slash
#############################################################

Move Firesword_Slash
	Requires	COMBAT WEAPON SWORD ATTACK
	Priority        4
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Firesword_Slash_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\wm_slash 1 30
	TEnd

	Type		huge
	  Anim          huge\wm_slash 1 30
	TEnd

	Type		fem
	  Anim          fem\wm_slash 1 30
	TEnd
MEnd

Move Firesword_Slash_Post
	Requires	COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Wep_half
	Priority        2

	Type		male
	  Anim          male\wm_slash 31
	TEnd

	Type		huge
	  Anim          huge\wm_slash 31
	TEnd

	Type		fem
	  Anim          fem\wm_slash 31
	TEnd
MEnd

#############################################################
## Firesword - Draw and Upcut
#############################################################

Move Firesword_DrawNUpcut
	Requires	COMBAT WEAPON SWORD 2HAND ATTACK WEPSPAWN2
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Firesword_DrawNUpcut_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\firesword_drawNupcut 1 89
	TEnd

	Type		huge
	  Anim          huge\firesword_drawNupcut 1 89
	TEnd

	Type		fem
	  Anim          fem\firesword_drawNupcut 1 89
	TEnd
MEnd

Move Firesword_DrawNUpcut_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Wep_half

	Type		male
	  Anim          male\firesword_drawNupcut 90
	TEnd

	Type		huge
	  Anim          huge\firesword_drawNupcut 90
	TEnd

	Type		fem
	  Anim          fem\firesword_drawNupcut 90
	TEnd
MEnd

#############################################################
## Firesword Upcut
#############################################################

Move Firesword_Upcut
	Requires	COMBAT WEAPON SWORD 2HAND ATTACK
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Firesword_Upcut_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\wm_upcut 1 50
	TEnd

	Type		huge
	  Anim          huge\wm_upcut 1 50
	TEnd

	Type		fem
	  Anim          fem\wm_upcut 1 50
	TEnd
MEnd

Move Firesword_Upcut_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Wep_half

	Type		male
	  Anim          male\wm_upcut 51
	TEnd

	Type		huge
	  Anim          huge\wm_upcut 51
	TEnd

	Type		fem
	  Anim          fem\wm_upcut 51
	TEnd
MEnd


#######################################
###### Torch Blow (sword)   ######
#######################################

Move Sword_TorchBlow_Draw
	Requires	COMBAT WEAPON FIRE BLOW SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Sword_TorchBlow
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          fem\draw_axe 1 30
	TEnd
MEnd

Move Sword_TorchBlow
	Requires	COMBAT WEAPON FIRE BLOW
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Sword_TorchBlow_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          fem\torch_blow 1 84
	TEnd
MEnd

Move Sword_TorchBlow_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Wep_half

	Type		male
	  Anim          fem\torch_blow 85
	TEnd
MEnd

#######################################
###### Torch Slash (sword)   ######
#######################################

Move Sword_TorchSlash_Draw
	Requires	COMBAT WEAPON FIRE SLASH SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Sword_TorchSlash1
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          fem\draw_axe 1 30
	TEnd
MEnd

Move Sword_TorchSlash1
	Requires	COMBAT WEAPON FIRE SLASH
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Sword_TorchSlash1_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          fem\axe_swoop 1 64
	TEnd
MEnd

Move Sword_TorchSlash1_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Wep_half

	Type		male
	  Anim          fem\axe_swoop 65
	TEnd
MEnd

#############################################################

Move Sword_TorchSlash2
	Requires	COMBAT WEAPON FIRE SLASH
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Sword_TorchSlash2_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          fem\axe_reverse_sweep 9 40
	TEnd
MEnd

Move Sword_TorchSlash2_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Wep_half

	Type		male
	  Anim          fem\axe_reverse_sweep 41
	TEnd
MEnd

#############################################################
## Magic Staff Cast
#############################################################

Move MagicStaff_Cast_Draw
	Requires	COMBAT WEAPON CLUB STAFF CAST SPAWNSIDE
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        MagicStaff_Cast
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\firesword_draw 1 25
	TEnd
MEnd

Move MagicStaff_Cast
	Requires	COMBAT WEAPON CLUB STAFF CAST
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        MagicStaff_Cast_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\staff_cast_med 1 54
	TEnd
MEnd

Move MagicStaff_Cast_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Wep_half

	Type		male
	  Anim          male\staff_cast_med 55
	TEnd
MEnd

##################################
## Magic Staff Melee (uppercut) ##
##################################

Move MagicStaff_Melee_Draw
	Requires	COMBAT WEAPON CLUB STAFF ATTACK SPAWNSIDE
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        MagicStaff_Melee
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\firesword_draw 1 25
	TEnd
MEnd

Move MagicStaff_Melee
	Requires	COMBAT WEAPON CLUB STAFF ATTACK
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        MagicStaff_Melee_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\2hclub_upcut 1 30
	TEnd
MEnd

Move MagicStaff_Melee_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        HammerReady_15

	Type		male
	  Anim          male\2hclub_upcut 31
	TEnd
MEnd

###################
###### sword jumpspin firesword  ######
###################

Move Firesword_DrawNSpin
	Requires	COMBAT WEAPON FIERCE SWORD ATTACK WEPSPAWN2
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Firesword_DrawNSpin_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\firesword_drawNspin 1 124
	  PlayFx        FEET\LAND1.FX  68
	TEnd

	Type		huge
	  Anim          huge\firesword_drawNspin 1 124
	  PlayFx        FEET\HUGELAND2.FX  68
	TEnd

	Type		fem
	  Anim          fem\firesword_drawNspin 1 124
	TEnd
MEnd

Move Firesword_DrawNSpin_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Wep_half

	Type		male
	  Anim          male\firesword_drawNspin 125
	TEnd

	Type		huge
	  Anim          huge\firesword_drawNspin 125
	TEnd

	Type		fem
	  Anim          fem\firesword_drawNspin 125
	  PlayFx        FEET\LAND1.FX  68
	TEnd
MEnd

#############################################################

Move Firesword_JumpSpin
	Requires	COMBAT WEAPON FIERCE SWORD ATTACK
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Firesword_JumpSpin_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\firesword_jumpspin 1 80
	  PlayFx        FEET\LAND1.FX 34
	TEnd

	Type		huge
	  Anim          huge\firesword_jumpspin 1 80
	  PlayFx        FEET\HUGELAND2.FX 34
	TEnd

	Type		fem
	  Anim          fem\firesword_jumpspin 1 80
	  PlayFx        FEET\LAND1.FX 34
	TEnd
MEnd

Move Firesword_JumpSpin_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Wep_half

	Type		male
	  Anim          male\firesword_jumpspin 81
	TEnd

	Type		huge
	  Anim          huge\firesword_jumpspin 81
	TEnd

	Type		fem
	  Anim          fem\firesword_jumpspin 81
	TEnd
MEnd

#########################
###### Left Wristshoot  ######
########################

Move LeftWristShoot
	Requires	COMBAT CAST HAND CROSSBOW LOW
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        LeftWristShoot_post
	Priority        5
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          fem\comb_wristshoot_L 1 43
	  PitchStart	12
	  PitchAngle	-0.5
	TEnd


	Type		fem
	  Anim          fem\comb_wristshoot_L 1 45
	  PitchStart	12
	  PitchAngle	-0.5
	TEnd
MEnd

Move LeftWristShoot_post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        CombatReady
	Priority        1

	Type		male
	  Anim          fem\comb_wristshoot_L 46
	TEnd


	Type		fem
	  Anim          fem\comb_wristshoot_L 46
	TEnd
MEnd

##########################
###### Gunslinger Shoot  ######
#########################

Move 1st_Gunslinger_shoot
	Requires	COMBAT WEAPON ATTACK HANDGUN SHOOT SPAWNHIP
	Priority	56
	Interrupts	<AttackIRQ> <HitIRQ> Draw_Gunslinger
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Gunslinger_Shoot
	Interpolate	8
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\gunslinger_shoot 1 31  ##35
	  PitchStart	14
	TEnd
MEnd

Move Gunslinger_Shoot
	Requires	COMBAT WEAPON ATTACK HANDGUN SHOOT
	Priority	5
	Interrupts	<AttackIRQ> <HitIRQ> Draw_Gunslinger
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Gunslinger_Shoot
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\gunslinger_2ndshoot 1 19
	TEnd
MEnd

#Move 1st_Gunslinger_shoot2
#	Requires	COMBAT WEAPON
#	Priority        2
#	Member		<DeathIRQ> <AttackIRQ>
#	NextMove        Gunslinger_Shoot_post
#	Sets            CANTMOVE
#	Flags		PitchToTarget NOSIZESCALE
#
#	Type		male
#	  Anim          male\gunslinger_shoot 37 50
#	TEnd
#MEnd

Move Gunslinger_Shoot_post
	Requires	COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Gunslinger_Shoot_PostCycle
	Priority        2
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\gunslinger_shoot 51 70
	TEnd
MEnd

Move Gunslinger_Shoot_PostCycle
	Requires	COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon.inc
	Flags		Cycle PitchToTarget NOSIZESCALE
	NextMove        postReady_back
	Priority        2

	Type		male
	  Anim          male\gunslinger_shoot 70 95
	TEnd
MEnd

##########################
###### Gunslinger Cone  ######
#########################

Move Gunslinger_Cone_Draw
	Requires	COMBAT WEAPON ATTACK HANDGUN SHOOT CONE SPAWNHIP
	Priority        57
	Interrupts	<AttackIRQ> <HitIRQ> Draw_Gunslinger
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Gunslinger_Cone
	Interpolate	8
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\gunslinger_cone 1 34
	  PitchStart	14
	TEnd
MEnd

Move Gunslinger_Cone
	Requires	COMBAT WEAPON ATTACK HANDGUN SHOOT CONE
	Priority        6
	Interrupts	<AttackIRQ> <HitIRQ> Draw_Gunslinger
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Gunslinger_Cone_Post
	Interpolate	8
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\gunslinger_cone 35 55
	TEnd
MEnd

Move Gunslinger_Cone_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Gunslinger_Cone_PostCycle
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\gunslinger_cone 56 77
	  PitchEnd	60
	TEnd
MEnd

Move Gunslinger_Cone_PostCycle
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	Flags		Cycle

	Type		male
	  Anim          male\gunslinger_cone 80 105
	TEnd
MEnd


###################
###### Hip Shoot)  ######
###################

Move HipShoot
	Requires	COMBAT ATTACK HANDGUN
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        HipShoot_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\pistol_hipshoot 1 35
	TEnd

	Type		Vahzilok
	  Anim          Vahzilok\vah_shoot 1 90
	TEnd
MEnd

Move HipShoot_Post
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        HipShoot_PostCycle

	Type		male
	  Anim          male\pistol_hipshoot 36
	TEnd

	Type		Vahzilok
	  Anim          Vahzilok\vah_shoot 91
	TEnd
MEnd

Move HipShoot_PostCycle
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat_Weapon.inc
	Flags		Cycle
	NextMove        postReady_back
	Scale           0.50

	Type		male
	  Anim          male\wep_cycle 1 31
	TEnd

	Type		Vahzilok
	  Anim          Vahzilok\skel_vah_combat_cycle 1
	TEnd
MEnd

#############################################################
## Pistol Shoot
#############################################################

Move Pistol_Shoot_Draw
	Requires	COMBAT WEAPON ATTACK HANDGUN SPAWNSIDE
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Pistol_Shoot1
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\draw_pistol 1 29
	  PlayFX		NPCFX\PistolDraw_SFX.fx
	TEnd

	Type		huge
	  Anim          male\draw_pistol 1 29
	  PlayFX		NPCFX\PistolDraw_SFX.fx
	TEnd

	Type		fem
	  Anim          male\draw_pistol 1 29
	  PlayFX		NPCFX\PistolDraw_SFX.fx
	TEnd
MEnd

Move Pistol_Shoot1
	Requires	COMBAT WEAPON ATTACK HANDGUN
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Pistol_Shoot1_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\pistol_hipshoot 1 35
	TEnd

	Type		huge
	  Anim          huge\pistol_hipshoot 1 35
	TEnd
MEnd

Move Pistol_Shoot1_Post
	Requires	COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        WepReady
	Priority        2
	Scale           0.50
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\pistol_hipshoot 36
	TEnd

	Type		huge
	  Anim          huge\pistol_hipshoot 36
	TEnd
MEnd

#############################################################

Move Pistol_Shoot2
	Requires	COMBAT WEAPON ATTACK HANDGUN
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Pistol_Shoot2_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\gunshoot 1 34
	TEnd

	Type		huge
	  Anim          huge\pistol_hipshoot 1 34
	TEnd
MEnd

Move Pistol_Shoot2_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Pistol_Shoot2_Post2
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\gunshoot 35 45
	TEnd

	Type		huge
	  Anim          huge\pistol_hipshoot 35 60
	TEnd
MEnd

Move Pistol_Shoot2_Post2
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        WepReady
	Flags		FixArmReg

	Type		male
	  Anim          male\gunshoot 60
	TEnd

	Type		huge
	  Anim          huge\pistol_hipshoot 60
	TEnd
MEnd


#############################################################
## Pistol 3 Shot Burst
#############################################################

Move Pistol_3Burst_Draw
	Requires	COMBAT WEAPON HANDGUN 3SHOTBURST SPAWNSIDE
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Pistol_3Burst
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\draw_pistol 1 29
	  PlayFX		NPCFX\PistolDraw_SFX.fx
	TEnd
MEnd

Move Pistol_3Burst
	Requires	COMBAT WEAPON HANDGUN 3SHOTBURST
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Pistol_3Burst_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\dual_guns_3_shot 1 51
	TEnd
MEnd

Move Pistol_3Burst_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Pistol_3Burst_PostCycle
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\dual_guns_3_shot 52
	  PitchEnd	58
	TEnd
MEnd

Move Pistol_3Burst_PostCycle
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	Flags		Cycle NOSIZESCALE
	Scale           0.50

	Type		male
	  Anim          male\dual_cycle 1 31
	TEnd
MEnd

#############################################################
## 1HAND MACHINEGUN BURST
#############################################################

Move 1Hburst
	Requires	COMBAT WEAPON MACHINEGUN HAND SHOOT
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        1Hburst2
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE NOSIZESCALE

	Type		male
	  Anim          Vahzilok\1Hburst 1 60
	TEnd
MEnd

Move 1Hburst2
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        WepReady

	Type		male
	  Anim          Vahzilok\1Hburst 61
	TEnd
MEnd

###########################
####### Bow (old version) ######
##########################

Move Bow
	Requires	COMBAT WEAPON OLDBOW
	Interrupts	<AttackIRQ> <HitIRQ> <Block>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Bowpost
	Priority        3
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE


	Type		male
	  Anim          male\bow_shoot 1 35
	  PitchAngle	-0.8
	TEnd

	Type		huge
	  Anim          huge\bow_shoot 1 35
	  PitchAngle	-0.8
	TEnd

	Type		fem
	  Anim          fem\bow_shoot 1 35
	  PitchAngle	-0.8
	TEnd
MEnd

Move Bowpost
	Requires	COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Bow_cycle
	Priority        2
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\bow_shoot 36
	  PitchAngle	-0.8
	  PitchEnd	50
	TEnd

	Type		huge
	  Anim          huge\bow_shoot 36
	  PitchAngle	-0.8
	  PitchEnd	50
	TEnd

	Type		fem
	  Anim          fem\bow_shoot 36
	  PitchAngle	-0.8
	  PitchEnd	50
	TEnd
MEnd

#############################################################

Move Bow_half
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	Flags		Cycle
	NextMove        Bow_cycle
	Scale           0.50

	Type		male
	  Anim          male\bow_cycle 15 31
	TEnd

	Type		huge
	  Anim          huge\bow_cycle 15 31
	TEnd

	Type		fem
	  Anim          fem\bow_cycle 15 31
	TEnd
MEnd

#############################################################

Move Bow_cycle
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	Flags		Cycle
	NextMove        PostReady_right
	Scale           0.50

	Type		male
	  Anim          male\bow_cycle 1
	TEnd

	Type		huge
	  Anim          huge\bow_cycle 1
	TEnd

	Type		fem
	  Anim          fem\bow_cycle 1
	TEnd
MEnd

#########################################################
## Undead Slaying Axe
#########################################################

Move GhostAxe_Draw
	Requires	COMBAT WEAPON GHOSTAXE UPPERCUT SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        GhostAxe
	Sets            CANTMOVE
	Flags		NOSIZESCALE FIXARMREG

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

Move GhostAxe
	Requires	COMBAT WEAPON GHOSTAXE UPPERCUT
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        GhostAxe_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE FIXARMREG

	Type		male
	  Anim          male\UndeadAxe_Uppercut 1 30
	TEnd

	Type		huge
	  Anim          huge\UndeadAxe_Uppercut 1 30
	TEnd

	Type		fem
	  Anim          fem\UndeadAxe_Uppercut 1 30
	TEnd
MEnd

Move GhostAxe_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        GhostAxe_Ready_15
	Scale           0.50
	Flags		FIXARMREG

	Type		male
	  Anim          male\UndeadAxe_Uppercut 31
	TEnd

	Type		huge
	  Anim          huge\UndeadAxe_Uppercut 31
	TEnd

	Type		fem
	  Anim          fem\UndeadAxe_Uppercut 31
	TEnd
MEnd

#############################################################

Move GhostAxe_Ready_15
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        GhostAxe_Ready
	Flags		FIXARMREG
	Scale           0.50

	Type		male
	  Anim          male\UndeadAxe_Cycle 15 31
	TEnd

	Type		huge
	  Anim          huge\UndeadAxe_Cycle 15 31
	TEnd

	Type		fem
	  Anim          fem\UndeadAxe_Cycle 15 31
	TEnd
MEnd

#############################################################

Move GhostAxe_Ready
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        postReady_back
	Flags		Cycle FIXARMREG
	Scale           0.50

	Type		male
	  Anim          male\UndeadAxe_Cycle 1 31
	TEnd

	Type		huge
	  Anim          huge\UndeadAxe_Cycle 1 31
	TEnd

	Type		fem
	  Anim          fem\UndeadAxe_Cycle 1 31
	TEnd
MEnd

## ASSASSIN'S STRIKE_BROADSWORD ###########################################################

Move AssassinsStrike_Broadsword_Draw
	Requires	COMBAT WEAPON WINDUPBROADSWORDBACKSTAB SPAWNBACK
	Priority        54
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <MoveIRQ> #<AttackIRQ>
	NextMove	AssassinsStrike_Broadsword_Windup
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\draw_axe 1 30
	TEnd
MEnd

Move AssassinsStrike_Broadsword_Windup
	Requires	COMBAT WEAPON WINDUPBROADSWORDBACKSTAB
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <MoveIRQ> #<AttackIRQ>
	NextMove        AssassinsStrike_Broadsword_Windup2
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\assassinstrike_broadsword 1 61
	TEnd
MEnd

Move AssassinsStrike_Broadsword_Windup2
	Requires	COMBAT WEAPON WINDUPBROADSWORDBACKSTAB
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <MoveIRQ> #<AttackIRQ>
	NextMove        AssassinsStrike_Broadsword
	Flags		Cycle NOSIZESCALE

	Type		male
	  Anim          male\assassinstrike_broadsword 61 61
	TEnd
MEnd

#############################################################

Move AssassinsStrike_Broadsword_Quick_Draw
	Requires	COMBAT WEAPON BROADSWORDBACKSTAB SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <MoveIRQ> #<AttackIRQ>
	NextMove	AssassinsStrike_Broadsword
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\draw_axe 1 30
	TEnd
MEnd

Move AssassinsStrike_Broadsword
	Requires	COMBAT WEAPON BROADSWORDBACKSTAB
	Priority        3
	Interrupts	<AttackIRQ> AssassinsStrike_Broadsword_Windup AssassinsStrike_Broadsword_Windup2
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        AssassinsStrike_Broadsword_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\assassinstrike_broadsword 61 98
	TEnd
MEnd

Move AssassinsStrike_Broadsword_Post
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        WepReady
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\assassinstrike_broadsword 98
	TEnd
MEnd

## PLACATE_BROADSWORD ###########################################################

Move Broadsword_Placate_Draw
	Requires	COMBAT WEAPON MACE PLACATE SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Broadsword_Placate
	Sets		CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\draw_axe 1 30
	TEnd
MEnd

Move Broadsword_Placate
	Requires	COMBAT WEAPON MACE PLACATE
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Broadsword_Placate_Post
	Sets		CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\placate_broadsword 1 45
	TEnd
MEnd

Move Broadsword_Placate_Post
	Requires	COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        WepReady
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\placate_broadsword 45
	TEnd
MEnd

## CREATE ELEMENTAL WEAPON ###########################################################

Move Createwep
	Requires	COMBAT WEAPON WEPSPAWN
	Priority        53
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Createweppost
	Scale           1.5
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\createwep2 1 75
	TEnd

	Type		huge
	  Anim          huge\createwep2 1 75
	TEnd

	Type		fem
	  Anim          fem\createwep2 1 75
	TEnd
MEnd

Move Createweppost
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Lazy_WepReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\createwep2 75
	TEnd

	Type		huge
	  Anim          huge\createwep2 75
	TEnd

	Type		fem
	  Anim          fem\createwep2 75
	TEnd
MEnd

#############################################################
## BRAWLS
#############################################################

## WEAPON BRAWL ###########################################################

Move Wep_Punch
	Requires	COMBAT WEAPON PUNCH
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Wep_Punch2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\wep_punch 1 15
	TEnd

	Type		huge
	  Anim          huge\wep_punch 1 15
	TEnd

	Type		fem
	  Anim          fem\wep_punch 1 15
	TEnd
MEnd

Move Wep_Punch2
	Requires	COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Wep_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\wep_punch 15
	TEnd

	Type		huge
	  Anim          huge\wep_punch 15
	TEnd

	Type		fem
	  Anim          fem\wep_punch 15
	TEnd
MEnd

## MACE BRAWL ###########################################################

Move Mace_Punch
	Requires	COMBAT WEAPON PUNCH MACE
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Mace_Punch2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\wm_chop 7 25
	TEnd

	Type		huge
	  Anim          huge\wm_chop 7 25
	TEnd

	Type		fem
	  Anim          fem\wm_chop 7 25
	TEnd
MEnd

Move Mace_Punch2
	Requires	COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Wep_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\wm_chop 25
	TEnd

	Type		huge
	  Anim          huge\wm_chop 25
	TEnd

	Type		fem
	  Anim          fem\wm_chop 25
	TEnd
MEnd

## GUN BRAWL ###########################################################

Move GunPunch
	Requires	COMBAT WEAPON GUN PUNCH
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        GunPunch2
	Sets            CANTMOVE
	Flags		FixArmReg NoSizeScale

	Type		male
	  Anim          male\gun_punch 1 15
	TEnd

	Type		huge
	  Anim          huge\gun_punch 1 15
	TEnd

	Type		fem
	  Anim          fem\gun_punch 1 15
	TEnd
MEnd

Move GunPunch2
	Requires	COMBAT WEAPON GUN
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        ShotgunReady_15
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\gun_punch 15
	TEnd

	Type		huge
	  Anim          huge\gun_punch 15
	TEnd

	Type		fem
	  Anim          fem\gun_punch 15
	TEnd
MEnd

## PISTOL BRAWL ###########################################################

Move SingleGun_PistolWhip
	Requires	COMBAT WEAPON HANDGUN PUNCH
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        SingleGun_PistolWhip2
	Sets            CANTMOVE
	SticksOnChild   BACKTOREADY
	Flags		NoSizeScale

	Type		male
	  Anim          male\Thug_PistolWhipR 1 20
	TEnd
MEnd

Move SingleGun_PistolWhip2
	Requires	COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        PISTOLS_Ready
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\Thug_PistolWhipR 20
	TEnd
MEnd


#############################################################

Move Draw_Carnival_Staff
	Requires	COMBAT WEAPON CARNIVALSTAFF SPAWNSIDE
	Priority        55
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        WepReady
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\firesword_draw 1
	TEnd
MEnd

#############################################################
## Carnival of Light - Radiant Burst
#############################################################
Move Radiant_Burst_Pre
	Requires	COMBAT WEAPON CARNIVALSTAFF ATTACK ALTERNATE
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Radiant_Burst_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\staff_cast_radiant_burst 1 40
	TEnd

MEnd

Move Radiant_Burst_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        WepReady

	Type		male
	  Anim          male\staff_cast_radiant_burst 41
	TEnd

MEnd

###################
###### Lighstaff  ######
###################

Move Lighstaff_Upcut
	Requires	COMBAT WEAPON CARNIVALSTAFF UPPERCUT ALTERNATE
	Priority        6
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Lighstaff_Upcut_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\wm_upcut 1 45
	TEnd

	Type		huge
	  Anim          huge\wm_upcut 1 45
	TEnd

	Type		fem
	  Anim          fem\wm_upcut 1 45
	TEnd
MEnd

Move Lighstaff_Upcut_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Lazy_WepReady_half

	Type		male
	  Anim          male\wm_upcut 46
	TEnd

	Type		huge
	  Anim          huge\wm_upcut 46
	TEnd

	Type		fem
	  Anim          fem\wm_upcut 46
	TEnd
MEnd

#############################################################
## light burst Staff Cast
#############################################################

Move Light_MagicStaff_Cast
	Requires	COMBAT WEAPON CARNIVALSTAFF UPPERCUT CAST
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Light_MagicStaff_Cast_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\staff_cast_med 1 54
	TEnd
MEnd

Move Light_MagicStaff_Cast_Post
	Requires	COMBAT WEAPON
	Priority        2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Wep_half

	Type		male
	  Anim          male\staff_cast_med 55
	TEnd
MEnd

#############################################################

Include sequencers\player\Weapons\Weapons_DualWield.inc
Include sequencers\player\WEAPONS\Weapons_Shotgun.inc
Include sequencers\player\WEAPONS\Weapons_Hammer.inc
Include sequencers\player\WEAPONS\Weapons_Whip.inc
Include sequencers\player\WEAPONS\Weapons_TwoHanded.inc
Include sequencers\player\WEAPONS\Weapons_BeamRifle.inc