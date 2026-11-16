#############################################################
## WALKS
#############################################################

Move WalkCycle
	Requires	FORWARD WALK
	Priority        2
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle MoveScale
	CycleMove	WalkCycle
	CycleMove	WalkCycle
	CycleMove	WaveWalkCycle
	NextMove        Ready
	Scale           1.1
	MoveRate	0.146

	Type		bis
	  Anim          male\npc_walk 1 48
	  PlayFx	FEET\Foot_NPCMale.fx 10
	  PlayFx	FEET\Foot_NPCMale.fx 34
	TEnd

	Type		worker
	  Anim          male\gurder_walk 1
	  PlayFx        GENERIC\PROP_GURDER.FX  0 CONSTANT
	  PlayFx	FEET\Foot_NPCMale.fx 10
	  PlayFx	FEET\Foot_NPCMale.fx 34
	  Scale         0.5
	TEnd
MEnd

Move WaveWalkCycle
	Requires	FORWARD WALK
	Priority        2
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle MoveScale
	CycleMove	WalkCycle
	NextMove        Ready
	Scale           1.3
	MoveRate	0.146

	Type		bis
	  Anim          male\wave_walk 1
	  PlayFx	FEET\Foot_NPCMale.fx 10
	  PlayFx	FEET\Foot_NPCMale.fx 34
	TEnd

	Type		worker
	  Anim          male\gurder_walk 1
	  PlayFx        GENERIC\PROP_GURDER.FX  0 CONSTANT
	  PlayFx	FEET\Foot_NPCMale.fx 10
	  PlayFx	FEET\Foot_NPCMale.fx 34
	  Scale         0.5
	TEnd

	Type		ghost
	  Anim          male\npc_walk 1 48
	TEnd
MEnd

## STROLL ###########################################################

Move StrollCycle
	Requires	FORWARD WALK
	Priority        2
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle MoveScale
	NextMove        Ready  ##WalkPost
	Scale           1.3
	MoveRate	0.146

	Type		bis
	  Anim          male\npc_walk_armsback 1 48
	  PlayFx	FEET\Foot_NPCMale.fx 10
	  PlayFx	FEET\Foot_NPCMale.fx 34
	TEnd

	Type		worker
	  Anim          male\gurder_walk 1
	  PlayFx        GENERIC\PROP_GURDER.FX  0 CONSTANT
	  PlayFx	FEET\Foot_NPCMale.fx 10
	  PlayFx	FEET\Foot_NPCMale.fx 34
	  Scale         0.5
	TEnd
MEnd

#############################################################
## CELLPHONE WALK
#############################################################

Move CellphoneCycle
	Requires	FORWARD WALK CELLPHONE
	Priority        3
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle MoveScale
	NextMove        Ready  ##WalkPost
	Scale           1.3
	MoveRate	0.146

	Type		bis
	  Anim          male\cellphone_walk 1
	  PlayFx	FEET\Foot_NPCMale.fx 10
	  PlayFx	FEET\Foot_NPCMale.fx 34
	  PlayFx	FEET\Foot_NPCMale.fx 58
	  PlayFx	FEET\Foot_NPCMale.fx 82
	  PlayFx	FEET\Foot_NPCMale.fx 106
	  PlayFx	FEET\Foot_NPCMale.fx 130
	  PlayFx	FEET\Foot_NPCMale.fx 154
	  PlayFx	FEET\Foot_NPCMale.fx 178
	  PlayFx	FEET\Foot_NPCMale.fx 202
	  PlayFx	FEET\Foot_NPCMale.fx 226
	  PlayFx	FEET\Foot_NPCMale.fx 250
	  PlayFx	FEET\Foot_NPCMale.fx 274
	  PlayFx	FEET\Foot_NPCMale.fx 298
	  PlayFx	FEET\Foot_NPCMale.fx 322
	  PlayFx	FEET\Foot_NPCMale.fx 346
	  PlayFx	FEET\Foot_NPCMale.fx 370
	  PlayFx	FEET\Foot_NPCMale.fx 394
	  PlayFx	FEET\Foot_NPCMale.fx 418
	  PlayFx	FEET\Foot_NPCMale.fx 442
	  PlayFx	FEET\Foot_NPCMale.fx 466
	  PlayFx        GENERIC\FX_cellphone.FX 0 CONSTANT
	TEnd

	Type		ghost
	  Anim          male\npc_walk 1 48
	TEnd
