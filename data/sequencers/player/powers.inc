#########################################################
## POWERS.INC - Sequencers related to power animations
#########################################################

#######################################################################
####### Lighter blow (torch blow w\ lighter - goes back to combat)   ###
#######################################################################

Move Lighter_Blow
	Requires	COMBAT LIGHTERBLOW
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Lighter_Blow_2
	Sets            CANTMOVE
	Flags		NoSizeScale PitchToTarget

	Type		male
	  Anim          male\lighter_blow 1 85
	TEnd
MEnd

Move Lighter_Blow_2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Lazy_CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\lighter_blow 85
	TEnd
MEnd


#########################
####### Jacobs Ladder ###
#########################

Move JacobsLadder
	Requires	COMBAT JACOBSLADDER
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        JacobsLadder_2
	Priority        2
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		male
	  Anim          male\jacobs_ladder 1 48
	  PitchEnd	41
	TEnd
MEnd

Move JacobsLadder_2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Lazy_CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\jacobs_ladder 48
	TEnd
MEnd


############################
###### Lightning Teleport  ######
###########################

Move Lightning_Teleport
	Requires	COMBAT LIGHTNINGTELEPORT
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Lightning_Teleport_2
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\glory 1 30
	TEnd
MEnd

Move Lightning_Teleport_2
	Requires	COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Lightning_Teleport_post
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\pre_poison 30 58
	TEnd
MEnd

Move Lightning_Teleport_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Lazy_CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\pre_poison 58
	TEnd
MEnd



################################
####### Horn Blow (rams horn)   ###
################################

Move HornBlow
	Requires	COMBAT HORNBLOW
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        HornBlow_2
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\horn_blow 1 85
	TEnd
MEnd

Move HornBlow_2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Lazy_CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\horn_blow 85
	TEnd
MEnd

##########################
####### 1 Hand Slam    ###
##########################

Move 1Hslam
	Requires	COMBAT 1HSLAM
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        1Hslam_2
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\1hslam 1 75
	TEnd
MEnd

Move 1Hslam_2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Lazy_CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\1hslam 75
	TEnd
MEnd


##########################
####### 2H Power Thrust ###
##########################

Move 2HPowerThrust_Pre
	Requires	COMBAT 2HPOWERTHRUST
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        2HPowerThrust
	Flags		CANTMOVE NoSizeScale PitchToTarget

	Type		male
	  Anim          male\2HPowerThrust 1 57
	  PitchStart	48
	TEnd
MEnd

Move 2HPowerThrust
	Requires	COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        2HPowerThrust_Post
	Flags		CANTMOVE NoSizeScale PitchToTarget
	Scale		0.9

	Type		male
	  Anim          male\mart_backstab 125 145
	TEnd
MEnd

Move 2HPowerThrust_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Lazy_CombatReady
	Flags		NoSizeScale ReqInputs PitchToTarget

	Type		male
	  Anim          male\mart_backstab 145
	  PitchEnd	60
	TEnd
MEnd


############################
## KickBrawl - Right Side ##
############################

Move KickBrawl_R
	Requires	COMBAT KICKBRAWL
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        KickBrawl_R_post
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\kickbrawl_R 1 20
	TEnd
MEnd

Move KickBrawl_R_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        KickBrawl_cycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\kickbrawl_R 20 37
	TEnd
MEnd

#############################################################

Move KickBrawl_L
	Requires	COMBAT KICKBRAWL
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        KickBrawl_L_post
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\kickbrawl_L 1 20
	TEnd
MEnd

Move KickBrawl_L_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        KickBrawl_cycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\kickbrawl_L 20 37
	TEnd
MEnd

#############################################################

Move KickBrawl_cycle
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		Cycle NoSizeScale
	NextMove        postReady_back
	Interpolate	8
	Scale           0.50

	Type		male
	  Anim          male\kick_cycle 1
	TEnd
MEnd


########################
####### Toss duffelbag ###
########################

Move Duffelbag_Toss
	Requires	COMBAT DUFFELBAGTOSS
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Duffelbag_Toss2
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\duffelbag_toss 1 48
	TEnd
MEnd

Move Duffelbag_Toss2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Lazy_CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\duffelbag_toss 48
	TEnd
MEnd


####################
####### Freak Sign ###
####################

Move FreakSign
	Requires	FREAKSHOWSIGN
	Priority	1
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        FreakSign_Post
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\hi 1 45
	TEnd
MEnd

Move FreakSign_Post
	Include sequencers\Members\Readies.inc
	NextMove	Emote_HandsOnHips_cycle

	Type		male
	  Anim          male\hi 45
	TEnd
MEnd

##################
####### Show ######
##################

Move Show
	Requires	SHOW
	Priority        1
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Show2
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\show 1 85
	TEnd
MEnd

Move Show2
	Include sequencers\Members\Readies.inc
	NextMove        Ready
	Scale		0.8
	Flags		NoSizeScale

	Type		male
	  Anim          male\show 85
	TEnd
MEnd

#################################
####### Detonate ######
##################################

Move Detonate
	Requires	COMBAT DETONATE
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Detonate2
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\detonate 1 30
	TEnd
MEnd

Move Detonate2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Lazy_CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\detonate 30
	TEnd
MEnd


########################
###### AFRAID Place Trap (for Detonator) ######
########################

Move Afraid_Place_Trap
	Requires	PLACETRAPAFRAID
	Priority        1
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Afraid_Place_Trap2
	Flags		CANTMOVE NoSizeScale
	Scale		0.5

	Type		male
	  Anim          male\Loot 1 19
	TEnd
MEnd

Move Afraid_Place_Trap2
	Include sequencers\Members\Combat.inc
	NextMove        Place_Trap_post
	Scale		0.5
	Flags		NoSizeScale

	Type		male
	  Anim          male\examine_hero 1
	TEnd
MEnd

########################
###### Place Trap  ######
########################

Move Place_Trap
	Requires	PLACETRAP
	Priority        1
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Place_Trap2
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\Loot 1 19
	TEnd
MEnd

Move Place_Trap2
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Place_Trap_post
	Flags		CANTMOVE NoSizeScale
	Scale		0.75

	Type		male
	  Anim          male\examine_hero 1
	TEnd
MEnd

Move Place_Trap_post
	Include sequencers\Members\Combat.inc
	NextMove        Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\Loot 24
	TEnd
MEnd

##############################################
###### Ninja Upgrade 1 (Kuji_Rin + palmcast) ######
##############################################

Move Ninja_Upgrade_Rin
	Requires	COMBAT NINJAUPGRADE
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Ninja_Upgrade_Rin2
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\kuji_rin 1 50
	TEnd

	Type		fem
	  Anim          fem\kuji_rin 1 50
	TEnd

	Type		huge
	  Anim          huge\kuji_rin 1 50
	TEnd
MEnd

Move Ninja_Upgrade_Rin2
	Requires	COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Ninja_Upgrade_Rin_Post
	Interpolate	15
	Flags		PitchToTarget CANTMOVE NoSizeScale

	Type		male
	  Anim          male\palmcast 1 20
	  PitchStart	5
	  PitchAngle	-0.5
	TEnd
MEnd

Move Ninja_Upgrade_Rin_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		male
	  Anim          male\palmcast 20
	  PitchEnd	34
	  PitchAngle	-0.5
	TEnd
MEnd


##############################################
###### Ninja Upgrade 2 (Kuji_Zen + palmcast) ######
##############################################

Move Ninja_Upgrade_Zen
	Requires	COMBAT NINJAUPGRADE HIGH
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Ninja_Upgrade_Zen2
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\kuji_sha2 1 50
	TEnd

	Type		fem
	  Anim          fem\kuji_sha 1 50
	TEnd

	Type		huge
	  Anim          huge\kuji_sha 1 50
	TEnd
MEnd

Move Ninja_Upgrade_Zen2
	Requires	COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Ninja_Upgrade_Zen_Post
	Interpolate	15
	Flags		PitchToTarget CANTMOVE NoSizeScale

	Type		male
	  Anim          male\2hand_cast 1 20
	TEnd

	Type		huge
	  Anim          huge\2hand_cast 1 20
	TEnd

	Type		fem
	  Anim          fem\2hand_cast 1 20
	TEnd
MEnd

Move Ninja_Upgrade_Zen_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\2hand_cast 20
	  PitchEnd	42
	TEnd

	Type		huge
	  Anim          huge\2hand_cast 20
	  PitchEnd	42
	TEnd

	Type		fem
	  Anim          fem\2hand_cast 20
	  PitchEnd	42
	TEnd
MEnd

#################################
###### Point Buff (ninjas) ######
#################################

Move Point_Buff
	Requires	NINJATRAIN
	Priority        1
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Point_Buff2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\point 1 30
	TEnd
MEnd

Move Point_Buff2
	Include sequencers\Members\Combat.inc
	NextMove        Emote_Point_cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\point 30
	TEnd
MEnd


#################################
###### Command Summon (ninjas) ######
#################################

Move Command_Summon_Pre
	Requires	NINJASUMMON
	Priority        1
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Command_Summon
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\raise_hand 1 50
	TEnd

	Type		huge
	  Anim          huge\raise_hand 1 50
	TEnd

	Type		fem
	  Anim          fem\raise_hand 1 50
	TEnd
MEnd

Move Command_Summon
	Include sequencers\Members\Combat.inc
	NextMove        Command_Summon_PostCycle
	Interpolate	15
	Flags		NoSizeScale

	Type		male
	  Anim          male\no 7 90
	TEnd

	Type		huge
	  Anim          huge\no 7 90
	TEnd

	Type		fem
	  Anim          fem\no 7 90
	TEnd
MEnd

Move Command_Summon_PostCycle
	Include sequencers\Members\Combat.inc
	Flags		CYCLE

	Type		male
	  Anim          male\no 110 115
	TEnd
MEnd


########################################
###### Widows Attack (signature move)  ######
########################################

Move Widows_Attack
	Requires	WIDOWSATTACK COMBAT
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Widows_Attack2

	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          fem\widows_attack 1 60
	TEnd
MEnd

Move Widows_Attack2
	Include sequencers\Members\Combat.inc
	NextMove        Widows_Attack_cycle
	NextMove	CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          fem\widows_attack 60 71
	TEnd
MEnd

Move Widows_Attack_cycle
	Include sequencers\Members\Combat.inc
	Flags		NoSizeScale Cycle

	Type		male
	  Anim          fem\widows_attack 90
	TEnd
MEnd


############################
###### TechShield Activate  ######
############################

Move TechShield_activate
	Requires	TECHSHIELD
	Priority        1
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        TechShield_Boostup

	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\wristband_activate 35 60
	  PlayFX	V_COV\Powers\Robotics\Wristband.fx 0
	TEnd

	Type		huge
	  Anim          huge\wristband_activate 35 60
	  PlayFX	V_COV\Powers\Robotics\Wristband.fx 0
	TEnd
MEnd

Move TechShield_Boostup
	Include sequencers\Members\Readies.inc
	NextMove        Ready
	Interpolate	15
	Flags		NoSizeScale

	Type		male
	  Anim          male\absorb 1
	TEnd

	Type		huge
	  Anim          huge\boostup 1
	TEnd

	Type		fem
	  Anim          fem\boostup 1
	TEnd
MEnd


##############################################################
###### Powder blow (meant for fem & land ver only, for now) ######
##############################################################

Move Powder
	Requires	COMBAT POWDERBLOW
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Powder2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          fem\blow_powder 1 45
	TEnd
MEnd

Move Powder2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          fem\blow_powder 45
	TEnd
MEnd


#################################
###### Underhand Toss ######
#################################

Move Underhand
	Requires	COMBAT UNDERHANDTOSS
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Underhand2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\Underhand_toss 1 45
	TEnd
MEnd

Move Underhand2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Lazy_CombatReady
	Interpolate	15
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\Underhand_toss 45
	TEnd
MEnd


##############################
######## WindUp_Wristband Activate  ######
##############################

Move PreWindUp_Wristband
	Requires	WRISTBANDWINDUP
	Priority        1
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups.inc
	NextMove        WindUp_Wristband_cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\wristband_activate 1 48
	TEnd

	Type		huge
	  Anim          huge\wristband_activate 1 48
	TEnd
MEnd

Move WindUp_Wristband_cycle
	Requires	WRISTBANDWINDUP
	Priority        1
	Include sequencers\Members\Windups.inc
	Flags		Cycle NoSizeScale
	NextMove	postReady_back

	Type		male
	  Anim          male\wristband_activate 48
	  PlayFx        FEET\Hidrum.fx 1
	  PlayFx        FEET\Hidrum.fx 6
	  PlayFx        FEET\Hidrum.fx 12
	  PlayFx        FEET\Hidrum.fx 39
	TEnd

	Type		huge
	  Anim          huge\wristband_activate 48
	TEnd
MEnd

##################################################
###### SUPERfAST_Wristband Activate (one touch)  ######
###################################################

Move Superfast_Wristband_activate
	Requires	WRISTBANDACTIVATE QUICK HIGH
	Priority        3
	Interrupts	<AttackIRQ> PreWindUp_Wristband WindUp_Wristband_cycle
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        postReady_back
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\wristband_activate 42 60
	  PlayFx        FEET\Hidrum.fx 5
	  PlayFx        FEET\Hidrum.fx 12
	TEnd

	Type		huge
	  Anim          huge\wristband_activate 42 60
	TEnd
MEnd


############################
###### Quick_Wristband Activate  ######
############################

Move Quick_Wristband_activate
	Requires	WRISTBANDACTIVATE QUICK #COMBAT WEAPON DUAL ARCHERY - This move needs to play over "Bow_Quick"
	Priority        2
	Interrupts	<AttackIRQ> PreWindUp_Wristband WindUp_Wristband_cycle
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        postReady_back
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\wristband_activate 35 60
	TEnd

	Type		huge
	  Anim          huge\wristband_activate 35 60
	TEnd
MEnd

#############################################################

Move WristbandActivate_Quick_NonRooted
	Requires	WRISTBANDACTIVATE QUICK NOTROOTED
	Priority        3
	Interrupts	<AttackIRQ> PreWindUp_Wristband WindUp_Wristband_cycle
	Member		<DeathIRQ> <AttackIRQ> <BlockIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove        postReady_back
	Flags		NoSizeScale

	Type		male
	  Anim          male\wristband_activate 35 60
	TEnd

	Type		huge
	  Anim          huge\wristband_activate 35 60
	TEnd
MEnd

############################
###### Wristband Activate  ######
############################

Move Wristband_activate
	Requires	WRISTBANDACTIVATE
	Priority        1
	Interrupts	<AttackIRQ> PreWindUp_Wristband WindUp_Wristband_cycle
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Wristband_post

	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\wristband_activate 1 60
	TEnd

	Type		huge
	  Anim          huge\wristband_activate 1 60
	TEnd
MEnd

Move Wristband_post
	Include sequencers\Members\Readies.inc
	NextMove	postReady_back
	Flags		NoSizeScale

	Type		male
	  Anim          male\wristband_activate 60
	TEnd

	Type		huge
	  Anim          huge\wristband_activate 60
	TEnd
MEnd


####################
###### Kuji in Sha  ######
###################

Move Kuji_Sha
	Requires	BOOSTUP ATTACK KUNGFU HIGH
	Priority        4
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat.inc
	NextMove        postReady
	Flags		NoSizeScale
	Wind		FLY
	scale		0.7

	Type		male
	  Anim          male\kuji_zen 1
	TEnd

	Type		fem
	  Anim          fem\kuji_zen 1
	TEnd

	Type		huge
	  Anim          huge\kuji_zen 1
	TEnd
MEnd


####################
###### Kuji in Rin  ######
###################

Move Kuji_Rin
	Requires	KUJIRIN
	Priority        1
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat.inc
	NextMove        postReady
	Flags		NoSizeScale
	Wind		FLY
	scale		0.7

	Type		male
	  Anim          male\kuji_rin 1
	TEnd

	Type		fem
	  Anim          fem\kuji_rin 1
	TEnd

	Type		huge
	  Anim          huge\kuji_rin 1
	TEnd
MEnd

####################
###### Kuji in Zen  ######
###################

Move Kuji_Zen
	Requires	KUJIZEN
	Priority        1
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Kuji_Zen_post
	Sets            CANTMOVE
	Flags		NoSizeScale
	Wind		FLY

	Type		male
	  Anim          male\kuji_sha2 1 50
	TEnd

	Type		fem
	  Anim          fem\kuji_sha 1 50
	TEnd

	Type		huge
	  Anim          huge\kuji_sha 1 50
	TEnd
MEnd

Move Kuji_Zen_post
	Include sequencers\Members\Combat.inc
	NextMove	postReady
	Wind		FLY
	Flags		NoSizeScale

	Type		male
	  Anim          male\kuji_sha2 50
	TEnd

	Type		fem
	  Anim          fem\kuji_sha 50
	TEnd

	Type		huge
	  Anim          huge\kuji_sha 50
	TEnd
MEnd

###################
## Danger Sense  ##
###################

Move Danger_Sense
	Requires	KUNGFUAREA KUNGFU LOOKOUT OBSERVE
	Priority        4
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat.inc
	NextMove        Danger_Sense_cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\lookquick 1
	TEnd
MEnd

Move Danger_Sense_cycle
	Include sequencers\Members\Combat.inc
	Flags		Cycle NoSizeScale
	Scale		0.7

	Type		male
	  Anim          male\lookquick 1
	TEnd
MEnd

#########################
###### Smoke Flash  ######
#########################

Move SmokeFlash
	Requires	COMBAT CAST HIDE ATTACK KUNGFU
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        SmokeFlash_post
	Interpolate	8
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\smokeflash 1 50
	TEnd
MEnd

Move SmokeFlash_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove	Lazy_CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\smokeflash 50
	TEnd
MEnd


##########################
###### Mind Probe QUICK  ######
##########################

Move MindProbe_Quick
	Requires	COMBAT PSIONIC PUNCH
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        MindProbe_Quick2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\mindprobe 1 30
	TEnd
MEnd

Move MindProbe_Quick2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Lazy_CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\mindprobe 30
	TEnd
MEnd


########################
###### Post_transform  ######
#######################

Move Post_transform
	Requires	COMBAT POISON LOW
	Priority        3
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat.inc
	Flags		NoSizeScale

	Type		male
	  Anim          male\drain 40 100
	TEnd

	Type		huge
	  Anim          huge\drain 40 100
	TEnd

	Type		fem
	  Anim          fem\drain 40 100
	TEnd
MEnd


######################
### WarWolf Whistle ########
######################

Move WarWolf
	Requires	HIGH EXCLAMATION SPAWN
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        WarWolf_post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\whistle 1 40
	  PlayFx        FEET\WHISTLE2.FX 19
	TEnd

	Type		huge
	  Anim          huge\whistle 1 40
	  PlayFx        FEET\WHISTLE2.FX 19
	TEnd

	Type		fem
	  Anim          fem\whistle 1 40
	  PlayFx        FEET\WHISTLE2.FX 19
	TEnd
MEnd

Move WarWolf_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        WarWolf_cycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\whistle 40
	TEnd

	Type		huge
	  Anim          huge\whistle 40
	TEnd

	Type		fem
	  Anim          fem\whistle 40
	TEnd
MEnd

Move WarWolf_cycle
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		Cycle NoSizeScale ReqInputs
	NextMove        postReady_back

	Type		male
	  Anim          male\threat_cycle 1
	TEnd
MEnd


#########################################
###### Ascendant Shield Block  (toggle) ######
#########################################

Move Ascendant
	Requires	UNYIELD SHIELD
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Ascendant_cycle
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\ascendant_shield_block 1 30
	  PlayFX	GENERIC\FX_Shield.fx 18 CONSTANT
	TEnd
MEnd

Move Ascendant_cycle
	Requires	UNYIELD SHIELD
	Member		<DeathIRQ> <AttackIRQ>
	Flags		Cycle NoSizeScale
	NextMove        postReady_back
	Sets            CANTMOVE

	Type		male
	  Anim          male\ascendant_shield_block 30 40
	  PlayFX	GENERIC\FX_Shield.fx 0 CONSTANT
	TEnd
MEnd


#################################
###### Sonic Yell  ######
#################################

Move SonicYell
	Requires	COMBAT YELL STRONG ATTACK
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        SonicYell_post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\sonic_yell 1 40
	TEnd

	Type		huge
	  Anim          huge\sonic_yell 1 40
	TEnd

	Type		fem
	  Anim          fem\sonic_yell 1 40
	TEnd
MEnd

Move SonicYell_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\sonic_yell 40
	TEnd

	Type		huge
	  Anim          huge\sonic_yell 40
	TEnd

	Type		fem
	  Anim          fem\sonic_yell 40
	TEnd
MEnd


#################################
###### Sing Cast  ######
#################################

Move SingCast
	Requires	COMBAT YELL YES CAST
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        SingCast_post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\sing_cast 1 68
	TEnd

	Type		huge
	  Anim          huge\sing_cast 1 68
	TEnd
MEnd

Move SingCast_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\sing_cast 68
	TEnd

	Type		huge
	  Anim          huge\sing_cast 68
	TEnd
MEnd


#################################
###### QUICK spit  ######
#################################

Move Spit
	Requires	COMBAT BLOW CAST QUICK
	Priority        4
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Spitpost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\quickspit 1 30
	TEnd
MEnd

