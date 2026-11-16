####################################################################
## POWERS_AIR.INC - Sequencers related to power animations in flight
####################################################################
## Priority:
##  Fly combat moves get +10 priority to resolve conficts with hover and combat hover modes

########################
## Air Toss duffelbag ##
########################

Move A_Duffelbag_Toss
	Requires	FLY COMBAT DUFFELBAGTOSS
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Duffelbag_Toss2
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\air_duffelbag_toss 1 48
	TEnd
MEnd

Move A_Duffelbag_Toss2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	Lazy_C_flycycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_duffelbag_toss 48
	TEnd
MEnd

#######################################################################
####### Air Lighter blow (torch blow w\ lighter - goes back to combat)   ###
#######################################################################

Move A_Lighter_Blow
	Requires	FLY COMBAT LIGHTERBLOW
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Lighter_Blow_2
	Flags		CANTMOVE PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_lighter_blow 1 85
	TEnd
MEnd

Move A_Lighter_Blow_2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	Lazy_C_flycycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_lighter_blow 85
	TEnd
MEnd

######################
### Air WarWolf Whistle ########
######################

Move A_WarWolf
	Requires	FLY HIGH EXCLAMATION SPAWN
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_WarWolf_post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_whistle 1 40
	  PlayFx        FEET\WHISTLE2.FX 19
	TEnd

	Type		huge
	  Anim          huge\air_whistle 1 40
	  PlayFx        FEET\WHISTLE2.FX 19
	TEnd

	Type		fem
	  Anim          fem\air_whistle 1 40
	  PlayFx        FEET\WHISTLE2.FX 19
	TEnd
MEnd

Move A_WarWolf_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_WarWolf_cycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_whistle 40
	TEnd

	Type		huge
	  Anim          huge\air_whistle 40
	TEnd

	Type		fem
	  Anim          fem\air_whistle 40
	TEnd
MEnd

Move A_WarWolf_cycle
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\float2 1
	TEnd

	Type		huge
	  Anim          huge\float 1
	TEnd

	Type		fem
	  Anim          fem\fly_ready 1
	TEnd
MEnd


############################
###### Air Lightning Teleport  ######
###########################

Move A_Lightning_Teleport
	Requires	FLY COMBAT LIGHTNINGTELEPORT
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Lightning_Teleport_2
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\air_glory 1 30
	TEnd
MEnd

Move A_Lightning_Teleport_2
	Requires	FLY COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Lightning_Teleport_post
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\air_pre_poison 30 58
	TEnd
MEnd

Move A_Lightning_Teleport_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	Lazy_C_flycycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_pre_poison 58
	TEnd
MEnd


#########################
####### air Jacobs Ladder   ###
#########################

Move A_JacobsLadder
	Requires	FLY COMBAT JACOBSLADDER
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_JacobsLadder_2
	Flags		CANTMOVE PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_jacobs_ladder 1 48
	  PitchEnd	41
	TEnd
MEnd


Move A_JacobsLadder_2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	Lazy_C_flycycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_jacobs_ladder 48
	TEnd
MEnd


###################
###### Stomp  ######
###################

Move A_Stomp
	Requires	FLY COMBAT STOMP
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Stomp2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_stomp 1 62
	TEnd
MEnd

Move A_Stomp2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	Lazy_C_flycycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_stomp 62
	TEnd
MEnd


########################
####### Air Chi Force   ###
########################

Move A_Chi_Force
	Requires	FLY COMBAT CHIFORCE
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Chi_Force_post
	Flags		CANTMOVE PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_chi_attack_dual 1 68
	TEnd
MEnd

Move A_Chi_Force_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	Lazy_C_flycycle
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		male
	  Anim          male\air_chi_attack_dual 68
	  PitchEnd	70
	TEnd
MEnd


#############################################################

Move A_2HPowerThrust_Pre
	Requires	FLY COMBAT 2HPOWERTHRUST
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_2HPowerThrust
	Sets            CANTMOVE
	Flags		NoSizeScale PitchToTarget

	Type		male
	  Anim          male\air_2HPowerThrust 1 57
	  PitchStart	48
	TEnd
MEnd

Move A_2HPowerThrust
	Requires	FLY COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_2HPowerThrust_Post
	Interpolate	8
	Flags		CANTMOVE PitchToTarget NoSizeScale
	Scale		0.9

	Type		male
	  Anim          male\air_mart_backstab 125 145
	TEnd
MEnd

Move A_2HPowerThrust_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	Lazy_C_flycycle
	Flags		NoSizeScale ReqInputs PitchToTarget

	Type		male
	  Anim          male\air_mart_backstab 145
	  PitchEnd	60
	TEnd
MEnd


##########################
####### Air Zeus's Lightning    ###
##########################

Move A_Zeus_Lightning
	Requires	FLY COMBAT ZEUSATTACK
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Zeus_Lightning_2
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\air_states_zeus 1 99
	TEnd
MEnd

Move A_Zeus_Lightning_2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	Lazy_C_flycycle_Blend
	Interpolate	15
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_states_zeus 100
	TEnd
MEnd


##########################
####### Air 1 Hand Slam    ###
##########################

Move A_1Hslam
	Requires	FLY COMBAT 1HSLAM
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_1Hslam_2
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\air_1hslam 1 75
	TEnd
MEnd

Move A_1Hslam_2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	Lazy_C_flycycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_1hslam 75
	TEnd
MEnd

##################
####### Air Show ######
##################

Move Air_Show
	Requires	FLY SHOW
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Air_Show2
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\air_show 1 85
	TEnd
MEnd

Move Air_Show2
	Requires	FLY
	Include sequencers\Members\Readies_Air.inc
	NextMove	flyready
	Scale		0.8
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_show 85
	TEnd
MEnd


#################################
####### Air Detonate ######
##################################

Move A_Detonate
	Requires	FLY COMBAT DETONATE
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Detonate2
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\air_detonate 1 30
	TEnd
MEnd

Move A_Detonate2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	Lazy_CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_detonate 30
	TEnd
MEnd


#################################
###### Air Point Buff (ninjas) ######
#################################

Move A_Point_Buff
	Requires	FLY NINJATRAIN
	Priority	12
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Point_Buff2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_point 1 30
	TEnd
MEnd


Move A_Point_Buff2
	Requires	FLY
	Include sequencers\Members\Readies_Air.inc
	NextMove	flyready
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_point 30
	TEnd
MEnd


#################################
###### Air Command Summon (ninjas) ######
#################################

Move A_Command_Summon
	Requires	FLY NINJASUMMON
	Priority	12
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Command_Summon2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_command 1 50
	TEnd
MEnd

Move A_Command_Summon2
	Requires	FLY
	Include sequencers\Members\Readies_Air.inc
	NextMove	flyready
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_command 50
	TEnd
MEnd


##############################################
###### Air Ninja Upgrade 1 (Kuji_Rin + palmcast) ######
##############################################

Move A_NinjA_Upgrade_Rin
	Requires	FLY COMBAT NINJAUPGRADE
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_NinjA_Upgrade_Rin2
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\air_kuji_rin 1 50
	TEnd

	Type		fem
	  Anim          fem\air_kuji_rin 1 50
	TEnd

	Type		huge
	  Anim          huge\air_kuji_rin 1 50
	TEnd
MEnd

Move A_NinjA_Upgrade_Rin2
	Requires	FLY COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_NinjA_Upgrade_Rin_Post
	Interpolate	15
	Flags		PitchToTarget CANTMOVE NoSizeScale

	Type		male
	  Anim          male\air_palmcast 1 20
	  PitchStart	5
	  PitchAngle	-0.5
	TEnd
MEnd

Move A_NinjA_Upgrade_Rin_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flyready
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		male
	  Anim          male\air_palmcast 20
	  PitchEnd	34
	  PitchAngle	-0.5
	TEnd
MEnd


##############################################
###### Air Ninja Upgrade 2 (Kuji_Zen + palmcast) ######
###############################################

Move A_NinjA_Upgrade_Zen
	Requires	FLY COMBAT NINJAUPGRADE HIGH
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_NinjA_Upgrade_Zen2
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\air_kuji_sha 1 50
	TEnd

	Type		fem
	  Anim          fem\air_kuji_sha 1 50
	TEnd

	Type		huge
	  Anim          huge\air_kuji_sha 1 50
	TEnd
MEnd

Move A_NinjA_Upgrade_Zen2
	Requires	FLY COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_NinjA_Upgrade_Zen_Post
	Interpolate	15
	Flags		PitchToTarget CANTMOVE NoSizeScale

	Type		male
	  Anim          male\air_2Hcast 1 20
	TEnd

	Type		huge
	  Anim          huge\air_2Hcast 1 20
	TEnd

	Type		fem
	  Anim          fem\air_2Hcast 1 20
	TEnd
MEnd

Move A_NinjA_Upgrade_Zen_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle_half
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		male
	  Anim          male\air_2Hcast 20
	  PitchEnd	42
	TEnd

	Type		huge
	  Anim          huge\air_2Hcast 20
	  PitchEnd	42
	TEnd

	Type		fem
	  Anim          fem\air_2Hcast 20
	  PitchEnd	42
	TEnd
MEnd


########################################
###### Air Widows Attack (signature move)  ######
########################################

Move A_Widows_Attack
	Requires	FLY COMBAT WIDOWSATTACK
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Widows_Attack2
	Interpolate	20
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          fem\widows_attack 1 60
	TEnd
MEnd

Move A_Widows_Attack2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_Widows_Attack_cycle
	Flags		NoSizeScale

	Type		male
	  Anim          fem\widows_attack 60 90
	TEnd
MEnd

Move A_Widows_Attack_cycle
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          fem\widows_attack 90
	TEnd
MEnd


############################
###### Air TechShield Activate  ######
############################

Move A_TechShield_activate
	Requires	FLY TECHSHIELD
	Priority	12
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_TechShield_Boostup
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_wristband_activate 35 60
	  PlayFX	V_COV\Powers\Robotics\Wristband.fx 0
	TEnd

	Type		huge
	  Anim          huge\air_wristband_activate 35 60
	  PlayFX	V_COV\Powers\Robotics\Wristband.fx 0
	TEnd
MEnd

Move A_TechShield_Boostup
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	Interpolate	15
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_boostup 1
	TEnd

	Type		huge
	  Anim          huge\air_boostup 1
	TEnd

	Type		fem
	  Anim          fem\air_boostup 1
	TEnd
MEnd


#################################
###### Air Underhand Toss ######
#################################

Move A_Underhand
	Requires	FLY COMBAT UNDERHANDTOSS
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Underhand2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_Underhand_toss 1 45
	TEnd
MEnd

Move A_Underhand2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	Lazy_CombatReady
	Interpolate	15
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_Underhand_toss 45
	TEnd
MEnd


##############################
######## Air WindUp_Wristband Activate  ######
##############################

Move A_PreWindUp_Wristband
	Requires	FLY WRISTBANDWINDUP
	Priority	12
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ>
	NextMove	A_WindUp_Wristband_cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_wristband_activate 1 48
	TEnd

	Type		huge
	  Anim          huge\air_wristband_activate 1 48
	TEnd
MEnd

Move A_WindUp_Wristband_cycle
	Requires	FLY WRISTBANDWINDUP
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ>
	Flags		Cycle NoSizeScale
	NextMove	A_WindUp_Wristband_post

	Type		male
	  Anim          male\air_wristband_activate 48 99
	TEnd

	Type		huge
	  Anim          huge\air_wristband_activate 48 99
	TEnd
MEnd

Move A_WindUp_Wristband_post
	Requires	FLY
	Include sequencers\Members\Readies_Air.inc
	NextMove	Ready
	Interpolate	8
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_wristband_activate 48
	TEnd

	Type		huge
	  Anim          huge\air_wristband_activate 48
	TEnd
MEnd


##################################################
###### Air SUPERfAST_Wristband Activate (one touch)  ######
###################################################

Move A_Superfast_Wristband_activate
	Requires	FLY WRISTBANDACTIVATE QUICK HIGH
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Wristband_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_wristband_activate 42 60
	TEnd

	Type		huge
	  Anim          huge\air_wristband_activate 42 60
	TEnd
MEnd


############################
###### Air QUICK_Wristband Activate  ######
############################

Move A_QUICK_Wristband_activate
	Requires	FLY WRISTBANDACTIVATE QUICK
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Wristband_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_wristband_activate 35 60
	TEnd

	Type		huge
	  Anim          huge\air_wristband_activate 35 60
	TEnd
MEnd

Move A_WristbandActivate_Quick_NonRooted
	Requires	FLY WRISTBANDACTIVATE QUICK NOTROOTED
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ> <BlockIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Wristband_post
	#Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_wristband_activate 35 60
	TEnd

	Type		huge
	  Anim          huge\air_wristband_activate 35 60
	TEnd
MEnd

############################
###### Air Wristband Activate  ######
############################

Move A_Wristband_activate
	Requires	FLY WRISTBANDACTIVATE
	Priority	12
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Wristband_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_wristband_activate 1 60
	TEnd

	Type		huge
	  Anim          huge\air_wristband_activate 1 60
	TEnd
MEnd

Move A_Wristband_post
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove	W_FlyReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_wristband_activate 60 99
	TEnd

	Type		huge
	  Anim          huge\air_wristband_activate 60 99
	TEnd
MEnd


#################################
###### Air Sonic Yell  ######
#################################

Move A_SonicYell
	Requires	FLY COMBAT YELL STRONG ATTACK
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_SonicYell_post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_sonic_yell 1 40
	TEnd

	Type		huge
	  Anim          huge\air_sonic_yell 1 40
	TEnd
MEnd

Move A_SonicYell_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	Lazy_C_flycycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_sonic_yell 40
	TEnd

	Type		huge
	  Anim          huge\air_sonic_yell 40
	TEnd
MEnd


#################################
###### Air Sing Cast  ######
#################################

Move A_SingCast
	Requires	FLY COMBAT YELL YES CAST
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_SingCast_post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_sing_cast 1 68
	TEnd
MEnd

Move A_SingCast_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	Lazy_C_flycycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_sing_cast 68
	TEnd
MEnd


#################################
###### QUICK spit  ######
#################################

Move A_Spit
	Requires	FLY COMBAT BLOW CAST QUICK
	Priority	15
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Spitpost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_QUICKspit 1 30
	TEnd
MEnd

Move A_Spitpost
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	Lazy_C_flycycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_QUICKspit 30
	TEnd
MEnd


###########################
###### Air Waterbackpack  ######
###########################

