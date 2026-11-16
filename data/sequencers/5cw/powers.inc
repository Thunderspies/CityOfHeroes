#############################################################
## POWERS
#############################################################

########################
######## Arm Guns Cone ###
#######################

Move ArmGun_Cone
	Requires	COMBAT WEAPON DUAL ARMGUNCONE
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        ArmGun_Cone_2
	Flags		CANTMOVE NoSizeScale PitchToTarget

	Type		5cw
	  Anim          male\armguns_singlecone_quick 1 45
	  PitchAngle	0.1
	TEnd
MEnd

Move ArmGun_Cone_2
	Requires	COMBAT WEAPON DUAL
	Priority        3
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        Fake_DualReady
	Flags		PitchToTarget

	Type		5cw
	  Anim          male\armguns_singlecone_quick 45
	  PitchEnd	61
	  PitchAngle	0.1
	TEnd
MEnd

########################
######## Arm Guns Burst ###
#######################

Move ArmGun_burst
	Requires	COMBAT WEAPON DUAL ARMGUNBURST
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        ArmGun_burst_2
	Flags		CANTMOVE NoSizeScale PitchToTarget

	Type		5cw
	  Anim          male\armguns_burst 1 50
	  PitchAngle	0.3
	TEnd
MEnd

Move ArmGun_burst_2
	Requires	COMBAT WEAPON DUAL
	Priority        4
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        Fake_DualReady
	Flags		PitchToTarget

	Type		5cw
	  Anim          male\armguns_burst 50
	  PitchEnd	55
	  PitchAngle	0.3
	TEnd

MEnd

#####################
###### Chest Cannon ####
#####################

Move ChestCannon
	Requires	CHESTCANNON
	Priority        1
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        ChestCannon_post
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		5cw
	  Anim          5cw\chestcannon 1 119
	  PlayFx        FEET\MM_Bot_FF_Left.FX  16
	  PlayFx        FEET\MM_Bot_FF_Right.FX  27
	  PlayFx        FEET\MM_Bot_FF_Left.FX  117
	  PitchEnd	77
	TEnd
MEnd

Move ChestCannon_post
	Include sequencers\Members\Combat.inc
	NextMove        Ready
	Flags		nosizescale

	Type		5cw
	  Anim          5cw\chestcannon 119
	  PlayFx        FEET\MM_Bot_FF_Right.FX  18
	TEnd
MEnd


###################
###### Telekinesis_quick  ######
###################

Move Telequick
	Requires	COMBAT ATTACK TELEKINESIS
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Telequickpost
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		5cw
	  Anim          male\telekinesis 2 35
	  PitchStart	9
	  PitchAngle	0.5
	TEnd
MEnd

Move Telequickpost
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		PitchToTarget

	Type		5cw
	  Anim          male\telekinesis 35
	  PitchEnd	97
	  PitchAngle	0.5
	TEnd
MEnd

###################
###### Arm Blast  ######
###################

Move ArmBlast
	Requires	ROBOTBLAST
	Priority        1
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        ArmBlast_post
	Sets            CANTMOVE

	Type		5cw
	  Anim          5cw\blast 1 55
	TEnd
MEnd

Move ArmBlast_post
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        Ready
	Flags		NoSizeScale

	Type		5cw
	  Anim          5cw\blast 55
	TEnd
MEnd


#################################
###### Arm Burst (rapid shots)  ######
#################################

Move ArmBurst
	Requires	ROBOTBURST
	Priority        1
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        ArmBurst_post
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		5cw
	  Anim          5cw\burst 1 55
	  PitchStart	9
	  PitchAngle	0.2
	TEnd
MEnd

Move ArmBurst_post
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        Ready
	Flags		NoSizeScale PitchToTarget

	Type		5cw
	  Anim          5cw\burst 55
	  PitchEnd	65
	  PitchAngle	0.2
	TEnd
MEnd

###################
###### Psionics  ######
###################

Move Psionics
	Requires	COMBAT PSIONIC
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        PostPsionics
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		5cw
	  Anim          male\psionic 2 89
	  PitchStart	13
	  PitchAngle	0.2
	TEnd
MEnd

Move PostPsionics
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Interpolate     0

	Type		5cw
	  Anim          male\psionic 162
	TEnd
MEnd

###################
###### Psionic Blast  ######
###################

Move PsiBlast
	Requires	COMBAT PSIONICBLAST
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        PsiBlast2
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		5cw
	  Anim          male\psionic 2 50
	  PitchStart	13
	  PitchAngle	0.2
	TEnd
MEnd

Move PsiBlast2
	Requires	COMBAT
	Priority        1
	Member		<DeathIRQ>
	NextMove        PsiBlast3
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		5cw
	  Anim          male\psionic 96 114
	  PitchAngle	0.2
	TEnd
MEnd

Move PsiBlast3
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	NextMove        PsiBlastpost
	Flags		PitchToTarget

	Type		5cw
	  Anim          male\psionic 114 162
	  PitchAngle	0.2
	TEnd
