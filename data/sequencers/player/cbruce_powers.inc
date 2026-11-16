#########################################################
## CBruce_Powers.INC - CBruce Power related sequencers
#########################################################

##################################
## Burn - Ground Summon Variant ##
##################################

Move BurnSummon_Pre
	Requires	COMBAT BURNSUMMON
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        BurnSummon
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\ground_summon 1 45
	TEnd

	Type		huge
	  Anim          huge\ground_summon 1 45
	TEnd

	Type		fem
	  Anim          fem\ground_summon 1 45
	TEnd
MEnd

Move BurnSummon
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatBlend
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\ground_summon 45
	TEnd

	Type		huge
	  Anim          huge\ground_summon 45
	TEnd

	Type		fem
	  Anim          fem\ground_summon 45
	TEnd
MEnd


#####################
## Air Burn Summon ##
#####################

Move A_BurnSummon_Pre
	Requires	FLY COMBAT BURNSUMMON
	Priority	13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_BurnSummon
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_terrify 1 45
	TEnd

	Type		huge
	  Anim          huge\air_terrify 1 45
	TEnd

	Type		fem
	  Anim          fem\air_terrify 1 45
	TEnd
MEnd

Move A_BurnSummon
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flyready
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_terrify 45
	TEnd

	Type		huge
	  Anim          huge\air_terrify 45
	TEnd

	Type		fem
	  Anim          fem\air_terrify 45
	TEnd
MEnd


##########################
## Unyielding - No Root ##
##########################

Move Unyielding_Pre_NoRoot
	Requires	UNYIELD NOTROOTED
	Priority        2
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat.inc
	NextMove        Unyielding_NoRoot
	Flags		NoSizeScale

	Type		male
	  Anim          male\unyield 1 2
	TEnd

	Type		huge
	  Anim          huge\unyield 1 2
	TEnd

	Type		fem
	  Anim          fem\unyield 1 2
	TEnd
MEnd

Move Unyielding_NoRoot
	Include sequencers\Members\Combat.inc
	Flags		NoSizeScale
	NextMove	Unyielding_Cycle_NoRoot

	Type		male
	  Anim          male\unyield 2 61
	TEnd

	Type		huge
	  Anim          huge\unyield 2 61
	TEnd

	Type		fem
	  Anim          fem\unyield 2 61
	TEnd
MEnd

Move Unyielding_Cycle_NoRoot
	Include sequencers\Members\Combat.inc
	Flags		NoSizeScale Cycle
	NextMove	CombatReady

	Type		male
	  Anim          male\unyield 64 65
	TEnd

	Type		huge
	  Anim          huge\unyield 64 65
	TEnd

	Type		fem
	  Anim          fem\unyield 64 65
	TEnd
MEnd


##########################
## Boost Aura - No Root ##
##########################

Move Boostaura_pre_NoRoot
	Requires	BOOSTAURA NOTROOTED
	Priority        2
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat.inc
	NextMove        Boostaura_NoRoot
	Flags		NoSizeScale

	Type		male
	  Anim          male\ground_summon 1 2
	TEnd

	Type		huge
	  Anim          huge\ground_summon 1 2
	TEnd

	Type		fem
	  Anim          fem\ground_summon 1 2
	TEnd
MEnd

Move Boostaura_NoRoot
	Include sequencers\Members\Combat.inc
	NextMove        Boostaura2_noroot
	Flags		NoSizeScale

	Type		male
	  Anim          male\ground_summon 2 40
	TEnd

	Type		huge
	  Anim          huge\ground_summon 2 40
	TEnd

	Type		fem
	  Anim          fem\ground_summon 2 40
	TEnd
MEnd

Move Boostaura2_NoRoot
	Include sequencers\Members\Combat.inc
	NextMove	Boostaura3_NoRoot
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

Move Boostaura3_NoRoot
	Include sequencers\Members\Combat.inc
	NextMove	Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\flameaura 47
	TEnd

	Type		huge
	  Anim          huge\fireaura 47
	TEnd

	Type		fem
	  Anim          fem\fireaura 47
	TEnd
MEnd


##############################
## Air Boost Aura - No Root ##
##############################

