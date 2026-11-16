#########################################################
## READIES.INC - Modified 'ready' sequences
#########################################################

## READY 1 #######################################################

Move Ready
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	CycleMove	Ready2

	Type		male
	  Anim          male\Skel_ready2 1
	TEnd
MEnd


Move postReady
	Include sequencers\Members\Readies.inc
	NextMove        Ready
	Scale           0.80
	Interpolate     8

	Type		male
	  Anim          male\post_ready 7 24
	TEnd
MEnd


Move postReady_back
	Include sequencers\Members\Readies.inc
	NextMove        Ready

	Type		male
	  Anim          male\postback 3
	TEnd
MEnd


Move postReady_right
	Include sequencers\Members\Readies.inc
	NextMove        Ready

	Type		male
	  Anim          male\postback_right 1
	TEnd
MEnd


## READY 2 #######################################################

Move Ready2
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	CycleMove	Ready2
	CycleMove	Ready_Look
	#CycleMove	Ready_2Hips

	Type		male
	  Anim          male\Skel_ready2 1
	TEnd
MEnd

## READY LOOK #######################################################

Move Ready_Look
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	CycleMove	Ready2
	#CycleMove	Ready_2Hips
	Scale		0.5

	Type		male
	  Anim          male\idle_look 1
	TEnd
MEnd

#### READY HIPS #######################################################
##
##Move Ready_2Hips
##	Include sequencers\Members\Readies.inc
##	Flags		Cycle FIXARMREG
##	CycleMove	Ready_2Hips_look
####	Scale		0.5
##	Interpolate	15
##
##	Type		male
##	  Anim          male\stand_2Hhips 1 64
##	TEnd
##MEnd
##
#### READY HIPS LOOK #######################################################
##
##Move Ready_2Hips_look
##	Include sequencers\Members\Readies.inc
##	Flags		Cycle FIXARMREG
##	CycleMove	Ready_Xarms
##	CycleMove	Ready_2Hips_post
##	CycleMove	Ready_Xarms
####	Scale		0.5
##
##	Type		male
##	  Anim          male\idle_2hips_look 1
##	TEnd
##MEnd
##
##Move Ready_2Hips_post
##	Include sequencers\Members\Readies.inc
##	Flags		Cycle FIXARMREG
##	CycleMove	Ready_Look
####	Scale		0.5
##	Interpolate	15
##
##	Type		male
##	  Anim          male\stand_2Hhips 109
##	TEnd
##MEnd
##
##
#### READY HIPS CYCLE #######################################################
##
##Move Hips_Cycle
##	Include sequencers\Members\Readies.inc
##	Flags		Cycle
####	Interpolate     0
##
##	Type		male
##	  Anim          male\thumbsup 99 100
##	TEnd
##MEnd
##
##
#### READY CROSS ARMS #######################################################
##
##Move Ready_Xarms
##	Include sequencers\Members\Readies.inc
##	Flags		Cycle FIXARMREG
##	CycleMove	Ready_Xarms_look
####	Scale		0.6
##	Interpolate	15
##
##	Type		male
##	  Anim          male\stand_Xarms 1 68
##	TEnd
##MEnd
##
#### READY CROSS ARMS LOOK #######################################################
##
##Move Ready_Xarms_look
##	Include sequencers\Members\Readies.inc
##	Flags		Cycle FIXARMREG
##	CycleMove	Ready_Xarms_post
##	CycleMove	Ready_Xarms_look
####	Scale		0.5
##
##	Type		male
##	  Anim          male\idle_xarms_look 1
##	TEnd
##MEnd
##
##Move Ready_Xarms_post
##	Include sequencers\Members\Readies.inc
##	Flags		Cycle FIXARMREG
##	CycleMove	Ready_2Hips_look
####	Scale		0.8
##	Interpolate	15
##
##	Type		male
##	  Anim          male\stand_Xarms 90
##	TEnd
##MEnd
##
##
#### READY CROSS ARMS CYCLE #######################################################
##
##Move Xarms_Cycle
##	Include sequencers\Members\Readies.inc
##	Flags		Cycle
####	Interpolate     0
##
##	Type		male
##	  Anim          male\no 110 115
##	TEnd
##MEnd

