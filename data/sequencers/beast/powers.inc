#############################################################
## powers.inc
#############################################################

#############################################################
###### Body Chunk Toss  ######
#########################

Move BodyChunk_Toss
	Requires	COMBAT CHUNKTOSS
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        BodyChunk_Toss2
	Flags		 CANTMOVE

	Type		Beast
	  Anim          Beast\bodychunk_toss 1 80
	TEnd
MEnd

Move BodyChunk_Toss2
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	NextMove        BodyChunk_Toss_cycle

	Type		Beast
	  Anim          Beast\bodychunk_toss 80
	TEnd
MEnd

Move BodyChunk_Toss_cycle
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half_fingers

	Type		Beast
	  Anim          beast\goblin_dual_cycle 1
	TEnd
MEnd

###################
###### Moment O Glory  ######
###################

Move Glory
	Requires	MOMENTGLORY
	Priority	1
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Glory2
	Sets            CANTMOVE

	Type		Beast
	  Anim          male\glory 1 76
	TEnd
MEnd

Move Glory2
	Include sequencers\Members\Combat.inc
	NextMove        PostGloryCycle_15

	Type		Beast
	  Anim          male\glory 76
	TEnd
MEnd

#############################################################

Move PostGloryCycle_15
	Include sequencers\Members\Combat.inc
	NextMove        PostGloryCycle
	Scale           0.50

	Type		Beast
	  Anim          male\combat_cycle 15 31
	TEnd
MEnd

#############################################################

Move PostGloryCycle
	Include sequencers\Members\Combat.inc
	Flags		Cycle
	Scale           0.50

	Type		Beast
	  Anim          male\combat_cycle 1 31
	TEnd
MEnd

###################
###### Poison Mode (MR.Spikey)  ######
###################

Move PrePoison
	Requires	COMBAT POISON
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        PrePoison2
	Sets            CANTMOVE

	Type		Beast
	  Anim          Beast\pre_poison 1 88
	TEnd
MEnd

Move PrePoison2
	Requires	COMBAT POISON
	Priority	2
	Include sequencers\Members\Combat.inc
	NextMove        Combat_fingers
	Scale           0.50

	Type		Beast
	  Anim          Beast\pre_poison 88
	TEnd
MEnd

###################
###### Blinding Cast  ######
###################

Move blinding
	Requires	COMBAT BLINDING
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        blindingpost
	Sets            CANTMOVE

	Type		Beast
	  Anim          Beast\blinding_cast 1 88
	TEnd
MEnd

Move blindingpost
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half_fingers

	Type		Beast
	  Anim          Beast\blinding_cast 88
	TEnd
MEnd

#############################
###### Quick_Blinding Cast  ###
#############################

Move Quickblinding
	Requires	COMBAT BLINDING LOW STRONG
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        blindingpost
	Sets            CANTMOVE

	Type		Beast
	  Anim          Beast\blinding_cast 39 88
	TEnd
MEnd

################################
###### DarkBlast or Megablast  ######
################################

Move Mega
	Requires	COMBAT MEGABLAST
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Megapost
	Flags		PitchToTarget
	Sets            CANTMOVE

	Type		Beast
	  Anim          Beast\mega_blast 1 110
	TEnd
MEnd


Move Megapost
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	NextMove        CombatBlend
	Scale           0.50

	Type		Beast
	  Anim          Beast\mega_blast 110
	TEnd
MEnd

################################
###### Rapidblast  ######
################################

Move Rapid
	Requires	COMBAT RAPIDBLAST
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Rapidpost
	Flags		PITCHTOTARGET
	Sets            CANTMOVE

	Type		Beast
	  Anim          Beast\rapid_cast 1 60
	TEnd
MEnd

Move Rapidpost
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady

	Type		Beast
	  Anim          Beast\rapid_cast 60
	TEnd
MEnd

#############################
###### Toung Strike   ######
############################

Move ToungStrike
	Requires	TOUNGSTRIKE
	Priority	1
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        ToungStrike_post
	Sets            CANTMOVE

	Type		Beast
	  Anim          Beast\toung_strike 1 32
	TEnd
MEnd

Move ToungStrike_post
	Include sequencers\Members\Combat.inc
	NextMove        Ready

	Type		Beast
	  Anim          Beast\toung_strike 32
	TEnd
MEnd

#############################
###### claw speed (no draw) ######
############################

