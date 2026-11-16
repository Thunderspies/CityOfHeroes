#############################################################
##  HIT REACTIONS
#############################################################

## HIT QUICK ###########################################################

Move HitQuick
	Requires	QUICK HIT
	Priority        2
	Interrupts	<ReactIRQ>
	Member		<ReactIRQ> <DeathIRQ> <AttackIRQ> <HitIRQ>
	NextMove        CombatReady

	Type		5cw
	  Anim          5cw\HQ_high 1
	TEnd

	Type		Warworks_WarWalker
	  Anim          5cw\HQ_high 1
	  PlayFX		ENEMYFX\Praetorian\Warworks\Warwalker_HitReacts.fx
	TEnd

MEnd

Move HitQuick2
	Requires	QUICK HIT
	Priority        2
	Interrupts	<ReactIRQ>
	Member		<ReactIRQ> <DeathIRQ> <AttackIRQ> <HitIRQ>
	NextMove        CombatReady

	Type		5cw
	  Anim          5cw\HQ_mid 1
	TEnd

	Type		Warworks_WarWalker
	  Anim          5cw\HQ_mid 1
	  PlayFX		ENEMYFX\Praetorian\Warworks\Warwalker_HitReacts.fx
	TEnd

MEnd

Move HitQuick3
	Requires	QUICK HIT
	Priority        2
	Interrupts	<ReactIRQ>
	Member		<ReactIRQ> <DeathIRQ> <AttackIRQ> <HitIRQ>
	NextMove        CombatReady

	Type		5cw
	  Anim          5cw\goliath_warwalker_hit_react 1
	TEnd

	Type		Warworks_WarWalker
	  Anim          5cw\goliath_warwalker_hit_react 1
	  PlayFX		ENEMYFX\Praetorian\Warworks\Warwalker_HitReacts.fx
	TEnd

MEnd

Move HitQuick4
	Requires	QUICK HIT
	Priority        2
	Interrupts	<ReactIRQ>
	Member		<ReactIRQ> <DeathIRQ> <AttackIRQ> <HitIRQ>
	NextMove        CombatReady

	Type		5cw
	  Anim          5cw\goliath_warwalker_hit_react_long 1
	TEnd

	Type		Warworks_WarWalker
	  Anim          5cw\goliath_warwalker_hit_react_long 1
	  PlayFX		ENEMYFX\Praetorian\Warworks\Warwalker_HitReacts.fx
	TEnd

MEnd

###################
###### Weapon Hit Quick  ######
###################

Move W_HitQuick
	Requires	COMBAT WEAPON QUICK HIT
	Priority        4
	Interrupts	<ReactIRQ>
	Member		<DeathIRQ> <HitIRQ> <AttackIRQ> <HitIRQ>
	NextMove        WepReady

	Type		5cw
	  Anim          5cw\rikti_HQ 1
	TEnd
MEnd


##########################
###### Hit Placate React ######
##########################

Move Hit_Placate
	Requires	Hit PLACATEHIT
	Interrupts	<ReactIRQ> FearStun_cycle FearStun
	Member		<ReactIRQ> <DeathIRQ> <HitIRQ> <AttackIRQ>
	NextMove        Hit_Placate_cycle
##	Flags		HitReact
	Priority        2

	Type		5cw
	  Anim          male\lookquick 1
	TEnd
MEnd

Move Hit_Placate_cycle
	Member		<Ready> <ReactIRQ> <DeathIRQ> <AttackIRQ> <HitIRQ>
	Flags		Cycle
	Interpolate     0
##	Flags		HitReact

	Type		5cw
	  Anim          male\lookquick 1
	TEnd
MEnd

###################
###### knockback Hit  ######
###################

Move KHF
	Requires	HIT KNOCKBACK
	Interrupts	<ReactIRQ> <DeathIRQ>

	NextMove        KHF_travel
	Priority        5
	Interpolate     3

	Type		5cw
	  Anim          5cw\knockback_fierce 3 16
	  Ragdoll	1
	TEnd

MEnd

Move KHF_travel
	Requires	AIR
	Flags		Cycle
	NextMove        KHF_impact
	Priority        1

	Type		5cw
	  Anim          5cw\knockback_fierce 16 17
	  Ragdoll	1
	TEnd

MEnd

Move KHF_Apex
	Requires	AIR APEX
	Interrupts	KHF_travel KHF
	NextMove        KHF_fallcycle
	Priority        2

	Type		5cw
	  Anim          5cw\knockback_fierce 18 25
	  Ragdoll	1
	TEnd