MEnd

Move PsiBlastpost
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady

	Type		5cw
	  Anim          male\psionic 162
	TEnd
MEnd


################################
###### Windblast  ######
################################

Move Windblast
	Requires	COMBAT WINDBLAST
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Windblastpost
	Sets            CANTMOVE

	Type		5cw
	  Anim          male\windblast 1 70
	TEnd
MEnd

Move Windblastpost
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half

	Type		5cw
	  Anim          male\windblast 70
	TEnd
MEnd


###################
###### Scream ######
###################

Move Scream
	Requires	COMBAT SCREAM
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Scream_post
	Sets            CANTMOVE

	Type		5cw
	  Anim          male\scream 10 58
	TEnd
MEnd

Move Scream_post
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	NextMove        Scream_cycle

	Type		5cw
	  Anim          male\scream 59
	TEnd
MEnd

Move Scream_cycle
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	Flags		Cycle
	NextMove        postReady_back

	Type		5cw
	  Anim          5cw\monster_cycle 1
	TEnd
MEnd


###################
###### Radius Attack  ######
###################

Move RadiusAttack
	Requires	COMBAT RADIUSATTACK
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        PostRadAttack
	Sets            CANTMOVE

	Type		5cw
	  Anim          5cw\radiusattack 1 30
	TEnd
MEnd

Move PostRadAttack
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	NextMove        RadAttackcycle

	Type		5cw
	  Anim          5cw\radiusattack 30
	TEnd
MEnd

Move RadAttackcycle
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	Flags		Cycle
	NextMove        CombatReady
	Scale           0.50

	Type		5cw
	  Anim          5cw\combat_cycle 1 31
	TEnd
MEnd

###############################################
###### Malta Healer Trans Mek (the healer)  ######
###############################################

Move Malta_Transform_heal
	Requires	HEAL MENACE STRONG
	Priority        5
	Interrupts	<AttackIRQ>
	NextMove        Malta_Transform_heal_cycle
	Sets            NOCOLLISION
	Flags		NotSelectable

	Type		5cw
	  Anim          5cw\malta_merge 1 67
	  PlayFx        ENEMYFX\Malta\Titan_transform_A.FX 32
	  PlayFx        ENEMYFX\Malta\TitanTransformFX\TitanTransform.fx 0 CONSTANT
	TEnd
MEnd

Move Malta_Transform_heal_cycle
	Flags		Cycle NotSelectable
	Sets            NOCOLLISION DIENOW

	Type		5cw
	  Anim          5cw\pre_poison 34 39
	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	TEnd
MEnd


###########################################
###### Sholder Missles              ######
###########################################

Move Missles
	Requires	ROCKETLAUNCHER BLAST FIERCE
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Missles2
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		5cw
	  Anim          5cw\missles 1 75
	  PitchStart	5
	TEnd
MEnd

Move Missles2
	Include sequencers\Members\Combat.inc
	NextMove        Ready
	Flags		NoSizeScale PitchToTarget

	Type		5cw
	  Anim          5cw\missles 75
	  PitchEnd	81
	TEnd
MEnd

########################
###### Quad Guns     ######
########################

Move QuadGuns
	Requires	MACHINEGUN BLAST FIERCE
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        QuadGuns2
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		5cw
	  Anim          5cw\quadguns 1 70
	  PitchStart	12
	TEnd
MEnd

Move QuadGuns2
	Include sequencers\Members\Combat.inc
	NextMove        Ready
	Interpolate     0
	Flags		NoSizeScale PitchToTarget

	Type		5cw
	  Anim          5cw\quadguns 119
	 PitchEnd	129
	TEnd
MEnd


###################
###### mouth_eye blast  ######
###################

Move Mouth
	Requires	COMBAT HEADCAST
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        PostMouth
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		5cw
	  Anim          male\eye 1 49
	  PitchStart	1
	  PitchAngle	0.1
	TEnd
MEnd


Move PostMouth
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	NextMove        Cast_Cycle

	Type		5cw
	  Anim          male\eye 49 60
	TEnd
MEnd


###################
###### Cast  ######
###################

Move Cast
	Requires	COMBAT CAST
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Castpost
	Priority        2
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		5cw
	  Anim          male\palmcast 1 30
	  PitchStart	5
	  PitchAngle	-0.5
	TEnd
MEnd

Move Castpost
	Include sequencers\Members\Combat.inc
	NextMove        Cast_Cycle

	Type		5cw
	  Anim          male\palmcast 30 45
	TEnd
MEnd

Move Cast_Cycle
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	Flags		Cycle
	NextMove        postReady_back
	Scale           0.50

	Type		5cw
	  Anim          male\combat_cycle 1 31
	TEnd
MEnd

################################
###### Wep shoot (Rikti Style) ######
###############################

Move Shoot
	Requires	COMBAT WEAPON CAST
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Shoot2
	Sets		CANTMOVE

	Type		5cw
	  Anim          5cw\rikti_wepshoot 1 47
	TEnd