Move clawspeed
	Requires	COMBAT CLAW CLAWSPEED
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        clawspeed_post
	Sets            CANTMOVE

	Type		Beast
	  Anim          male\claw_speed 1 55
	TEnd
MEnd

Move clawspeed_post
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	NextMove        CombatBlend

	Type		Beast
	  Anim          male\claw_speed 55
	TEnd
MEnd

###################
###### mouth_eye blast  ######
###################

Move Mouth
	Requires	COMBAT HEADCAST
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        PostMouth
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		Beast
	  Anim          male\eye 1 50
	  PitchStart	1
	  PitchAngle	0.1
	TEnd
MEnd

Move PostMouth
	Include sequencers\Members\Combat.inc
	NextMove        CombatBlend

	Type		Beast
	  Anim          male\eye 50 60
	TEnd
MEnd

##########################
###### Horn Ram   ####
##########################

Move HornRam
	Requires	HEAD HEADPAIN SWIPE ATTACK
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        HornRam_post
	Sets            CANTMOVE

	Type		male
	  Anim          Beast\horn_ram 1 50
	TEnd
MEnd

Move HornRam_post
	Include sequencers\Members\Combat.inc
	NextMove        Ready
	Flags		NoSizeScale

	Type		male
	  Anim          Beast\horn_ram 50
	TEnd
MEnd

##########################
###### Throw Root   ####
##########################

Move TossRoot
	Requires	SHORTTOSS STRONG BODY
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        TossRoot_post
	Sets            CANTMOVE

	Type		male
	  Anim          Beast\goblin_toss_root 1 85
	TEnd
MEnd

Move TossRoot_post
	Include sequencers\Members\Combat.inc
	NextMove        Ready

	Type		male
	  Anim          Beast\goblin_toss_root 85
	TEnd
MEnd

##########################
###### Howl Cast      ####
##########################

Move Howl_cast
	Requires	COMBAT YELL STRONG ATTACK LOW
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Howl_cast_post
	Sets            CANTMOVE

	Type		Beast
	  Anim          Beast\howl 1 50
	TEnd
MEnd

Move Howl_cast_post
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	NextMove        Ready_A

	Type		Beast
	  Anim          Beast\howl 50
	TEnd
MEnd

#################################
###### Sonic Yell  ######
#################################

Move SonicYell
	Requires	COMBAT YELL STRONG ATTACK
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        SonicYell_post
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		Beast
	  Anim          male\sonic_yell 1 40
	TEnd
MEnd

Move SonicYell_post
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half

	Type		Beast
	  Anim          male\sonic_yell 40
	TEnd
MEnd

#################################
###### Sing Cast  ######
#################################

Move SingCast
	Requires	COMBAT YELL YES CAST
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        SingCast_post
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		Beast
	  Anim          male\sing_cast 1 68
	TEnd
MEnd

Move SingCast_post
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady

	Type		Beast
	  Anim          male\sing_cast 68
	TEnd
MEnd

##########################
###### Hoof\Paw Attack  ####
##########################

Move Hoof_R
	Requires	CLAW SWIPE
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Hoof_R_post
	Sets            CANTMOVE

	Type		Beast
	  Anim          Beast\paw_R 5 25
	  PlayFX	Generic\Roar.fx
	TEnd
MEnd

Move Hoof_R_post
	Include sequencers\Members\Combat.inc
	NextMove        Ready_A

	Type		Beast
	  Anim          Beast\paw_R 25
	TEnd
MEnd

#############################################################

Move Hoof_L
	Requires	Claw Swipe
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Hoof_L_post
	Sets            CANTMOVE

	Type		Beast
	  Anim          Beast\paw_L 1 25
	TEnd
MEnd

Move Hoof_L_post
	Include sequencers\Members\Combat.inc
	NextMove        Ready_B

	Type		Beast
	  Anim          Beast\paw_L 25
	TEnd
MEnd

#############################################################
## CLAW SWIPE
#############################################################

Move ClawSwipe_Right
	Requires	COMBAT CLAW ATTACK
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	ClawSwipe_Right_Post
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		Beast
	  Anim		huge\claw_mid2 1 20
	  PlayFX		NPCFX\NPC_WhooshBig_SFX.fx 5
	TEnd
MEnd