Move A_Boostaura_pre_NoRoot
	Requires	FLY BOOSTAURA NOTROOTED
	Priority        13
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Air.inc
	NextMove        A_Boostaura_NoRoot
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_ground_summon 1 2
	TEnd

	Type		huge
	  Anim          huge\air_ground_summon 1 2
	TEnd

	Type		fem
	  Anim          fem\air_ground_summon 1 2
	TEnd
MEnd

Move A_Boostaura_NoRoot
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove        A_Boostaura2_NoRoot
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_ground_summon 2 40
	TEnd

	Type		huge
	  Anim          huge\air_ground_summon 2 40
	TEnd

	Type		fem
	  Anim          fem\air_ground_summon 2 40
	TEnd
MEnd

Move A_Boostaura2_NoRoot
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_Boostaura3_NoRoot
	Flags		NoSizeScale ReqInputs

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

Move A_Boostaura3_NoRoot
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove	flyready
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_fireaura 47
	TEnd

	Type		huge
	  Anim          huge\air_fireaura 47
	TEnd

	Type		fem
	  Anim          fem\air_fireaura 47
	TEnd
MEnd


####################################
## Boost Aura Fast Cast - No Root ##
####################################

Move Boostaura_FastCast_Pre_NoRoot
	Requires	BOOSTAURA NOTROOTED HIGH
	Priority        3
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat.inc
	NextMove	Boostaura_FastCast_NoRoot
	Flags		NoSizeScale

	Type		male
	  Anim          male\flameaura 1 2
	TEnd

	Type		huge
	  Anim          huge\fireaura 1 2
	TEnd

	Type		fem
	  Anim          fem\fireaura 1 2
	TEnd
MEnd

Move Boostaura_FastCast_NoRoot
	Include sequencers\Members\Combat.inc
	NextMove	Boostaura3_NoRoot
	Flags		NoSizeScale

	Type		male
	  Anim          male\flameaura 2 47
	TEnd

	Type		huge
	  Anim          huge\fireaura 2 47
	TEnd

	Type		fem
	  Anim          fem\fireaura 2 47
	TEnd
MEnd


########################################
## Air Boost Aura Fast Cast - No Root ##
########################################

Move A_Boostaura_FastCast_Pre_NoRoot
	Requires	FLY BOOSTAURA NOTROOTED HIGH
	Priority        14
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Air.inc
	NextMove	A_Boostaura_FastCast_NoRoot
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_fireaura 1 2
	TEnd

	Type		huge
	  Anim          huge\air_fireaura 1 2
	TEnd

	Type		fem
	  Anim          fem\air_fireaura 1 2
	TEnd
MEnd

Move A_Boostaura_FastCast_NoRoot
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove	Boostaura3_NoRoot
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_fireaura 2 47
	TEnd

	Type		huge
	  Anim          huge\air_fireaura 2 47
	TEnd

	Type		fem
	  Anim          fem\air_fireaura 2 47
	TEnd
MEnd


################################
## Quick Boost Aura - No Root ##
################################

Move Boostaura_quick_pre_NoRoot
	Requires	BOOSTAURA NOTROOTED QUICK
	Priority        3
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat.inc
	NextMove        Boostaura_quick_noroot
	Flags		NoSizeScale

	Type		male
	  Anim          male\flameaura 1 2
	TEnd

	Type		huge
	  Anim          huge\fireaura 1 2
	TEnd

	Type		fem
	  Anim          fem\fireaura 1 2
	TEnd
MEnd

Move Boostaura_quick_noroot
	Include sequencers\Members\Combat.inc
	NextMove	Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\flameaura 2
	TEnd

	Type		huge
	  Anim          huge\fireaura 2
	TEnd

	Type		fem
	  Anim          fem\fireaura 2
	TEnd
MEnd


####################################
## Air Quick Boost Aura - No Root ##
####################################

Move A_Boostaura_quick_pre_noroot
	Requires	FLY BOOSTAURA NOTROOTED QUICK
	Priority        14
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Air.inc
	NextMove        A_Boostaura_quick_noroot
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_fireaura 1 2
	TEnd

	Type		huge
	  Anim          huge\air_fireaura 1 2
	TEnd

	Type		fem
	  Anim          fem\air_fireaura 1 2
	TEnd
MEnd

Move A_Boostaura_quick_noroot
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove	Flyready
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_fireaura 2
	TEnd

	Type		huge
	  Anim          huge\air_fireaura 2
	TEnd

	Type		fem
	  Anim          fem\air_fireaura 2
	TEnd
