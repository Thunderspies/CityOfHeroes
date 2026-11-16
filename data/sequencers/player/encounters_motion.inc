#############################################################
## ENCOUNTERS_MOTION.INC - AnimLists that require Modes
#############################################################

#############################################################
## IDLE MODE
#############################################################

## IDLE READY ###########################################################

Move Idle_Ready
	Requires	IDLE
	Priority	1
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\skel_ready2 1
	TEnd

	Type		Vahzilok
	  Anim          Vahzilok\skel_vah_combat_cycle 1
	TEnd

	Type		Crystal
	  Anim          male\MonComb_cycle 1
	TEnd
MEnd


## IDLE WALK FORWARD ###########################################################

Move Idle_Walk_Pre
	Requires	FORWARD IDLE
	Priority	2
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		ReqInputs
	NextMove        Idle_Walk_Cycle

	Type		male
	  Anim          male\walk 37 48
	TEnd

	Type		Vahzilok
	  Anim          Vahzilok\vah_walk 24 25
	TEnd

	Type		Sprocket
	  Anim          male\gurder_walk 1
	  PlayFx        GENERIC\PROP_GURDER.FX  0 Constant
	  Scale         0.5
	TEnd

	Type		Crystal
	  Anim          male\MonComb_walk 1
	  MoveRate	0.146
	  Scale		1.1
	TEnd
MEnd

Move Idle_Walk_Cycle
	Requires	FORWARD IDLE
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle
	NextMove        Idle_Walk_Post

	Type		male
	  Anim          male\walk 1 48
	TEnd

	Type		Vahzilok
	  Anim          Vahzilok\vah_walk 1 25
	TEnd

	Type		Sprocket
	  Anim          male\gurder_walk 1
	  PlayFx        GENERIC\PROP_GURDER.FX  0 Constant
	  Scale         0.5
	TEnd

	Type		Crystal
	  Anim          male\MonComb_walk 1
	  MoveRate	0.146
	  Scale		1.1
	TEnd
MEnd

Move Idle_Walk_Post
	Requires	IDLE
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        Idle_Ready

	Type		male
	  Anim          male\walk 47 48
	TEnd

	Type		Vahzilok
	  Anim          Vahzilok\vah_postrun 4 28
	TEnd

	Type		Sprocket
	  Anim          male\lookquick_sprocket 86
	  PlayFx        GENERIC\PROP_GURDER.FX  0 Constant
	TEnd

	Type		Crystal
	  Anim          male\MonComb_postwalk 1
	  Scale		2
	TEnd
MEnd


#################################  NPC Walk Cycles  #######################################

##########################
######  Walk Sullen  #####
##########################
#
#Move Walk_Sullen
#	Requires	FORWARD test9
#	Include sequencers\Interrupts\Encounters_Motion.inc
#	Member		<ReactIRQ> <DeathIRQ> <AttackIRQ> <BlockIRQ>
#	Flags		Cycle NoSizeScale ReqInputs
#	NextMove        Ready
#	Priority        2
#	scale		.85						#uses speedscale .15
#
#	Type		male
#	  Anim          male\Walk_Sullen 1
#	TEnd
#MEnd
#
#
#########################################################
##################    Walk Nervous    ###################
#########################################################
#
#Move Walk_Nervous
#	Requires	FORWARD test7
#	Interrupts	<Ready>  ##<CombatMove> <WepMove> <TwoHandMove>
#	Member		<ReactIRQ> <DeathIRQ> <AttackIRQ> <BlockIRQ>
#	Flags		Cycle NoSizeScale ReqInputs
#	NextMove        Ready
#	Priority        2
#	scale		1.5					#speedscale .2
#
#	Type		male
#	  Anim          male\Walk_Nervous 1
#	TEnd
#MEnd
#
###################################################################
##############   Walk Nervous With BriefCase   ###################
##################################################################

#Move Walk_NervousCase
#	Requires	FORWARD test9
#	Interrupts	<Ready>  ##<CombatMove> <WepMove> <TwoHandMove>
#	Member		<ReactIRQ> <DeathIRQ> <AttackIRQ> <BlockIRQ>
#	Flags		Cycle PICKRANDOMLY
#	NextMove        Ready
#	Priority        2
#	scale		1.25
#	CycleMove       Walk_NervousCase
#	CycleMove       Walk_NervousCase
#	CycleMove       Walk_NervousCaseRight
#	CycleMove       Walk_NervousCaseLeft
#
#	Type		male
#	  Anim          male\Walk_NervousCase 1
#	  PlayFx        GENERIC\BriefCaseClutch.fx  0 Constant
#	TEnd
#MEnd
#
#Move Walk_NervousCaseRight
#	Requires	FORWARD test9
#	Interrupts	<Ready>  ##<CombatMove> <WepMove> <TwoHandMove>
#	Member		<ReactIRQ> <DeathIRQ> <AttackIRQ> <BlockIRQ>
#	Flags		Cycle PICKRANDOMLY
#	NextMove        Ready
#	Scale		1
#	Priority        2
#	scale		1.25
#	CycleMove       Walk_NervousCase
#
#	Type		male
#	  Anim          male\Walk_NervousCaseR 1
#	  PlayFx        GENERIC\BriefCaseClutch.fx  0 Constant
#	TEnd
#MEnd
#
#Move Walk_NervousCaseLeft
#	Requires	FORWARD test9
#	Interrupts	<Ready>  ##<CombatMove> <WepMove> <TwoHandMove>
#	Member		<ReactIRQ> <DeathIRQ> <AttackIRQ> <BlockIRQ>
#	Flags		Cycle PICKRANDOMLY
#	NextMove        Ready
#	Priority        2
#	scale		1.25
#	CycleMove       Walk_NervousCase
#
#	Type		male
#	  Anim          male\Walk_NervousCaseL 1
#	  PlayFx        GENERIC\BriefCaseClutch.fx  0 Constant
#	TEnd
#MEnd

########################################################
##############   Walk ShoppingCart   ###################
########################################################

#Move Walk_ShoppingCart7
#	Requires	FORWARD test8
#	Interrupts	<Ready>  ##<CombatMove> <WepMove> <TwoHandMove>
#	Member		<ReactIRQ> <DeathIRQ> <AttackIRQ> <BlockIRQ>
#	Flags		Cycle NoSizeScale ReqInputs
#	NextMove        Ready
#	Priority        2
#	scale		.75						#uses speedscale .15
#
#	Type		male
#	  Anim          male\Walk_ShopCart 1
#	  PlayFx        GENERIC\ShoppingCart.fx  0 Constant
#	  PlayFx	GENERIC\Garbage_flys.fx
#	TEnd
#MEnd
#
#########################################################
#################   Walk Paranoid   #####################
#########################################################


#Move Walk_Paranoid
#	Requires	FORWARD test8
#	Interrupts	<Ready>  ##<CombatMove> <WepMove> <TwoHandMove>
#	Member		<ReactIRQ> <DeathIRQ> <AttackIRQ> <BlockIRQ>
#	Flags		Cycle PICKRANDOMLY
#	Priority        2
#	#Interpolate	8
#	CycleMove       Walk_Paranoid
#	CycleMove       Walk_Paranoid_Left
#	CycleMove       Walk_Paranoid_Right
#	#NextMove        Ready
#
#	Type		male
#	  Anim          male\Walk_Paranoid 1
#	TEnd
#MEnd
#
####  Look Left  ###
#
#Move Walk_Paranoid_Left
#	Requires	FORWARD test8
#	Interrupts	<Ready>  ##<CombatMove> <WepMove> <TwoHandMove>
#	Member		<ReactIRQ> <DeathIRQ> <AttackIRQ> <BlockIRQ>
#	Flags		Cycle PICKRANDOMLY
#	Priority        2
#	#Interpolate	8
#	CycleMove       Walk_Paranoid
#	CycleMove       Walk_Paranoid_Right
#	#NextMove        Ready
#
#	Type		male
#	  Anim          male\Walk_Paranoid_Left 1
#	TEnd
#MEnd
#
####  Look Right  ###
#
#Move Walk_Paranoid_Right
#	Requires	FORWARD test8
#	Interrupts	<Ready>  ##<CombatMove> <WepMove> <TwoHandMove>
#	Member		<ReactIRQ> <DeathIRQ> <AttackIRQ> <BlockIRQ>
#	Flags		Cycle PICKRANDOMLY
#	Priority        2
#	#Interpolate	8
#	CycleMove       Walk_Paranoid
#	CycleMove       Walk_Paranoid_Left
#	#NextMove        Ready
#
#	Type		male
#	  Anim          male\Walk_Paranoid_Right 1
#	TEnd
#MEnd
#
#
##########################################################
##################  Walk Garbage Bag  ####################
##########################################################
#
#
#Move Walk_GarbageBag
#	Requires	FORWARD test9
#	Interrupts	<Ready>  ##<CombatMove> <WepMove> <TwoHandMove>
#	Member		<ReactIRQ> <DeathIRQ> <AttackIRQ> <BlockIRQ>
#	Flags		Cycle NoSizeScale ReqInputs
#	NextMove        Ready
#	Priority        2
#
#	Type		male
#	  Anim          male\Walk_GarbageBag 1
#	  PlayFx        GENERIC\GarbageBag_Carriable.fx  0 Constant
#	TEnd
#MEnd