Move ClawSwipe_Right_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove	ClawSwipe_PostCycle
	Flags		NoSizeScale ReqInputs

	Type		Beast
	  Anim		huge\claw_mid2 21 31
	TEnd
MEnd

#############################################################

Move ClawSwipe_Left
	Requires	COMBAT CLAW ATTACK
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	ClawSwipe_Left_Post
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		Beast
	  Anim		huge\claw_mid2 40 60
	  PlayFX		NPCFX\NPC_WhooshBig_SFX.fx 5
	TEnd
MEnd

Move ClawSwipe_Left_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	ClawSwipe_PostCycle
	Flags		NoSizeScale ReqInputs

	Type		Beast
	  Anim		huge\claw_mid2 61 71
	TEnd
MEnd

#############################################################

Move ClawSwipe_PostCycle
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		Cycle NoSizeScale
	NextMove	postReady_back
	Scale		0.50

	Type		huge
	  Anim		huge\claw_cycle	1 31
	TEnd
MEnd

###################
###### Ball Cast  ######
###################

Move BallCast
	Requires	COMBAT BALLCAST
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        BallCastpost
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		Beast
	  Anim          Beast\ball_cast 1 70
	  PitchEnd	60
	TEnd
MEnd

Move BallCastpost
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half

	Type		Beast
	  Anim          Beast\ball_cast 70
	TEnd
MEnd

################################
###### Windblast  ######
################################

Move Windblast
	Requires	COMBAT WINDBLAST
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Windblastpost
	Sets            CANTMOVE

	Type		Beast
	  Anim          Beast\windblast 1 70
	TEnd
MEnd

Move Windblastpost
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half

	Type		Beast
	  Anim          Beast\windblast 70
	TEnd
MEnd

###################
###### Ground Summon  ######
###################

Move G_Summon
	Requires	COMBAT GROUNDSUMMON
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        G_Summonpost
	Sets            CANTMOVE

	Type		Beast
	  Anim          male\teleport 1 60
	TEnd
MEnd

Move G_Summonpost
	Include sequencers\Members\Combat.inc
	NextMove        CombatBlend

	Type		Beast
	  Anim          male\teleport 60 144
	TEnd
MEnd


###################
###### Sky Summon  ######
###################

Move S_Summon
	Requires	COMBAT SKYSUMMON
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        S_Summonpost
	Sets            CANTMOVE

	Type		Beast
	  Anim          Beast\sky_summon 1 60
	TEnd
MEnd

Move S_Summonpost
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady

	Type		Beast
	  Anim          Beast\sky_summon 60
	TEnd
MEnd

###################
###### Stop cast ######
###################

Move Stop
	Requires	COMBAT STOPCAST
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Stop_cycle
	Flags		PitchToTarget
	Sets            CANTMOVE

	Type		Beast
	  Anim          Beast\stop 1 32
	  PitchAngle	0.5
	TEnd
MEnd

Move Stop_cycle
	Requires	COMBAT
	Priority	1
	Member		<DeathIRQ>
	NextMove        Stop_post
	Flags		PitchToTarget
	Sets            CANTMOVE

	Type		Beast
	  Anim          Beast\stop 33 60
	  PitchAngle	0.5
	TEnd
MEnd

Move Stop_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Priority	1
	Scale           0.50
	Flags		PitchToTarget

	Type		Beast
	  Anim          Beast\stop 60
	  PitchEnd	64
	  PitchAngle	0.5
	TEnd
MEnd

###################
###### magic cast Left  ######
###################

Move Magic_L
	Requires	COMBAT MAGIC CAST
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Magic_Lpost
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		Beast
	  Anim          Beast\magic_L 1 30
	  PitchStart	10
	  PitchAngle	0.5
	TEnd
MEnd

Move Magic_Lpost
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Flags		PitchToTarget

	Type		Beast
	  Anim          Beast\magic_L 30
	  PitchEnd	60
	  PitchAngle	0.5
	TEnd
MEnd

###################
###### Heal cast  ######
###################

Move Heal
	Requires	COMBAT HEAL
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Healpost
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		Beast
	  Anim          Beast\heal 1 68
	  PitchStart	8
	  PitchEnd	63
	  PitchAngle	0.1
	TEnd
MEnd

Move Healpost
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady

	Type		Beast
	  Anim          Beast\heal 68
	TEnd
MEnd

###################
###### Air Heal cast  ######
###################