MEnd

Move Shoot2
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        R_WepReady

	Type		5cw
	  Anim          5cw\rikti_wepshoot 47
	TEnd
MEnd

Move R_WepReady
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat_Weapon.inc
	Flags		Cycle
	Scale           0.50

	Type		5cw
	  Anim          5cw\rikti_cycle 1 31
	TEnd
MEnd

###################
###### Hip Shoot)  ######
###################

Move HipShoot
	Requires	COMBAT ATTACK HANDGUN
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove	HipShoot2
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		5cw
	  Anim          5cw\pistol_hipshoot 1 35
	  PitchAngle	0
	TEnd
MEnd

Move HipShoot2
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        WepReady
	Flags		PitchToTarget

	Type		5cw
	  Anim          5cw\pistol_hipshoot 35
	  PitchAngle	0
	  PitchEnd	40
	TEnd
MEnd

Move WepReady
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat_Weapon.inc
	Flags		Cycle
	Scale           0.50

	Type		5cw
	  Anim          5cw\wep_cycle 1 31
	TEnd
MEnd

###################
###### QUICK SniperBlast (hands)  ######
###################

Move SniperBlast
	Requires	COMBAT QUICK SNIPERBLAST
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        SniperBlast2
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		5cw
	  Anim          5cw\sniperblast 1 100
	PitchAngle	0
	  PitchEnd	81
	TEnd
MEnd

Move SniperBlast2
	Requires	COMBAT
	Priority        1
	Member		<ReactIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <BlockIRQ>
	NextMove        CombatReady_15

	Type		5cw
	  Anim          5cw\sniperblast 100
	TEnd
MEnd

###########################################
###### Flamethrower (long) ######
###########################################

Move Flamethrower
	Requires	COMBAT FLAMETHROWER
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Flamethrower2
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		5cw
	  Anim          5cw\flamethrower 1 185
	TEnd
MEnd

Move Flamethrower2
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		PitchToTarget

	Type		5cw
	  Anim          5cw\flamethrower 185
	  PitchAngle	0
	  PitchEnd	176
	TEnd
MEnd


###################
###### Brawling ######
###################

Move Brawl
	Requires	COMBAT PUNCH
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Brawlpost

	Type		5cw
	  Anim          5cw\frisbee 5 30
	TEnd
MEnd

Move Brawlpost
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady_15

	Type		5cw
	  Anim          5cw\frisbee 30
	TEnd
MEnd

#############################################################

Move Brawl_R
	Requires	COMBAT PUNCH
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Brawlpost_R

	Type		5cw
	  Anim          5cw\claw_low 50 73
	TEnd

	Type		Clockwork_King
	  Anim          5cw\claw_low 50 73
	  PlayFX	Powers\Claws\clawsStreaks15.fx
	TEnd

	Type		Clockwork_Assembler
	  Anim          5cw\claw_low 50 73
	  PlayFX	Powers\Claws\clawsStreaks15.fx
	TEnd
MEnd

Move Brawlpost_R
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	NextMove        Brawlcycle_15

	Type		5cw
	  Anim          5cw\claw_low 73
	TEnd
MEnd

#############################################################

Move Brawl_L
	Requires	COMBAT PUNCH
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Brawlpost_L

	Type		5cw
	  Anim          5cw\claw_high 68 97
	TEnd

	Type		clockwork_King
	  Anim          5cw\claw_high 68 97
	  PlayFX	Powers\Claws\clawsStreaks15.fx
	TEnd

	Type		clockwork_Assembler
	  Anim          5cw\claw_high 68 97
	  PlayFX	Powers\Claws\clawsStreaks15.fx
	TEnd
MEnd

Move Brawlpost_L
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	NextMove        Brawlcycle_15

	Type		5cw
	  Anim          5cw\claw_high 97
	TEnd
MEnd

#############################################################

Move Brawlcycle_15
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Brawlcycle
	Priority        1
	Scale           0.50

	Type		5cw
	  Anim          5cw\claw_cycle 15 31
	TEnd
MEnd

#############################################################

Move Brawlcycle
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		Cycle
	NextMove        postReady_back
	Priority        1
	Scale           0.50

	Type		5cw
	  Anim          5cw\claw_cycle 1
	TEnd
MEnd

######################
###### 1 Hand Swipe  ######
#####################

Move 1H_Swipe
	Requires	COMBAT HAND SWIPE
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        1H_cycle
	Sets            CANTMOVE

	Type		5cw
	  Anim          Vahzilok\vah_swipeL 1
	TEnd
MEnd

#############################################################

Move 1H_Swipe_L
	Requires	COMBAT HAND SWIPE
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        1H_cycle
	Sets            CANTMOVE

	Type		5cw
	  Anim          Vahzilok\vah_smash 1
	TEnd
MEnd

#############################################################

Move 1H_cycle
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	Flags		Cycle
	NextMove        postReady

	Type		5cw
	  Anim          Vahzilok\skel_vah_combat_cycle 1
	TEnd
