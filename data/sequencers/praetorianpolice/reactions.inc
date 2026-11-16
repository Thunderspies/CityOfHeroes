#############################################################
## REACTIONS.INC - Hits, Rez, Status Effects, etc.
#############################################################

#############################################################
## Hit Quick
#############################################################

Move HitQuick
	Requires	QUICK HIT
	Priority        2
	Interrupts	<HitIRQ> FearStun_cycle FearStun
	Include sequencers\Members\Reactions.inc
	NextMove        Fake_CombatCycle_Half
	Flags		HitReact

	Type		male
	  Anim          male\HQ_high 3
	  PlayFX	ENEMYFX\V_PraetorianPolice\FormShield\FormShield_QuickHit01.fx 0 #CONSTANT
	TEnd
MEnd

#############################################################
## Combat Hit Quick
#############################################################

Move C_HitQuick
	Requires	COMBAT QUICK HIT
	Priority        3
	Interrupts	<HitIRQ> FearStun_cycle FearStun
	Include sequencers\Members\Reactions.inc
	NextMove        CombatReady
	Flags		HitReact

	Type		male
	  Anim          male\HQ_high 1
	  PlayFX	ENEMYFX\V_PraetorianPolice\FormShield\FormShield_QuickHit01.fx 0 #CONSTANT
	TEnd
MEnd

#############################################################

Move C_HitQuick_mid
	Requires	COMBAT QUICK HIT
	Priority        3
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        CombatReady
	Flags		HitReact

	Type		male
	  Anim          male\HQ_mid2 2
	  PlayFX	ENEMYFX\V_PraetorianPolice\FormShield\FormShield_QuickHit01.fx 0 #CONSTANT
	TEnd
MEnd


##############################################################
### Weapon Hit Quick
##############################################################
#
#Move W_HitQuick
#	Requires	COMBAT WEAPON QUICK HIT
#	Priority        4
#	Interrupts	<HitIRQ> FearStun_cycle FearStun
#	Include sequencers\Members\Reactions.inc
#	NextMove        WepReady
#	Flags		HitReact
#
#	Type		male
#	  Anim          male\wepHQ_high 1
#	TEnd
#MEnd
#
#
##############################################################
### Shotgun Hit Quick
##############################################################
#
#Move SG_HitQuick
#	Requires	COMBAT WEAPON GUN QUICK HIT
#	Priority        5
#	Interrupts	<HitIRQ> FearStun_cycle FearStun
#	Include sequencers\Members\Reactions.inc
#	NextMove        ShotgunReady
#	Flags		HitReact
#	Flags		FixArmReg
#
#	Type		male
#	  Anim          male\shotgunHQ_high 1
#	TEnd
#
#	Type		huge
#	  Anim          huge\shotgunHQ_high 2
#	TEnd
#
#	Type		fem
#	  Anim          fem\rocket_HQ 1
#	TEnd
#MEnd

