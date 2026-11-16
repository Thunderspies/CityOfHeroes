#############################################################
## PSYCHIC, SONIC POWERS - Custom Animations 1
#############################################################

## ALT SONIC YELL ###########################################################

Move Alt_SonicYell_Pre
	Requires	COMBAT ATTACK SPEECH ALTERNATE
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Alt_SonicYell
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\alt_stopcast 1 40
	TEnd

	Type		huge
	  Anim          huge\alt_stopcast 1 40
	TEnd

MEnd

Move Alt_SonicYell
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Alt_SonicYell_post
	Flags		NoSizeScale

	Type		male
	  Anim          male\alt_stopcast 40 70
	TEnd

	Type		huge
	  Anim          huge\alt_stopcast 40 70
	TEnd

MEnd

Move Alt_SonicYell_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	#Interpolate	8
	Flags		NoSizeScale

	Type		male
	  Anim          male\sonic_yell 70
	TEnd

	Type		huge
	  Anim          huge\sonic_yell 70
	TEnd

	Type		fem
	  Anim          fem\sonic_yell 70
	TEnd

MEnd

## ALT SONIC SONIC YELL ###########################################################

Move Alt_SonicSonicYell
	Requires	COMBAT YELL STRONG ATTACK SPEECH ALTERNATE
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Alt_SonicYell
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\alt_stopcast 1 40
	TEnd

	Type		huge
	  Anim          huge\alt_stopcast 1 40
	TEnd

MEnd
######AIR ALT SONIC YELL#######################################################

Move A_Alt_SonicSonicYell_Pre
	Requires	FLY COMBAT YELL STRONG ATTACK SPEECH ALTERNATE
	Priority        17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Alt_SonicYell
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_alt_stopcast 1 40
	TEnd

MEnd

Move A_Alt_SonicYell
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_Alt_SonicYell_post
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_alt_stopcast 40 70
	TEnd

MEnd


Move A_Alt_SonicYell_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        Lazy_C_flycycle
	#Interpolate	8
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_sonic_yell 70
	TEnd

	Type		huge
	  Anim          huge\air_sonic_yell 70
	TEnd

MEnd

## ALT SONIC MOUTH  ###########################################################

Move Alt_SonicMouth_Pre
	Requires	COMBAT HEADCAST SPEECH ALTERNATE ONE
	Priority        5
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Alt_SonicMouth_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\alt_stopcast 1 64
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\alt_stopcast 1 64
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

MEnd

Move Alt_SonicMouth_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatBlend
	#Interpolate	8
	Flags		NoSizeScale

	Type		male
	  Anim          male\eye 65 70
	TEnd

	Type		huge
	  Anim          huge\eye 65 70
	  PitchAngle	0.1
	TEnd

	Type		fem
	  Anim          fem\eye 65 70
	  PitchAngle	0.1
	TEnd

MEnd

######## AIR ALT SONIC MOUTH #####################################################

Move A_Alt_SonicMouth
	Requires	FLY COMBAT HEADCAST SPEECH ALTERNATE ONE
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Alt_SonicMouth_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_alt_stopcast 1 64
	  PitchStart	1
	  PitchAngle	0.1
	PitchEnd	40
	TEnd

MEnd

Move A_Alt_SonicMouth_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	#Interpolate	8
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_mouthcast 65
	TEnd

	Type		huge
	  Anim          huge\air_mouth 65
	TEnd

	Type		fem
	  Anim          fem\air_mouth 65
	TEnd

MEnd

########Alt Mouth Slow Attack#####################################################

Move Alt_Mouth_Slow_Pre
	Requires	COMBAT HEADCAST HIGH STRONG ALTERNATE TWO
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Alt_Mouth_Slow
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale
	Scale		0.625

	Type		male
	  Anim          male\alt_stopcast 1 40
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\alt_stopcast 1 40
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

MEnd

Move Alt_Mouth_Slow
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Alt_Mouth_Post
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\alt_stopcast 40 70
	TEnd

	Type		huge
	  Anim          huge\alt_stopcast 40 70
	TEnd

MEnd

Move Alt_Mouth_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatBlend
	#Interpolate	8
	Flags		NoSizeScale

	Type		male
	  Anim          male\eye 70
	TEnd

	Type		huge
	  Anim          huge\eye 70
	TEnd

	Type		fem
	  Anim          fem\eye 70
	TEnd

MEnd

######AIR ALT Mouth Slow#######################################################

Move A_Alt_Mouth_Slow_Pre
	Requires	FLY COMBAT HEADCAST HIGH STRONG ALTERNATE TWO
	Priority	17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Alt_Mouth_Slow
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale
	Scale		0.625

	Type		male
	  Anim          male\air_alt_stopcast 1 40
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

MEnd

Move A_Alt_Mouth_Slow
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_Alt_Mouth_Post
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_alt_stopcast 40 64
	TEnd

MEnd

Move A_Alt_Mouth_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	#Interpolate	8
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_mouthcast 65
	TEnd

	Type		huge
	  Anim          huge\air_mouth 65
	TEnd

	Type		fem
	  Anim          fem\air_mouth 65
	TEnd

MEnd

## ALT HeadCast  ###########################################################