MEnd

####################################
## Radius Attack - No Root ##
####################################

Move SymbioticRadiusAttack_NoRoot_Pre
	Requires	RADIUSATTACK NOTROOTED HIGH
	Priority        3
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat.inc
	NextMove	SymbioticRadiusAttack_NoRoot
	Flags		NoSizeScale

	Type		male
	  Anim          male\radius_chrono_shift 1 2
	TEnd

	Type		huge
	  Anim          huge\radius_chrono_shift 1 2
	TEnd

MEnd

Move SymbioticRadiusAttack_NoRoot
	Include sequencers\Members\Combat.inc
	NextMove	CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\radius_chrono_shift 3
	TEnd

	Type		huge
	  Anim          huge\radius_chrono_shift 3
	TEnd

MEnd

####################################
## Radius Attack - No Root ##
####################################

Move A_SymbioticRadiusAttack_NoRoot_Pre
	Requires	FLY RADIUSATTACK NOTROOTED HIGH
	Priority        14
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_air.inc
	NextMove	A_SymbioticRadiusAttack_NoRoot
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_radius_chrono_shift 1 2
	TEnd

MEnd

Move A_SymbioticRadiusAttack_NoRoot
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_radius_chrono_shift 3
	TEnd


MEnd

#############################
## Ground Summon - No Root ##
#############################

Move GroundSummon_Pre_NoRoot
	Requires	GROUNDSUMMON NOTROOTED #COMBAT
	Priority        3
	Interrupts	<AttackIRQ> PreCombat
	Include sequencers\Members\Combat.inc
	NextMove        GroundSummon_NoRoot
	Flags		NoSizeScale

	Type		male
	  Anim          male\ground_summon 1 2
	TEnd

	Type		huge
	  Anim          huge\ground_summon 1 2
	TEnd

	Type		fem
	  Anim          fem\ground_summon 1 2
	TEnd
MEnd

Move GroundSummon_NoRoot
	Include sequencers\Members\Combat.inc
	NextMove        CombatBlend
	Flags		NoSizeScale

	Type		male
	  Anim          male\ground_summon 2
	TEnd

	Type		huge
	  Anim          huge\ground_summon 2
	TEnd

	Type		fem
	  Anim          fem\ground_summon 2
	TEnd
MEnd


#################################
## Air Ground Summon - No Root ##
#################################

Move A_GroundSummon_Pre_NoRoot
	Requires	FLY GROUNDSUMMON NOTROOTED #COMBAT
	Priority        14
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Air.inc
	NextMove        A_GroundSummon_NoRoot
	Flags		NoSizeScale

	Type		male
	  Anim          male\teleport 1 2
	TEnd

	Type		huge
	  Anim          huge\ground_summon 1 2
	TEnd

	Type		fem
	  Anim          fem\ground_summon 1 2
	TEnd
MEnd

Move A_GroundSummon_NoRoot
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flyready
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_ground_summon 2
	TEnd

	Type		huge
	  Anim          huge\air_ground_summon 2
	TEnd

	Type		fem
	  Anim          fem\air_ground_summon 2
	TEnd
MEnd

#############################
## Sky Summon - No Root ##
#############################

Move SkySummon_pre_NoRoot
	Requires	COMBAT SKYSUMMON NOTROOTED
	Priority        3
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat.inc
	NextMove        SkySummon_NoRoot
	Flags		NoSizeScale

	Type		male
	  Anim          male\sky_summon 1 2
	TEnd

	Type		huge
	  Anim          huge\sky_summon 1 2
	TEnd

	Type		fem
	  Anim          fem\sky_summon 1 2
	TEnd
MEnd

Move SkySummon_NoRoot
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\sky_summon 2
	TEnd

	Type		huge
	  Anim          huge\sky_summon 2
	TEnd

	Type		fem
	  Anim          fem\sky_summon 2
	TEnd
MEnd


##############################
## Air Sky Summon - No Root ##
##############################

Move A_SkySummon_pre_NoRoot
	Requires	FLY COMBAT SKYSUMMON NOTROOTED
	Priority        14
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Air.inc
	NextMove        A_SkySummon_NoRoot
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_skysummon 1 2
	TEnd

	Type		huge
	  Anim          huge\air_skysummon 1 2
	TEnd

	Type		fem
	  Anim          fem\air_skysummon 1 2
	TEnd