##############################################################
### FreakshowSign_react
##############################################################
#
#Move FreakshowSign_react
#	Requires	HIT FREAKSHOWREACT
#	Priority        2
#	Interrupts	<ReactIRQ> <DeathIRQ>
#	Include sequencers\Members\Reactions.inc
#	NextMove        Emote_Hi2
#	Flags		HitReact
#
#	Type		male
#	  Anim          male\hi 1 45
#	TEnd
#
#	Type		huge
#	  Anim          huge\hi 1 45
#	TEnd
#
#	Type		fem
#	  Anim          fem\hi 1 45
#	TEnd
#MEnd
#
##############################################################
###  Millitary Upgrade2 Hit
##############################################################
#
#Move MillitaryUpgrade2_Hit
#	Requires	HIT MILITARYUPGRADE2
#	Priority        2
#	Interrupts	<HitIRQ> <DeathIRQ> <AttackIRQ>
#	Sets		CANTMOVE
#	Flags		NoSizeScale FixArmReg
#	NextMove	MillitaryUpgrade2
#
#	Type		male
#	  Anim          male\military_upgrade 1
#	  PlayFX	V_COV\Powers\Paramilitary\Paramilitary_Upgrade_1.fx 0 Constant
#	TEnd
#MEnd
#
#Move MillitaryUpgrade2
#	Include sequencers\Members\Reactions.inc
#	NextMove	MillitaryUpgrade3
#	Flags		NoSizeScale FixArmReg
#
#	Type		male
#	  Anim          male\formal_salute2 1 40
#	TEnd
#MEnd
#
#Move MillitaryUpgrade3
#	Include sequencers\Members\Reactions.inc
#	NextMove	MillitaryUpgrade4_cycle
#
#	Type		male
#	  Anim          male\at_ease 1 60
#	TEnd
#MEnd
#
#Move MillitaryUpgrade4_cycle
#	Include sequencers\Members\Reactions.inc
#	Flags		Cycle
#
#	Type		male
#	  Anim          male\at_ease 61 65
#	TEnd
#MEnd
#
#
##############################################################
### Thugs Upgrade2 Hit
##############################################################
#
#Move ThugsUpgrade_Hit
#	Requires	HIT THUGSUPGRADE2
#	Priority        2
#	Interrupts	<ReactIRQ> <DeathIRQ> <AttackIRQ>
#	Sets		CANTMOVE
#	Flags		NoSizeScale FixArmReg
#	NextMove	ThugsUpgrade_2
#
#	Type		male
#	  Anim          male\thugs_upgrade 1 296
#	  PlayFX	V_COV\Powers\Thugs\Thugs_Upgrade_2.fx 20 Constant
#	TEnd
#MEnd
#
#Move ThugsUpgrade_2
#	Include sequencers\Members\Reactions.inc
#	NextMove	ThugsUpgrade_3
#	Flags		NoSizeScale FixArmReg
#
#	Type		male
#	  Anim          male\lookquick 1
#	TEnd
#MEnd
#
#Move ThugsUpgrade_3
#	Include sequencers\Members\Reactions.inc
#	NextMove	ThugsUpgrade_cycle
#	Scale		0.5
#
#	Type		male
#	  Anim          male\suspicious 1
#	TEnd
#MEnd
#
#Move ThugsUpgrade_cycle
#	Include sequencers\Members\Reactions.inc
#	Flags		Cycle
#	Scale		0.5
#
#	Type		male
#	  Anim          male\suspicious 1
#	TEnd
#MEnd
#
##############################################################
### ParaMilitary Catch Serum
##############################################################
#
#Move PM_BoostUpSerum
#	Requires	BOOSTUP SERUM HIT
#	Priority        3
#	Interrupts	<ReactIRQ>
#	Include sequencers\Members\Reactions.inc
#	NextMove        PM_BoostUpSerum_Ready
#
#	Type		male
#	  Anim          male\Paramilitary_CatchBoost 1
#	TEnd
#MEnd
#
#Move PM_BoostUpSerum_Ready
#	Requires	BOOSTUP SERUM
#	Include sequencers\Members\Reactions.inc
#	Flags		Cycle
#	Interpolate	15
#
#	Type		male
#	  Anim          male\Skel_ready2 1 60
#	TEnd
#MEnd
#
##############################################################
### ParaMilitary Boost Up
##############################################################
#
#Move PM_BoostUpAmmo
#	Requires	BOOSTUP EQUIP HIT
#	Priority        5
#	Interrupts	<ReactIRQ>
#	NextMove        PM_BoostUpEquip_Ready
#	Sets		CANTMOVE
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim          male\Paramilitary_CatchBoost 1 45
#	TEnd
#MEnd
#
#Move PM_BoostUpEquip_Ready
#	Requires	BOOSTUP EQUIP HIT
#	Sets		CANTMOVE
#	Flags		Cycle NoSizeScale
#	Interpolate	15
#
#	Type		male
#	  Anim          male\Skel_ready2 1 60
#	TEnd
#MEnd
#
#
##############################################################
### Ninja Upgrade 1 Hit
##############################################################
#
#Move NinjaUpgrade_Hit
#	Requires	HIT NINJAUPGRADE
#	Priority        2
#	Interrupts	<ReactIRQ>
#	Include sequencers\Members\Reactions.inc
#	Sets		CANTMOVE
#	Flags		NoSizeScale
#	NextMove	NinjaUpgrade_post   ##Bow_Reverant
#
#	Type		male
#	  Anim          male\kuji_rin 1 70
#	  PlayFX	V_COV\Powers\Ninjitsu\NinjaPsionicsDefence.fx 0 Constant
#	TEnd
#MEnd
#
#Move NinjaUpgrade_cycle
#	Requires	HIT NINJAUPGRADE
#	Include sequencers\Members\Reactions.inc
#	Sets		CANTMOVE
#	Flags		Cycle NoSizeScale
#	NextMove	NinjaUpgrade_post
#
#	Type		male
#	  Anim          male\kuji_rin 70 130
#	  PlayFX	V_COV\Powers\Ninjitsu\NinjaPsionicsDefence.fx 0 Constant
#	TEnd
#MEnd
#
#Move NinjaUpgrade_post
#	Include sequencers\Members\Reactions.inc
#	Sets		CANTMOVE
#	Flags		NoSizeScale
#	NextMove	NinjaUpgrade_post_cycle
#
#	Type		male
#	  Anim          male\quick_bow 1 104
#	TEnd
#MEnd
#
#Move NinjaUpgrade_post_cycle
#	Include sequencers\Members\Reactions.inc
#	Flags		NoSizeScale Cycle
#
#	Type		male
#	  Anim          male\quick_bow 104 163
#	TEnd
#MEnd
#
#
##############################################################
### Ninja Upgrade 2 Hit
##############################################################
#
#Move NinjaUpgrade2_Hit
#	Requires	HIT NINJAUPGRADE HIGH
#	Priority        3
#	Interrupts	<ReactIRQ>
#	Include sequencers\Members\Reactions.inc
#	Sets		CANTMOVE
#	Flags		NoSizeScale
#	NextMove	NinjaUpgrade2_cycle
#
#	Type		male
#	  Anim          male\kuji_sha2 1 70  ##130
#	  PlayFX	V_COV\Powers\Ninjitsu\NinjaElude.fx 0 Constant
#	TEnd
#MEnd
#
#Move NinjaUpgrade2_cycle
#	Requires	HIT NINJAUPGRADE HIGH
#	Include sequencers\Members\Reactions.inc
#	Sets		CANTMOVE
#	Flags		NoSizeScale Cycle
#	NextMove	NinjaUpgrade_post_cycle
#
#	Type		male
#	  Anim          male\kuji_sha2 70 130
#	  PlayFX	V_COV\Powers\Ninjitsu\NinjaElude.fx 0 Constant
#	TEnd
#MEnd
#
#
##############################################################
### Ninja train Stealth Hit
##############################################################
#
#Move NinjaStealth_Hit
#	Requires	HIT NINJATRAIN
#	Priority        2
#	Interrupts	<ReactIRQ>
#	Include sequencers\Members\Reactions.inc
#	Sets		CANTMOVE
#	Flags		NoSizeScale
#	NextMove	NinjaStealth_post   ##Bow_Reverant
#
#	Type		male
#	  Anim          male\smokeflash 1 50
#	  PlayFX	V_COV\Powers\Ninjitsu\Zero_AOEWhiteSmoke.fx 45
#	TEnd
#MEnd
#
#Move NinjaStealth_post
#	Include sequencers\Members\Reactions.inc
#	Flags		NoSizeScale
#	NextMove	Lazy_CombatReady
#
#	Type		male
#	  Anim          male\smokeflash 51
#	TEnd
#MEnd


#############################################################
## Hit Barf
#############################################################

Move HitBarf
	Requires	HIT SAD BLAST
	Priority        3
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        Hit_Barf_cycle

	Type		male
	  Anim          male\barf 1 139
	  PlayFX	Generic\Vomit.fx 25
	TEnd

	Type		fem
	  Anim          male\barf 1 139
	  PlayFX	Generic\Vomit_female.fx 25
	TEnd

	Type		huge
	  Anim          huge\quickbarf 1
	  PlayFX	Generic\Vomit.fx 25
	TEnd
MEnd

Move Hit_Barf_cycle
	Include sequencers\Members\Reactions.inc
	Flags		Cycle FixArmReg

	Type		male
	  Anim          male\barf_cycle 1 50
	TEnd

	Type		huge
	  Anim          huge\barf_cycle 1 50
	TEnd
MEnd

#############################################################
## Wep Hit Barf
#############################################################