Move Alt_HeadCast_Pre
	Requires	COMBAT HEADCAST ALTERNATE
	Priority        3
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Alt_HeadCast_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\alt_stopcast_fists 1 50
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\alt_stopcast_fists 1 50
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

MEnd

Move Alt_HeadCast_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Interpolate	0
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\alt_stopcast_fists 50 70
	TEnd

	Type		huge
	  Anim          huge\alt_stopcast_fists 50 70
	TEnd

MEnd

######## AIR ALT HeadCast #####################################################

Move A_Alt_HeadCast
	Requires	FLY COMBAT HEADCAST ALTERNATE
	Priority        14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Alt_HeadCast_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_alt_stopcast_fists 1 64
	  PitchStart	1
	  PitchAngle	0.1
	PitchEnd	50
	TEnd

MEnd

Move A_Alt_HeadCast_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	Interpolate	0
	Scale           0.50
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_alt_stopcast_fists 65
	TEnd


MEnd


## ALT SONIC BLOW CAST ###########################################################

Move Alt_SonicBlow_Pre
	Requires	COMBAT BLOW SPEECH ALTERNATE ONE
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Alt_SonicBlow
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\alt_headcast 1 61
	  PitchStart	25
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\alt_headcast 1 61
	  PitchStart	25
	  PitchAngle	0.2
	TEnd

MEnd

Move Alt_SonicBlow
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Alt_SonicBlowpost
	Scale		0.75
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		male
	  Anim          male\alt_headcast 61 91
	TEnd

	Type		huge
	  Anim          huge\alt_headcast 61 91
	TEnd

MEnd

Move Alt_SonicBlowpost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Scale           0.5
	#Interpolate	8
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		male
	  Anim          male\blow_cast 95
	  PitchEnd	85
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\blow_cast 95
	  PitchEnd	85
	  PitchAngle	0.2
	TEnd

	Type		fem
	  Anim          fem\blow_cast 95
	  PitchEnd	85
	  PitchAngle	0.2
	TEnd

MEnd
#############################################################

Move A_Alt_SonicBlow_Pre
	Requires	FLY COMBAT BLOW SPEECH ALTERNATE ONE
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Alt_SonicBlow
	Sets            CANTMOVE
	Flags		NoSizeScale PitchtoTarget

	Type		male
	  Anim          male\air_alt_headcast 1 61
	pitchstart	20
	pitchangle	0.2
	TEnd

MEnd

Move A_Alt_SonicBlow
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_Alt_SonicBlowpost
	Flags		NoSizeScale PitchtoTarget

	Type		male
	  Anim          male\air_alt_headcast 61 91
	Pitchend	65
	pitchangle	0.2
	TEnd

MEnd

Move A_Alt_SonicBlowpost
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle_half
	#Interpolate	8
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_blow_cast 95
	TEnd

MEnd

## ALT SONIC QUICK SPIT ###########################################################

Move Alt_SonicSpit_Pre
	Requires	COMBAT BLOW CAST QUICK ALTERNATE
	Priority        5
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Alt_SonicSpit
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\alt_palmcast 1 31
	TEnd

	Type		huge
	  Anim          huge\alt_palmcast 1 31
	TEnd
MEnd

Move Alt_SonicSpit
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Alt_SonicSpit_Post
	Flags		NoSizeScale

	Type		male
	  Anim          male\alt_palmcast 31 74
	TEnd

	Type		huge
	  Anim          huge\alt_palmcast 31 74
	TEnd
MEnd

Move Alt_SonicSpit_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Lazy_CombatReady
	#Interpolate	8
	Flags		NoSizeScale

	Type		male
	  Anim          male\quickspit 75
	TEnd
MEnd

######ALT SONIC SPIT#######################################################

Move A_Alt_SonicSpit_Pre
	Requires	FLY COMBAT BLOW CAST QUICK ALTERNATE
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Alt_SonicSpit
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_alt_palmcast 1 31
	TEnd
MEnd

Move A_Alt_SonicSpit
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_Alt_SonicSpit_Post
	Flags		NoSizeScale PitchToTarget

	Type		male
	  Anim          male\air_alt_palmcast 31 74
	PitchEnd	60
	TEnd
MEnd

Move A_Alt_SonicSpit_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	#Interpolate	8
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_quickspit 75
	TEnd
MEnd

## ALT SONIC SING CAST ###########################################################

Move Alt_SonicSingCast_Pre
	Requires	COMBAT YELL YES CAST SPEECH ALTERNATE
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Alt_SingCast
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\alt_palmcast 1 30
	TEnd

	Type		huge
	  Anim          huge\alt_palmcast 1 30
	TEnd

MEnd

Move Alt_SingCast
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Alt_SingCast_post
	Flags		NoSizeScale

	Type		male
	  Anim          male\alt_palmcast 30 81
	TEnd

	Type		huge
	  Anim          huge\alt_palmcast 30 81
	TEnd

MEnd

Move Alt_SingCast_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	#Interpolate	8
	Flags		NoSizeScale

	Type		male
	  Anim          male\sing_cast 105
	TEnd

	Type		huge
	  Anim          huge\sing_cast 105
	TEnd

MEnd


########AIR  ALT SONIC SING CAST#####################################################