MEnd

Move KHF_fallcycle
	Requires	AIR
	Flags		Cycle
	NextMove        KHF_impact
	Priority        1

	Type		5cw
	  Anim          5cw\knockback_fierce 25 26
	  Ragdoll	1
	TEnd

MEnd

Move KHF_impact
	NextMove        getup_back
	Interpolate     0
	Sets            CANTMOVE

	Type		5cw
	  Anim          5cw\knockback_fierce 27
	  PlayFx        FEET\BIGBODYFALL.FX
	  Ragdoll	1
	TEnd

MEnd

#############################################################

Move getup_back
	NextMove        getup_back2
	Interpolate     0
	Sets            CANTMOVE
	Member 		<ReactIRQ> <RagdollBellyIRQ>
	Interrupts	<ReactIRQ> <RaiseDeadIRQ> <RaiseDeadBackIRQ>

	Type		5cw
	  Anim          5cw\getup_back 5 7
	TEnd
MEnd

Move getup_back2
	NextMove        Ready
	Interpolate     0
	Sets            CANTMOVE
	Member 		<ReactIRQ> <DeathIRQ> <HitIRQ> <AttackIRQ> <RagdollBellyIRQ>

	Type		5cw
	  Anim          5cw\getup_back 8 100
	TEnd
MEnd

#############################################################

Move KHF_Deathcycle
	Requires	DEATH
	Member 		<RaiseDeadBackIRQ>
	Interrupts	getup_back Ragdoll_belly
	Flags		Cycle
	Priority        1
	Interpolate     0
	Sets            NOCOLLISION CANTMOVE DIENOW TARGET

	Type		5cw
	  Anim          5cw\knockback_fierce 71 75
	  Ragdoll	1
	TEnd
MEnd

###################
###### Impact Death   ######
###################

Move Impact_Death
	Requires	DEATH
	Interrupts	getup_back getup_back2
	NextMove        Impact_postdeath
	Priority        1
	Sets            NOCOLLISION

	Type		5cw
	  Anim          5cw\knockback_fierce 27
	  PlayFx        FEET\MALEDEATH4.FX
	  Ragdoll	0
	TEnd

	Type		clockwork_assembler
	  Anim          5cw\knockback_fierce 27
	  PlayFx        FEET\clockdeath1.FX
	  Ragdoll	0
	TEnd

	Type		clockwork_king
	  Anim          5cw\knockback_fierce 27
	  PlayFx        FEET\clockdeath2.FX
	  Ragdoll	0
	TEnd

	Type		Malta_Kronos
	  Anim          5cw\knockback_fierce 27
	  PlayFx        FEET\maltadeath1.FX
	  Ragdoll	0
	TEnd

	Type		Malta_Hercules
	  Anim          5cw\knockback_fierce 27
	  PlayFx        FEET\maltadeath3.FX
	  Ragdoll	0
	TEnd

	Type		BPTotem
	  Anim          5cw\death_front 3 122
	  PlayFx        FEET\HugeDeath2.fx
	  Ragdoll	0
	TEnd
MEnd

Move Impact_postdeath
	Requires	DEATH
	Flags		Cycle
	Priority        1
	Sets            NOCOLLISION DIENOW TARGET
	NextMove	getup_back

	Type		5cw
	  Anim          5cw\knockback_fierce 71 75
	  Ragdoll	0
	TEnd
MEnd



###############################
###### Ragdoll Belly getup   ######
##############################

Move Ragdoll_belly
	Requires	RAGDOLLBELLY
	Interrupts	<ReactIRQ> <DeathIRQ> <RagdollBellyIRQ>
	NextMove        KnockDown_cycle
	Priority        1
	Sets            CANTMOVE

	Type		5cw
	  Anim          Beast\getup_belly 1
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

	Type		5cw
	  Anim          Beast\getup_belly 1 100
	TEnd

MEnd

Move Revive_belly_post
	Include sequencers\Members\Readies.inc
	NextMove        KnockDown_cycle
	Flags		NoSizeScale

	Type		5cw
	  Anim          Beast\getup_belly 120
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

	Type		5cw
	  Anim          Beast\getup_belly 1 120
	TEnd

MEnd

###################
###### Knock Down  ######
###################

Move KnockDown
	Requires	HIT KNOCKDOWN
	Interrupts	<ReactIRQ>
	Member		<DeathIRQ>
	NextMove        KnockDown2
	Priority        2
	Sets            CANTMOVE

	Type		5cw
	  Anim          Beast\impact_belly 2
	TEnd