MEnd

Move A_SkySummon_NoRoot
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flyready
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_skysummon 2
	TEnd

	Type		huge
	  Anim          huge\air_skysummon 2
	TEnd

	Type		fem
	  Anim          fem\air_skysummon 2
	TEnd
MEnd

############################
## Sonic Summon - No Root ##
############################

Move SonicSummon_pre_NoRoot
	Requires	COMBAT SONICSUMMON
	Priority        2
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat.inc
	NextMove        SonicSummon_NoRoot
	Flags		NoSizeScale

	Type		male
	  Anim          male\sky_summon 1 2
	TEnd

	Type		huge
	  Anim          huge\sky_summon 1 2
	TEnd

	Type		fem
	  Anim          fem\sky_summon 1 2
	TEnd
MEnd

Move SonicSummon_NoRoot
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\sky_summon 2 60
	TEnd

	Type		huge
	  Anim          huge\sky_summon 2 60
	TEnd

	Type		fem
	  Anim          fem\sky_summon 2 60
	TEnd
MEnd

################################
## Air Sonic Summon - No Root ##
################################

Move A_SonicSummon_pre_NoRoot
	Requires	FLY COMBAT SONICSUMMON
	Priority        13
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Air.inc
	NextMove        A_SonicSummon_NoRoot
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_skysummon 1 2
	TEnd

	Type		huge
	  Anim          huge\air_skysummon 1 2
	TEnd

	Type		fem
	  Anim          fem\air_skysummon 1 2
	TEnd
MEnd

Move A_SonicSummon_NoRoot
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove        airS_Summonpost
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_skysummon 2 60
	TEnd

	Type		huge
	  Anim          huge\air_skysummon 2 60
	TEnd

	Type		fem
	  Anim          fem\air_skysummon 2 60
	TEnd
MEnd


########################
## Levitate - No Root ##
########################

Move Levitate_NoRoot
	Requires	COMBAT LEVITATE NOTROOTED
	Priority        3
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat.inc
	NextMove        Levitatepost_NoRoot
	Flags		NoSizeScale

	Type		male
	  Anim          male\levitate 1 2
	TEnd

	Type		huge
	  Anim          huge\levitate 1 2
	TEnd

	Type		fem
	  Anim          fem\levitate 1 2
	TEnd
MEnd

Move Levitatepost_NoRoot
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Flags		NoSizeScale

	Type		male
	  Anim          male\levitate 2
	TEnd

	Type		huge
	  Anim          huge\levitate 2
	TEnd

	Type		fem
	  Anim          fem\levitate 2
	TEnd
MEnd

############################
## Air Levitate - No Root ##
############################

Move A_Levitate_NoRoot
	Requires	FLY COMBAT LEVITATE NOTROOTED
	Priority        14
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Air.inc
	NextMove        A_Levitatepost_NoRoot
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_levitate 1 2
	TEnd

	Type		huge
	  Anim          huge\air_levitate 1 2
	TEnd

	Type		fem
	  Anim          fem\air_levitate 1 2
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_levitate 1 2
	TEnd
MEnd

Move A_Levitatepost_NoRoot
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flycycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_levitate 2
	TEnd

	Type		huge
	  Anim          huge\air_levitate 2
	TEnd

	Type		fem
	  Anim          fem\air_levitate 2
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_levitate 2
	TEnd
MEnd


##############################
## Practiced Brawler Toggle ##
##############################

Move PracticedBrawler_pre_NoRoot
	Requires	COMBAT PRACTICEDBRAWLER
	Priority        2
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\MA_taunt 1
	TEnd

	Type		huge
	  Anim          huge\MA_taunt 1
	TEnd

	Type		fem
	  Anim          fem\MA_taunt 1
	TEnd
MEnd


##################################
## Air Practiced Brawler Toggle ##
##################################

Move A_PracticedBrawler_pre_NoRoot
	Requires	FLY COMBAT PRACTICEDBRAWLER
	Priority        13
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Air.inc
	NextMove        A_PracticedBrawler_NoRoot
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_MA_taunt 1 2
	TEnd

	Type		huge
	  Anim          huge\air_MA_taunt 1 2
	TEnd

	Type		fem
	  Anim          fem\air_MA_taunt 1 2
	TEnd
MEnd

