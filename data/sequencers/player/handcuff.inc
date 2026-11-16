###################
###### Handcuff Mode ######
###################

Move Pre_Handcuff
	Requires	HANDCUFF
	Interrupts	<Ready> <MonsterGroup> <WepCombat> <DualHandCombat> <Block> <HitIRQ>
	Member		<ReactIRQ> <DeathIRQ> <Handcuff> <AttackIRQ>
	Flags		ReqInputs
	NextMove        HandcuffReady
	Priority        1


	Type		male
	  Anim          male\handcuff_ready 1
	TEnd

MEnd


Move HandcuffReady
	Requires	HANDCUFF
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Handcuff> <AttackIRQ>
	Flags		Cycle
	CycleMove	HandcuffReady
	CycleMove	HandcuffReady_lookquick
	NextMove        postReady_back
	Priority        1

	Type		male
	  Anim          male\handcuff_ready 1
	TEnd

MEnd

Move HandcuffReady_lookquick
	Requires	HANDCUFF
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Handcuff> <AttackIRQ>
	Flags		Cycle
	CycleMove	HandcuffReady
	NextMove        postReady_back
	Priority        1
	Interpolate	8

	Type		male
	  Anim          male\handcuff_lookquick 1
	TEnd

MEnd


Move Handcuff_Breath
	Requires	HANDCUFF
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Handcuff> <AttackIRQ>
	NextMove        HandcuffReady_lookquick
	Priority        1


	Type		male
	  Anim          male\handcuff_breath_cycle 1
	TEnd

MEnd



###################
### Handcuff run Forward ##
###################

Move HC_RunPre
	Requires	HANDCUFF Forward
	Interrupts	<Handcuff> <PostMoveCycle> <HandcuffAttack> <Block> <HitIRQ>
	Member		<CombatMove> <DeathIRQ> <AttackIRQ>
	Flags		ReqInputs MoveScale SmoothSprint
	MoveRate        0.7
	NextMove        HC_RunCycle
	Priority        2

	Type		male
	  Anim          male\handcuff_run 1
	  PlayFx        FEET\MALELEFT.FX  10
	  PlayFx        FEET\MALERIGHT.FX  21
	  SmoothSprint  3.3
	TEnd

MEnd


Move HC_RunCycle
	Requires	HANDCUFF Forward
	Member		<HandcuffMove> <DeathIRQ> <AttackIRQ>
	Flags		Cycle MoveScale SmoothSprint
	MoveRate        0.7
	NextMove        HC_RunPost
	Priority        2

	Type		male
	  Anim          male\handcuff_run 1
	  PlayFx        FEET\MALELEFT.FX  10
	  PlayFx        FEET\MALERIGHT.FX  21
	  SmoothSprint  3.3
	TEnd

MEnd


Move HC_RunPost
	Requires	HANDCUFF
	Member		<HandcuffMove> <ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <PostMoveCycle>
	Flags		ReqInputs
	NextMove        Handcuff_Breath
	Priority        1


	Type		male
	  Anim          male\handcuff_Postrun 3
	  PlayFx	FEET\LAND3.FX:LAND  3
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 12
	TEnd

MEnd




#######################
###### Handcuff Jump  ###
#######################

Move HC_jumpPre
	Requires	JUMP Handcuff
	Interrupts	<PostMoveCycle> <Handcuff> <HandcuffMove> C_runjumpPre <iceslide> <HandcuffSwim> <Swim> <CombatAttack> <jumpFall> <Claw> <Block> <HitIRQ> <StunMove>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove        HC_Hop
	Priority        2

	Type		male
	  Anim          male\handcuff_jump 10 11
	TEnd

MEnd


Move HC_Hop
	Requires	JUMP Handcuff
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove        HC_Hop2
	Priority        2

	Type		male
	  Anim          male\handcuff_jump 12 87
	  PlayFx        FEET\JUMP1.FX
	TEnd
MEnd


Move HC_Hop2
	Requires	JUMP Handcuff
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove        HC_jumpApex
	Priority        2

	Type		male
	  Anim          male\handcuff_jump 89 99
	TEnd

MEnd


Move HC_jumpApex
	Requires	APEX Handcuff
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove        HC_jumpFall
	Priority        2

	Type		male
	  Anim          male\handcuff_jump 100 101
	TEnd

MEnd


Move HC_jumpFall
	Requires	AIR Handcuff
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove       HC_jumpFall2
	Priority        2

	Type		male
	  Anim          male\handcuff_jump 102 144
	TEnd

MEnd


Move HC_jumpFall2
	Requires	AIR Handcuff
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove        HC_JumpPost
	Priority        2

	Type		male
	  Anim          male\handcuff_jump 145
	TEnd

MEnd


