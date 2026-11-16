#############################################################
## REACTIONS.INC - Hits, Rez, Status Effects, etc.
#############################################################

#############################################################
## Hit Quick
#############################################################

Move HitQuick
	Requires	QUICK HIT
	Priority        2
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        Fake_CombatCycle_Half
	Flags		HitReact

	Type		male
	  Anim          male\clockwork_HQ_high 2
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_HitReact_SFX.fx
	TEnd
MEnd

#############################################################
## Combat Hit Quick
#############################################################

Move C_HitQuick
	Requires	COMBAT QUICK HIT
	Priority        3
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        Wrist_Ready_cycle
	Flags		HitReact

	Type		male
	  Anim          male\clockwork_HQ_high_wrist 2
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_HitReact_SFX.fx
	TEnd
MEnd

#############################################################

Move C_HitQuick_mid
	Requires	COMBAT QUICK HIT
	Priority        3
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        Wrist_Ready_cycle
	Flags		HitReact

	Type		male
	  Anim          male\clockwork_HQ_mid_wrist 2
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_HitReact_SFX.fx
	TEnd
MEnd

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
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_HitReact_SFX.fx
	  PlayFX	Generic\Vomit.fx 25
	TEnd
MEnd

Move Hit_Barf_cycle
	Include sequencers\Members\Reactions.inc
	Flags		Cycle FixArmReg

	Type		male
	  Anim          male\barf_cycle 1 50
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
	  Anim          male\clockwork_drain 40 100
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
	  Anim          male\clockwork_drain 1 100
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
	Priority        2
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        weak_post
	Flags		HitReact NoSizeScale
	Interpolate	0

	Type		male
	  Anim          male\speed_react 1
	TEnd
MEnd

Move Speed_react2
	Requires	HIT SPEEDPUNCH
	Priority        2
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        weak_post
	Flags		HitReact NoSizeScale
	Interpolate	0

	Type		male
	  Anim          male\speed_react 21
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

	Type		huge
	  Anim          huge\speed_react 3 39
	TEnd

	Type		fem
	  Anim          fem\speed_react 3 39
	TEnd
MEnd

Move KungfuSpeed_react2
	Include sequencers\Members\Reactions.inc
	NextMove        KungfuSpeed_react3
	Flags		NoSizeScale

	Type		male
	  Anim          male\clockwork_drain 40 52
	TEnd

	Type		huge
	  Anim          huge\drain 40 52
	TEnd

	Type		fem
	  Anim          fem\drain 40 52
	TEnd
MEnd

Move KungfuSpeed_react3
	Include sequencers\Members\Reactions.inc
	NextMove        Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\HQ_high 1
	TEnd

	Type		huge
	  Anim          huge\HQ_high 2
	TEnd

	Type		fem
	  Anim          fem\combat_HQ 1
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

	Type		huge
	  Anim          huge\speed_react 3 35
	TEnd

	Type		fem
	  Anim          fem\speed_react 3 35
	TEnd
MEnd

Move ClawsSpeed_react2
	Include sequencers\Members\Reactions.inc
	NextMove        ClawsSpeed_react3
	Flags		NoSizeScale

	Type		male
	  Anim          male\clockwork_drain 40 41
	TEnd

	Type		huge
	  Anim          huge\drain 40 41
	TEnd

	Type		fem
	  Anim          fem\drain 40 41
	TEnd
MEnd

Move ClawsSpeed_react3
	Include sequencers\Members\Reactions.inc
	NextMove        postReady_back
	Flags		NoSizeScale

	Type		male
	  Anim          male\HQ_high 1
	TEnd

	Type		huge
	  Anim          huge\HQ_high 2
	TEnd

	Type		fem
	  Anim          fem\combat_HQ 1
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

	Type		huge
	  Anim          huge\speed_react 3 53
	TEnd

	Type		fem
	  Anim          fem\speed_react 3 53
	TEnd
MEnd

Move DualBlade_SpeedHit_Pause
	Include sequencers\Members\Reactions.inc
	NextMove        DualBlade_SpeedHit_Followup
	Flags		NoSizeScale

	Type		male
	  Anim          male\clockwork_drain 40 57
	TEnd

	Type		huge
	  Anim          huge\drain 40 57
	TEnd

	Type		fem
	  Anim          fem\drain 40 57
	TEnd
MEnd

Move DualBlade_SpeedHit_Followup
	Include sequencers\Members\Reactions.inc
	NextMove        postReady_back
	Flags		NoSizeScale

	Type		male
	  Anim          male\HQ_high 1
	TEnd

	Type		huge
	  Anim          huge\HQ_high 2
	TEnd

	Type		fem
	  Anim          fem\combat_HQ 1
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
	  Anim          male\clockwork_absorb 2
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
	  Anim          male\clockwork_boost 1
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
	Interpolate     0
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\knockback_fierce 3 15
	  Ragdoll	1
	TEnd
MEnd