Move A_Alt_SonicSingCast_Pre
	Requires	FLY COMBAT YELL YES CAST SPEECH ALTERNATE
	Priority        17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Alt_SingCast
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_alt_palmcast 1 30
	TEnd
MEnd

Move A_Alt_SingCast
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_Alt_SingCast_post
	Flags		NoSizeScale PitchToTarget

	Type		male
	  Anim          male\air_alt_palmcast 30 81
	PitchEnd	60
	TEnd
MEnd

Move A_Alt_SingCast_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	Lazy_C_flycycle
	#Interpolate	8
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_sing_cast 105
	TEnd
MEnd

##################################
###### Alt Boost_up  ######
##################################

Move Alt_Boostup
	Requires	BOOSTUP ALTERNATE
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ> <BlockIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove        Alt_Boostuppost
	Flags		NoSizeScale

	Type		male
	  Anim          male\alt_sky_summon 1 20
	TEnd

	Type		huge
	  Anim          huge\alt_sky_summon 1 20
	TEnd

MEnd

Move Alt_Boostuppost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\alt_sky_summon 21
	TEnd

	Type		huge
	  Anim          huge\alt_sky_summon 21
	TEnd

MEnd

###################
###### ALT air Boost_up  ######
###################

Move A_Alt_Boostup
	Requires	FLY BOOSTUP ALTERNATE
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Alt_Boostuppost
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_alt_sky_summon 1 20
	TEnd

MEnd

Move A_Alt_Boostuppost
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_alt_sky_summon 21
	TEnd

MEnd

## ALT PSI BLAST - QUICK ###########################################################

Move Alt_PsiBlast_Quick_Pre
	Requires	COMBAT PSIONICBLAST HIGH ALTERNATE
	Priority        4
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Alt_PsiBlast
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale
	Scale		2.0

	Type		male
	  Anim          male\alt_magiccast_long 2 51
#	  PitchStart	13
#	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\alt_magiccast_long 2 51
#	  PitchStart	13
#	  PitchAngle	0.2
	TEnd

MEnd

## ALT AIR PSI BLAST - QUICK ###########################################################

Move A_Alt_PsiBlast_QUICK_Pre
	Requires	FLY COMBAT PSIONICBLAST HIGH ALTERNATE
	Priority	15
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Alt_PsiBlast
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale
	scale		2.0

	Type		male
	  Anim          male\air_alt_magiccast_long 2 51
#	  PitchStart	13
#	  PitchAngle	0.2
	TEnd

MEnd

## ALT PSI BLAST ###########################################################

Move Alt_PsiBlast_Pre
	Requires	COMBAT PSIONICBLAST ALTERNATE
	Priority        3
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Alt_PsiBlast
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\alt_magiccast_long 2 51
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\alt_magiccast_long 2 51
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

MEnd

Move Alt_PsiBlast
	Requires	COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Alt_PsiBlast_Post
	Sets            CANTMOVE
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		male
	  Anim          male\alt_magiccast_long 96 121
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\alt_magiccast_long 96 121
	  PitchAngle	0.2
	TEnd

MEnd

Move Alt_PsiBlast_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		ReqInputs NoSizeScale

	Type		male
	  Anim          male\alt_magiccast_long 121
	TEnd

	Type		huge
	  Anim          huge\alt_magiccast_long 121
	TEnd

MEnd

## ALT AIR PSIONIC BLAST ###########################################################

Move A_Alt_PsiBlast_Pre
	Requires	FLY COMBAT PSIONICBLAST ALTERNATE
	Priority	14
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Alt_PsiBlast
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_alt_magiccast_long 2 51
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

MEnd

Move A_Alt_PsiBlast
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_Alt_PsiBlast_Post
	Sets		CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_alt_magiccast_long 96 121
	  PitchAngle	0.2
	TEnd

MEnd

Move A_Alt_PsiBlast_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_alt_magiccast_long 121
	PitchEnd	170
	  PitchAngle	0.2
	TEnd

MEnd


###################
###### ALT Psionic Windup  Comes Before Alt_PsiQuick ######
###################

Move Alt_Pre_PsiWindup
	Requires	COMBAT MYSICWINDUP ALTERNATE ONE
	Priority        4
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups.inc
	NextMove        Alt_PsiWindup
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\alt_magiccast_long 1 50
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\alt_magiccast_long 1 50
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

MEnd

Move Alt_PsiWindup
	Requires	COMBAT MYSICWINDUP ALTERNATE ONE
	Include sequencers\Members\Windups.inc
	Flags		Cycle PitchToTarget NoSizeScale
	NextMove        Alt_PsiWindup_post

	Type		male
	  Anim          male\alt_magiccast_long 50 89
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\alt_magiccast_long 50 89
	  PitchAngle	0.2
	TEnd

MEnd

Move Alt_PsiWindup_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\alt_magiccast_long 165
	TEnd

	Type		huge
	  Anim          huge\alt_magiccast_long 165
	TEnd

MEnd

###### ALT QUICK Psionic Blast (comes after Alt_Pre_PsiWindup)############

Move Alt_PsiQuick
	Requires	COMBAT PSIONICBLAST QUICK ALTERNATE
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Alt_PsiQuick2
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\alt_magiccast_long 97 125
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\alt_magiccast_long 97 125
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