MEnd

#############################################################
## CLIPBOARD WALKS
#############################################################

## CLIPBOARD FORWARD ###########################################################

Move ClipboardCycle
	Requires	FORWARD WALK CLIPBOARD
	Priority        3
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle MoveScale
	NextMove        Ready  ##WalkPost
	Scale           1.3
	MoveRate	0.146

	Type		bis
	  Anim          male\clipboard_walk 1
	  PlayFx        GENERIC\FX_clipboard.FX 0 CONSTANT
	  PlayFx        GENERIC\FX_clipboard_SFX.FX 0 CONSTANT
	  PlayFx	FEET\Foot_NPCMale.fx 10
	  PlayFx	FEET\Foot_NPCMale.fx 34
	TEnd

	Type		ghost
	  Anim          male\npc_walk 1 48
	TEnd
MEnd

## SIDECLIPBOARD FORWARD ###########################################################

Move SideClipboardCycle
	Requires	FORWARD WALK CLIPBOARD
	Priority        3
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle MoveScale
	NextMove        Ready  ##WalkPost
	Scale           1.3
	MoveRate	0.146

	Type		bis
	  Anim          male\sideclipboard_walk 1
	  PlayFx        GENERIC\FX_clipboard.FX 0 CONSTANT
	  PlayFx	FEET\Foot_NPCMale.fx 10
	  PlayFx	FEET\Foot_NPCMale.fx 34
	TEnd

	Type		ghost
	  Anim          male\npc_walk 1 48
	TEnd
MEnd

#############################################################
## BRIEFCASE WALK
#############################################################

Move BriefcaseCycle
	Requires	FORWARD WALK BRIEFCASE
	Priority        3
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle MoveScale
	NextMove        Ready  ##WalkPost
	Scale           1.3
	MoveRate	0.146

	Type		bis
	  Anim          male\briefcase_walk 1
	  PlayFx        GENERIC\FX_briefcase_R.FX 0 CONSTANT
	  PlayFx	FEET\Foot_NPCMale.fx 10
	  PlayFx	FEET\Foot_NPCMale.fx 34
	TEnd

	Type		worker
	  Anim          male\briefcase_walk 1
	  PlayFx        GENERIC\FX_toolbox.FX 0 CONSTANT
	  PlayFx	FEET\Foot_NPCMale.fx 10
	  PlayFx	FEET\Foot_NPCMale.fx 34
	TEnd

	Type		ghost
	  Anim          male\npc_walk 1 48
	TEnd
MEnd

#############################################################
## GROCERIES WALK
#############################################################

## CARRYING GROCERIES LEFT###########################################################

Move Walk_Groceries_Left
	Requires	FORWARD WALK GROCERIES
	Priority        3
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle MoveScale
	NextMove        Ready  ##WalkPost

	Type		Male
	  Anim          Male\walkV_groceries_L 1
	  PlayFx        GENERIC\groceries_l.fx 0 CONSTANT
	  PlayFx	FEET\Foot_NPCMale.fx 10
	  PlayFx	FEET\Foot_NPCMale.fx 34
	  MoveRate      0.18
	TEnd
MEnd

## CARRYING GROCERIES RIGHT###########################################################

Move Walk_Groceries_Right
	Requires	FORWARD WALK GROCERIES
	Priority        3
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle MoveScale
	NextMove        Ready  ##WalkPost

	Type		Male
	  Anim          Male\walkV_groceries_R 1
	  PlayFx        GENERIC\groceries_r.fx 0 CONSTANT
	  PlayFx	FEET\Foot_NPCMale.fx 10
	  PlayFx	FEET\Foot_NPCMale.fx 34
	  MoveRate      0.18
	TEnd
MEnd

## CARRYING GROCERIES 2HAND ###########################################################

Move Walk_Groceries_2handed
	Requires	FORWARD WALK GROCERIES
	Priority        3
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle MoveScale
	NextMove        Ready

	Type		Male
	  Anim          Male\walkV_groceries_2h 1
	  PlayFx        GENERIC\groceries_r.fx 0 CONSTANT
	  PlayFx	FEET\Foot_NPCMale.fx 10
	  PlayFx	FEET\Foot_NPCMale.fx 34
	  MoveRate      0.18
	TEnd