#############################################################
## LIMP MODE
#############################################################

## LIMP FORWARD ###########################################################

Move Limp_Forward_Pre
	Requires	LIMP FORWARD
	Priority	2
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		ReqInputs
	NextMove        Limp_Forward_Cycle
	Scale           0.70

	Type		male
	  Anim          male\weak_walk 10 28
	TEnd
MEnd

Move Limp_Forward_Cycle
	Requires	LIMP FORWARD
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle
	NextMove        Limp_Forward_Post
	Scale           0.70

	Type		male
	  Anim          male\weak_walk 1 28
	TEnd
MEnd

Move Limp_Forward_Post
	Requires	LIMP
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        Ready

	Type		male
	  Anim          male\weak_walk 27 28
	TEnd
MEnd


#############################################################
## LOITER
#############################################################

## LOITER READY ###########################################################

Move Loiter_ReadyA
	Requires	LOITER
	Priority	1
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Scale           0.50

	Type		male
	  Anim          male\suspicious 1
	  PlayFX		Feet\Foot_NPCMale.fx 7
	  PlayFX		Feet\Foot_NPCMale.fx 12
	  PlayFX		Feet\Foot_NPCMale.fx 67
	  PlayFX		Feet\Foot_NPCMale.fx 72
	TEnd

	Type		PraetorianClockworkMale
	  Anim          male\suspicious 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 6
	  PlayFX 		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 25
	  PlayFX 		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 41
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 61
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 66
	  PlayFX 		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 83
	TEnd

	Type		PraetorianClockworkFem
	  Anim          male\suspicious 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 6
	  PlayFX 		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 25
	  PlayFX 		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 41
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 61
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 66
	  PlayFX 		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 83
	TEnd

	Type		PraetorianClockworkHuge
	  Anim          male\suspicious 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 6
	  PlayFX 		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 25
	  PlayFX 		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 41
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 61
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 66
	  PlayFX 		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 83
	TEnd

	Type		Sprocket
	  Anim          male\lookquick_sprocket 1
	  PlayFx        GENERIC\PROP_GURDER.FX  0 Constant
	TEnd

	Type		Crystal
	  Anim          male\MonComb_cycle 1
	TEnd
MEnd

#############################################################

Move Loiter_ReadyB
	Requires	LOITER
	Priority	1
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Scale           0.30

	Type		male
	  Anim          male\suspicious 1
	  PlayFX		Feet\Foot_NPCMale.fx 7
	  PlayFX		Feet\Foot_NPCMale.fx 12
	  PlayFX		Feet\Foot_NPCMale.fx 67
	  PlayFX		Feet\Foot_NPCMale.fx 72
	TEnd

	Type		PraetorianClockworkMale
	  Anim          male\suspicious 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 3
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 8
	  PlayFX 		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 25
	  PlayFX 		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 41
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 63
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 68
	  PlayFX 		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 83
	TEnd

	Type		PraetorianClockworkFem
	  Anim          male\suspicious 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 3
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 8
	  PlayFX 		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 25
	  PlayFX 		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 41
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 63
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 68
	  PlayFX 		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 83
	TEnd

	Type		PraetorianClockworkHuge
	  Anim          male\suspicious 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 3
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 8
	  PlayFX 		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 25
	  PlayFX 		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 41
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 63
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 68
	  PlayFX 		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 83
	TEnd

	Type		Sprocket
	  Anim          male\lookquick_sprocket 1
	  PlayFx        GENERIC\PROP_GURDER.FX  0 Constant
	TEnd

	Type		Crystal
	  Anim          male\MonComb_cycle 1
	TEnd
MEnd

#############################################################

Move Loiter_ReadyC
	Requires	LOITER
	Priority	1
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Scale           0.80

	Type		male
	  Anim          male\suspicious 1
	  PlayFX		Feet\Foot_NPCMale.fx 7
	  PlayFX		Feet\Foot_NPCMale.fx 12
	  PlayFX		Feet\Foot_NPCMale.fx 67
	  PlayFX		Feet\Foot_NPCMale.fx 72
	TEnd

	Type		PraetorianClockworkMale
	  Anim          male\suspicious 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 6
	  PlayFX 		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 25
	  PlayFX 		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 41
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 61
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 66
	  PlayFX 		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 83
	TEnd

	Type		PraetorianClockworkFem
	  Anim          male\suspicious 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 6
	  PlayFX 		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 25
	  PlayFX 		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 41
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 61
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 66
	  PlayFX 		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 83
	TEnd

	Type		PraetorianClockworkHuge
	  Anim          male\suspicious 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 6
	  PlayFX 		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 25
	  PlayFX 		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 41
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 61
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_FeetMale.fx 66
	  PlayFX 		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 83
	TEnd

	Type		Sprocket
	  Anim          male\lookquick_sprocket 1
	  PlayFx        GENERIC\PROP_GURDER.FX  0 Constant
	TEnd

	Type		Crystal
	  Anim          male\MonComb_cycle 1
	TEnd
MEnd


## LOITER SNEAK ###########################################################

Move Loiter_Sneak_Forward
	Requires	FORWARD LOITER
	Priority	2
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle
	Scale           0.50

	Type		male
	  Anim          male\sneek 1
	TEnd
MEnd


#############################################################
## BOX MODE
#############################################################

## BOX(LOOT) READY ###########################################################

Move BoxReady
	Requires	CARRY BOX LOOT
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\box_ready 1
	  PlayFx        GENERIC\PROP_THEFT.FX  0 Constant
	TEnd
MEnd


## BOX (LOOT) SNEAK FORWARD ###########################################################

Move BoxSneak_pre
	Requires	FORWARD CARRY HIGH BOX LOOT
	Priority	5
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		ReqInputs
	NextMove        BoxSneak_cycle
	Scale           0.40

	Type		male
	  Anim          male\box_sneek 1
	  PlayFx        GENERIC\PROP_THEFT.FX 0 Constant
	TEnd
MEnd

Move BoxSneak_cycle
	Requires	FORWARD CARRY HIGH BOX LOOT
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle
	NextMove        BoxSneak_post
	Scale           0.40

	Type		male
	  Anim          male\box_sneek 1
	  PlayFx        GENERIC\PROP_THEFT.FX 0 Constant
	TEnd
MEnd

Move BoxSneak_post
	Requires	CARRY BOX LOOT
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        BoxReady
	Scale		0.7

	Type		male
	  Anim          male\box_ready 1
	  PlayFx        GENERIC\PROP_THEFT.FX  0 Constant
	TEnd
MEnd

#Move Box_Ditch_Run
#	Requires	FORWARD
#	Interrupts	Box_Ready Box_Cycle
#	Member		<BasicMove> <DeathIRQ> <AttackIRQ>
#	NextMove        Box_Ditch_Run2
#	Priority	1
#	Sets            CANTMOVE
#	SetsOnChild     SHORTTOSS
#
#	Type		male
#	  Anim          male\box_ditch 1 12
#	  PlayFx        GENERIC\PROP_THEFT_TOSS.FX
#	TEnd
#MEnd
#
#
#Move Box_Ditch_Run2
#	Requires	FORWARD
#	Member		<BasicMove> <DeathIRQ> <AttackIRQ>
#	NextMove        WalkCycle
#	Priority	1
#
#	Type		male
#	  Anim          male\box_ditch 12
#	TEnd
#MEnd



#############################################################
## CRATE MODE
#############################################################

## CRATE READY ###########################################################

Move CrateReady
	Requires	CARRY BOX LOITER
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle FixWeaponReg

	Type		male
	  Anim          male\crate_walk_post 45
	  PlayFx        GENERIC\crate.fx  0 Constant
	TEnd

	Type		PraetorianClockworkMale
	  Anim          male\crate_walk_post 45
	  PlayFx        GENERIC\crate.fx  0 Constant
	TEnd

MEnd

## CRATE WALK FORWARD ###########################################################

Move CrateWalk_pre
	Requires	CARRY BOX LOITER FORWARD
	Priority	4
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		ReqInputs FixWeaponReg
	NextMove        CrateWalk_cycle

	Type		male
	  Anim          male\crate_walk 37 48
	  PlayFx        GENERIC\crate.fx  0 Constant
	TEnd

	Type		PraetorianClockworkMale
	  Anim          male\crate_walk 37 48
	  PlayFx        GENERIC\crate.fx  0 Constant
	TEnd
MEnd

Move CrateWalk_cycle
	Requires	CARRY BOX LOITER FORWARD
	Priority	4
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle FixWeaponReg
	NextMove        CrateWalk_cycle_post

	Type		male
	  Anim          male\crate_walk 1 48
	  PlayFx        GENERIC\crate.fx  0 Constant
	  PlayFx        FEET\NPCMALELEFT.FX  6
	  PlayFx        FEET\NPCMALERIGHT.FX  29
	TEnd

	Type		PraetorianClockworkMale
	  Anim          male\crate_walk 1 48
	  PlayFx        GENERIC\crate.fx  0 Constant
	  PlayFx        FEET\NPCMALELEFT.FX  6
	  PlayFx        FEET\NPCMALERIGHT.FX  29
	TEnd
MEnd