Move A_Waterbackpack
	Requires	FLY COMBAT FIRE NO
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Waterbackpack2
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_waterbackpack 1 100
	TEnd
MEnd

Move A_Waterbackpack2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	Interpolate	8
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		male
	  Anim          male\air_waterbackpack 171
	  PitchEnd	75
	TEnd
MEnd

#####################
###### Air 2 Hand Swipe  ######
#####################

Move A_2H_Swipe
	Requires	FLY COMBAT 2HAND SWIPE
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_2H_cycle
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_mid 1 31
	TEnd

	Type		huge
	  Anim          huge\air_claw_mid 1 31
	TEnd

	Type		fem
	  Anim          fem\air_claw_mid 1 31
	TEnd
MEnd

Move A_2H_cycle
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	Flags		Cycle NoSizeScale
	Scale           0.50

	Type		male
	  Anim          male\air_claw_cycle 1 31
	TEnd
MEnd


###################
###### Air Moment O Glory  ######
###################

Move A_Glory
	Requires	FLY MOMENTGLORY
	Priority	12
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Glory2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_glory 1 45
	TEnd

	Type		huge
	  Anim          huge\air_glory 1 45
	TEnd

	Type		fem
	  Anim          fem\air_glory 1 45
	TEnd
MEnd

Move A_Glory2
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_glory 45
	TEnd

	Type		huge
	  Anim          huge\air_glory 45
	TEnd

	Type		fem
	  Anim          fem\air_glory 45
	TEnd
MEnd


###################
###### Air Leadership  ######
###################

Move A_Leadership_Pre
	Requires	FLY LEADERSHIP
	Priority	12
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_Leadership
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_leadership 1 2
	TEnd

	Type		huge
	  Anim          huge\air_leadership 1 2
	TEnd

	Type		fem
	  Anim          fem\air_leadership 1 2
	TEnd
MEnd

Move A_Leadership
	Requires	FLY
	Include sequencers\Members\Readies_Air.inc
	NextMove	flyready
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_leadership 3 109
	TEnd

	Type		huge
	  Anim          huge\air_leadership 3 109
	TEnd

	Type		fem
	  Anim          fem\air_leadership 3 109
	TEnd
MEnd


###################
###### Air TechWindup  ######
###################

Move A_Pre_TechWindup
	Requires	FLY TECHWINDUP
	Priority	12
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ>
	NextMove	A_TechWindup
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_sniperblast 1 40
	  PitchStart	5
	  PitchAngle	-0.5
	TEnd

	Type		huge
	  Anim          huge\air_sniperblast 1 40
	  PitchStart	5
	  PitchAngle	-0.5
	TEnd

	Type		fem
	  Anim          fem\air_sniperblast 1 40
	  PitchStart	5
	  PitchAngle	-0.5
	TEnd
MEnd

Move A_TechWindup
	Requires	FLY TECHWINDUP
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ>
	Flags		Cycle PitchToTarget NoSizeScale
	NextMove	A_TechWindup_post

	Type		male
	  Anim          male\air_sniperblast 40 60
	  PitchAngle	-0.5
	TEnd

	Type		huge
	  Anim          huge\air_sniperblast 40 60
	  PitchAngle	-0.5
	TEnd

	Type		fem
	  Anim          fem\air_sniperblast 40 60
	  PitchAngle	-0.5
	TEnd
MEnd

Move A_TechWindup_post
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_sniperblast 119
	TEnd

	Type		huge
	  Anim          huge\air_sniperblast 119
	TEnd

	Type		fem
	  Anim          fem\air_sniperblast 119
	TEnd
MEnd


###################
###### Air SniperBlast (hands)  ######
###################

Move A_SniperBlast
	Requires	FLY COMBAT STRONG CAST
	Priority	14
	Interrupts	<AttackIRQ> A_Pre_TechWindup A_TechWindup
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_SniperBlast2
	Flags		PitchToTarget NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\air_sniperblast 62 100
	  PitchAngle	-0.5
	  PitchEnd	94
	TEnd

	Type		huge
	  Anim          huge\air_sniperblast 62 100
	  PitchAngle	-0.5
	  PitchEnd	94
	TEnd

	Type		fem
	  Anim          fem\air_sniperblast 62 100
	  PitchAngle	-0.5
	  PitchEnd	94
	TEnd
MEnd

Move A_SniperBlast2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_sniperblast 100
	TEnd

	Type		huge
	  Anim          huge\air_sniperblast 100
	TEnd

	Type		fem
	  Anim          fem\air_sniperblast 100
	TEnd
MEnd


#########################################
###### Air QUICK SniperBlast (hands)  ######
#########################################

Move A_Q_SniperBlast
	Requires	FLY COMBAT QUICK SNIPERBLAST
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Q_SniperBlast2
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_sniperblast 1 100
	  PitchAngle	-0.5
	  PitchEnd	94
	TEnd

	Type		huge
	  Anim          huge\air_sniperblast 1 100
	  PitchAngle	-0.5
	  PitchEnd	94
	TEnd

	Type		fem
	  Anim          fem\air_sniperblast 1 100
	  PitchAngle	-0.5
	  PitchEnd	94
	TEnd
MEnd

Move A_Q_SniperBlast2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_sniperblast 100
	TEnd

	Type		huge
	  Anim          huge\air_sniperblast 100
	TEnd

	Type		fem
	  Anim          fem\air_sniperblast 100
	TEnd
MEnd


###################
###### Air WindUp  ######
###################

Move A_Pre_WindUp
	Requires	FLY WINDUP
	Priority	12
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups_Air.inc
	NextMove	A_WindUp
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_windup 1 42
	TEnd

	Type		huge
	  Anim          huge\air_windup 1 42
	TEnd

	Type		fem
	  Anim          fem\air_windup 1 42
	TEnd
MEnd

Move A_WindUp
	Requires	FLY WINDUP
	Include sequencers\Members\Windups_Air.inc
	Flags		Cycle NoSizeScale
	NextMove	A_WindUp_post

	Type		male
	  Anim          male\air_windup 42 63
	TEnd

	Type		huge
	  Anim          huge\air_windup 42 63
	TEnd

	Type		fem
	  Anim          fem\air_windup 42 63
	TEnd
MEnd

Move A_WindUp_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_windup 63 77
	TEnd

	Type		huge
	  Anim          huge\air_windup 63 77
	TEnd

	Type		fem
	  Anim          fem\air_windup 63 77
	TEnd
MEnd


################################
###### Air Windblast  ######
################################

Move A_Windblast
	Requires	FLY COMBAT WINDBLAST
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Windblastpost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_windblast 1 70
	TEnd

	Type		huge
	  Anim          huge\air_windblast 1 70
	TEnd

	Type		fem
	  Anim          fem\air_windblast 1 70
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_windblast 1 70
	TEnd
MEnd

Move A_Windblastpost
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_windblast 70
	TEnd

	Type		huge
	  Anim          huge\air_windblast 70
	TEnd

	Type		fem
	  Anim          fem\air_windblast 70
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_windblast 70
	TEnd
MEnd


################################
###### Air Rapidblast  ######
################################

Move A_Rapid
	Requires	FLY COMBAT RAPIDBLAST
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Rapidpost
	Sets            CANTMOVE
	Flags		PITCHTOTARGET NoSizeScale

	Type		male
	  Anim          male\air_energy_rapid 1 31
	TEnd
MEnd

Move A_Rapidpost
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_energy_rapid 31
	TEnd
MEnd


################################
###### Air DarkBlast or Megablast  ######
################################

Move A_Mega
	Requires	FLY COMBAT MEGABLAST
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Megapost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_darkblast 1 76
	  PitchStart	62
	TEnd

	Type		huge
	  Anim          huge\air_megablast 1 76
	  PitchStart	62
	TEnd

	Type		fem
	  Anim          fem\air_megablast 1 76
	  PitchStart	62
	TEnd
MEnd

Move A_Megapost
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle_half
	Scale           0.50
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_darkblast 76
	TEnd

	Type		huge
	  Anim          huge\air_megablast 76
	TEnd

	Type		fem
	  Anim          fem\air_megablast 76
	TEnd
MEnd


###################
###### Air Blow Cast  ######
###################

Move A_Blow
	Requires	FLY COMBAT BLOW
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Blowpost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_blow_cast 1 80
	  PitchStart	12
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\air_blow_cast 1 80
	  PitchStart	12
	  PitchAngle	0.2
	TEnd

	Type		fem
	  Anim          fem\air_blow_cast 1 80
	  PitchStart	12
	  PitchAngle	0.2
	TEnd
MEnd

Move A_Blowpost
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle_half
	Scale           0.50
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_blow_cast 80
	  PitchEnd	85
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\air_blow_cast 80
	  PitchEnd	85
	  PitchAngle	0.2
	TEnd

	Type		fem
	  Anim          fem\air_blow_cast 80
	  PitchEnd	85
	  PitchAngle	0.2
	TEnd
MEnd


###################
###### Air QUICK_Blinding Cast  ######
###################

Move A_QUICKblinding
	Requires	FLY COMBAT BLINDING LOW STRONG
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_blindingpost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_blinding 39 88
	TEnd

	Type		huge
	  Anim          huge\air_blinding 39 88
	TEnd

	Type		fem
	  Anim          fem\air_blinding 39 88
	TEnd
MEnd


###################
###### Air Blinding Cast  ######
###################

Move A_blinding
	Requires	FLY COMBAT BLINDING
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_blindingpost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_blinding 1 88
	TEnd

	Type		huge
	  Anim          huge\air_blinding 1 88
	TEnd

	Type		fem
	  Anim          fem\air_blinding 1 88
	TEnd
MEnd

Move A_blindingpost
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle_half
	Scale           0.50
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_blinding 88
	TEnd

	Type		huge
	  Anim          huge\air_blinding 88
	TEnd

	Type		fem
	  Anim          fem\air_blinding 88
	TEnd
MEnd


###################
###### Air Boost Aura (flameaura)  ######
###################

Move A_Boostaura
	Requires	FLY BOOSTAURA
	Priority	12
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Boostaura2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_ground_summon 1 40
	TEnd

	Type		huge
	  Anim          huge\air_ground_summon 1 40
	TEnd

	Type		fem
	  Anim          fem\air_ground_summon 1 40
	TEnd
MEnd

Move A_Boostaura2
	Requires	FLY
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Boostaura3
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_fireaura 1 47
	TEnd

	Type		huge
	  Anim          huge\air_fireaura 1 47
	TEnd

	Type		fem
	  Anim          fem\air_fireaura 1 47
	TEnd
MEnd

Move A_Boostaura3
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flyready
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_fireaura 48
	TEnd

	Type		huge
	  Anim          huge\air_fireaura 48
	TEnd

	Type		fem
	  Anim          fem\air_fireaura 48
	TEnd
MEnd


###################
###### Air QUICK Boost Aura (flameaura)  ######
###################

Move A_BoostaurA_QUICK
	Requires	FLY QUICK BOOSTAURA
	Priority	13
	Interrupts	<AttackIRQ>
	NextMove	A_BoostaurA_QUICK2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_fireaura 1 48
	TEnd

	Type		huge
	  Anim          huge\air_fireaura 1 48
	TEnd

	Type		fem
	  Anim          fem\air_fireaura 1 48
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_boostaura 1 48
	TEnd
MEnd

Move A_BoostaurA_QUICK2
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flyready
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_fireaura 48
	TEnd

	Type		huge
	  Anim          huge\air_fireaura 48
	TEnd

	Type		fem
	  Anim          fem\air_fireaura 48
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_boostaura 48
	TEnd
MEnd


###################
###### Air Poison Mode (MR.Spikey)  ######
###################

Move A_PrePoison
	Requires	FLY COMBAT POISON
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_PrePoison2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_pre_poison 1 88
	TEnd

	Type		huge
	  Anim          huge\air_pre_poison 1 88
	TEnd

	Type		fem
	  Anim          fem\air_pre_poison 1 88
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_pre_poison 1 88
	TEnd
MEnd

Move A_PrePoison2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flyready
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_pre_poison 88
	  Scale		0.50
	TEnd

	Type		huge
	  Anim          huge\air_pre_poison 88
	  Scale		0.50
	TEnd

	Type		fem
	  Anim          fem\air_pre_poison 88
	  Scale		0.50
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_pre_poison 88
	TEnd
MEnd

###############################################
###### transform_into_Kheldian ######
###############################################

Move A_Transform_Kheldian
	Requires	FLY COMBAT POISON STRONG MID
	Priority	15
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_Transform_Kheldian2
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_pre_poison 1 2
	  PlayFx        FEET\Transform.FX
	TEnd

	Type		huge
	  Anim          huge\air_pre_poison 1 2
	  PlayFx        FEET\Transform.FX
	TEnd

	Type		fem
	  Anim          fem\air_pre_poison 1 2
	  PlayFx        FEET\Transform.FX
	TEnd
MEnd

Move A_Transform_Kheldian2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flyready
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_pre_poison 3
	  PlayFx        FEET\Transform.FX
	TEnd

	Type		huge
	  Anim          huge\air_pre_poison 3
	  PlayFx        FEET\Transform.FX
	TEnd

	Type		fem
	  Anim          fem\air_pre_poison 3
	  PlayFx        FEET\Transform.FX
	TEnd
MEnd

##################
###### Air Poison Stab  ######
###################

Move A_Poisonstab_R
	Requires	FLY COMBAT ATTACK POISON
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Poisonstab_R2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_poison_stab 1 22
	TEnd

	Type		huge
	  Anim          huge\air_poison_stab 1 22
	TEnd

	Type		fem
	  Anim          fem\air_poison_stab 1 22
	TEnd
MEnd

Move A_Poisonstab_R2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_poisoncycle_15
	Scale           0.50
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_poison_stab 22 46
	TEnd

	Type		huge
	  Anim          huge\air_poison_stab 22 46
	TEnd

	Type		fem
	  Anim          fem\air_poison_stab 22 46
	TEnd
MEnd

#############################################################

Move A_Poisonstab_L
	Requires	FLY COMBAT ATTACK POISON
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Poisonstab_L2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_poison_stab 48 68
	TEnd

	Type		huge
	  Anim          huge\air_poison_stab 48 68
	TEnd

	Type		fem
	  Anim          fem\air_poison_stab 48 68
	TEnd
MEnd

Move A_Poisonstab_L2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_poisoncycle_15
	Scale           0.50
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_poison_stab 68
	TEnd

	Type		huge
	  Anim          huge\air_poison_stab 68
	TEnd

	Type		fem
	  Anim          fem\air_poison_stab 68
	TEnd