MEnd

Move Alt_PsiQuick2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\alt_magiccast_long 165
	 PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\alt_magiccast_long 165
	 PitchAngle	0.2
	TEnd

MEnd

###################
###### Air Psionic Windup  Comes Before A_Alt_PsiQUICK######
###################

Move A_Alt_Pre_PsiWindup
	Requires	FLY COMBAT MYSICWINDUP ALTERNATE ONE
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ>
	NextMove	A_Alt_PsiWindup
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_alt_magiccast_long 1 49
	 PitchStart	13
	  PitchAngle	0.2
	TEnd

MEnd

Move A_Alt_PsiWindup
	Requires	FLY COMBAT MYSICWINDUP ALTERNATE ONE
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ>
	Flags		Cycle PitchToTarget NoSizeScale
	NextMove	A_Alt_PsiWindup_post

	Type		male
	  Anim          male\air_alt_magiccast_long 49 89
	  PitchAngle	0.2
	TEnd

MEnd

Move A_Alt_PsiWindup_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_alt_magiccast_long 165
	TEnd

MEnd


###### Air ALT QUICK Psionic Blast (comes after A_Alt_Pre_PsiWindup)###########

Move A_Alt_PsiQUICK
	Requires	FLY COMBAT PSIONICBLAST QUICK ALTERNATE
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Alt_PsiQUICK2
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_alt_magiccast_long 97 125
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

MEnd

Move A_Alt_PsiQUICK2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_alt_magiccast_long 165
	  PitchAngle	0.2
	TEnd

MEnd

##### ALT Psionics Slow Cast########################################################

Move Alt_Psionics_SlowCast
	Requires	COMBAT PSIONIC HIGH ALTERNATE
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Alt_PostPsionics
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale
	Scale		0.9

	Type		male
	  Anim          male\alt_magiccast_long 9 89
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\alt_magiccast_long 9 89
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

MEnd

Move Alt_PostPsionics
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\alt_magiccast_long 162
	TEnd

	Type		huge
	  Anim          huge\alt_magiccast_long 162
	TEnd

MEnd

####### AIR Alt Psionics Slow Cast######################################################

Move A_Alt_Psionics_SlowerCast
	Requires	FLY COMBAT PSIONIC HIGH ALTERNATE
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Alt_PostPsionics
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale
	Scale		0.9

	Type		male
	  Anim          male\air_alt_magiccast_long 2 89
	  PitchStart	13
	  PitchAngle	0.2
	TEnd

MEnd

Move A_Alt_PostPsionics
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_alt_magiccast_long 162
	TEnd

MEnd

###################
###### ALT Terrify ######
###################

Move Alt_Terrify_Pre
	Requires	COMBAT TERRIFY ALTERNATE
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Alt_Terrify
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\alt_energy_torrent 1 39
	TEnd

	Type		huge
	  Anim          huge\alt_energy_torrent 1 39
	TEnd

MEnd

Move Alt_Terrify
	Include sequencers\Members\Combat.inc
	NextMove        Alt_Terrify_post
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\alt_energy_torrent 40 60
	  PitchEnd	63
	TEnd

	Type		huge
	  Anim          huge\alt_energy_torrent 40 60
	  PitchEnd	63
	TEnd

MEnd

Move Alt_Terrify_post
	Include sequencers\Members\Readies_Fake.inc
	NextMove        Fake_Combat_half
	Scale           0.50
	Flags		NoSizeScale

	Type		male
	  Anim          male\terrify 75
	  PitchEnd	63
	TEnd

	Type		huge
	  Anim          huge\terrify 75
	  PitchEnd	63
	TEnd

	Type		fem
	  Anim          fem\terrify 75
	  PitchEnd	63
	TEnd

MEnd

###################
###### ALT air Terrify ######
###################

Move A_Alt_Terrify_Pre
	Requires	FLY COMBAT TERRIFY ALTERNATE
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Alt_Terrify
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_alt_energy_torrent 1 39
	TEnd

MEnd

Move A_Alt_Terrify
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_Alt_Terrify_post
	Scale           0.50
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_alt_energy_torrent 40 60
	  PitchEnd	63
	TEnd

MEnd

Move A_Alt_Terrify_post
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove	Fake_C_flycycle_Half
	Scale           0.50
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_terrify 75
	  PitchEnd	63
	TEnd

MEnd

###################
###### Alt_TechWindup  ######
###################

Move Alt_Pre_TechWindup
	Requires	TECHWINDUP ALTERNATE TWO
	Priority        3
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups.inc
	NextMove        Alt_TechWindup
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\alt_palmcast_sniper 1 40
	  PitchStart	5
	  PitchAngle	-0.5
	TEnd

	Type		huge
	  Anim          huge\alt_palmcast_sniper 1 40
	  PitchStart	5
	  PitchAngle	-0.5
	TEnd

MEnd

Move Alt_TechWindup
	Requires	TECHWINDUP ALTERNATE TWO
	Include sequencers\Members\Windups.inc
	Flags		Cycle PitchToTarget NoSizeScale
	NextMove        Alt_TechWindup_post

	Type		male
	  Anim          male\alt_palmcast_sniper 40 60
	  PitchAngle	-0.5
	TEnd

	Type		huge
	  Anim          huge\alt_palmcast_sniper 40 60
	  PitchAngle	-0.5
	TEnd