Move W_HitBarf
	Requires	WEAPON COMBAT HIT SAD BLAST
	Priority        6
	Interrupts	<HitIRQ> HitBarf
	Include sequencers\Members\Reactions.inc
	NextMove        W_Hit_Barf_cycle

	Type		male
	  Anim          male\wep_quickbarf 1
	  PlayFX	Generic\Vomit.fx 25
	TEnd

	Type		fem
	  Anim          male\wep_quickbarf 1
	  PlayFX	Generic\Vomit_female.fx 25
	TEnd
MEnd

Move W_Hit_Barf_cycle
	Requires	WEAPON COMBAT
	Include sequencers\Members\Reactions.inc
	Flags		Cycle FixArmReg  ##HitReact

	Type		male
	  Anim          male\wep_barf_cycle 1 50
	TEnd
MEnd


#############################################################
## Hit Placate React
#############################################################

Move Hit_Placate
	Requires	HIT PLACATEHIT
	Priority        2
	Interrupts	<HitIRQ> FearStun_cycle FearStun
	Include sequencers\Members\Reactions.inc
	NextMove        Hit_Placate_cycle

	Type		male
	  Anim          male\lookquick 1
	TEnd
MEnd

Move Hit_Placate_cycle
	Include sequencers\Members\Reactions.inc
	Flags		Cycle

	Type		male
	  Anim          male\lookquick 1
	TEnd
MEnd


#############################################################
## #cage react\wriggle
#############################################################

Move cage_wriggle
	Requires	HIT WRIGGLE
	Priority        2
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions.inc
	Flags		ReqInputs
	NextMove        cage_wriggle2
	Scale           0.60

	Type		male
	  Anim          male\wriggle 1 17
	TEnd

	Type		huge
	  Anim          huge\wriggle 1 17
	TEnd

	Type		fem
	  Anim          fem\wriggle 1 17
	TEnd
MEnd

Move cage_wriggle2
	Requires	HIT WRIGGLE
	Include sequencers\Members\Reactions.inc
	Flags		Cycle
	NextMove        postReady
	Scale           0.60

	Type		male
	  Anim          male\wriggle 18
	TEnd

	Type		huge
	  Anim          huge\wriggle 18
	TEnd

	Type		fem
	  Anim          fem\wriggle 18
	TEnd
MEnd


#############################################################
## electro\pain react
#############################################################

Move electro_react
	Requires	HIT ENERGY
	Priority        2
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        postReady
	Flags		HitReact

	Type		male
	  Anim          male\electro 1
	TEnd
MEnd


#############################################################
## electro_pain_LONG
#############################################################

Move electro_react_Long
	Requires	STRONG HIT ENERGY
	Priority        3
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        electro_react_Long2
	Flags		HitReact

	Type		male
	  Anim          male\electro 1 115
	TEnd
MEnd

Move electro_react_Long2
	Include sequencers\Members\Reactions.inc
	NextMove        weak_post
	Flags		HitReact

	Type		male
	  Anim          male\electro 1 20
	TEnd
MEnd

#############################################################
## weak post
#############################################################

Move weak_post
	Include sequencers\Members\Reactions.inc
	Flags		HitReact

	Type		male
	  Anim          male\drain 40 100
	TEnd
MEnd

#############################################################
## drain react
#############################################################

Move Drain_react
	Requires	HIT DRAINED
	Priority        2
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        postReady
	Flags		HitReact

	Type		male
	  Anim          male\drain 1 100
	TEnd
MEnd


#############################################################
## sonic\psi pain react
#############################################################

Move sonic_react
	Requires	HIT PSIONIC
	Priority        2
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        weak_post
	Flags		HitReact

	Type		male
	  Anim          male\sonicpain 1
	TEnd
MEnd


#############################################################
## choke react
#############################################################

Move choke_react
	Requires	HIT MAGIC
	Priority        2
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        weak_post
	Flags		HitReact

	Type		male
	  Anim          male\choke 1
	  PlayFx        FEET\CHOKE1.fx
	TEnd

	Type		huge
	  Anim          huge\choke 1
	  PlayFx        FEET\CHOKE1.fx
	TEnd

	Type		fem
	  Anim          fem\choke 1
 	  PlayFx        FEET\CHOKE2.fx
	TEnd
MEnd


#############################################################
## Speed_attack react
#############################################################

Move Speed_react
	Requires	HIT SPEEDPUNCH
	Priority        6
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        weak_post
	Flags		HitReact NoSizeScale

	Type		male
	  Anim          male\speed_react 3 71
	TEnd
MEnd


#############################################################
## Kungfu_speed react
#############################################################

Move KungfuSpeed_react
	Requires	HIT KUNGFUSPEED
	Priority        2
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        KungfuSpeed_react2
	Flags		HitReact NoSizeScale

	Type		male
	  Anim          male\speed_react 3 39
	TEnd
MEnd

Move KungfuSpeed_react2
	Include sequencers\Members\Reactions.inc
	NextMove        KungfuSpeed_react3
	Flags		NoSizeScale

	Type		male
	  Anim          male\drain 40 52
	TEnd
MEnd

Move KungfuSpeed_react3
	Include sequencers\Members\Reactions.inc
	NextMove        postReady_back
	Flags		NoSizeScale

	Type		male
	  Anim          male\HQ_high 1
	TEnd
MEnd


#############################################################
## Claw_speed react
#############################################################

Move ClawsSpeed_react
	Requires	HIT CLAWSPEED
	Priority        2
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        ClawsSpeed_react2
	Flags		HitReact NoSizeScale

	Type		male
	  Anim          male\speed_react 3 35
	TEnd
MEnd

Move ClawsSpeed_react2
	Include sequencers\Members\Reactions.inc
	NextMove        ClawsSpeed_react3
	Flags		NoSizeScale

	Type		male
	  Anim          male\drain 40 41
	TEnd
MEnd

Move ClawsSpeed_react3
	Include sequencers\Members\Reactions.inc
	NextMove        postReady_back
	Flags		NoSizeScale

	Type		male
	  Anim          male\HQ_high 1
	TEnd
MEnd


#############################################################
## Dual Blades Speed Hit Reaction
#############################################################

Move DualBlade_SpeedHit
	Requires	HIT DUAL CLAWSPEED
	Priority        3
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        DualBlade_SpeedHit_Pause
	Flags		HitReact NoSizeScale

	Type		male
	  Anim          male\speed_react 3 53
	TEnd