Move CrateWalk_cycle_post
	Requires	CARRY BOX LOITER
	Priority	3
	Flags		FixWeaponReg
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        CrateReady
	Scale		0.7

	Type		male
	  Anim          male\crate_walk_post 7  ##crate_putdown 47
	  PlayFx        GENERIC\crate.fx  0 Constant
	TEnd

	Type		PraetorianClockworkMale
	  Anim          male\crate_walk_post 7  ##crate_putdown 47
	  PlayFx        GENERIC\crate.fx  0 Constant
	TEnd
MEnd


#############################################################
## TNT CRATE MODE
#############################################################

## TNT_CRATE READY ###########################################################

Move TNT_CrateReady
	Requires	CARRY BOX LOITER HIGH
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle FixWeaponReg

	Type		male
	  Anim          male\crate_walk_post 45
	  PlayFx        GENERIC\tnt_crate.fx 0 Constant
	TEnd

	Type		PraetorianClockworkMale
	  Anim          male\crate_walk_post 45
	  PlayFx        GENERIC\tnt_crate.fx 0 Constant
	TEnd
MEnd


## TNT_CRATE WALK FORWARD ###########################################################

Move TNT_CrateWalk_pre
	Requires	CARRY BOX LOITER FORWARD HIGH
	Priority	5
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		ReqInputs FixWeaponReg
	NextMove        TNT_CrateWalk_cycle

	Type		male
	  Anim          male\crate_walk 37 48
	  PlayFx        GENERIC\tnt_crate.fx 0 Constant
	TEnd

	Type		PraetorianClockworkMale
	  Anim          male\crate_walk 37 48
	  PlayFx        GENERIC\tnt_crate.fx 0 Constant
	TEnd
MEnd

Move TNT_CrateWalk_cycle
	Requires	CARRY BOX LOITER FORWARD HIGH
	Priority	5
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle FixWeaponReg
	NextMove        TNT_CrateWalk_cycle_post

	Type		male
	  Anim          male\crate_walk 1 48
	  PlayFx        GENERIC\tnt_crate.fx  0 Constant
	  PlayFx        FEET\NPCMALELEFT.FX  6
	  PlayFx        FEET\NPCMALERIGHT.FX  29
	TEnd

	Type		PraetorianClockworkMale
	  Anim          male\crate_walk 1 48
	  PlayFx        GENERIC\tnt_crate.fx  0 Constant
	  PlayFx        FEET\NPCMALELEFT.FX  6
	  PlayFx        FEET\NPCMALERIGHT.FX  29
	TEnd
MEnd

Move TNT_CrateWalk_cycle_post
	Requires	CARRY BOX LOITER HIGH
	Priority	4
	Flags		FixWeaponReg
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        CrateReady
	Scale		0.7

	Type		male
	  Anim          male\crate_walk_post 7  ##crate_putdown 47
	  PlayFx        GENERIC\tnt_crate.fx  0 Constant
	TEnd

	Type		PraetorianClockworkMale
	  Anim          male\crate_walk_post 7  ##crate_putdown 47
	  PlayFx        GENERIC\tnt_crate.fx  0 Constant
	TEnd
MEnd

### (alt version w\ shoping cart)

#Move TNT_CrateWalk_cycle_Alt
#	Requires	CARRY BOX LOITER FORWARD high
#	Interrupts	TNT_CrateReady CrateReady <Ready> <BasicMove> <CombatMove> <WepMove> <TwoHandMove>
#	Member		<ReactIRQ> <DeathIRQ> <AttackIRQ> <BlockIRQ>
#	Flags		Cycle
#	Priority	5
#	scale		0.75						#uses speedscale .15
#
#	Type		male
#	  Anim          male\Walk_ShopCart 1
#	  PlayFx        GENERIC\TNT_ShoppingCart.fx  0 Constant
#	TEnd
#MEnd


#####  LootCrate Mode encounters  ###########################

###################
### LootCrate Ready  ##
###################

Move LootCrateReady
	Requires	CARRY BOX LOITER LOW
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\crate_walk_post 45
	  PlayFx        GENERIC\loot_crate.fx  0 Constant
	TEnd
MEnd

## LOOTCRATE WALK FORWARD ###########################################################

Move LootCrateWalk_pre
	Requires	CARRY BOX LOITER FORWARD LOW
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		ReqInputs
	NextMove        LootCrateWalk_cycle

	Type		male
	  Anim          male\crate_walk 37 48
	  PlayFx        GENERIC\loot_crate.fx  0 Constant
	TEnd
MEnd

Move LootCrateWalk_cycle
	Requires	CARRY BOX LOITER FORWARD LOW
	Priority	5
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle
	NextMove        LootCrateWalk_cycle_post

	Type		male
	  Anim          male\crate_walk 1 48
	  PlayFx        GENERIC\loot_crate.fx  0 Constant
	  PlayFx        FEET\NPCMALELEFT.FX  6
	  PlayFx        FEET\NPCMALERIGHT.FX  29
	TEnd
MEnd

Move LootCrateWalk_cycle_post
	Requires	CARRY BOX LOITER LOW
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        LootCrateReady
	Scale		0.7

	Type		male
	  Anim          male\crate_walk_post 7  ##crate_putdown 47
	  PlayFx        GENERIC\loot_crate.fx  0 Constant
	TEnd
MEnd

#############################################################
## SNEAK MODE
#############################################################

###################
### Sneak & turn Run ##
###################

Move Sneakback
	Requires	FORWARD SNEAK WALK FEAR
	Priority	4
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle
	NextMove        SneakTurn
	Scale           0.80

	Type		male
	  Anim          bis\sneekNturn 1 25
	TEnd

	Type		fem
	  Anim          BF\rev_sneekback 1 25
	TEnd
MEnd

Move SneakTurn
	Requires	FORWARD
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        RunCycle

	Type		male
	  Anim          bis\sneekNturn 1
	TEnd

	Type		fem
	  Anim          BF\sneekNturn 1
	TEnd
MEnd


#############################################################
## FEAR MODE
#############################################################

## FEAR RUN FORWARD ###########################################################

Move F_runCycle
	Requires	FORWARD FEAR
	Priority	3 # changed from 2 to 3 because the male npc would continue to switch back to FearStun every occasionally
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle MoveScale
	Scale           1.45
	MoveRate	0.7
	NextMove	WalkCycle

	Type		male
	  Anim          bis\bookrun 1 25
	  PlayFx	FEET\Foot_NPCFemale.fx 10
	  PlayFx	FEET\Foot_NPCFemale.fx 22
	TEnd
MEnd

#############################################################

Move F_runCycle_B
	Requires	FORWARD FEAR
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle MoveScale
	Scale           1.45
	MoveRate	0.7
	NextMove	WalkCycle

	Type		male
	  Anim          bis\armsrun 1 25
	  PlayFx	FEET\Foot_NPCMale.fx 10
	  PlayFx	FEET\Foot_NPCMale.fx 22
	TEnd
MEnd

#############################################################

Move F_runCycle_C
	Requires	FORWARD FEAR
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle MoveScale
	Scale           1.45
	MoveRate	0.7
	NextMove	WalkCycle

	Type		male
	  Anim          bis\scaredrun 1 25
	  PlayFx	FEET\Foot_NPCMale.fx 10
	  PlayFx	FEET\Foot_NPCMale.fx 22
	TEnd
MEnd


#############################################################
## PICKAXE MODE
#############################################################

## PIXAXE READY ###########################################################

Move PickAxe
	Requires	LOOKOUT WORK WAIT POSE GROUNDSUMMON HIGH
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale		0.5
	Interpolate	15

	Type		Male
	  Anim          Male\pickaxe_idle 1
	  PlayFx        GENERIC\Pickaxe.FX 0 Constant
	TEnd
MEnd


## PICKAXE WALK ###########################################################

Move PickAxeWalk_pre
	Requires	FORWARD LOOKOUT WORK WAIT POSE GROUNDSUMMON HIGH
	Priority	7
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		ReqInputs
	NextMove        PickAxeWalk_cycle
	Flags		MoveScale

	Type		Male
	  Anim          Male\pickaxe_walk 1
	  PlayFx        GENERIC\Pickaxe.FX 0 Constant
	  MoveRate	0.2
	TEnd
MEnd

Move PickAxeWalk_cycle
	Requires	FORWARD LOOKOUT WORK WAIT POSE GROUNDSUMMON HIGH
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle MoveScale
	NextMove        PickAxeWalk_Post

	Type		Male
	  Anim          Male\pickaxe_walk 1
	  PlayFx        GENERIC\Pickaxe.FX 0 Constant
	  MoveRate	0.2
	  PlayFx        FEET\NPCMALELEFT.FX  1
	  PlayFx        FEET\NPCMALERIGHT.FX  16
	TEnd
MEnd

Move PickAxeWalk_Post
	Requires	LOOKOUT WORK WAIT POSE GROUNDSUMMON HIGH
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        PickAxe
	Sets            CANTMOVE
	Interpolate	15

	Type		Male
	  Anim          Male\pickaxe_idle 1
	  PlayFx        GENERIC\Pickaxe.FX 0 Constant
	TEnd
MEnd


## ALTERNATE PICKAXE WALK ###########################################################

Move Alt_PickAxeWalk_pre
	Requires	FORWARD LOOKOUT WORK WAIT POSE GROUNDSUMMON HIGH
	Priority	7
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		ReqInputs
	NextMove        Alt_PickAxeWalk_cycle
	Flags		MoveScale

	Type		Male
	  Anim          Male\pickaxe_walk_relaxed 1
	  PlayFx        GENERIC\Pickaxe.FX 0 Constant
	  MoveRate	0.2
	TEnd