MEnd

Move Alt_TechWindup_post
	Include sequencers\Members\Readies.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\alt_palmcast_sniper 119
	TEnd

	Type		huge
	  Anim          huge\alt_palmcast_sniper 119
	TEnd

MEnd

###################
###### Alt_Air TechWindup  ######
###################

Move A_Alt_Pre_TechWindup
	Requires	FLY TECHWINDUP ALTERNATE TWO
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ>
	NextMove	A_Alt_TechWindup
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_alt_palmcast_sniper 1 40
	  PitchStart	10
	  PitchAngle	-0.5
	TEnd

MEnd

Move A_Alt_TechWindup
	Requires	FLY TECHWINDUP ALTERNATE TWO
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ>
	Flags		Cycle PitchToTarget NoSizeScale
	NextMove	A_Alt_TechWindup_post

	Type		male
	  Anim          male\air_alt_palmcast_sniper 40 60
	  PitchAngle	-0.5
	TEnd

MEnd

Move A_Alt_TechWindup_post
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	Flags		NoSizeScale ReqInputs PitchToTarget

	Type		male
	  Anim          male\air_alt_palmcast_sniper 119
	TEnd

MEnd

####################
#######Alt_WindUp  ######
####################
#
#Move Alt_Pre_WindUp
#	Requires	COMBAT WINDUP ALTERNATE
#	Priority        2
#	Interrupts	<AttackIRQ>
#	Include sequencers\Members\Windups.inc
#	NextMove        WindUp
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim          male\windup_power 1 42
#	TEnd
#
#MEnd
#
#Move Alt_WindUp
#	Requires	COMBAT WINDUP ALTERNATE
#	Include sequencers\Members\Windups.inc
#	Flags		Cycle NoSizeScale
#	NextMove        Alt_WindUp_post
#
#	Type		male
#	  Anim          male\windup_power 42 63
#	TEnd
#
#MEnd
#
#Move Alt_WindUp_post
#	Requires	COMBAT
#	Include sequencers\Members\Combat.inc
#	NextMove        CombatReady
#	Flags		NoSizeScale ReqInputs
#
#	Type		male
#	  Anim          male\windup_power 63 77
#	TEnd
#MEnd
#
#
####################
####### Alt_Air WindUp  ######
####################
#
#Move A_Alt_Pre_WindUp
#	Requires	FLY WINDUP ALTERNATE
#	Priority	13
#	Interrupts	<AttackIRQ>
#	Include sequencers\Members\Windups_Air.inc
#	NextMove	A_Alt_WindUp
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim          male\air_windup 1 42
#	TEnd
#
#MEnd
#
#Move A_Alt_WindUp
#	Requires	FLY WINDUP ALTERNATE
#	Include sequencers\Members\Windups_Air.inc
#	Flags		Cycle NoSizeScale
#	NextMove	A_Alt_WindUp_post
#
#	Type		male
#	  Anim          male\air_windup 42 63
#	TEnd
#
#MEnd
#
#Move A_Alt_WindUp_post
#	Requires	FLY COMBAT
#	Include sequencers\Members\Combat_Air.inc
#	NextMove	C_flycycle
#	Flags		NoSizeScale ReqInputs
#
#	Type		male
#	  Anim          male\air_windup 63 77
#	TEnd
#
#MEnd

###################
###### Alt_SniperBlast (hands)  ######
###################

Move Alt_SniperBlast
	Requires	COMBAT STRONG CAST ALTERNATE
	Priority        4
	Interrupts	<AttackIRQ> Pre_Alt_TechWindup Alt_TechWindup
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Alt_SniperBlast2
	Flags		PitchToTarget NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\alt_palmcast_sniper 62 100
	  PitchAngle	-0.5
	  PitchEnd	94
	TEnd

	Type		huge
	  Anim          huge\alt_palmcast_sniper 62 100
	  PitchAngle	-0.5
	  PitchEnd	94
	TEnd

MEnd

Move Alt_SniperBlast2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\alt_palmcast_sniper 100
	TEnd

	Type		huge
	  Anim          huge\alt_palmcast_sniper 100
	TEnd

MEnd

###################
######Alt_Air SniperBlast (hands)  ######
###################

Move A_Alt_SniperBlast
	Requires	FLY COMBAT STRONG CAST ALTERNATE
	Priority	15
	Interrupts	<AttackIRQ> A_Alt_Pre_TechWindup A_Alt_TechWindup
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_Alt_SniperBlast2
	Flags		PitchToTarget NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\air_alt_palmcast_sniper 62 100
	  PitchAngle	-0.5
	TEnd

MEnd

Move A_Alt_SniperBlast2
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	Flags		NoSizeScale PitchToTarget

	Type		male
	  Anim          male\air_alt_palmcast_sniper 100
	  PitchAngle	-0.5
	  PitchEnd	125
	TEnd

MEnd

###################
###### Alt_2Hand Cast  ######
###################