Move HC_JumpPost
	Requires	Handcuff
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ>
	Flags		ReqInputs
	NextMove        Handcuff_Breath
	Priority        1
	Interpolate     1


	Type		male
	  Anim          male\handcuff_Postjump 2
	  PlayFx        FEET\LAND1.FX
	TEnd

MEnd




#####  Swim mode  ###################################################


##################
###  Swim ready  #####
##################

Move HC_swim_readypre
	Requires	SWIM HANDCUFF
	Interrupts	Ready <Swim>
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <HandcuffSwim> <AttackIRQ>
	Flags		ReqInputs FixHipsReg
	NextMove        HC_swim_ready
	Priority        2
	Scale           0.5

	Type		male
	  Anim          male\handcuff_swim 1
	TEnd

MEnd


Move HC_swim_ready
	Requires	SWIM HANDCUFF
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <HandcuffSwim> <AttackIRQ>
	Flags		Cycle FixHipsReg
	NextMove        Ready
	Priority        2
	Scale           0.5

	Type		male
	  Anim          male\handcuff_swim 1
	TEnd


MEnd


##################
### Handcuff swim forward  ####
##################

Move HC_SwimPre
	Requires	FORWARD SWIM HANDCUFF
	Interrupts	HC_swim_readypre HC_swim_ready <PostMoveCycle> <BasicMove> <HandcuffMove> <CombatMove> <WepMove> <TwoHandMove> <StunMove> <DualHandMove> <iceslide>
	Member		<HandcuffSwim> <ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ>
	Flags		ReqInputs FixHipsReg
	NextMove        HC_SwimCycle
	Priority        3
	Scale           0.75

	Type		male
	  Anim          male\handcuff_swim_forward 1 35
	  PlayFx        FEET\SWIM1.FX
	TEnd
MEnd


Move HC_SwimCycle
	Requires	FORWARD SWIM HANDCUFF
	Member		<HandcuffSwim> <ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ>
	Flags		Cycle FixHipsReg
	NextMove        HC_SwimPost
	Priority        3
	Scale           0.75

	Type		male
	  Anim          male\handcuff_swim_forward  1 35
	  PlayFx        FEET\SWIM1.FX
	TEnd
MEnd


Move HC_SwimPost
	Requires	SWIM HANDCUFF
	Priority        2
	Member		<HandcuffSwim> <ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ>
	Flags		FixHipsReg
	NextMove        HC_swim_ready
	Interpolate     20
	Scale           0.5

	Type		male
	  Anim          male\handcuff_swim 80
	TEnd

MEnd


##################################
### Handcuff Swim Postjump  ####
#################################

Move HC_Swim_JumpPost
	Requires	SWIM HANDCUFF
	Interrupts	HC_JumpPost JumpPost C_JumpPost W_JumpPost D_JumpPost RunJumpPostA C_RunJumpPostA W_RunJumpPostA SG_JumpPost
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <HandcuffSwim> <AttackIRQ>
	Flags		FixHipsReg
	NextMove        HC_swim_ready
	Priority        2
	Interpolate     5
	Scale           0.8

	Type		male
	  Anim          male\handcuff_swim_postjump 5
	TEnd

MEnd




#### Handcuff fly  ###########################

####################
#### Fly Mode  ####
####################

Move HC_Fall_Fready
	Requires	FLY HANDCUFF
	Priority        2
	Interrupts	<FlyFall>
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <FlyHandcuff>
	NextMove        HC_flyready
	Scale		0.5

	Type		male
	  Anim          male\handcuff_float 43
	TEnd
MEnd

Move HC_Preflyready
	Requires	FLY HANDCUFF
	Interrupts	<Handcuff> <HandcuffMove> <Ready>  <Jumps> <PostMoveCycle>
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <FlyHandcuff>
	Flags		ReqInputs
	NextMove        HC_flyready
	Priority        3
	Scale		0.5

	Type		male
	  Anim          male\handcuff_float 1 38
	TEnd

MEnd


Move HC_flyready
	Requires	FLY HANDCUFF
	Priority        3
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <FlyHandcuff>
	Flags		Cycle
	CycleMove	HC_flyready_2
	NextMove        HC_PreFall
	Scale		0.5

	Type		male
	  Anim          male\handcuff_float 39
	TEnd

MEnd


Move HC_flyready_2
	Requires	FLY HANDCUFF
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <FlyHandcuff>
	Flags		Cycle
	CycleMove	HC_flyready
	NextMove        HC_PreFall
	Priority        3
	Interpolate	20
	Scale		0.7

	Type		male
	  Anim          male\handcuff_float 39
	TEnd

MEnd



###################
###### Fly Down  ######
###################

Move HC_PreFall
	Requires	AIR HANDCUFF
	Priority        2
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Fall> <AttackIRQ>
	Flags		Cycle
	NextMove        HC_JumpPost

	Type		male
	  Anim          male\handcuff_fall 1
	TEnd

MEnd