MEnd

###################
###### Energy Torrent ######
###################

Move Energy_Torrent
	Requires	COMBAT ENERGYTORRENT
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        E_Torrent_post
	Sets            CANTMOVE

	Type		5cw
	  Anim          5cw\energy_torrent 1 64
	TEnd
MEnd

Move E_Torrent_post
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady_15
	Scale           0.50

	Type		5cw
	  Anim          5cw\energy_torrent 65
	TEnd
MEnd

###################
###### 2Hand Cast  ######
###################

Move 2HandCast
	Requires	COMBAT 2HAND CAST
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        2HandCastpost
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		5cw
	  Anim          5cw\2hand_cast 1 50
	  PitchAngle	0
	  PitchEnd	46
	TEnd
MEnd

Move 2HandCastpost
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady_15

	Type		5cw
	  Anim          5cw\2hand_cast 50
	TEnd
MEnd

###################
###### magic throw  ######
###################

Move magicthrow
	Requires	COMBAT MAGICTHROW
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        magicthrowpost
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		5cw
	  Anim          5cw\frisbee 1 30
	  PitchAngle	0
	  PitchStart	11
	TEnd
MEnd

Move magicthrowpost
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady_15

	Type		5cw
	  Anim          5cw\frisbee 30 70
	  PitchAngle	0
	  PitchEnd	25
	TEnd
MEnd

###################
###### Radius Effect  ######
###################

Move RadiusEffect
	Requires	COMBAT RADIUSEFFECT
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        RadiusEffePost
	Sets            CANTMOVE

	Type		5cw
	  Anim          5cw\radiuseffect 1 60
	TEnd
MEnd

Move RadiusEffePost
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	NextMove        RadEffecycle

	Type		5cw
	  Anim          5cw\radiuseffect 60
	TEnd
MEnd

Move RadEffecycle
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	Flags		Cycle
	NextMove        CombatReady
	Scale           0.50

	Type		5cw
	  Anim          5cw\combat_cycle 1 31
	TEnd
MEnd

###################
###### Ground Summon  ######
###################

Move G_Summon
	Requires	COMBAT GROUNDSUMMON
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        G_Summonpost
	Sets            CANTMOVE

	Type		5cw
	  Anim          5cw\ground_summon 1 104
	TEnd
MEnd

Move G_Summonpost
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady_15
	Scale           0.50

	Type		5cw
	  Anim          5cw\ground_summon 104
	TEnd
MEnd

###################
###### Levitate  ######
###################

Move Levitate
	Requires	COMBAT LEVITATE
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Levitate_cycle
	Sets            CANTMOVE

	Type		5cw
	  Anim          5cw\levitate 1 54
	TEnd
MEnd

Move Levitate_cycle
	Requires	COMBAT LEVITATE
	Priority        2
	Member		<DeathIRQ>
	NextMove        Levitatepost
	Sets            CANTMOVE

	Type		5cw
	  Anim          5cw\levitate 54 94
	TEnd
MEnd

Move Levitatepost
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady_15

	Type		5cw
	  Anim          5cw\levitate 94
	TEnd
MEnd

################################
###### Rapidblast  ######
################################

Move Rapid
	Requires	COMBAT RAPIDBLAST
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Rapidpost
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		5cw
	  Anim          5cw\energy_rapid 1 59
	  PitchAngle	0
	TEnd
MEnd

Move Rapidpost
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		PitchToTarget

	Type		5cw
	  Anim          5cw\energy_rapid 60
	  PitchAngle	0
	  PitchEnd	67
	TEnd
MEnd

################################
###### DarkBlast or Megablast  ######
################################

Move Mega
	Requires	COMBAT MEGABLAST
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Megapost
	Sets            CANTMOVE

	Type		5cw
	  Anim          5cw\megablast 1 110
	TEnd
MEnd

Move Megapost
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady_15
	Scale           0.50

	Type		5cw
	  Anim          5cw\megablast 110
	TEnd
MEnd

###################
###### Stomp  ######
###################

Move Stomp
	Requires	COMBAT STOMP
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Stomp2
	Sets            CANTMOVE

	Type		5cw
	  Anim          5cw\stomp 1 62
	  PlayFX	ENEMYFX\BanishedPantheon\BanishedPantheon_Revamped\Sentinel_FrigidStomp_VOX.fx
	TEnd
MEnd

Move Stomp2
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half

	Type		5cw
	  Anim          5cw\stomp 62
	TEnd
MEnd

###################
###### Doorsmash Kick ######
###################

Move DoorKick
	Requires	COMBAT STRONG KICK
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        DoorKick_post
	Sets            CANTMOVE

	Type		5cw
	  Anim          5cw\doorsmash_kick 1 54
	TEnd
MEnd

Move DoorKick_post
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half

	Type		5cw
	  Anim          5cw\doorsmash_kick 54
	TEnd
MEnd

###################
###### Stop cast ######
###################