Move Alt_2HandCast
	Requires	COMBAT 2HAND CAST ALTERNATE
	Priority        4
	Interrupts	<AttackIRQ> <Defiance> <Windups> Pre_WindUp WindUp
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Alt_2HandCastpost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\alt_palmcast_sniper_quick 1 50
	  PitchEnd	42
	  PitchAngle	0
	TEnd

	Type		huge
	  Anim          huge\alt_palmcast_sniper_quick 1 50
	  PitchEnd	42
	  PitchAngle	0
	TEnd
MEnd

Move Alt_2HandCastpost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\alt_palmcast_sniper_quick 50
	TEnd

	Type		huge
	  Anim          huge\alt_palmcast_sniper_quick 50
	TEnd

MEnd


###################
###### Alt_Air 2Hand Cast  ######
###################

Move A_Alt_2HandCast
	Requires	FLY COMBAT 2HAND CAST ALTERNATE
	Priority	16
	Interrupts	<AttackIRQ> <Windups>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Alt_2HandCastpost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_alt_palmcast_sniper_quick 1 50
	  PitchEnd	42
	TEnd

MEnd

Move A_Alt_2HandCastpost
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	Flags		NoSizeScale ReqInputs PitchToTarget

	Type		male
	  Anim          male\air_alt_palmcast_sniper_quick 50
	PitchEnd	50
	  PitchAngle	0
	TEnd

MEnd


###################
###### Alt_Stop cast ######
###################

Move Alt_Stop
	Requires	COMBAT STOPCAST ALTERNATE
	Priority        3
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Alt_Stop_cycle
	Flags		PitchToTarget NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\alt_stopcast_long 1 31
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\alt_stopcast_long 1 31
	  PitchAngle	0.5
	TEnd

MEnd

Move Alt_Stop_cycle
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Alt_Stop_post
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\alt_stopcast_long 31 60
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\alt_stopcast_long 31 60
	  PitchAngle	0.5
	TEnd

MEnd

Move Alt_Stop_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Scale           0.50
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\alt_stopcast_long 60
	  PitchEnd	64
	  PitchAngle	0.5
	TEnd

	Type		huge
	  Anim          huge\alt_stopcast_long 60
	  PitchEnd	64
	  PitchAngle	0.5
	TEnd

MEnd

###################
###### Alt_air Stop cast ######
###################

Move A_Alt_Stop
	Requires	FLY COMBAT STOPCAST ALTERNATE
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Alt_Stop_cycle
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_alt_stopcast_long 1 31
	  PitchAngle	0.5
	TEnd

MEnd

Move A_Alt_Stop_cycle
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_Alt_Stop_post
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		male
	  Anim          male\air_alt_stopcast_long 31 60
	  PitchAngle	0.5
	TEnd

MEnd

Move A_Alt_Stop_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	Scale           0.50
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_alt_stopcast_long 60
	  PitchEnd	64
	  PitchAngle	0.5
	TEnd

MEnd


################################
###### Alt Rapidblast Quick ######
################################

Move Alt_Rapid_Quick
	Requires	COMBAT RAPIDBLAST QUICK ALTERNATE
	Priority        4
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Alt_Rapid_Quick_Main
	Flags		PITCHTOTARGET NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\alt_palmcast_long 1 31
	TEnd

	Type		huge
	  Anim          huge\alt_palmcast_long 1 31
	TEnd
MEnd

Move Alt_Rapid_Quick_Main
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Alt_Rapid_Quick_Post
	Flags		NoSizeScale
	#Interpolate	2

	Type		male
	  Anim          male\alt_palmcast_long 31 100
	TEnd

	Type		huge
	  Anim          huge\alt_palmcast_long 31 100
	TEnd
MEnd

Move Alt_Rapid_Quick_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\alt_palmcast_long 101
	TEnd

	Type		huge
	  Anim          huge\alt_palmcast_long 101
	TEnd
MEnd

########Alt #####################################################

Move Alt_Rapidblast
	Requires	COMBAT RAPIDBLAST ALTERNATE
	Priority        3
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Alt_Rapid
	Flags		PITCHTOTARGET NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\alt_palmcast_long 1 50
	TEnd

	Type		huge
	  Anim          huge\alt_palmcast_long 1 50
	TEnd

MEnd

Move Alt_Rapid
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Alt_Rapid_Post
	#Interpolate	2
	Flags		NoSizeScale

	Type		male
	  Anim          male\alt_palmcast_long 50 100
	TEnd

	Type		huge
	  Anim          huge\alt_palmcast_long 50 100
	TEnd
MEnd

Move Alt_Rapid_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\alt_palmcast_long 101
	TEnd

	Type		huge
	  Anim          huge\alt_palmcast_long 101
	TEnd

MEnd

#############################################################

Move Alt_Rapid_Slow
	Requires	COMBAT RAPIDBLAST SLOW ALTERNATE
	Priority        4
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Alt_Rapid_Slow_Post
	Flags		PITCHTOTARGET NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\alt_palmcast_long 1 60
	TEnd

	Type		huge
	  Anim          huge\alt_palmcast_long 1 60
	TEnd
MEnd

Move Alt_Rapid_Slow_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\alt_palmcast_long 60
	TEnd

	Type		huge
	  Anim          huge\alt_palmcast_long 60
	TEnd