MEnd

Move DualBlade_SpeedHit_Pause
	Include sequencers\Members\Reactions.inc
	NextMove        DualBlade_SpeedHit_Followup
	Flags		NoSizeScale

	Type		male
	  Anim          male\drain 40 57
	TEnd
MEnd

Move DualBlade_SpeedHit_Followup
	Include sequencers\Members\Reactions.inc
	NextMove        postReady_back
	Flags		NoSizeScale

	Type		male
	  Anim          male\HQ_high 1
	TEnd
MEnd


#############################################################
## Boost_up react
#############################################################

Move Hit_Boostup
	Requires	HIT BOOSTUP
	Priority        2
	Interrupts	<ReactIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        Ready

	Type		male
	  Anim          male\absorb 1
	TEnd
MEnd


#############################################################
## Boost_down react
#############################################################

Move hit_Boost_D
	Requires	HIT BOOSTDOWN
	Priority        2
	Interrupts	<ReactIRQ>
	Flags		HitReact
	NextMove        Ready
	Include sequencers\Members\Reactions.inc

	Type		male
	  Anim          male\boost 1
	TEnd
MEnd


#############################################################
## knockback Hit
#############################################################

Move KHF
	Requires	HIT KNOCKBACK
	Priority        5
	Interrupts	<HitIRQ> <DeathIRQ>
	NextMove        KHF_travel
	Interpolate     3
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\knockback_fierce 3 15
	  Ragdoll	1
	TEnd

	Type		huge
	  Anim          huge\knockback_fierce 3 15
	  Ragdoll	1
	TEnd

	Type		fem
	  Anim          fem\knockback_fierce 3 16
	  Ragdoll	1
	TEnd
MEnd

Move KHF_travel
	Requires	AIR
	NextMove        KHF_impact
	Sets            CANTMOVE
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\knockback_fierce 14 15
	  Ragdoll	1
	TEnd

	Type		huge
	  Anim          huge\knockback_fierce 15 16
	  Ragdoll	1
	TEnd

	Type		fem
	  Anim          fem\knockback_fierce 16 17
	  Ragdoll	1
	TEnd
MEnd

Move KHF_Apex
	Requires	AIR APEX
	Interrupts	KHF_travel KHF
	NextMove        KHF_fallcycle
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\knockback_fierce 17 24
	  Ragdoll	1
	TEnd

	Type		huge
	  Anim          huge\knockback_fierce 17 24
	  Ragdoll	1
	TEnd

	Type		fem
	  Anim          fem\knockback_fierce 18 25
	  Ragdoll	1
	TEnd
MEnd

Move KHF_fallcycle
	Requires	AIR
	Member		<RagdollBellyIRQ>
	NextMove        KHF_impact
	Sets            CANTMOVE
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\knockback_fierce 23 24
	  Ragdoll	1
	TEnd

	Type		huge
	  Anim          huge\knockback_fierce 24 25
	  Ragdoll	1
	TEnd

	Type		fem
	  Anim          fem\knockback_fierce 24 25
	  Ragdoll	1
	TEnd
MEnd

Move KHF_impact
	NextMove        getup_back
	Sets            CANTMOVE
	Flags		NoSizeScale
	Interpolate     0

	Type		male
	  Anim          male\knockback_fierce 25
	  PlayFx        FEET\MALEDEATH7.FX
	  Ragdoll	1
	TEnd

	Type		huge
	  Anim          huge\knockback_fierce 25
	  PlayFx        FEET\MALEDEATH7.FX
	  Ragdoll	1
	TEnd

	Type		fem
	  Anim          fem\knockback_fierce 27
	  PlayFx        FEET\MALEDEATH7.FX
	  Ragdoll	1
	TEnd
MEnd


#############################################################
### Slow default get up is for bad guys
#############################################################

Move getup_back
	Member		<RagdollBellyIRQ> #<ReactIRQ>
	NextMove        getup_back2
	Interpolate     0
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\getup_back 5 53
	TEnd

	Type		huge
	  Anim          huge\getup_back 5 53
	TEnd

	Type		fem
	  Anim          fem\getup_back 5 53
	TEnd
MEnd

Move getup_back2
	NextMove        Ready
	Include sequencers\Members\Reactions.inc
	Flags		NoSizeScale

	Type		male
	  Anim          male\getup_back 53 100
	TEnd

	Type		huge
	  Anim          huge\getup_back 53 100
	TEnd

	Type		fem
	  Anim          fem\getup_back 53 100
	TEnd
MEnd

#############################################################
## Player knockback
#############################################################

Move PlayerKnockback
	Requires	HIT KNOCKBACK PLAYER
	Priority        6
	Interrupts	<HitIRQ> <DeathIRQ>
	NextMove        PlayerKnockback_Travel
	Sets            CANTMOVE
	Flags		NoSizeScale
	Interpolate     3

	Type		male
	  Anim          male\knockback_fierce 3 15
	TEnd
MEnd

Move PlayerKnockback_Travel
	Requires	AIR
	NextMove        PlayerKnockback_Impact
	Sets            CANTMOVE
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\knockback_fierce 14 15
	TEnd
MEnd

Move PlayerKnockback_Apex
	Requires	AIR APEX
	Interrupts	PlayerKnockback PlayerKnockback_Travel
	NextMove        PlayerKnockback_FallCycle
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\knockback_fierce 17 24
	TEnd
MEnd

Move PlayerKnockback_FallCycle
	Requires	AIR
	NextMove        PlayerKnockback_Impact
	Sets            CANTMOVE
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\knockback_fierce 23 24
	TEnd
MEnd

Move PlayerKnockback_Impact
	NextMove        Quick_getup_back
	Interpolate     0
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\knockback_fierce 25
	  PlayFx        FEET\MALEDEATH7.FX
	TEnd
MEnd

#############################################################
## Sweeps
#############################################################

Move Sweep_low
	Requires	SWEEP LOW HIT
	Priority        3
	Interrupts	<HitIRQ> <DeathIRQ>
	NextMove        Sweep_impact
	Interpolate     0
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\sweep 3 31
	TEnd

	Type		huge
	  Anim          huge\sweep 3 31
	TEnd

	Type		fem
	  Anim          fem\sweep 3 31
	TEnd