MEnd


Move KnockDown2
	Member		<DeathIRQ> <ReactIRQ>
	NextMove        KnockDown_cycle
	Sets            CANTMOVE

	Type		5cw
	  Anim          Beast\getup_belly 1
	TEnd
MEnd


Move KnockDown_cycle
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	Scale           0.50
	Interpolate     0

	Type		5cw
	  Anim          Beast\claw_cycle 1 31
	TEnd
MEnd


###################
###### Sweeps  ######
###################

Move Sweep_low
	Requires	SWEEP LOW HIT
	Interrupts	<ReactIRQ>
	Member		<DeathIRQ>
	NextMove        Sweep_impact
	Priority        3
	Interpolate     0
	Sets            CANTMOVE

	Type		5cw
	  Anim          5cw\sweep 3 31
	  PlayFx        FEET\MALEDEATH5.FX
	TEnd
MEnd


Move Sweep_impact
	Member		<DeathIRQ>
	NextMove        getup_back
	Interpolate     0
	Sets            CANTMOVE

	Type		5cw
	  Anim          5cw\impact_back 1
	  PlayFx        FEET\MALEDEATH6.FX
	TEnd
MEnd


Move Sweep_High
	Requires	SWEEP HIGH HIT
	Interrupts	<ReactIRQ>
	Member		<DeathIRQ>
	NextMove        Sweep_impact
	Priority        3
	Interpolate     0
	Sets            CANTMOVE

	Type		5cw
	  Anim          5cw\sweep_flip 2 28
	  PlayFx        FEET\MALEDEATH5.FX
	TEnd
MEnd

###################
###### Stun  ######
###################

Move Stun
	Requires	STUN HIT
	Interrupts	<ReactIRQ>
	Member		<DeathIRQ> <ReactIRQ> <AttackIRQ> <HitIRQ>
	NextMove        Stun_cycle
	Priority        2

	Type		5cw
	  Anim          5cw\stun_hit 2
	TEnd
MEnd


Move Stun_cycle
	Requires	STUN
	Member		<DeathIRQ> <ReactIRQ> <AttackIRQ> <HitIRQ>
	Flags		Cycle
	NextMove        Ready
	Priority        1
	Scale           0.50

	Type		5cw
	  Anim          5cw\stun_cycle 1 31
	TEnd
MEnd


###################
### Stun forward  #####
###################

Move Stun_forward
	Requires	STUN  Forward
	Interrupts	Stun Stun_cycle <BasicMove> <CombatMove> <WepMove> <TwoHandMove> dizzy <PostMoveCycle>
	Member		<ReactIRQ> <DeathIRQ>
	NextMove        Stun_cycle
	Priority        8
	Scale		0.5
	Flags		Cycle
	Interpolate	20

	Type		5cw
	  Anim          male\stun_walk 1 27 ##zombie_walk_armsdown 1 27
	TEnd

MEnd





###################
###### Sleep  ######
###################

Move Sleep
	Requires	STUN SLEEP HIT
	Interrupts	<ReactIRQ> Stun Stun_cycle
	Member		<DeathIRQ> <AttackIRQ> <HitIRQ>
	NextMove        Sleep_cycle
	Priority        3
	Scale           0.40

	Type		5cw
	  Anim          5cw\sleep 1 52
	TEnd
MEnd


Move Sleep_cycle
	Requires	STUN SLEEP
	Member		<DeathIRQ> <ReactIRQ> <AttackIRQ> <HitIRQ>
	Flags		Cycle
	NextMove        Ready
	Priority        2
	Scale           0.20

	Type		5cw
	  Anim          5cw\sleep 55 85
	TEnd
MEnd

###################
###### choke Stun  ######
###################

Move chokeStun
	Requires	STUN HIT CHOKESTUN
	Interrupts	<ReactIRQ> Stun Stun_cycle
	Member		<DeathIRQ> <AttackIRQ> <HitIRQ>
	NextMove        chokeStun_cycle
	Priority        3

	Type		5cw
	  Anim          5cw\electro 1
	TEnd
MEnd


Move chokeStun_cycle
	Requires	STUN CHOKESTUN
	Member		<DeathIRQ> <AttackIRQ> <HitIRQ>
	Flags		Cycle
	NextMove        weak_post
	Priority        2

	Type		5cw
	  Anim          5cw\electro 1
	TEnd
MEnd

###################
###### Sonic Stun  ######
###################