MEnd

#############################################################

Move A_poisoncycle_15
	Requires	FLY COMBAT POISON
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_poisoncycle
	Scale           0.50
	Sets            FLY
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_cycle 15 31
	TEnd

	Type		huge
	  Anim          huge\air_claw_cycle 15 31
	TEnd

	Type		fem
	  Anim          fem\air_claw_cycle 15 31
	TEnd
MEnd

#############################################################

Move A_poisoncycle
	Requires	FLY COMBAT POISON
	Include sequencers\Members\Combat_Air.inc
	Flags		Cycle NoSizeScale
	NextMove	C_PreFall
	Scale           0.50
	Sets            FLY

	Type		male
	  Anim          male\air_claw_cycle 1 31
	TEnd

	Type		huge
	  Anim          huge\air_claw_cycle 1 31
	TEnd

	Type		fem
	  Anim          fem\air_claw_cycle 1 31
	TEnd
MEnd


###################
###### Air Cast  ######
###################

Move airCast
	Requires	FLY COMBAT CAST
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	airCastpost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_palmcast 1 30
	  PitchStart	5
	  PitchAngle	-0.5
	TEnd

	Type		huge
	  Anim          huge\air_cast 1 30
	  PitchStart	5
	  PitchAngle	-0.5
	TEnd

	Type		fem
	  Anim          fem\air_cast 1 30
	  PitchStart	5
	  PitchAngle	-0.5
	TEnd
MEnd

Move airCastpost
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flyready
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_palmcast 30
	TEnd

	Type		huge
	  Anim          huge\air_cast 30
	TEnd

	Type		fem
	  Anim          fem\air_cast 30
	TEnd
MEnd


###################
###### Air 2Hand Cast  ######
###################

Move A_2HandCast
	Requires	FLY COMBAT 2HAND CAST
	Priority	15
	Interrupts	<AttackIRQ> <Windups>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_2HandCastpost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_2Hcast 1 50
	  PitchEnd	42
	  PitchAngle	0
	TEnd

	Type		huge
	  Anim          huge\air_2Hcast 1 50
	  PitchEnd	42
	  PitchAngle	0
	TEnd

	Type		fem
	  Anim          fem\air_2Hcast 1 50
	  PitchEnd	42
	  PitchAngle	0
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_2hcast 1 50
	 PitchEnd	42
	  PitchAngle	0
	TEnd
MEnd

Move A_2HandCastpost
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_2Hcast 50
	TEnd

	Type		huge
	  Anim          huge\air_2Hcast 50
	TEnd

	Type		fem
	  Anim          fem\air_2Hcast 50
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_2hcast 50
	TEnd
MEnd


###################
###### Air Ball Cast  ######
###################

Move A_BallCast
	Requires	FLY COMBAT BALLCAST
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_BallCastpost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_ball_cast 1 70
	 PitchEnd	60
	TEnd

	Type		huge
	  Anim          huge\air_ballcast 1 70
	  PitchEnd	60
	TEnd

	Type		fem
	  Anim          fem\air_ballcast 1 70
	  PitchEnd	60
	TEnd
MEnd

Move A_BallCastpost
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_ball_cast 70
	TEnd

	Type		huge
	  Anim          huge\air_ballcast 70
	TEnd

	Type		fem
	  Anim          fem\air_ballcast 70
	TEnd
MEnd


###################
###### Air Repulsion Bomb  ######
###################

Move A_RepulsionBomb_Pre
	Requires	FLY COMBAT REPULSION
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_RepulsionBomb_02
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_ball_cast 1 30
	 PitchEnd	60
	TEnd

	Type		huge
	  Anim          huge\air_ballcast 1 30
	  PitchEnd	60
	TEnd

	Type		fem
	  Anim          fem\air_ballcast 1 30
	  PitchEnd	60
	TEnd
MEnd

Move A_RepulsionBomb_02
	Requires	FLY COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	Priority	13
	NextMove	A_RepulsionBomb_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_ball_cast 45 69
	 PitchEnd	60
	TEnd

	Type		huge
	  Anim          huge\air_ballcast 45 69
	  PitchEnd	60
	TEnd

	Type		fem
	  Anim          fem\air_ballcast 45 69
	  PitchEnd	60
	TEnd
MEnd

Move A_RepulsionBomb_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_ball_cast 69
	TEnd

	Type		huge
	  Anim          huge\air_ballcast 69
	TEnd

	Type		fem
	  Anim          fem\air_ballcast 69
	TEnd
MEnd


###################
###### Air magic throw  ######
###################

Move A_magicthrow
	Requires	FLY COMBAT MAGICTHROW
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_magicthrowpost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_frisbee 1 30
	  PitchAngle	0.3
	  PitchStart	11
	  PitchEnd	23
	TEnd

	Type		huge
	  Anim          huge\air_frisbe 1 30
	  PitchAngle	0.3
	  PitchStart	11
	  PitchEnd	23
	TEnd

	Type		fem
	  Anim          fem\air_frisbe 1 30
	  PitchAngle	0.3
	  PitchStart	11
	  PitchEnd	23
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_frisbee 1 30
	  PitchAngle	0.3
	  PitchStart	11
	  PitchEnd	23
	TEnd
MEnd

Move A_magicthrowpost
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_frisbee 30
	TEnd

	Type		huge
	  Anim          huge\air_frisbe 30
	TEnd

	Type		fem
	  Anim          fem\air_frisbe 30
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_frisbee 30
	TEnd
MEnd


###################
###### Air Smash Cast Radius  ######
###################

Move A_SmashCast
	Requires	FLY COMBAT SMASH
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_SmashCastpost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_inferno 1 45
	TEnd

	Type		huge
	  Anim          huge\air_smashcast 1 45
	TEnd

	Type		fem
	  Anim          fem\air_smashcast 1 45
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_1hsmash 1 45
	TEnd
MEnd

Move A_SmashCastpost
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove	Fake_C_flycycle_Half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_inferno 45
	TEnd

	Type		huge
	  Anim          huge\air_smashcast 45
	TEnd

	Type		fem
	  Anim          fem\air_smashcast 45
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_1hsmash 45
	TEnd
MEnd


###################
###### Air Disrupt  ######
###################

Move A_Disrupt
	Requires	FLY COMBAT DISRUPT
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Disruptpost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_disrupt 1 69
	TEnd

	Type		huge
	  Anim          huge\air_disrupt 1 69
	TEnd

	Type		fem
	  Anim          fem\air_disrupt 1 69
	TEnd
MEnd

Move A_Disruptpost
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle_half
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_disrupt 69 121
	TEnd

	Type		huge
	  Anim          huge\air_disrupt 69 121
	TEnd

	Type		fem
	  Anim          fem\air_disrupt 69 121
	TEnd
MEnd


###################
###### Air Levitate  ######
###################

Move A_Levitate
	Requires	FLY COMBAT LEVITATE
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Levitatepost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_levitate 1 50
	TEnd

	Type		huge
	  Anim          huge\air_levitate 1 50
	TEnd

	Type		fem
	  Anim          fem\air_levitate 1 50
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_levitate 1 50
	TEnd
MEnd

Move A_Levitatepost
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_levitate 50
	TEnd

	Type		huge
	  Anim          huge\air_levitate 50
	TEnd

	Type		fem
	  Anim          fem\air_levitate 50
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_levitate 50
	TEnd
MEnd


###################
###### Air Telekinesis_QUICK  ######
###################

Move airTeleQUICK
	Requires	FLY COMBAT ATTACK TELEKINESIS
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	airTeleQUICKpost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_telekinesis 2 35
	  PitchStart	9
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\air_telekinesis 2 35
	  PitchStart	9
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\air_telekinesis 2 35
	  PitchStart	9
	  PitchAngle	0.5
	TEnd
MEnd

Move airTeleQUICKpost
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flyready
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		male
	  Anim          male\air_telekinesis 35
	  PitchEnd	96
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\air_telekinesis 35
	  PitchEnd	96
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\air_telekinesis 35
	  PitchEnd	96
	  PitchAngle	0.5
	TEnd
MEnd


###################
###### Air Psionic Windup  ######
###################

Move A_Pre_PsiWindup
	Requires	FLY COMBAT MYSICWINDUP #STARTOFFENSESTANCE
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ>
	NextMove	A_PsiWindup
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_psionics 1 49
	 PitchStart	13
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\air_psionic 1 49
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

	Type		fem
	  Anim          fem\air_psionic 1 49
	  PitchStart	13
	  PitchAngle	0.2
	TEnd
MEnd

Move A_PsiWindup
	Requires	FLY COMBAT MYSICWINDUP
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ>
	Flags		Cycle PitchToTarget NoSizeScale
	NextMove	A_PsiWindup_post

	Type		male
	  Anim          male\air_psionics 49 89
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\air_psionic 49 89
	  PitchAngle	0.2
	TEnd

	Type		fem
	  Anim          fem\air_psionic 49 89
	  PitchAngle	0.2
	TEnd
MEnd

Move A_PsiWindup_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_psionics 165
	TEnd

	Type		huge
	  Anim          huge\air_psionic 165
	TEnd

	Type		fem
	  Anim          fem\air_psionic 165
	TEnd
MEnd


###################
###### Air QUICK Psionic Blast (comes after psi windup)  ######
###################

Move A_PsiQUICK
	Requires	FLY COMBAT QUICK PSIONICBLAST
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_PsiQUICK2
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_psionics 97 125
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\air_psionic 97 125
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

	Type		fem
	  Anim          fem\air_psionic 97 125
	  PitchStart	13
	  PitchAngle	0.2
	TEnd
MEnd

Move A_PsiQUICK2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_psionics 165
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\air_psionic 165
	  PitchAngle	0.2
	TEnd

	Type		fem
	  Anim          fem\air_psionic 165
	  PitchAngle	0.2
	TEnd
MEnd


###################
###### Air Psionics  ######
###################

Move airPsionics
	Requires	FLY COMBAT PSIONIC
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	airPostPsionics
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_psionics 2 89
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\air_psionic 2 89
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

	Type		fem
	  Anim          fem\air_psionic 2 89
	  PitchStart	13
	  PitchAngle	0.2
	TEnd
MEnd

#############################################################

Move airPsionics_SlowerCast
	Requires	FLY COMBAT PSIONIC HIGH
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	airPostPsionics
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale
	Scale		0.9

	Type		male
	  Anim          male\air_psionics 2 89
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\air_psionic 2 89
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

	Type		fem
	  Anim          fem\air_psionic 2 89
	  PitchStart	13
	  PitchAngle	0.2
	TEnd
MEnd

Move airPostPsionics
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle_half
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_psionics 162
	TEnd

	Type		huge
	  Anim          huge\air_psionic 162
	TEnd

	Type		fem
	  Anim          fem\air_psionic 162
	TEnd
MEnd

#############################################################

Move A_Psionics_NoRoot
	Requires	FLY COMBAT PSIONIC NOTROOTED
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>  <MoveIRQ> <IRQEmotesCombat>
	NextMove	airPostPsionics
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_psionics 2 89
	TEnd

	Type		huge
	  Anim          huge\air_psionic 2 89
	TEnd

	Type		fem
	  Anim          fem\air_psionic 2 89
	TEnd
MEnd

## PSIONIC BLAST (QUICK) ###########################################################

Move A_PsiBlast_QUICK_Pre
	Requires	FLY COMBAT PSIONICBLAST HIGH
	Priority	14
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_PsiBlast
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale
	scale		2.0

	Type		male
	  Anim          male\air_psionics 2 51
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\air_psionic 2 51
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

	Type		fem
	  Anim          fem\air_psionic 2 51
	  PitchStart	13
	  PitchAngle	0.2
	TEnd
MEnd


## PSIONIC BLAST ###########################################################

Move A_PsiBlast_Pre
	Requires	FLY COMBAT PSIONICBLAST
	Priority	13
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_PsiBlast
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_psionics 2 51
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\air_psionic 2 51
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

	Type		fem
	  Anim          fem\air_psionic 2 51
	  PitchStart	13
	  PitchAngle	0.2
	TEnd
MEnd

Move A_PsiBlast
	Requires	FLY COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_PsiBlast_Post
	Sets		CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_psionics 96 121
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\air_psionic 96 121
	  PitchAngle	0.2
	TEnd

	Type		fem
	  Anim          fem\air_psionic 96 121
	  PitchAngle	0.2
	TEnd
MEnd

Move A_PsiBlast_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle_half
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_psionics 1221
	TEnd

	Type		huge
	  Anim          huge\air_psionic 121
	TEnd

	Type		fem
	  Anim          fem\air_psionic 121
	TEnd
MEnd


###################
###### Air Radius Effect  ######
###################

Move A_RadiusEffect
	Requires	FLY COMBAT RADIUSEFFECT
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_RadiusEffectPost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_radiuseffect 1 60
	TEnd

	Type		huge
	  Anim          huge\air_radiuseffect 1 60
	TEnd

	Type		fem
	  Anim          fem\air_radiuseffect 1 60
	TEnd
MEnd

Move A_RadiusEffectPost
	Requires	FLY
	Include sequencers\Members\Readies_Fake.inc
	NextMove	Fake_C_flycycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_radiuseffect 60
	TEnd

	Type		huge
	  Anim          huge\air_radiuseffect 60
	TEnd

	Type		fem
	  Anim          fem\air_radiuseffect 60
	TEnd
MEnd


###################
###### Air Radius Attack  ######
###################

Move airRadiusAttack
	Requires	FLY COMBAT RADIUSATTACK
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	airPostRadAttack
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_radiusattack 1 30
	TEnd

	Type		huge
	  Anim          huge\air_radiusattack 1 30
	TEnd

	Type		fem
	  Anim          fem\air_radiusattack 1 30
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_radiusattack 1 30
	TEnd
MEnd

Move airPostRadAttack
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flyready
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_radiusattack 30
	TEnd

	Type		huge
	  Anim          huge\air_radiusattack 30
	TEnd

	Type		fem
	  Anim          fem\air_radiusattack 30
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_radiusattack 30
	TEnd
MEnd


###################
###### Air Force Ram  ######
###################

Move airForceRam
	Requires	FLY COMBAT FORCE
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	airPostForceRam
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_forceram 1 33
	  PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\air_forceram 1 33
	  PitchAngle	0.1
	TEnd

	Type		fem
	  Anim          fem\air_forceram 1 33
	  PitchAngle	0.1
	TEnd
MEnd