Move KHF_travel
	Requires	AIR
	Sets            CANTMOVE
	Flags		Cycle NoSizeScale
	#NextMove        KHF_impact
	NextMove        getup_back

	Type		male
	  Anim          male\knockback_fierce 14 15
	  Ragdoll	1
	TEnd
MEnd

Move KHF_Apex
	Requires	AIR APEX
	Interrupts	KHF KHF_travel
	NextMove        KHF_fallcycle
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\knockback_fierce 17 24
	  Ragdoll	1
	TEnd
MEnd

Move KHF_fallcycle
	Requires	AIR
	#NextMove        KHF_impact
	NextMove        getup_back
	Sets            CANTMOVE
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\knockback_fierce 23 24
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
	  #Ragdoll	1
	TEnd
MEnd


#############################################################
### Slow default get up is for bad guys
#############################################################

Move getup_back
	Member		<RagdollBellyIRQ> #<ReactIRQ>
	NextMove        getup_back2
	Interpolate     5
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\clockwork_getup_back 5 53
	TEnd
MEnd

Move getup_back2
	NextMove        Ready
	Include sequencers\Members\Reactions.inc
	Flags		NoSizeScale

	Type		male
	  Anim          male\clockwork_getup_back 53 100
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
	  Anim          male\clockwork_getup_belly 1
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
MEnd

Move ElectroStun_cycle
	Requires	STUN ELECTROSTUN
	Include sequencers\Members\Reactions.inc
	Flags		Cycle
	NextMove        weak_post

	Type		male
	  Anim          male\electro 1
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
	  Anim          male\wriggle 2 17
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
MEnd

Move Lev_Stun_cycle
	Requires	STUN FLOATSTUN
	Include sequencers\Members\Reactions.inc
	Flags		Cycle
	NextMove        JumpPost

	Type		male
	  Anim          male\float2 1
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
MEnd

Move FearStun_cycle
	Requires	FEAR
	Include sequencers\Members\Reactions.inc
	Flags		Cycle
	Interpolate	8

	Type		male
	  Anim          male\fear_stun 25 60
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
	  Anim          male\clockwork_getup_belly 1
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
	  Anim          male\clockwork_getup_belly 1
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

	Type		male
	  Anim          male\clockwork_getup_belly 1 100
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
	  Anim          male\clockwork_getup_belly 100
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
	  Anim          male\clockwork_getup_belly 1 100
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
	  Anim          male\clockwork_getup_back 5 66
	TEnd

	Type		huge
	  Anim          huge\getup_back 5 66
	TEnd

	Type		fem
	  Anim          fem\getup_back 5 66
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
	Sets            NOCOLLISION
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

#############################################################
##BRAWLING CUSTOM HIT REACTION
#############################################################

## Initial Strike Hit Quick #############################################

Move Clockwork_Initial_Strike_HQ
	Requires	BRAWLING QUICK HIT
	Priority        3
	Interrupts	<HitIRQ> FearStun_cycle FearStun
	NextMove        Fake_CombatCycle
	Flags		HitReact
#	SticksOnChild	QUICK HIT

	Type		male
	  Anim          male\brawling_initial_strike_HQ_clockwork 1
	TEnd

	Type		fem
	  Anim          fem\brawling_initial_strike_HQ 1
	TEnd

	Type		huge
	  Anim          huge\brawling_initial_strike_HQ 1
	TEnd
MEnd

Move Clockwork_C_Brawling_HitQuick
	Requires	COMBAT BRAWLING QUICK HIT
	Priority        4
	Interrupts	<HitIRQ> FearStun_cycle FearStun
	NextMove        CombatReady
	Flags		HitReact

	Type		male
	  Anim          male\brawling_initial_strike_HQ_clockwork 1
	TEnd

	Type		fem
	  Anim          fem\brawling_initial_strike_HQ 1
	TEnd

	Type		huge
	  Anim          huge\brawling_initial_strike_HQ 1
	TEnd

MEnd

## Spinning Strike Hit #############################################

Move Clockwork_Spinning_Strike_Hit
	Requires	BRAWLING HIT SPIN
	Priority        3
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        Fake_CombatCycle
	Flags		HitReact

	Type		male
	  Anim          male\brawling_spinning_strike_Hit_clockwork 1 53
	TEnd

	Type		fem
	  Anim          fem\brawling_spinning_strike_Hit 1 53
	TEnd

	Type		huge
	  Anim          huge\brawling_spinning_strike_Hit 1 53
	TEnd

MEnd

Move Clockwork_C_Spinning_Strike_Hit
	Requires	COMBAT BRAWLING HIT SPIN
	Priority        4
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        CombatReady
	Flags		HitReact

	Type		male
	  Anim          male\brawling_spinning_strike_Hit_clockwork 1 53
	TEnd

	Type		fem
	  Anim          fem\brawling_spinning_strike_Hit 1 53
	TEnd

	Type		huge
	  Anim          huge\brawling_spinning_strike_Hit 1 53
	TEnd

MEnd