MEnd

Move Alt_PickAxeWalk_cycle
	Requires	FORWARD LOOKOUT WORK WAIT POSE GROUNDSUMMON HIGH
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle MoveScale
	NextMove        PickAxeWalk_Post

	Type		Male
	  Anim          Male\pickaxe_walk_relaxed 1
	  PlayFx        GENERIC\Pickaxe.FX 0 Constant
	  PlayFx        FEET\NPCMALELEFT.FX  1
	  PlayFx        FEET\NPCMALERIGHT.FX  16
	  MoveRate	0.2
	TEnd
MEnd


#############################################################
## BARREL MODE
#############################################################

## BARREL READY ###########################################################

Move BarrelRoll_Ready
	Requires	LOITER PROP WORK
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\barrel_roll 1
	  PlayFx        GENERIC\barrelroll.fx  0 Constant
	TEnd
MEnd


## BARREL ROLL WALK ###########################################################

Move Barrel_roll_pre
	Requires	FORWARD LOITER PROP WORK
	Priority	4
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		ReqInputs
	NextMove        Barrel_roll_cycle

	Type		male
	  Anim          male\barrel_roll 1
	  PlayFx        GENERIC\barrelroll.fx  0 Constant
	TEnd
MEnd

Move Barrel_roll_cycle
	Requires	FORWARD LOITER PROP WORK
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle ##MoveScale
	NextMove	Barrel_roll_post

	Type		male
	  Anim          male\barrel_roll 1
	  PlayFx        GENERIC\barrelroll.fx 0 CONSTANT
	  PlayFx        FEET\NPCMALELEFT.FX 6
	  PlayFx        FEET\NPCMALERIGHT.FX 30
	TEnd
MEnd

Move Barrel_roll_post
	Requires	LOITER PROP WORK
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        BarrelRoll_Ready

	Scale		0.7
	Sets            CANTMOVE

	Type		male
	  Anim          male\lookquick 1
	  PlayFx        GENERIC\barrelroll.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
## WHEELBARROW MODE
#############################################################

## WHEELBARROW READY ###########################################################

Move WheelBarrow_Ready
	Requires	LOITER PROP WORK HIGH
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Priority	4

	Type		male
	  Anim          male\wheelbarrel_roll 1
	  PlayFx        GENERIC\wheelbarrel.fx  0 Constant
	TEnd
MEnd


## WHEELBARROW FORWARD ###########################################################

Move WheelBarrow_Forward
	Requires	FORWARD LOITER PROP WORK HIGH
	Priority	5
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle
	NextMove	WheelBarrow_Ready

	Type		male
	  Anim          male\wheelbarrel_roll 1
	  PlayFx        GENERIC\wheelbarrel.fx  0 Constant
	  PlayFx        FEET\NPCMALELEFT.FX  6
	  PlayFx        FEET\NPCMALERIGHT.FX  30
	TEnd
MEnd

#############################################################
## WHEELBARROW W/PUMKIN
#############################################################

Move Pumpkin_WheelBarrow_Ready
	Requires	LOITER PROP WORK HIGH QUICK
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\wheelbarrel_roll 1
	  PlayFx        GENERIC\wheelbarrel_pumpkins.fx 0 CONSTANT
	TEnd
MEnd


## WHEELBARROW FORWARD (w/Pumpkin) ###########################################################

Move Pumpkin_WheelBarrow_Forward
	Requires	FORWARD LOITER PROP WORK HIGH QUICK
	Priority	6
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle
	NextMove	Pumpkin_WheelBarrow_Ready

	Type		male
	  Anim          male\wheelbarrel_roll 1
	  PlayFx        GENERIC\wheelbarrel_pumpkins.fx 0 CONSTANT
	  PlayFx        FEET\NPCMALELEFT.FX 6
	  PlayFx        FEET\NPCMALERIGHT.FX 30
	TEnd
MEnd


#############################################################
## HANDCART MODE
#############################################################

Move Handcart_Ready
	Requires	LOITER PROP LOW
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\handcart_look 1
	  PlayFx        GENERIC\handcart.fx  0 Constant
	TEnd
MEnd

## HAND CART WALK ###########################################################

Move Handcart_walk
	Requires	FORWARD LOITER PROP LOW
	Priority	4
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle MoveScale
	NextMove	Handcart_Ready
	Scale           1.1

	Type		male
	  Anim          male\handcart_walk 1
	  MoveRate	0.146
	  PlayFx        GENERIC\handcart.fx 0 CONSTANT
	  PlayFx        FEET\NPCMALELEFT.FX 6
	  PlayFx        FEET\NPCMALERIGHT.FX 30
	TEnd
MEnd


#############################################################
## TORCH MODE
#############################################################

Move Torch
	Requires	LOOKOUT WORK WAIT POSE
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale		0.5
	Interpolate	15

	Type		male
	  Anim          male\holdtorch 1
	  PlayFx        Generic\Torch_left.fx  0 Constant
	TEnd
MEnd

## TORCH WALK ###########################################################

Move TorchWalk_pre
	Requires	FORWARD LOOKOUT WORK WAIT POSE
	Priority	5
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		ReqInputs
	NextMove        TorchWalk_cycle

	Type		male
	  Anim          male\aleart_torch_walk 1 33  ##new_walk_leftTorch 1 33  ##torchwalk_long 1 48
	  PlayFx        Generic\Torch_left.fx  0 Constant
	  MoveRate	0.15  ##0.146
	TEnd

	Type		fem
	  Anim          fem\aleart_torch_walk 1 33
	  PlayFx        Generic\Torch_left.fx  0 Constant
	  MoveRate	0.15
	TEnd
MEnd

Move TorchWalk_cycle
	Requires	FORWARD LOOKOUT WORK WAIT POSE
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle MoveScale
	NextMove        TorchWalk_Post

	Type		male
	  Anim          male\aleart_torch_walk 1 33  ##new_walk_leftTorch 1 33  ##torchwalk_long 1 48
	  PlayFx        Generic\Torch_left.fx 0 CONSTANT
	  MoveRate	0.15  ##0.146
	TEnd

	Type		fem
	  Anim          fem\aleart_torch_walk 1 33
	  PlayFx        Generic\Torch_left.fx 0 CONSTANT
	  MoveRate	0.15
	TEnd
MEnd

Move TorchWalk_Post
	Requires	LOOKOUT WORK WAIT POSE
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        Torch
	Sets            CANTMOVE
	Interpolate	15

	Type		male
	  Anim          male\holdtorch 1
	  PlayFx        Generic\Torch_left.fx 0 CONSTANT
	TEnd

MEnd

#############################################################
## BOOK MODE
#############################################################

Move BookWalk_A
	Requires	FORWARD BOX PLAN POSE
	Priority	4
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle MoveScale
	NextMove        Ready

	Type		male
	  Anim          male\walk_book 1 33
	  PlayFx        Generic\Books_1.fx  0 Constant
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	10
	  MoveRate	0.15
	TEnd

	Type		fem
	  Anim          fem\walk_book_mid 1 25
	  PlayFx        Generic\Books_1.fx  0 Constant
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT	10
	  MoveRate	0.17
	TEnd
MEnd

###############################################################
##
##Move BookWalk_B
##	Requires	FORWARD BOX PLAN POSE
##	Priority	4
##	Include sequencers\Interrupts\Encounters_Motion.inc
##	Include sequencers\Members\Encounters_Motion.inc
##	Flags		Cycle MoveScale
##	NextMove        Ready
##
##
##	Type		male
##	  Anim          male\walk_book 1 33
##	  PlayFx        Generic\Books_1.fx  0 Constant
##	  MoveRate	0.15
##	TEnd
##
##	Type		fem
##	  Anim          fem\walk_book 1 25
##	  PlayFx        Generic\Books_1.fx  0 Constant
##	  MoveRate	0.17
##	TEnd
##MEnd


#############################################################
## MAGIC BOOK MODE
#############################################################

Move MagicWalk_A
	Requires	FORWARD LOITER MAGIC
	Priority	3
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle MoveScale
	NextMove        Ready

	Type		male
	  Anim          male\walk_book 1 33
	  PlayFx        Generic\Books_1.fx  0 Constant
	  PlayFX	ENEMYFX\Croatoa\Wisp_Blue\Croatoa_Wisp_Blue.fx 0 Constant
	  MoveRate	0.15
	TEnd

	Type		fem
	  Anim          fem\walk_book_mid 1 25
	  PlayFx        Generic\Books_1.fx  0 Constant
	  PlayFX	ENEMYFX\Croatoa\Wisp_Blue\Croatoa_Wisp_Blue.fx 0 Constant
	  MoveRate	0.17
	TEnd
MEnd


#############################################################
## SHOTGUN MODE
#############################################################

Move ShotPatrol_pre
	Requires	FORWARD COMBAT WEAPON SHOTGUN LOOKOUT
	Priority	5
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		ReqInputs
	NextMove        ShotPatrol_cycle

	Type		male
	  Anim          male\walk_patrol_shotgun 1
  	  PlayFx        WEAPONS\SHOTGUN.FX 5 Constant
	TEnd
MEnd