Move airPostForceRam
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flyready
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		male
	  Anim          male\air_forceram 33
	  PitchAngle	0.1
	  PitchEnd	44
	TEnd

	Type		huge
	  Anim          huge\air_forceram 33
	  PitchAngle	0.1
	  PitchEnd	44
	TEnd

	Type		fem
	  Anim          fem\air_forceram 33
	  PitchAngle	0.1
	  PitchEnd	44
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

	Type		male
	  Anim          male\air_cage 1 35
	TEnd

	Type		huge
	  Anim          huge\air_cage 1 35
	TEnd

	Type		fem
	  Anim          fem\air_cage 1 35
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_handclap 1 35
	TEnd
MEnd

Move airCagepost
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flyready
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_cage 30
	TEnd

	Type		huge
	  Anim          huge\air_cage 30
	TEnd

	Type		fem
	  Anim          fem\air_cage 30
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_handclap 30
	TEnd
MEnd


## AIR HEADCAST ###########################################################

Move A_Mouth
	Requires	FLY COMBAT HEADCAST
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Mouth_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_mouthcast 1 50
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\air_mouth 1 50
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		fem
	  Anim          fem\air_mouth 1 50
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		Wisp
	  Anim          Wisp\headcast 1 90
	TEnd
MEnd

#############################################################

Move A_Mouth_Slow
	Requires	FLY COMBAT HEADCAST HIGH STRONG
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Mouth_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale
	Scale		0.625

	Type		male
	  Anim          male\air_mouthcast 1 50
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\air_mouth 1 50
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		fem
	  Anim          fem\air_mouth 1 50
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		Wisp
	  Anim          Wisp\headcast 1 90
	TEnd
MEnd

Move A_Mouth_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flyready
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_mouthcast 50
	TEnd

	Type		huge
	  Anim          huge\air_mouth 50
	TEnd

	Type		fem
	  Anim          fem\air_mouth 50
	TEnd

	Type		Wisp
	  Anim          Wisp\headcast 90
	TEnd
MEnd


###################
###### Air mouth_eye blast_SFX - WITH SOUND!  ######
###################

Move airMouth_SFX
	Requires	FLY COMBAT MOUTHSFX
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Mouth_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_mouthcast 1 49
	  PitchStart	1
	  PitchAngle	0.1
	  PlayFX	feet\Male_Rage1.fx
	TEnd

	Type		huge
	  Anim          huge\air_mouth 1 49
	  PitchStart	1
	  PitchAngle	0.1
	  PlayFX	feet\Male_Rage2.fx
	TEnd

	Type		fem
	  Anim          fem\air_mouth 1 49
	  PitchStart	1
	  PitchAngle	0.1
	  PlayFX	feet\FemRage.fx
	TEnd

	Type		Wisp
	  Anim          Wisp\headcast 1 90
	TEnd
MEnd


###################
###### Air LONG mouth_eye blast  ######
###################

Move airMouth_long
	Requires	FLY COMBAT HIGH HEADCAST
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	airMouth_2
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_mouthcast 1 46
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\air_mouth 1 46
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		fem
	  Anim          fem\air_mouth 1 46
	  PitchStart	1
	  PitchAngle	0.1
	TEnd
MEnd

Move airMouth_2
	Requires	FLY COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	airMouth_8
	Scale           0.07
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_mouthcast 40 47
	  PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\air_mouth 40 47
	  PitchAngle	0.1
	TEnd

	Type		fem
	  Anim          fem\air_mouth 40 47
	  PitchAngle	0.1
	TEnd
MEnd

Move airMouth_8
	Requires	FLY COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Mouth_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_mouthcast 40 49
	  PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\air_mouth 40 49
	  PitchAngle	0.1
	TEnd

	Type		fem
	  Anim          fem\air_mouth 40 49
	  PitchAngle	0.1
	TEnd
MEnd


###################
###### Air Sky Summon  ######
###################

Move airS_Summon
	Requires	FLY COMBAT SKYSUMMON
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	airS_Summonpost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_skysummon 1 60
	TEnd

	Type		huge
	  Anim          huge\air_skysummon 1 60
	TEnd

	Type		fem
	  Anim          fem\air_skysummon 1 60
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_skysummon 1 60
	TEnd
MEnd

Move airS_Summonpost
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flyready
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_skysummon 60
	TEnd

	Type		huge
	  Anim          huge\air_skysummon 60
	TEnd

	Type		fem
	  Anim          fem\air_skysummon 60
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_skysummon 60
	TEnd
MEnd


###################
###### Air Ground Summon  ######
###################

Move airG_Summon
	Requires	FLY COMBAT GROUNDSUMMON
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	airG_Summonpost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_groundsummon 1 60
	TEnd

	Type		huge
	  Anim          huge\air_ground_summon 1 60
	TEnd

	Type		fem
	  Anim          fem\air_ground_summon 1 60
	TEnd
MEnd

Move airG_Summonpost
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flyready
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_groundsummon 60
	TEnd

	Type		huge
	  Anim          huge\air_ground_summon 60
	TEnd

	Type		fem
	  Anim          fem\air_ground_summon 60
	TEnd
MEnd


###################
###### Air magic cast Left  ######
###################

Move airMagic_L
	Requires	FLY COMBAT MAGIC CAST
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	airMagic_Lpost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_magicL 1 30
	  PitchStart	10
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\air_magicL 1 30
	  PitchStart	10
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\air_magicL 1 30
	  PitchStart	10
	  PitchAngle	0.5
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_magicL 1 30
	  PitchStart	10
	  PitchAngle	0.5
	TEnd
MEnd

Move airMagic_Lpost
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flyready
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_magicL 30
	  PitchEnd	60
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\air_magicL 30
	  PitchEnd	60
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\air_magicL 30
	  PitchEnd	60
	  PitchAngle	0.5
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_magicL 30
	  PitchEnd	60
	  PitchAngle	0.5
	TEnd
MEnd


###################
###### Air Steal power  ######
###################

Move A_Steal
	Requires	FLY COMBAT STEAL
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Stealpost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_steal 1 58
	  PitchStart	8
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\air_steal 1 58
	  PitchStart	8
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\air_steal 1 58
	  PitchStart	8
	  PitchAngle	0.5
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_steal 1 58
	  PitchStart	8
	  PitchAngle	0.5
	TEnd
MEnd

Move A_Stealpost
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		male
	  Anim          male\air_steal 58
	  PitchEnd	70
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\air_steal 58
	  PitchEnd	70
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\air_steal 58
	  PitchEnd	70
	  PitchAngle	0.5
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_steal 58
	  PitchEnd	70
	  PitchAngle	0.5
	TEnd
MEnd


###################
###### Air Bigger Steal power  ######
###################

Move A_BigSteal
	Requires	FLY COMBAT STRONG STEAL
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_BigStealpost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_new_steal 1 58
	  PitchStart	10
	  PitchEnd	109
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\air_new_steal 1 58
	  PitchStart	10
	  PitchEnd	109
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\air_new_steal 1 58
	  PitchStart	10
	  PitchEnd	109
	  PitchAngle	0.5
	TEnd
MEnd

Move A_BigStealpost
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_new_steal 58
	TEnd

	Type		huge
	  Anim          huge\air_new_steal 58
	TEnd

	Type		fem
	  Anim          fem\air_new_steal 58
	TEnd
MEnd


###################
###### Air Heal cast  ######
###################

Move airHeal
	Requires	FLY COMBAT HEAL
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	airHealpost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_heal 1 35
	  PitchStart	8
	  PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\air_heal 1 35
	  PitchStart	8
	  PitchAngle	0.1
	TEnd

	Type		fem
	  Anim          fem\air_heal 1 35
	  PitchStart	8
	  PitchAngle	0.1
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_heal 1 60
	  PitchStart	8
	  PitchAngle	0.1
	TEnd
MEnd

Move airHealpost
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flyready
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		male
	  Anim          male\air_heal 35
	  PitchEnd	63
	  PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\air_heal 35
	  PitchEnd	63
	  PitchAngle	0.1
	TEnd

	Type		fem
	  Anim          fem\air_heal 35
	  PitchEnd	63
	  PitchAngle	0.1
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_heal 60
	  PitchEnd	63
	  PitchAngle	0.1
	TEnd
MEnd

###################
###### air Wisp AOE  ######
###################

Move A_Wisp_AOE
	Requires	FLY COMBAT BOOSTUP RADIUS
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Wisp_AOE_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          Wisp\headcast_AOE 1 105
	TEnd
MEnd

Move A_Wisp_AOE_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          Wisp\headcast_AOE 105
	TEnd
MEnd

###################
###### air Boost_up  ######
###################

Move A_Boostup
	Requires	FLY BOOSTUP
	Priority	12
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Boostuppost
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

	Type		Wisp
	  Anim          Wisp\headcast_AOE 1 2
	TEnd
MEnd

Move A_Boostuppost
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_boostup 2
	TEnd

	Type		huge
	  Anim          huge\air_boostup 2
	TEnd

	Type		fem
	  Anim          fem\air_boostup 2
	TEnd

	Type		Wisp
	  Anim          Wisp\headcast_AOE 3 105
	TEnd
MEnd

##################
###### air Boost_down  ######
###################

Move A_Boost_D
	Requires	FLY BOOSTDOWN
	Priority	12
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Boost_Dpost
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_boostdown 1 2
	TEnd

	Type		huge
	  Anim          huge\air_boostdown 1 2
	TEnd

	Type		fem
	  Anim          fem\air_boostdown 1 2
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_boostdown 1 2
	TEnd
MEnd

Move A_Boost_Dpost
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_boostdown 2
	TEnd

	Type		huge
	  Anim          huge\air_boostdown 2
	TEnd

	Type		fem
	  Anim          fem\air_boostdown 2
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_boostdown 2
	TEnd
MEnd

###################
###### air Wall  ######
###################

Move A_Wall
	Requires	FLY COMBAT WALL
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Wallpost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_wall 1 30
	TEnd

	Type		huge
	  Anim          huge\air_wall 1 30
	TEnd

	Type		fem
	  Anim          fem\air_wall 1 30
	TEnd
MEnd

Move A_Wallpost
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_wall 30
	TEnd

	Type		huge
	  Anim          huge\air_wall 30
	TEnd

	Type		fem
	  Anim          fem\air_wall 30
	TEnd
MEnd


###################
###### air Flurry o Fists  ######
###################

Move A_Flurry
	Requires	FLY COMBAT PUNCH SPEEDPUNCH
	Priority	14
	Interrupts	<AttackIRQ> A_Speed_react
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Flurrypost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_speedattack 1 88
	TEnd

	Type		huge
	  Anim          huge\air_speedattack 1 88
	TEnd

	Type		fem
	  Anim          fem\air_speedattack 1 88
	TEnd
MEnd

Move A_Flurrypost
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flyready
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_speedattack 88
	TEnd

	Type		huge
	  Anim          huge\air_speedattack 88
	TEnd

	Type		fem
	  Anim          fem\air_speedattack 88
	TEnd
MEnd


## AIR 'BRAWL' PUNCH - RIGHT ###########################################################

Move A_BrawlRight
	Requires	FLY COMBAT PUNCH #Don't use BRAWL for this
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_BrawlRight_Post
	Sets            CANTMOVE
	SticksOnChild   FLY LOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_brawl_HIGH 1 25
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_swipeR 1 18
	TEnd
MEnd

Move A_BrawlRight_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_Brawlcycle_15
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_brawl_HIGH 25 71
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_swipeR 18
	TEnd
MEnd

## 'BRAWL' PUNCH - LEFT ###########################################################

Move A_BrawlLeft
	Requires	FLY COMBAT PUNCH #Don't use BRAWL for this
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_BrawlLeft_Post
	Sets            CANTMOVE
	SticksOnChild   FLY Love
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_brawl_HIGH 78 102
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_swipeL 1 18
	TEnd
MEnd

Move A_BrawlLeft_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_Brawlcycle_15
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_brawl_HIGH 102
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_swipeL 19
	TEnd
MEnd

#############################################################

Move AirBrawlcycle
	Requires	FLY COMBAT
	Priority	12
	Include sequencers\Members\Combat_Air.inc
	Flags		Cycle NoSizeScale
	NextMove	C_PreFall
	Scale           0.50

	Type		male
	  Anim          male\air_brawl_cycle 1 31
	TEnd

	Type		huge
	  Anim          huge\air_brawlcycle 1 31
	TEnd

	Type		fem
	  Anim          fem\air_brawlcycle 1 31
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_float 1
	TEnd
MEnd

#############################################################

Move A_Brawlcycle_15
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	AirBrawlcycle
	Scale           0.50
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_brawl_cycle 16 31
	TEnd

	Type		huge
	  Anim          huge\air_brawlcycle 16 31
	TEnd

	Type		fem
	  Anim          fem\air_brawlcycle 16 31
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_float 1
	TEnd
MEnd

###################
###### air 1 2 punch ######
###################

Move A_1_2punch
	Requires	FLY COMBAT PUNCH COMBO2
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_1_2punchpost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_1_2punch 1 40
	TEnd

	Type		huge
	  Anim          huge\air_1_2punch 1 40
	TEnd

	Type		fem
	  Anim          fem\air_1_2punch 1 40
	TEnd
MEnd

Move A_1_2punchpost
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_Brawlcycle_15
	Scale           0.50
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_1_2punch 40
	TEnd

	Type		huge
	  Anim          huge\air_1_2punch 40
	TEnd

	Type		fem
	  Anim          fem\air_1_2punch 40
	TEnd
MEnd


###################
###### air Gut Punch ######
###################

Move A_Gutpunch
	Requires	FLY COMBAT GUTPUNCH
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Gutpunch_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_gut_punch 1 31
	TEnd

	Type		huge
	  Anim          huge\air_gut_punch 1 31
	TEnd

	Type		fem
	  Anim          fem\air_gut_punch 1 31
	TEnd
MEnd

Move A_Gutpunch_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_Brawlcycle_15
	Scale           0.50
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_gut_punch 31
	TEnd

	Type		huge
	  Anim          huge\air_gut_punch 31
	TEnd

	Type		fem
	  Anim          fem\air_gut_punch 31
	TEnd
MEnd


###################
###### air jumpNsmash ######
###################

Move A_jumpNsmash
	Requires	FLY COMBAT JUMPNSMASH
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_jumpNsmash_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_jump_n_smash 1 98
	TEnd

	Type		huge
	  Anim          huge\air_jump_n_smash 1 98
	TEnd

	Type		fem
	  Anim          fem\air_jump_n_smash 1 98
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_jump_n_smash 1 98
	TEnd
MEnd