Move Stop
	Requires	COMBAT STOPCAST
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Stop_cycle
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		5cw
	  Anim          5cw\stop 1 32
	  PitchAngle	0.5
	TEnd
MEnd

Move Stop_cycle
	Requires	COMBAT
	Priority        1
	Member		<DeathIRQ>
	NextMove        Stop_post
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		5cw
	  Anim          5cw\stop 33 60
	  PitchAngle	0.5
	TEnd
MEnd

Move Stop_post
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady_15
	Scale           0.50
	Flags		PitchToTarget

	Type		5cw
	  Anim          5cw\stop 60
	  PitchAngle	0.5
	  PitchEnd	64
	TEnd
MEnd
########################
######## Air Arm Guns Cone ###
#######################

Move A_ArmGun_Cone
	Requires	FLY COMBAT WEAPON DUAL ARMGUNCONE
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        A_ArmGun_Cone_2
	Flags		CANTMOVE NoSizeScale PitchToTarget

	Type		5cw
	  Anim          male\air_armguns_singlecone_quick 1 45
	  PitchAngle	0.1
	TEnd
MEnd

Move A_ArmGun_Cone_2
	Requires	FLY COMBAT WEAPON DUAL
	Priority        4
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        A_Fake_DualReady
	Flags		PitchToTarget

	Type		5cw
	  Anim          male\air_armguns_singlecone_quick 45
	  PitchEnd	61
	  PitchAngle	0.1
	TEnd
MEnd

########################
######## Air Arm Guns Burst ###
#######################

Move A_ArmGun_burst
	Requires	FLY COMBAT WEAPON DUAL ARMGUNBURST
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        A_ArmGun_burst_2
	Flags		CANTMOVE NoSizeScale PitchToTarget

	Type		5cw
	  Anim          male\air_armguns_burst 1 50
	  PitchAngle	0.3
	TEnd
MEnd

Move A_ArmGun_burst_2
	Requires	FLY COMBAT WEAPON DUAL
	Priority        5
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        A_Fake_DualReady
	Flags		PitchToTarget

	Type		5cw
	  Anim          male\air_armguns_burst 50
	  PitchEnd	55
	  PitchAngle	0.3
	TEnd
MEnd

#####################
###### air Chest Cannon ####
#####################

Move A_ChestCannon
	Requires	FLY CHESTCANNON
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        A_ChestCannon_post
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		5cw
	  Anim          5cw\air_chestcannon 1 120
	  PitchEnd	77
	TEnd
MEnd

Move A_ChestCannon_post
	Requires	FLY
	Priority        1
	Include sequencers\Members\Combat_Air.inc
	NextMove        flyready
	Flags		nosizescale

	Type		5cw
	  Anim          5cw\air_chestcannon 120
	TEnd
MEnd

######################
###### Air 1 Hand Swipe  ######
#####################

Move A_1H_Swipe
	Requires	FLY COMBAT HAND SWIPE
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        A_1H_post
	Sets            CANTMOVE

	Type		5cw
	  Anim          male\air_1h_smash 1 30
	TEnd
MEnd

Move A_1H_post
	Requires	FLY COMBAT
	Priority        2
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flycycle_half

	Type		5cw
	  Anim          male\air_1h_smash 31
	TEnd
MEnd

###################
###### Air Brawling ######
###################

Move AirBrawl_R
	Requires	FLY COMBAT PUNCH
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        AirBrawl_Rpost
	Sets            CANTMOVE

	Type		5cw
	  Anim          male\air_brawl_high 1 24
	TEnd
MEnd

Move AirBrawl_Rpost
	Requires	FLY COMBAT
	Priority        2
	Include sequencers\Members\Combat_Air.inc
	NextMove        A_Brawlcycle_15

	Type		5cw
	  Anim          male\air_brawl_high 24 71
	TEnd
MEnd

#############################################################

Move AirBrawl_L
	Requires	FLY COMBAT PUNCH
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        AirBrawl_Lpost
	Sets            CANTMOVE

	Type		5cw
	  Anim          male\air_brawl_high 78 100
	TEnd
MEnd

Move AirBrawl_Lpost
	Requires	FLY COMBAT
	Priority        2
	Include sequencers\Members\Combat_Air.inc
	NextMove        A_Brawlcycle_15

	Type		5cw
	  Anim          male\air_brawl_high 100
	TEnd
MEnd

#############################################################

Move A_Brawlcycle_15
	Requires	FLY COMBAT
	Priority        2
	Include sequencers\Members\Combat_Air.inc
	NextMove        AirBrawlcycle
	Scale           0.50

	Type		5cw
	  Anim          male\air_brawl_cycle 16 31
	TEnd
MEnd

#############################################################

Move AirBrawlcycle
	Requires	FLY COMBAT
	Priority        2
	Include sequencers\Members\Combat_Air.inc
	Flags		Cycle
	NextMove        C_PreFall
	Scale           0.50

	Type		5cw
	  Anim          male\air_brawl_cycle 1 31
	TEnd