Move ShotPatrol_cycle
	Requires	FORWARD COMBAT WEAPON SHOTGUN LOOKOUT
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle
	NextMove        ShotPatrol_post
	CycleMove	ShotPatrol_cycle_02
	CycleMove	ShotPatrol_cycle_03
	CycleMove	ShotPatrol_cycle

	Type		male
	  Anim          male\walk_patrol_shotgun 1
  	  PlayFx        WEAPONS\SHOTGUN.FX 0 Constant
	TEnd
MEnd

Move ShotPatrol_cycle_02
	Requires	FORWARD COMBAT WEAPON SHOTGUN LOOKOUT
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle
	NextMove        ShotPatrol_post
	CycleMove	ShotPatrol_cycle_03
	CycleMove	ShotPatrol_cycle
	CycleMove	ShotPatrol_cycle_02

	Type		male
	  Anim          male\walk_patrol_shotgun 1
  	  PlayFx        WEAPONS\SHOTGUN.FX 0 Constant
	TEnd
MEnd

Move ShotPatrol_cycle_03
	Requires	FORWARD COMBAT WEAPON SHOTGUN LOOKOUT
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle
	NextMove        ShotPatrol_post
	CycleMove	ShotPatrol_cycle
	CycleMove	ShotPatrol_cycle_02
	CycleMove	ShotPatrol_cycle_03

	Type		male
	  Anim          male\walk_patrol_shotgun 1
  	  PlayFx        WEAPONS\SHOTGUN.FX 0 Constant
	TEnd
MEnd

Move ShotPatrol_post
	Requires	COMBAT WEAPON SHOTGUN LOOKOUT
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        Ready
	Scale		0.7

	Type		male
	  Anim          male\walk_patrol_shotgun 1
  	  PlayFx        WEAPONS\SHOTGUN.FX 0 Constant
	TEnd
MEnd


#############################################################
## MACE MODE
#############################################################

Move MacePatrol_pre
	Requires	FORWARD COMBAT WEAPON BLUNT LOOKOUT HIGH
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		ReqInputs
	NextMove        MacePatrol_cycle
	Priority	6

	Type		male
	  Anim          male\walk_patrol_mace 1
 	  PlayFx        WEAPONS\Trunchoen.fx  0 Constant
	TEnd
MEnd

Move MacePatrol_cycle
	Requires	FORWARD COMBAT WEAPON BLUNT LOOKOUT HIGH
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle
	NextMove        MacePatrol_post
	CycleMove	MacePatrol_cycle_02
	CycleMove	MacePatrol_cycle_03
	CycleMove	MacePatrol_cycle

	Type		male
	  Anim          male\walk_patrol_mace 1
 	  PlayFx        WEAPONS\Trunchoen.fx  0 Constant
	TEnd
MEnd

Move MacePatrol_cycle_02
	Requires	FORWARD COMBAT WEAPON BLUNT LOOKOUT HIGH
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle
	NextMove        MacePatrol_post
	CycleMove	MacePatrol_cycle_03
	CycleMove	MacePatrol_cycle
	CycleMove	MacePatrol_cycle_02

	Type		male
	  Anim          male\walk_patrol_mace 1
 	  PlayFx        WEAPONS\Trunchoen.fx 0 CONSTANT
	TEnd
MEnd

Move MacePatrol_cycle_03
	Requires	FORWARD COMBAT WEAPON BLUNT LOOKOUT HIGH
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle
	NextMove        MacePatrol_post
	CycleMove	MacePatrol_cycle
	CycleMove	MacePatrol_cycle_02
	CycleMove	MacePatrol_cycle_03

	Type		male
	  Anim          male\walk_patrol_mace 1
 	  PlayFx        WEAPONS\Trunchoen.fx 0 CONSTANT
	TEnd
MEnd

Move MacePatrol_post
	Requires	COMBAT WEAPON BLUNT LOOKOUT HIGH
	Priority	5
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        Ready
	Scale		0.7

	Type		male
	  Anim          male\walk_patrol_mace 1
	  PlayFx        WEAPONS\Trunchoen.fx 0 CONSTANT
	TEnd
MEnd


#############################################################
## BODYBAG MODE
#############################################################

Move BodyBag_Ready
	Requires	BODYBAG
	Priority	1
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle FixArmReg

	Type		male
	  Anim          male\BodyBag_Idle 1
	  PlayFx	GENERIC\FX_bodyBag.FX  1 Constant
	TEnd
MEnd

########################
### BodyBag Walk   #####  In Future: set up like Crate mode - w\ post and idles
#######################

Move BodyBag_Walk_pre
	Requires	FORWARD BODYBAG
	Priority	2
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		ReqInputs FixArmReg
	NextMove        BodyBag_Walk_cycle
	Scale		0.75

	Type		male
	  Anim          male\BodyBag_Walk 1
	  PlayFx	GENERIC\FX_bodyBag.FX 0 CONSTANT
	TEnd
MEnd

Move BodyBag_Walk_cycle
	Requires	FORWARD BODYBAG
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle FixArmReg ##MoveScale
	NextMove	BodyBag_Walk_post
	Scale		0.75

	Type		male
	  Anim          male\BodyBag_Walk 1
	  PlayFx	GENERIC\FX_bodyBag.FX 0 CONSTANT
	TEnd
MEnd

Move BodyBag_Walk_post
	Requires	BODYBAG
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        BodyBag_Ready
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\BodyBag_Idle 1
	  PlayFx	GENERIC\FX_bodyBag.FX 0 CONSTANT
	TEnd
MEnd

#############################################################

Move BodyBag_Toss
	Requires	BODYBAGTOSS
	Priority        1
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	NextMove        Ready

	Type		male
	  Anim          male\BodyBag_Toss 1
	  PlayFx	GENERIC\FX_bodyBagDispose.FX  0
	  PlayFx	GENERIC\FX_bodyBagLinger.FX  80
	TEnd
MEnd


#############################################################
## CEMENT SHOE MODE
#############################################################

Move CementIdleBase
	Requires	CEMENTSHOES
	Priority        1
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       CementIdleBase
	CycleMove       CementIdleBase
	CycleMove       CementIdleBase
	CycleMove       CementIdleShake

	Type		male
	  Anim          male\CementShoes_Base 1
	  PlayFX	Generic\CementBucket.fx 0 Constant
	TEnd
MEnd

Move CementIdleShake
	Requires	CEMENTSHOES
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       CementIdleBase


	Type		male
	  Anim          male\CementShoes_Shake 1
	  PlayFX	Generic\CementBucket.fx 0 Constant
	TEnd
MEnd

###########################
### Cement Shoes Forward ##
###########################

Move CementWalkPre
	Requires	FORWARD CEMENTSHOES
	Priority        2
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		ReqInputs
	NextMove        CementWalkCycle

	Type		male
	  Anim          male\CementShoes_Walk 14 38
	  PlayFX	Generic\CementBucket.fx 0 Constant
	TEnd
MEnd

Move CementWalkCycle
	Requires	FORWARD CEMENTSHOES
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle #MoveScale
	NextMove        CementWalkPost
	scale		2.5

	Type		male
	  Anim          male\CementShoes_Walk 1 38
	  PlayFX	Generic\CementBucket.fx 0 Constant
	TEnd
MEnd

Move CementWalkPost
	Requires	CEMENTSHOES
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        CementIdleBase

	Type		male
	  Anim          male\CementShoes_Walk 1
	  PlayFX	Generic\CementBucket.fx 0 Constant
	TEnd
MEnd

###########################
###  Cement Shoes Jump  ###
###########################

Move Cement_jumpPre
	Requires	FORWARD CEMENTSHOES JUMP
	Priority        10
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	NextMove        Cement_jumpUp

	Type		male
	  Anim          male\CementShoes_Jump 1 6
	  PlayFX	Generic\CementBucket.fx 0 Constant
	TEnd
MEnd

Move Cement_jumpUp
	Requires	FORWARD CEMENTSHOES JUMP
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle
	NextMove        Cement_jumpApex

	Type		male
	  Anim          male\CementShoes_Jump 7 8
	  PlayFX	Generic\CementBucket.fx 0 Constant
	TEnd
MEnd

Move Cement_jumpApex
	Requires	CEMENTSHOES APEX
	Include sequencers\Members\Encounters_Motion.inc
	NextMove        Cement_jumpFall

	Type		male
	  Anim          male\CementShoes_Jump 9 13
	  PlayFX	Generic\CementBucket.fx 0 Constant
	TEnd
MEnd

Move Cement_jumpFall
	Requires	CEMENTSHOES AIR
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle ReqInputs
	NextMove        jumpPost

	Type		male
	  Anim          male\CementShoes_Jump 14 15
	  PlayFX	Generic\CementBucket.fx 0 Constant
	TEnd
MEnd

Move Cement_JumpPost
	Requires	CEMENTSHOES
	Interrupts	JumpPost C_JumpPost
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        CementWalkPre
	Priority        3
	Interpolate     1
	Scale		0.7

	Type		male
	  Anim          male\CementShoes_Jump 16 28
	  PlayFX	Generic\CementBucket.fx 0 Constant
	TEnd
MEnd


##########################
### Cement Shoes Death ###
##########################

Move Cement_Death
	Requires	CEMENTSHOES DEATH
	Priority        3
	Interrupts	<DeathIRQ>
	Member		<RaiseDeadIRQ>
	NextMove        Cement_PostDeath
	Sets            NOCOLLISION

	Type		male
	  Anim          male\CementShoes_Death 1
	  PlayFX	Generic\CementBucket.fx 0 Constant
	TEnd