Move A_jumpNsmash_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_jumpNsmash_cycle_15
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_jump_n_smash 98
	 Scale		0.50
	TEnd

	Type		huge
	  Anim          huge\air_jump_n_smash 98
	  Scale		0.50
	TEnd

	Type		fem
	  Anim          fem\air_jump_n_smash 98
	  Scale		0.50
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_jump_n_smash 98
	TEnd
MEnd

#############################################################

Move A_jumpNsmash_cycle_15
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_jumpNsmash_cycle
	Scale           0.50
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_cycle 15 31
	TEnd

	Type		huge
	  Anim          huge\air_claw_cycle 15 31
	TEnd

	Type		fem
	  Anim          fem\air_claw_cycle 15 31
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_float 1
	TEnd
MEnd

#############################################################

Move A_jumpNsmash_cycle
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	Flags		Cycle NoSizeScale
	NextMove	C_PreFall
	Scale           0.50

	Type		male
	  Anim          male\air_claw_cycle 1 31
	TEnd

	Type		huge
	  Anim          huge\air_claw_cycle 1 31
	TEnd

	Type		fem
	  Anim          fem\air_claw_cycle 1 31
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_float 1
	TEnd
MEnd


###################
###### air Baseball punch ######
###################

Move A_BBpunch
	Requires	FLY COMBAT BASEBALLPUNCH
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_BBpunchpost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_baseball_punch 1 44
	TEnd

	Type		huge
	  Anim          huge\air_baseball_punch 1 44
	TEnd

	Type		fem
	  Anim          fem\air_baseball_punch 1 44
	TEnd
MEnd

Move A_BBpunchpost
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle_half
	Scale           0.50
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_baseball_punch 44
	TEnd

	Type		huge
	  Anim          huge\air_baseball_punch 44
	TEnd

	Type		fem
	  Anim          fem\air_baseball_punch 44
	TEnd
MEnd


###################
###### air Windup Uppercut ######
###################

Move A_WindUpcut
	Requires	FLY COMBAT WINDUPPERCUT
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_WindUpcut_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_windup_upcut 1 65
	TEnd

	Type		huge
	  Anim          huge\air_windup_upcut 1 65
	TEnd

	Type		fem
	  Anim          fem\air_windup_upcut 1 65
	TEnd
MEnd

Move A_WindUpcut_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle_half
	Scale           0.50
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_windup_upcut 65
	TEnd

	Type		huge
	  Anim          huge\air_windup_upcut 65
	TEnd

	Type		fem
	  Anim          fem\air_windup_upcut 65
	TEnd
MEnd

###################
###### air Stop cast ######
###################

Move A_Stop
	Requires	FLY COMBAT STOPCAST
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Stop_cycle
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_stop 1 31
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\air_stop 1 31
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\air_stop 1 31
	  PitchAngle	0.5
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_stopcast 1 31
	  PitchAngle	0.5
	TEnd
MEnd

Move A_Stop_cycle
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_Stop_post
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		male
	  Anim          male\air_stop 31 60
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\air_stop 31 60
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\air_stop 31 60
	  PitchAngle	0.5
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_stopcast 31 60
	  PitchAngle	0.5
	TEnd
MEnd

Move A_Stop_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle_half
	Scale           0.50
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_stop 60
	  PitchEnd	64
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\air_stop 60
	  PitchEnd	64
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\air_stop 60
	  PitchEnd	64
	  PitchAngle	0.5
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_stopcast 60
	  PitchEnd	74
	  PitchAngle	0.5
	TEnd
MEnd


###################
###### Air Scream ######
###################

Move A_Scream
	Requires	FLY COMBAT SCREAM
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Scream_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_scream 10 58
	  PlayFX	feet\MaleTaunt2.fx
	TEnd

	Type		huge
	  Anim          huge\air_scream 10 58
	  PlayFX	feet\MaleTaunt7.fx
	TEnd

	Type		fem
	  Anim          fem\air_scream 10 58
	  PlayFX	feet\FemTaunt2.fx
	TEnd
MEnd

Move A_Scream_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_scream 58
	TEnd

	Type		huge
	  Anim          huge\air_scream 58
	TEnd

	Type		fem
	  Anim          fem\air_scream 58
	TEnd
MEnd


###################
###### air Terrify ######
###################

Move A_Terrify
	Requires	FLY COMBAT TERRIFY
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Terrify_post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_terrify 1 60
	TEnd

	Type		huge
	  Anim          huge\air_terrify 1 60
	TEnd

	Type		fem
	  Anim          fem\air_terrify 1 60
	TEnd
MEnd

Move A_Terrify_post
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove	Fake_C_flycycle_Half
	Scale           0.50
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		male
	  Anim          male\air_terrify 60
	  PitchEnd	63
	TEnd

	Type		huge
	  Anim          huge\air_terrify 60
	  PitchEnd	63
	TEnd

	Type		fem
	  Anim          fem\air_terrify 60
	  PitchEnd	63
	TEnd
MEnd


###################
###### air Energy Torrent ######
###################

Move A_Energy_Torrent
	Requires	FLY COMBAT ENERGYTORRENT
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_E_Torrent_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_energy_torrent 1 65
	TEnd

	Type		huge
	  Anim          huge\air_energy_torrent 1 65
	TEnd

	Type		fem
	  Anim          fem\air_energy_torrent 1 65
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_energytorrent 1 65
	TEnd
MEnd

Move A_E_Torrent_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_energy_torrent 65
	  Scale		0.50
	TEnd

	Type		huge
	  Anim          huge\air_energy_torrent 65
	  Scale		0.50
	TEnd

	Type		fem
	  Anim          fem\air_energy_torrent 65
	  Scale		0.50
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_energytorrent 65
	  Scale		1.1
	TEnd
MEnd


###################
###### air hypnotize ######
###################

Move A_Hypnotize
	Requires	FLY COMBAT HYPNOTIZE
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Hypnotize2
	Scale           1.50
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_hypnotize 1 90
	TEnd

	Type		huge
	  Anim          huge\air_hypnotize 1 90
	TEnd

	Type		fem
	  Anim          fem\air_hypnotize 1 90
	TEnd
MEnd

Move A_Hypnotize2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_Hypnotize_post
	Scale           1.50
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		male
	  Anim          male\air_hypnotize 90 175
	  PitchEnd	147
	TEnd

	Type		huge
	  Anim          huge\air_hypnotize 90 175
	  PitchEnd	147
	TEnd

	Type		fem
	  Anim          fem\air_hypnotize 90 175
	  PitchEnd	147
	TEnd
MEnd

Move A_Hypnotize_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	Scale           0.50
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_hypnotize 175
	TEnd

	Type		huge
	  Anim          huge\air_hypnotize 175
	TEnd

	Type		fem
	  Anim          fem\air_hypnotize 175
	TEnd
MEnd


###########################
###### Air Crystal throw  ######
##########################

Move A_Crystalthrow
	Requires	FLY COMBAT CAST TELEATTACK
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Crystalthrow2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\mystic_crystal_throw 1 60
	TEnd
MEnd

Move A_Crystalthrow2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flyready
	Flags		ReqInputs

	Type		male
	  Anim          male\mystic_crystal_throw 60
	TEnd
MEnd


##############################
###### Air Predictable Teleport  ######
##############################

Move A_Predict_Teleport
	Requires	FLY SUPERTELEPORT
	Priority	12
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Predict_Teleport2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_fireaura 1 55
	TEnd

	Type		huge
	  Anim          huge\air_fireaura 1 55
	TEnd

	Type		fem
	  Anim          fem\air_fireaura 1 55
	TEnd
MEnd

Move A_Predict_Teleport2
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove	FlyReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_fireaura 55
	TEnd

	Type		huge
	  Anim          huge\air_fireaura 55
	TEnd

	Type		fem
	  Anim          fem\air_fireaura 55
	TEnd
MEnd


###################
###### Air Long_QUICK Boost Aura (built-in pause for TELEPORT)  ######
###################

Move A_BoostaurA_long
	Requires	FLY QUICK STRONG BOOSTAURA
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_BoostaurA_long2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_fireaura 1 54
	TEnd

	Type		huge
	  Anim          huge\air_fireaura 1 54
	TEnd

	Type		fem
	  Anim          fem\air_fireaura 1 54
	TEnd
MEnd

Move A_BoostaurA_long2
	Requires	FLY COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_BoostaurA_long3
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_fireaurA_pose 1 30
	TEnd

	Type		huge
	  Anim          huge\air_fireaurA_pose 1 30
	TEnd

	Type		fem
	  Anim          fem\air_fireaurA_pose 1 30
	TEnd
MEnd

Move A_BoostaurA_long3
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flyready
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_fireaura 55
	TEnd

	Type		huge
	  Anim          huge\air_fireaura 55
	TEnd

	Type		fem
	  Anim          fem\air_fireaura 55
	TEnd
MEnd


###################
###### Air WindUp Magic  ######
###################

Move A_Magic_WindUp_Pre
	Requires	FLY COMBAT MAGICWINDUP
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ>
	NextMove	A_Magic_WindUp_Cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_ground_summon 1 65
	TEnd

	Type		huge
	  Anim          huge\air_ground_summon 1 65
	TEnd

	Type		fem
	  Anim          fem\air_ground_summon 1 65
	TEnd
MEnd

Move A_Magic_WindUp_Cycle
	Requires	FLY COMBAT MAGICWINDUP
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ>
	Flags		Cycle NoSizeScale
	NextMove	A_Magic_WindUp_post

	Type		male
	  Anim          male\air_ground_summon 65 104
	TEnd

	Type		huge
	  Anim          huge\air_ground_summon 65 104
	TEnd

	Type		fem
	  Anim          fem\air_ground_summon 65 104
	TEnd
MEnd

Move A_Magic_WindUp_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle_half
	Scale           0.50
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_ground_summon 104
	TEnd

	Type		huge
	  Anim          huge\air_ground_summon 104
	TEnd

	Type		fem
	  Anim          fem\air_ground_summon 104
	TEnd
MEnd


###################
###### Air EyeCast Windup  ######
###################

Move A_Pre_EyeWindup
	Requires	FLY COMBAT POWERWINDUP
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ>
	NextMove	A_EyeWindup
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_mouthcast 1 40
	  PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\air_mouth 1 40
	  PitchAngle	0.1
	TEnd

	Type		fem
	  Anim          fem\air_mouth 1 40
	  PitchAngle	0.1
	TEnd
MEnd

Move A_EyeWindup
	Requires	FLY COMBAT POWERWINDUP
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ>
	Flags		Cycle PitchToTarget NoSizeScale
	NextMove	EyeWindup_post

	Type		male
	  Anim          male\air_mouthcast 40 46
	  PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\air_mouth 40 46
	  PitchAngle	0.1
	TEnd

	Type		fem
	  Anim          fem\air_mouth 40 46
	PitchAngle	0.1
	TEnd

MEnd

Move A_EyeWindup_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle_half
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_mouthcast 46
	  PitchEnd	50
	  PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\air_mouth 46
	  PitchEnd	50
	  PitchAngle	0.1
	TEnd

	Type		fem
	  Anim          fem\air_mouth 46
	  PitchEnd	50
	  PitchAngle	0.1
	TEnd
MEnd


###################
###### Air Telekinesis + Magic Cast  ######
###################

Move A_Tele_Magic
	Requires	FLY COMBAT MAGIC TELEKINESIS
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Tele_Magic2
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_telekinesis 1 75
	  PitchStart	10
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\air_telekinesis 1 75
	  PitchStart	10
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\air_telekinesis 1 75
	  PitchStart	10
	  PitchAngle	0.5
	TEnd
MEnd

Move A_Tele_Magic2
	Requires	FLY
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Tele_Magic_post
	Interpolate	8
	Sets            CANTMOVE
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		male
	  Anim          male\air_magicL 5 30
	   PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\air_magicL 5 30
	   PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\air_magicL 5 30
	   PitchAngle	0.5
	TEnd
MEnd

Move A_Tele_Magic_post
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flyready
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		male
	  Anim          male\air_magicL 30 66
	  PitchEnd	60
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\air_magicL 30 66
	  PitchEnd	60
	  PitchAngle	0.5
	TEnd

	Type		fem
	  Anim          fem\air_magicL 30 66
	  PitchEnd	60
	  PitchAngle	0.5
	TEnd
MEnd


###################
###### Air Levitate + Forceram  ######
###################

Move A_Lev_Ram
	Requires	FLY COMBAT STRONG LEVITATE
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Lev_Ram2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_levitate 1 98
	TEnd

	Type		huge
	  Anim          huge\air_levitate 1 98
	TEnd

	Type		fem
	  Anim          fem\air_levitate 1 98
	TEnd
MEnd

Move A_Lev_Ram2
	Requires	FLY
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Lev_Ram2_post
	Interpolate	8
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_forceram 5 30
	TEnd

	Type		huge
	  Anim          huge\air_forceram 5 30
	TEnd

	Type		fem
	  Anim          fem\air_forceram 5 30
	TEnd
MEnd

Move A_Lev_Ram2_post
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flyready
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_forceram 30 57
	TEnd

	Type		huge
	  Anim          huge\air_forceram 30 57
	TEnd

	Type		fem
	  Anim          fem\air_forceram 30 57
	TEnd
MEnd


###################
###### Air Med Other Windup (tricorder) ######
###################

Move A_MedOther_Pre
	Requires	FLY MEDOTHER WINDUP HIGH
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ>
	NextMove	A_MedOther_Cycle
	Flags		NoSizeScale
	Scale		1.5

	Type		male
	  Anim          male\air_med_other 1 88
	TEnd

	Type		huge
	  Anim          huge\air_med_other 1 88
	TEnd

	Type		fem
	  Anim          fem\air_med_other 1 88
	TEnd
MEnd

Move A_MedOther_Cycle
	Requires	FLY MEDOTHER WINDUP
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ>
	Flags		Cycle NoSizeScale
	NextMove	A_MedOther_post

	Type		male
	  Anim          male\air_med_other 88 138
	TEnd

	Type		huge
	  Anim          huge\air_med_other 88 138
	TEnd

	Type		fem
	  Anim          fem\air_med_other 88 138
	TEnd
MEnd

Move A_MedOther_post
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove	flyready
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_med_other 138
	TEnd

	Type		huge
	  Anim          huge\air_med_other 138
	TEnd

	Type		fem
	  Anim          fem\air_med_other 138
	TEnd
MEnd


###################
###### Air Med Self Windup (tricorder) ######
###################