Move A_PracticedBrawler_NoRoot
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove        Fake_C_flycycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_MA_taunt 2
	TEnd

	Type		huge
	  Anim          huge\air_MA_taunt 2
	TEnd

	Type		fem
	  Anim          fem\air_MA_taunt 2
	TEnd
MEnd


#############################
## Radius Effect - No Root ##
#############################

Move RadiusEffect_Pre_NoRoot
	Requires	RADIUSEFFECT NOTROOTED #COMBAT
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ> <BlockIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove        RadiusEffect_NoRoot
	Flags		NoSizeScale

	Type		male
	  Anim          male\radiuseffect 1 2
	TEnd

	Type		huge
	  Anim          huge\radiuseffect 1 2
	TEnd

	Type		fem
	  Anim          fem\radiuseffect 1 2
	TEnd
MEnd

Move RadiusEffect_NoRoot
	Include sequencers\Members\Combat.inc
	NextMove        Fake_CombatCycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\radiuseffect 2
	TEnd

	Type		huge
	  Anim          huge\radiuseffect 2
	TEnd

	Type		fem
	  Anim          fem\radiuseffect 2
	TEnd
MEnd


#################################
## Air Radius Effect - No Root ##
#################################

Move A_RadiusEffect_Pre_NoRoot
	Requires	FLY RADIUSEFFECT NOTROOTED #COMBAT
	Priority        13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_RadiusEffect_NoRoot
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_radiuseffect 1 2
	TEnd

	Type		huge
	  Anim          huge\air_radiuseffect 1 2
	TEnd

	Type		fem
	  Anim          fem\air_radiuseffect 1 2
	TEnd
MEnd

Move A_RadiusEffect_NoRoot
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flycycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_radiuseffect 2
	TEnd

	Type		huge
	  Anim          huge\air_radiuseffect 2
	TEnd

	Type		fem
	  Anim          fem\air_radiuseffect 2
	TEnd
MEnd

#############################
## Radius Attack - No Root ##
#############################

Move RadiusAttack_Pre_NoRoot
	Requires	COMBAT RADIUSATTACK NOTROOTED
	Priority        3
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat.inc
	NextMove        RadiusAttack_NoRoot
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

Move RadiusAttack_NoRoot
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


#################################
## Air Radius Attack - No Root ##
#################################

Move A_RadiusAttack_Pre_NoRoot
	Requires	FLY COMBAT RADIUSATTACK NOTROOTED
	Priority        14
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Air.inc
	NextMove        A_RadiusAttack_NoRoot
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

Move A_RadiusAttack_NoRoot
	Include sequencers\Members\Combat_Air.inc
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


#####################
## Boost_up Attack ##
#####################

Move Boostup_Attack
	Requires	BOOSTUPATTACK #COMBAT
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ> #<MoveIRQ>
	NextMove        Boostup_Attack_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\absorb 1 40
	TEnd

	Type		Wisp
	  Anim          Wisp\headcast_AOE 1 104
	TEnd
MEnd

Move Boostup_Attack_Post
	Include sequencers\Members\Combat.inc
	NextMove        BoostupReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\absorb 40
	TEnd

	Type		Wisp
	  Anim          Wisp\headcast_AOE 104
	TEnd
MEnd


#########################
## Air Boost_up Attack ##
#########################

Move A_Boostup_Attack
	Requires	FLY BOOSTUPATTACK #COMBAT
	Priority        13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Boostup_Attack_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\absorb 1 40
	TEnd

	Type		Wisp
	  Anim          Wisp\headcast_AOE 1 104
	TEnd
MEnd

Move A_Boostup_Attack_Post
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove        BoostupReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\absorb 40
	TEnd

	Type		Wisp
	  Anim          Wisp\headcast_AOE 104
	TEnd
MEnd


###################
## Granite Armor ##
###################

Move Boostup_Rooted
	Requires	BOOSTUP LOCKDOWN
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Boostuppost_Rooted
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\absorb 1 45
	TEnd

	Type		huge
	  Anim          huge\boostup 1 45
	TEnd

	Type		fem
	  Anim          fem\boostup 1 45
	TEnd
MEnd

Move Boostuppost_Rooted
	Include sequencers\Members\Combat.inc
	NextMove        BoostupReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\absorb 45
	TEnd

	Type		huge
	  Anim          huge\boostup 45
	TEnd

	Type		fem
	  Anim          fem\boostup 45
	TEnd