MEnd

#################################
###### Air Arm Burst (rapid shots)  ######
#################################

Move A_ArmBurst
	Requires	FLY ROBOTBURST
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        A_ArmBurst_post
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		5cw
	  Anim          5cw\air_burst 1 55
	  PitchStart	5
	  PitchEnd	36
	  PitchAngle	0.2
	TEnd
MEnd

Move A_ArmBurst_post
	Requires	FLY
	Priority        1
	Include sequencers\Members\Combat_Air.inc
	NextMove        flyready
	Flags		NoSizeScale

	Type		5cw
	  Anim          5cw\air_burst 55
	TEnd
MEnd


###################
###### Air Arm Blast  ######
###################

Move A_ArmBlast
	Requires	FLY ROBOTBLAST
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        A_ArmBlast_post
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		5cw
	  Anim          5cw\air_blast 1 55
	  PitchStart	5
	  PitchEnd	36
	  PitchAngle	0.2
	TEnd
MEnd

Move A_ArmBlast_post
	Requires	FLY
	Priority        1
	Include sequencers\Members\Combat_Air.inc
	NextMove        flyReady
	Flags		NoSizeScale

	Type		5cw
	  Anim          5cw\air_blast 55
	TEnd
MEnd

###########################################
###### Air Sholder Missles              ######
###########################################

Move A_Missles
	Requires	FLY ROCKETLAUNCHER BLAST FIERCE
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        A_Missles2
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		5cw
	  Anim          5cw\air_missile 1 75
	  PitchStart	5
	TEnd
MEnd

Move A_Missles2
	Requires	FLY
	Priority        1
	Include sequencers\Members\Combat_Air.inc
	NextMove        flyReady
	Flags		NoSizeScale PitchToTarget

	Type		5cw
	  Anim          5cw\air_missile 75
	  PitchEnd	94
	TEnd
MEnd

########################
###### Air Quad Guns     ######
########################

Move A_QuadGuns
	Requires	FLY MACHINEGUN BLAST FIERCE
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        A_QuadGuns2
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		5cw
	  Anim          5cw\air_quadguns 1 70
	  PitchStart	1
	TEnd
MEnd

Move A_QuadGuns2
	Requires	FLY
	Priority        1
	Include sequencers\Members\Combat_Air.inc
	NextMove        flyReady
	Interpolate     0
	Flags		NoSizeScale PitchToTarget

	Type		5cw
	  Anim          5cw\air_quadguns 119
	 PitchEnd	117
	TEnd
MEnd

################################
###### Air Wep shoot (Rikti Style) ######
###############################

Move A_Shoot
	Requires	FLY COMBAT WEAPON CAST
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        A_Shoot2

	Type		5cw
	  Anim          5cw\air_rikti_shoot 1 47
	TEnd
MEnd

Move A_Shoot2
	Requires	FLY
	Priority        1
	Include sequencers\Members\Combat_Air.inc
	NextMove        flyReady

	Type		5cw
	  Anim          5cw\air_rikti_shoot 47
	TEnd
MEnd

###########################################
###### Air Flamethrower (long) ######
###########################################

Move A_Flamethrower
	Requires	FLY COMBAT FLAMETHROWER
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        A_Flamethrower2
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		5cw
	  Anim          5cw\air_flamethrower 1 185
	  PitchEnd	162
	TEnd
MEnd

Move A_Flamethrower2
	Requires	FLY COMBAT
	Priority        2
	Include sequencers\Members\Combat_Air.inc
	NextMove        flyReady

	Type		5cw
	  Anim          5cw\air_flamethrower 185
	TEnd
MEnd

###################
###### Air Telekinesis_quick  ######
###################

Move A_Telequick
	Requires	FLY COMBAT ATTACK TELEKINESIS
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        A_Telequickpost
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		5cw
	  Anim          male\air_telekinesis 2 35
	  PitchStart	9
	  PitchAngle	0.5
	TEnd
MEnd

Move A_Telequickpost
	Requires	FLY
	Priority        1
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flycycle
	Flags		PitchToTarget

	Type		5cw
	  Anim          male\air_telekinesis 35
	  PitchEnd	97
	  PitchAngle	0.5
	TEnd
MEnd

###################
###### air Stop cast ######
###################

Move A_Stop
	Requires	FLY COMBAT STOPCAST
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        A_Stop_post
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		5cw
	  Anim          male\air_stop 1 60
	  PitchAngle	0.5
	TEnd
MEnd

Move A_Stop_post
	Requires	FLY COMBAT
	Priority        2
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flycycle_half
	Scale           0.50
	Flags		PitchToTarget  NoSizeScale

	Type		5cw
	  Anim          male\air_stop 60
	  PitchEnd	64
	  PitchAngle	0.5
	TEnd
MEnd

#########################################
###### Air Quick SniperBlast (hands)  ######
#########################################