Move Spitpost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Lazy_CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\quickspit 31
	TEnd
MEnd

######################
###### Waterbackpack ######
#####################

Move Waterbackpack
	Requires	COMBAT FIRE NO
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Waterbackpack_2
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\waterbackpack 1 100
	TEnd
MEnd

Move Waterbackpack_2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		PitchToTarget ReqInputs NoSizeScale
	Interpolate	8

	Type		male
	  Anim          male\waterbackpack 171
	  PitchEnd	75
	TEnd
MEnd


######################
###### 1 Hand Swipe  ######
#####################

Move 1H_Swipe
	Requires	COMBAT HAND SWIPE
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        1H_cycle
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\FRK_1Hattack 1 33
	TEnd

	Type		Vahzilok
	  Anim          Vahzilok\vah_swipeL 1
	TEnd
MEnd

#############################################################

Move 1H_Swipe_L
	Requires	COMBAT HAND SWIPE
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        1H_cycle
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\FRK_1Hattack 41 71
	TEnd

	Type		Vahzilok
	  Anim          Vahzilok\vah_smash 1
	TEnd
MEnd

#############################################################

Move 1H_cycle
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		Cycle NoSizeScale
	NextMove        postReady

	Type		male
	  Anim          male\FRK_cycle 1
	TEnd

	Type		Vahzilok
	  Anim          Vahzilok\skel_vah_combat_cycle 1
	TEnd
MEnd


######################
###### 2 Hand Swipe  ######
#####################

Move 2H_Swipe
	Requires	COMBAT 2HAND SWIPE
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        2H_cycle
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\claw_mid2 1 31
	TEnd
	Type		mako
	  Anim          male\claw_mid_hands 1 31
	TEnd
MEnd

#############################################################

Move 2H_Swipe_L
	Requires	COMBAT 2HAND SWIPE
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        2H_cycle
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\claw_mid2 40 71
	TEnd
	Type		mako
	  Anim          male\claw_mid_hands 40 71
	TEnd
MEnd

#############################################################

Move 2H_cycle
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		Cycle NoSizeScale
	NextMove        postReady
	Scale           0.50

	Type		male
	  Anim          male\claw_cycle 1 31
	TEnd
	Type		mako
	  Anim          male\claw_cycle_hands 1 31
	TEnd
MEnd


##################################
###### 1 Hand Pound (freak, lefthand)  ######
#################################

Move 1H_Pound
	Requires	COMBAT HAND CHOP
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        1H_Pound2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\1H_smash 101 120
	TEnd
MEnd

Move 1H_Pound2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        1H_Pound_post
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\1H_smash 120 140
	TEnd
MEnd

#############################################################

Move 1H_Pound_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Scale           0.50
	Flags		NoSizeScale

	Type		male
	  Anim          male\1H_smash 141
	TEnd
MEnd

#############################################################

Move 1H_Pound_B
	Requires	COMBAT HAND CHOP
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        1H_Pound_B_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\frisbee 1 30
	TEnd
MEnd

Move 1H_Pound_B_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Scale           0.80
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\frisbee 30 70
	TEnd
MEnd


###################
###### Moment O Glory  ######
###################

Move Glory
	Requires	MOMENTGLORY
	Priority        1
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Glory2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\glory 1 45
	TEnd

	Type		huge
	  Anim          huge\glory 1 45
	TEnd

	Type		fem
	  Anim          fem\glory 1 45
	TEnd
MEnd

Move Glory2
	Include sequencers\Members\Combat.inc
	NextMove        Fake_CombatCycle_Half
	Flags		NoSizeScale

	Type		male
	  Anim          male\glory 45
	TEnd

	Type		huge
	  Anim          huge\glory 45
	TEnd

	Type		fem
	  Anim          fem\glory 45
	TEnd
MEnd


################
## Leadership ##
################

Move Leadership_pre
	Requires	LEADERSHIP
	Priority        1
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Readies.inc
	NextMove        Leadership
	Flags		NoSizeScale

	Type		male
	  Anim          male\leadership 1 10
	TEnd

	Type		huge
	  Anim          huge\leadership 1 10
	TEnd

	Type		fem
	  Anim          fem\leadership 1 10
	TEnd
MEnd

Move Leadership
	Include sequencers\Members\Combat.inc
	NextMove        Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\leadership 10 109
	TEnd

	Type		huge
	  Anim          huge\leadership 10 109
	TEnd

	Type		fem
	  Anim          fem\leadership 10 109
	TEnd
MEnd


###################
###### Med Other Windup (tricorder) ######
###################

Move MedOther_Pre
	Requires	MEDOTHER WINDUP HIGH
	Priority        3
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups.inc
	NextMove        MedOther_Cycle
	Flags		NoSizeScale
	Scale		1.5

	Type		male
	  Anim          male\med_other 1 88
	TEnd

	Type		huge
	  Anim          huge\med_other 1 88
	TEnd

	Type		fem
	  Anim          fem\med_other 1 88
	TEnd
MEnd

Move MedOther_Cycle
	Requires	MEDOTHER WINDUP
	Include sequencers\Members\Windups.inc
	Flags		Cycle NoSizeScale
	NextMove        MedOther_post

	Type		male
	  Anim          male\med_other 88 138
	TEnd

	Type		huge
	  Anim          huge\med_other 88 138
	TEnd

	Type		fem
	  Anim          fem\med_other 88 138
	TEnd
MEnd

Move MedOther_post
	Include sequencers\Members\Readies.inc
	NextMove        Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\med_other 138
	TEnd

	Type		huge
	  Anim          huge\med_other 138
	TEnd

	Type		fem
	  Anim          fem\med_other 138
	TEnd
MEnd

###################
###### Med Self Windup (tricorder) ######
###################

Move Pre_MedSelf
	Requires	MEDSELF
	Priority        1
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups.inc
	NextMove        MedSelf
	Flags		NoSizeScale

	Type		male
	  Anim          male\med_self 1 15
	TEnd

	Type		huge
	  Anim          huge\med_self 1 15
	TEnd

	Type		fem
	  Anim          fem\med_self 1 15
	TEnd
MEnd

#############################################################

Move MedSelf
	Requires	MEDSELF
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ>
	NextMove        MedSelf2

	Type		male
	  Anim          male\med_self 15 105
	TEnd

	Type		huge
	  Anim          huge\med_self 15 105
	TEnd

	Type		fem
	  Anim          fem\med_self 15 105
	TEnd
MEnd

Move MedSelf2
	Include sequencers\Members\readies.inc
	Flags		NoSizeScale
	NextMove        MedSelf_post

	Type		male
	  Anim          male\med_self 106 245
	TEnd

	Type		huge
	  Anim          huge\med_self 106 245
	TEnd

	Type		fem
	  Anim          fem\med_self 106 245
	TEnd
MEnd

Move MedSelf_post
	Include sequencers\Members\readies.inc
	NextMove        Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\med_self 245
	TEnd

	Type		huge
	  Anim          huge\med_self 245
	TEnd

	Type		fem
	  Anim          fem\med_self 245
	TEnd
MEnd

## AID SELF (Includes FX) ###########################################################

Move AidSelf_Pre
	Requires	MEDSELF TWO
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ>
	NextMove        AidSelf
	Flags		NoSizeScale
	Scale		1.5

	Type		male
	  Anim          male\med_self 1 15
	TEnd

	Type		huge
	  Anim          huge\med_self 1 15
	TEnd

	Type		fem
	  Anim          fem\med_self 1 15
	TEnd
MEnd

Move AidSelf
	Requires	MEDSELF TWO
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ>
	Flags		NoSizeScale
	NextMove        AidSelf_Post
	Scale		1.5

	Type		male
	  Anim          male\med_self 15 118
#	  PlayFX	POWERS\Medicine\MSelf.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\med_self 15 118
#	  PlayFX	POWERS\Medicine\MSelf.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\med_self 15 118
#	  PlayFX	POWERS\Medicine\MSelf.fx 0 CONSTANT
	TEnd
MEnd

Move AidSelf_Post
	Include sequencers\Members\readies.inc
	NextMove        Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\med_self 118
#	  PlayFX	POWERS\Medicine\MSelf.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\med_self 118
#	  PlayFX	POWERS\Medicine\MSelf.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\med_self 118
#	  PlayFX	POWERS\Medicine\MSelf.fx 0 CONSTANT
	TEnd
MEnd

###################################
###### Quick_Med Self (tricorder) ######
###################################

Move Quick_MedSelf
	Requires	MEDSELF QUICK
	Priority       	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Quick_MedSelf_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\med_self 1 165
	TEnd

	Type		huge
	  Anim          huge\med_self 1 165
	TEnd

	Type		fem
	  Anim          fem\med_self 1 165
	TEnd
MEnd

Move Quick_MedSelf_post
	Include sequencers\Members\Readies.inc
	NextMove        Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\med_self 165
	TEnd

	Type		huge
	  Anim          huge\med_self 165
	TEnd

	Type		fem
	  Anim          fem\med_self 165
	TEnd
MEnd


## REVIVE ###########################################################

Move Revive_Windup_Pre
	Requires	WINDUP MEDDEAD LOW
	Priority        3
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups.inc
	NextMove        Revive_Windup_Cycle
	Flags		NoSizeScale FixWeaponReg
	Scale		1.5

	Type		male
	  Anim          male\med_dead 1 100
	TEnd

	Type		huge
	  Anim          huge\med_dead 1 100
	TEnd

	Type		fem
	  Anim          fem\med_dead 1 100
	TEnd
MEnd

Move Revive_Windup_Cycle
	Requires	WINDUP MEDDEAD
	Include sequencers\Members\Windups.inc
	Flags		Cycle NoSizeScale FixWeaponReg
	NextMove        Revive_Post

	Type		male
	  Anim          male\med_dead 100 200
	TEnd

	Type		huge
	  Anim          huge\med_dead 100 200
	TEnd

	Type		fem
	  Anim          fem\med_dead 100 200
	TEnd
MEnd

## Non-Windup version ###########################################################

Move Revive_Pre
	Requires	MEDDEAD SLOW
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Revive
	Flags		NoSizeScale FixWeaponReg
	Sets		CANTMOVE
	Scale		1.5

	Type		male
	  Anim          male\med_dead 1 100
	TEnd

	Type		huge
	  Anim          huge\med_dead 1 100
	TEnd

	Type		fem
	  Anim          fem\med_dead 1 100
	TEnd
MEnd

Move Revive
	Member		<DeathIRQ> <AttackIRQ>
	Flags		NoSizeScale FixWeaponReg
	Sets		CANTMOVE
	NextMove        Revive_Post

	Type		male
	  Anim          male\med_dead 100 150
	TEnd

	Type		huge
	  Anim          huge\med_dead 100 150
	TEnd

	Type		fem
	  Anim          fem\med_dead 100 150
	TEnd
MEnd

#############################################################

Move Revive_Post
	Requires	MEDDEAD GETUP
	Priority        2
	Interrupts	<AttackIRQ> <Windups>
	Include sequencers\Members\Readies.inc
	NextMove        Ready
	Flags		NoSizeScale FixWeaponReg

	Type		male
	  Anim          male\med_dead 150
	TEnd

	Type		huge
	  Anim          huge\med_dead 150
	TEnd

	Type		fem
	  Anim          fem\med_dead 150
	TEnd
MEnd

###################
###### TechWindup  ######
###################

Move Pre_TechWindup
	Requires	TECHWINDUP
	Priority        1
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups.inc
	NextMove        TechWindup
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\sniperblast 1 40
	  PitchStart	5
	  PitchAngle	-0.5
	TEnd

	Type		huge
	  Anim          huge\sniperblast 1 40
	  PitchStart	5
	  PitchAngle	-0.5
	TEnd

	Type		fem
	  Anim          fem\sniperblast 1 40
	  PitchStart	5
	  PitchAngle	-0.5
	TEnd
MEnd

Move TechWindup
	Requires	TECHWINDUP
	Priority        1
	Include sequencers\Members\Windups.inc
	Flags		Cycle PitchToTarget NoSizeScale
	NextMove        TechWindup_post

	Type		male
	  Anim          male\sniperblast 40 60
	  PitchAngle	-0.5
	TEnd

	Type		huge
	  Anim          huge\sniperblast 40 60
	  PitchAngle	-0.5
	TEnd

	Type		fem
	  Anim          fem\sniperblast 40 60
	  PitchAngle	-0.5
	TEnd
MEnd

Move TechWindup_post
	Include sequencers\Members\Readies.inc
	NextMove        Combat_half
	Flags		NoSizeScale

	Type		male
	  Anim          male\sniperblast 119
	TEnd

	Type		huge
	  Anim          huge\sniperblast 119
	TEnd

	Type		fem
	  Anim          fem\sniperblast 119
	TEnd
MEnd


#############################################################################
###### Med Beam QUICK (no windup note: max file is NOINFLUENCE_MED_BEAM)  ######
#############################################################################

Move Med_Beam
	Requires	TRICORDERBEAM
	Priority        1
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Med_Beam_post
	Flags		CANTMOVE PitchToTarget NoSizeScale

	Type		male
	  Anim          male\med_beam 1 90
	  PitchStart	19
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\med_beam 1 90
	  PitchStart	19
	  PitchAngle	0.2
	TEnd

	Type		fem
	  Anim          fem\med_beam 1 90
	  PitchStart	19
	  PitchAngle	0.2
	TEnd
MEnd

Move Med_Beam_post
	Include sequencers\Members\Readies.inc
	NextMove        Ready
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\med_beam 90
	  PitchEnd	100
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\med_beam 90
	  PitchEnd	100
	  PitchAngle	0.2
	TEnd

	Type		fem
	  Anim          fem\med_beam 90
	  PitchEnd	100
	  PitchAngle	0.2
	TEnd
MEnd


###################
###### SniperBlast (hands)  ######
###################

Move SniperBlast
	Requires	COMBAT STRONG CAST
	Priority        3
	Interrupts	<AttackIRQ> Pre_TechWindup TechWindup
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        SniperBlast2
	Flags		PitchToTarget NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\sniperblast 62 100
	  PitchAngle	-0.5
	  PitchEnd	94
	TEnd

	Type		huge
	  Anim          huge\sniperblast 62 100
	  PitchAngle	-0.5
	  PitchEnd	94
	TEnd

	Type		fem
	  Anim          fem\sniperblast 62 100
	  PitchAngle	-0.5
	  PitchEnd	94
	TEnd
MEnd

Move SniperBlast2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\sniperblast 100
	TEnd

	Type		huge
	  Anim          huge\sniperblast 100
	TEnd

	Type		fem
	  Anim          fem\sniperblast 100
	TEnd
MEnd


####################################
###### QUICK SniperBlast (hands)  ######
####################################

Move Q_SniperBlast
	Requires	COMBAT QUICK SNIPERBLAST
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Q_SniperBlast2
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\sniperblast 1 100
	  PitchAngle	-0.5
	  PitchEnd	94
	TEnd

	Type		huge
	  Anim          huge\sniperblast 1 100
	  PitchAngle	-0.5
	  PitchEnd	94
	TEnd

	Type		fem
	  Anim          fem\sniperblast 1 100
	  PitchAngle	-0.5
	  PitchEnd	94
	TEnd
MEnd

Move Q_SniperBlast2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\sniperblast 100
	TEnd

	Type		huge
	  Anim          huge\sniperblast 100
	TEnd

	Type		fem
	  Anim          fem\sniperblast 100
	TEnd
MEnd


###################
###### WindUp  ######
###################

Move Pre_WindUp
	Requires	COMBAT WINDUP
	Priority        2
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups.inc
	NextMove        WindUp
	Flags		NoSizeScale

	Type		male
	  Anim          male\windup_power 1 42
	TEnd

	Type		huge
	  Anim          huge\windup_power 1 42
	TEnd

	Type		fem
	  Anim          fem\windup_power 1 42
	TEnd
MEnd

Move WindUp
	Requires	COMBAT WINDUP
	Include sequencers\Members\Windups.inc
	Flags		Cycle NoSizeScale
	NextMove        WindUp_post

	Type		male
	  Anim          male\windup_power 42 63
	TEnd

	Type		huge
	  Anim          huge\windup_power 42 63
	TEnd

	Type		fem
	  Anim          fem\windup_power 42 63
	TEnd
MEnd

Move WindUp_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\windup_power 63 77
	TEnd

	Type		huge
	  Anim          huge\windup_power 63 77
	TEnd

	Type		fem
	  Anim          fem\windup_power 63 77
	TEnd
MEnd


###################
###### WindUp Magic  ######
###################

Move Magic_WindUp_Pre
	Requires	COMBAT MAGICWINDUP
	Priority        2
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups.inc
	NextMove        Magic_WindUp
	Flags		NoSizeScale

	Type		male
	  Anim          male\ground_summon 1 15
	TEnd

	Type		huge
	  Anim          huge\ground_summon 1 15
	TEnd

	Type		fem
	  Anim          fem\ground_summon 1 15
	TEnd
MEnd

Move Magic_WindUp
	Requires	COMBAT MAGICWINDUP
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups.inc
	NextMove        Magic_WindUp_cycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\ground_summon 15 65
	TEnd

	Type		huge
	  Anim          huge\ground_summon 15 65
	TEnd

	Type		fem
	  Anim          fem\ground_summon 15 65
	TEnd
MEnd

Move Magic_WindUp_cycle
	Requires	COMBAT MAGICWINDUP
	Include sequencers\Members\Windups.inc
	Flags		Cycle NoSizeScale
	NextMove        Magic_WindUp_post

	Type		male
	  Anim          male\ground_summon 65 104
	TEnd

	Type		huge
	  Anim          huge\ground_summon 65 104
	TEnd

	Type		fem
	  Anim          fem\ground_summon 65 104
	TEnd
MEnd

Move Magic_WindUp_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Scale           0.50
	Flags		NoSizeScale

	Type		male
	  Anim          male\ground_summon 104
	TEnd

	Type		huge
	  Anim          huge\ground_summon 104
	TEnd

	Type		fem
	  Anim          fem\ground_summon 104
	TEnd
MEnd


###################
###### UNyielding Stance (toggle) ######
###################

Move UNyield
	Requires	UNYIELD
	Priority        1
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        UNyield_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\unyield 1 65
	TEnd

	Type		huge
	  Anim          huge\unyield 1 65
	TEnd

	Type		fem
	  Anim          fem\unyield 1 65
	TEnd
MEnd

Move UNyield_post
	Include sequencers\Members\Readies.inc
	NextMove        Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\unyield 65
	TEnd

	Type		huge
	  Anim          huge\unyield 65
	TEnd

	Type		fem
	  Anim          fem\unyield 65 93
	TEnd
MEnd


################################
###### Windblast  ######
################################

Move Windblast
	Requires	COMBAT WINDBLAST
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Windblastpost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\windblast 1 70
	TEnd

	Type		huge
	  Anim          huge\windblast 1 70
	TEnd

	Type		fem
	  Anim          fem\windblast 1 70
	TEnd
MEnd

Move Windblastpost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\windblast 70
	TEnd

	Type		huge
	  Anim          huge\windblast 70
	TEnd

	Type		fem
	  Anim          fem\windblast 70
	TEnd
MEnd


################################
###### Rapidblast  ######
################################

Move Rapid_Quick
	Requires	COMBAT RAPIDBLAST QUICK
	Priority        3
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Rapid_Quick_Post
	Flags		PITCHTOTARGET NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\energy_rapid 1 31
	TEnd
MEnd

Move Rapid_Quick_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\energy_rapid 31
	TEnd
MEnd

#############################################################

Move Rapid
	Requires	COMBAT RAPIDBLAST
	Priority        2
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Rapid_Post
	Flags		PITCHTOTARGET NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\energy_rapid 1 50
	TEnd

	Type		huge
	  Anim          huge\energy_rapid 1 50
	TEnd

	Type		fem
	  Anim          fem\energy_rapid 1 50
	TEnd
MEnd

Move Rapid_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\energy_rapid 50
	TEnd

	Type		huge
	  Anim          huge\energy_rapid 50
	TEnd

	Type		fem
	  Anim          fem\energy_rapid 50
	TEnd
MEnd

#############################################################

Move Rapid_Slow
	Requires	COMBAT RAPIDBLAST SLOW
	Priority        3
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Rapid_Slow_Post
	Flags		PITCHTOTARGET NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\energy_rapid 1 60
	TEnd
MEnd

Move Rapid_Slow_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\energy_rapid 60
	TEnd
MEnd

################################
###### DarkBlast or Megablast  ######
################################

Move Mega
	Requires	COMBAT MEGABLAST
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Megapost
	Flags		PitchToTarget NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\darkblast 1 76
	  PitchStart	62
	TEnd

	Type		huge
	  Anim          huge\megablast 1 76
	  PitchStart	62
	TEnd

	Type		fem
	  Anim          fem\megablast 1 76
	  PitchStart	62
	TEnd
MEnd

Move Megapost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Scale           0.50
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\darkblast 76
	TEnd

	Type		huge
	  Anim          huge\megablast 76
	TEnd

	Type		fem
	  Anim          fem\megablast 76
	TEnd
MEnd

#############################################################

Move Megablast_2HandCast_Pre
	Requires	COMBAT MEGABLAST 2HAND
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Megablast_2HandCast
	Flags		PitchToTarget NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\darkblast 1 64
	TEnd
MEnd