MEnd

Move Cement_PostDeath
	Requires	CEMENTSHOES DEATH
	Member		<RaiseDeadIRQ>
	Flags		Cycle
	Sets            NOCOLLISION DIENOW TARGET
	Wind		DEATHWIND

	Type		male
	  Anim          male\CementShoes_Death 65
	  PlayFX	Generic\CementBucket.fx 0 Constant
	TEnd
MEnd


#############################################################
## PICKET MODE
#############################################################

Move Picket_Stand
	Requires	LOOKOUT WORK WAIT POSE LOW
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Priority        5
	Scale		0.5
	Interpolate	15

	Type		Male
	  Anim          male\sign_chant 3
	  PlayFx        GENERIC\Protest_sign_A.fx  0 Constant
	TEnd
MEnd

#####################
### Picket March  ###
#####################

Move PicketMarch_pre
	Requires	LOOKOUT WORK WAIT POSE LOW FORWARD
	Priority        6
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		ReqInputs
	NextMove        PicketMarch_cycle
	Flags		MoveScale

	Type		Male
	  Anim          male\PicketMarch 1
	  PlayFx        GENERIC\Protest_sign_A.fx  0 Constant
	  MoveRate	.14
	TEnd

MEnd

Move PicketMarch_cycle
	Requires	LOOKOUT WORK WAIT POSE LOW FORWARD
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle MoveScale
	NextMove        PicketMarch_Post

	Type		Male
	  Anim          male\PicketMarch 1
	  PlayFx        GENERIC\Protest_sign_A.fx  0 Constant
	  MoveRate	0.14
	TEnd
MEnd

Move PicketMarch_Post
	Requires	LOOKOUT WORK WAIT POSE LOW
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        PickAxe
	Sets            CANTMOVE
	Interpolate	15

	Type		Male
	  Anim          male\PicketMarch 1
	  PlayFx        GENERIC\Protest_sign_A.fx  0 Constant
	TEnd
MEnd


#############################################################
## PURSESNATCH MODE
#############################################################

Move PL_PurseMug
	Requires	PURSESEARCH
	Priority        1
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly NoSizeScale
	scale		0.75
	CycleMove       PL_PurseMug
	CycleMove       PL_PurseMugToss
	CycleMove       PL_PurseMugLeft
	CycleMove       PL_PurseMugRight

	Type		male
	  Anim          male\PL_PurseMug 1 100
	  Playfx	Generic\FX_OpenPurse.fx  0 Constant
	TEnd
MEnd

Move PL_PurseMugToss
	Requires	PURSESEARCH
	Priority        1
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly NoSizeScale
	CycleMove       PL_PurseMug
	CycleMove       PL_PurseMug
	CycleMove       PL_PurseMugLeft
	CycleMove       PL_PurseMugRight

	Type		male
	  Anim          male\PL_PurseMug 100
	  Playfx	Generic\FX_OpenPurse.fx  0 Constant
	TEnd
MEnd

Move PL_PurseMugLeft
	Requires	PURSESEARCH
	Priority        1
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly NoSizeScale
	CycleMove       PL_PurseMug
	CycleMove       PL_PurseMugToss
	CycleMove       PL_PurseMug
	CycleMove       PL_PurseMugRight

	Type		male
	  Anim          male\PL_PurseMugLeft 1
	  Playfx	Generic\FX_OpenPurse.fx  0 Constant
	TEnd
MEnd

Move PL_PurseMugRight
	Requires	PURSESEARCH
	Priority        1
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly NoSizeScale
	CycleMove       PL_PurseMug
	CycleMove       PL_PurseMugToss
	CycleMove       PL_PurseMugLeft
	CycleMove       PL_PurseMug

	Type		male
	  Anim          male\PL_PurseMugRight 1
	  Playfx	Generic\FX_OpenPurse.fx  0 Constant
	TEnd
MEnd

## PURSESNATCH FORWARD ###########################################################

Move PL_PurseSnatchRun
	Requires	PURSESEARCH FORWARD
	Priority        2
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle
	Scale           1.45
	MoveRate	0.7
	NextMove	PL_PurseMug

	Type		male
	  Anim          male\PurseSnatchRun 1
	  PlayFx	GENERIC\FX_PurseSnatchRun.fx 0 Constant
	TEnd
MEnd


#############################################################
## CARRY JUNK MODE
#############################################################

Move Walk_Junk
	Requires	FORWARD CARRY HIGH
	Priority	3
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle
	Scale           0.50

	Type		male
	  Anim          male\plank_walk 1
	  PlayFx        GENERIC\PROP_GURDER.FX  0 Constant
	TEnd
MEnd


#############################################################
##  SCAN WALK
#############################################################
#
#Move ScanWalk_Center
#	Requires	WALK FORWARD Scan
#	Interrupts	<MonsterGroup> <Ready> <PostMoveCycle> <Combat> <CombatAttack> <MoveIRQ>
#	Member		<BasicMove> <ReactIRQ> <DeathIRQ> <AttackIRQ>
#	Flags		Cycle  MoveScale SmoothSprint AlwaysSizeScale
#	CycleMove	ScanWalk_PreLeft
#	CycleMove	ScanWalk_PreRight
#	NextMove        ScanWalk_Post
#	Scale		2.4
#	MoveRate        0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
#	Priority	3
#
#	Type		male
#	  Anim          male\mekmen_walk_scan 1 25
#	  PlayFx        FEET\ROBOFOOT2.FX  1
#	  PlayFx        FEET\ROBOFOOT1.FX  12
#	  SmoothSprint  2.8 ##feet from root to hip position you want to use to smooth sprinting
#	TEnd
#MEnd
#
#Move ScanWalk_PreRight
#	Requires	WALK FORWARD Scan
#	Member		<BasicMove> <ReactIRQ> <DeathIRQ> <AttackIRQ>
#	Flags		Cycle  MoveScale SmoothSprint AlwaysSizeScale
#	CycleMove	ScanWalk_Right
#	NextMove        ScanWalk_Post
#	Scale		2.4
#	MoveRate        0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
#	Priority	3
#
#	Type		male
#	  Anim          male\mekmen_walk_scan 27 75
#	  PlayFx        FEET\ROBOFOOT2.FX  1
#	  PlayFx        FEET\ROBOFOOT1.FX  12
#	  PlayFx        FEET\ROBOFOOT2.FX  24
#	  PlayFx        FEET\ROBOFOOT1.FX  36
#	  SmoothSprint  2.8 ##feet from root to hip position you want to use to smooth sprinting
#	TEnd
#MEnd
#
#Move ScanWalk_Right
#	Requires	WALK FORWARD Scan
#	Member		<BasicMove> <ReactIRQ> <DeathIRQ> <AttackIRQ>
#	Flags		Cycle  MoveScale SmoothSprint AlwaysSizeScale
#	CycleMove	ScanWalk_Right
#	CycleMove	ScanWalk_PostRight
#	NextMove        ScanWalk_Post
#	Scale		2.4
#	MoveRate        0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
#	Priority	3
#
#	Type		male
#	  Anim          male\mekmen_walk_scan 77 101
#	  PlayFx        FEET\ROBOFOOT2.FX  1
#	  PlayFx        FEET\ROBOFOOT1.FX  12
#	  SmoothSprint  2.8 ##feet from root to hip position you want to use to smooth sprinting
#	TEnd
#MEnd
#
#Move ScanWalk_PostRight
#	Requires	WALK FORWARD Scan
#	Member		<BasicMove> <ReactIRQ> <DeathIRQ> <AttackIRQ>
#	Flags		Cycle  MoveScale SmoothSprint AlwaysSizeScale
#	CycleMove	ScanWalk_Center
#	NextMove        ScanWalk_Post
#	Scale		2.4
#	MoveRate        0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
#	Priority	3
#
#	Type		male
#	  Anim          male\mekmen_walk_scan 103 151
#	  PlayFx        FEET\ROBOFOOT2.FX  1
#	  PlayFx        FEET\ROBOFOOT1.FX  12
#	  PlayFx        FEET\ROBOFOOT2.FX  24
#	  PlayFx        FEET\ROBOFOOT1.FX  36
#	  SmoothSprint  2.8 ##feet from root to hip position you want to use to smooth sprinting
#	TEnd
#MEnd
#
#Move ScanWalk_PreLeft
#	Requires	WALK FORWARD Scan
#	Member		<BasicMove> <ReactIRQ> <DeathIRQ> <AttackIRQ>
#	Flags		Cycle  MoveScale SmoothSprint AlwaysSizeScale
#	CycleMove	ScanWalk_Left
#	NextMove        ScanWalk_Post
#	Scale		2.4
#	MoveRate        0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
#	Priority	3
#
#	Type		male
#	  Anim          male\mekmen_walk_scan 153 201
#	  PlayFx        FEET\ROBOFOOT2.FX  1
#	  PlayFx        FEET\ROBOFOOT1.FX  12
#	  PlayFx        FEET\ROBOFOOT2.FX  24
#	  PlayFx        FEET\ROBOFOOT1.FX  36
#	  SmoothSprint  2.8 ##feet from root to hip position you want to use to smooth sprinting
#	TEnd
#MEnd
#
#Move ScanWalk_Left
#	Requires	WALK FORWARD Scan
#	Member		<BasicMove> <ReactIRQ> <DeathIRQ> <AttackIRQ>
#	Flags		Cycle  MoveScale SmoothSprint AlwaysSizeScale
#	CycleMove	ScanWalk_Left
#	CycleMove	ScanWalk_PostLeft
#	NextMove        ScanWalk_Post
#	Scale		2.4
#	MoveRate        0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
#	Priority	3
#
#	Type		male
#	  Anim          male\mekmen_walk_scan 203 227
#	  PlayFx        FEET\ROBOFOOT2.FX  1
#	  PlayFx        FEET\ROBOFOOT1.FX  12
#	  SmoothSprint  2.8 ##feet from root to hip position you want to use to smooth sprinting
#	TEnd
#MEnd
#
#Move ScanWalk_PostLeft
#	Requires	WALK FORWARD Scan
#	Member		<BasicMove> <ReactIRQ> <DeathIRQ> <AttackIRQ>
#	Flags		Cycle  MoveScale SmoothSprint AlwaysSizeScale
#	CycleMove	ScanWalk_Center
#	NextMove        ScanWalk_Post
#	Scale		2.4
#	MoveRate        0.7  ## 0.7 x 30 = I animated this to move at 21 feet per second
#	Priority	3
#
#	Type		male
#	  Anim          male\mekmen_walk_scan 229 277
#	  PlayFx        FEET\ROBOFOOT2.FX  1
#	  PlayFx        FEET\ROBOFOOT1.FX  12
#	  PlayFx        FEET\ROBOFOOT2.FX  24
#	  PlayFx        FEET\ROBOFOOT1.FX  36
#	  SmoothSprint  2.8 ##feet from root to hip position you want to use to smooth sprinting
#	TEnd
#MEnd
#
#Move ScanWalk_Post
#	Member		<ReactIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ>
#	NextMove        Ready
#	Priority	0
#	Interpolate     8
#
#	Type		male
#	  Anim          5cw\monster_postrun 1
#	  PlayFx        FEET\ROBOLAND1.FX  3
#	TEnd
#MEnd