Move airHeal_cast
	Requires	FLY COMBAT HEAL
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	airHealpost_cast
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		Beast
	  Anim          male\air_heal 1 35
	  PitchStart	8
	  PitchAngle	0.1
	TEnd
MEnd

Move airHealpost_cast
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flyready
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		Beast
	  Anim          male\air_heal 35
	  PitchEnd	63
	  PitchAngle	0.1
	TEnd

MEnd

#####################
## Air Burn Summon ##
#####################

Move A_BurnSummon_Pre_cast
	Requires	FLY COMBAT BURNSUMMON
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_BurnSummon_cast
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\air_terrify 1 45
	TEnd


MEnd

Move A_BurnSummon_cast
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flyready
	Flags		NoSizeScale ReqInputs

	Type		Beast
	  Anim          male\air_terrify 45
	TEnd
MEnd

###################
###### air Terrify ######
###################

Move A_Terrify_cast
	Requires	FLY COMBAT TERRIFY
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Terrify_post_cast
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		Beast
	  Anim          male\air_terrify 1 60
	TEnd

MEnd

Move A_Terrify_post_cast
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove	Fake_C_flycycle_Half
	Scale           0.50
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		Beast
	  Anim          male\air_terrify 60
	  PitchEnd	63
	TEnd

MEnd

###########################
###### Taunt_C		######
###########################

Move Taunt_C
	Requires	COMBAT TAUNT VARIATIONC
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Taunt_C2
	Interpolate     8
	Sets            CANTMOVE

	Type		Beast
	  Anim          Beast\paw_R 1 45
	  PlayFX	Generic\Roar.fx
	TEnd
MEnd

Move Taunt_C2
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	NextMove        Taunt_C_cycle

	Type		Beast
	  Anim          Beast\paw_R 45
	TEnd
MEnd

Move Taunt_C_cycle
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	Flags		Cycle
	NextMove        postReady_back

	Type		Beast
	  Anim          Beast\Skel_ready 1
	TEnd
MEnd

###################
###### Steal power  ######
###################

Move Steal
	Requires	COMBAT STEAL
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Stealpost
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		Beast
	  Anim          male\graspearth 1 58
	  PitchStart	8
	  PitchAngle	0.5
	TEnd
MEnd

Move Stealpost
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady

	Type		Beast
	  Anim          male\graspearth 58
	TEnd
MEnd

###################
###### jumpNsmash ######
###################

Move jumpNsmash
	Requires	COMBAT JUMPNSMASH
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        jumpNsmash_post
	Sets            CANTMOVE

	Type		Beast
	  Anim          Beast\goblin_jump_stab 1 50
	TEnd
MEnd

Move jumpNsmash_post
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	NextMove        Ready
	Flags		NoSizeScale

	Type		Beast
	  Anim          Beast\goblin_jump_stab 50
	TEnd
MEnd

###################
###### Smash Cast Radius  ######
###################

Move SmashCast
	Requires	COMBAT SMASH
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        SmashCastpost
	Sets            CANTMOVE

	Type		Beast
	  Anim          Beast\2Hsmash 1 44
	TEnd
MEnd

Move SmashCastpost
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half_fingers
	Scale           0.80

	Type		Beast
	  Anim          Beast\2Hsmash 44
	TEnd
MEnd

###################
###### 1 Hand Smash  ######
###################

Move 1HSmash_R
	Requires	COMBAT PUNCH SMASH
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        1HSmash2_R
	Sets            CANTMOVE

	Type		Beast
	  Anim          Beast\1Hsmash 12 36
	TEnd
MEnd

Move 1HSmash2_R
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	NextMove        1HSmashpost_R

	Type		Beast
	  Anim          Beast\1Hsmash 36 58
	TEnd

	Type		Magmite
	  Anim          Beast\1Hsmash 36 58
	  PlayFx	Volumes\Lava\ShallowSplash\Encounter_ShallowSplash.fx 1
	TEnd
MEnd

Move 1HSmashpost_R
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half_fingers
	Scale           0.50

	Type		Beast
	  Anim          Beast\1Hsmash 58 82
	TEnd
MEnd

#############################################################

Move 1HSmash_L
	Requires	COMBAT PUNCH SMASH
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        1HSmash2_L
	Sets            CANTMOVE

	Type		Beast
	  Anim          Beast\1Hsmash 96 120
	TEnd
MEnd