Move Megablast_2HandCast
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        2HandCastpost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\2hand_cast 15 50
	  PitchEnd	42
	  PitchAngle	0
	TEnd
MEnd

###################
###### Blow Cast  ######
###################

Move Blow
	Requires	COMBAT BLOW
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Blowpost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\blow_cast 1 80
	  PitchStart	12
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\blow_cast 1 80
	  PitchStart	12
	  PitchAngle	0.2
	TEnd

	Type		fem
	  Anim          fem\blow_cast 1 80
	  PitchStart	12
	  PitchAngle	0.2
	TEnd


	Type		mako
	  Anim          male\blow_cast_jaw 1 80
	  PitchStart	12
	  PitchAngle	0.2
	TEnd
MEnd

Move Blowpost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Scale           0.5
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		male
	  Anim          male\blow_cast 80
	  PitchEnd	85
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\blow_cast 80
	  PitchEnd	85
	  PitchAngle	0.2
	TEnd

	Type		fem
	  Anim          fem\blow_cast 80
	  PitchEnd	85
	  PitchAngle	0.2
	TEnd

	Type		mako
	  Anim          male\blow_cast_jaw 80
	  PitchEnd	85
	  PitchAngle	0.2
	 TEnd
MEnd


###################
###### Blinding Cast  ######
###################

Move blinding
	Requires	COMBAT BLINDING
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        blindingpost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\blinding 1 88
	TEnd

	Type		huge
	  Anim          huge\blinding 1 88
	TEnd

	Type		fem
	  Anim          fem\blinding 1 88
	TEnd
MEnd

Move blindingpost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Scale           0.50
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\blinding 88
	TEnd

	Type		huge
	  Anim          huge\blinding 88
	TEnd

	Type		fem
	  Anim          fem\blinding 88
	TEnd
MEnd


#############################
###### Quick_Blinding Cast  ###
#############################

Move Quickblinding
	Requires	COMBAT BLINDING LOW STRONG
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        blindingpost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\blinding 39 88
	TEnd

	Type		huge
	  Anim          huge\blinding 39 88
	TEnd

	Type		fem
	  Anim          fem\blinding 39 88
	TEnd
MEnd


###################
###### Boost Aura (flameaura)  ######
###################

# fixme - brian added longer cast here to fix bugs on 6\7\06

Move Boostaura_FastCast
	Requires	BOOSTAURA HIGH
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Boostaura3
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\flameaura 1 47
	TEnd

	Type		huge
	  Anim          huge\fireaura 1 47
	TEnd

	Type		fem
	  Anim          fem\fireaura 1 47
	TEnd
MEnd

#############################################################

Move Boostaura
	Requires	BOOSTAURA
	Priority        1
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Boostaura2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\ground_summon 1 40
	TEnd

	Type		huge
	  Anim          huge\ground_summon 1 40
	TEnd

	Type		fem
	  Anim          fem\ground_summon 1 40
	TEnd
MEnd

Move Boostaura2
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Boostaura3
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\flameaura 1 48
	TEnd

	Type		huge
	  Anim          huge\fireaura 1 48
	TEnd

	Type		fem
	  Anim          fem\fireaura 1 48
	TEnd
MEnd

Move Boostaura3
	Include sequencers\Members\Readies.inc
	NextMove        Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\flameaura 48
	TEnd

	Type		huge
	  Anim          huge\fireaura 48
	TEnd

	Type		fem
	  Anim          fem\fireaura 48
	TEnd
MEnd


##############################
###### Predictable Teleport  ######
##############################

Move Predict_Teleport
	Requires	SUPERTELEPORT
	Priority        1
	Interrupts	<AttackIRQ> <Windups>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Predict_Teleport2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\flameaura 1 55
	TEnd

	Type		huge
	  Anim          huge\fireaura 1 55
	TEnd

	Type		fem
	  Anim          fem\fireaura 1 55
	TEnd
MEnd

Move Predict_Teleport2
	Include sequencers\Members\Combat.inc
	NextMove        Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\flameaura 55
	TEnd

	Type		huge
	  Anim          huge\fireaura 55
	TEnd

	Type		fem
	  Anim          fem\fireaura 55
	TEnd
MEnd


###################
###### Long_Quick Boost Aura (built-in pause for TELEPORT)  ######
###################

Move Boostaura_long
	Requires	QUICK STRONG BOOSTAURA
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Boostaura_long2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\flameaura 1 54
	TEnd

	Type		huge
	  Anim          huge\fireaura 1 54
	TEnd

	Type		fem
	  Anim          fem\fireaura 1 54
	TEnd
MEnd

Move Boostaura_long2
	Requires	COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Boostaura_long3
	Interpolate	0
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\fireaura_pose 1 30
	TEnd

	Type		huge
	  Anim          huge\fireaura_pose 1 30
	TEnd

	Type		fem
	  Anim          fem\fireaura_pose 1 30
	TEnd
MEnd

Move Boostaura_long3
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Ready
	Interpolate	0
	Flags		NoSizeScale

	Type		male
	  Anim          male\flameaura 55
	TEnd

	Type		huge
	  Anim          huge\fireaura 55
	TEnd

	Type		fem
	  Anim          fem\fireaura 55
	TEnd
MEnd

###################
###### QUICK Boost Aura (short) ######
###################

Move Boostaura_quick
	Requires	QUICK BOOSTAURA
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Boostaura_quick2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\flameaura 2 48
	TEnd

	Type		huge
	  Anim          huge\fireaura 2 48
	TEnd

	Type		fem
	  Anim          fem\fireaura 2 48
	TEnd
MEnd

Move Boostaura_quick2
	Include sequencers\Members\Readies.inc
	NextMove        Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\flameaura 48
	TEnd

	Type		huge
	  Anim          huge\fireaura 48
	TEnd

	Type		fem
	  Anim          fem\fireaura 48
	TEnd
MEnd


###############################################
###### transform_into_Monster (pre poison)  ######
###############################################

Move Transform
	Requires	COMBAT POISON STRONG
	Priority        3
	Interrupts	<AttackIRQ>
##	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Vanish_Death
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\pre_poison 1 37
	  PlayFx        FEET\Transform.FX
	TEnd

	Type		huge
	  Anim          huge\pre_poison 1 37
	  PlayFx        FEET\Transform.FX
	TEnd

	Type		fem
	  Anim          fem\pre_poison 1 37
	  PlayFx        FEET\Transform.FX
	TEnd
MEnd


################################################
####### Generic Post Transform (post poison)   ###
################################################

Move GenericPostTransform
	Requires	COMBAT POSTTRANSFORM
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        GenericPostTransform_2
	Flags		CANTMOVE PitchToTarget NoSizeScale

	Type		male
	  Anim          male\pre_poison 40 50
	TEnd

	Type		huge
	  Anim          huge\pre_poison 40 50
	TEnd

	Type		fem
	  Anim          fem\pre_poison 40 50
	TEnd
MEnd

Move GenericPostTransform_2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\pre_poison 50
	TEnd

	Type		huge
	  Anim          huge\pre_poison 50
	TEnd

	Type		fem
	  Anim          fem\pre_poison 50
	TEnd
MEnd


################################################################################
###### Rikti Shapeshifter transform (actually, his power is being turned off)  ######
################################################################################

Move RiktiShapeshiftTransform
	Requires	RIKTISHAPESHIFTTRANSFORM
	Priority        1
	Interrupts	<AttackIRQ>
	NextMove        RiktiShapeshiftTransform_fade
	Flags		NotSelectable NOCOLLISION CANTMOVE NoSizeScale

	Type		male
	  Anim          male\lookquick 1
	  PlayFx	Generic\Faerie_entityFadeOut.fx 58 constant
	TEnd
MEnd

Move RiktiShapeshiftTransform_fade
	Flags		Cycle NotSelectable NOCOLLISION CANTMOVE

	Type		male
	  Anim          male\lookquick 1
	  PlayFx	Generic\Faerie_entityFadeOut.fx 0 constant
	TEnd
MEnd


###############################################
###### transform_into_Kheldian ######
###############################################

Move Transform_Kheldian
	Requires	COMBAT POISON STRONG MID
	Priority        4
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat.inc
	NextMove        Transform_Kheldian2
	Flags		NoSizeScale

	Type		male
	  Anim          male\pre_poison 1 2
	  PlayFx        FEET\Transform.FX
	TEnd

	Type		huge
	  Anim          huge\pre_poison 1 2
	  PlayFx        FEET\Transform.FX
	TEnd

	Type		fem
	  Anim          fem\pre_poison 1 2
	  PlayFx        FEET\Transform.FX
	TEnd
MEnd

Move Transform_Kheldian2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\pre_poison 3
	  PlayFx        FEET\Transform.FX
	TEnd

	Type		huge
	  Anim          huge\pre_poison 3
	  PlayFx        FEET\Transform.FX
	TEnd

	Type		fem
	  Anim          fem\pre_poison 3
	  PlayFx        FEET\Transform.FX
	TEnd
MEnd

#############################################################

Move Deactivate_Kheldian
	Requires	COMBAT POISON STRONG MID LOW
	Priority        5
	Interrupts	<AttackIRQ> Transform_Kheldian_cycle Transform_Kheldian2
	Include sequencers\Members\Combat.inc
	NextMove        postReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\drain 40 100
	TEnd

	Type		huge
	  Anim          huge\drain 40 100
	TEnd

	Type		fem
	  Anim          fem\drain 40 100
	TEnd
MEnd


###################
###### Poison Mode (MR.Spikey)  ######
###################

Move PrePoison
	Requires	COMBAT POISON
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        PrePoison2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\pre_poison 1 88
	TEnd

	Type		huge
	  Anim          huge\pre_poison 1 88
	TEnd

	Type		fem
	  Anim          fem\pre_poison 1 88
	TEnd
MEnd

Move PrePoison2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Scale           0.50
	Flags		NoSizeScale

	Type		male
	  Anim          male\pre_poison 88
	TEnd

	Type		huge
	  Anim          huge\pre_poison 88
	TEnd

	Type		fem
	  Anim          fem\pre_poison 88
	TEnd
MEnd


##################
###### Poison Stab  ######
###################

Move Poisonstab_R
	Requires	COMBAT ATTACK POISON
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Poisonstab_R2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\poison_stab 1 22
	TEnd

	Type		huge
	  Anim          huge\poison_stab 1 22
	TEnd

	Type		fem
	  Anim          fem\poison_stab 1 22
	TEnd
MEnd

Move Poisonstab_R2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        poisoncycle_15
	Scale           0.50
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\poison_stab 22 46
	TEnd

	Type		huge
	  Anim          huge\poison_stab 22 46
	TEnd

	Type		fem
	  Anim          fem\poison_stab 22 46
	TEnd
MEnd

#############################################################

Move poisoncycle_15
	Requires	COMBAT POISON
	Include sequencers\Members\Combat.inc
	NextMove        poisoncycle
	Scale           0.50
	Flags		NoSizeScale

	Type		male
	  Anim          male\claw_cycle 15 31
	TEnd

	Type		huge
	  Anim          huge\claw_cycle 15 31
	TEnd

	Type		fem
	  Anim          fem\claw_cycle 15 31
	TEnd
MEnd

#############################################################

Move poisoncycle
	Requires	COMBAT POISON
	Include sequencers\Members\Combat.inc
	Flags		Cycle NoSizeScale
	NextMove        postReady_back
	Scale           0.50

	Type		male
	  Anim          male\claw_cycle 1 31
	TEnd

	Type		huge
	  Anim          huge\claw_cycle 1 31
	TEnd

	Type		fem
	  Anim          fem\claw_cycle 1 31
	TEnd
MEnd


###################
###### Cast  ######
###################

Move Cast
	Requires	COMBAT CAST
	Priority        2
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Castpost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\palmcast 1 30
	  PitchStart	5
	  PitchAngle	-0.5
	TEnd

	Type		huge
	  Anim          huge\palmcast 1 30
	  PitchStart	5
	  PitchAngle	-0.5
	TEnd

	Type		fem
	  Anim          fem\palmcast 1 30
	  PitchStart	5
	  PitchAngle	-0.5
	TEnd
MEnd

Move Castpost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatBlend
	Flags		NoSizeScale

	Type		male
	  Anim          male\palmcast 30 45
	TEnd

	Type		huge
	  Anim          huge\palmcast 30 45
	TEnd

	Type		fem
	  Anim          fem\palmcast 30
	TEnd
MEnd

###################
###### 2Hand Cast  ######
###################

Move 2HandCast
	Requires	COMBAT 2HAND CAST
	Priority        3
	Interrupts	<AttackIRQ> <Defiance> <Windups> Pre_WindUp WindUp
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        2HandCastpost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\2hand_cast 1 50
	  PitchEnd	42
	  PitchAngle	0
	TEnd

	Type		huge
	  Anim          huge\2hand_cast 1 50
	  PitchEnd	42
	  PitchAngle	0
	TEnd

	Type		fem
	  Anim          fem\2hand_cast 1 50
	  PitchEnd	42
	  PitchAngle	0
	TEnd
MEnd

Move 2HandCastpost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Flags		NoSizeScale

	Type		male
	  Anim          male\2hand_cast 50
	TEnd

	Type		huge
	  Anim          huge\2hand_cast 50
	TEnd

	Type		fem
	  Anim          fem\2hand_cast 50
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

	Type		male
	  Anim          male\ball_cast 1 70
	  PitchEnd	60
	TEnd

	Type		huge
	  Anim          huge\ball_cast 1 70
	  PitchEnd	60
	TEnd

	Type		fem
	  Anim          fem\ball_cast 1 70
	  PitchEnd	60
	TEnd
MEnd

Move BallCastpost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Flags		NoSizeScale

	Type		male
	  Anim          male\ball_cast 70
	TEnd

	Type		huge
	  Anim          huge\ball_cast 70
	TEnd

	Type		fem
	  Anim          fem\ball_cast 70
	TEnd
MEnd


###################
###### Repulsion Bomb  ######
###################

Move RepulsionBomb_Pre
	Requires	COMBAT REPULSION
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        RepulsionBomb_02
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale
	Scale		1.25

	Type		male
	  Anim          male\ball_cast 1 30
	  PitchEnd	60
	TEnd

	Type		huge
	  Anim          huge\ball_cast 1 30
	  PitchEnd	60
	TEnd

	Type		fem
	  Anim          fem\ball_cast 1 30
	  PitchEnd	60
	TEnd
MEnd

Move RepulsionBomb_02
	Requires	COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        RepulsionBomb_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale
	Scale		1.25

	Type		male
	  Anim          male\ball_cast 45 70
	  PitchEnd	60
	TEnd

	Type		huge
	  Anim          huge\ball_cast 45 70
	  PitchEnd	60
	TEnd

	Type		fem
	  Anim          fem\ball_cast 45 70
	  PitchEnd	60
	TEnd
MEnd

Move RepulsionBomb_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Flags		NoSizeScale

	Type		male
	  Anim          male\ball_cast 70
	TEnd

	Type		huge
	  Anim          huge\ball_cast 70
	TEnd

	Type		fem
	  Anim          fem\ball_cast 70
	TEnd
MEnd


###################
###### magic throw  ######
###################

Move magicthrow
	Requires	COMBAT MAGICTHROW
	Priority        2
	NextMove        magicthrowpost
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\frisbee 1 30
	  PitchAngle	0.3
	  PitchStart	11
	  PitchEnd	23
	TEnd

	Type		huge
	  Anim          huge\frisbee 1 30
	  PitchStart	11
	  PitchEnd	23
	TEnd

	Type		fem
	  Anim          fem\frisbee 1 30
	  PitchStart	11
	  PitchEnd	23
	TEnd
MEnd

Move magicthrowpost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Flags		NoSizeScale

	Type		male
	  Anim          male\frisbee 30 70
	TEnd

	Type		huge
	  Anim          huge\frisbee 30 70
	TEnd

	Type		fem
	  Anim          fem\frisbee 30 70
	TEnd
MEnd


###################
###### Smash Cast Radius  ######
###################

Move SmashCast
	Requires	COMBAT SMASH
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        SmashCastpost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\2Hsmash 1 45
	TEnd

	Type		huge
	  Anim          huge\2Hsmash 1 45
	TEnd

	Type		fem
	  Anim          fem\2Hsmash 1 45
	TEnd
MEnd

Move SmashCastpost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Fake_CombatCycle_Half
	Scale           0.80
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\2Hsmash 45
	TEnd

	Type		huge
	  Anim          huge\2Hsmash 45
	TEnd

	Type		fem
	  Anim          fem\2Hsmash 45
	TEnd
MEnd


###################
###### Disrupt  ######
###################

Move Disrupt
	Requires	COMBAT DISRUPT
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Disruptpost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\disrupt 1 70
	TEnd

	Type		huge
	  Anim          huge\disrupt 1 70
	TEnd

	Type		fem
	  Anim          fem\disrupt 1 70
	TEnd
MEnd

Move Disruptpost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\disrupt 70
	TEnd

	Type		huge
	  Anim          huge\disrupt 70
	TEnd

	Type		fem
	  Anim          fem\disrupt 70
	TEnd
MEnd


###################
###### Levitate  ######
###################

Move Levitate
	Requires	COMBAT LEVITATE
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Levitatepost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\levitate 1 50
	TEnd

	Type		huge
	  Anim          huge\levitate 1 50
	TEnd

	Type		fem
	  Anim          fem\levitate 1 50
	TEnd
MEnd

Move Levitatepost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\levitate 50
	TEnd

	Type		huge
	  Anim          huge\levitate 50
	TEnd

	Type		fem
	  Anim          fem\levitate 50
	TEnd
MEnd


###################
###### Levitate + Forceram  ######
###################

Move Lev_Ram
	Requires	COMBAT STRONG LEVITATE
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Lev_Ram2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\levitate 1 98
	TEnd

	Type		huge
	  Anim          huge\levitate 1 98
	TEnd

	Type		fem
	  Anim          fem\levitate 1 98
	TEnd
MEnd

Move Lev_Ram2
	Requires	COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Lev_Ram2_post
	Interpolate	8
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\forceram 5 30
	TEnd

	Type		huge
	  Anim          huge\forceram 5 30
	TEnd

	Type		fem
	  Anim          fem\forceram 5 30
	TEnd
MEnd

Move Lev_Ram2_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatBlend
	Flags		NoSizeScale

	Type		male
	  Anim          male\forceram 30 57
	TEnd

	Type		huge
	  Anim          huge\forceram 30
	TEnd

	Type		fem
	  Anim          fem\forceram 30
	TEnd
MEnd


##########################
###### Telekinesis CAST  ######
##########################

Move Telekinesis
	Requires	COMBAT TELEKINESIS
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Telekinecycle
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\telekinesis 2 53
	  PitchStart	9
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\telekinesis 2 53
	  PitchStart	9
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\telekinesis 2 53
	  PitchStart	9
	  PitchAngle	0.5
	TEnd
MEnd

Move Telekinecycle
	Requires	COMBAT TELEKINESIS
	Member		<DeathIRQ> <AttackIRQ>
	Flags		Cycle PitchToTarget NoSizeScale
	NextMove        PostTelekine
	Sets            CANTMOVE

	Type		male
	  Anim          male\telekinesis 53 92
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\telekinesis 53 92
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\telekinesis 53 92
	  PitchAngle	0.5
	TEnd
MEnd

Move PostTelekine
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatBlend
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\telekinesis 92 105
	TEnd

	Type		huge
	  Anim          huge\telekinesis 92
	TEnd

	Type		fem
	  Anim          fem\telekinesis 92
	TEnd
MEnd


###################
###### Telekinesis_quick  ######
###################

Move Telequick
	Requires	COMBAT ATTACK TELEKINESIS
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Telequickpost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\telekinesis 2 35
	  PitchStart	9
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\telekinesis 2 35
	  PitchStart	9
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\telekinesis 2 35
	  PitchStart	9
	  PitchAngle	0.5
	TEnd
MEnd

Move Telequickpost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		male
	  Anim          male\telekinesis 35
	  PitchEnd	97
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\telekinesis 35
	  PitchEnd	97
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\telekinesis 35
	  PitchEnd	97
	  PitchAngle	0.5
	TEnd
MEnd


###################
###### Telekinesis + Magic Cast  ######
###################

Move Tele_Magic
	Requires	COMBAT MAGIC TELEKINESIS
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Tele_Magic2
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\telekinesis 1 75
	  PitchStart	10
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\telekinesis 1 75
	  PitchStart	10
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\telekinesis 1 75
	  PitchStart	10
	  PitchAngle	0.5
	TEnd
MEnd

Move Tele_Magic2
	Requires	COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Tele_Magic_post
	Interpolate	8
	Sets            CANTMOVE
	Flags		NoSizeScale PitchToTarget

	Type		male
	  Anim          male\magic_L 5 30
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\magic_L 5 30
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\magic_L 5 30
	  PitchAngle	0.5
	TEnd
MEnd

Move Tele_Magic_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatBlend
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\magic_L 30 66
	  PitchEnd	60
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\magic_L 30
	  PitchEnd	60
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\magic_L 30
	  PitchEnd	60
	  PitchAngle	0.5
	TEnd
MEnd


###################
###### Asorb Pain  ######
###################

Move AsorbPain
	Requires	COMBAT STRONG TELEKINESIS
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        AsorbPain2
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\telekinesis 2 91
	  PitchStart	9
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\telekinesis 2 91
	  PitchStart	9
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\telekinesis 2 91
	  PitchStart	9
	  PitchAngle	0.5
	TEnd