###################
### Pipe Lookout   #####
###################

Move PipeLookout_A
	Requires	COMBAT WEAPON BLUNT LOOKOUT HIGH 2HAND
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove       PipeLookout_B
	CycleMove       PipeLookout_C
	Scale           0.50

	Type		male
	  Anim          male\bathold_lookright 1
	  PlayFx        Generic\prop_pipe_small.fx 0 Constant
	TEnd
MEnd

#############################################################

Move PipeLookout_B
	Requires	COMBAT WEAPON BLUNT LOOKOUT HIGH 2HAND
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	Scale           0.90
	CycleMove       PipeLookout_C
	CycleMove       PipeLookout_A

	Type		male
	  Anim          male\bathold_lookleft 1
	  PlayFx        Generic\prop_pipe_small.fx 0 Constant
	TEnd
MEnd

#############################################################

Move PipeLookout_C
	Requires	COMBAT WEAPON BLUNT LOOKOUT HIGH 2HAND
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	Scale           0.7
	CycleMove       PipeLookout_A
	CycleMove       PipeLookout_B

	Type		male
	  Anim          male\bathold_lookleft 1
	  PlayFx        Generic\prop_pipe_small.fx 0 Constant
	TEnd
MEnd


###################
### Pipe patrol walk##
###################

Move PipePatrol_pre
	Requires	FORWARD COMBAT WEAPON BLUNT LOOKOUT HIGH 2HAND
	Priority	7
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		ReqInputs
	NextMove        PipePatrol_cycle

	Type		male
	  Anim          male\walk_patrol_mace 1
 	  PlayFx        Generic\prop_pipe_small.fx 0 Constant
	TEnd
MEnd

Move PipePatrol_cycle
	Requires	FORWARD COMBAT WEAPON BLUNT LOOKOUT HIGH 2HAND
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle
	NextMove        PipePatrol_post
	CycleMove	PipePatrol_cycle_02
	CycleMove	PipePatrol_cycle_03
	CycleMove	PipePatrol_cycle

	Type		male
	  Anim          male\walk_patrol_mace 1
 	  PlayFx        Generic\prop_pipe_small.fx 0 Constant
	TEnd
MEnd

Move PipePatrol_cycle_02
	Requires	FORWARD COMBAT WEAPON BLUNT LOOKOUT HIGH 2HAND
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle
	NextMove        PipePatrol_post
	CycleMove	PipePatrol_cycle_03
	CycleMove	PipePatrol_cycle
	CycleMove	PipePatrol_cycle_02

	Type		male
	  Anim          male\walk_patrol_mace 1
 	  PlayFx        Generic\prop_pipe_small.fx 0 Constant
	TEnd
MEnd

Move PipePatrol_cycle_03
	Requires	FORWARD COMBAT WEAPON BLUNT LOOKOUT HIGH 2HAND
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle
	NextMove        PipePatrol_post
	CycleMove	PipePatrol_cycle
	CycleMove	PipePatrol_cycle_02
	CycleMove	PipePatrol_cycle_03

	Type		male
	  Anim          male\walk_patrol_mace 1
 	  PlayFx        Generic\prop_pipe_small.fx 0 Constant
	TEnd
MEnd

Move PipePatrol_post
	Requires	COMBAT WEAPON BLUNT LOOKOUT HIGH 2HAND
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        PipeLookout_A
	Scale		0.7
	Sets		CantMove

	Type		male
	  Anim          male\walk_patrol_mace 1
	  PlayFx        Generic\prop_pipe_small.fx 0 Constant
	TEnd
MEnd


#############################################################
## ZOMBIE MODE
#############################################################

Move Zombie_Ready
	Requires	YELL SAD HEAD ZOMBIE HIGH LAUGH
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale           0.50

	Type		male
	  Anim          male\zombie_ALT_ready 1
	TEnd
MEnd


## ZOMBIE FORWARD ###########################################################

Move Zombie_Forward
	Requires	FORWARD YELL SAD HEAD ZOMBIE HIGH LAUGH
	Priority	7
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle MoveScale
	NextMove        Zombie_Forward_Post

	Type		male
	  Anim          male\zombie_walk_armsdown 1 27
	  MoveRate	0.18
	TEnd
MEnd

Move Zombie_Forward_Post
	Requires	YELL SAD HEAD ZOMBIE HIGH LAUGH
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        Zombie_Ready

	Type		male
	  Anim          male\zombie_postrun 1
	TEnd
MEnd

## DUST Patrol Alert ###########################################################

Move Encounter_Rifle_Alert_Patrol_Pre
	Requires	 LOITER ALERT HIGH
	Priority	3
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		ReqInputs Cycle NoSizeScale
	CycleMove        Encounter_Rifle_Alert_Patrol_Pre
	CycleMove        Encounter_Rifle_Alert_Patrol_Post
	CycleMove        Encounter_Rifle_Alert_Patrol_Cycle
	CycleMove        Encounter_Rifle_Alert_Patrol_Post

	Type		male
	  Anim          male\dust_alert_patrol 1
	  PlayFx        WEAPONS\Sniper_Rifle.FX  0 Constant
	  PlayFX	Feet\Foot_NPCMale.fx 1
	  PlayFX	Feet\Foot_NPCMale.fx 24
	TEnd

	Type		fem
	  Anim          male\dust_alert_patrol 1
	  PlayFx        WEAPONS\Sniper_Rifle.FX  0 Constant
	  PlayFX	Feet\Foot_NPCFemale.fx 1
	  PlayFX	Feet\Foot_NPCFemale.fx 24
	TEnd

	Type		huge
	  Anim          male\dust_alert_patrol 1
	  PlayFx        WEAPONS\Sniper_Rifle.FX  0 Constant
	  PlayFX	Feet\Foot_NPCMale.fx 1
	  PlayFX	Feet\Foot_NPCMale.fx 24
	TEnd

MEnd

Move Encounter_Rifle_Alert_Patrol_Cycle
	Requires	 LOITER ALERT HIGH
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle NoSizeScale
	CycleMove        Encounter_Rifle_Alert_Patrol_Pre
	CycleMove        Encounter_Rifle_Alert_Patrol_Post
	CycleMove        Encounter_Rifle_Alert_Patrol_Post
	CycleMove        Encounter_Rifle_Alert_Patrol_Cycle

	Type		male
	  Anim          male\dust_alert_patrol 1
	  PlayFx        WEAPONS\Sniper_Rifle.FX  0 Constant
	  PlayFX	Feet\Foot_NPCMale.fx 1
	  PlayFX	Feet\Foot_NPCMale.fx 24
	TEnd

	Type		fem
	  Anim          male\dust_alert_patrol 1
	  PlayFx        WEAPONS\Sniper_Rifle.FX  0 Constant
	  PlayFX	Feet\Foot_NPCFemale.fx 1
	  PlayFX	Feet\Foot_NPCFemale.fx 24
	TEnd

	Type		huge
	  Anim          male\dust_alert_patrol 1
	  PlayFx        WEAPONS\Sniper_Rifle.FX  0 Constant
	  PlayFX	Feet\Foot_NPCMale.fx 1
	  PlayFX	Feet\Foot_NPCMale.fx 24
	TEnd

MEnd