Move 1HSmash2_L
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	NextMove        1HSmashpost_L

	Type		Beast
	  Anim          Beast\1Hsmash 120 140
	TEnd

	Type		Magmite
	  Anim          Beast\1Hsmash 120 140
	  PlayFx	Volumes\Lava\ShallowSplash\Encounter_ShallowSplash.fx 1
	TEnd
MEnd

Move 1HSmashpost_L
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half_fingers
	Scale           0.50

	Type		Beast
	  Anim          Beast\1Hsmash 140
	TEnd
MEnd

###################
###### Radius Attack  ######
###################

Move RadiusAttack
	Requires	COMBAT RADIUSATTACK
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        PostRadAttack
	Sets            CANTMOVE

	Type		Beast
	  Anim          male\radiusattack 1 30
	TEnd
MEnd

Move PostRadAttack
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	NextMove        RadAttackcycle

	Type		Beast
	  Anim          male\radiusattack 30
	TEnd
MEnd

Move RadAttackcycle
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	Flags		Cycle
	NextMove        CombatReady
	Scale           0.50

	Type		Beast
	  Anim          male\combat_cycle 1 31
	TEnd
MEnd

#################################
## Air Radius Effect - No Root ##
#################################

Move A_RadiusEffect_Pre_NoRoot
	Requires	FLY RADIUSEFFECT NOTROOTED #COMBAT
	Priority        14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_RadiusEffect_NoRoot
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\air_radiuseffect 1 2
	TEnd
MEnd

Move A_RadiusEffect_NoRoot
	Requires	FLY
	Include sequencers\Members\Combat.inc
	NextMove        C_flycycle
	Flags		NoSizeScale ReqInputs

	Type		Beast
	  Anim          male\air_radiuseffect 2
	TEnd
MEnd
###################
###### magic throw  ######
###################

Move magicthrow
	Requires	COMBAT MAGICTHROW
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        magicthrowpost
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		Beast
	  Anim          Beast\frisbee 1 30
	  PitchAngle	0.3
	  PitchStart	11
	  PitchEnd	23
	TEnd

	Type		Crystal
	  Anim          male\frisbee 1 30
	  PitchAngle	0.3
	  PitchStart	11
	  PitchEnd	23
	TEnd
MEnd

Move magicthrowpost
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half

	Type		Beast
	  Anim          Beast\frisbee 30
	TEnd

	Type		Crystal
	  Anim          male\frisbee 30
	TEnd
MEnd

#############################################################

Move A_magicthrow
	Requires	FLY COMBAT MAGICTHROW
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_magicthrowpost
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		Beast
	  Anim          male\air_frisbee 1 30
	  PitchAngle	0.3
	  PitchStart	11
	  PitchEnd	23
	TEnd
MEnd

Move A_magicthrowpost
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flycycle_half

	Type		Beast
	  Anim          male\air_frisbee 30
	TEnd
MEnd

###################
###### Fling  ######
###################

Move Fling_R
	Requires	COMBAT MAGICTHROW STRONG
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Fling_R_post
	Sets            CANTMOVE
	Interpolate	8

	Type		Beast
	  Anim          Beast\fling 1 30
	TEnd
MEnd

Move Fling_R_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Scale		1.5

	Type		Beast
	  Anim          Beast\fling 30
	TEnd
MEnd

#############################################################

Move A_Fling
	Requires	FLY COMBAT MAGICTHROW STRONG
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Fling_L_post
	Interpolate	8
	Sets            CANTMOVE

	Type		Beast
	  Anim          male\air_baseball_punch 23 44
	TEnd
MEnd

Move A_Fling_L_post
	Requires	FLY COMBAT
	Priority	2
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flycycle_half
	Scale           0.50

	Type		Beast
	  Anim          male\air_baseball_punch 44 81
	TEnd
MEnd

###################
###### Telekinesis_quick  ######
###################

Move Telequick
	Requires	COMBAT ATTACK TELEKINESIS
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Telequickpost
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		Beast
	  Anim          male\telekinesis 2 96
	TEnd
MEnd

Move Telequickpost
	Requires	COMBAT
	Priority	1
	Member		<ReactIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <BlockIRQ> <IRQEmotesCombat>
	NextMove        Tele_cycle
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\telekinesis 96
	TEnd
MEnd

Move Tele_cycle
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	Flags		Cycle
	NextMove        PostCombatReady
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\combat_cycle 1 31
	TEnd