MEnd

Move AsorbPain2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        AsorbPaincycle
	Scale           0.20
	Interpolate	8
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\sleep 9 55
	TEnd

	Type		huge
	  Anim          huge\sleep 9 55
	TEnd

	Type		fem
	  Anim          fem\sleep 9 55
	TEnd
MEnd

Move AsorbPaincycle
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		Cycle NoSizeScale
	Scale           0.20

	Type		male
	  Anim          male\sleep 55 85
	TEnd

	Type		huge
	  Anim          huge\sleep 55 85
	TEnd

	Type		fem
	  Anim          fem\sleep 55 85
	TEnd
MEnd


###################
###### Psionic Windup  ######
###################

Move Pre_PsiWindup
	Requires	COMBAT MYSICWINDUP
	Priority        3
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups.inc
	NextMove        PsiWindup
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\psionic 1 50
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\psionic 1 50
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

	Type		fem
	  Anim          fem\psionic 1 50
	  PitchStart	13
	  PitchAngle	0.2
	TEnd
MEnd

Move PsiWindup
	Requires	COMBAT MYSICWINDUP
	Include sequencers\Members\Windups.inc
	Flags		Cycle PitchToTarget NoSizeScale
	NextMove        PsiWindup_post

	Type		male
	  Anim          male\psionic 50 89
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\psionic 50 89
	  PitchAngle	0.2
	TEnd

	Type		fem
	  Anim          fem\psionic 50 89
	  PitchAngle	0.2
	TEnd
MEnd

Move PsiWindup_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\psionic 165
	TEnd

	Type		huge
	  Anim          huge\psionic 165
	TEnd

	Type		fem
	  Anim          fem\psionic 165
	TEnd
MEnd


###################
###### QUICK Psionic Blast (comes after psi windup)  ######
###################

Move PsiQuick
	Requires	COMBAT QUICK PSIONICBLAST
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        PsiQuick2
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\psionic 97 125
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\psionic 97 125
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

	Type		fem
	  Anim          fem\psionic 97 125
	  PitchStart	13
	  PitchAngle	0.2
	TEnd
MEnd

Move PsiQuick2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\psionic 165
	 PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\psionic 165
	  PitchAngle	0.2
	TEnd

	Type		fem
	  Anim          fem\psionic 165
	  PitchAngle	0.2
	TEnd
MEnd


###################
###### Psionics  ######
###################

# fixme - brian added longer cast here to fix bugs on 6\7\06

Move Psionics
	Requires	COMBAT PSIONIC
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        PostPsionics
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\psionic 2 89
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\psionic 2 89
	  PitchStart	5
	  PitchAngle	0.2
	TEnd

	Type		fem
	  Anim          fem\psionic 2 89
	  PitchStart	5
	  PitchAngle	0.2
	TEnd
MEnd

#############################################################

Move Psionics_SlowCast
	Requires	COMBAT PSIONIC HIGH
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        PostPsionics
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale
	Scale		0.9

	Type		male
	  Anim          male\psionic 9 89
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\psionic 9 89
	  PitchStart	5
	  PitchAngle	0.2
	TEnd

	Type		fem
	  Anim          fem\psionic 9 89
	  PitchStart	5
	  PitchAngle	0.2
	TEnd
MEnd

Move PostPsionics
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\psionic 162
	TEnd

	Type		huge
	  Anim          huge\psionic 162
	TEnd

	Type		fem
	  Anim          fem\psionic 162
	TEnd
MEnd

#############################################################

Move Psionics_NoRoot
	Requires	COMBAT PSIONIC NOTROOTED
	Priority        3
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat.inc
	NextMove        PostPsionics
	Flags		NoSizeScale

	Type		male
	  Anim          male\psionic 2 89
	TEnd

	Type		huge
	  Anim          huge\psionic 2 89
	TEnd

	Type		fem
	  Anim          fem\psionic 2 89
	TEnd
MEnd

## PSI BLAST - QUICK ###########################################################

Move PsiBlast_Quick_Pre
	Requires	COMBAT PSIONICBLAST HIGH
	Priority        3
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        PsiBlast
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale
	Scale		2.0

	Type		male
	  Anim          male\psionic 2 51
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\psionic 2 51
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

	Type		fem
	  Anim          fem\psionic 2 51
	  PitchStart	13
	  PitchAngle	0.2
	TEnd
MEnd


## PSI BLAST ###########################################################

Move PsiBlast_Pre
	Requires	COMBAT PSIONICBLAST
	Priority        2
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        PsiBlast
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\psionic 2 51
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\psionic 2 51
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

	Type		fem
	  Anim          fem\psionic 2 51
	  PitchStart	13
	  PitchAngle	0.2
	TEnd
MEnd

Move PsiBlast
	Requires	COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        PsiBlast_Post
	Sets            CANTMOVE
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		male
	  Anim          male\psionic 96 121
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\psionic 96 121
	  PitchAngle	0.2
	TEnd

	Type		fem
	  Anim          fem\psionic 96 121
	  PitchAngle	0.2
	TEnd
MEnd

Move PsiBlast_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		male
	  Anim          male\psionic 121
	TEnd

	Type		huge
	  Anim          huge\psionic 121
	TEnd

	Type		fem
	  Anim          fem\psionic 121
	TEnd
MEnd


###################
###### Wall  ######
###################

Move Wall
	Requires	COMBAT WALL
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Wallpost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\wall 2 30
	TEnd

	Type		huge
	  Anim          huge\wall 2 30
	TEnd

	Type		fem
	  Anim          fem\wall 2 30
	TEnd
MEnd

Move Wallpost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\wall 30
	TEnd

	Type		huge
	  Anim          huge\wall 30
	TEnd

	Type		fem
	  Anim          fem\wall 30
	TEnd
MEnd


###################
###### Radius Effect  ######
###################

Move RadiusEffect_Pre
	Requires	COMBAT RADIUSEFFECT
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ> <IRQEmotesCombat> #<MoveIRQ>
	NextMove        RadiusEffect
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\radiuseffect 1 60
	TEnd

	Type		huge
	  Anim          huge\radiuseffect 1 60
	TEnd

	Type		fem
	  Anim          fem\radiuseffect 1 60
	TEnd
MEnd

Move RadiusEffect
	Include sequencers\Members\Readies_Fake.inc
	NextMove        Fake_CombatCycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\radiuseffect 60
	TEnd

	Type		huge
	  Anim          huge\radiuseffect 60
	TEnd

	Type		fem
	  Anim          fem\radiuseffect 60
	TEnd
MEnd

###################
###### Radius Attack  ######
###################

Move RadiusAttack
	Requires	COMBAT RADIUSATTACK
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        PostRadAttack
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\radiusattack 1 30
	TEnd

	Type		huge
	  Anim          huge\radiusattack 1 30
	TEnd

	Type		fem
	  Anim          fem\radiusattack 1 30
	TEnd
MEnd

Move PostRadAttack
	Include sequencers\Members\Readies_Fake.inc
	NextMove        Fake_CombatCycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\radiusattack 30
	TEnd

	Type		huge
	  Anim          huge\radiusattack 30
	TEnd

	Type		fem
	  Anim          fem\radiusattack 30
	TEnd
MEnd

###################
###### Force Ram  ######
###################

Move ForceRam
	Requires	COMBAT FORCE
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        PostForceRam
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\forceram 1 33
	  PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\forceram 1 33
	  PitchAngle	0.1
	TEnd

	Type		fem
	  Anim          fem\forceram 1 33
	  PitchAngle	0.1
	TEnd
MEnd

Move PostForceRam
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatBlend
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		male
	  Anim          male\forceram 33
	  PitchAngle	0.1
	  PitchEnd	44
	TEnd

	Type		huge
	  Anim          huge\forceram 33
	  PitchAngle	0.1
	  PitchEnd	44
	TEnd

	Type		fem
	  Anim          fem\forceram 33
	  PitchAngle	0.1
	  PitchEnd	44
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

	Type		male
	  Anim          male\hand_clap 1 35
	TEnd

	Type		huge
	  Anim          huge\hand_clap 1 35
	TEnd

	Type		fem
	  Anim          fem\hand_clap 1 35
	TEnd
MEnd

Move Cagepost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Scale           0.50
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\hand_clap 35
	TEnd

	Type		huge
	  Anim          huge\hand_clap 35
	TEnd

	Type		fem
	  Anim          fem\hand_clap 35
	TEnd
MEnd


###################
###### EyeCast Windup  ######
###################

Move Pre_EyeWindup
	Requires	COMBAT POWERWINDUP
	Priority        2
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups.inc
	NextMove        EyeWindup
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\eye 1 40
	  PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\eye 1 40
	  PitchAngle	0.1
	TEnd

	Type		fem
	  Anim          fem\eye 1 40
	  PitchAngle	0.1
	TEnd
MEnd

Move EyeWindup
	Requires	COMBAT POWERWINDUP
	Priority        2
	Include sequencers\Members\Windups.inc
	Flags		Cycle NoSizeScale PitchToTarget
	NextMove        EyeWindup_post

	Type		male
	  Anim          male\eye 40 46
	  PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\eye 40 46
	PitchAngle	0.1
	TEnd

	Type		fem
	  Anim          fem\eye 40 46
	  PitchAngle	0.1
	TEnd
MEnd

Move EyeWindup_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatBlend
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		male
	  Anim          male\eye 46
	  PitchEnd	50
	  PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\eye 46
	  PitchEnd	50
	  PitchAngle	0.1
	TEnd

	Type		fem
	  Anim          fem\eye 46
	  PitchEnd	50
	  PitchAngle	0.1
	TEnd
MEnd


## HEADCAST ###########################################################

Move Mouth
	Requires	COMBAT HEADCAST
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Mouth_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\eye 1 50
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\eye 1 50
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		fem
	  Anim          fem\eye 1 50
	  PitchStart	1
	  PitchAngle	0.1
	TEnd
MEnd

#############################################################

Move Mouth_Slow
	Requires	COMBAT HEADCAST HIGH STRONG
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Mouth_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale
	Scale		0.625

	Type		male
	  Anim          male\eye 1 50
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\eye 1 50
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		fem
	  Anim          fem\eye 1 50
	  PitchStart	1
	  PitchAngle	0.1
	TEnd
MEnd

Move Mouth_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatBlend
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\eye 50 60
	TEnd

	Type		huge
	  Anim          huge\eye 50
	TEnd

	Type		fem
	  Anim          fem\eye 50
	TEnd
MEnd


###################
###### mouth_eye blast_SFX - WITH SOUND!  ######
###################

Move Mouth_SFX
	Requires	COMBAT MOUTHSFX
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Mouth_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\eye 1 40
	  PitchStart	1
	  PitchAngle	0.1
	  PlayFX	feet\Male_Rage1.fx
	TEnd

	Type		huge
	  Anim          huge\eye 1 40
	  PitchStart	1
	  PitchAngle	0.1
	  PlayFX	feet\Male_Rage2.fx
	TEnd

	Type		fem
	  Anim          fem\eye 1 40
	  PitchStart	1
	  PitchAngle	0.1
	  PlayFX	feet\FemRage.fx
	TEnd
MEnd


###################
###### LONG mouth_eye blast  ######
###################

Move Mouth_Long
	Requires	COMBAT HIGH HEADCAST
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Mouth_2
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\eye 1 46
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\eye 1 46
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		fem
	  Anim          fem\eye 1 46
	  PitchStart	1
	  PitchAngle	0.1
	TEnd
MEnd

Move Mouth_2
	Requires	COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Mouth_8
	Scale           0.20
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\eye 40 47
	   PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\eye 40 47
	   PitchAngle	0.1
	TEnd

	Type		fem
	  Anim          fem\eye 40 47
	   PitchAngle	0.1
	TEnd
MEnd

Move Mouth_8
	Requires	COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Mouth_Post
	Sets            CANTMOVE
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		male
	  Anim          male\eye 40 49
	   PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\eye 40 49
	   PitchAngle	0.1
	TEnd

	Type		fem
	  Anim          fem\eye 40 49
	   PitchAngle	0.1
	TEnd
MEnd


###################
###### Sky Summon  ######
###################

Move S_Summon
	Requires	COMBAT SKYSUMMON
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        S_Summonpost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\sky_summon 1 60
	TEnd

	Type		huge
	  Anim          huge\sky_summon 1 60
	TEnd

	Type		fem
	  Anim          fem\sky_summon 1 60
	TEnd
MEnd

Move S_Summonpost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatBlend
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\sky_summon 60 73
	TEnd

	Type		huge
	  Anim          huge\sky_summon 60
	TEnd

	Type		fem
	  Anim          fem\sky_summon 60
	TEnd
MEnd

###################
###### HEART OF DARKNESS  ######
###################

Move HeartOfDarkness
	Requires	COMBAT SKYSUMMON SIGNATURE
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        HeartOfDarkness2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\sky_summon 1 50
	TEnd

	Type		huge
	  Anim          huge\sky_summon 1 50
	TEnd

	Type		fem
	  Anim          fem\sky_summon 1 50
	TEnd
MEnd

Move HeartOfDarkness2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatBlend
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\sky_summon 50 73
	TEnd

	Type		huge
	  Anim          huge\sky_summon 50
	TEnd

	Type		fem
	  Anim          fem\sky_summon 50
	TEnd
MEnd

###################
###### Ground Summon  ######
###################

Move G_Summon
	Requires	COMBAT GROUNDSUMMON
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        G_Summonpost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\ground_summon 1 105
	TEnd

	Type		huge
	  Anim          huge\ground_summon 1 105
	TEnd

	Type		fem
	  Anim          fem\ground_summon 1 105
	TEnd
MEnd

Move G_Summonpost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatBlend
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\ground_summon 105
	TEnd

	Type		huge
	  Anim          huge\ground_summon 105
	TEnd

	Type		fem
	  Anim          fem\ground_summon 105
	TEnd
MEnd


###################
###### Hibernate (Boostup + boostaura)   ######
###################

Move Hibernate
	Requires	HIBERNATE
	Priority        1
	Interrupts	<ReactIRQ> <AttackIRQ>
	NextMove        Hibernate_Cycle
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\absorb 1 59
	TEnd

	Type		huge
	  Anim          huge\boostup 1 59
	TEnd

	Type		fem
	  Anim          fem\boostup 1 59
	TEnd
MEnd

Move Hibernate_Cycle
	Requires	HIBERNATE
	Priority        1
	NextMove        Hibernate_Post
	Sets            CANTMOVE
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\boostup_ice_pose 1 2
	TEnd

	Type		huge
	  Anim          huge\boostup_ice 1 2
	TEnd

	Type		fem
	  Anim          fem\boostup_ice 1 2
	TEnd
MEnd

Move Hibernate_Post
	Include sequencers\Members\Combat.inc
	Interpolate	8
	Flags		NoSizeScale

	Type		male
	  Anim          male\flameaura 13
	TEnd

	Type		huge
	  Anim          huge\fireaura 13
	TEnd

	Type		fem
	  Anim          fem\fireaura 13
	TEnd
MEnd


###################
###### Flip boost ######
###################

Move FlipBoost_Pre
	Requires	FIERCE SPIN
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        FlipBoost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\claw_jump_area 1 45
	TEnd

	Type		huge
	  Anim          huge\claw_jump_area 1 45
	TEnd

	Type		fem
	  Anim          fem\claw_jump_area 1 45
	TEnd
MEnd

Move FlipBoost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\claw_jump_area 45
	TEnd

	Type		huge
	  Anim          huge\claw_jump_area 45
	TEnd

	Type		fem
	  Anim          fem\claw_jump_area 45
	TEnd
MEnd

Move FlipBoost2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Scale           0.70
	Interpolate	2
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\comb_postjump 1
	TEnd

	Type		huge
	  Anim          huge\combat_postjump 1
	TEnd

	Type		fem
	  Anim          fem\combat_postjump 1
	TEnd
MEnd


##################################
###### Boost_up (Just for buffs) ######
##################################

Move Boostup
	Requires	BOOSTUP
	Priority        1
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ> <BlockIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove        Boostuppost
	Flags		NoSizeScale

	Type		male
	  Anim          male\absorb 1 10
	TEnd

	Type		huge
	  Anim          huge\boostup 1 10
	TEnd

	Type		fem
	  Anim          fem\boostup 1 10
	TEnd

	Type		Wisp
	  Anim          Wisp\headcast_AOE 1 104
	TEnd
MEnd

Move Boostuppost
	Member		<DeathIRQ> <AttackIRQ> <BlockIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove        BoostupReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\absorb 11
	TEnd

	Type		huge
	  Anim          huge\boostup 11
	TEnd

	Type		fem
	  Anim          fem\boostup 11
	TEnd

	Type		Wisp
	  Anim          Wisp\headcast_AOE 105
	TEnd
MEnd

Move BoostupReady
	Include sequencers\Members\Readies.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\Skel_ready2 1 60
	TEnd

	Type		huge
	  Anim          huge\mon_ready 1 60
	TEnd

	Type		fem
	  Anim          fem\Skel_ready2 1 60
	TEnd

	Type		Wisp
	  Anim          Wisp\float 1
	TEnd
MEnd


###################
###### Boost_down  ######
###################

Move Boost_D
	Requires	BOOSTDOWN
	Priority        1
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ> <BlockIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove        Boost_Dpost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\boost 1 10
	TEnd

	Type		huge
	  Anim          huge\boostdown 1 10
	TEnd

	Type		fem
	  Anim          fem\boostdown 1 10
	TEnd
MEnd

Move Boost_Dpost
	Member		<DeathIRQ> <AttackIRQ> <BlockIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove        BoostDnReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\boost 10
	TEnd

	Type		huge
	  Anim          huge\boostdown 10
	TEnd

	Type		fem
	  Anim          fem\boostdown 10
	TEnd
MEnd

Move BoostDnReady
	Include sequencers\Members\Readies.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\Skel_ready2 1 60
	TEnd

	Type		huge
	  Anim          huge\mon_ready 1 60
	TEnd

	Type		fem
	  Anim          fem\Skel_ready2 1 60
	TEnd
MEnd


###################
###### magic cast Left  ######
###################

Move Magic_L
	Requires	COMBAT MAGIC CAST
	Priority        3
	Interrupts	<AttackIRQ> <Windups> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Magic_Lpost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\magic_L 1 30
	  PitchStart	10
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\magic_L 1 30
	  PitchStart	10
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\magic_L 1 30
	  PitchStart	10
	  PitchAngle	0.5
	TEnd
MEnd

Move Magic_Lpost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatBlend
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		male
	  Anim          male\magic_L 30 66
	  PitchEnd	60
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\magic_L 30
	  PitchEnd	60
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\magic_L 30
	  PitchEnd	60
	  PitchAngle	0.5
	TEnd
MEnd


###################
###### Steal power  ######
###################

Move Steal
	Requires	COMBAT STEAL
	Priority        2
	Interrupts	<AttackIRQ> <Windups>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Stealpost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\graspearth 1 58
	  PitchStart	8
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\graspearth 1 58
	  PitchStart	8
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\steal 1 58
	  PitchStart	8
	  PitchAngle	0.5
	TEnd
MEnd

Move Stealpost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\graspearth 58
	TEnd

	Type		huge
	  Anim          huge\graspearth 58
	TEnd

	Type		fem
	  Anim          fem\steal 58
	TEnd
MEnd


###################
###### Bigger Steal power  ######
###################

Move BigSteal
	Requires	COMBAT STRONG STEAL
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        BigStealpost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\new_steal 1 125
	  PitchStart	10
	  PitchEnd	109
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\new_steal 1 125
	  PitchStart	10
	  PitchEnd	109
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\new_steal 1 125
	  PitchStart	10
	  PitchEnd	109
	  PitchAngle	0.5
	TEnd
MEnd

Move BigStealpost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\new_steal 125
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\new_steal 125
	TEnd

	Type		fem
	  Anim          fem\new_steal 125
	TEnd
MEnd


###################
###### Heal cast  ######
###################

Move Heal
	Requires	COMBAT HEAL
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Healpost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\heal 1 35
	  PitchStart	8
	  PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\heal 1 35
	  PitchStart	8
	  PitchAngle	0.1
	TEnd

	Type		fem
	  Anim          fem\heal 1 35
	  PitchStart	8
	  PitchAngle	0.1
	TEnd
MEnd

Move Healpost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\heal 35
	  PitchEnd	63
	  PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\heal 35
	  PitchEnd	63
	  PitchAngle	0.1
	TEnd

	Type		fem
	  Anim          fem\heal 35
	  PitchEnd	63
	  PitchAngle	0.1
	TEnd
MEnd


###################
###### Astral sit ######
###################

Move Astral_getup
	Include sequencers\Members\Readies.inc
	NextMove        Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\astro 75
	TEnd
#
#	Type		huge
#	  Anim          huge\thumbsup 1
#	TEnd
#
#	Type		fem
#	  Anim          fem\thumbsup 1
#	TEnd
MEnd

###################
###### Stone Armor  ######
###################

Move Stonearmor
	Requires	STONEARMOR
	Priority        1
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Stonearmor2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\stonearmor 1 100
	TEnd

	Type		huge
	  Anim          huge\stonearmor 1 100
	TEnd

	Type		fem
	  Anim          fem\stonearmor 1 100
	TEnd