Move Encounter_Rifle_Alert_Patrol_Post
	Requires	 LOITER ALERT HIGH
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove        Encounter_Rifle_Alert_Patrol_Post
	CycleMove        Encounter_Rifle_Alert_Patrol_Post
	CycleMove        Encounter_Rifle_Alert_Patrol_Cycle
	CycleMove        Encounter_Rifle_Alert_Patrol_Pre

	Type		male
	  Anim          male\dust_alert_patrol 1
	  PlayFx        WEAPONS\Sniper_Rifle.FX  0 Constant
	  PlayFX	Feet\Foot_NPCMale.fx 1
	  PlayFX	Feet\Foot_NPCMale.fx 24
	TEnd

	Type		fem
	  Anim          male\dust_alert_patrol 1
	  PlayFx        WEAPONS\Sniper_Rifle.FX  0 Constant
	  PlayFX	Feet\Foot_NPCFemale.fx 1
	  PlayFX	Feet\Foot_NPCFemale.fx 24
	TEnd

	Type		huge
	  Anim          male\dust_alert_patrol 1
	  PlayFx        WEAPONS\Sniper_Rifle.FX  0 Constant
	  PlayFX	Feet\Foot_NPCMale.fx 1
	  PlayFX	Feet\Foot_NPCMale.fx 24
	TEnd


MEnd

## Shalice Casting While Walking ###########################################################

Move Shalice_Casting_Walk_Pre
	Requires	LOITER ENDGAME PATROL CAST
	Priority	4
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		ReqInputs Cycle NoSizeScale
	CycleMove        Shalice_Casting_Walk_Pre
	CycleMove        Shalice_Left_Casting_Walk_Cycle
	CycleMove        Shalice_Right_Casting_Walk_Cycle
	CycleMove        Shalice_Casting_Walk_Post

	Type		male
	  Anim          male\emote_left_arm_casting 1 65
	  PlayFX	Feet\NPCMaleLeft.fx 0
	  PlayFX	Feet\NPCMaleRight.fx 16
	  PlayFX	Feet\NPCMaleLeft.fx 32
	  PlayFX	Feet\NPCMaleRight.fx 48
	  PlayFX	Feet\NPCMaleLeft.fx 64
	  PlayFX	Feet\NPCMaleRight.fx 80
	  PlayFX	Feet\NPCMaleLeft.fx 96
	  PlayFX	Feet\NPCMaleRight.fx 112
	  PlayFX	Feet\NPCMaleLeft.fx 128
	TEnd

	Type		fem
	  Anim          fem\emote_left_arm_casting 1 65
	  PlayFX	Feet\NPCMaleLeft.fx 0
	  PlayFX	Feet\NPCMaleRight.fx 16
	  PlayFX	Feet\NPCMaleLeft.fx 32
	  PlayFX	Feet\NPCMaleRight.fx 48
	  PlayFX	Feet\NPCMaleLeft.fx 64
	  PlayFX	Feet\NPCMaleRight.fx 80
	  PlayFX	Feet\NPCMaleLeft.fx 96
	  PlayFX	Feet\NPCMaleRight.fx 112
	  PlayFX	Feet\NPCMaleLeft.fx 128
	TEnd

MEnd

Move Shalice_Right_Casting_Walk_Cycle
	Requires	LOITER ENDGAME PATROL CAST
	Priority	4
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle NoSizeScale
	CycleMove        Shalice_Casting_Walk_Pre
	CycleMove        Shalice_Left_Casting_Walk_Cycle
	CycleMove        Shalice_Right_Casting_Walk_Cycle
	CycleMove        Shalice_Casting_Walk_Post

	Type		male
	  Anim          male\emote_right_arm_casting 65	193
	  PlayFX	Feet\NPCMaleLeft.fx 0
	  PlayFX	Feet\NPCMaleRight.fx 16
	  PlayFX	Feet\NPCMaleLeft.fx 32
	  PlayFX	Feet\NPCMaleRight.fx 48
	  PlayFX	Feet\NPCMaleLeft.fx 64
	  PlayFX	Feet\NPCMaleRight.fx 80
	  PlayFX	Feet\NPCMaleLeft.fx 96
	  PlayFX	Feet\NPCMaleRight.fx 112
	  PlayFX	Feet\NPCMaleLeft.fx 128
	  PlayFX	ENEMYFX\Praetorian\MoM\Shalice_WalkCast_RightHand.fx
	TEnd

	Type		fem
	  Anim          fem\emote_right_arm_casting 65 193
	  PlayFX	Feet\NPCMaleLeft.fx 0
	  PlayFX	Feet\NPCMaleRight.fx 16
	  PlayFX	Feet\NPCMaleLeft.fx 32
	  PlayFX	Feet\NPCMaleRight.fx 48
	  PlayFX	Feet\NPCMaleLeft.fx 64
	  PlayFX	Feet\NPCMaleRight.fx 80
	  PlayFX	Feet\NPCMaleLeft.fx 96
	  PlayFX	Feet\NPCMaleRight.fx 112
	  PlayFX	Feet\NPCMaleLeft.fx 128
	  PlayFX	ENEMYFX\Praetorian\MoM\Shalice_WalkCast_RightHand.fx
	TEnd


MEnd


Move Shalice_Left_Casting_Walk_Cycle
	Requires	LOITER ENDGAME PATROL CAST
	Priority	4
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle NoSizeScale
	CycleMove        Shalice_Casting_Walk_Pre
	CycleMove        Shalice_Left_Casting_Walk_Cycle
	CycleMove        Shalice_Right_Casting_Walk_Cycle
	CycleMove        Shalice_Casting_Walk_Post

	Type		male
	  Anim          male\emote_left_arm_casting 65 193
	  PlayFX	Feet\NPCMaleLeft.fx 0
	  PlayFX	Feet\NPCMaleRight.fx 16
	  PlayFX	Feet\NPCMaleLeft.fx 32
	  PlayFX	Feet\NPCMaleRight.fx 48
	  PlayFX	Feet\NPCMaleLeft.fx 64
	  PlayFX	Feet\NPCMaleRight.fx 80
	  PlayFX	Feet\NPCMaleLeft.fx 96
	  PlayFX	Feet\NPCMaleRight.fx 112
	  PlayFX	Feet\NPCMaleLeft.fx 128
	  PlayFX	ENEMYFX\Praetorian\MoM\Shalice_WalkCast_LeftHand.fx
	TEnd

	Type		fem
	  Anim          fem\emote_left_arm_casting 65 193
	  PlayFX	Feet\NPCMaleLeft.fx 0
	  PlayFX	Feet\NPCMaleRight.fx 16
	  PlayFX	Feet\NPCMaleLeft.fx 32
	  PlayFX	Feet\NPCMaleRight.fx 48
	  PlayFX	Feet\NPCMaleLeft.fx 64
	  PlayFX	Feet\NPCMaleRight.fx 80
	  PlayFX	Feet\NPCMaleLeft.fx 96
	  PlayFX	Feet\NPCMaleRight.fx 112
	  PlayFX	Feet\NPCMaleLeft.fx 128
	  PlayFX	ENEMYFX\Praetorian\MoM\Shalice_WalkCast_LeftHand.fx
	TEnd


MEnd


Move Shalice_Casting_Walk_Post
	Requires	LOITER ENDGAME PATROL CAST
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale ReqInputs
	CycleMove        Shalice_Casting_Walk_Pre
	CycleMove        Shalice_Left_Casting_Walk_Cycle
	CycleMove        Shalice_Right_Casting_Walk_Cycle
	CycleMove        Shalice_Casting_Walk_Post

	Type		male
	  Anim          male\emote_left_arm_casting 193 321
	  PlayFX	Feet\NPCMaleLeft.fx 0
	  PlayFX	Feet\NPCMaleRight.fx 16
	  PlayFX	Feet\NPCMaleLeft.fx 32
	  PlayFX	Feet\NPCMaleRight.fx 48
	  PlayFX	Feet\NPCMaleLeft.fx 64
	  PlayFX	Feet\NPCMaleRight.fx 80
	  PlayFX	Feet\NPCMaleLeft.fx 96
	  PlayFX	Feet\NPCMaleRight.fx 112
	  PlayFX	Feet\NPCMaleLeft.fx 128
	TEnd

	Type		fem
	  Anim          fem\emote_left_arm_casting 193 321
	  PlayFX	Feet\NPCMaleLeft.fx 0
	  PlayFX	Feet\NPCMaleRight.fx 16
	  PlayFX	Feet\NPCMaleLeft.fx 32
	  PlayFX	Feet\NPCMaleRight.fx 48
	  PlayFX	Feet\NPCMaleLeft.fx 64
	  PlayFX	Feet\NPCMaleRight.fx 80
	  PlayFX	Feet\NPCMaleLeft.fx 96
	  PlayFX	Feet\NPCMaleRight.fx 112
	  PlayFX	Feet\NPCMaleLeft.fx 128
	TEnd

MEnd

######################################################

Move Cry_Walk_Pre
	Requires	IDLE WALK CRY
	Priority	3
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		ReqInputs
	NextMove        Cry_Walk_Cycle

	Type		male
	  Anim          male\walk_cry 1 72
	TEnd

MEnd

Move Cry_Walk_Cycle
	Requires	IDLE WALK CRY
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle
	NextMove        Idle_Walk_Post

	Type		male
	  Anim          male\walk_cry 1 72
	TEnd

MEnd