MEnd


###################
###### Brawling ######
###################

Move Brawl_R
	Requires	COMBAT PUNCH
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Brawlpost_R
	Sets            CANTMOVE

	Type		Beast
	  Anim          Beast\swipe_mid 1 15
	  PlayFx        POWERS\POWERPUNCH\PUNCHWHOOSH2.FX
	TEnd

	Type		Crystal
	  Anim          male\claw_mid2 1 15
	TEnd
MEnd

Move Brawlpost_R
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	NextMove        Brawlcycle
	scale		0.6
	Flags		NoSizeScale

	Type		Beast
	  Anim          Beast\swipe_mid 15 31
	TEnd

	Type		Crystal
	  Anim          male\claw_mid2 15 31
	TEnd
MEnd

#############################################################

Move AirBrawl_R
	Requires	FLY COMBAT PUNCH
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        AirBrawl_Rpost
	Sets            CANTMOVE

	Type		Beast
	  Anim          male\air_brawl_high 1 24
	TEnd
MEnd

Move AirBrawl_Rpost
	Requires	FLY COMBAT
	Priority	2
	Include sequencers\Members\Combat_Air.inc
	NextMove        A_Brawlcycle_15

	Type		Beast
	  Anim          male\air_brawl_high 24 71
	TEnd
MEnd

#############################################################

Move Brawl_L
	Requires	COMBAT PUNCH
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Brawlpost_L
	Sets            CANTMOVE

	Type		Beast
	  Anim          Beast\swipe_mid 40 66
	  PlayFx        POWERS\POWERPUNCH\PUNCHWHOOSH2.FX
	TEnd

	Type		Crystal
	  Anim          male\claw_mid2 40 54
	TEnd
MEnd

Move Brawlpost_L
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	NextMove        Brawlcycle
	scale		0.6
	Flags		NoSizeScale

	Type		Beast
	  Anim          Beast\swipe_mid 66 70
	TEnd

	Type		Crystal
	  Anim          male\claw_mid2 64 71
	TEnd
MEnd

#############################################################

Move AirBrawl_L
	Requires	FLY COMBAT PUNCH
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        AirBrawl_Lpost
	Sets            CANTMOVE

	Type		Beast
	  Anim          male\air_brawl_high 78 100
	TEnd
MEnd

Move AirBrawl_Lpost
	Requires	FLY COMBAT
	Priority	2
	Member		<ReactIRQ> <DeathIRQ> <FlyCombatAttack> <BlockIRQ> <AttackIRQ>
	NextMove        A_Brawlcycle_15

	Type		Beast
	  Anim          male\air_brawl_high 100
	TEnd
MEnd

#############################################################
## BRAWL CYCLE
#############################################################

Move Brawlcycle_15
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	NextMove        Brawlcycle
	Scale           0.50
	Flags		NoSizeScale

	Type		Beast
	  Anim          Beast\claw_cycle 15 31
	TEnd

	Type		Crystal
	  Anim          male\claw_cycle 15 31
	TEnd
MEnd

#############################################################

Move Brawlcycle
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	Flags		Cycle NoSizeScale
	NextMove        postReady_back
	Interpolate	8
	Scale           0.50

	Type		Beast
	  Anim          Beast\claw_cycle 1 31
	TEnd

	Type		Crystal
	  Anim          male\claw_cycle 1 31
	TEnd
MEnd

#############################################################

Move AirBrawlcycle
	Requires	FLY COMBAT
	Priority	2
	Include sequencers\Members\Combat_Air.inc
	Flags		Cycle
	NextMove        C_PreFall
	Scale           0.50

	Type		Beast
	  Anim          male\air_brawl_cycle 1 31
	TEnd
MEnd

Move A_Brawlcycle_15
	Requires	FLY COMBAT
	Priority	2
	Include sequencers\Members\Combat_Air.inc
	NextMove        AirBrawlcycle
	Scale           0.50

	Type		Beast
	  Anim          male\air_brawl_cycle 15 31
	TEnd
MEnd

###################
###### Blow Cast  ######
###################

Move Blow
	Requires	COMBAT BLOW
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Blowpost
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		Beast
	  Anim          Beast\blow_cast 1 80
	TEnd
MEnd