Move SonicStun_hit
	Requires	STUN HIT SONICSTUN
	Interrupts	<ReactIRQ> Stun Stun_cycle
	Member		<DeathIRQ> <AttackIRQ> <HitIRQ>
	NextMove        SonicStun_cycle
	Priority        3

	Type		5cw
	  Anim          5cw\electro 1
	TEnd
MEnd


Move SonicStun_cycle
	Requires	STUN SONICSTUN
	Member		<DeathIRQ> <AttackIRQ> <HitIRQ>
	Flags		Cycle
	NextMove        weak_post
	Priority        2

	Type		5cw
	  Anim          5cw\electro 1
	TEnd
MEnd

###################
###### Electro Stun  ######
###################

Move ElectroStun_hit
	Requires	STUN HIT ELECTROSTUN
	Interrupts	<ReactIRQ> Stun Stun_cycle
	Member		<DeathIRQ> <AttackIRQ> <HitIRQ>
	NextMove        ElectroStun_cycle
	Priority        3

	Type		5cw
	  Anim          5cw\electro 1
	TEnd
MEnd

Move ElectroStun_cycle
	Requires	STUN ELECTROSTUN
	Member		<DeathIRQ> <AttackIRQ> <HitIRQ>
	Flags		Cycle
	NextMove        weak_post
	Priority        2

	Type		5cw
	  Anim          5cw\electro 1
	TEnd
MEnd

###################
###### Lasso Stun  ######
###################

Move LassoStun_hit
	Requires	STUN HIT LASSOSTUN
	Interrupts	<ReactIRQ> Stun Stun_cycle
	Member		<DeathIRQ> <AttackIRQ> <HitIRQ>
	NextMove        LassoStun_cycle
	Priority        3
	Scale           0.60

	Type		5cw
	  Anim          5cw\wriggle 1 17
	TEnd
MEnd


Move LassoStun_cycle
	Requires	STUN LASSOSTUN
	Member		<DeathIRQ> <AttackIRQ> <HitIRQ>
	Flags		Cycle
	NextMove        weak_post
	Priority        2
	Scale           0.60

	Type		5cw
	  Anim          5cw\wriggle 18
	TEnd
MEnd

###################
###### Freeze Stun  ######
###################

Move FreezeStun_hit
	Requires	STUN STRONG HIT LASSOSTUN
	Interrupts	<ReactIRQ> Stun Stun_cycle
	Member		<DeathIRQ>
	NextMove        FreezeStun_cycle
	Priority        4

	Type		5cw
	  Anim          5cw\freezestun 1 20
	TEnd
MEnd


Move FreezeStun_cycle
	Requires	STUN STRONG LASSOSTUN
	Member		<DeathIRQ>
	Flags		Cycle
	NextMove        weak_post
	Priority        3

	Type		5cw
	  Anim          5cw\freezestun 20 30
	TEnd
MEnd


#############################################################
###### MopUp Freeze Stun (will freeze after attack powers)  ######
#############################################################

Move MopUp_FreezeStun_cycle
	Requires	STUN STRONG LASSOSTUN
	Interrupts	<ReactIRQ>
	Member		<DeathIRQ>
	Flags		Cycle
	NextMove        weak_post
	Priority        3

	Type		5cw
	  Anim          5cw\freezestun 20 30
	TEnd

MEnd



###################
###### Levitate Stun  ######
###################

Move Lev_Stun
	Requires	STUN HIT FLOATSTUN
	Interrupts	<ReactIRQ> Stun Stun_cycle
	Member		<DeathIRQ> <AttackIRQ> <HitIRQ>
	NextMove        Lev_Stun_cycle
	Priority        3

	Type		5cw
	  Anim          5cw\pre_fly 2
	TEnd
MEnd


Move Lev_Stun_cycle
	Requires	STUN FLOATSTUN
	Member		<DeathIRQ> <AttackIRQ> <HitIRQ>
	Flags		Cycle
	NextMove        JumpPost
	Priority        2

	Type		5cw
	  Anim          5cw\float 1
	TEnd
MEnd

###################
###### Lev_Stun Hit Quick  ######
###################

Move Lev_Stun_HitQuick
	Requires	HIT
	Interrupts	Lev_Stun Lev_Stun_cycle
	Member		<ReactIRQ> <DeathIRQ> <AttackIRQ> <HitIRQ>
	NextMove        Lev_Stun_cycle
	Priority        1

	Type		5cw
	  Anim          5cw\float_HQ 2
	TEnd
MEnd