## THREAT READY #######################################################

Move Threat_Cycle
	Include sequencers\Members\Readies.inc
	Flags		Cycle

	Type		male
	  Anim          male\threat_cycle 1
	TEnd
MEnd


## ACTION READY #######################################################

Move Action_Cycle
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	Scale           0.50

	Type		male
	  Anim          male\action_cycle 1 31
	TEnd
MEnd

## CHARACTER CREATION READY #######################################################

Move BodyReady
	Requires	PROFILE
	Priority        7
	Interrupts	<Ready>
	Include sequencers\Members\Readies.inc
	Flags		Cycle

	Type		male
	  Anim          male\Skel_ready2 1
	TEnd
MEnd

## COSTUME READY #######################################################

Move CostumeReady
	Requires	PROFILE HIGH
	Interrupts	<Ready> BodyReady
	Include sequencers\Members\Readies.inc
	Flags		Cycle

	Type		male
	  Anim          male\costume_ready 1
	TEnd
MEnd

## PROP READY 1 #######################################################

Move Prop_ready_1
	Requires	PROP
	Priority        1
	Interrupts	Ready
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	Interpolate	8

	Type		male
	  Anim          male\shotgun_observe_L 1
	  PlayFx        WEAPONS\SHOTGUN.FX  0 Constant
	  Scale		0.5
	TEnd
MEnd

## PROP READY 2 #######################################################

Move Prop_ready_2
	Requires	PROP
	Priority        1
	Interrupts	Ready
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	Interpolate	8

	Type		male
	  Anim          male\shotgun_look 1
	  PlayFx        WEAPONS\SHOTGUN.FX  0 Constant
	  Scale		0.3
	TEnd
MEnd

## PROP READY 3 #######################################################

Move Prop_ready_3
	Requires	PROP
	Priority        1
	Interrupts	Ready
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	Interpolate	8

	Type		male
	  Anim          male\shield_lookright 1
	  PlayFx        WEAPONS\swat_shield.fx  0 Constant
	  PlayFx        WEAPONS\Riot_Baton.fx  0 Constant
	  Scale		0.5
	TEnd
MEnd


## COMBAT READY #######################################################

Move PreCombat
	Requires	COMBAT
	Priority        1
	Interrupts	<Ready> PostWepReady
	Include sequencers\Members\Combat.inc
	Flags		ReqInputs
	NextMove        CombatReady
	Scale           0.50

	Type		male
	  Anim          male\pre_combat 3
	TEnd
MEnd

Move CombatReady
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		Cycle
	NextMove        postReady_back
	Scale           0.50

	Type		male
	  Anim          male\combat_cycle 1 31
	TEnd
MEnd

Move CombatBlend
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		ReqInputs
	NextMove        CombatReady
	Scale           0.50

	Type		male
	  Anim          male\combat_cycle 1 31
	TEnd
MEnd

Move Combat_half
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		ReqInputs
	NextMove        CombatReady
	Scale           0.70

	Type		male
	  Anim          male\combat_cycle 15 31
	TEnd
MEnd

## LAZY COMBAT READY #######################################################

Move Lazy_CombatReady
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		Cycle
	NextMove        postReady_back
	Scale           0.50

	Type		male
	  Anim          male\combat_cycle 1 31
	TEnd
MEnd

Move Lazy_Combat_half
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		ReqInputs
	NextMove        Lazy_CombatReady
	Scale           0.70

	Type		male
	  Anim          male\combat_cycle 15 31
	TEnd
MEnd


Move Lazy_Combat_Blend
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		ReqInputs
	NextMove        Lazy_CombatReady
	Scale           0.5
	Interpolate     15

	Type		male
	  Anim          male\combat_cycle 1 31
	TEnd
MEnd

## WEAPON COMBAT READY #######################################################

Move PreWep
	Requires	COMBAT WEAPON
	Priority        2
	Interrupts	<Ready> PreCombat  ##<Combat>
	Include sequencers\Members\Combat_Weapon.inc
	Flags		ReqInputs
	NextMove        WepReady
	Scale           0.50

	Type		male
	  Anim          male\pre_wep 3
	TEnd
MEnd