MEnd

Move Stonearmor2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\stonearmor 100
	TEnd

	Type		huge
	  Anim          huge\stonearmor 100
	TEnd

	Type		fem
	  Anim          fem\stonearmor 100
	TEnd
MEnd


###################
###### Stomp  ######
###################

Move Stomp
	Requires	COMBAT STOMP
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Stomp2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\stomp 1 62
	TEnd

	Type		huge
	  Anim          huge\stomp 1 62
	TEnd

	Type		fem
	  Anim          fem\stomp 1 62
	TEnd

	Type		RockSuit
	  Anim          Beast\stomp 1 62
	TEnd
MEnd

Move Stomp2
	Include sequencers\Members\Readies_Fake.inc
	NextMove        Fake_CombatCycle_Half
	Flags		NoSizeScale

	Type		male
	  Anim          male\stomp 62
	TEnd

	Type		huge
	  Anim          huge\stomp 62
	TEnd

	Type		fem
	  Anim          fem\stomp 62
	TEnd

	Type		RockSuit
	  Anim          beast\stomp 62
	TEnd
MEnd

## 'BRAWL' PUNCH - RIGHT ###########################################################

Move Brawl_R
	Requires	COMBAT PUNCH #Don't use BRAWL for this
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Brawlpost_R
	Sets            CANTMOVE
	Flags		NoSizeScale
	SticksOnChild   PUNCH RIGHT

	Type		male
	  Anim          male\brawl_high2 1 25
	  PlayFx        POWERS\POWERPUNCH\PUNCHWHOOSH2.FX
	TEnd
MEnd

Move Brawlpost_R
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Brawlcycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\brawl_high2 25 48
	TEnd
MEnd

## 'BRAWL' PUNCH - LEFT ###########################################################

Move Brawl_L
	Requires	COMBAT PUNCH #Don't use BRAWL for this
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Brawlpost_L
	Sets            CANTMOVE
	Flags		NoSizeScale
	SticksOnChild   PUNCH LEFT

	Type		male
	  Anim          male\brawl_high2 51 75
	  PlayFx        POWERS\POWERPUNCH\PUNCHWHOOSH2.FX
	TEnd
MEnd

Move Brawlpost_L
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Brawlcycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\brawl_high2 75
	TEnd
MEnd

## 'BRAWL' CYCLE ###########################################################

Move Brawlcycle_15
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Brawlcycle
	Scale           0.50
	Flags		NoSizeScale

	Type		male
	  Anim          male\brawlcycle 15 31
	TEnd

	Type		huge
	  Anim          huge\brawlcycle 15 31
	TEnd

	Type		fem
	  Anim          fem\brawlcycle 15 31
	TEnd
MEnd

#############################################################

Move Brawlcycle
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		Cycle NoSizeScale
	NextMove        postReady_back
	Scale           0.50

	Type		male
	  Anim          male\brawlcycle 1 31
	TEnd

	Type		huge
	  Anim          huge\brawlcycle 1 31
	TEnd

	Type		fem
	  Anim          fem\brawlcycle 1 31
	TEnd
MEnd

###############
## 1 2 punch ##
###############

Move 1_2punch
	Requires	COMBAT PUNCH COMBO2
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        1_2punchpost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\1_2punch 1 40
	  PlayFx        POWERS\POWERPUNCH\PUNCHWHOOSH3.FX
	TEnd

	Type		huge
	  Anim          huge\1_2punch 1 40
	  PlayFx        POWERS\POWERPUNCH\PUNCHWHOOSH3.FX
	TEnd

	Type		fem
	  Anim          fem\1_2punch 1 40
	TEnd
MEnd

Move 1_2punchpost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Brawlcycle_15
	Scale           0.50
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\1_2punch 40
	TEnd

	Type		huge
	  Anim          huge\1_2punch 40
	TEnd

	Type		fem
	  Anim          fem\1_2punch 40
	TEnd
MEnd


###################
###### Gut Punch ######
###################

Move Gutpunch
	Requires	COMBAT GUTPUNCH
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Gutpunch_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\gut_punch 1 31
	  PlayFX		NPCFX\NPC_WhooshBig_SFX.fx 7
	TEnd

	Type		huge
	  Anim          huge\gut_punch 1 31
	  PlayFX		NPCFX\NPC_WhooshBig_SFX.fx 7
	TEnd

	Type		fem
	  Anim          fem\gut_punch 1 31
	  PlayFX		NPCFX\NPC_WhooshBig_SFX.fx 7
	TEnd
MEnd

Move Gutpunch_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Brawlcycle_15
	Scale           0.50
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\gut_punch 31
	TEnd

	Type		huge
	  Anim          huge\gut_punch 31
	TEnd

	Type		fem
	  Anim          fem\gut_punch 31
	TEnd
MEnd


###################
###### Doorsmash Kick ######
###################

Move DoorKick
	Requires	COMBAT STRONG KICK
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        DoorKick_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\doorsmash_kick 1 55
	TEnd

	Type		huge
	  Anim          huge\doorsmash_kick 1 55
	TEnd

	Type		fem
	  Anim          fem\doorsmash_kick 1 55
	TEnd
MEnd

Move DoorKick_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\doorsmash_kick 55
	TEnd

	Type		huge
	  Anim          huge\doorsmash_kick 55
	TEnd

	Type		fem
	  Anim          fem\doorsmash_kick 55
	TEnd
MEnd


###################
###### Weave ######
###################

Move Weave_pre
	Requires	PREMODE BRAWL
	Priority        2
	Interrupts	<ReactIRQ>
	Member		<DeathIRQ> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove        Weave
	Flags		NoSizeScale

	Type		male
	  Anim          male\weave 86 88
	  PlayFX	Feet\Weave.fx 2
	TEnd

	Type		huge
	  Anim          huge\weave 86 88
	  PlayFX	Feet\Weave.fx 2
	TEnd

	Type		fem
	  Anim          fem\weave 86 88
	  PlayFX	Feet\Weave.fx 2
	TEnd
MEnd

Move Weave
	Include sequencers\Members\Combat.inc
	NextMove	Weave_cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\weave 88 142
	  PlayFX	Feet\Weave.fx 5
	  PlayFX	Feet\Weave.fx 52
	TEnd

	Type		huge
	  Anim          huge\weave 88 142
	  PlayFX	Feet\Weave.fx 5
	  PlayFX	Feet\Weave.fx 52
	TEnd

	Type		fem
	  Anim          fem\weave 88 142
	  PlayFX	Feet\Weave.fx 5
	  PlayFX	Feet\Weave.fx 52
	TEnd
MEnd

Move Weave_cycle
	Include sequencers\Members\Combat.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\weave 35 142
	  PlayFX	Feet\Weave.fx 1
	  PlayFX	Feet\Weave.fx 52
	  PlayFX	Feet\Weave.fx 60
	TEnd

	Type		huge
	  Anim          huge\weave 35 142
	  PlayFX	Feet\Weave.fx 1
	  PlayFX	Feet\Weave.fx 52
	  PlayFX	Feet\Weave.fx 60
	TEnd

	Type		fem
	  Anim          fem\weave 35 142
	  PlayFX	Feet\Weave.fx 1
	  PlayFX	Feet\Weave.fx 52
	  PlayFX	Feet\Weave.fx 60
	TEnd


MEnd


###################
###### jumpNsmash ######
###################

Move jumpNsmash
	Requires	COMBAT JUMPNSMASH
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        jumpNsmash_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\jump_n_smash 1 98
	TEnd

	Type		huge
	  Anim          huge\jump_n_smash 1 98
	TEnd

	Type		fem
	  Anim          fem\jump_n_smash 1 98
	TEnd

	Type		RockSuit
	  Anim          beast\jump_n_smash 1 98
	TEnd
MEnd

Move jumpNsmash_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        jumpNsmash_cycle_15
	Scale           0.50
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\jump_n_smash 98
	TEnd

	Type		huge
	  Anim          huge\jump_n_smash 98
	TEnd

	Type		fem
	  Anim          fem\jump_n_smash 98
	TEnd

	Type		RockSuit
	  Anim          beast\jump_n_smash 98
	TEnd
MEnd

#############################################################

Move jumpNsmash_cycle
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		Cycle NoSizeScale
	NextMove        postReady_back
	Scale           0.50

	Type		male
	  Anim          male\claw_cycle 1 31
	TEnd

	Type		huge
	  Anim          huge\claw_cycle 1 31
	TEnd

	Type		fem
	  Anim          fem\claw_cycle 1 31
	TEnd
MEnd

#############################################################

Move jumpNsmash_cycle_15
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        jumpNsmash_cycle
	Scale           0.50
	Flags		NoSizeScale

	Type		male
	  Anim          male\claw_cycle 15 31
	TEnd

	Type		huge
	  Anim          huge\claw_cycle 15 31
	TEnd

	Type		fem
	  Anim          fem\claw_cycle 15 31
	TEnd
MEnd


###################
###### Baseball punch ######
###################

Move BBpunch
	Requires	COMBAT BASEBALLPUNCH
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        BBpunchpost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\baseball_punch 1 44
	TEnd

	Type		huge
	  Anim          huge\baseball_punch 1 44
	TEnd

	Type		fem
	  Anim          fem\baseball_punch 1 44
	TEnd
MEnd

Move BBpunchpost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Scale           0.50
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\baseball_punch 44
	TEnd

	Type		huge
	  Anim          huge\baseball_punch 44
	TEnd

	Type		fem
	  Anim          fem\baseball_punch 44
	TEnd
MEnd


###################
###### Short Toss ######
###################

Move ShortToss_Squat
	Requires	COMBAT SHORTTOSS
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        ShortLift
	Sets            CANTMOVE
	Flags		NoSizeScale FixArmReg
	Scale           1.5

	Type		male
	  Anim          male\short_toss 1 30
	TEnd

	Type		huge
	  Anim          huge\short_toss 1 30
	TEnd

	Type		fem
	  Anim          fem\short_toss 1 30
	TEnd
MEnd

Move ShortLift
	Requires	COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        ShortToss
	Sets            CANTMOVE
	Flags		NoSizeScale FixArmReg
	Scale           1.625

	Type		male
	  Anim          male\short_toss 30 85
	TEnd

	Type		huge
	  Anim          huge\short_toss 30 85
	TEnd

	Type		fem
	  Anim          fem\short_toss 30 85
	TEnd
MEnd

Move ShortToss
	Requires	COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        ShortToss_post
	Sets            CANTMOVE
	Flags		NoSizeScale FixArmReg
	Scale           1.3

	Type		male
	  Anim          male\short_toss 85 115
	TEnd

	Type		huge
	  Anim          huge\short_toss 85 115
	TEnd

	Type		fem
	  Anim          fem\short_toss 85 115
	TEnd
MEnd

Move ShortToss_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Scale           0.50
	Flags		NoSizeScale ReqInputs FixArmReg

	Type		male
	  Anim          male\short_toss 115
	TEnd

	Type		huge
	  Anim          huge\short_toss 115
	TEnd

	Type		fem
	  Anim          fem\short_toss 115
	TEnd
MEnd


###################
###### Windup Uppercut ######
###################

Move WindUpcut
	Requires	COMBAT WINDUPPERCUT
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        WindUpcut_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\windup_upcut 1 65
	TEnd

	Type		huge
	  Anim          huge\windup_upcut 1 65
	TEnd

	Type		fem
	  Anim          fem\windup_upcut 1 65
	TEnd
MEnd

Move WindUpcut_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Scale           0.50
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\windup_upcut 65
	TEnd

	Type		huge
	  Anim          huge\windup_upcut 65
	TEnd

	Type		fem
	  Anim          fem\windup_upcut 65
	TEnd
MEnd


###################
###### Stop cast ######
###################

Move Stop
	Requires	COMBAT STOPCAST
	Priority        2
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Stop_cycle
	Flags		PitchToTarget NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\stop 1 31
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\stop 1 31
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\stop 1 31
	  PitchAngle	0.5
	TEnd
MEnd

Move Stop_cycle
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Stop_post
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		male
	  Anim          male\stop 31 60
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\stop 31 60
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\stop 31 60
	  PitchAngle	0.5
	TEnd
MEnd

Move Stop_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Scale           0.50
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		male
	  Anim          male\stop 60
	  PitchEnd	64
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\stop 60
	  PitchEnd	64
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\stop 60
	  PitchEnd	64
	  PitchAngle	0.5
	TEnd
MEnd

###################
###### Blaster Ranged Darkblast Gloom ######
###################

Move Darkblast_Gloom
	Requires	COMBAT STOPCAST BLAST
	Priority        3
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Darkblast_Gloom_post
	Flags		PitchToTarget NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\Darkblast_Gloom 1 50
	TEnd
#
#	Type		huge
#	  Anim          huge\Darkblast_Gloom 1 65
#	TEnd
#
#	Type		fem
#	  Anim          fem\Darkblast_Gloom 1 65
#	TEnd
MEnd

Move Darkblast_Gloom_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Scale           0.5
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\Darkblast_Gloom 51
	TEnd

#	Type		huge
#	  Anim          huge\windup_upcut 65
#	TEnd
#
#	Type		fem
#	  Anim          fem\windup_upcut 65
#	TEnd
MEnd

###################
###### Scream ######
###################

Move Scream
	Requires	COMBAT SCREAM
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Scream_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\scream 10 58
	    PlayFX	feet\MaleTaunt2.fx
	TEnd

	Type		huge
	  Anim          huge\scream 10 58
	  PlayFX	feet\MaleTaunt7.fx
	TEnd

	Type		fem
	  Anim          fem\scream 10 58
	  PlayFX	feet\femTaunt2.fx
	TEnd

	Type		mako
	  Anim          male\scream_jaw 10 58
	    PlayFX	feet\MaleTaunt2.fx
	TEnd
MEnd

Move Scream_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Scream_cycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\scream 58
	TEnd

	Type		huge
	  Anim          huge\scream 58
	TEnd

	Type		fem
	  Anim          fem\scream 58
	TEnd

	Type		mako
	  Anim          male\scream_jaw 58
	TEnd
MEnd

Move Scream_cycle
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		Cycle ReqInputs NoSizeScale
	NextMove        postReady_back

	Type		male
	  Anim          male\comb_beatchest 95 144
	TEnd

	Type		huge
	  Anim          huge\comb_beatchest 95 144
	TEnd

	Type		fem
	  Anim          fem\comb_beatchest 95 144
	TEnd
MEnd

###################
###### Terrify ######
###################

Move Terrify
	Requires	COMBAT TERRIFY
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Terrify_post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\terrify 1 60
	TEnd

	Type		huge
	  Anim          huge\terrify 1 60
	TEnd

	Type		fem
	  Anim          fem\terrify 1 60
	TEnd
MEnd

Move Terrify_post
	Include sequencers\Members\Readies_Fake.inc
	NextMove        Fake_Combat_half
	Scale           0.50
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\terrify 60
	  PitchEnd	63
	TEnd

	Type		huge
	  Anim          huge\terrify 60
	  PitchEnd	63
	TEnd

	Type		fem
	  Anim          fem\terrify 60
	  PitchEnd	63
	TEnd
MEnd


###################
###### Energy Torrent ######
###################

Move Energy_Torrent
	Requires	COMBAT ENERGYTORRENT
	Priority        2
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        E_Torrent_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\energy_torrent 1 65
	TEnd

	Type		huge
	  Anim          huge\energy_torrent 1 65
	TEnd

	Type		fem
	  Anim          fem\energy_torrent 1 65
	TEnd
MEnd

Move E_Torrent_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Scale           0.50
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\energy_torrent 65
	TEnd

	Type		huge
	  Anim          huge\energy_torrent 65
	TEnd

	Type		fem
	  Anim          fem\energy_torrent 65
	TEnd
MEnd


###################
###### hypnotize ######
###################

Move Hypnotize
	Requires	COMBAT HYPNOTIZE
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Hypnotize2
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale
	Scale           1.50

	Type		male
	  Anim          male\hypnotize 1 90
	TEnd

	Type		huge
	  Anim          huge\hypnotize 1 90
	TEnd

	Type		fem
	  Anim          fem\hypnotize 1 90
	TEnd
MEnd

Move Hypnotize2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Hypnotize_post
	Flags		PitchToTarget ReqInputs NoSizeScale
	Scale           1.50

	Type		male
	  Anim          male\hypnotize 90 175
	  PitchEnd	167
	TEnd

	Type		huge
	  Anim          huge\hypnotize 90 175
	  PitchEnd	167
	TEnd

	Type		fem
	  Anim          fem\hypnotize 90 175
	  PitchEnd	167
	TEnd
MEnd

Move Hypnotize_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale ReqInputs
	Scale           0.50

	Type		male
	  Anim          male\hypnotize 175
	TEnd

	Type		huge
	  Anim          huge\hypnotize 175
	TEnd

	Type		fem
	  Anim          fem\hypnotize 175
	TEnd
MEnd


###################
###### Superpunch ######
###################

Move Superpunch_R
	Requires	COMBAT QUICK PUNCH HAND
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        S_punchpost_R
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\superpunch_low 1 15
	TEnd

	Type		huge
	  Anim          huge\superpunch 1 15
	TEnd

	Type		fem
	  Anim          fem\superpunch 1 15
	TEnd
MEnd

Move S_punchpost_R
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Spunchcycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\superpunch_low 15 48
	TEnd

	Type		huge
	  Anim          huge\superpunch 15 48
	TEnd

	Type		fem
	  Anim          fem\superpunch 15 48
	TEnd
MEnd

#############################################################

Move Superpunch_L
	Requires	COMBAT QUICK PUNCH HAND
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        S_punchpost_L
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\superpunch_low 50 65
	TEnd

	Type		huge
	  Anim          huge\superpunch 50 65
	TEnd

	Type		fem
	  Anim          fem\superpunch 50 65
	TEnd
MEnd

Move S_punchpost_L
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Spunchcycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\superpunch_low 65
	TEnd

	Type		huge
	  Anim          huge\superpunch 65
	TEnd

	Type		fem
	  Anim          fem\superpunch 65
	TEnd
MEnd

#############################################################

Move Spunchcycle
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		Cycle NoSizeScale
	NextMove        postReady_back

	Type		male
	  Anim          male\brawlcycle 1
	TEnd

	Type		huge
	  Anim          huge\brawlcycle 1 31
	TEnd

	Type		fem
	  Anim          fem\brawlcycle 1 31
	TEnd
MEnd


###################
###### 1 Hand Smash  ######
###################

Move OneHandSmash_Right
	Requires	COMBAT PUNCH SMASH
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        OneHandSmash_Right2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\1H_smash 12 36
	TEnd

	Type		huge
	  Anim          huge\1H_smash 12 36
	TEnd

	Type		fem
	  Anim          fem\1H_smash 12 36
	TEnd
MEnd

Move OneHandSmash_Right2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        OneHandSmash_Right_Post
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\1H_smash 36 59
	TEnd

	Type		huge
	  Anim          huge\1H_smash 36 59
	TEnd

	Type		fem
	  Anim          fem\1H_smash 36 59
	TEnd
MEnd

Move OneHandSmash_Right_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Scale           0.50
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\1H_smash 59 82
	TEnd

	Type		huge
	  Anim          huge\1H_smash 59 82
	TEnd

	Type		fem
	  Anim          fem\1H_smash 59 82
	TEnd
MEnd

#############################################################

Move OneHandSmash_Left
	Requires	COMBAT PUNCH SMASH
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        OneHandSmash_Left2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\1H_smash 96 120
	TEnd

	Type		huge
	  Anim          huge\1H_smash 96 120
	TEnd

	Type		fem
	  Anim          fem\1H_smash 96 120
	TEnd
MEnd

Move OneHandSmash_Left2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        OneHandSmash_Left_Post
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\1H_smash 120 140
	TEnd

	Type		huge
	  Anim          huge\1H_smash 120 140
	TEnd

	Type		fem
	  Anim          fem\1H_smash 120 140
	TEnd
MEnd

Move OneHandSmash_Left_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Scale           0.50
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\1H_smash 140
	TEnd

	Type		huge
	  Anim          huge\1H_smash 140
	TEnd

	Type		fem
	  Anim          fem\1H_smash 140
	TEnd
MEnd


###################
###### Flurry o Fists  ######
###################

Move Flurry
	Requires	COMBAT PUNCH SPEEDPUNCH
	Priority        3
	Interrupts	<AttackIRQ> Speed_react
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Flurrypost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\speedattack 1 92
	TEnd

	Type		huge
	  Anim          huge\speedattack 1 92
	TEnd

	Type		fem
	  Anim          fem\speedattack 1 92
	TEnd
MEnd

Move Flurrypost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\speedattack 92
	TEnd

	Type		huge
	  Anim          huge\speedattack 92
	TEnd

	Type		fem
	  Anim          fem\speedattack 92
	TEnd
MEnd


###################
###### Leap Kick ######
###################

Move LeapKick
	Requires	COMBAT LEAPKICK
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        LeapKickMid
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\leapkick 1 46
	  PlayFx        FEET\LAND1.FX 42
	TEnd

	Type		huge
	  Anim          huge\leapkick 1 46
	  PlayFx        FEET\HUGELAND2.FX 42
	TEnd

	Type		fem
	  Anim          fem\leapkick 1 46
	  PlayFx        FEET\LAND1.FX 42
	TEnd