###################
###### Fear Stun  ######
###################

Move FearStun
	Requires	STUN Fear
	Interrupts	<ReactIRQ> Stun Stun_cycle
	Member		<DeathIRQ> <AttackIRQ> <HitIRQ>
	NextMove        FearStun_cycle
	Priority        2

	Type		5cw
	  Anim          male\fear_stun 1
	TEnd

MEnd

Move FearStun_cycle
	Requires	STUN Fear
	Member		<DeathIRQ> <AttackIRQ> <HitIRQ>
	Flags		Cycle
	Priority        2
	Interpolate	8

	Type		5cw
	  Anim          male\fear_stun 25 60
	TEnd

MEnd





###################
###### electro\pain react  ######
###################

Move electro_react
	Requires	HIT ENERGY
	Interrupts	<ReactIRQ>
	Member		<DeathIRQ> <HitIRQ> <AttackIRQ> <HitIRQ>
	NextMove        postReady_back
	Priority        2

	Type		5cw
	  Anim          5cw\electro 1
	TEnd

MEnd

###################
###### electro_pain_LONG  ######
###################

Move electro_react_Long
	Requires	STRONG HIT ENERGY
	Interrupts	<ReactIRQ>
	Member		<DeathIRQ> <AttackIRQ> <HitIRQ>
	NextMove        electro_react_Long2
	Priority        3

	Type		5cw
	  Anim          5cw\electro 1 115
	TEnd

MEnd


Move electro_react_Long2
	Member		<DeathIRQ> <AttackIRQ> <HitIRQ>
	NextMove        weak_post

	Type		5cw
	  Anim          5cw\electro 1 20
	TEnd

MEnd

###################
###### weak post  ######
###################

Move weak_post
	Member		<ReactIRQ> <DeathIRQ> <HitIRQ> <AttackIRQ>
	NextMove        CombatReady

	Type		5cw
	  Anim          5cw\drain 40
	TEnd

MEnd

###################
###### drain react  ######
###################

Move Drain_react
	Requires	HIT DRAINED
	Interrupts	<ReactIRQ>
	Member		<DeathIRQ> <HitIRQ> <AttackIRQ>
	NextMove        postReady_back
	Priority        2

	Type		5cw
	  Anim          5cw\drain 1
	TEnd

MEnd

###################
###### sonic\psi pain react  ######
###################

Move sonic_react
	Requires	HIT PSIONIC
	Interrupts	<ReactIRQ>
	Member		<DeathIRQ> <HitIRQ> <AttackIRQ>
	NextMove        postReady_back
	Priority        2

	Type		5cw
	  Anim          5cw\electro 1
	TEnd
MEnd

###################
###### choke react  ######
###################

Move choke_react
	Requires	HIT MAGIC
	Interrupts	<ReactIRQ>
	Member		<DeathIRQ> <HitIRQ> <AttackIRQ>
	NextMove        postReady_back
	Priority        2

	Type		5cw
	  Anim          5cw\electro 1
	TEnd
MEnd

#############################################################

Move Speed_react
	Requires	HIT SPEEDPUNCH
	Interrupts	<ReactIRQ>
	Member		<DeathIRQ> <HitIRQ> <AttackIRQ>
	NextMove        weak_post
	Priority        2
	Scale           0.75

	Type		5cw
	  Anim          5cw\speed_react 3 34
	TEnd

MEnd

###################
###### Kungfu_speed react  ######
###################

Move KungfuSpeed_react
	Requires	HIT KUNGFUSPEED
	Interrupts	<ReactIRQ>
	Member		<DeathIRQ> <HitIRQ> <AttackIRQ>
	NextMove        KungfuSpeed_react2
	Priority        2
	Flags		NoSizeScale

	Type		5cw
	  Anim          5cw\speed_react 3 39
	TEnd
MEnd

Move KungfuSpeed_react2
	Member		<DeathIRQ> <HitIRQ> <AttackIRQ>
	NextMove        KungfuSpeed_react3
	Flags		NoSizeScale

	Type		5cw
	  Anim          5cw\drain 40 52
	TEnd
MEnd

Move KungfuSpeed_react3
	Member		<DeathIRQ> <HitIRQ> <AttackIRQ>
	NextMove        postReady_back
	Flags		NoSizeScale

	Type		5cw
	  Anim          5cw\HQ_high 3 25
	TEnd
MEnd

###################
###### Claw_speed react  ######
###################

