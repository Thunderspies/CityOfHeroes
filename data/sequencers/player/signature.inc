#########################################################
## SIGNATURE.INC - Signature Character Powers
#########################################################

##############################
###### Mirror Spirit Teleport  ######
##############################

Move MirrorSpirit_Teleport
	Requires	MIRRIORTELEPORT
	Priority        1
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        MirrorSpirit_Teleport2
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          fem\kuji_zen 1 50
	  PlayFX	V_COV\EnemyPowers\SignatureHeroes\MirrorSpirit\Mirror_teleport_A.fx 15
	  PlayFx        GENERIC\ENTITYVANISH.FX 40 CONSTANT
	TEnd
MEnd

### character has now moved - character begins invisible ###

Move MirrorSpirit_Teleport2
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        MirrorSpirit_Teleport3
	Interpolate	0
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          fem\kuji_zen 11 30
	  PlayFx	GENERIC\ENTITYVANISH.FX 0 CONSTANT
	  PlayFX	V_COV\EnemyPowers\SignatureHeroes\MirrorSpirit\Mirror_teleport_A.fx 0
	TEnd
MEnd

Move MirrorSpirit_Teleport3
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        MirrorSpirit_post
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          fem\kuji_zen 31
	TEnd
MEnd

Move MirrorSpirit_post
	Include sequencers\Members\Readies.inc
	NextMove        Ready
	Scale		0.6
	Flags		NoSizeScale

	Type		male
	  Anim          male\stand_2Hhips 123
	TEnd

	Type		fem
	  Anim          fem\stand_2Hhips 123
	TEnd
MEnd

##########################
####### Mirrior Hold   ###
##########################

Move Mirrior_Hold
	Requires	COMBAT MIRRIORHOLD
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Mirrior_Hold2
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\kuji_zen 1 50
	TEnd

	Type		fem
	  Anim          fem\kuji_zen 1 50
	TEnd


	Type		huge
	  Anim          huge\kuji_zen 1 50
	TEnd
MEnd

Move Mirrior_Hold2
	Requires	COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Mirrior_Hold_post
	Flags		CANTMOVE NoSizeScale PitchToTarget
	Interpolate	15

	Type		male
	  Anim          male\palmcast 1 20
	  PitchStart	5
	  PitchAngle	-0.5
	TEnd
MEnd

Move Mirrior_Hold_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Lazy_CombatReady
	Flags		NoSizeScale PitchToTarget

	Type		male
	  Anim          male\palmcast 20
	  PitchEnd	34
	  PitchAngle	-0.5
	TEnd
MEnd


########################
####### Chi Force   ###
########################

Move Chi_Force
	Requires	COMBAT CHIFORCE
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Chi_Force_post
	Priority        2
	Flags		CANTMOVE PitchToTarget NoSizeScale

	Type		male
	  Anim          male\chi_attack_dual 1 67
	TEnd
MEnd

Move Chi_Force_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Lazy_CombatReady
	Flags		NoSizeScale PitchToTarget ReqInputs

	Type		male
	  Anim          male\chi_attack_dual 67
	  PitchEnd	70
	TEnd
MEnd

##########################
####### Zeus's Lightning    ###
##########################

Move Zeus_Lightning
	Requires	COMBAT ZEUSATTACK
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Zeus_Lightning_2
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\states_zeus 1 100
	TEnd
MEnd

Move Zeus_Lightning_2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Lazy_Combat_Blend
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\states_zeus 100
	TEnd
MEnd


###################
## Black Scorpion Tail sting
###################

Move BS_Sting
	Requires	COMBAT BSSTING
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        BS_Sting_post
	Flags		CANTMOVE
	SticksOnChild	COMBAT BSSTING

	Type		male
	  Anim          huge\BS_STING 1 25
	TEnd
MEnd

Move BS_Sting_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove	CombatReady
	SticksOnChild	COMBAT
	Flags		ReqInputs

	Type		male
	  Anim          huge\BS_STING 25
	TEnd