Move Blowpost
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half_fingers
	Scale		0.5
	Flags		NoSizeScale

	Type		Beast
	  Anim          Beast\blow_cast 80
	TEnd
MEnd

#################################
###### Quick spit  ######
#################################

Move Spit
	Requires	COMBAT BLOW CAST QUICK
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Spitpost
	Priority	4
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		Beast
	  Anim          Beast\quickspit 1 35
	TEnd
MEnd

Move Spitpost
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	NextMove        SpitCycle
	Flags		NoSizeScale

	Type		Beast
	  Anim          Beast\quickspit 35 48
	TEnd
MEnd

Move SpitCycle
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	Flags		Cycle

	Type		Beast
	  Anim		Beast\quickspit 48 84
	TEnd
MEnd

###################
###### Energy Torrent ######
###################

Move Energy_Torrent
	Requires	COMBAT ENERGYTORRENT
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        E_Torrent_post
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		Beast
	  Anim          Beast\energy_torrent 1 65
	TEnd
MEnd

Move E_Torrent_post
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half_fingers
	Flags		NoSizeScale

	Type		Beast
	  Anim          Beast\energy_torrent 65
	TEnd
MEnd

###################
###### 2Hand Cast  ######
###################

Move 2HandCast
	Requires	COMBAT 2HAND CAST
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        2HandCastpost
	Priority	3
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		Beast
	  Anim          Beast\2hand_cast 1 50
	TEnd
MEnd

Move 2HandCastpost
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half_fingers
	Flags		NoSizeScale

	Type		Beast
	  Anim          Beast\2hand_cast 50
	TEnd
MEnd

###################
###### Throw Knife  ######
###################

Move Throwknife
	Requires	COMBAT THROWKNIFE
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Throwknife_post
	Flags		PitchToTarget

	Type		Beast
	  Anim          Beast\knife_throw 1 24
	TEnd
MEnd

Move Throwknife_post
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half_fingers
	Flags		NoSizeScale

	Type		Beast
	  Anim          Beast\knife_throw 24
	TEnd
MEnd

#################################################

###################
###### Air Throw Knife  ######
###################

Move A_ThrowKnife_Pre
	Requires	FLY COMBAT THROWKNIFE
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_ThrowKnife
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		Beast
	  Anim          male\air_knifethrow 1 21
	  PitchAngle	0.5
	TEnd
MEnd

Move A_ThrowKnife
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_ThrowKnife_Post
	Scale           0.5
	Flags		NOSIZESCALE ReqInputs

	Type		Beast
	  Anim          male\air_knifethrow 21 31
	TEnd

MEnd

Move A_ThrowKnife_Post
	Requires	FLY COMBAT
	Priority	12
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flyready
	Flags		ReqInputs
	Scale		0.7

	Type		Beast
	  Anim          male\air_knifethrow 31
	TEnd

MEnd

###################
###### Stomp  ######
###################

Move Stomp
	Requires	COMBAT STOMP
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Stomp2
	Sets            CANTMOVE

	Type		Beast
	  Anim          Beast\stomp 1 62
	TEnd

	Type		Crystal
	  Anim          male\stomp 1 62
	TEnd
MEnd

Move Stomp2
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half_fingers
	Flags		NoSizeScale

	Type		Beast
	  Anim          Beast\stomp 62
	TEnd

	Type		Crystal
	  Anim          male\stomp 62
	TEnd
MEnd


###################
###### Cast  ######
###################

Move Cast
	Requires	COMBAT CAST
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Castpost
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		Beast
	  Anim          Beast\palmcast 1 30
	  PitchStart	5
	  PitchAngle	-0.5
	TEnd

	Type		Crystal
	  Anim          male\palmcast 1 30
	  PitchStart	5
	  PitchAngle	-0.5
	TEnd
MEnd

Move Castpost
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half_fingers
	Flags		NoSizeScale

	Type		Beast
	  Anim          Beast\palmcast 30 45
	TEnd

	Type		Crystal
	  Anim          male\palmcast 30 45
	TEnd
MEnd

###################
###### Short Toss ######
###################

Move ShortToss
	Requires	COMBAT SHORTTOSS
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        ShortToss_post
	Sets            CANTMOVE

	Type		Beast
	  Anim          Beast\short_toss 1 115
	TEnd

	Type		Crystal
	  Anim          male\short_toss 1 115
	TEnd
MEnd