MEnd

#######################
## Air Granite Armor ##
#######################

Move A_Boostup_Rooted
	Requires	FLY BOOSTUP LOCKDOWN
	Priority        13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Boostuppost_Rooted
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_boostup 1 45
	TEnd

	Type		huge
	  Anim          huge\air_boostup 1 45
	TEnd

	Type		fem
	  Anim          fem\air_boostup 1 45
	TEnd

MEnd

Move A_Boostuppost_Rooted
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flycycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_boostup 45
	TEnd

	Type		huge
	  Anim          huge\air_boostup 45
	TEnd

	Type		fem
	  Anim          fem\air_boostup 45
	TEnd
MEnd

##########################
## Taunt_A (beat chest) ##
##########################

Move TauntANoRoot_pre
	Requires	COMBAT TAUNTA NOTROOTED
	Priority        3
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat.inc
	NextMove        TauntANoRoot_cycle
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

Move TauntANoRoot_cycle
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


#####################################
## Air Taunt_A NOROOT (beat chest) ##
#####################################

Move A_TauntANoRoot_pre
	Requires	FLY COMBAT TAUNTA NOTROOTED
	Priority        14
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flycycle #A_TauntANoRoot_cycle
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

#Move A_TauntANoRoot_cycle
#	Requires	FLY COMBAT
#	Include sequencers\Members\Combat_Air.inc
#	Flags		Cycle
#	NextMove	C_PreFall
#	Priority        2
#
#	Type		male
#	  Anim          male\air_comb_beatchest 95 144
#	TEnd
#
#	Type		huge
#	  Anim          huge\air_comb_beatchest 95 144
#	TEnd
#
#	Type		fem
#	  Anim          fem\air_comb_beatchest 95 144
#	TEnd
#MEnd

########################
## Boostdown - Rooted ##
########################

Move Boostdown_Pre_Rooted
	Requires	BOOSTDOWN ROOTED
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Boostdown_Rooted
	Flags		NoSizeScale
	Sets		CANTMOVE

	Type		male
	  Anim          male\boost 1 45
	TEnd

	Type		huge
	  Anim          huge\boostdown 1 45
	TEnd

	Type		fem
	  Anim          fem\boostdown 1 45
	TEnd
MEnd

Move Boostdown_Rooted
	Include sequencers\Members\Combat.inc
	NextMove        BoostDnReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\boost 45
	TEnd

	Type		huge
	  Anim          huge\boostdown 45
	TEnd

	Type		fem
	  Anim          fem\boostdown 45
	TEnd
MEnd

########################
## Air Boostdown - Rooted ##
########################

Move A_Boostdown_Pre_Rooted
	Requires	FLY BOOSTDOWN ROOTED
	Priority        12
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Boostdown_Rooted
	Flags		NoSizeScale
	Sets		CANTMOVE

	Type		male
	  Anim          male\air_boostdown 1 45
	TEnd

	Type		huge
	  Anim          huge\air_boostdown 1 45
	TEnd

	Type		fem
	  Anim          fem\air_boostdown 1 45
	TEnd
MEnd

Move A_Boostdown_Rooted
	Requires	FLY
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flycycle
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\air_boostdown 45
	TEnd

	Type		huge
	  Anim          huge\air_boostdown 45
	TEnd

	Type		fem
	  Anim          fem\air_boostdown 45
	TEnd
MEnd



##########################################################
## 		ALTERNATE ANIMATIONS			##
##########################################################


##########################
## Alt Beat Chest Taunt_A (alternate animation) ##
##########################

Move Alt_TauntANoRoot_pre
	Requires	COMBAT TAUNTA ALTERNATE NOTROOTED
	Priority        4
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat.inc
	NextMove        TauntANoRoot_cycle
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

Move Alt_TauntANoRoot_cycle
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		Cycle
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

##########################
## AIR Alternate Beat Chest Taunt_A (alternate animation) ##
##########################

Move A_Alt_TauntANoRoot_pre
	Requires	FLY COMBAT TAUNTA ALTERNATE NOTROOTED
	Priority        15
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flycycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_alt_beatchest 1 91
	TEnd

	Type		huge
	  Anim          huge\air_alt_beatchest 1 91
	TEnd

MEnd
############################################