Move ClawSpeed_react
	Requires	HIT CLAWSPEED
	Interrupts	<ReactIRQ>
	Member		<DeathIRQ> <HitIRQ> <AttackIRQ>
	NextMove        ClawSpeed_react2
	Priority        2
	Flags		NoSizeScale

	Type		5cw
	  Anim          5cw\speed_react 3 39
	TEnd
MEnd


Move ClawSpeed_react2
	Member		<DeathIRQ> <HitIRQ> <AttackIRQ>
	NextMove        ClawSpeed_react3
	Flags		NoSizeScale

	Type		5cw
	  Anim          5cw\drain 40 53
	TEnd
MEnd


Move ClawSpeed_react3
	Member		<DeathIRQ> <HitIRQ> <AttackIRQ>
	NextMove        postReady_back
	Flags		NoSizeScale

	Type		5cw
	  Anim          5cw\HQ_high 3 25
	TEnd
MEnd

###################
###### levitate react  ######
###################

Move lev_react
	Requires	HIT LEVITATE
	Interrupts	<ReactIRQ>
	Member		<DeathIRQ> <HitIRQ>
	Flags		Cycle
	NextMove        lev_fall
	Priority        2
	Scale           0.50

	Type		5cw
	  Anim          5cw\lev_float 1
	TEnd
MEnd


Move lev_fall
	Requires	AIR
	Member		<DeathIRQ>
	Flags		ReqInputs
	NextMove        lev_fall_B
	Priority        1

	Type		5cw
	  Anim          5cw\lev_fall 2 17
	TEnd
MEnd


Move lev_fall_B
	Requires	AIR
	Member		<DeathIRQ>
	Flags		Cycle
	NextMove        lev_splat
	Priority        1

	Type		5cw
	  Anim          5cw\lev_fall 24 49
	TEnd
MEnd


Move lev_splat
	Member		<DeathIRQ>
	NextMove        lev_getup

	Type		5cw
	  Anim          5cw\impact_belly 1
	  PlayFx        feet\BIGBODYFALL.FX
	TEnd
MEnd


##################
###### Boost_up react  ######
###################

Move Hit_Boostup
	Requires	HIT BOOSTUP
	Interrupts	<ReactIRQ>
	Member		<ReactIRQ> <DeathIRQ> <HitIRQ> <AttackIRQ>
	NextMove        postReady_back
	Priority        2

	Type		5cw
	  Anim          5cw\drain 1
	TEnd
MEnd

###################
###### Boost_down react  ######
###################

Move Boost_D
	Requires	HIT BOOSTDOWN
	Interrupts	<ReactIRQ>
	Member		<ReactIRQ> <DeathIRQ> <HitIRQ> <AttackIRQ>
	NextMove        postReady_back
	Priority        2

	Type		5cw
	  Anim          5cw\drain 1
	TEnd
MEnd

#######################
###### Quick Upgrade React  ######
########################

Move Hit_Upgrade_quick
	Requires	HIT REPAIR QUICK
	Priority        3
	Interrupts	<ReactIRQ> <DeathIRQ> <AttackIRQ>
	NextMove        Hit_Upgrade_quick_post
	Flags		CANTMOVE NoSizeScale
	Scale		1.5

	Type		5cw
	  Anim          5cw\upgrade 1 257
	  PlayFx        V_COV\Powers\Robotics\RoboHatch_upgrade_quick.fx 0 CONSTANT
	  PlayFx        FEET\MM_Bot_FF_Left.fx 25
	  PlayFx        FEET\MM_Bot_FF_Right.fx 49
	  PlayFx        FEET\MM_Bot_FF_Left.fx 240
	  PlayFx        FEET\MM_Bot_FF_Right.fx 241
	TEnd
MEnd

Move Hit_Upgrade_quick_post
	Include sequencers\Members\Readies.inc
	NextMove	Ready

	Type		5cw
	  Anim          5cw\upgrade 258
	  PlayFx        FEET\MM_Bot_FF_Left.fx 72
	  PlayFx        FEET\MM_Bot_FF_Right.fx 48
	TEnd
MEnd

###################
###### Air Hit Quick  ######
###################

Move A_HitQuick
	Requires	FLY QUICK HIT
	Priority        3
	Interrupts	<ReactIRQ> A_FearStun_cycle A_FearStun
	Member		<DeathIRQ> <HitIRQ> <AttackIRQ>
	NextMove        C_flycycle
	Flags		HitReact

	Type		5cw
	  Anim          male\air_combat_HQ 2
	TEnd

MEnd