Move ShortToss_post
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Scale           0.50
	Flags		NoSizeScale

	Type		Beast
	  Anim          Beast\short_toss 115
	TEnd

	Type		Crystal
	  Anim          Beast\short_toss 115
	TEnd
MEnd

###################
###### Levitate  ######
###################

Move Levitate
	Requires	COMBAT LEVITATE
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Levitate_cycle
	Sets            CANTMOVE

	Type		Beast
	  Anim          Beast\levitate 1 54
	TEnd
MEnd

Move Levitate_cycle
	Requires	COMBAT LEVITATE
	Priority	2
	Member		<DeathIRQ>
	NextMove        Levitatepost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		Beast
	  Anim          Beast\levitate 55 94
	TEnd
MEnd

Move Levitatepost
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Flags		NoSizeScale

	Type		Beast
	  Anim          Beast\levitate 94
	TEnd
MEnd

###################
###### Boost_up  ######
###################

Move Boostup
	Requires	BOOSTUP
	Priority	1
	Interrupts	<ReactIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Boostuppost

	Type		Beast
	  Anim          Beast\windup_power 1 65
	TEnd
MEnd

Move Boostuppost
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady

	Type		Beast
	  Anim          Beast\windup_power 65
	TEnd
MEnd

###############################################
###### Merge Natterlings (the healer)  ######
###############################################

Move Transform_heal
	Requires	HEAL MENACE STRONG
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Transform_heal_cycle
	Sets            NOCOLLISION CANTMOVE
	Flags		NotSelectable
	SticksOnChild   Test1

	Type		Beast
	  Anim          Beast\fuse 1 13
	  PlayFx        ENEMYFX\Rularuu\Natterling\Natterling_transform_A.FX 1
	  PlayFx        ENEMYFX\Rularuu\Natterling\FX_Natterling_fuse.fx 1
	  PlayFx        ENEMYFX\Rularuu\Natterling\FX_Natterling_fuse_B.fx 1
	TEnd
MEnd

Move Transform_heal_cycle
	Flags		Cycle NotSelectable
	Sets            NOCOLLISION DIENOW CANTMOVE

	Type		Beast
	  Anim          5cw\pre_poison 34 39
	  PlayFx        GENERIC\ENTITYVANISH.FX  0 CONSTANT
	TEnd
MEnd

###################
###### air Wall  ######
###################

Move A_Wall_cast
	Requires	FLY COMBAT WALL
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Wallpost_cast
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\air_wall 1 30
	TEnd
MEnd

Move A_Wallpost_cast
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle_half
	Flags		NoSizeScale ReqInputs

	Type		Beast
	  Anim          male\air_wall 30
	TEnd

MEnd

#############################################################

Move A_Sword_Slash
	Requires	FLY COMBAT WEAPON SWORD ATTACK
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Sword_Slash_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		Beast
	  Anim          male\air_wm_slash 1 30
	TEnd

MEnd

Move A_Sword_Slash_Post
	Requires	FLY COMBAT WEAPON
	Priority	13
	Include sequencers\Members\Combat_Weapon_air.inc
	NextMove        W_flycycle_half
	Flags		NOSIZESCALE

	Type		Beast
	  Anim          male\air_wm_slash 31
	TEnd

MEnd

###################
###### Stomp  ######
###################

Move A_Stomp_Attack
	Requires	FLY COMBAT STOMP
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Stomp2_Attack
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\air_stomp 1 62
	TEnd
MEnd

Move A_Stomp2_Attack
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	Lazy_C_flycycle
	Flags		NoSizeScale ReqInputs

	Type		Beast
	  Anim          male\air_stomp 62
	TEnd
MEnd

###################
###### Cage  ######
###################

Move Cage
	Requires	COMBAT CAGE
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Cagepost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		Beast
	  Anim          Beast\hand_clap 1 35
	TEnd


MEnd

Move Cagepost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Scale           0.50
	Flags		NoSizeScale ReqInputs

	Type		Beast
	  Anim          Beast\hand_clap 35
	TEnd

MEnd

###################
###### Air Cage  ######
###################

Move airCage
	Requires	FLY COMBAT CAGE
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	airCagepost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		Beast
	  Anim          Beast\air_cage 1 35
	TEnd

MEnd

Move airCagepost
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flyready
	Flags		NoSizeScale

	Type		Beast
	  Anim          Beast\air_cage 30
	TEnd

MEnd