Move A_Q_SniperBlast
	Requires	FLY COMBAT QUICK SNIPERBLAST
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        A_Q_SniperBlast2
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		5cw
	  Anim          huge\air_sniperblast 1 100
	  PitchAngle	-0.5
	  PitchEnd	94
	TEnd
MEnd

Move A_Q_SniperBlast2
	Requires	FLY COMBAT
	Priority        2
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flycycle_half

	Type		5cw
	  Anim          huge\air_sniperblast 100
	TEnd
MEnd

###############################################
###### Malta Finished Transformed Mek (pre poison)  ######
###############################################

Move Pre_Malta_Transformed
	Requires	COMBAT POISON STRONG
	Priority        5
	Interrupts	<AttackIRQ>
	NextMove        Malta_Transformed
	Interpolate	0
	Sets            CANTMOVE

	Type		5cw
	  Anim          5cw\pre_poison 1 7
	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	TEnd
MEnd

Move Malta_Transformed
	NextMove        Malta_Transformed_2
	Sets            CANTMOVE

	Type		5cw
	  Anim          5cw\pre_poison 43 97
	  PlayFx        GENERIC\quick_ENTITYFADEIN.FX 0 CONSTANT
	TEnd
MEnd

Move Malta_Transformed_2
	Member		<Ready> <ReactIRQ> <DeathIRQ> <AttackIRQ>
	NextMove	Malta_Transformed_cycle
	Scale		0.5


	Type		5cw
	  Anim          5cw\pre_poison 96
	TEnd
MEnd

Move Malta_Transformed_cycle
	Member		<Ready> <ReactIRQ> <DeathIRQ> <AttackIRQ>
	Flags		Cycle
	Scale		0.5

	Type		5cw
	  Anim          5cw\combat_cycle 1
	TEnd
MEnd

#########################################
###### Banished Pantheon Sentinel  ######
#########################################

###################
###### Sentinel_Voracious_Maw ######
###################

Move Sentinel_Voracious_Maw
	Requires	COMBAT GIANT CAST
	Priority        3
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Sentinel_Voracious_Maw_post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		5cw
	  Anim          5cw\sentinel_voracious_maw 1 72
	  PlayFX	ENEMYFX\BanishedPantheon\BanishedPantheon_Revamped\Sentinel_VoraciousMaw_VOX.fx
	  PitchAngle	0.3
	  Pitchend	50
	TEnd

MEnd

Move Sentinel_Voracious_Maw_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		5cw
	  Anim          5cw\sentinel_voracious_maw 73
	TEnd

MEnd

###################
###### Sentinel_Frigid_Grasp  ######
###################

Move Sentinel_Frigid_Grasp
	Requires	COMBAT GIANT 2HAND CAST
	Priority        4
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Sentinel_Frigid_Grasp_post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		5cw
	  Anim          5cw\sentinel_frigid_grasp 1 80
	  PlayFX	ENEMYFX\BanishedPantheon\BanishedPantheon_Revamped\Sentinel_FrigidGrasp_VOX.fx
	   PitchAngle	0.3
	   Pitchend	80
	TEnd

MEnd

Move Sentinel_Frigid_Grasp_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		5cw
	  Anim          5cw\sentinel_frigid_grasp 81
	TEnd

MEnd

###################
###### Sentinel_Enervating_Strike  ######
###################

Move Sentinel_Enervating_Strike
	Requires	COMBAT GIANT 2HAND SPEEDPUNCH
	Priority        4
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Sentinel_Enervating_Strike_post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		5cw
	  Anim          5cw\sentinel_enervating_strike 1 99
	  PlayFX	ENEMYFX\BanishedPantheon\BanishedPantheon_Revamped\Sentinel_Enervation_VOX.fx
	  PitchAngle	0.3
	   Pitchend	50
	TEnd

MEnd

Move Sentinel_Enervating_Strike_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		5cw
	  Anim          5cw\sentinel_enervating_strike 100
	TEnd

MEnd

###################
###### Sentinel Impale ######
###################

Move Sentinel_Impale
	Requires	COMBAT GIANT 2HAND SKYSUMMON
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Sentinel_Impale_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		5cw
	  Anim          5cw\sentinel_impale 1 60
	  PlayFX	ENEMYFX\BanishedPantheon\BanishedPantheon_Revamped\Sentinel_Impale_VOX.fx
	TEnd

MEnd

Move Sentinel_Impale_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale ReqInputs

	Type		5cw
	  Anim          5cw\sentinel_impale 60 73
	TEnd

MEnd

###################
###### Ball Cast  ######
###################

Move BallCast
	Requires	COMBAT BALLCAST
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        BallCastpost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		5cw
	  Anim          5cw\ball_cast 1 70
	  PitchEnd	60
	TEnd

MEnd

Move BallCastpost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		5cw
	  Anim          5cw\ball_cast 70
	TEnd

MEnd

###################
###### ThrowProjectile  ######
###################