###################
###### Air Combat Hit Quick  ######
###################

Move A_C_HitQuick
	Requires	FLY COMBAT QUICK HIT
	Priority        4
	Interrupts	<ReactIRQ>
	Member		<DeathIRQ> <HitIRQ> <AttackIRQ>
	NextMove        C_flycycle
	Flags		HitReact

	Type		5cw
	  Anim          male\air_combat_HQ 2
	TEnd

MEnd

Move A_C_HitQuick_L
	Requires	FLY COMBAT QUICK HIT
	Priority        4
	Interrupts	<ReactIRQ>
	Member		<DeathIRQ> <HitIRQ> <AttackIRQ>
	NextMove        C_flycycle
	Flags		HitReact

	Type		5cw
	  Anim          male\air_combat_HQ 2
	TEnd
MEnd

###################
###### Air Weapon Hit Quick  ######
###################

Move A_W_HitQuick
	Requires	FLY COMBAT WEAPON QUICK HIT
	Priority        6
	Interrupts	<ReactIRQ>
	Member		<DeathIRQ> <HitIRQ> <AttackIRQ>
	NextMove        W_flycycle
	Flags		HitReact

	Type		5cw
	  Anim          male\air_wep_HQ 1
	TEnd
MEnd

###################
###### Air cage react\wriggle  ######
###################

Move A_cage_wriggle
	Requires	FLY HIT WRIGGLE
	Interrupts	<ReactIRQ>
	Member		<ReactIRQ> <DeathIRQ> <HitIRQ> <AttackIRQ>
	Flags		ReqInputs
	NextMove        A_cage_wriggle2
	Priority        3
	Scale           0.60

	Type		5cw
	  Anim          male\air_wriggle 1 17
	TEnd
MEnd

Move A_cage_wriggle2
	Requires	FLY HIT WRIGGLE
	Priority        3
	Member		<ReactIRQ> <DeathIRQ> <HitIRQ> <AttackIRQ>
	Flags		Cycle
	NextMove        postAir_C_W
	Scale           0.60

	Type		5cw
	  Anim          male\air_wriggle 18
	TEnd
MEnd

###################
###### Air electro\pain react  ######
###################

Move A_electro_react
	Requires	FLY HIT ENERGY
	Interrupts	<ReactIRQ>
	Member		<ReactIRQ> <DeathIRQ> <HitIRQ> <AttackIRQ>
	NextMove        A_weak_post
	Flags		HitReact
	Priority        3

	Type		5cw
	  Anim          male\air_electro 1
	TEnd

MEnd

###################
###### Air weak post  ######
###################

Move A_weak_post
	Requires	FLY
	Member		<ReactIRQ> <DeathIRQ> <HitIRQ> <AttackIRQ>
	NextMove        postAir_C_W
	Priority        1

	Type		5cw
	  Anim          male\air_drain 40
	TEnd

MEnd

###################
###### Air drain react  ######
###################

Move A_Drain_react
	Requires	FLY HIT DRAINED
	Interrupts	<ReactIRQ>
	Member		<ReactIRQ> <DeathIRQ> <HitIRQ> <AttackIRQ>
	NextMove        postAir_C_W
	Flags		HitReact
	Priority        3

	Type		5cw
	  Anim          male\air_drain 1
	TEnd


MEnd

###################
###### Air sonic\psi pain react  ######
###################

Move A_sonic_react
	Requires	FLY HIT PSIONIC
	Interrupts	<ReactIRQ>
	Member		<ReactIRQ> <DeathIRQ> <HitIRQ> <AttackIRQ>
	NextMove        A_weak_post
	Priority        3

	Type		5cw
	  Anim          male\air_sonicpain 1
	TEnd
MEnd

###################
###### Air choke react  ######
###################

Move A_choke_react
	Requires	FLY HIT MAGIC
	Interrupts	<ReactIRQ>
	Member		<ReactIRQ> <DeathIRQ> <HitIRQ> <AttackIRQ>
	NextMove        A_weak_post
	Flags		HitReact
	Priority        3

	Type		5cw
	  Anim          male\air_choke 1
	TEnd
MEnd

###################
###### Air Speed_attack react  ######
###################

Move A_Speed_react
	Requires	FLY HIT SPEEDPUNCH
	Interrupts	<ReactIRQ>
	Member		<ReactIRQ> <DeathIRQ> <HitIRQ> <AttackIRQ>
	NextMove        A_weak_post
	Flags		HitReact
	Priority        3

	Type		5cw
	  Anim          male\air_speedreact 3
	TEnd