Move A_Pre_MedSelf
	Requires	FLY MEDSELF
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ>
	NextMove	A_MedSelf
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_med_self 1 2
	TEnd

	Type		huge
	  Anim          huge\air_med_self 1 2
	TEnd

	Type		fem
	  Anim          fem\air_med_self 1 2
	TEnd
MEnd

Move A_MedSelf
	Requires	FLY MEDSELF
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ>
	Flags		Cycle NoSizeScale
	NextMove	A_MedSelf2

	Type		male
	  Anim          male\air_med_self 3 105
	TEnd

	Type		huge
	  Anim          huge\air_med_self 3 105
	TEnd

	Type		fem
	  Anim          fem\air_med_self 3 105
	TEnd
MEnd

Move A_MedSelf2
	Requires	FLY
	Include sequencers\Members\Readies_Air.inc
	Flags		NoSizeScale ReqInputs
	NextMove	flyready

	Type		male
	  Anim          male\air_med_self 105
	TEnd

	Type		huge
	  Anim          huge\air_med_self 105
	TEnd

	Type		fem
	  Anim          fem\air_med_self 105
	TEnd
MEnd

## AID SELF (INCLUDES FX) ###########################################################

Move A_AidSelf_Pre
	Requires	FLY MEDSELF TWO
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ>
	NextMove	A_AidSelf
	Flags		NoSizeScale
	Scale		1.5

	Type		male
	  Anim          male\air_med_self 1 2
	TEnd

	Type		huge
	  Anim          huge\air_med_self 1 2
	TEnd

	Type		fem
	  Anim          fem\air_med_self 1 2
	TEnd
MEnd

Move A_AidSelf
	Requires	FLY
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ>
	Flags		NoSizeScale
	NextMove	A_AidSelf_Post
	Scale		1.5

	Type		male
	  Anim          male\air_med_self 2 117
	  PlayFX	POWERS\Medicine\MSelf.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\air_med_self 2 117
	  PlayFX	POWERS\Medicine\MSelf.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\air_med_self 2 117
	  PlayFX	POWERS\Medicine\MSelf.fx 0 CONSTANT
	TEnd
MEnd

Move A_AidSelf_Post
	Requires	FLY
	Include sequencers\Members\Readies_Air.inc
	Flags		NoSizeScale ReqInputs
	NextMove	flyready

	Type		male
	  Anim          male\air_med_self 117
	  PlayFX	POWERS\Medicine\MSelf.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\air_med_self 117
	  PlayFX	POWERS\Medicine\MSelf.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\air_med_self 117
	  PlayFX	POWERS\Medicine\MSelf.fx 0 CONSTANT
	TEnd
MEnd

###################
###### Air QUICK_Med Self Windup (tricorder) ######
###################

Move A_QUICK_MedSelf
	Requires	FLY MEDSELF QUICK
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_QUICK_MedSelf_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_med_self 1 165
	TEnd

	Type		huge
	  Anim          huge\air_med_self 1 165
	TEnd

	Type		fem
	  Anim          fem\air_med_self 1 165
	TEnd
MEnd

Move A_QUICK_MedSelf_post
	Requires	FLY
	Include sequencers\Members\Readies_Air.inc
	NextMove	flyready
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_med_self 165
	TEnd

	Type		huge
	  Anim          huge\air_med_self 165
	TEnd

	Type		fem
	  Anim          fem\air_med_self 165
	TEnd
MEnd


###################
###### Air Med dead Windup (tricorder) ######
###################

Move A_Revive_Windup
	Requires	FLY MEDDEAD WINDUP LOW
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <HitIRQ> <ReactIRQ> <MoveIRQ> <Windups>
	NextMove	A_Revive_WindupCycle
	Flags		NoSizeScale FixWeaponReg
	Scale		1.5

	Type		male
	  Anim          male\air_med_dead 1 99
	TEnd

	Type		huge
	  Anim          huge\air_med_dead 1 99
	TEnd

	Type		fem
	  Anim          fem\air_med_dead 1 99
	TEnd
MEnd

Move A_Revive_WindupCycle
	Requires	FLY MEDDEAD WINDUP
	Member		<DeathIRQ> <HitIRQ> <ReactIRQ> <MoveIRQ> <Windups>
	Flags		Cycle NoSizeScale FixWeaponReg
	NextMove	A_Revive

	Type		male
	  Anim          male\air_med_dead 100 200
	TEnd

	Type		huge
	  Anim          huge\air_med_dead 100 200
	TEnd

	Type		fem
	  Anim          fem\air_med_dead 100 200
	TEnd
MEnd

#############################################################

Move A_Revive
	Requires	FLY MEDDEAD GETUP
	Priority	13
	Interrupts	<Windups> A_Revive_Windup A_Revive_WindupCycle
	Include sequencers\Members\Readies_Air.inc
	NextMove	flyready
	Flags		NoSizeScale FixWeaponReg

	Type		male
	  Anim          male\air_med_dead 150
	TEnd

	Type		huge
	  Anim          huge\air_med_dead 150
	TEnd

	Type		fem
	  Anim          fem\air_med_dead 150
	TEnd
MEnd


#############################################################################
###### Air Med Beam QUICK (no windup note: max file is NOINFLUENCE_MED_BEAM)  ######
#############################################################################

Move A_Med_Beam
	Requires	FLY TRICORDERBEAM
	Priority	12
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Med_Beam_post
	Flags		CANTMOVE PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_med_beam 1 90
	  PitchStart	19
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\air_med_beam 1 90
	  PitchStart	19
	  PitchAngle	0.2
	TEnd

	Type		fem
	  Anim          fem\air_med_beam 1 90
	  PitchStart	19
	  PitchAngle	0.2
	TEnd

MEnd

Move A_Med_Beam_post
	Requires	FLY
	Include sequencers\Members\Readies_Air.inc
	NextMove	flyready
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		male
	  Anim          male\air_med_beam 90
	  PitchEnd	100
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\air_med_beam 90
	  PitchEnd	100
	  PitchAngle	0.2
	TEnd

	Type		fem
	  Anim          fem\air_med_beam 90
	  PitchEnd	100
	  PitchAngle	0.2
	TEnd
MEnd


###################
###### Air Doorsmash Kick ######
###################

Move A_DoorKick
	Requires	FLY COMBAT STRONG KICK
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_DoorKick_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_doorsmash_kick 1 55
	TEnd

	Type		huge
	  Anim          huge\air_doorsmash_kick 1 55
	TEnd

	Type		fem
	  Anim          fem\air_doorsmash_kick 1 55
	TEnd
MEnd

Move A_DoorKick_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_doorsmash_kick 55
	TEnd

	Type		huge
	  Anim          huge\air_doorsmash_kick 55
	TEnd

	Type		fem
	  Anim          fem\air_doorsmash_kick 55
	TEnd
MEnd


###################
###### Air Weave ######
###################

Move A_Weave_Pre
	Requires	FLY PREMODE BRAWL
	Priority	13
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_Weave
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_weave 86 88
	TEnd

	Type		huge
	  Anim          huge\air_weave 86 88
	TEnd

	Type		fem
	  Anim          fem\air_weave 86 88
	TEnd
MEnd

Move A_Weave
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_Weave_Cycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_weave 88 142
	TEnd

	Type		huge
	  Anim          huge\air_weave 88 142
	TEnd

	Type		fem
	  Anim          fem\air_weave 88 142
	TEnd
MEnd

Move A_Weave_Cycle
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\air_weave 35 142
	TEnd

	Type		huge
	  Anim          huge\air_weave 35 142
	TEnd

	Type		fem
	  Anim          fem\air_weave 35 142
	TEnd
MEnd



###################
###### Air Flip boost ######
###################

Move A_FlipBoost
	Requires	FLY FIERCE SPIN
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_FlipBoost2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_claw_jump_area 1 61
	TEnd

	Type		huge
	  Anim          huge\air_claw_jump_area 1 61
	TEnd

	Type		fem
	  Anim          fem\air_claw_jump_area 1 61
	TEnd
MEnd

Move A_FlipBoost2
	Requires	FLY
	Include sequencers\Members\Readies_Air.inc
	NextMove	Flyready
	Interpolate	8
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\flyup 113
	TEnd

	Type		huge
	  Anim          huge\flyup 113
	TEnd

	Type		fem
	  Anim          fem\flyup 113
	TEnd
MEnd


###################
###### Air Superpunch ######
###################

Move A_Superpunch_R
	Requires	FLY COMBAT QUICK PUNCH HAND
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Superpunch_R2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_superpunch 1 18
	TEnd

	Type		huge
	  Anim          huge\air_superpunch 1 18
	TEnd

	Type		fem
	  Anim          fem\air_superpunch 1 18
	TEnd
MEnd

Move A_Superpunch_R2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_Spunchcycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_superpunch 18 39
	TEnd

	Type		huge
	  Anim          huge\air_superpunch 18 39
	TEnd

	Type		fem
	  Anim          fem\air_superpunch 18 39
	TEnd
MEnd

#############################################################

Move A_Superpunch_L
	Requires	FLY COMBAT QUICK PUNCH HAND
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Superpunch_L2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_superpunch 40 58
	TEnd

	Type		huge
	  Anim          huge\air_superpunch 40 58
	TEnd

	Type		fem
	  Anim          fem\air_superpunch 40 58
	TEnd
MEnd

Move A_Superpunch_L2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_Spunchcycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_superpunch 58 79
	TEnd

	Type		huge
	  Anim          huge\air_superpunch 58 79
	TEnd

	Type		fem
	  Anim          fem\air_superpunch 58 79
	TEnd
MEnd

#############################################################

Move A_Spunchcycle
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	Flags		Cycle NoSizeScale
	NextMove	C_PreFall

	Type		male
	  Anim          male\air_brawl_cycle 1 40
	TEnd

	Type		huge
	  Anim          huge\air_brawlcycle 1 40
	TEnd

	Type		fem
	  Anim          fem\air_brawlcycle 1 40
	TEnd
MEnd

###################
###### Air Smash Cast Radius  ######
###################

Move A_1HSmash_Right
	Requires	FLY COMBAT PUNCH SMASH
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_1HSmash_Right2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_1H_smash 12 36
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_1hsmash 1 45
	TEnd
MEnd

Move A_1HSmash_Right2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_1HSmash_Right_Post
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_1H_smash 36 59
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_1hsmash 45 48
	TEnd
MEnd

Move A_1HSmash_Right_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle_half
	Flags		NoSizeScale ReqInputs
	Scale		0.5

	Type		male
	  Anim          male\air_1H_smash 59 82
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_1hsmash 48
	TEnd
MEnd

#############################################################

Move A_1HSmash_Left
	Requires	FLY COMBAT PUNCH SMASH
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_1HSmash_Left2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_1H_smash 96 119
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_1hsmash 1 45
	TEnd
MEnd

Move A_1HSmash_Left2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_1HSmash_Left_Post
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_1H_smash 119 141
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_1hsmash 46 48
	TEnd
MEnd

Move A_1HSmash_Left_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle_half
	Flags		NoSizeScale
	Scale		0.5

	Type		male
	  Anim          male\air_1H_smash 141
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_1hsmash 48
	TEnd
MEnd

###################
###### Air MA kick ######
###################

Move A_MA_kick
	Requires	FLY COMBAT KUNGFU KICK
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_MA_kick2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_MA_kick 1 25
	TEnd

	Type		huge
	  Anim          huge\air_MA_kick 1 25
	TEnd

	Type		fem
	  Anim          fem\air_MA_kick 1 25
	TEnd
MEnd

Move A_MA_kick2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_MA_kick 25
	TEnd

	Type		huge
	  Anim          huge\air_MA_kick 25
	TEnd

	Type		fem
	  Anim          fem\air_MA_kick 25
	TEnd
MEnd

###################
###### Air MA Shadow kick ######
###################

Move A_MA_Shadowkick
	Requires	FLY COMBAT KUNGFU KUNGFUSPEED
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_MA_Shadowkick2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_MA_shadowkick 1 125
	TEnd

	Type		huge
	  Anim          huge\air_MA_shadowkick 1 125
	TEnd

	Type		fem
	  Anim          fem\air_MA_shadowkick 1 125
	TEnd
MEnd

Move A_MA_Shadowkick2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_MA_shadowkick 125
	TEnd

	Type		huge
	  Anim          huge\air_MA_shadowkick 125
	TEnd

	Type		fem
	  Anim          fem\air_MA_shadowkick 125
	TEnd
MEnd

###############################
###### Air MA roundhouse kick ######
##############################

Move A_MA_Roundhouse
	Requires	FLY COMBAT KUNGFU KICK Strong
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_MA_Roundhouse2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_spinkick_mid 1 25
	TEnd
MEnd

Move A_MA_Roundhouse2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_roundhouse_cycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_spinkick_mid 25
	TEnd
MEnd

#############################################################

Move A_roundhouse_cycle
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	Flags		Cycle NoSizeScale
	NextMove	C_PreFall

	Type		male
	  Anim          male\air_spinkick_cycle 1
	TEnd
MEnd


###################
###### Air MA Palm stun ######
###################

Move A_MA_Palmstun
	Requires	FLY COMBAT KUNGFU PUNCH KUNGFUSTUN
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_MA_Palmstun2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_MA_palmstun 35 84
	TEnd

	Type		huge
	  Anim          huge\air_MA_palmstun 35 84
	TEnd

	Type		fem
	  Anim          fem\air_MA_palmstun 35 84
	TEnd
MEnd

Move A_MA_Palmstun2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_MA_palmstun 84
	TEnd

	Type		huge
	  Anim          huge\air_MA_palmstun 84
	TEnd

	Type		fem
	  Anim          fem\air_MA_palmstun 84
	TEnd
MEnd


###################
###### Air MA Sweep kick ######
###################

Move A_MA_Sweepkick
	Requires	FLY COMBAT KUNGFU KUNGFUAREA ATTACK
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_MA_Sweepkick_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_MA_sweepkick 40 85
	TEnd

	Type		huge
	  Anim          huge\air_MA_sweepkick 40 85
	TEnd

	Type		fem
	  Anim          fem\air_MA_sweepkick 40 85
	TEnd
MEnd

Move A_MA_Sweepkick_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_MA_sweepkick 85
	TEnd

	Type		huge
	  Anim          huge\air_MA_sweepkick 85
	TEnd

	Type		fem
	  Anim          fem\air_MA_sweepkick 85
	TEnd
MEnd


###################
###### Air MA Hobble kick ######
###################