MEnd

Move LeapKickMid
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        LeapKickpost
	Flags		NoSizeScale ReqInputs
	Scale		4

	Type		male
	  Anim          male\leapkick 46 71
	  PlayFx        FEET\LAND1.FX
	TEnd

	Type		huge
	  Anim          huge\leapkick 46 71
	  PlayFx        FEET\HUGELAND2.FX
	TEnd

	Type		fem
	  Anim          fem\leapkick 46 71
	  PlayFx        FEET\LAND1.FX
	TEnd
MEnd

Move LeapKickpost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\leapkick 71
	  PlayFx        FEET\LAND1.FX
	TEnd

	Type		huge
	  Anim          huge\leapkick 71
	  PlayFx        FEET\HUGELAND2.FX
	TEnd

	Type		fem
	  Anim          fem\leapkick 71
	  PlayFx        FEET\LAND1.FX
	TEnd
MEnd


#########################
## Martial Arts - Kick ##
#########################

Move MA_Kick
	Requires	COMBAT KUNGFU KICK
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        MA_Kick_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\MA_kick 1 25
	TEnd

	Type		huge
	  Anim          huge\MA_kick 1 25
	TEnd

	Type		fem
	  Anim          fem\MA_kick 1 25
	TEnd
MEnd

Move MA_Kick_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\MA_kick 25
	TEnd

	Type		huge
	  Anim          huge\MA_kick 25
	TEnd

	Type		fem
	  Anim          fem\MA_kick 25
	TEnd
MEnd


###################
###### MA Shadow kick ######
###################

Move MA_Shadowkick
	Requires	COMBAT KUNGFU KUNGFUSPEED
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        MA_Shadowkick_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\MA_shadowkick 1 125
	TEnd

	Type		huge
	  Anim          huge\MA_shadowkick 1 125
	TEnd

	Type		fem
	  Anim          fem\MA_shadowkick 1 125
	TEnd
MEnd

Move MA_Shadowkick_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Flags		ReqInputs

	Type		male
	  Anim          male\MA_shadowkick 125
	TEnd

	Type		huge
	  Anim          huge\MA_shadowkick 125
	TEnd

	Type		fem
	  Anim          fem\MA_shadowkick 125
	TEnd
MEnd


###########################
###### MA roundhouse kick ######
##########################

Move MA_Roundhouse
	Requires	COMBAT KUNGFU KICK STRONG
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        MA_Roundhouse_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\kick_highspin 1 25
	TEnd
MEnd

Move MA_Roundhouse_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        MA_Roundhouse_PostCycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\kick_highspin 25
	TEnd
MEnd

Move MA_Roundhouse_PostCycle
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		Cycle NoSizeScale ReqInputs
	NextMove        postReady_back
	Scale           0.50

	Type		male
	  Anim          male\kick_cycle 1
	TEnd
MEnd


###################
###### MA Palm stun ######
###################

Move MA_Palmstun
	Requires	COMBAT KUNGFU PUNCH KUNGFUSTUN
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        MA_Palmstun_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\MA_palmstun 35 85
	TEnd

	Type		huge
	  Anim          huge\MA_palmstun 35 85
	TEnd

	Type		fem
	  Anim          fem\MA_palmstun 35 85
	TEnd
MEnd

Move MA_Palmstun_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\MA_palmstun 85
	TEnd

	Type		huge
	  Anim          huge\MA_palmstun 85
	TEnd

	Type		fem
	  Anim          fem\MA_palmstun 85
	TEnd
MEnd


###################
###### MA Sweep kick ######
###################

Move MA_Sweepkick
	Requires	COMBAT KUNGFU KUNGFUAREA
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        MA_Sweepkick_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\short_sweepkick 1 45
	TEnd

	Type		huge
	  Anim          huge\short_sweepkick 1 45
	TEnd

	Type		fem
	  Anim          fem\short_sweepkick 1 45
	TEnd
MEnd

Move MA_Sweepkick_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\short_sweepkick 45
	TEnd

	Type		huge
	  Anim          huge\short_sweepkick 45
	TEnd

	Type		fem
	  Anim          fem\short_sweepkick 45
	TEnd
MEnd

###################
###### MA Hobble kick ######
###################

Move MA_Hobblekick
	Requires	COMBAT KUNGFU LOW KICK FEET
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        MA_Hobblekick2
	Sets            CANTMOVE
	Flags		NoSizeScale
	Scale		1.25

	Type		male
	  Anim          male\MA_hobble 1 60
	TEnd

	Type		huge
	  Anim          huge\MA_hobble 1 60
	TEnd

	Type		fem
	  Anim          fem\MA_hobble 1 60
	TEnd
MEnd

Move MA_Hobblekick2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Flags		NoSizeScale ReqInputs
	Scale		1.125

	Type		male
	  Anim          male\MA_hobble 60
	TEnd

	Type		huge
	  Anim          huge\MA_hobble 60
	TEnd

	Type		fem
	  Anim          fem\MA_hobble 60
	TEnd
MEnd

###################
###### MA Stun kick ######
###################

Move MA_Stunkick
	Requires	COMBAT KUNGFU KICK KUNGFUSTUN
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        MA_Stunkick2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\MA_stunkick 25 100
	  PlayFx        FEET\LAND1.FX 80
	TEnd

	Type		huge
	  Anim          huge\MA_stunkick 25 100
	  PlayFx        FEET\HUGELAND2.FX 80
	TEnd

	Type		fem
	  Anim          fem\MA_stunkick 25 100
	  PlayFx        FEET\LAND1.FX 80
	TEnd
MEnd

Move MA_Stunkick2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		ReqInputs

	Type		male
	  Anim          male\MA_stunkick 100
	TEnd

	Type		huge
	  Anim          huge\MA_stunkick 100
	TEnd

	Type		fem
	  Anim          fem\MA_stunkick 100
	TEnd
MEnd

###################
###### MA Iron kick ######
###################

Move MA_Ironkick
	Requires	COMBAT KUNGFU HIGH KICK
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        MA_Ironkick_Post
	Interpolate	8
	Sets            CANTMOVE
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim          male\MA_ironkick 34 83
	TEnd

	Type		huge
	  Anim          huge\MA_ironkick 34 83
	TEnd

	Type		fem
	  Anim          fem\MA_ironkick 34 83
	TEnd
MEnd

Move MA_Ironkick_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Wind		ATTACK
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\MA_ironkick 83
	TEnd

	Type		huge
	  Anim          huge\MA_ironkick 83
	TEnd

	Type		fem
	  Anim          fem\MA_ironkick 83
	TEnd
MEnd

############################
##### Mart Backstab Windup ####
############################

Move MartWindup
	Requires	COMBAT WINDUPMARTBACKSTAB
	Priority        2
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups.inc
	NextMove        MartWindup_Cycle
	Flags		NoSizeScale PitchToTarget

	Type		male
	  Anim          male\mart_backstab 1 62
	TEnd
MEnd

Move MartWindup_Cycle
	Requires	COMBAT WINDUPMARTBACKSTAB
	Include sequencers\Members\Windups.inc
	Flags		Cycle NoSizeScale PitchToTarget

	Type		male
	  Anim          male\mart_backstab 65 115
	TEnd
MEnd

#############################################################

Move MartBackstab
	Requires	COMBAT MARTBACKSTAB
	Priority	2
	Interrupts	<AttackIRQ> <Windups>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        MartBackstab2
	Sets            CANTMOVE
	Flags		NoSizeScale PitchToTarget

	Type		male
	  Anim          male\mart_backstab 120 145
	TEnd
MEnd

Move MartBackstab2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Lazy_CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\mart_backstab 145
	TEnd
MEnd

##################
##### Placate ####
##################

Move Placate
	Requires	COMBAT PLACATE
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Placate2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\placate 1 40
	TEnd
MEnd

Move Placate2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Lazy_CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\placate 40
	TEnd
MEnd


#####################
######## Spin  ###### this is the new version of whirlwhind - anim is interuptable
#####################

Move Pre_Spin
	Requires	WHIRLWIND QUICK #COMBAT WEAPON DUAL SHIELD
	Priority        6
	Interrupts	<AttackIRQ> <MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Spin_cycle
	Wind		FREEZE
	Flags		NoSizeScale
	Sets		CANTMOVE

	Type		male
	  Anim          male\spin 1 45
	TEnd

	Type		huge
	  Anim          huge\spin 1 45
	TEnd

	Type		fem
	  Anim          fem\spin 1 45
	TEnd
MEnd

Move Spin_quick
	Requires	WHIRLWIND
	Priority        1
	Interrupts	<MoveIRQ>
	Member		<DeathIRQ> <AttackIRQ> <ReactIRQ> <HitIRQ> #<MoveIRQ> <IRQEmotesCombat>
	NextMove        Spin_cycle
	Wind		FREEZE
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\spin 1 25
	TEnd

	Type		huge
	  Anim          huge\spin 1 25
	TEnd

	Type		fem
	  Anim          fem\spin 1 25
	TEnd
MEnd

Move Spin_cycle
	Requires	WHIRLWIND
	Member		<DeathIRQ> <AttackIRQ> <ReactIRQ> <HitIRQ> #<MoveIRQ> <IRQEmotesCombat>
	NextMove        JumpPost
	Wind		FREEZE
	Flags		NoSizeScale Cycle

	Type		male
	  Anim          male\spin 25 53
	TEnd

	Type		huge
	  Anim          huge\spin 25 53
	TEnd

	Type		fem
	  Anim          fem\spin 25 53
	TEnd
MEnd

##################
###### Throw Knife  ######
###################

Move ThrowKnife_Pre
	Requires	COMBAT THROWKNIFE
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        ThrowKnife
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\wm_throwknife 1 22
	TEnd

	Type		huge
	  Anim          huge\wm_throwknife 1 22
	TEnd

	Type		fem
	  Anim          fem\wm_throwknife 1 22
	TEnd
MEnd

Move ThrowKnife
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        ThrowKnife_Post
	Scale           0.375
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\wm_throwknife 22 31
	TEnd

	Type		huge
	  Anim          huge\wm_throwknife 22 31
	TEnd

	Type		fem
	  Anim          fem\wm_throwknife 22 31
	TEnd
MEnd

Move ThrowKnife_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Scale           0.5
	Flags		ReqInputs

	Type		male
	  Anim          male\wm_throwknife 31
	TEnd

	Type		huge
	  Anim          huge\wm_throwknife 31
	TEnd

	Type		fem
	  Anim          fem\wm_throwknife 31
	TEnd
MEnd

#############################################################

Move ThrowProjectile_Pre
	Requires	COMBAT THROWKNIFE ROOTED
	Priority        3
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        ThrowProjectile
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\wm_throwknife 1 21
	TEnd
MEnd

Move ThrowProjectile
	Requires	COMBAT
	Member		<DeathIRQ> <AttackIRQ> <MoveIRQ>
	NextMove        ThrowProjectile_Post
	Scale           0.375
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\wm_throwknife 21 31
	TEnd
MEnd

Move ThrowProjectile_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Scale           0.5
	Flags		ReqInputs

	Type		male
	  Anim          male\wm_throwknife 31
	TEnd
MEnd

###################
###### Boomerang  ######
###################

Move Boomerang
	Requires	COMBAT WEAPON BOOMERANG
	Priority        3
	Interrupts	<AttackIRQ> <Block>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Boomerangpost
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\boomerang 1 55
	TEnd

	Type		huge
	  Anim          huge\boomerang 1 55
	TEnd

	Type		fem
	  Anim          fem\boomerang 1 55
	TEnd
MEnd

Move Boomerangpost
	Requires	COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        WepReady
	Flags		ReqInputs

	Type		male
	  Anim          male\boomerang 55
	TEnd

	Type		huge
	  Anim          huge\boomerang 55
	TEnd

	Type		fem
	  Anim          fem\boomerang 55
	TEnd
MEnd

###################
###### Grenade Lob  ######
###################

Move Grenadelob
	Requires	COMBAT WEAPON GRENADE
	Priority        3
	Interrupts	<AttackIRQ> <Block> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Grenadelobpost
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\grenade_lob 1 40
	TEnd

	Type		huge
	  Anim          huge\grenade_lob 1 40
	TEnd

	Type		fem
	  Anim          fem\grenade_lob 1 40
	TEnd
MEnd

Move Grenadelobpost
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        CombatReady
	Flags		ReqInputs

	Type		male
	  Anim          male\grenade_lob 40
	TEnd

	Type		huge
	  Anim          huge\grenade_lob 40
	TEnd

	Type		fem
	  Anim		fem\grenade_lob 40
	TEnd
MEnd

#############################################################
## QUICK Serum
#############################################################

Move Serum_Quick
	Requires	COMBAT BOOSTUP SERUM QUICK
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Serum_Quick_Post
	Interpolate	15

	Type		male
	  Anim		male\Paramilitary_CatchBoost 10 130
	TEnd
MEnd

Move Serum_Quick_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove	Serum_Quick_PostCycle
	Interpolate	15
	Flags		ReqInputs

	Type		male
	  Anim          male\comb_beatchest 1 91
	  PlayFx        FEET\MALETAUNTA.FX
	TEnd

	Type		huge
	  Anim          huge\comb_beatchest 1 46
	  PlayFx        FEET\HUGETAUNTA.FX
	TEnd

	Type		fem
	  Anim          fem\comb_beatchest 1 46
	  PlayFx        FEET\FEMALETAUNTA.FX
	TEnd
MEnd

Move Serum_Quick_PostCycle
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		Cycle
	NextMove        postReady_back

	Type		male
	  Anim          male\comb_beatchest 95 144
	TEnd

	Type		huge
	  Anim          huge\comb_beatchest 95 144
	TEnd

	Type		fem
	  Anim          fem\comb_beatchest 95 144
	TEnd
MEnd

## POWER ANALYZER ###########################################################

Move PowerAnalyze_Pre
	Requires	WRISTBANDACTIVATE OBSERVE
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Sets            CANTMOVE
	NextMove	PowerAnalyze

	Type		male
	  Anim          male\wristband_activate 1 45
	#  PlayFX	V_COV\Powers\ArachnosSoldier\Training_Surveillance.fx 0 CONSTANT
	TEnd

	Type		V_male
	  Anim          male\wristband_activate 1 45
	#  PlayFX	V_COV\Powers\ArachnosSoldier\V_Training_Surveillance.fx 0 CONSTANT
	TEnd
MEnd

Move PowerAnalyze
	Include sequencers\Members\Readies.inc
	NextMove	WindUp_Wristband_post

	Type		male
	  Anim          male\wristband_activate 45
	#  PlayFX	V_COV\Powers\ArachnosSoldier\Training_Surveillance.fx 0 CONSTANT
	TEnd

	Type		V_male
	  Anim          male\wristband_activate 45
	#  PlayFX	V_COV\Powers\ArachnosSoldier\V_Training_Surveillance.fx 0 CONSTANT
	TEnd
MEnd

## REVEAL MAP ###########################################################

Move RevealMap_Pre
	Requires	WRISTBANDACTIVATE OBSERVE TWO
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Sets            CANTMOVE
	NextMove	RevealMap

	Type		male
	  Anim          male\wristband_activate 1 45
	  PlayFX	POWERS\ItemsAndTempPowers\Reveal\RevealMap.fx 0 CONSTANT
	TEnd

	Type		V_male
	  Anim          male\wristband_activate 1 45
	  PlayFX	POWERS\ItemsAndTempPowers\Reveal\RevealMap_Villain.fx 0 CONSTANT
	TEnd
MEnd

Move RevealMap
	Include sequencers\Members\Readies.inc
	NextMove	WindUp_Wristband_post

	Type		male
	  Anim          male\wristband_activate 45
	  PlayFX	POWERS\ItemsAndTempPowers\Reveal\RevealMap.fx 0 CONSTANT
	TEnd

	Type		V_male
	  Anim          male\wristband_activate 45
	  PlayFX	POWERS\ItemsAndTempPowers\Reveal\RevealMap_Villain.fx 0 CONSTANT
	TEnd
MEnd

## PLANT DEVICE ###########################################################

Move PlantDevice_Pre
	Requires	LOOT QUICK
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ> <MoveIRQ>
	NextMove        PlantDevice_Cycle
	Scale           0.70

	Type		male
	  Anim          male\Loot 3 19
	TEnd
MEnd

Move PlantDevice_Cycle
	Requires	LOOT QUICK
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ>
	Flags		Cycle
	NextMove        PlantDevice_Post
	Scale		0.5

	Type		male
	  Anim          male\examine_hero 1
	TEnd
MEnd

Move PlantDevice_Post
	Include sequencers\Members\Readies.inc
	NextMove        Ready

	Type		male
	  Anim          male\Loot 26
	TEnd

	Type		huge
	  Anim          huge\Loot 26
	TEnd

	Type		fem
	  Anim          fem\Loot 26
	TEnd
MEnd

#################
# Dynamite Thow ##
#################

Move Dynamite_Throw
	Requires	COMBAT DYNAMITE
	Priority        2
	Interrupts	<AttackIRQ> <ReactIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Dynamite_Throw2
	Sets            CANTMOVE

	Type		male
	  Anim          male\draw_axe 1 20
	TEnd
MEnd

Move Dynamite_Throw2
	Requires	COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Dynamite_Throw_Post
	Sets            CANTMOVE

	Type		male
	  Anim          male\knife_throw 1 17
	TEnd
MEnd

Move Dynamite_Throw_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Dynamite_Throw_PostCycle
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\knife_throw 17
	TEnd
MEnd

Move Dynamite_Throw_PostCycle
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        postReady_back
	Flags		Cycle NOSIZESCALE
	Scale		0.5

	Type		male
	  Anim          male\wep_cycle 1 31
	TEnd
MEnd

##########################
###### Small Rock Throw  ######
##########################

Move SmRock
	Requires	COMBAT SHORTTOSS LOW QUICK
	Priority        5
	Interrupts	<AttackIRQ> <ReactIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        SmRock2
	Sets            CANTMOVE

	Type		male
	  Anim          male\rockthrow 1 80
	TEnd
MEnd

Move SmRock2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Menace_A
	Flags		ReqInputs

	Type		male
	  Anim          male\rockthrow 80
	TEnd
MEnd

#######################################################
###### Going Rogue Alignment Powers  ######
#######################################################

###################
###### Alignment Call to Justice  ######
###################

Move AlignmentPower_Justice
	Requires	ALIGNMENT MOMENTGLORY
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        AlignmentPower_Justice_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\glory 1 45
	TEnd

	Type		huge
	  Anim          huge\glory 1 45
	TEnd

	Type		fem
	  Anim          fem\glory 1 45
	TEnd
MEnd

Move AlignmentPower_Justice_Post
	Include sequencers\Members\Combat.inc
	NextMove        Fake_CombatCycle_Half
	Flags		NoSizeScale

	Type		male
	  Anim          male\glory 45
	TEnd

	Type		huge
	  Anim          huge\glory 45
	TEnd

	Type		fem
	  Anim          fem\glory 45
	TEnd
MEnd

##########################
## Alignment Frenzy ##
##########################

Move AlignmentPower_FrenzyNoRoot_pre
	Requires	COMBAT ALIGNMENT TAUNTA NOTROOTED
	Priority        4
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat.inc
	NextMove        AlignmentPower_FrenzyNoRoot_cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\comb_beatchest 1 91
	  PlayFx        FEET\MALETAUNTA.FX
	TEnd

	Type		huge
	  Anim          huge\comb_beatchest 1 91
	  PlayFx        FEET\HUGETAUNTA.FX
	TEnd

	Type		fem
	  Anim          fem\comb_beatchest 1 91
	  PlayFx        FEET\FEMALETAUNTA.FX
	TEnd
MEnd

Move AlignmentPower_FrenzyNoRoot_cycle
	Include sequencers\Members\Combat.inc
	Flags		Cycle
	NextMove        postReady_back

	Type		male
	  Anim          male\comb_beatchest 95 144
	TEnd

	Type		huge
	  Anim          huge\comb_beatchest 95 144
	TEnd

	Type		fem
	  Anim          fem\comb_beatchest 95 144
	TEnd
MEnd

###################
###### Alignment Power Fear  ######
###################

Move AlignmentPower_Fear_pre
	Requires	COMBAT ALIGNMENT MOUTHSFX
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        AlignmentPower_Fear_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\eye 1 40
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\eye 1 40
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		fem
	  Anim          fem\eye 1 40
	  PitchStart	1
	  PitchAngle	0.1
	TEnd
MEnd

Move AlignmentPower_Fear_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatBlend
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\eye 50 60
	TEnd

	Type		huge
	  Anim          huge\eye 50
	TEnd

	Type		fem
	  Anim          fem\eye 50
	TEnd
MEnd

#############################
## Alignment Duplicity - No Root ##
#############################

Move AlignmentPower_Duplicity_Pre_NotRoot
	Requires	COMBAT ALIGNMENT RADIUSATTACK NOTROOTED
	Priority        4
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat.inc
	NextMove        AlignmentPower_Duplicity_Post_NotRoot
	Flags		NoSizeScale

	Type		male
	  Anim          male\radiusattack 1 2
	TEnd

	Type		huge
	  Anim          huge\radiusattack 1 2
	TEnd

	Type		fem
	  Anim          fem\radiusattack 1 2
	TEnd
MEnd