MEnd

###################
###### Air Kungfu_speed react  ######
###################

Move A_KungfuSpeed_react
	Requires	FLY HIT KUNGFUSPEED
	Interrupts	<ReactIRQ>
	Member		<ReactIRQ> <DeathIRQ> <HitIRQ> <AttackIRQ>
	NextMove        A_weak_post
	Flags		HitReact
	Priority        3

	Type		5cw
	  Anim          male\air_speedreact 3
	TEnd
MEnd

###################
###### Air Claw_speed react  ######
###################

Move A_ClawSpeed_react
	Requires	FLY HIT CLAWSPEED
	Interrupts	<ReactIRQ>
	Member		<ReactIRQ> <DeathIRQ> <HitIRQ> <AttackIRQ>
	NextMove        A_weak_post
	Flags		HitReact
	Priority        3

	Type		5cw
	  Anim          male\air_speedreact 3
	TEnd
MEnd

##################
###### Air Boost_up react  ######
###################

Move A_Hit_Boostup
	Requires	FLY HIT BOOSTUP
	Flags		HitReact
	Interrupts	<ReactIRQ>
	Member		<ReactIRQ> <DeathIRQ> <HitIRQ> <AttackIRQ>
	NextMove        C_flycycle
	Priority        3

	Type		5cw
	  Anim          male\air_boostup 1
	TEnd
MEnd

Move A_hit_Boost_D
	Requires	FLY HIT BOOSTDOWN
	Interrupts	<ReactIRQ>
	Member		<ReactIRQ> <DeathIRQ> <HitIRQ> <AttackIRQ>
	NextMove        C_flycycle
	Flags		HitReact
	Priority        3

	Type		5cw
	  Anim          male\air_boostdown 1
	TEnd
MEnd

###################
###### Air Fear Stun  ######
###################

Move A_FearStun
	Requires	FLY Fear
	Interrupts	<ReactIRQ> Stun Stun_cycle
	Member		<DeathIRQ> <AttackIRQ> <HitIRQ>
	NextMove        A_FearStun_cycle
	Priority        3

	Type		5cw
	  Anim          male\air_fear_stun 1
	TEnd
MEnd

Move A_FearStun_cycle
	Requires	FLY Fear
	Member		<DeathIRQ> <AttackIRQ> <HitIRQ>
	Flags		Cycle
	Priority        3
	Interpolate	8

	Type		5cw
	  Anim          male\air_fear_stun 25 60
	TEnd
MEnd

###################
###### Air knockback Hit  ######
###################

Move A_KHF
	Requires	FLY HIT KNOCKBACK
	Interrupts	<ReactIRQ> <DeathIRQ> KHF
	NextMove        flyready
	Priority        8

	Type		5cw
	  Anim          male\air_KB_quick 1
	TEnd
MEnd

###################
###### Air Sweeps  ######
###################

Move A_Sweep_low
	Requires	SWEEP LOW HIT FLY
	Interrupts	<ReactIRQ> <DeathIRQ> Sweep_low
	NextMove        flyready
	Priority        4

	Type		5cw
	  Anim          male\air_KB_quick 1
	TEnd
MEnd

Move A_Sweep_High
	Requires	SWEEP HIGH HIT FLY
	Interrupts	<ReactIRQ> <DeathIRQ> Sweep_Highk
	NextMove        flyready
	Priority        4

	Type		5cw
	  Anim          male\air_KB_quick 1
	TEnd
MEnd

#############################################################
## MASTERMIND HENCHMEN
#############################################################

#######################
###### Upgrade React  ######
#######################

Move Hit_Upgrade
	Requires	HIT REPAIR
	Priority        2
	Interrupts	<ReactIRQ> <DeathIRQ> <AttackIRQ>
	NextMove        Ready
	Flags		CANTMOVE NoSizeScale

	Type		5cw
	  Anim          5cw\upgrade 1
	  PlayFx        V_COV\Powers\Robotics\RoboHatch_upgrade.fx 0 CONSTANT
	  PlayFx        FEET\MM_Bot_FF_Left.fx	25
	  PlayFx        FEET\MM_Bot_FF_Right.fx	49
	  PlayFx        FEET\MM_Bot_FF_Left.fx	240
	  PlayFx        FEET\MM_Bot_FF_Right.fx	241
	  PlayFx        FEET\MM_Bot_FF_Left.fx	329
	  PlayFx        FEET\MM_Bot_FF_Right.fx	302
	TEnd
MEnd