Move A_MA_Hobblekick
	Requires	FLY COMBAT KUNGFU LOW KICK FEET
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_MA_Hobblekick2
	Sets            CANTMOVE
	Flags		NoSizeScale
	Scale		1.25

	Type		male
	  Anim          male\air_MA_hobble 1 60
	TEnd

	Type		huge
	  Anim          huge\air_MA_hobble 1 60
	TEnd

	Type		fem
	  Anim          fem\air_MA_hobble 1 60
	TEnd
MEnd

Move A_MA_Hobblekick2
	Requires	FLY COMBAT
	Priority	12
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle_half
	Flags		NoSizeScale ReqInputs
	Scale		1.125

	Type		male
	  Anim          male\air_MA_hobble 60
	TEnd

	Type		huge
	  Anim          huge\air_MA_hobble 60
	TEnd

	Type		fem
	  Anim          fem\air_MA_hobble 60
	TEnd
MEnd


###################
###### Air MA Stun kick ######
###################

Move A_MA_Stunkick
	Requires	FLY COMBAT KUNGFU KICK KUNGFUSTUN
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_MA_Stunkick2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_MA_stunkick 25 100
	TEnd

	Type		huge
	  Anim          huge\air_MA_stunkick 25 100
	TEnd

	Type		fem
	  Anim          fem\air_MA_stunkick 25 100
	TEnd
MEnd

Move A_MA_Stunkick2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_MA_stunkick 100
	TEnd

	Type		huge
	  Anim          huge\air_MA_stunkick 100
	TEnd

	Type		fem
	  Anim          fem\air_MA_stunkick 100
	TEnd
MEnd


###################
###### Air MA Iron kick ######
###################

Move A_MA_Ironkick
	Requires	FLY COMBAT KUNGFU HIGH KICK
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_MA_Ironkick2
	Sets            CANTMOVE
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_MA_ironkick 34 84
	TEnd

	Type		huge
	  Anim          huge\air_MA_ironkick 34 84
	TEnd

	Type		fem
	  Anim          fem\air_MA_ironkick 34 84
	TEnd
MEnd

Move A_MA_Ironkick2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle_half
	Wind		ATTACK
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_MA_ironkick 84
	TEnd

	Type		huge
	  Anim          huge\air_MA_ironkick 84
	TEnd

	Type		fem
	  Anim          fem\air_MA_ironkick 84
	TEnd
MEnd


###############################
###### Air Mind Probe QUICK  ######
###############################

Move A_MindProbe_QUICK
	Requires	FLY COMBAT Psionic Punch
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_MindProbe_QUICK2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_mindprobe 1 30
	TEnd
MEnd

Move A_MindProbe_QUICK2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	Lazy_C_flycycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_mindprobe 30
	TEnd
MEnd


###############################
###### Air Smoke Flash  ######
###############################

Move A_SmokeFlash
	Requires	FLY COMBAT CAST HIDE ATTACK KUNGFU
	Priority	18
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_SmokeFlash2
	Interpolate	8
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_smokeflash 1 51
	TEnd
MEnd

Move A_SmokeFlash2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	Lazy_C_flycycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_smokeflash 51
	TEnd
MEnd


#############################
###### Air Kuji in Sha ######
#############################

Move A_Kuji_Sha
	Requires	FLY BOOSTUP ATTACK KUNGFU HIGH
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ> <ReactIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Kuji_Sha2
	Wind		FLY
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_kuji_zen 1 2
	TEnd

	Type		fem
	  Anim          fem\air_kuji_zen 1 2
	TEnd

	Type		huge
	  Anim          huge\air_kuji_zen 1 2
	TEnd
MEnd

Move A_Kuji_Sha2
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove	Lazy_C_flycycle
	Wind		FLY
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_kuji_zen 3
	TEnd

	Type		fem
	  Anim          fem\air_kuji_zen 3
	TEnd

	Type		huge
	  Anim          huge\air_kuji_zen 3
	TEnd
MEnd


#############################
###### Air Kuji in Rin ######
#############################

Move A_Kuji_Rin
	Requires	FLY KUJIRIN
	Priority	12
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_Kuji_Rin2
	Wind		FLY
	Scale		0.7
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_kuji_rin 1 2
	TEnd

	Type		fem
	  Anim          fem\air_kuji_rin 1 2
	TEnd

	Type		huge
	  Anim          huge\air_kuji_rin 1 2
	TEnd
MEnd

Move A_Kuji_Rin2
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove	Lazy_C_flycycle
	Wind		FLY
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_kuji_rin 3
	TEnd

	Type		fem
	  Anim          fem\air_kuji_rin 3
	TEnd

	Type		huge
	  Anim          huge\air_kuji_rin 3
	TEnd
MEnd


###############################
###### Air Kuji in Zen  ######
###############################

Move A_Kuji_Zen
	Requires	FLY KUJIZEN
	Priority	12
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Kuji_Zen2
	Sets            CANTMOVE
	Wind		FLY
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_kuji_sha 1 50
	TEnd

	Type		fem
	  Anim          fem\air_kuji_sha 1 50
	TEnd

	Type		huge
	  Anim          huge\air_kuji_sha 1 50
	TEnd
MEnd

Move A_Kuji_Zen2
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove	Lazy_C_flycycle
	Wind		FLY
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_kuji_sha 50
	TEnd

	Type		fem
	  Anim          fem\air_kuji_sha 50
	TEnd

	Type		huge
	  Anim          huge\air_kuji_sha 50
	TEnd
MEnd


######################
## Air Danger Sense ##
######################

Move A_DangerSense
	Requires	FLY KUNGFUAREA KUNGFU LOOKOUT #ATTACK
	Priority	14
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_DangerSense2
	Wind		FLY
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_lookQUICK 1 2
	TEnd
MEnd

Move A_DangerSense2
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	CycleMove	A_Danger_Sense_cycle
	Flags		NoSizeScale Cycle
	NextMove	PreFall

	Type		male
	  Anim          male\air_lookQUICK 2
	TEnd
MEnd

Move A_Danger_Sense_cycle
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	Flags		NoSizeScale Cycle
	NextMove	PreFall

	Type		male
	  Anim          male\air_lookQUICK 1
	TEnd
MEnd


##################################
###### Air Mart Backstab Windup  ###
##################################

Move A_MartWindup
	Requires	FLY COMBAT WINDUPMARTBACKSTAB
	Priority	13
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups_Air.inc
	NextMove	A_MartWindup_Cycle
	Flags		NoSizeScale PitchToTarget

	Type		male
	  Anim          male\air_mart_backstab 1 65
	TEnd
MEnd

Move A_MartWindup_Cycle
	Requires	FLY COMBAT WINDUPMARTBACKSTAB
	Priority	13
	Include sequencers\Members\Windups_Air.inc
	Flags		Cycle NoSizeScale PitchToTarget

	Type		male
	  Anim          male\air_mart_backstab 65 115
	TEnd
MEnd


#########################
##### Air Mart Backstab ####
#########################

Move A_MartBackstab
	Requires	FLY COMBAT MARTBACKSTAB
	Priority	13
	Interrupts	<AttackIRQ> <Windups>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_MartBackstab2
	Sets            CANTMOVE
	Flags		NoSizeScale PitchToTarget

	Type		male
	  Anim          male\air_mart_backstab 120 145
	TEnd
MEnd

Move A_MartBackstab2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	Lazy_C_flycycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_mart_backstab 145
	TEnd
MEnd


##################
##### Air Placate ####
##################

Move A_Placate
	Requires	FLY COMBAT PLACATE
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Placate2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_placate 1 40
	TEnd
MEnd

Move A_Placate2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	Lazy_C_flycycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_placate 40
	TEnd
MEnd

####################################################################
## Place Trap
####################################################################

Move A_Place_Trap
	Requires	FLY PLACETRAP
	Priority	12
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Place_Trap2
	Flags		CANTMOVE NoSizeScale
	Scale		0.75

	Type		male
	  Anim          male\Loot 1 19
	TEnd
MEnd

Move A_Place_Trap2
	Requires	FLY
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Place_Trap_post
	Flags		CANTMOVE NoSizeScale
	Scale		0.75

	Type		male
	  Anim          male\examine_hero 1
	TEnd
MEnd

Move A_Place_Trap_post
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove	Place_Trap_post_cycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\Loot 24 45
	TEnd
MEnd


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

	Type		male
	  Anim          male\air_knifethrow 1 21
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\air_knifethrow 1 21
	TEnd

	Type		fem
	  Anim          fem\air_knifethrow 1 21
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_crystal_throw 1 59
	  PitchStart	27
	TEnd
MEnd

Move A_ThrowKnife
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_ThrowKnife_Post
	Flags		ReqInputs
	Scale		0.375

	Type		male
	  Anim          male\air_knifethrow 21 31
	TEnd

	Type		huge
	  Anim          huge\air_knifethrow 21 31
	TEnd

	Type		fem
	  Anim          fem\air_knifethrow 21 31
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_crystal_throw 59 61
	  PitchEnd	70
	TEnd
MEnd

Move A_ThrowKnife_Post
	Requires	FLY COMBAT
	Priority	12
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flyready
	Flags		ReqInputs
	Scale		0.5

	Type		male
	  Anim          male\air_knifethrow 31
	TEnd

	Type		huge
	  Anim          huge\air_knifethrow 31
	TEnd

	Type		fem
	  Anim          fem\air_knifethrow 31
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_crystal_throw 61
	  PitchEnd	70
	TEnd
MEnd

#############################################################

Move A_ThrowProjectile_Pre
	Requires	FLY COMBAT THROWKNIFE ROOTED
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_ThrowProjectile
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\air_knifethrow 1 21
	  PitchAngle	0.5
	TEnd
MEnd

Move A_ThrowProjectile
	Requires	FLY COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_ThrowProjectile_Post
	Scale		0.375
	Sets		CANTMOVE
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_knifethrow 21 31
	TEnd
MEnd

Move A_ThrowProjectile_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flyready
	Flags		ReqInputs
	Scale		0.5

	Type		male
	  Anim          male\air_knifethrow 31
	TEnd
MEnd

#############################################################
## QUICK Serum
#############################################################

Move A_Serum_QUICK
	Requires	FLY COMBAT BOOSTUP SERUM QUICK
	Priority	15
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_Serum_QUICK_Post
	Interpolate	15
 	Flags		NOSIZESCALE

	Type		male
	  Anim		male\Paramilitary_CatchBoost 12 130
	TEnd
MEnd

Move A_Serum_QUICK_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_Serum_QUICK_PostCycle
	Interpolate	15
 	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_comb_beatchest 1 46
	  PlayFx        FEET\MALETAUNTA.FX
	TEnd

	Type		huge
	  Anim          huge\air_comb_beatchest 1 46
	  PlayFx        FEET\HUGETAUNTA.FX
	TEnd

	Type		fem
	  Anim          fem\air_comb_beatchest 1 46
	  PlayFx        FEET\FEMALETAUNTA.FX
	TEnd
MEnd

Move A_Serum_QUICK_PostCycle
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
 	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_comb_beatchest 46 106
	TEnd

	Type		huge
	  Anim          huge\air_comb_beatchest 46 106
	TEnd

	Type		fem
	  Anim          fem\air_comb_beatchest 46 106
	TEnd
MEnd

## POWER ANALYZER ###########################################################

Move A_PowerAnalyze_Pre
	Requires	FLY WRISTBANDACTIVATE OBSERVE
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Sets            CANTMOVE
 	Flags		NOSIZESCALE
	NextMove	A_PowerAnalyze

	Type		male
	  Anim          male\air_wristband_activate 1 45
	  PlayFX	V_COV\Powers\ArachnosSoldier\Training_Surveillance.fx 0 CONSTANT
	TEnd

	Type		V_male
	  Anim          male\air_wristband_activate 1 45
	  PlayFX	V_COV\Powers\ArachnosSoldier\V_Training_Surveillance.fx 0 CONSTANT
	TEnd
MEnd

Move A_PowerAnalyze
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove	Ready
 	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_wristband_activate 45
	  PlayFX	V_COV\Powers\ArachnosSoldier\Training_Surveillance.fx 0 CONSTANT
	TEnd

	Type		V_male
	  Anim          male\air_wristband_activate 45
	  PlayFX	V_COV\Powers\ArachnosSoldier\V_Training_Surveillance.fx 0 CONSTANT
	TEnd
MEnd


## REVEAL MAP ###########################################################

Move A_RevealMap_Pre
	Requires	FLY WRISTBANDACTIVATE OBSERVE TWO
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
 	Flags		NOSIZESCALE
	Sets            CANTMOVE
	NextMove	A_RevealMap

	Type		male
	  Anim          male\air_wristband_activate 1 45
	  PlayFX	POWERS\ItemsAndTempPowers\Reveal\RevealMap.fx 0 CONSTANT
	TEnd

	Type		V_male
	  Anim          male\air_wristband_activate 1 45
	  PlayFX	POWERS\ItemsAndTempPowers\Reveal\RevealMap_Villain.fx 0 CONSTANT
	TEnd
MEnd

Move A_RevealMap
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
 	Flags		NOSIZESCALE ReqInputs
	NextMove	Ready

	Type		male
	  Anim          male\air_wristband_activate 45
	  PlayFX	POWERS\ItemsAndTempPowers\Reveal\RevealMap.fx 0 CONSTANT
	TEnd

	Type		V_male
	  Anim          male\air_wristband_activate 45
	  PlayFX	POWERS\ItemsAndTempPowers\Reveal\RevealMap_Villain.fx 0 CONSTANT
	TEnd
MEnd

## PLANT DEVICE ###########################################################

Move A_PlantDevice_Pre
	Requires	FLY LOOT QUICK
	Priority	13
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Air.inc
	Flags		NOSIZESCALE
	NextMove	A_PlantDevice_Cycle
	Scale           0.70

	Type		male
	  Anim          male\Loot 3 19
	TEnd
MEnd

Move A_PlantDevice_Cycle
	Requires	FLY LOOT QUICK
	Include sequencers\Members\Combat_Air.inc
	Flags		NOSIZESCALE Cycle
	NextMove	A_PlantDevice_Post
	Scale		0.5

	Type		male
	  Anim          male\examine_hero 1
	TEnd
MEnd

Move A_PlantDevice_Post
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
 	Flags		NOSIZESCALE ReqInputs
	NextMove	Ready

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

#######################################################
###### Going Rogue Alignment Powers  ######
#######################################################

###################
###### Air Call to Justice  ######
###################

Move A_AlignmentPower_Justice
	Requires	FLY ALIGNMENT MOMENTGLORY
	Priority	12
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_AlignmentPower_Justice2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_glory 1 45
	TEnd

	Type		huge
	  Anim          huge\air_glory 1 45
	TEnd

	Type		fem
	  Anim          fem\air_glory 1 45
	TEnd