MEnd

## CARRYING GROCERIES BOTH ###########################################################

Move Walk_Groceries_Left_Right
	Requires	FORWARD WALK GROCERIES
	Priority        3
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle MoveScale
	NextMove        Ready

	Type		Male
	  Anim          Male\walkV_groceries_LR 1
	  PlayFx        GENERIC\groceries_l.fx 0 CONSTANT
	  PlayFx        GENERIC\groceries_r.fx 0 CONSTANT
	  PlayFx	FEET\Foot_NPCMale.fx 10
	  PlayFx	FEET\Foot_NPCMale.fx 34
	  MoveRate      0.18
	TEnd
MEnd

#############################################################
## OPPRESSED WALKS
#############################################################

## OPPRESSED ###########################################################

Move Oppressed_Walk
	Requires	FORWARD WALK SAD
	Priority        3
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle MoveScale
	NextMove        Ready

	Type		Male
	  Anim          Male\walk_oppress 1
	  PlayFx	FEET\Foot_NPCMale.fx 10
	  PlayFx	FEET\Foot_NPCMale.fx 34
	  MoveRate      0.14
	TEnd
MEnd

## OPPRESSED BRIEFCASE LEFT ###########################################################

Move Oppressed_Walk_Briefcase_Left
	Requires	FORWARD WALK BRIEFCASE SAD
	Priority        4
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle MoveScale
	NextMove        Ready

	Type		Male
	  Anim          Male\walk_oppress_hold_l 1
	  PlayFx        GENERIC\FX_briefcase_L.fx 0 CONSTANT
	  PlayFx	FEET\Foot_NPCMale.fx 10
	  PlayFx	FEET\Foot_NPCMale.fx 34
	  MoveRate      0.18
	TEnd
MEnd

## OPPRESSED BRIEFCASE RIGHT ###########################################################

Move Oppressed_Walk_Briefcase_Right
	Requires	FORWARD WALK BRIEFCASE SAD
	Priority        4
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle MoveScale
	NextMove        Ready

	Type		Male
	  Anim          Male\walk_oppress_hold_r 1
	  PlayFx        GENERIC\FX_briefcase_R.fx 0 CONSTANT
	  PlayFx	FEET\Foot_NPCMale.fx 10
	  PlayFx	FEET\Foot_NPCMale.fx 34
	  MoveRate      0.18
	TEnd
MEnd

## OPPRESSED BRIEFCASE BOTH ###########################################################

Move Oppressed_Walk_Briefcase_Both
	Requires	FORWARD WALK BRIEFCASE BUM
	Priority        3
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle MoveScale
	NextMove        Ready

	Type		Male
	  Anim          Male\walk_oppress_hold_both 1
	  PlayFx        GENERIC\FX_briefcase_R.fx 0 CONSTANT
	  PlayFx        GENERIC\FX_briefcase_L.fx 0 CONSTANT
	  PlayFx	FEET\Foot_NPCMale.fx 10
	  PlayFx	FEET\Foot_NPCMale.fx 34
	  MoveRate      0.18
	TEnd
MEnd

#############################################################
## NERVOUS WALKS
#############################################################

## WALK NERVOUS ###########################################################

Move Walk_Nervous
	Requires	FORWARD WALK LOOKOUT THREAT
	Priority        4
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle
	NextMove        Ready
	scale		0.75

	Type		male
	  Anim          male\Walk_Nervous 1
	  PlayFx	FEET\Foot_NPCMale.fx 10
	  PlayFx	FEET\Foot_NPCMale.fx 34
	TEnd
MEnd

## WALK NERVOUS WITH BRIEFCASE ###########################################################

Move Walk_NervousCase
	Requires	FORWARD WALK LOOKOUT THREAT BRIEFCASE
	Priority        5
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle
	NextMove        Ready
	scale		0.75
	CycleMove       Walk_NervousCase
	CycleMove       Walk_NervousCase
	CycleMove       Walk_NervousCaseRight
	CycleMove       Walk_NervousCaseLeft

	Type		male
	  Anim          male\Walk_NervousCase 1
	  PlayFx        GENERIC\BriefCaseClutch.fx  0 CONSTANT
	  PlayFx	FEET\Foot_NPCMale.fx 10
	  PlayFx	FEET\Foot_NPCMale.fx 34
	TEnd