Move AlignmentPower_Duplicity_Post_NotRoot
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\radiusattack 2
	TEnd

	Type		huge
	  Anim          huge\radiusattack 2
	TEnd

	Type		fem
	  Anim          fem\radiusattack 2
	TEnd
MEnd

##  ###########################################################
## Olympian Guard - Issue 19 - End Game
##  ###########################################################

## Beam Sweep ###########################################################

Move Olympian_Beam_Sweep
	Requires	COMBAT HEADCAST ENDGAME
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Olympian_Beam_Sweep_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\olympian_beam_sweep 1 59
	TEnd

MEnd

Move Olympian_Beam_Sweep_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\olympian_beam_sweep 59
	TEnd

MEnd

###################
###### Radius Effect  ######
###################

Move StunningAura_RadiusEffect_Pre
	Requires	COMBAT RADIUSEFFECT STUNNING
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        StunningAura_RadiusEffect
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\radiuseffect 1 60
	TEnd

	Type		huge
	  Anim          huge\radiuseffect 1 60
	TEnd

	Type		fem
	  Anim          fem\radiuseffect 1 60
	TEnd
MEnd

Move StunningAura_RadiusEffect
	Include sequencers\Members\Readies_Fake.inc
	NextMove        Fake_CombatCycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\radiuseffect 60
	TEnd

	Type		huge
	  Anim          huge\radiuseffect 60
	TEnd

	Type		fem
	  Anim          fem\radiuseffect 60
	TEnd
MEnd

###################
###### Med Police Drone (tricorder) ######
###################

Move Police_Drone_Med_Pre
	Requires	MEDOTHER ENDGAME
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Police_Drone_Med_Cycle
	Flags		NoSizeScale
	Sets            CANTMOVE
	Scale		1.5

	Type		male
	  Anim          male\med_other 1 88
	TEnd

	Type		huge
	  Anim          huge\med_other 1 88
	TEnd

	Type		fem
	  Anim          fem\med_other 1 88
	TEnd
MEnd

Move Police_Drone_Med_Cycle
	Include sequencers\Members\Combat.inc
	Flags		NoSizeScale
	NextMove       Police_Drone_Med_post

	Type		male
	  Anim          male\med_other 88 138
	TEnd

	Type		huge
	  Anim          huge\med_other 88 138
	TEnd

	Type		fem
	  Anim          fem\med_other 88 138
	TEnd
MEnd

Move Police_Drone_Med_post
	Include sequencers\Members\Readies.inc
	NextMove        Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\med_other 138
	TEnd

	Type		huge
	  Anim          huge\med_other 138
	TEnd

	Type		fem
	  Anim          fem\med_other 138
	TEnd
MEnd

###########################################################
## IDF Heavy Trooper Attacks
###########################################################

##  missile volley ###########################################################


Move IDF_Missile_Volley_Pre
	Requires	COMBAT MISSLE HIGH ENDGAME
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        IDF_Missile_Volley_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\idf_trooper_missile_volley 1 56
	TEnd

MEnd

Move IDF_Missile_Volley_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\idf_trooper_missile_volley 56
	TEnd

MEnd

##  missile barrage ###########################################################


Move IDF_Missile_Barrage_Pre
	Requires	COMBAT MISSLE LOW ENDGAME
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        IDF_Missile_Barrage_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\idf_trooper_missile_barrage 1 37
	TEnd

MEnd

Move IDF_Missile_Barrage_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\idf_trooper_missile_barrage 38
	TEnd

MEnd

## Nerve Gas ###########################################################

Move IDF_Nerve_Gas_Pre
	Requires	COMBAT MISSLE RIGHT ENDGAME
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        IDF_Nerve_Gas_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\idf_trooper_nerve_gas 1 37
	TEnd

MEnd

Move IDF_Nerve_Gas_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\idf_trooper_nerve_gas 38
	TEnd

MEnd

## EMP Rockets ###########################################################

Move IDF_EMP_Rockets_Pre
	Requires	COMBAT MISSLE LEFT ENDGAME
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        IDF_EMP_Rockets_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\idf_trooper_emp_rockets 1 41
	TEnd

MEnd

Move IDF_EMP_Rockets_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\idf_trooper_emp_rockets 42
	TEnd

MEnd

##  plasma strafe tier02 ###########################################################


Move IDF_Plasma_Strafe_Pre
	Requires	COMBAT FIERCE MISSLE ENDGAME
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        IDF_Plasma_Strafe_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\idf_trooper_plasma_strafe 1 45
	TEnd

MEnd

Move IDF_Plasma_Strafe_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\idf_trooper_plasma_strafe 45
	TEnd

MEnd

##  All Out Assault Tier 03 ###########################################################


Move IDF_Assault_Tier03_Pre
	Requires	COMBAT MISSLE ENDGAME ALTERNATE
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        IDF_Assault_Tier03_Post
	Sets            CANTMOVE
	Flags		NoSizeScale
	SticksonChild	COMBAT MISSLE ONE

	Type		male
	  Anim          male\idf_trooper_missile_barrage 1 37
	TEnd

MEnd

Move IDF_Assault_Tier03_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale
	SticksonChild	COMBAT

	Type		male
	  Anim          male\idf_trooper_missile_barrage 38
	TEnd

MEnd

##  BURST Tier 03 ###########################################################


Move IDF_Burst_Pre
	Requires	COMBAT MISSLE LOW ENDGAME BEAM
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        IDF_Burst_Post
	Sets            CANTMOVE
	Flags		NoSizeScale
	SticksonChild	COMBAT MISSLE TWO

	Type		male
	  Anim          male\idf_trooper_missile_barrage 1 37
	TEnd

MEnd

Move IDF_Burst_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale
	SticksonChild	COMBAT

	Type		male
	  Anim          male\idf_trooper_missile_barrage 38
	TEnd

MEnd

##  Targeted Strike Tier 03 ###########################################################


Move IDF_Targeted_Strike_Pre
	Requires	COMBAT MISSLE ONE ALTERNATE
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        IDF_Targeted_Strike_Post
	Sets            CANTMOVE
	Flags		NoSizeScale
	SticksonChild	COMBAT MISSLE ONE

	Type		male
	  Anim          male\idf_trooper_targeted_strike 1 37
	TEnd

MEnd

Move IDF_Targeted_Strike_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale
	SticksonChild	COMBAT

	Type		male
	  Anim          male\idf_trooper_targeted_strike 38
	TEnd

MEnd

##  Penetrate Tier 03 ###########################################################

Move IDF_Penetrate_Tier03_Pre
	Requires	COMBAT MISSLE HIGH ALTERNATE
	Priority        4
	Interrupts	<AttackIRQ> Pre_PenetrateWindup PenetrateWindup
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        IDF_Penetrate_Tier03_Post
	Sets            CANTMOVE
	Flags		NoSizeScale PitchToTarget
	SticksonChild	COMBAT ENDGAME MISSLE ONE

	Type		male
	  Anim          male\idf_trooper_missile_barrage 14 33
	TEnd

MEnd

Move IDF_Penetrate_Tier03_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale
	SticksonChild	COMBAT

	Type		male
	  Anim          male\idf_trooper_missile_barrage 34
	TEnd

MEnd


###################
###### Penetrate Windup Tier 03  ######
###################

Move Pre_PenetrateWindup
	Requires	TECHWINDUP MISSLE ALTERNATE
	Priority        3
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups.inc
	NextMove        PenetrateWindup
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\idf_trooper_missile_barrage 1 14
	  PitchStart	5
	  PitchAngle	-0.5
	TEnd

MEnd

Move PenetrateWindup
	Requires	TECHWINDUP MISSLE
	Priority        2
	Include sequencers\Members\Windups.inc
	Flags		Cycle PitchToTarget NoSizeScale
	NextMove        PenetrateWindup_post

	Type		male
	  Anim          male\idf_trooper_missile_barrage 14 33
	  PitchAngle	-0.5
	TEnd

MEnd

Move PenetrateWindup_post
	Include sequencers\Members\Readies.inc
	NextMove        Combat_half
	Flags		NoSizeScale

	Type		male
	  Anim          male\idf_trooper_missile_barrage 41
	TEnd


MEnd

## Leaping: Spring Attack ###########################################################

Move SpringAttack_Pre
	Requires	COMBAT BULLRUSH ALTERNATE
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        SpringAttack_Hidden
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\leaping_spring_attack 1 24

	TEnd
MEnd

Move SpringAttack_Hidden
	Requires	COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        SpringAttack
	Flags		NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\leaping_spring_attack 24 28
	 PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	TEnd
MEnd

Move SpringAttack
	Requires	COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        SpringAttack_Post
	Flags		NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\leaping_spring_attack 28 48
	TEnd
MEnd


Move SpringAttack_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\leaping_spring_attack 49 88
	TEnd
MEnd

####Speed: Burnout Buff ########################################

Move Burnout_Boostup_NoRoot
	Requires	COMBAT BOOSTUP ALTERNATE LOW NOTROOTED
	Priority	5
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat.inc
	NextMove        Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\flight_afterburner 1
	TEnd

	Type		huge
	  Anim          male\flight_afterburner 1
	TEnd

	Type		fem
	  Anim          fem\flight_afterburner 1
	TEnd
MEnd

##########################
## Flight - Afterburner ##
##########################

Move Afterburner_pre
	Requires	COMBAT AFTERBURNER ALTERNATE NOTROOTED
	Priority        4
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat.inc
	NextMove        Afterburner_cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\alt_beatchest 1 91
	TEnd

	Type		huge
	  Anim          huge\alt_beatchest 1 91
	TEnd

	Type		fem
	  Anim          fem\alt_beatchest 1 91
	TEnd

MEnd

Move Afterburner_cycle
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	#Flags		Cycle
	NextMove        postReady_back
	Interpolate	8

	Type		male
	  Anim          male\alt_beatchest_post 92 144
	TEnd

	Type		huge
	  Anim          huge\comb_beatchest 95 144
	TEnd

	Type		fem
	  Anim          fem\comb_beatchest 95 144
	TEnd
MEnd

###################
###### Palmcast Time Crawl  ######
###################

Move Time_Crawl_Pre
	Requires	COMBAT CAST MANIPULATION
	Priority        3
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Time_Crawl
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\palmcast_time_crawl 1 20
	  PitchStart	5
	  PitchAngle	-0.5
	TEnd

	Type		huge
	  Anim          huge\palmcast_time_crawl 1 20
	  PitchStart	5
	  PitchAngle	-0.5
	TEnd


MEnd

Move Time_Crawl
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Time_Crawl_Post
	Flags		NoSizeScale PitchToTarget

	Type		male
	  Anim          male\palmcast_time_crawl 21 30
	  PitchStart	5
	  PitchAngle	-0.5
	TEnd

	Type		huge
	  Anim          huge\palmcast_time_crawl 21 30
	  PitchStart	5
	  PitchAngle	-0.5
	TEnd

MEnd


Move Time_Crawl_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatBlend
	Flags		NoSizeScale

	Type		male
	  Anim          male\palmcast_time_crawl 31
	TEnd

	Type		huge
	  Anim          huge\palmcast_time_crawl 31
	TEnd


MEnd

###################
###### Radius Chrono Shift  ######
###################

Move Chrono_Shift_Pre
	Requires	COMBAT RADIUSATTACK MANIPULATION
	Priority        3
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Chrono_Shift
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\radius_chrono_shift 1 49
	TEnd

	Type		huge
	  Anim          huge\radius_chrono_shift 1 49
	TEnd

MEnd

Move Chrono_Shift
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Chrono_Shift_Post
	Flags		NoSizeScale

	Type		male
	  Anim          male\radius_chrono_shift 50 65
	TEnd

	Type		huge
	  Anim          huge\radius_chrono_shift 50 65
	TEnd

MEnd


Move Chrono_Shift_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\radius_chrono_shift 66
	TEnd

	Type		huge
	  Anim          huge\radius_chrono_shift 66
	TEnd

MEnd

################################
######  Villain Groups:The Awakened Powers
#################################

#################################
###### BURNOUT SUBDUE  ######
#################################

Move Subdue
	Requires	COMBAT PSIONIC HEADCAST
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Subdue_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\eye 1 50
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\eye 1 50
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		fem
	  Anim          fem\eye 1 50
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

MEnd

#############################################################

Move Subdue_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatBlend
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\eye 50 60
	TEnd

	Type		huge
	  Anim          huge\eye 50
	TEnd

	Type		fem
	  Anim          fem\eye 50
	TEnd


MEnd

#################################
###### BURNOUT MENTAL BLAST  ######
#################################

Move Mentalblast_pre
	Requires	COMBAT PSIONIC PSIONICBLAST HIGH
	Priority        4
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Mentalblast
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale
	Scale		2.0

	Type		male
	  Anim          male\psionic 2 51
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\psionic 2 51
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

	Type		fem
	  Anim          fem\psionic 2 51
	  PitchStart	13
	  PitchAngle	0.2
	TEnd
MEnd

Move Mentalblast
	Requires	COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Mentalblast_Post
	Sets            CANTMOVE
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		male
	  Anim          male\psionic 96 121
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\psionic 96 121
	  PitchAngle	0.2
	TEnd

	Type		fem
	  Anim          fem\psionic 96 121
	  PitchAngle	0.2
	TEnd
MEnd

Move Mentalblast_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		male
	  Anim          male\psionic 121
	TEnd

	Type		huge
	  Anim          huge\psionic 121
	TEnd

	Type		fem
	  Anim          fem\psionic 121
	TEnd

MEnd

#################################
###### BURNOUT OVERLOAD  ######
#################################

Move Awakened_Burnout_Overload
	Requires	OVERLOAD
	Priority        1
	Interrupts	<DeathIRQ> <ReactIRQ> <HitIRQ> <Combat> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	Member		<ExplodeIRQ>
	NextMove        Awakened_Burnout_Overload_Explode
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\v_awakened_rapidboil 1 70
	TEnd

	Type		fem
	  Anim          male\v_awakened_rapidboil 1 70
	TEnd

MEnd

Move Awakened_Burnout_Overload_Explode
	Interpolate     0
	Flags		CANTMOVE NoSizeScale NotSelectable
	Scale           0.50
	Sets            NOCOLLISION
	NextMove        Awakened_Burnout_Overload_Post

	Type		male
	  Anim          male\v_awakened_rapidboil 71 80
	TEnd

	Type		fem
	  Anim          male\v_awakened_rapidboil 71 80
	TEnd


MEnd

Move Awakened_Burnout_Overload_Post
	Interpolate     0
	Flags		cycle CANTMOVE NoSizeScale NotSelectable
	Scale           0.50
	Sets            NOCOLLISION DIENOW

	Type		male
	  Anim          male\v_awakened_rapidboil 80 80
	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	TEnd

	Type		fem
	  Anim          male\v_awakened_rapidboil 80 80
	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	TEnd


MEnd


#################################
## CONDUIT PSYCHIC SCREAM ###########################################################
#################################

Move PsychicScream
	Requires	COMBAT PSIONIC SCREAM
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        PsychicScream_post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\eye 1 50
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\eye 1 50
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		fem
	  Anim          fem\eye 1 50
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

MEnd

Move PsychicScream_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Priority        1

	Type		male
	  Anim          male\eye 50 60
	TEnd

	Type		huge
	  Anim          huge\eye 50
	TEnd

	Type		fem
	  Anim          fem\eye 50
	TEnd

MEnd

###################
###### Heal Allies  ######
###################

Move ConduitHeal
	Requires	COMBAT PSIONIC HEAL
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        ConduitHeal_post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\heal 1 35
	  PitchStart	8
	  PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\heal 1 35
	  PitchStart	8
	  PitchAngle	0.1
	TEnd

	Type		fem
	  Anim          fem\heal 1 35
	  PitchStart	8
	  PitchAngle	0.1
	TEnd

MEnd

Move ConduitHeal_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\heal 35
	  PitchEnd	63
	  PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\heal 35
	  PitchEnd	63
	  PitchAngle	0.1
	TEnd

	Type		fem
	  Anim          fem\heal 35
	  PitchEnd	63
	  PitchAngle	0.1
	TEnd

MEnd

###################
###### OPPRESSOR MIND SIPHON ######
###################

Move Oppressor_MindSiphon
	Requires	COMBAT PSIONIC ENERGYTORRENT SPEECH
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        E_Torrent_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\energy_torrent 1 64
	TEnd

	Type		huge
	  Anim          huge\energy_torrent 1 64
	TEnd

	Type		fem
	  Anim          fem\energy_torrent 1 64
	TEnd

MEnd

###################
###### OPPRESSOR PSIONIC BLAST ######
###################

Move Oppressor_PsionicBlast
	Requires	COMBAT PSIONIC BLOW SPEECH
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Blowpost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\blow_cast 1 79
	  PitchStart	12
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\blow_cast 1 79
	  PitchStart	12
	  PitchAngle	0.2
	TEnd

	Type		fem
	  Anim          fem\blow_cast 1 79
	  PitchStart	12
	  PitchAngle	0.2
	TEnd

MEnd

#################################
###### OPPRESSOR PSIONIC SHOCKWAVE  ######
#################################

Move Oppressor_PsionicShockwave
	Requires	COMBAT PSIONIC BLOW OVERLOAD
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Oppressor_PsionicShockwave_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\v_awakened_rapidboil 1 70
	TEnd

	Type		fem
	  Anim          fem\v_awakened_rapidboil 1 70
	TEnd

MEnd

#############################################################

Move Oppressor_PsionicShockwave_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\v_awakened_rapidboil 71
	TEnd

	Type		fem
	  Anim          fem\v_awakened_rapidboil 71
	TEnd

MEnd

################################
###### MINDSTORM PSIONIC TORNADO ######
################################

Move PsionicTornado
	Requires	COMBAT PSIONIC SKYSUMMON
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        S_Summonpost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\sky_summon 1 60
	TEnd

	Type		huge
	  Anim          huge\sky_summon 1 60
	TEnd

	Type		fem
	  Anim          fem\sky_summon 1 60
	TEnd

MEnd

###################
###### MINDSTORM PSIONIC LIGHTNING ######
###################

Move PsionicLightning
	Requires	COMBAT PSIONIC STOPCAST
	Priority        3
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Stop_cycle
	Flags		PitchToTarget NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\stop 1 31
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\stop 1 31
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\stop 1 31
	  PitchAngle	0.5
	TEnd

MEnd

################################
###### MINDSTORM MASS LEVITATE ######
################################

Move Mindstorm_MassLevitate
	Requires	COMBAT PSIONIC WINDBLAST
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Windblastpost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\windblast 1 70
	TEnd

	Type		huge
	  Anim          huge\windblast 1 70
	TEnd

	Type		fem
	  Anim          fem\windblast 1 70
	TEnd

MEnd

###################
###### MINDSTORM FRAZZLE  ######
###################

Move Mindstorm_Frazzle
	Requires	COMBAT PSIONIC MAGIC CAST
	Priority        3
	Interrupts	<AttackIRQ> <Windups> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Magic_Lpost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\magic_L 1 30
	  PitchStart	10
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\magic_L 1 30
	  PitchStart	10
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\magic_L 1 30
	  PitchStart	10
	  PitchAngle	0.5
	TEnd

MEnd

#######################################################################
####### Darkness Control (Haunt)   ###
#######################################################################

Move Haunt_Pre
	Requires	COMBAT LIGHTERBLOW ALTERNATE
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Haunt_Attack
	Sets            CANTMOVE
	Flags		NoSizeScale PitchToTarget

	Type		male
	  Anim          male\onehand_palmcast 1 36
	  PitchStart	5
	  PitchAngle	-0.5
	TEnd
MEnd

Move Haunt_Attack
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Haunt_Post
	Sets            CANTMOVE
	Flags		NoSizeScale PitchToTarget

	Type		male
	  Anim          male\onehand_palmcast 36 43
	  PitchStart	5
	  PitchAngle	-0.5
	TEnd
MEnd

Move Haunt_Post
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\onehand_palmcast 44
	TEnd
MEnd

##################################
###### Boost_up (Just for buffs) ######
##################################

Move Boostup_Apparitions
	Requires	BOOSTUP ENDGAME
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ> <BlockIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove        Boostuppost_Apparitions
	Flags		NoSizeScale

	Type		P_Apparitions_Minion
	  Anim          fem\boostup 1 10
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim          fem\boostup 1 10
	TEnd

	Type		P_Apparitions_Boss
	  Anim          fem\boostup 1 10
	TEnd

MEnd

Move Boostuppost_Apparitions
	Member		<DeathIRQ> <AttackIRQ> <BlockIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove        BoostupReady_Apparitions
	Flags		NoSizeScale

	Type		P_Apparitions_Minion
	  Anim          fem\boostup 11
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim          fem\boostup 11
	TEnd

	Type		P_Apparitions_Boss
	  Anim          fem\boostup 1 10
	TEnd

MEnd

Move BoostupReady_Apparitions
	Include sequencers\Members\Readies.inc
	NextMove        R_Vanish_postdeath_Apparitions
	Sets            NOCOLLISION
	Flags		NotSelectable

	Type		P_Apparitions_Minion
	  Anim          male\death_front 47 129
	  PlayFx        GENERIC\ENTITYVANISH.FX  0 CONSTANT
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim          male\death_front 47 129
	  PlayFx        GENERIC\ENTITYVANISH.FX  0 CONSTANT
	TEnd

	Type		P_Apparitions_Boss
	  Anim          male\death_front 47 129
	  PlayFx        GENERIC\ENTITYVANISH.FX  0 CONSTANT
	TEnd