Move WepReady
	Requires	COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon.inc
	Flags		Cycle
	NextMove        postReady_back
	Scale           0.50

	Type		male
	  Anim          male\wep_cycle 1 31
	TEnd
MEnd

Move PostWepReady
	Include sequencers\Members\Readies.inc
	NextMove        Ready

	Type		male
	  Anim          male\post_wep 1
	TEnd
MEnd

Move Wep_half
	Requires	COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon.inc
	Flags		ReqInputs
	NextMove        WepReady
	Scale           0.50

	Type		male
	  Anim          male\wep_cycle 15 31
	TEnd
MEnd

Move Lazy_WepReady
	Requires	COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon.inc
	Flags		Cycle
	NextMove        postReady_back
	Scale           0.50

	Type		male
	  Anim          male\wep_cycle 1 31
	TEnd
MEnd

Move Lazy_WepReady_half
	Requires	COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Lazy_WepReady
	Scale           0.50

	Type		male
	  Anim          male\wep_cycle 15 31
	TEnd
MEnd

Move Blend_WepReady
	Requires	COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon.inc
	Flags		Cycle
	NextMove        postReady_back
	Scale           0.50

	Type		male
	  Anim          male\wep_cycle 1 31
	TEnd
MEnd

### SHOTGUN READY #######################################################
#
#Move ShotgunReady
#	Requires	COMBAT WEAPON GUN
#	Member		<ReactIRQ> <DeathIRQ> <TwoHandCombat> <AttackIRQ> <BlockIRQ>
#	Flags		Cycle FixArmReg
#	NextMove        postReady_back
#	Scale           0.50
#
#	Type		male
#	  Anim          male\rocket_cycle 1 31
#	TEnd
#MEnd
#
#Move ShotgunReady_15
#	Requires	COMBAT WEAPON GUN
#	Member		<ReactIRQ> <DeathIRQ> <TwoHandCombat> <AttackIRQ> <BlockIRQ>
#	NextMove        ShotgunReady
#	Scale           0.50
#	Flags		FixArmReg
#
#	Type		male
#	  Anim          male\rocket_cycle 15 31
#	TEnd
#MEnd
#
#Move PostShotgun
#	Member		<ReactIRQ> <DeathIRQ> <TwoHandCombat> <PostMoveCycle> <AttackIRQ> <BlockIRQ>
#	Flags		ReqInputs
#	NextMove        Ready
##
#	Type		male
#	  Anim          male\post_shotgun 1
#	TEnd
#MEnd
#
#Move RocketReady
#	Requires	COMBAT WEAPON GUN
#	Member		<ReactIRQ> <DeathIRQ> <TwoHandCombat> <AttackIRQ> <BlockIRQ>
#	Flags		Cycle FixArmReg
#	NextMove        postReady_back
#	Scale           0.50
#
#	Type		male
#	  Anim          male\rocket_cycle 1 31
#	TEnd
#MEnd
#
#Move RocketReady_15
#	Requires	COMBAT WEAPON GUN
#	Member		<ReactIRQ> <DeathIRQ> <TwoHandCombat> <AttackIRQ> <BlockIRQ>
#	Flags		FixArmReg
#	NextMove        RocketReady
#	Scale           0.50
#
#	Type		male
#	  Anim          male\rocket_cycle 15 31
#	TEnd
#MEnd

## BACK TO READY #######################################################

Move BackToNeutralReady
	Requires	CLEAR
	Priority        3
	Interrupts	<Ready> <MonsterGroup> <ReactIRQ> <Combat> <WepCombat> <TwoHandCombat> <PostMoveCycle> <Block>
	Include sequencers\Members\Readies.inc
	NextMove	Ready_2Hips_look
	Interpolate     15
	Scale		0.5

	Type		male
	  Anim          male\stand_2Hhips 1 64
	TEnd

	Type		huge
	  Anim          huge\stand_2Hhips 1 64
	TEnd

	Type		fem
	  Anim          fem\stand_2Hhips 1 64
	TEnd
MEnd

## READY #######################################################

Move Stand_Cycle
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	Interpolate     0

	Type		male
	  Anim          male\what 99 100
	TEnd
MEnd