MEnd

Move Walk_NervousCaseRight
	Requires	FORWARD WALK LOOKOUT THREAT BRIEFCASE
	Priority        5
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle
	NextMove        Ready
	scale		0.75
	CycleMove       Walk_NervousCase

	Type		male
	  Anim          male\Walk_NervousCaseR 1
	  PlayFx        GENERIC\BriefCaseClutch.fx  0 CONSTANT
	  PlayFx	FEET\Foot_NPCMale.fx 10
	  PlayFx	FEET\Foot_NPCMale.fx 34
	TEnd
MEnd

Move Walk_NervousCaseLeft
	Requires	FORWARD WALK LOOKOUT THREAT BRIEFCASE
	Priority        5
	Interrupts	<Ready> <PostMoveCycle> RunPre RunCycle
	Member		<BasicMove> <ReactIRQ> <DeathIRQ>
	Flags		Cycle
	NextMove        Ready
	scale		0.75
	CycleMove       Walk_NervousCase

	Type		male
	  Anim          male\Walk_NervousCaseL 1
	  PlayFx        GENERIC\BriefCaseClutch.fx  0 CONSTANT
	  PlayFx	FEET\Foot_NPCMale.fx 10
	  PlayFx	FEET\Foot_NPCMale.fx 34
	TEnd
MEnd

#############################################################
## BUM WALKS  (COV STUFF)
#############################################################

## BUM WALK GARBAGE BAG ###########################################################

Move Walk_GarbageBag
	Requires	FORWARD WALK BUM
	Priority        3
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle
	NextMove        Ready

	Type		male
	  Anim          male\Walk_GarbageBag 1
	  PlayFx        GENERIC\GarbageBag_Carriable.fx 0 CONSTANT
	  PlayFx	FEET\Foot_NPCMale.fx 10
	  PlayFx	FEET\Foot_NPCMale.fx 34
	TEnd
MEnd

## BUM WALK NERVOUS ###########################################################

Move Walk_Nervous_Bum
	Requires	FORWARD WALK BUM
	Priority        3
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle
	NextMove        Ready
	scale		0.75

	Type		male
	  Anim          male\Walk_Nervous 1
	  PlayFx	FEET\Foot_NPCMale.fx 10
	  PlayFx	FEET\Foot_NPCMale.fx 34
	TEnd
MEnd

## SCHIZO WALK (BANGING HEAD) ###########################################################

Move walk_schizo1
	Requires	WALK FORWARD BUM
	Priority        3
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle MoveScale PICKRANDOMLY
	Interpolate	20
	cyclemove	walk_schizo2
	cyclemove	walk_schizo3
	cyclemove	walk_schizo1

	Type		Male
	  Anim          Male\walk_schizo1 1
	  PlayFx	FEET\Foot_NPCMale.fx 10
	  PlayFx	FEET\Foot_NPCMale.fx 22
	  MoveRate      0.18
	TEnd
MEnd

Move walk_schizo2
	Requires	WALK FORWARD BUM
	Priority        3
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle MoveScale PICKRANDOMLY
	Interpolate	15
	cyclemove	walk_schizo3
	cyclemove	walk_schizo1
	cyclemove	walk_schizo2

	Type		Male
	  Anim          Male\walk_schizo2 1
	  PlayFx	FEET\Foot_NPCMale.fx 10
	  PlayFx	FEET\Foot_NPCMale.fx 22
	  MoveRate      0.18
	TEnd
MEnd

Move walk_schizo3
	Requires	WALK FORWARD BUM
	Priority        3
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle MoveScale PICKRANDOMLY
	Interpolate	15
	cyclemove	walk_schizo1
	cyclemove	walk_schizo2
	cyclemove	walk_schizo3

	Type		Male
	  Anim          Male\walk_schizo3 1
	  PlayFx	FEET\Foot_NPCMale.fx 10
	  PlayFx	FEET\Foot_NPCMale.fx 22
	  MoveRate      0.18
	TEnd
MEnd

## WALK SULLEN ###########################################################

Move Walk_Sullen
	Requires	FORWARD WALK BUM
	Priority        3
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle
	NextMove        Ready
	scale		0.75

	Type		male
	  Anim          male\Walk_Sullen 1
	  PlayFx	FEET\Foot_NPCMale.fx 10
	  PlayFx	FEET\Foot_NPCMale.fx 34
	TEnd