MEnd

################################
###### Alt Air Rapidblast  ######
################################

Move A_Alt_Rapid
	Requires	FLY COMBAT RAPIDBLAST ALTERNATE
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Alt_Rapid_Main
	Sets            CANTMOVE
	Flags		PITCHTOTARGET NoSizeScale

	Type		male
	  Anim          male\air_alt_palmcast_long 1 31
	Pitchstart 	5
	TEnd

MEnd

Move A_Alt_Rapid_Main
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_Alt_Rapidpost
	#Interpolate	30
	Flags		NoSizeScale PITCHTOTARGET

	Type		male
	  Anim          male\air_alt_palmcast_long 31 100
	PitchEnd	65
	TEnd

MEnd


Move A_Alt_Rapidpost
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_alt_palmcast_long 101
	TEnd

MEnd


###################
###### Alt magic throw  ######
###################

Move Alt_Magicthrow
	Requires	COMBAT MAGICTHROW ALTERNATE
	Priority        3
	NextMove        Alt_magicthrowpost
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\alt_energy_torrent_long 1 30
	  PitchAngle	0.3
	  PitchStart	11
	  PitchEnd	23
	TEnd

	Type		huge
	  Anim          huge\alt_energy_torrent_long 1 30
	  PitchAngle	0.3
	  PitchStart	11
	  PitchEnd	23
	TEnd

MEnd

Move Alt_magicthrowpost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\alt_energy_torrent_long 30 70
	TEnd

	Type		huge
	  Anim          huge\alt_energy_torrent_long 30 70
	TEnd

MEnd

###################
###### Air magic throw  ######
###################

Move A_Alt_magicthrow
	Requires	FLY COMBAT MAGICTHROW ALTERNATE
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Alt_magicthrowpost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_alt_energy_torrent_long 1 30
	  PitchAngle	0.3
	  PitchStart	11
	  PitchEnd	23
	TEnd

MEnd

Move A_Alt_magicthrowpost
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_alt_energy_torrent_long 30
	TEnd

MEnd

##################
######Alt_ Throw Knife  ######
###################

Move Alt_ThrowKnife_Pre
	Requires	COMBAT THROWKNIFE ALTERNATE
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Alt_ThrowKnife
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\alt_energy_torrent_short 1 22
	TEnd

	Type		huge
	  Anim          huge\alt_energy_torrent_short 1 22
	TEnd

MEnd

Move Alt_ThrowKnife
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Alt_ThrowKnife_Post
	Scale           0.375
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\alt_energy_torrent_short 22 31
	TEnd

	Type		huge
	  Anim          huge\alt_energy_torrent_short 22 31
	TEnd

MEnd

Move Alt_ThrowKnife_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Scale           0.5

	Type		male
	  Anim          male\alt_energy_torrent_short 31
	TEnd

	Type		huge
	  Anim          huge\alt_energy_torrent_short 31
	TEnd

MEnd

#############################################################

Move Alt_ThrowProjectile_Pre
	Requires	COMBAT THROWKNIFE ROOTED ALTERNATE
	Priority        4
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Alt_ThrowProjectile
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\alt_energy_torrent_short 1 21
	TEnd

	Type		huge
	  Anim          huge\alt_energy_torrent_short 1 21
	TEnd

MEnd

Move Alt_ThrowProjectile
	Requires	COMBAT
	Member		<DeathIRQ> <AttackIRQ> <MoveIRQ>
	NextMove        Alt_ThrowProjectile_Post
	Scale           0.375
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\alt_energy_torrent_short 21 31
	TEnd

	Type		huge
	  Anim          huge\alt_energy_torrent_short 21 31
	TEnd
MEnd

Move Alt_ThrowProjectile_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Scale           0.5

	Type		male
	  Anim          male\alt_energy_torrent_short 31
	TEnd

	Type		huge
	  Anim          huge\alt_energy_torrent_short 31
	TEnd
MEnd

###################
###### Air Throw Knife  ######
###################

Move A_Alt_ThrowKnife_Pre
	Requires	FLY COMBAT THROWKNIFE ALTERNATE
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Alt_ThrowKnife
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\air_alt_energy_torrent_short 1 21
	  PitchAngle	0.5
	TEnd

MEnd

Move A_Alt_ThrowKnife
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_Alt_ThrowKnife_Post
	Scale		0.375

	Type		male
	  Anim          male\air_alt_energy_torrent_short 21 31
	TEnd

MEnd

Move A_Alt_ThrowKnife_Post
	Requires	FLY COMBAT
	Priority	12
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	Scale		0.5

	Type		male
	  Anim          male\air_alt_energy_torrent_short 31
	TEnd

MEnd

#############################################################

Move A_Alt_ThrowProjectile_Pre
	Requires	FLY COMBAT THROWKNIFE ROOTED ALTERNATE
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Alt_ThrowProjectile
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\air_alt_energy_torrent_short 1 21
	  PitchAngle	0.5
	TEnd
MEnd

Move A_Alt_ThrowProjectile
	Requires	FLY COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Alt_ThrowProjectile_Post
	Scale		0.375
	Sets		CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_alt_energy_torrent_short 21 31
	TEnd
MEnd