#### canned moves  #####################################

###################
### Basic Runout  ####
###################

Move HC_Basic_runout
	Requires	EMERGE RUNOUT HANDCUFF
	Interrupts	<AttackIRQ> <HandcuffMove>  <Handcuff>  <PostMoveCycle> <RunIn>
	NextMove        HandcuffReady
	Priority        8
	Sets            CANTMOVE WALKTHROUGH
	Flags		DoNotDelay

	Type		male
	  Anim          male\handcuff_basic_runout 2
	TEnd

MEnd


###################
### Elevator Runout  ####  (this move is broken for now -steve)
###################

Move HC_Elevator_Wait
	Requires	EMERGE ELEVATOR HANDCUFF
	Interrupts	<AttackIRQ> <HandcuffMove>  <Handcuff>  <PostMoveCycle> <RunIn>
	NextMove        HC_Elevator_runout
	Priority        10
	Sets            CANTMOVE WALKTHROUGH
	Flags		DoNotDelay
	Scale		0.5

	Type		male
	  Anim          male\elevator_wait 1 30
	TEnd


MEnd

Move HC_Elevator_runout
	Requires	HANDCUFF
	NextMove        HandcuffReady
	Priority        1
	Sets            CANTMOVE WALKTHROUGH
	Interpolate	8

	Type		male
	  Anim          male\handcuff_basic_runout 1  #basic_runout 57
	TEnd

MEnd


###################
### Truck Jump IN  ####
###################

Move HG_Truck_Jump_IN
	Requires	RUNIN JUMPINTRUCK HANDCUFF
	Interrupts	<AttackIRQ> <HandcuffMove>  <Handcuff>  <PostMoveCycle> <RunIn>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        HG_Truck_Jump_In_post
	Sets		CANTMOVE RUNIN
	Priority        11
	Interpolate	8


	Type		male
	  Anim          male\handcuff_truck_in 1 40
	  PlayFx	Generic\Faerie_entityFadeOut.fx 0 constant
	TEnd

MEnd

########### (Continues in the truck jump in animation, but RUNIN bit isn't set, so the transfer begins)


Move HG_Truck_Jump_In_post
	Requires	HANDCUFF
	NextMove        HandcuffReady
	Priority        6
	Member		<RunIn>
	Sets            CANTMOVE
	Flags		cycle NotSelectable

	Type		male
	  Anim          male\handcuff_truck_in 41
	  PlayFx	Generic\Faerie_entityFadeOut.fx 0 constant
	TEnd
MEnd

###################
### Truck Jump OUT  ####
###################

Move HG_Truck_Jump_OUT
	Requires	EMERGE JUMPOUTOFTRUCK HANDCUFF
	Interrupts	<AttackIRQ> <HandcuffMove>  <Handcuff>  <PostMoveCycle> <RunIn>
	NextMove        HandcuffReady
	Priority        12
	Sets            CANTMOVE
	Flags		DoNotDelay

	Type		male
	  Anim          male\handcuff_truck_out 1
	  PlayFx	Generic\entityFadeIn.fx
	TEnd

MEnd







###################
### Snake Hole Jump IN  ####
###################

Move HC_SnakeHole_Jump_IN
	Requires	RUNIN JUMPINHOLE HANDCUFF
	Interrupts	<AttackIRQ> <HandcuffMove>  <Handcuff>  <PostMoveCycle> <RunIn>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        HC_SnakeHole_Jump_In_post
	Sets		CANTMOVE RUNIN
	Flags		NotSelectable
	Priority        10
	Interpolate	8

	Type		male
	  Anim          male\handcuff_hole_in 1 35
	  PlayFx	Generic\Faerie_entityFadeOut.fx 0 constant
	TEnd

MEnd

########### (Continues in the truck jump in animation, but RUNIN bit isn't set, so the transfer begins)


Move HC_SnakeHole_Jump_In_post
	Requires	HANDCUFF
	NextMove        HandcuffReady
	Priority        6
	Member		<RunIn>
	Sets            CANTMOVE
	Flags		cycle NotSelectable

	Type		male
	  Anim          male\handcuff_hole_in 35
	  PlayFx	Generic\Faerie_entityFadeOut.fx 0 constant
	TEnd
MEnd

##########################
### Snake Hole Jump OUT  ####
#########################

Move HC_SnakeHole_Jump_OUT
	Requires	EMERGE JUMPOUTOFHOLE HANDCUFF
	Interrupts	<AttackIRQ> <HandcuffMove>  <Handcuff>  <PostMoveCycle> <RunIn>
	NextMove        HandcuffReady
	Priority        11
	Sets            CANTMOVE
	Flags		NotSelectable DoNotDelay

	Type		male
	  Anim          male\handcuff_hole_out 1
##	  PlayFx	Generic\entityFadeIn.fx
	TEnd

MEnd