MEnd


###################
## Black Scorpion Tail Blaster
###################

Move BS_TailBlast
	Requires	COMBAT BSBLAST
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        BS_TailBlast_post
	Flags		CANTMOVE PitchToTarget
	SticksOnChild	COMBAT BSBLAST

	Type		male
	  Anim          huge\BS_blast_tail 1 40
	TEnd
MEnd

Move BS_TailBlast_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove	CombatReady
	SticksOnChild	COMBAT
	scale		0.8

	Type		male
	  Anim          huge\BS_blast_tail 40
	TEnd
MEnd

###################
## Black Scorpion 360 tail swipe
###################

Move BS_Tailarc
	Requires	COMBAT BSARC
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        BS_Tailarc_post
	Flags		CANTMOVE
	SticksOnChild	COMBAT BSARC
	scale		0.8

	Type		male
	  Anim          huge\BS_spin_360 1 35
	TEnd
MEnd

Move BS_Tailarc_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove	CombatReady
	SticksOnChild	COMBAT
	scale		0.8

	Type		male
	  Anim          huge\BS_spin_360 35
	TEnd
MEnd


###################
## Black Scorpion Arm Blast
###################

Move BS_ArmBlast
	Requires	COMBAT BSARMCANNON
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        BS_ArmBlast_post
	Flags		CANTMOVE PitchToTarget
	SticksOnChild	COMBAT BSARMCANNON

	Type		male
	  Anim          huge\BS_blast_arm 1 65
	TEnd
MEnd

Move BS_ArmBlast_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove	CombatReady
	SticksOnChild	COMBAT

	Type		male
	  Anim          huge\BS_blast_arm 65 75
	TEnd
MEnd


########################
##  Miss Liberty Belt ##
########################

Move Liberty_Belt
	Requires	COMBAT LIBERTYBELT
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Liberty_Belt_post
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\glory 1 30
	TEnd
MEnd

Move Liberty_Belt_post
	Requires	Combat
	Include sequencers\Members\Combat.inc
	NextMove        Lazy_Combat_half

	Type		male
	  Anim          male\glory 30
	TEnd
MEnd


#############
## Synapse ##
#############

Move Synapse_Spin
	Requires	COMBAT SYNAPSESPIN
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        JumpPost
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\spin 1
	TEnd

	Type		huge
	  Anim          huge\spin 1
	TEnd

	Type		fem
	  Anim          fem\spin 1
	TEnd
MEnd


## MINOTAUR BELLOW ###########################################################

Move Bellow_Draw
	Requires	COMBAT WEAPON BLOW SPEECH SPAWNBACK
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Bellow
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

Move Bellow
	Requires        COMBAT WEAPON BLOW SPEECH
	Priority        4
	Interrupts      <AttackIRQ>
	Member          <DeathIRQ> <AttackIRQ>
	NextMove        Bellow_Post
	Sets            CANTMOVE
	Flags		NoSizeScale
	Scale		0.75

	Type            male
	  Anim          male\blow_cast 1 80
	TEnd

	Type            huge
	  Anim          huge\blow_cast 1 80
	TEnd

	Type            fem
	  Anim          fem\blow_cast 1 80
	TEnd
MEnd

Move Bellow_Post
	Requires	COMBAT WEAPON
	Include sequencers\Members\Combat.inc
	NextMove        Bellow_Post_Cycle
	Flags		NoSizeScale ReqInputs

	Type            male
	  Anim          male\blow_cast 80 85
	TEnd

	Type            huge
	  Anim          huge\blow_cast 80 85
	TEnd

	Type            fem
	  Anim          fem\blow_cast 80 85
	TEnd
MEnd

Move Bellow_Post_Cycle
	Requires        COMBAT WEAPON
	Include sequencers\Members\Combat.inc
	Flags           Cycle
	NextMove        postReady_back
	Scale           0.50

	Type            male
	  Anim          male\wep_cycle 1 31
	TEnd
MEnd