MEnd

Move Sweep_impact
	Member		<DeathIRQ>
	NextMove        getup_back
	Interpolate     0
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\knockback_fierce 26
	  PlayFx        FEET\MALEDEATH7.FX
	TEnd

	Type		huge
	  Anim          huge\knockback_fierce 26
	  PlayFx        FEET\MALEDEATH6.FX
	TEnd

	Type		fem
	  Anim          fem\knockback_fierce 26
	  PlayFx        FEET\MALEDEATH7.FX
	TEnd
MEnd

#############################################################

Move Sweep_High
	Requires	SWEEP HIGH HIT
	Priority        3
	Interrupts	<HitIRQ> <DeathIRQ>
	NextMove        Sweep_impact
	Interpolate     0
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\sweep_flip 2 28
	  PlayFx        FEET\MALEDEATH7.FX
	TEnd

	Type		huge
	  Anim          huge\sweep_flip 2 28
	  PlayFx        FEET\MALEDEATH5.FX
	TEnd

	Type		fem
	  Anim          fem\sweep_flip 2 28
	  PlayFx        FEET\MALEDEATH7.FX
	TEnd
MEnd

#############################################################
## Stun
#############################################################

Move Stun
	Requires	STUN HIT
	Priority        2
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        Stun_cycle

	Type		male
	  Anim          male\stun_hit 2 96
	TEnd
MEnd

Move Stun_cycle
	Requires	STUN
	Include sequencers\Members\Reactions.inc
	Flags		Cycle
	Scale           0.50

	Type		male
	  Anim          male\stun_cycle 1 31
	TEnd
MEnd

#############################################################
## Sleep
#############################################################

Move Sleep
	Requires	STUN SLEEP HIT
	Priority        3
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        Sleep2
	Scale           0.40

	Type		male
	  Anim          male\sleep 1 12
	TEnd
MEnd

#############################################################

Move Sleep2
	Requires	STUN SLEEP
	Priority        2
	Interrupts	<ReactIRQ> <AttackIRQ> <HitIRQ> <Defiance>
	Include sequencers\Members\Reactions.inc
	NextMove        Sleep_cycle
	Scale           0.20
	Flags		ReqInputs

	Type		male
	  Anim          male\sleep 12 55
	TEnd
MEnd

Move Sleep_cycle
	Requires	STUN SLEEP
	Include sequencers\Members\Reactions.inc
	Flags		Cycle
	NextMove        Ready
	Scale           0.20

	Type		male
	  Anim          male\sleep 55 85
	TEnd
MEnd


#############################################################
## choke Stun
#############################################################

Move chokeStun
	Requires	STUN HIT CHOKESTUN
	Priority        3
	Interrupts	<HitIRQ> Stun Stun_cycle
	Include sequencers\Members\Reactions.inc
	NextMove        chokeStun_cycle

	Type		male
	  Anim          male\choke 1
	  PlayFx        FEET\CHOKE1.fx
	TEnd

	Type		huge
	  Anim          huge\choke 1
	  PlayFx        FEET\CHOKE1.fx
	TEnd

	Type		fem
	  Anim          fem\choke 1
	  PlayFx        FEET\CHOKE2.fx
	TEnd
MEnd

Move chokeStun_cycle
	Requires	STUN CHOKESTUN
	Priority        2
	Include sequencers\Members\Reactions.inc
	Flags		Cycle
	NextMove        weak_post

	Type		male
	  Anim          male\choke 1
	  PlayFx        FEET\CHOKE1.fx
	TEnd

	Type		huge
	  Anim          huge\choke 1
	  PlayFx        FEET\CHOKE1.fx
	TEnd

	Type		fem
	  Anim          fem\choke 1
	  PlayFx        FEET\CHOKE2.fx
	TEnd
MEnd


#############################################################
## Sonic Stun
#############################################################

Move SonicStun_hit
	Requires	STUN HIT SONICSTUN
	Priority        3
	Interrupts	<HitIRQ> Stun Stun_cycle
	Include sequencers\Members\Reactions.inc
	NextMove        SonicStun_cycle

	Type		male
	  Anim          male\sonicpain 1
	TEnd

	Type		huge
	  Anim          huge\sonic_pain 1
	TEnd

	Type		fem
	  Anim          fem\sonic_pain 1
	TEnd
MEnd

Move SonicStun_cycle
	Requires	STUN SONICSTUN
	Include sequencers\Members\Reactions.inc
	Flags		Cycle
	NextMove        weak_post

	Type		male
	  Anim          male\sonicpain 1
	TEnd

	Type		huge
	  Anim          huge\sonic_pain 1
	TEnd

	Type		fem
	  Anim          fem\sonic_pain 1
	TEnd
MEnd


#############################################################
## Electro Stun
#############################################################

Move ElectroStun_hit
	Requires	STUN HIT ELECTROSTUN
	Priority        3
	Interrupts	<HitIRQ> Stun Stun_cycle
	Include sequencers\Members\Reactions.inc
	NextMove        ElectroStun_cycle

	Type		male
	  Anim          male\electro 1
	TEnd

	Type		huge
	  Anim          huge\electro 1
	TEnd

	Type		fem
	  Anim          fem\electro 1
	TEnd
MEnd

Move ElectroStun_cycle
	Requires	STUN ELECTROSTUN
	Include sequencers\Members\Reactions.inc
	Flags		Cycle
	NextMove        weak_post

	Type		male
	  Anim          male\electro 1
	TEnd

	Type		huge
	  Anim          huge\electro 1
	TEnd

	Type		fem
	  Anim          fem\electro 1
	TEnd
MEnd


#############################################################
## Lasso Stun
#############################################################

Move LassoStun_hit
	Requires	STUN HIT LASSOSTUN
	Priority        3
	Interrupts	<HitIRQ> Stun Stun_cycle
	Include sequencers\Members\Reactions.inc
	NextMove        LassoStun_cycle
	Scale           0.60

	Type		male
	  Anim          male\wriggle 1 17
	TEnd

	Type		huge
	  Anim          huge\wriggle 1 17
	TEnd

	Type		fem
	  Anim          fem\wriggle 1 17
	TEnd
MEnd