MEnd

Move R_Vanish_postdeath_Apparitions
	Include sequencers\Members\Readies.inc
	Flags		Cycle NotSelectable
	Sets            NOCOLLISION

	Type		male
	  Anim          male\death_front 129 130
	  PlayFx        GENERIC\ENTITYVANISH.FX  0 CONSTANT
	TEnd

	Type		P_Apparitions_Minion
	  Anim          male\death_front 129 130
	  PlayFx        GENERIC\ENTITYVANISH.FX  0 CONSTANT
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim          male\death_front 129 130
	  PlayFx        GENERIC\ENTITYVANISH.FX  0 CONSTANT
	TEnd

	Type		P_Apparitions_Boss
	  Anim          male\death_front 129 130
	  PlayFx        GENERIC\ENTITYVANISH.FX  0 CONSTANT
	TEnd

MEnd

###################
###### Abomination - Fist Ground Pound ######
###################

Move Abomination_Fist_Ground_Pound_Lock
	Requires	COMBAT ABOMINATION MEGABLAST LOW
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Abomination_Fist_Ground_Pound_Tell
	#Sets            CANTMOVE
	Flags		NoSizeScale

	Type		Abomination
	  Anim          huge\abomination_ground_pound 1 80
	TEnd

MEnd

Move Abomination_Fist_Ground_Pound_Tell
	Requires	COMBAT ABOMINATION MEGABLAST HIGH
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Abomination_Fist_Ground_Pound_Attack
	Flags		NoSizeScale
	#Sets            CANTMOVE

	Type		Abomination
	  Anim          huge\abomination_ground_pound 80 120
	TEnd

MEnd


Move Abomination_Fist_Ground_Pound_Attack
	Requires	COMBAT ABOMINATION MEGABLAST
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Abomination_Fist_Ground_Pound_Post
	Flags		NoSizeScale
	#Sets            CANTMOVE

	Type		Abomination
	  Anim          huge\abomination_ground_pound 120 217
	TEnd

MEnd

Move Abomination_Fist_Ground_Pound_Post
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		Abomination
	  Anim          huge\abomination_ground_pound 218
	TEnd

MEnd

###################
###### Abomination - Leg Stomp ######
###################

Move Abomination_Leg_Stomp_Tell
	Requires	COMBAT ABOMINATION STOMP LOW
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Abomination_Leg_Stomp_Lock
	#Sets            CANTMOVE
	Flags		NoSizeScale

	Type		Abomination
	  Anim          huge\abomination_leg_stomp 1 80
	TEnd

MEnd

Move Abomination_Leg_Stomp_Lock
	Requires	COMBAT ABOMINATION STOMP HIGH
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Abomination_Leg_Stomp_Attack
	Flags		NoSizeScale
	#Sets            CANTMOVE

	Type		Abomination
	  Anim          huge\abomination_leg_stomp 80 103
	TEnd

MEnd



Move Abomination_Leg_Stomp_Attack
	Requires	COMBAT ABOMINATION STOMP
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Abomination_Leg_Stomp_Post
	Flags		NoSizeScale
	#Sets            CANTMOVE

	Type		Abomination
	  Anim          huge\abomination_leg_stomp 103 190
	TEnd

MEnd

Move Abomination_Leg_Stomp_Post
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		Abomination
	  Anim          huge\abomination_leg_stomp 190
	TEnd

MEnd

###################
###### Abomination -  Electrical Discharge ######
###################

Move Abomination_Electrical_Discharge
	Requires	BOOSTAURA ABOMINATION QUICK
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        CombatReady
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		Abomination
	  Anim          huge\abomination_electrical_discharge 1
	TEnd
MEnd

###################

## Destiny BULLRUSH ###########################################################

Move Destiny_Bullrush_Pre
	Requires	COMBAT ENDGAME BULLRUSH
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Destiny_Bullrush
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\incarnate_destiny_attack 1 35
	  PlayFx        GENERIC\ENTITYVANISH.FX 15 CONSTANT
	TEnd
MEnd

Move Destiny_Bullrush
	Requires	COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Destiny_Bullrush_Post
	Flags		NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\incarnate_destiny_attack 32 45
	TEnd
MEnd

Move Destiny_Bullrush_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\incarnate_destiny_attack 46
	TEnd
MEnd

###########################
###### Incarnate Vorpal Judgement ######
##########################

Move Vorpal_Judgement
	Requires	COMBAT KUNGFU KICK STRONG SWIPE
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove       Vorpal_Judgement_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\storm_kick 1 25
	TEnd
MEnd

Move Vorpal_Judgement_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\storm_kick 25
	TEnd
MEnd

##################################
###### Pet Summoning Toggle ######
##################################

Move Pet_Summon_Cast
	Requires	COMBAT SUMMON CAST STOP
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ> <BlockIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove        Pet_Summon_Cast_post
	Flags		NoSizeScale

	Type		male
	  Anim          male\onehand_palmcast 1 36
	TEnd

#	Type		huge
#	  Anim          huge\boostup 1 10
#	TEnd
#
#	Type		fem
#	  Anim          fem\boostup 1 10
#	TEnd
#
#	Type		Wisp
#	  Anim          Wisp\headcast_AOE 1 104
#	TEnd
MEnd

Move Pet_Summon_Cast_post
	Member		<DeathIRQ> <AttackIRQ> <BlockIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove        Pet_Summon_Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\onehand_palmcast 36
	TEnd

#	Type		huge
#	  Anim          huge\boostup 11
#	TEnd
#
#	Type		fem
#	  Anim          fem\boostup 11
#	TEnd
#
#	Type		Wisp
#	  Anim          Wisp\headcast_AOE 105
#	TEnd
MEnd

Move Pet_Summon_Ready
	Include sequencers\Members\Readies.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\combat_cycle 1 31
	TEnd

#	Type		huge
#	  Anim          huge\mon_ready 1 60
#	TEnd
#
#	Type		fem
#	  Anim          fem\Skel_ready2 1 60
#	TEnd
#
#	Type		Wisp
#	  Anim          Wisp\float 1
#	TEnd
MEnd

###################
###### Pet Summoning Toggle Levitate  ######
###################

Move Pet_Summon_Levitate
	Requires	COMBAT SUMMON LEVITATE STOP
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ> <BlockIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove        Pet_Summon_Levitate_post
	Flags		NoSizeScale

	Type		male
	  Anim          male\levitate 1 50
	TEnd

	Type		huge
	  Anim          huge\levitate 1 50
	TEnd

	Type		fem
	  Anim          fem\levitate 1 50
	TEnd
MEnd

Move Pet_Summon_Levitate_post
	Member		<DeathIRQ> <AttackIRQ> <BlockIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove        Pet_Summon_Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\levitate 50
	TEnd

	Type		huge
	  Anim          huge\levitate 50
	TEnd

	Type		fem
	  Anim          fem\levitate 50
	TEnd
MEnd

###################
###### Pet Summoning Toggle Wristband Activate  ######
###################

Move Pet_Summon_wristbandactivate
	Requires	COMBAT SUMMON WRISTBANDACTIVATE STOP
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ> <BlockIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove        Pet_Summon_wristbandactivate_post
	Flags		NoSizeScale

	Type		male
	  Anim          male\wristband_activate 1 45
	TEnd

MEnd

Move Pet_Summon_wristbandactivate_post
	Member		<DeathIRQ> <AttackIRQ> <BlockIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove        Pet_Summon_Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\wristband_activate 45
	TEnd

MEnd

###########################
###### BEAST MASTERY ######
###########################

###################
###### Summon wolves ######
###################

Move Summon_wolves
	Requires	COMBAT SCREAM ANIMAL
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Summon_wolves_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\scream 10 58
#	    PlayFX	Emotes\PlayerHowl.fx 5
	TEnd

	Type		huge
	  Anim          huge\scream 10 58
#	  PlayFX	Emotes\PlayerHowl.fx 5
	TEnd

	Type		fem
	  Anim          fem\scream 10 58
#	  PlayFX	Emotes\PlayerHowl.fx 5
	TEnd

MEnd

Move Summon_wolves_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Scream_cycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\scream 58
	TEnd

	Type		huge
	  Anim          huge\scream 58
	TEnd

	Type		fem
	  Anim          fem\scream 58
	TEnd

MEnd


##################
###### Call Hawk ######
###################

Move Call_Hawk_Pre
	Requires	COMBAT THROWKNIFE ANIMAL
	Priority        3
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Call_Hawk
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\call_hawk 1 16
	TEnd
MEnd

Move Call_Hawk
	Requires	COMBAT
	Member		<DeathIRQ> <AttackIRQ> <MoveIRQ>
	NextMove        Call_Hawk_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\call_hawk 17 50
	TEnd
MEnd

Move Call_Hawk_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Scale           0.5
	Flags		ReqInputs

	Type		male
	  Anim          male\call_hawk 51
	TEnd
MEnd


###################
###### Train Beast  ######
###################

Move Train_Beast
	Requires	COMBAT WALL GROWL
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Train_Beast_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\train_beast 1 60
	TEnd

MEnd

Move Train_Beast_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\train_beast 61
	TEnd

MEnd

###################
###### Summon Lion  ######
###################

Move Summon_Lion
	Requires	COMBAT SUMMON ANIMAL
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Summon_Lion_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\summon_lion 1 60
	TEnd

MEnd

Move Summon_Lion_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\summon_lion 61
	TEnd

MEnd

###################
###### Summon Dire Wolf  ######
###################

Move Summon_Dire_Wolf
	Requires	COMBAT SUMMON GROWL GIANT
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Summon_Dire_Wolf_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\summon_dire_wolf 1 60
	TEnd

MEnd

Move Summon_Dire_Wolf_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\summon_dire_wolf 61
	TEnd

MEnd

###################
###### Tame Beast  ######
###################

Move Tame_Beast
	Requires	COMBAT BOOST ANIMAL
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Tame_Beast_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\Tame_Beast 1 60
	TEnd

MEnd

Move Tame_Beast_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\Tame_Beast 61
	TEnd

MEnd

#################################
###### Origin Pool Sorcery Spirit Ward  ######
#################################

Move Spirit_Ward
	Requires	COMBAT ORIGINPOOL CAST QUICK
	Priority        4
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Spirit_Ward_post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\spirit_ward 1 60
	TEnd
MEnd

Move Spirit_Ward_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\spirit_ward 61
	TEnd
MEnd

#################################
###### Origin Pool Sorcery Arcane Bolt  ######
#################################

Move Arcane_Bolt
	Requires	COMBAT ORIGINPOOL CAST
	Priority        3
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Arcane_Bolt_post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\Arcane_Bolt 1 60
	TEnd
MEnd

Move Arcane_Bolt_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\Arcane_Bolt 61
	TEnd
MEnd

#################################
###### Origin Pool Sorcery Enflame  ######
#################################

Move Enflame
	Requires	COMBAT ORIGINPOOL DISRUPT
	Priority        3
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Enflame_post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\Enflame 1 62
	TEnd
MEnd

Move Enflame_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\Enflame 63
	TEnd
MEnd

##############################
###### Origin Pool Sorcery Translocation  ######
##############################

Move Translocation
	Requires	ORIGINPOOL TELEPORT
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Translocation_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\translocation 1 18
	TEnd


MEnd

Move Translocation_post
	Include sequencers\Members\Combat.inc
	NextMove        Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\translocation 19
	TEnd

MEnd

##############################
###### Origin Pool Force Of Will Weaken Resolve  ######
##############################

Move Weaken_Resolve
	Requires	COMBAT ORIGINPOOL PSIONIC
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Weaken_Resolve_post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale
	Scale		0.9

	Type		male
	  Anim          male\weaken_resolve 1 60
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

MEnd

Move Weaken_Resolve_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\weaken_resolve 61
	TEnd

MEnd

##############################
###### Origin Pool Force Of Will Project Will  ######
##############################

Move Project_Will
	Requires	COMBAT ORIGINPOOL BLAST
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Project_Will_post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale
	Scale		0.9

	Type		male
	  Anim          male\project_will 1 62
	  PitchStart	50
	  PitchAngle	0.2
	TEnd

MEnd

Move Project_Will_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\project_will 63
	TEnd

MEnd

##############################
###### Origin Pool Force Of Will Wall Of Force  ######
##############################

Move Wall_Of_Force
	Requires	COMBAT ORIGINPOOL SONICBLAST
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Wall_Of_Force_post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale
	Scale		0.9

	Type		male
	  Anim          male\wall_of_force 1 31
	  PitchStart	29
	  PitchAngle	0.2
	TEnd

MEnd

Move Wall_Of_Force_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\wall_of_force 32
	TEnd

MEnd

##############################
###### Origin Pool Experiment Jaunt  ######
##############################

Move Jaunt
	Requires	ORIGINPOOL SUPERTELEPORT
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Jaunt_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\jaunt 1 30
	TEnd


MEnd

Move Jaunt_post
	Include sequencers\Members\Combat.inc
	NextMove        Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\jaunt 31
	TEnd

MEnd

##########################
## SymbioticArmor Defensive Adaptation ##
##########################

Move SymbioticArmor_Defensive_Adaptation
	Requires	COMBAT BEATCHESTSHORT NOTROOTED
	Priority        3
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat.inc
	NextMove        AlignmentPower_FrenzyNoRoot_cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\comb_beatchest_short 1 30
#	  PlayFx        FEET\MALETAUNTA.FX
	TEnd

MEnd

Move SymbioticArmor_Defensive_Adaptation_post
	Include sequencers\Members\Combat.inc
	#Flags		Cycle
	NextMove        postReady_back

	Type		male
	  Anim          male\comb_beatchest_short 31
	TEnd

MEnd

###################
###### Martial Assault Trick Shot  ######
###################

Move Trick_Shot
	Requires	COMBAT MARTIALASSAULT MAGICTHROW
	Priority        3
	NextMove        Trick_Shot_post
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\trick_shot 1 31
	TEnd

MEnd

Move Trick_Shot_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\trick_shot 32
	TEnd

MEnd


## Martial Assualt Masterful Throw #############################################

Move Masterful_Throw_Windup
	Requires	COMBAT MARTIALASSAULT WINDUP
	Priority	3
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups.inc
	Flags		NoSizeScale PitchToTarget
	NextMove	Masterful_Throw_Windup_Cycle

	Type		male
	  Anim		male\masterful_throw 1 10
	TEnd
MEnd

Move Masterful_Throw_Windup_Cycle
	Requires	COMBAT MARTIALASSAULT WINDUP
	Include sequencers\Members\Windups.inc
	Flags		Cycle NoSizeScale PitchToTarget
	NextMove	CombatReady

	Type		male
	  Anim		male\masterful_throw 11 26
	TEnd
MEnd

#############################################################

Move Masterful_Throw
	Requires	COMBAT MARTIALASSAULT STAB
	Priority	3
	Interrupts	<AttackIRQ> <Windups>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Masterful_Throw_Post
	Sets		CANTMOVE
	Flags		NoSizeScale PitchToTarget

	Type		male
	  Anim		male\masterful_throw 27 62
	TEnd
MEnd

Move Masterful_Throw_Post
	Include sequencers\Members\Combat.inc
	NextMove	CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim		male\masterful_throw 63
	TEnd
MEnd


###### Martial Assault Explosive Shuriken  ######

Move Explosive_Shuriken
	Requires	COMBAT MARTIALASSAULT MAGICTHROW EXPLODE
	Priority        4
	NextMove        Explosive_Shuriken_post
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\explosive_shuriken 1 50
	  PitchAngle	0.3
	  PitchStart	28
	  PitchEnd	48
	TEnd

MEnd

Move Explosive_Shuriken_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\explosive_shuriken 51
	TEnd

MEnd

###### Martial Assault Spinning Kick  ######

Move MartialAssualt_SpinningKick
	Requires	COMBAT MARTIALASSAULT KICK
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        MartialAssualt_SpinningKick_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\spinning_kick 1 32
	TEnd
MEnd

Move MartialAssualt_SpinningKick_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\spinning_kick 33
	TEnd
MEnd

###### Martial Manipulation Inner Will ######

Move MartialManipulation_InnerWill
	Requires	MARTIALASSAULT HAND
	Priority        2
	NextMove        MartialManipulation_InnerWill_post
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\inner_will 1 40

	TEnd

MEnd

Move MartialManipulation_InnerWill_post
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\inner_will 41
	TEnd

MEnd

############################
###### Burst Of Speed  ######
###########################

Move Burst_Of_Speed
	Requires	COMBAT MARTIALASSAULT TELEPORT
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Burst_Of_Speed_2
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\burst_of_speed 1 10
	TEnd
MEnd

Move Burst_Of_Speed_2
	Requires	COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Burst_Of_Speed_post
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\burst_of_speed 11 40
	TEnd
MEnd

Move Burst_Of_Speed_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\burst_of_speed 41
	TEnd
MEnd

##############################
###### Origin Pool Utility Belt Bolas  ######
##############################

#Move Bolas
#	Requires	COMBAT ORIGINPOOL THROWBRAWL
#	Priority        3
#	Interrupts	<AttackIRQ>
#	Member		<DeathIRQ> <AttackIRQ>
#	NextMove        Bolas_post
#	Sets            CANTMOVE
#	Flags		PitchToTarget NoSizeScale
#	Scale		0.9
#
#	Type		male
#	  Anim          male\bolas 1 55
#	  PitchStart	50
#	  PitchAngle	0.2
#	TEnd
#
#MEnd

#Move Bolas_post
#	Requires	COMBAT
#	Include sequencers\Members\Combat.inc
#	NextMove        CombatReady
#	Flags		NoSizeScale ReqInputs
#
#	Type		male
#	  Anim          male\bolas 56
#	TEnd
#
#MEnd

##############################
###### Origin Pool Utility Belt Life Support System  ######
##############################

#Move Life_Support_System
#	Requires	COMBAT ORIGINPOOL WRISTBANDACTIVATE
#	Priority        3
#	Interrupts	<AttackIRQ>
#	Member		<DeathIRQ> <AttackIRQ>
#	NextMove        Life_Support_System_post
#	Sets            CANTMOVE
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim          male\wrist_activation_right 1 38
#	TEnd
#
#MEnd

#Move Life_Support_System_post
#	Requires	COMBAT
#	Include sequencers\Members\Combat.inc
#	NextMove        CombatReady
#	Flags		NoSizeScale ReqInputs
#
#	Type		male
#	  Anim          male\wrist_activation_right 39
#	TEnd
#
#MEnd

##############################
###### Origin Pool Gadgetry Nano Net  ######
##############################

#Move Nano_Net
#	Requires	COMBAT ORIGINPOOL CAGE
#	Priority        3
#	Interrupts	<AttackIRQ>
#	Member		<DeathIRQ> <AttackIRQ>
#	NextMove        Nano_Net_post
#	Sets            CANTMOVE
#	Flags		PitchToTarget NoSizeScale
#	Scale		0.9
#
#	Type		male
#	  Anim          male\nano_net 1 44
#	  PitchStart	50
#	  PitchAngle	0.2
#	TEnd
#
#MEnd

#Move Nano_Net_post
#	Requires	COMBAT
#	Include sequencers\Members\Combat.inc
#	NextMove        CombatReady
#	Flags		NoSizeScale ReqInputs
#
#	Type		male
#	  Anim          male\nano_net 45
#	TEnd
#
#MEnd

##############################
###### Origin Pool Gadgetry Energy Drone  ######
##############################

#Move Energy_Drone
#	Requires	COMBAT ORIGINPOOL DRONE TWO
#	Priority        4
#	Interrupts	<AttackIRQ>
#	Member		<DeathIRQ> <AttackIRQ>
#	NextMove        Energy_Drone_post
#	Sets            CANTMOVE
#	Flags		PitchToTarget NoSizeScale
#	Scale		0.9
#
#	Type		male
#	  Anim          male\energy_drone 1 86
#	  PitchStart	50
#	  PitchAngle	0.2
#	TEnd
#
#MEnd

#Move Energy_Drone_post
#	Requires	COMBAT
#	Include sequencers\Members\Combat.inc
#	NextMove        CombatReady
#	Flags		NoSizeScale ReqInputs
#
#	Type		male
#	  Anim          male\energy_drone 87
#	TEnd
#
#MEnd

##############################
###### Origin Pool Gadgetry Blaster Barrage  ######
##############################

#Move Blaster_Barrage
#	Requires	COMBAT ORIGINPOOL DRONE SIX
#	Priority        4
#	Interrupts	<AttackIRQ>
#	Member		<DeathIRQ> <AttackIRQ>
#	NextMove        Blaster_Barrage_post
#	Sets            CANTMOVE
#	Flags		PitchToTarget NoSizeScale
#	Scale		0.9
#
#	Type		male
#	  Anim          male\Blaster_Barrage 21 90
#	  PitchStart	50
#	  PitchAngle	0.2
#	TEnd
#
#MEnd

#Move Blaster_Barrage_post
#	Requires	COMBAT
#	Include sequencers\Members\Combat.inc
#	NextMove        CombatReady
#	Flags		NoSizeScale ReqInputs
#
#	Type		male
#	  Anim          male\Blaster_Barrage 91
#	TEnd
#
#MEnd