Move ThrowProjectile_Pre
	Requires	COMBAT THROWKNIFE ROOTED
	Priority        3
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        ThrowProjectile
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		5cw
	  Anim          5cw\throwknife 1 21
	TEnd
MEnd

Move ThrowProjectile
	Requires	COMBAT
	Member		<DeathIRQ> <AttackIRQ> <MoveIRQ>
	NextMove        ThrowProjectile_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		0.5

	Type		5cw
	  Anim          5cw\throwknife 21 30
	TEnd
MEnd

Move ThrowProjectile_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		ReqInputs
	Scale		0.7

	Type		5cw
	  Anim          5cw\throwknife 31
	TEnd
MEnd

##############################################################
##  Nemesis Claw Drill
###############################################################

Move Nemesis_Claw_Drill_Pre

	Requires	COMBAT SPIN STAB ENDGAME
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	Flags		Cantmove nosizescale
	SticksOnChild	COMBAT SPIN STAB ENDGAME
	NextMove        Nemesis_Claw_Drill

	Type		Nemesis_Heavy
	  Anim          5cw\claw_drill 1 30
  	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_ServoHi.fx 20
   	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_FootSoft.fx 40
   	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_Smash.fx 45
	TEnd
MEnd

Move Nemesis_Claw_Drill
	Include sequencers\Members\Combat.inc
	NextMove        Nemesis_Claw_Drill_post
	Flags		Cantmove nosizescale
	SticksOnChild	COMBAT SPIN STAB ENDGAME


	Type		Nemesis_Heavy
	  Anim          5cw\claw_drill 31 110
	TEnd
MEnd

Move Nemesis_Claw_Drill_post
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	SticksOnChild	COMBAT SPIN STAB ENDGAME
	flags		nosizescale

	Type		Nemesis_Heavy
	  Anim          5cw\claw_drill 111
	TEnd
MEnd
#########################
###################
###### Nemesis Monstrosity Gatling Gun Burst (hands)  ######
###################

Move Nemesis_GatlingGunBurst
	Requires	COMBAT HIGH SNIPERBLAST ENDGAME
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Nemesis_GatlingGunBurst_post
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		Nemesis_Heavy
	  Anim          5cw\nemesis_gatlinggunburst 1 90
  	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_ServoHi.fx 1
   	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_Foot.fx 14
	PitchAngle	0
	  PitchEnd	81
	TEnd


MEnd

Move Nemesis_GatlingGunBurst_post
	Requires	COMBAT
	Priority        1
	Member		<ReactIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <BlockIRQ>
	NextMove        CombatReady

	Type		Nemesis_Heavy
	  Anim          5cw\nemesis_gatlinggunburst 91
  	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_ServoLo.fx 10
   	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_FootSoft.fx 30
	TEnd
MEnd
#############################################################

###################
######Nemesis Monstrosity Gatling Gun Sweep ######
###################

Move Nemesis_GatlingGunSweep
	Requires	COMBAT MAGICTHROW ENDGAME HIGH
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Nemesis_GatlingGunSweep_post
	Sets            CANTMOVE
	Flags		nosizescale

	Type		Nemesis_Heavy
	  Anim          5cw\sweeping_ray 1 180
  	  PlayFX		ENEMYFX\Praetorian\Warworks\Goliath_ServoLo.fx 1
  	  PlayFX		ENEMYFX\Praetorian\Warworks\Goliath_Foot.fx 40
  	  PlayFX		ENEMYFX\Praetorian\Warworks\Goliath_ServoHi.fx 65
  	  PlayFX		ENEMYFX\Praetorian\Warworks\Goliath_ServoLo.fx 130
  	  PlayFX		ENEMYFX\Praetorian\Warworks\Goliath_Foot.fx 178
	TEnd
MEnd



Move Nemesis_GatlingGunSweep_post
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		nosizescale

	Type		Nemesis_Heavy
	  Anim          5cw\sweeping_ray 180 198
	TEnd
MEnd

##########################################################
#Nemsis Monstrosity Pandora Beam
##########################################################


Move Nemesis_PandoraBeam_Tell
	Requires	COMBAT BOOSTUP ENDGAME LOW
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Nemesis_PandoraBeam_attack
	Flags		nosizescale

	Type		Nemesis_Heavy
	  Anim          5cw\nemesis_monstrosity_pandora_beam 1 124
  	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_ServoLo.fx 1
  	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_ServoHi.fx 55
	TEnd

MEnd

Move Nemesis_PandoraBeam_attack
	Requires	COMBAT BLAST ENDGAME
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Nemesis_PandoraBeam_post
	Flags		nosizescale

	Type		Nemesis_Heavy
	  Anim          5cw\nemesis_monstrosity_pandora_beam 125 218
  	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_ServoAttack.fx 30
	TEnd

MEnd


Move Nemesis_PandoraBeam_post
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		nosizescale

	Type		Nemesis_Heavy
	  Anim          5cw\nemesis_monstrosity_pandora_beam 219
  	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_ServoHi.fx 1
   	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_Foot.fx 33
	TEnd

MEnd