Move LassoStun_cycle
	Requires	STUN LASSOSTUN
	Include sequencers\Members\Reactions.inc
	Flags		Cycle
	NextMove        weak_post
	Scale           0.60

	Type		male
	  Anim          male\wriggle 18
	TEnd

	Type		huge
	  Anim          huge\wriggle 18
	TEnd

	Type		fem
	  Anim          fem\wriggle 18
	TEnd
MEnd


#############################################################
## LockDown Stun
#############################################################

Move LockDown_hit
	Requires	STUN HIT LOCKDOWN
	Priority        3
	Interrupts	<HitIRQ> Stun Stun_cycle
	Include sequencers\Members\Reactions.inc
	NextMove        LockDown_cycle
	Scale           0.60

	Type		male
	  Anim          male\lockdown 1 17
	TEnd
MEnd

Move LockDown_cycle
	Requires	STUN LOCKDOWN
	Include sequencers\Members\Reactions.inc
	Flags		Cycle
	NextMove        weak_post
	Scale           0.60
	Interpolate     14

	Type		male
	  Anim          male\lockdown 18
	TEnd
MEnd

#############################################################
## Freeze Stun
#############################################################

Move FreezeStun_hit
	Requires	STUN STRONG HIT LASSOSTUN
	Priority        4
	Interrupts	<HitIRQ> Stun Stun_cycle
	Include sequencers\Members\Reactions.inc
	NextMove        FreezeStun_cycle
	SticksOnChild   STUN STRONG LASSOSTUN ##(for anim FX)

	Type		male
	  Anim          male\freezestun 1 20
	TEnd

	Type		huge
	  Anim          huge\freeze 1 20
	TEnd

	Type		fem
	  Anim          fem\freeze 1 20
	TEnd
MEnd

Move FreezeStun_cycle
	Requires	STUN STRONG LASSOSTUN
	Include sequencers\Members\Reactions.inc
	Flags		Cycle
	NextMove        weak_post
	SticksOnChild   STUN STRONG LASSOSTUN ##(for anim FX)

	Type		male
	  Anim          male\freezestun 20 30
	TEnd

	Type		huge
	  Anim          huge\freeze 20 30
	TEnd

	Type		fem
	  Anim          fem\freeze 20 30
	TEnd
MEnd

#############################################################
## MopUp Freeze Stun (will freeze after attack powers)
#############################################################

Move MopUp_FreezeStun_cycle
	Requires	STUN STRONG LASSOSTUN
	Priority        3
	Interrupts	<ReactIRQ>
	Include sequencers\Members\Reactions.inc
	Flags		Cycle
	NextMove        weak_post

	Type		male
	  Anim          male\freezestun 20 30
	TEnd

	Type		huge
	  Anim          huge\freeze 20 30
	TEnd

	Type		fem
	  Anim          fem\freeze 20 30
	TEnd
MEnd

#############################################################
## Levitate Stun
#############################################################

Move Lev_Stun
	Requires	STUN HIT FLOATSTUN
	Priority        3
	Interrupts	<HitIRQ> Stun Stun_cycle
	Include sequencers\Members\Reactions.inc
	NextMove        Lev_Stun_cycle

	Type		male
	  Anim          male\pre_fly 2
	TEnd

	Type		huge
	  Anim          huge\pre_fly 2
	TEnd

	Type		fem
	  Anim          fem\pre_fly 2
	TEnd
MEnd

Move Lev_Stun_cycle
	Requires	STUN FLOATSTUN
	Include sequencers\Members\Reactions.inc
	Flags		Cycle
	NextMove        JumpPost

	Type		male
	  Anim          male\float2 1
	TEnd

	Type		huge
	  Anim          huge\float 1
	TEnd

	Type		fem
	  Anim          fem\float 1
	TEnd
MEnd

#############################################################
## Lev_Stun Hit Quick
#############################################################

Move Lev_Stun_HitQuick
	Requires	HIT
	Priority        1
	Interrupts	Lev_Stun Lev_Stun_cycle
	Include sequencers\Members\Reactions.inc
	NextMove        Lev_Stun_cycle
	Flags		HitReact

	Type		male
	  Anim          male\float_HQ 2
	TEnd

	Type		huge
	  Anim          huge\float_HQ 2
	TEnd

	Type		fem
	  Anim          fem\float_HQ 2
	TEnd
MEnd


#############################################################
## Fear Stun
#############################################################

Move FearStun
	Requires	FEAR
	Priority        1
	Interrupts	<Ready> <BasicMove> Stun Stun_cycle
	Include sequencers\Members\Reactions.inc
	NextMove        FearStun_cycle

	Type		male
	  Anim          male\fear_stun 1
	TEnd

	Type		Wisp
	  Anim          male\air_fear_stun 1
	TEnd
MEnd

Move FearStun_cycle
	Requires	FEAR
	Include sequencers\Members\Reactions.inc
	Flags		Cycle
	Interpolate	8

	Type		male
	  Anim          male\fear_stun 25 60
	TEnd


	Type		Wisp
	  Anim          male\air_fear_stun 25 60
	TEnd
MEnd


#############################################################
## Widows Snare Stun
#############################################################

Move WidowsSnareStun
	Requires	STUN HIGH LASSOSTUN FLOATSTUN
	Priority        4
	Interrupts	<ReactIRQ> Stun Stun_cycle
	Member		<Defiance>
	NextMove        WidowsSnareStun_cycle

	Type		male
	  Anim          male\widows_snare 1 60
	TEnd
MEnd

Move WidowsSnareStun_cycle
	Requires	STUN HIGH LASSOSTUN FLOATSTUN
	Member		<Defiance>
	NextMove	WidowsSnareStun_fall
	Flags		Cycle

	Type		male
	  Anim          male\widows_snare 61
	TEnd
MEnd

Move WidowsSnareStun_fall
	Member		<DeathIRQ>
	NextMove        WidowsSnareStun_impact
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\widows_snare_post 1
	TEnd
MEnd

Move WidowsSnareStun_impact
	Member		<DeathIRQ>
	NextMove        WidowsSnareStun_getup
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\impact_belly 2
	TEnd

	Type		huge
	  Anim          huge\impact_belly 2
	TEnd

	Type		fem
	  Anim          fem\impact_belly 2
	TEnd
MEnd