MEnd

Move A_AlignmentPower_Justice2
	Requires	FLY ALIGNMENT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle_half
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_glory 45
	TEnd

	Type		huge
	  Anim          huge\air_glory 45
	TEnd

	Type		fem
	  Anim          fem\air_glory 45
	TEnd
MEnd

#####################################
## Air Frenzy NOROOT (beat chest) ##
#####################################

Move A_AlignmentPower_Frenzy
	Requires	FLY COMBAT ALIGNMENT TAUNTA NOTROOTED
	Priority        15
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat.inc
	NextMove        C_PreFall #A_Alignment_Frenzy_cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_comb_beatchest 1 91
	  PlayFx        FEET\MALETAUNTA.FX
	TEnd

	Type		huge
	  Anim          huge\air_comb_beatchest 1 91
	  PlayFx        FEET\HUGETAUNTA.FX
	TEnd

	Type		fem
	  Anim          fem\air_comb_beatchest 1 91
	  PlayFx        FEET\FEMALETAUNTA.FX
	TEnd
MEnd

Move A_AlignmentPower_Frenzy_cycle
	Requires	FLY COMBAT
	Include sequencers\Members\Combat.inc
	Flags		Cycle
	NextMove	C_PreFall
	Priority        2

	Type		male
	  Anim          male\air_comb_beatchest 95 144
	TEnd

	Type		huge
	  Anim          huge\air_comb_beatchest 95 144
	TEnd

	Type		fem
	  Anim          fem\air_comb_beatchest 95 144
	TEnd
MEnd

###################
###### Alignment Air Fear Incarnate  ######
###################

Move A_AlignmentPower_Fear_pre
	Requires	FLY COMBAT ALIGNMENT MOUTHSFX
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_AlignmentPower_Fear_post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_mouthcast 1 49
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\air_mouth 1 49
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		fem
	  Anim          fem\air_mouth 1 49
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		Wisp
	  Anim          Wisp\headcast 1 90
	TEnd
MEnd

Move A_AlignmentPower_Fear_post
	Requires	FLY COMBAT ALIGNMENT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flyready
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_mouthcast 50
	TEnd

	Type		huge
	  Anim          huge\air_mouth 50
	TEnd

	Type		fem
	  Anim          fem\air_mouth 50
	TEnd

	Type		Wisp
	  Anim          Wisp\headcast 90
	TEnd
MEnd

#################################
## Alignment Air Duplicity - No Root ##
#################################

Move A_AlignmentPower_Duplicity_Pre_NotRoot
	Requires	FLY COMBAT ALIGNMENT RADIUSATTACK NOTROOTED
	Priority        15
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat.inc
	NextMove        A_AlignmentPower_Duplicity_Post_NotRoot
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_radiusattack 1 2
	TEnd

	Type		huge
	  Anim          huge\air_radiusattack 1 2
	TEnd

	Type		fem
	  Anim          fem\air_radiusattack 1 2
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_radiusattack 1 2
	TEnd
MEnd

Move A_AlignmentPower_Duplicity_Post_NotRoot
	Requires	FLY COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        C_flyready
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_radiusattack 2
	TEnd

	Type		huge
	  Anim          huge\air_radiusattack 2
	TEnd

	Type		fem
	  Anim          fem\air_radiusattack 2
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_radiusattack 2
	TEnd
MEnd

####Speed: Burnout Buff ########################################

Move A_Burnout_Boostup
	Requires	FLY COMBAT BOOSTUP ALTERNATE LOW NOTROOTED
	Priority	16
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Air.inc
	NextMove        flyready
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_flight_afterburner 1
	TEnd

	Type		huge
	  Anim          male\air_flight_afterburner 1
	TEnd

	Type		fem
	  Anim          male\air_flight_afterburner 1
	TEnd
MEnd


##########################
## AIR Afterburner) ##
##########################

Move A_AfterburnerNoRoot_pre
	Requires	FLY COMBAT AFTERBURNER ALTERNATE NOTROOTED
	Priority        15
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Air.inc
	NextMove        flycycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_alt_beatchest 1 91
	TEnd

	Type		huge
	  Anim          huge\air_alt_beatchest 1 91
	TEnd

MEnd

###################
#####Time Manipulation Powerset######

###### Palmcast Time Crawl  ######
###################

Move A_Time_Crawl_Pre
	Requires	FLY COMBAT CAST MANIPULATION
	Priority        14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Time_Crawl
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_palmcast_time_crawl 1 20
	  PitchStart	5
	  PitchAngle	-0.5
	TEnd

MEnd

Move A_Time_Crawl
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        A_Time_Crawl_Post
	Flags		NoSizeScale PitchToTarget ReqInputs

	Type		male
	  Anim          male\air_palmcast_time_crawl 21 30
	  PitchStart	5
	  PitchAngle	-0.5
	TEnd

MEnd


Move A_Time_Crawl_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        Lazy_C_flycycle_Blend
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_palmcast_time_crawl 31
	TEnd

MEnd

###################
###### Radius Chrono Shift  ######
###################

Move A_Chrono_Shift_Pre
	Requires	FLY COMBAT RADIUSATTACK MANIPULATION
	Priority        14
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Chrono_Shift
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_radius_chrono_shift 1 49
	TEnd

MEnd

Move A_Chrono_Shift
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        A_Chrono_Shift_Post
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_radius_chrono_shift 50 65
	TEnd

MEnd


Move A_Chrono_Shift_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flycycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_radius_chrono_shift 66
	TEnd

MEnd

#######################################################################
####### Air Haunt (Darkness Control)   ###
#######################################################################

Move A_Haunt_Pre
	Requires	FLY COMBAT LIGHTERBLOW ALTERNATE
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Haunt
	Flags		CANTMOVE PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_onehand_palmcast 1 36
	  PitchStart	5
	  PitchAngle	-0.5
	TEnd
MEnd

Move A_Haunt
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_Haunt_Post
	Flags		CANTMOVE NoSizeScale ReqInputs pitchtotarget

	Type		male
	  Anim          male\air_onehand_palmcast 36 43
	  PitchStart	5
	  PitchAngle	-0.5
	TEnd
MEnd

Move A_Haunt_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_onehand_palmcast 44
	TEnd
MEnd

## Destiny BULLRUSH AIR ###########################################################

Move A_Destiny_Bullrush_Pre
	Requires	FLY COMBAT ENDGAME BULLRUSH #WEAPON
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Destiny_Bullrush
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_incarnate_destiny_attack 1 31
	TEnd
MEnd

Move A_Destiny_Bullrush
	Requires	FLY COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Destiny_Bullrush_Post
	Flags		NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\air_incarnate_destiny_attack 32 45
	TEnd
MEnd

Move A_Destiny_Bullrush_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flycycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_incarnate_destiny_attack 46
	TEnd
MEnd

###################
###### Summon Lion  ######
###################

Move A_Summon_Lion
	Requires	FLY COMBAT SUMMON ANIMAL
	Priority        14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Summon_Lion_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\summon_lion_air 1 60
	TEnd

MEnd

Move A_Summon_Lion_post
	Requires	COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flycycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\summon_lion_air 61
	TEnd

MEnd

###################
###### Summon Dire Wolf  ######
###################

Move A_Summon_Dire_Wolf
	Requires	FLY COMBAT SUMMON GROWL GIANT
	Priority        15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Summon_Dire_Wolf_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\summon_dire_wolf_air 1 60
	TEnd

MEnd

Move A_Summon_Dire_Wolf_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flycycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\summon_dire_wolf_air 61
	TEnd

MEnd

###################
###### Tame Beast Air  ######
###################

Move A_Tame_Beast
	Requires	FLY COMBAT BOOST ANIMAL
	Priority        14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Tame_Beast_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\Tame_Beast_air 1 60
	TEnd

MEnd

Move A_Tame_Beast_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flyready
	Flags		NoSizeScale

	Type		male
	  Anim          male\Tame_Beast_air 61
	TEnd

MEnd

#################################
###### Origin Pool Sorcery Spirit Ward  ######
#################################

Move A_Spirit_Ward
	Requires	FLY COMBAT ORIGINPOOL CAST QUICK
	Priority        15
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Spirit_Ward_post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\spirit_ward_air 1 60
	TEnd
MEnd

Move A_Spirit_Ward_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flycycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\spirit_ward_air 61
	TEnd
MEnd

#################################
###### Origin Pool Sorcery Arcane Bolt  ######
#################################

Move A_Arcane_Bolt
	Requires	FLY COMBAT ORIGINPOOL CAST
	Priority        14
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Arcane_Bolt_post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\Arcane_Bolt_air 1 60
	TEnd
MEnd

Move A_Arcane_Bolt_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flycycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\Arcane_Bolt_air 61
	TEnd
MEnd

#################################
###### Origin Pool Sorcery Enflame  ######
#################################

Move A_Enflame
	Requires	FLY COMBAT ORIGINPOOL DISRUPT
	Priority        14
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Enflame_post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\Enflame_air 1 62
	TEnd
MEnd

Move A_Enflame_post
	Requires	COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flycycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\Enflame_air 63
	TEnd
MEnd

##############################
###### Origin Pool Sorcery Translocation  ######
##############################

Move A_Translocation
	Requires	FLY ORIGINPOOL TELEPORT
	Priority        13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Translocation_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\translocation 1 18
	TEnd


MEnd

Move A_Translocation_post
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flycycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\translocation 19
	TEnd

MEnd

##############################
###### Origin Pool Force Of Will Weaken Resolve  ######
##############################

Move A_Weaken_Resolve
	Requires	FLY COMBAT ORIGINPOOL PSIONIC
	Priority        14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Weaken_Resolve_post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale
	Scale		0.9

	Type		male
	  Anim          male\weaken_resolve_air 1 60
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

MEnd

Move A_Weaken_Resolve_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_air.inc
	NextMove        C_flycycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\weaken_resolve_air 61
	TEnd

MEnd

##############################
###### Origin Pool Force Of Will Project Will  ######
##############################

Move A_Project_Will
	Requires	FLY COMBAT ORIGINPOOL BLAST
	Priority        14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Project_Will_post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale
	Scale		0.9

	Type		male
	  Anim          male\project_will_air 1 62
	  PitchStart	45
	  PitchAngle	0.2
	TEnd

MEnd

Move A_Project_Will_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flycycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\project_will_air 63
	TEnd

MEnd

##############################
###### Origin Pool Force Of Will Wall Of Force  ######
##############################

Move A_Wall_Of_Force
	Requires	FLY COMBAT ORIGINPOOL SONICBLAST
	Priority        14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Wall_Of_Force_post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale
	Scale		0.9

	Type		male
	  Anim          male\wall_of_force_air 1 31
	  PitchStart	29
	  PitchAngle	0.2
	TEnd

MEnd

Move A_Wall_Of_Force_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flycycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\wall_of_force_air 32
	TEnd

MEnd

###################
###### Martial Assault Trick Shot  ######
###################

Move A_Trick_Shot
	Requires	FLY COMBAT MARTIALASSAULT MAGICTHROW
	Priority        14
	NextMove        A_Trick_Shot_post
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\trick_shot_air 1 31
	TEnd

MEnd

Move A_Trick_Shot_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flycycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\trick_shot_air 32
	TEnd

MEnd

## Martial Assualt Masterful Throw #############################################

Move A_Masterful_Throw_Windup
	Requires	FLY COMBAT MARTIALASSAULT WINDUP
	Priority	14
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups_Air.inc
	Flags		NoSizeScale PitchToTarget
	NextMove	A_Masterful_Throw_Windup_Cycle

	Type		male
	  Anim		male\masterful_throw_air 1 10
	TEnd
MEnd

Move A_Masterful_Throw_Windup_Cycle
	Requires	FLY COMBAT MARTIALASSAULT WINDUP
	Include sequencers\Members\Windups_air.inc
	Flags		Cycle NoSizeScale PitchToTarget
	NextMove	C_flycycle

	Type		male
	  Anim		male\masterful_throw_air 11 26
	TEnd
MEnd

#############################################################

Move A_Masterful_Throw
	Requires	FLY COMBAT MARTIALASSAULT STAB
	Priority	14
	Interrupts	<AttackIRQ> <Windups>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Masterful_Throw_Post
	Sets		CANTMOVE
	Flags		NoSizeScale PitchToTarget

	Type		male
	  Anim		male\masterful_throw_air 27 62
	TEnd
MEnd

Move A_Masterful_Throw_Post
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	Flags		NoSizeScale

	Type		male
	  Anim		male\masterful_throw_air 63
	TEnd
MEnd

###### Martial Assault Explosive Shuriken  ######

Move A_Explosive_Shuriken
	Requires	FLY COMBAT MARTIALASSAULT MAGICTHROW EXPLODE
	Priority        15
	NextMove        A_Explosive_Shuriken_post
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\explosive_shuriken_air 1 50
	  PitchAngle	0.3
	  PitchStart	28
	  PitchEnd	48
	TEnd

MEnd

Move A_Explosive_Shuriken_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flycycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\explosive_shuriken_air 51
	TEnd

MEnd

###### Martial Assault Spinning Kick  ######

Move A_MartialAssualt_SpinningKick
	Requires	FLY COMBAT MARTIALASSAULT KICK
	Priority        14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_MartialAssualt_SpinningKick_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\spinning_kick_air 1 32
	TEnd
MEnd

Move A_MartialAssualt_SpinningKick_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flycycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\spinning_kick_air 33
	TEnd
MEnd

###### Air Martial Manipulation Inner Will ######

Move A_MartialManipulation_InnerWill
	Requires	FLY MARTIALASSAULT HAND
	Priority        13
	NextMove        A_MartialManipulation_InnerWill_post
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\inner_will_air 1 40

	TEnd

MEnd

Move A_MartialManipulation_InnerWill_post
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flycycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\inner_will_air 41
	TEnd

MEnd

############################
###### Burst Of Speed Air ######
###########################

Move A_Burst_Of_Speed
	Requires	FLY COMBAT MARTIALASSAULT TELEPORT
	Priority        14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Burst_Of_Speed_2
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\burst_of_speed_air 1 10
	TEnd
MEnd

Move A_Burst_Of_Speed_2
	Requires	FLY COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Burst_Of_Speed_post
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\burst_of_speed_air 11 40
	TEnd
MEnd

Move A_Burst_Of_Speed_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flycycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\burst_of_speed_air 41
	TEnd
MEnd