MEnd

## WALK SHOPPINGCART ###########################################################

Move Walk_ShoppingCart7
	Requires	FORWARD WALK BUM
	Priority        3
	Interrupts	<Ready> <PostMoveCycle> RunPre RunCycle
	Member		<BasicMove> <ReactIRQ> <DeathIRQ>
	Flags		Cycle
	NextMove        Ready
	scale		0.75

	Type		male
	  Anim          male\Walk_ShopCart 1
	  PlayFx        GENERIC\ShoppingCart.fx  0 CONSTANT
	  PlayFx	GENERIC\Garbage_flys.fx	0 CONSTANT
	  PlayFx	FEET\Foot_NPCMale.fx 10
	  PlayFx	FEET\Foot_NPCMale.fx 22
	TEnd
MEnd

##  WALK PARANOID ###########################################################

Move Walk_Paranoid
	Requires	FORWARD WALK BUM
	Priority        3
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle
	NextMove        Ready
	Scale		0.75
	CycleMove       Walk_Paranoid
	CycleMove       Walk_Paranoid_LeftIn
	CycleMove       Walk_Paranoid_RightIn

	Type		male
	  Anim          male\Walk_Paranoid 1
	  PlayFx	FEET\Foot_NPCMale.fx 10
	  PlayFx	FEET\Foot_NPCMale.fx 34
	TEnd
MEnd

Move Walk_Paranoid_LeftIn
	Requires	FORWARD WALK BUM
	Priority        3
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle
	NextMove        Ready
	Scale		0.75
	CycleMove       Walk_Paranoid_LeftOut
	CycleMove       Walk_Paranoid_LeftLoop

	Type		male
	  Anim          male\Walk_Paranoid_Left 1 26
	TEnd
MEnd

Move Walk_Paranoid_LeftLoop
	Requires	FORWARD WALK BUM
	Priority        3
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle
	NextMove        Ready
	Scale		0.75
	CycleMove       Walk_Paranoid_LeftOut

	Type		male
	  Anim          male\Walk_Paranoid_Left 26 58
	  PlayFx	FEET\Foot_NPCMale.fx 10
	  PlayFx	FEET\Foot_NPCMale.fx 34
	TEnd
MEnd

Move Walk_Paranoid_LeftOut
	Requires	FORWARD WALK Bum
	Priority        3
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle
	NextMove        Ready
	Scale		0.75
	CycleMove       Walk_Paranoid
	CycleMove       Walk_Paranoid_RightIn

	Type		male
	  Anim          male\Walk_Paranoid_Left 58
	TEnd
MEnd

Move Walk_Paranoid_RightIn
	Requires	FORWARD WALK BUM
	Priority        3
	Interrupts	<Ready> <PostMoveCycle> RunPre RunCycle
	Member		<BasicMove> <ReactIRQ> <DeathIRQ>
	Flags		Cycle
	NextMove        Ready
	Scale		0.75
	CycleMove       Walk_Paranoid_RightOut
	CycleMove       Walk_Paranoid_RightLoop

	Type		male
	  Anim          male\Walk_Paranoid_Right 1 38
	TEnd
MEnd

Move Walk_Paranoid_RightLoop
	Requires	FORWARD WALK BUM
	Priority        3
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle
	NextMove        Ready
	Scale		0.75
	CycleMove       Walk_Paranoid_RightOut

	Type		male
	  Anim          male\Walk_Paranoid_Right 38 70
	  PlayFx	FEET\Foot_NPCMale.fx 10
	  PlayFx	FEET\Foot_NPCMale.fx 34
	TEnd
MEnd

Move Walk_Paranoid_RightOut
	Requires	FORWARD WALK BUM
	Priority        3
	Interrupts	<Ready> <PostMoveCycle> RunPre RunCycle
	Member		<BasicMove> <ReactIRQ> <DeathIRQ>
	Flags		Cycle
	NextMove        Ready
	Scale		0.75
	CycleMove       Walk_Paranoid
	CycleMove       Walk_Paranoid_LeftIn

	Type		male
	  Anim          male\Walk_Paranoid_Right 70
	TEnd
MEnd