Move WidowsSnareStun_getup
	Include sequencers\Members\Reactions.inc
	NextMove        KnockDown_cycle

	Type		male
	  Anim          male\getup_belly 1
	TEnd

	Type		huge
	  Anim          huge\getup_belly 1
	TEnd

	Type		fem
	  Anim          fem\getup_belly 1
	TEnd
MEnd

#############################################################
## Barf Stun
#############################################################

Move barf_Stun
	Requires	STUN BARFSTUN
	Priority        3
	Interrupts	<ReactIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        barf_Stun_Cycle_A
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\barf 1 139
	  PlayFX	Powers\PoisonControl\Vomit.fx 25
	TEnd

	Type		fem
	  Anim          male\barf 1 139
	  PlayFX	Powers\PoisonControl\Vomit_female.fx 25
	TEnd

	Type		huge
	  Anim          huge\quickbarf 1
	  PlayFX	Powers\PoisonControl\HugeVomit.fx 25
	TEnd
MEnd

Move barf_Stun_Cycle_A
	Requires	STUN BARFSTUN
	Include sequencers\Members\Reactions.inc
	Sets		CANTMOVE
	Flags		Cycle NoSizeScale
	CycleMove	barf_Stun_Cycle_B
	CycleMove	barf_Stun_Cycle_C

	Type		male
	  Anim          male\barf_cycle 1 50
	TEnd

	Type		huge
	  Anim          huge\barf_cycle 1 50
	TEnd
MEnd

Move barf_Stun_Cycle_B
	Requires	STUN BARFSTUN
	Include sequencers\Members\Reactions.inc
	Sets		CANTMOVE
	Flags		Cycle NoSizeScale
	CycleMove	barf_Stun_Cycle_A
	CycleMove	barf_Stun_Cycle_C

	Type		male
	  Anim          male\quickbarf 1
	  PlayFX	Powers\PoisonControl\Vomit.fx 25
	TEnd

	Type		fem
	  Anim          male\quickbarf 1
	  PlayFX	Powers\PoisonControl\Vomit_female.fx 25
	TEnd

	Type		huge
	  Anim          huge\quickbarf 1
	  PlayFX	Powers\PoisonControl\HugeVomit.fx 25
	TEnd
MEnd

Move barf_Stun_Cycle_C
	Requires	STUN BARFSTUN
	Include sequencers\Members\Reactions.inc
	Sets		CANTMOVE
	Flags		Cycle NoSizeScale
	CycleMove	barf_Stun_Cycle_B
	CycleMove	barf_Stun_Cycle_A
	Interpolate	8

	Type		male
	  Anim          male\barf 1 139
	  PlayFX	Powers\PoisonControl\Vomit.fx 25
	TEnd

	Type		fem
	  Anim          male\barf 1 139
	  PlayFX	Powers\PoisonControl\Vomit_female.fx 25
	TEnd

	Type		huge
	  Anim          huge\quickbarf 1
	  PlayFX	Powers\PoisonControl\HugeVomit.fx 25
	TEnd
MEnd


#############################################################
## Knock Down
#############################################################

Move KnockDown
	Requires	HIT KNOCKDOWN
	Priority        2
	Interrupts	<HitIRQ>
	Member		<DeathIRQ>
	NextMove        KnockDown2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\impact_belly 2
	TEnd

	Type		huge
	  Anim          huge\impact_belly 2
	TEnd

	Type		fem
	  Anim          fem\impact_belly 2
	TEnd
MEnd

Move KnockDown2
	Include sequencers\Members\Reactions.inc
	NextMove        KnockDown_cycle
	Sets            CANTMOVE

	Type		male
	  Anim          male\getup_belly 1
	TEnd

	Type		huge
	  Anim          huge\getup_belly 1
	TEnd

	Type		fem
	  Anim          fem\getup_belly 1
	TEnd
MEnd

Move KnockDown_cycle
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	Scale           0.50
	Interpolate     0

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
## Raise Dead
#############################################################

Move RaiseDead_Pre
	Requires	RAISEDEAD PLAYER
	Priority        2
	Interrupts	<ReactIRQ> <RaiseDeadIRQ> <RaiseDeadBackIRQ>
	NextMove        RaiseDead
	Flags		NoSizeScale
	Scale           0.75 ##Scale is set so that sequence takes 4 seconds
	##Sets		CANTMOVE

	Type		male
	  Anim          male\raisedead 1 91
	TEnd

	Type		huge
	  Anim          huge\raisedead 1 91
	TEnd

	Type		fem
	  Anim          fem\raisedead 1 91
	TEnd
MEnd

Move RaiseDead
	Include sequencers\Members\Reactions.inc
	NextMove        PreFall
	Flags		NoSizeScale
	Scale           0.75

	Type		male
	  Anim          male\raisedead 91 98
	TEnd

	Type		huge
	  Anim          huge\raisedead 91 98
	TEnd

	Type		fem
	  Anim          fem\raisedead 91 98
	TEnd
MEnd

## RAISE DEAD - CRITTER ###########################################################

Move RaiseDead_Critter
	Requires	RAISEDEAD
	Interrupts	<ReactIRQ> <RaiseDeadIRQ> <RaiseDeadBackIRQ>
	Sets		CANTMOVE
	Flags		NoSizeScale NotSelectable

	Type		male
	  Anim          male\raisedead 1
	TEnd

	Type		huge
	  Anim          huge\raisedead 1
	TEnd

	Type		fem
	  Anim          fem\raisedead 1
	TEnd
MEnd


#############################################################
## Revive(GetUp) - From Belly
#############################################################

Move Revive_belly_Pre
	Requires	HIT GETUP
	Priority        2
	Interrupts	<RaiseDeadIRQ> <RaiseDeadBackIRQ> #<ReactIRQ>
	NextMove        Revive_belly_post
	Flags		NoSizeScale NotSelectable
	Sets		CANTMOVE

	Type		male
	  Anim          male\getup_belly 1 120
	TEnd

	Type		huge
	  Anim          huge\getup_belly 1 120
	TEnd

	Type		fem
	  Anim          fem\getup_belly 1 120
	TEnd
MEnd

Move Revive_belly_post
	Include sequencers\Members\Readies.inc
	NextMove        KnockDown_cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\getup_belly 120
	TEnd

	Type		huge
	  Anim          huge\getup_belly 120
	TEnd

	Type		fem
	  Anim          fem\getup_belly 120
	TEnd