Move A_Alt_ThrowProjectile_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	Scale		0.5

	Type		male
	  Anim          male\air_alt_energy_torrent_short 31
	TEnd
MEnd

## HEADCAST ###########################################################

Move Alt_Mouth
	Requires	COMBAT HEADCAST ALTERNATE TWO
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Alt_Mouth_Main
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\alt_headcast_short 1 50
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

	Type		huge
	  Anim          huge\alt_headcast_short 1 50
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

MEnd

Move Alt_Mouth_Main
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Alt_MouthBeam_Post
	Flags		NoSizeScale

	Type		male
	  Anim          male\alt_headcast_short 50 85
	TEnd

	Type		huge
	  Anim          huge\alt_headcast_short 50 85
	TEnd

MEnd

Move Alt_MouthBeam_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatBlend
	#Interpolate	8
	Flags		NoSizeScale

	Type		male
	  Anim          male\eye 85
	TEnd

	Type		huge
	  Anim          huge\eye 85
	TEnd

	Type		fem
	  Anim          fem\eye 85
	TEnd
MEnd

## AIR HEADCAST ###########################################################

Move A_Alt_Mouth
	Requires	FLY COMBAT HEADCAST ALTERNATE TWO
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Alt_Mouth_Main
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_alt_headcast_short 1 50
	  PitchStart	1
	  PitchAngle	0.1
	TEnd

MEnd

Move A_Alt_Mouth_Main
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_Alt_MouthBeam_Post
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_alt_headcast_short 50 85
	TEnd

MEnd


Move A_Alt_MouthBeam_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	#Interpolate	8
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_mouthcast 85
	TEnd

	Type		huge
	  Anim          huge\air_mouth 85
	TEnd

	Type		fem
	  Anim          fem\air_mouth 85
	TEnd

	Type		Wisp
	  Anim          Wisp\headcast 85
	TEnd

MEnd

###################
######Alt_ Blow Cast  ######
###################

Move Alt_Blow
	Requires	COMBAT BLOW ALTERNATE
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Alt_Blowpost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\alt_palmcast_medium 1 80
	  PitchStart	12
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\alt_palmcast_medium 1 80
	  PitchStart	12
	  PitchAngle	0.2
	TEnd

MEnd

Move Alt_Blowpost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Scale           0.5
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\alt_palmcast_medium 80
	  PitchEnd	85
	  PitchAngle	0.2
	TEnd

	Type		huge
	  Anim          huge\alt_palmcast_medium 80
	  PitchEnd	85
	  PitchAngle	0.2
	TEnd
MEnd

###################
######Alt_ Air Blow Cast  ######
###################

Move A_Alt_Blow
	Requires	FLY COMBAT BLOW ALTERNATE
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Alt_Blowpost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_alt_palmcast_medium 1 80
	  PitchStart	12
	  PitchAngle	0.2
	TEnd

MEnd

Move A_Alt_Blowpost
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	Scale           0.50
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_alt_palmcast_medium 80
	  PitchEnd	85
	  PitchAngle	0.2
	TEnd

MEnd


###################
###### Alt_Radius Attack  ######
###################

Move Alt_RadiusAttack
	Requires	COMBAT RADIUSATTACK ALTERNATE
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ> <IRQEmotesCombat> #<MoveIRQ>
	NextMove        Alt_PostRadAttack_Main
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\alt_radius_effect_long 1 30
	TEnd

	Type		huge
	  Anim          huge\alt_radius_effect_long 1 30
	TEnd

MEnd

Move Alt_PostRadAttack_Main
	Include sequencers\Members\Readies_Fake.inc
	NextMove        Fake_CombatCycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\alt_radius_effect_long 30
	TEnd

	Type		huge
	  Anim          huge\alt_radius_effect_long 30
	TEnd

MEnd

###################
###### Alt_ Air Radius Attack  ######
###################

Move A_Alt_RadiusAttack
	Requires	FLY COMBAT RADIUSATTACK ALTERNATE
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Alt_PostRadAttack
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_alt_radius_effect_long 1 30
	TEnd

MEnd

Move A_Alt_PostRadAttack
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_alt_radius_effect_long 30
	TEnd

MEnd


###################
###### Alt_ Cast  ######
###################

Move Alt_Cast
	Requires	COMBAT CAST ALTERNATE
	Priority        3
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Alt_Castpost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\alt_energy_torrent_medium 1 30
	  PitchStart	5
	  PitchAngle	-0.5
	TEnd

	Type		huge
	  Anim          huge\alt_energy_torrent_medium 1 30
	  PitchStart	5
	  PitchAngle	-0.5
	TEnd

MEnd

Move Alt_Castpost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\alt_energy_torrent_medium 30
	TEnd

	Type		huge
	  Anim          huge\alt_energy_torrent_medium 30
	TEnd

MEnd

###################
###### Alt_ Air Cast  ######
###################

Move A_Alt_Cast
	Requires	FLY COMBAT CAST ALTERNATE
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Alt_Castpost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\air_alt_energy_torrent_medium 1 30
	  PitchStart	5
	  PitchAngle	-0.5
	TEnd
MEnd

Move A_Alt_Castpost
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_alt_energy_torrent_medium 30
	TEnd

MEnd

############################################################################################################