MEnd


#############################################################
## Revive - NPC version (Rooted for 4 seconds)
#############################################################

Move Revive_belly_Pre_rooted
	Requires	HIT GETUP LOCKDOWN
	Priority        3
	Interrupts	<ReactIRQ> <RaiseDeadIRQ> <RaiseDeadBackIRQ>
	NextMove        Revive_belly_post
	Flags		NoSizeScale NotSelectable
	Sets		CANTMOVE

	Type		male
	  Anim          male\getup_belly 1 120
	TEnd

	Type		huge
	  Anim          huge\getup_belly 1 120
	TEnd

	Type		fem
	  Anim          fem\getup_belly 1 120
	TEnd
MEnd


#############################################################
## Revive(GetUp) - From Back
#############################################################

Move Revive_back
	Requires	HIT GETUP
	Priority        2
	Interrupts	<RaiseDeadBackIRQ>
	Flags		NotSelectable
	Sets            CANTMOVE

	Type		male
	  Anim          male\getup_back 5 66
	TEnd

	Type		huge
	  Anim          huge\getup_back 5 66
	TEnd

	Type		fem
	  Anim          fem\getup_back 5 66
	TEnd
MEnd


#############################################################
## Ragdoll Belly getup
#############################################################

Move Ragdoll_belly
	Requires	RAGDOLLBELLY
	Priority        1
	Interrupts	<ReactIRQ> <DeathIRQ> <RagdollBellyIRQ>
	NextMove        KnockDown_cycle
	Sets            CANTMOVE

	Type		male
	  Anim          male\getup_belly 1
	TEnd

	Type		huge
	  Anim          huge\getup_belly 1
	TEnd

	Type		fem
	  Anim          fem\getup_belly 1
	TEnd
MEnd


#############################################################
## Reclaim (reclaimationpad)
#############################################################

Move Reclaim
	Requires	RECLAIM EMERGE
	Priority        7
	Interrupts	<DeathIRQ> <RaiseDeadIRQ> <AttackIRQ> <BasicMove> <CombatMove> <WepMove> <TwoHandMove> <Ready> <PostMoveCycle> <Combat> <WepCombat> <TwoHandCombat> <RunIn>
	NextMove        Reclaim2
	Sets            CANTMOVE
	Flags		NOTSELECTABLE

	Type		male
	  Anim          male\drain 100 160
	  PlayFx	POWERS\Teleport\teleport.fx  50
	  PlayFx	Doors\reclPadPlayerInvisible.fx
	  PlayFx	Doors\reclPadPlayerFadeIn.fx
	TEnd

	Type		huge
	  Anim          huge\drain 100 160
	  PlayFx	POWERS\Teleport\teleport.fx  50
	  PlayFx	Doors\reclPadPlayerInvisible.fx
	  PlayFx	Doors\reclPadPlayerFadeIn.fx
	TEnd

	Type		fem
	  Anim          fem\drain 100 160
	  PlayFx	POWERS\Teleport\teleport.fx  50
	  PlayFx	Doors\reclPadPlayerInvisible.fx
	  PlayFx	Doors\reclPadPlayerFadeIn.fx
	TEnd
MEnd

Move Reclaim2
	Include sequencers\Members\Reactions.inc
	Sets            CANTMOVE
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

##########################
###### BARF   ######
##########################

Move Barf
	Requires	SAD BLAST ATTACK
	Priority        3
	Interrupts	<ReactIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        Barf2
	Sets            CANTMOVE
	Flags		FixArmReg NoSizeScale
	Interpolate	15

	Type		male
	  Anim          male\barf 1 60
	  PlayFX	Generic\Vomit.fx 25
	TEnd

	Type		fem
	  Anim          male\barf 1 60
	  PlayFX	Generic\Vomit_female.fx 25
	TEnd

	Type		huge
	  Anim          huge\quickbarf 1 60
	  PlayFX	Generic\Vomit.fx 25
	TEnd
MEnd

Move Barf2
	Include sequencers\Members\Reactions.inc
	NextMove        Barf_cycle
	Flags		FixArmReg NoSizeScale

	Type		male
	  Anim          male\barf 60 139
	TEnd

	Type		huge
	  Anim          huge\quickbarf 60
	TEnd
MEnd

Move Barf_cycle
	Include sequencers\Members\Reactions.inc
	Flags		Cycle FixArmReg NoSizeScale
	NextMove        postReady_back

	Type		male
	  Anim          male\barf_cycle 1 50
	TEnd

	Type		huge
	  Anim          huge\barf_cycle 1 50
	TEnd
MEnd

#############################################################

Move QuickBarf
	Requires	SAD BLAST ATTACK
	Priority        10
	Interrupts	Barf_cycle
	Include sequencers\Members\Reactions.inc
	NextMove        QuickBarf2
	Sets            CANTMOVE
	Flags		FixArmReg NoSizeScale
	Interpolate	0

	Type		male
	  Anim          male\quickbarf 1 60
	  PlayFX	Generic\Vomit.fx 25
	TEnd

	Type		fem
	  Anim          male\quickbarf 1 60
	  PlayFX	Generic\Vomit_female.fx 25
	TEnd

	Type		huge
	  Anim          huge\quickbarf 1 60
	  PlayFX	Generic\Vomit.fx 25
	TEnd
MEnd

Move QuickBarf2
	Include sequencers\Members\Reactions.inc
	NextMove        Barf_cycle
	Sets            CANTMOVE
	Flags		FixArmReg NoSizeScale

	Type		male
	  Anim          male\quickbarf 60
	TEnd

	Type		huge
	  Anim          huge\quickbarf 60
	TEnd
MEnd

###############################
###### Fallout (after death)  ######
##############################

Move Fallout
	Requires	STRONG RAISEDEAD
	Priority        2
	Interrupts	<RaiseDeadIRQ> <RaiseDeadBackIRQ> <ReactIRQ>
	NextMove        KHF_Deathcycle
	Sets            NOCOLLISION CANTMOVE
	Flags		NoSizeScale NotSelectable

	Type		male
	  Anim          male\fallout 1
	TEnd

	Type		huge
	  Anim          huge\fallout 1
	TEnd

	Type		fem
	  Anim          fem\fallout 1
	TEnd
MEnd