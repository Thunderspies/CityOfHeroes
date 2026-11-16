## RUN FORWARD ###########################################################

Move Shield_RunPre
	Requires	FORWARD	SHIELD #COMBAT WEAPON
	Priority	12
	Interrupts	<MoveIRQ> <BasicMove> <CombatMove> <WepMove> <DualHandMove> #<ShieldCombat>
	Member		<DeathIRQ> <AttackIRQ> <ShieldMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL>
	Flags		ReqInputs
	NextMove	Shield_RunCycle

	Type		male
	  Anim		male\shield_Run 12 25
	  PlayFx	FEET\MALERIGHT.FX 10
	TEnd

	Type		fem
	  Anim		fem\dual_run 12 25
	  PlayFx	FEET\FEMRIGHT.FX 10
	TEnd

	Type		huge
	  Anim		male\shield_Run 12 25
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT 3
	TEnd
MEnd

Move Shield_RunCycle
	Requires	FORWARD	SHIELD #COMBAT WEAPON
	Member		<DeathIRQ> <AttackIRQ> <ShieldMove> #<IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	0.7
	NextMove	Shield_RunPost

	Type		male
	  Anim		male\shield_Run 1 25
	  PlayFx	FEET\MALELEFT.FX 10
	  PlayFx	FEET\MALERIGHT.FX 21
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		fem\dual_run 1 25
	  PlayFx	FEET\FEMLEFT.FX 10
	  PlayFx	FEET\FEMRIGHT.FX 21
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		male\shield_Run 1 25
	  PlayFx	FEET\HUGELEFT.FX 10
	  PlayFx	FEET\HUGERIGHT.FX 21
	  SmoothSprint	3.3
	TEnd
MEnd

#############################################################

Move Shield_RunPost
	Requires	SHIELD #COMBAT WEAPON
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_CombatReady_Blend

	Type		male
	  Anim		male\shield_RunPost 1
	  PlayFx	FEET\LAND3.FX  3
	  Scale		0.7
	TEnd

	Type		fem
	  Anim		male\shield_RunPost 1
	  PlayFx	FEET\LAND4.FX  3
	  Scale		0.7
	TEnd

	Type		huge
	  Anim		male\shield_RunPost 1
	  PlayFx	FEET\HUGELAND1.FX  3
	  Scale		0.7
	TEnd
MEnd


## RUN BACK ###########################################################

Move Shield_RunbackPre
	Requires	BACKWARD SHIELD #COMBAT WEAPON
	Priority	14
	Interrupts	<BasicMove> <CombatMove> <WepMove> <DualHandMove> <MoveIRQ> #<ShieldCombat>
	Member		<DeathIRQ> <AttackIRQ> <ShieldMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL>
	Flags		ReqInputs
	NextMove	Shield_RunbackCycle

	Type		male
	  Anim		male\shield_RunBack 1 24
	  PlayFx	FEET\MALERIGHT.FX  10
	TEnd

	Type		fem
	  Anim		male\shield_RunBack 1 24
	  PlayFx	FEET\FEMRIGHT.FX  10
	TEnd

	Type		huge
	  Anim		male\shield_RunBack 1 24
	  PlayFx	FEET\HUGERIGHT.FX  10
	TEnd
MEnd

Move Shield_RunbackCycle
	Requires	BACKWARD SHIELD #COMBAT WEAPON
	Member		<DeathIRQ> <AttackIRQ> <ShieldMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	0.7
	NextMove	Shield_RunPost

	Type		male
	  Anim		male\shield_RunBack 1 24
	  PlayFx	FEET\MALELEFT.FX  10
	  PlayFx	FEET\MALERIGHT.FX  21
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		male\shield_RunBack 1 24
	  PlayFx	FEET\FEMLEFT.FX	 10
	  PlayFx	FEET\FEMRIGHT.FX  21
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		male\shield_RunBack 1 24
	  PlayFx	FEET\HUGELEFT.FX  10
	  PlayFx	FEET\HUGERIGHT.FX  21
	  SmoothSprint	3.3
	TEnd
MEnd


## RUN LEFT ###########################################################

Move Shield_RunLeftPre
	Requires	STEPLEFT SHIELD #COMBAT WEAPON
	Priority	14
	Interrupts	<BasicMove> <CombatMove> <WepMove> <DualHandMove> <MoveIRQ> Shield_RunPre #<ShieldCombat>
	Member		<DeathIRQ> <AttackIRQ> <ShieldMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL>
	Flags		ReqInputs MoveScale SmoothSprint
	NextMove	Shield_RunLeftCycle
	MoveRate	0.7

	Type		male
	  Anim		male\shield_RunLeft 1 25
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		male\shield_RunLeft 1 25
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		male\shield_RunLeft 1 25
	  SmoothSprint	3.3
	TEnd
MEnd

Move Shield_RunLeftCycle
	Requires	STEPLEFT SHIELD #COMBAT WEAPON
	Priority	14
	Interrupts	<BasicMove> <CombatMove> <WepMove> <DualHandMove> <MoveIRQ> Shield_RunCycle
	Member		<DeathIRQ> <AttackIRQ> <ShieldMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	0.7
	NextMove	Shield_RunPost

	Type		male
	  Anim		male\shield_RunLeft 1 25
	  PlayFx	FEET\MALELEFT.FX 10
	  PlayFx	FEET\MALERIGHT.FX 1
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		male\shield_RunLeft 1 25
	  PlayFx	FEET\FEMLEFT.FX 10
	  PlayFx	FEET\FEMRIGHT.FX 21
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		male\shield_RunLeft 1 25
	  PlayFx	FEET\HUGELEFT.FX 10
	  PlayFx	FEET\HUGERIGHT.FX 21
	  SmoothSprint	3.3
	TEnd
MEnd

## RUN RIGHT ###########################################################

Move Shield_RunRightPre
	Requires	STEPRIGHT SHIELD #COMBAT WEAPON
	Priority	14
	Interrupts	<BasicMove> <CombatMove> <WepMove> <DualHandMove> <MoveIRQ> Shield_RunPre #<ShieldCombat>
	Member		<DeathIRQ> <AttackIRQ> <ShieldMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL>
	Flags		ReqInputs MoveScale SmoothSprint
	NextMove	Shield_RunRightCycle
	MoveRate	0.7

	Type		male
	  Anim		male\shield_RunRight 1 25
	  SmoothSprint	3.3
	TEnd
MEnd

Move Shield_RunRightCycle
	Requires	STEPRIGHT SHIELD #COMBAT WEAPON
	Priority	14
	Interrupts	<BasicMove> <CombatMove> <WepMove> <DualHandMove> <MoveIRQ> Shield_RunCycle
	Member		<DeathIRQ> <AttackIRQ> <ShieldMove> <IRQJumps> <IRQSwim> <IRQSlide> <GroundMoveALL>
	Flags		Cycle MoveScale	SmoothSprint
	MoveRate	0.7
	NextMove	Shield_RunPost

	Type		male
	  Anim		male\shield_RunRight 1 25
	  PlayFx	FEET\MALELEFT.FX  10
	  PlayFx	FEET\MALERIGHT.FX  21
	  SmoothSprint	3.3
	TEnd

	Type		fem
	  Anim		male\shield_RunRight 1 25
	  PlayFx	FEET\FEMLEFT.FX	 10
	  PlayFx	FEET\FEMRIGHT.FX  21
	  SmoothSprint	3.3
	TEnd

	Type		huge
	  Anim		male\shield_RunRight 1 25
	  PlayFx	FEET\HUGELEFT.FX  10
	  PlayFx	FEET\HUGERIGHT.FX  21
	  SmoothSprint	3.3
	TEnd
MEnd

#############################################################
## JUMPS
#############################################################

## JUMP UP ###########################################################

Move Shield_Jump_Pre
	Requires	JUMP AIR SHIELD #COMBAT WEAPON
	Priority	15
	Interrupts	<IRQJumps> <ShieldCombat> <ShieldFall> <BlockIRQ> <HitIRQ> <MoveIRQ>
	Member		<DeathIRQ> <ShieldJump> <AttackIRQ>
	NextMove	Shield_Jump_Cycle

	Type		male
	  Anim		male\shield_Jump 3 6
	  PlayFx	FEET\SUPERJUMP1.FX:JUMP
	TEnd

	Type		huge
	  Anim		male\shield_Jump 3 6
	  PlayFx	FEET\HUGEJUMP2.FX:JUMP
	TEnd

	Type		fem
	  Anim		male\shield_Jump 3 6
	  PlayFx	FEET\FEMJUMP1.FX:JUMP
	TEnd
MEnd

Move Shield_Jump_Cycle
	Requires	JUMP AIR SHIELD #COMBAT WEAPON
	Member		<DeathIRQ> <ShieldJump> <AttackIRQ>
	Flags		Cycle
	NextMove	Shield_Jump_Apex

	Type		male
	  Anim		male\shield_Jump 7 64
	TEnd
MEnd

Move Shield_Jump_Apex
	Requires	APEX JUMP AIR SHIELD #COMBAT WEAPON
	Member		<DeathIRQ> <ShieldJump> <ShieldFall> <AttackIRQ>
	NextMove	Shield_Jump_Fall

	Type		male
	  Anim		male\shield_Jump 65 66
	TEnd
MEnd

Move Shield_Jump_Fall
	Requires	AIR SHIELD #COMBAT WEAPON
	Member		<DeathIRQ> <ShieldJump> <ShieldFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Shield_Jump_Fall_Cycle

	Type		male
	  Anim		male\shield_Jump 67 89
	TEnd
MEnd

Move Shield_Jump_Fall_Cycle
	Requires	AIR SHIELD #COMBAT WEAPON
	Member		<DeathIRQ> <ShieldJump> <ShieldFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Shield_Jump_Post

	Type		male
	  Anim		male\shield_Jump 89
	TEnd
MEnd

Move Shield_Jump_Post
	Requires	SHIELD #COMBAT WEAPON
	Include sequencers\Members\Combat_Shield.inc
	Flags		ReqInputs
	NextMove	Shield_CombatReady
	Interpolate	1
	SticksONChild	LAND

	Type		male
	  Anim		male\shield_JumpPost 1 70
	  PlayFx	FEET\LAND1.FX:LAND
	  Scale		1.2
	TEnd

	Type		huge
	  Anim		male\shield_JumpPost 1 70
	  PlayFx	FEET\HUGELAND2.FX:LAND
	  Scale		1.2
	TEnd

	Type		fem
	  Anim		male\shield_JumpPost 1 70
	  PlayFx	FEET\LAND1.FX:LAND
	  Scale		1.2
	TEnd
MEnd

## JUMP TO FORWARD ###########################################################

Move Shield_JumpToHop
	Requires	FORWARD	JUMP AIR SHIELD #COMBAT WEAPON
	Priority	16
	Interrupts	Shield_Jump_Pre Shield_Jump_Cycle
	Member		<DeathIRQ> <ShieldJump> <AttackIRQ>
	NextMove	Shield_Hop
	Interpolate	15

	Type		male
	  Anim		male\shield_Hop 10 11
	TEnd
MEnd

## JUMP FORWARD ###########################################################

Move Shield_Hop_Pre
	Requires	FORWARD	JUMP AIR SHIELD #COMBAT WEAPON
	Priority	16
	Interrupts	<IRQJumps> <ShieldCombat> <ShieldMove> <ShieldFall> <PostMoveCycle> <MoveIRQ>
	Member		<DeathIRQ> <ShieldJump> <AttackIRQ>
	NextMove	Shield_Hop_Cycle
	Flags		ReqInputs

	Type		male
	  Anim		male\shield_Hop 10 91
	  PlayFx	FEET\JUMP1.FX:JUMP
	TEnd

	Type		huge
	  Anim		male\shield_Hop 10 91
	  PlayFx	FEET\HUGEJUMP2.FX:JUMP
	TEnd

	Type		fem
	  Anim		male\shield_Hop 10 91
	  PlayFx	FEET\FEMJUMP1.FX:JUMP
	TEnd
MEnd

Move Shield_Hop_Cycle
	Requires	JUMP AIR SHIELD #COMBAT WEAPON
	Member		<DeathIRQ> <ShieldJump> <AttackIRQ>
	Flags		Cycle
	NextMove	Shield_Hop_Apex

	Type		male
	  Anim		male\shield_Hop 91 91
	TEnd
MEnd

Move Shield_Hop_Apex
	Requires	APEX AIR SHIELD #COMBAT WEAPON
	Member		<DeathIRQ> <ShieldJump> <ShieldFall> <AttackIRQ>
	NextMove	Shield_Hop_Fall

	Type		male
	  Anim		male\shield_Hop 92 93
	TEnd
MEnd

Move Shield_Hop_Fall
	Requires	AIR SHIELD #COMBAT WEAPON
	Member		<DeathIRQ> <ShieldJump> <ShieldFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Shield_Hop_Fall_Cycle

	Type		male
	  Anim		male\shield_Hop 94 128
	TEnd
MEnd

Move Shield_Hop_Fall_Cycle
	Requires	AIR SHIELD #COMBAT WEAPON
	Member		<DeathIRQ> <ShieldJump> <ShieldFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Shield_Jump_Post

	Type		male
	  Anim		male\shield_Hop 128
	TEnd
MEnd

## WEAPON JUMP ###########################################################

Move Shield_Weapon_Jump_Pre
	Requires	JUMP AIR SHIELD WEAPON #COMBAT WEAPON
	Priority	16
	Interrupts	<IRQJumps> <ShieldCombat> <ShieldMove> <ShieldFall> <PostMoveCycle> <BlockIRQ> <HitIRQ> <MoveIRQ>
	Member		<DeathIRQ> <ShieldJump> <AttackIRQ>
	NextMove	Shield_Weapon_Jump_Cycle

	Type		male
	  Anim		male\shield_Weapon_Jump 3 6
	  PlayFx	FEET\SUPERJUMP1.FX
	TEnd

	Type		fem
	  Anim		male\shield_Weapon_Jump 3 6
	  PlayFx	FEET\FEMJUMP1.FX
	TEnd

	Type		huge
	  Anim		male\shield_Weapon_Jump 3 6
	  PlayFx	FEET\HUGEJUMP2.FX
	TEnd
MEnd

Move Shield_Weapon_Jump_Cycle
	Requires	JUMP AIR SHIELD #COMBAT WEAPON
	Member		<DeathIRQ> <ShieldJump> <AttackIRQ>
	NextMove	Shield_Weapon_JumpApex
	Flags		Cycle

	Type		male
	  Anim		male\shield_Weapon_Jump 7 64
	TEnd
MEnd

Move Shield_Weapon_JumpApex
	Requires	APEX AIR SHIELD #COMBAT WEAPON
	Member		<DeathIRQ> <ShieldCombat> <ShieldJump> <ShieldFall> <AttackIRQ>
	NextMove	Shield_Weapon_Jump_Fall

	Type		male
	  Anim		male\shield_Weapon_Jump 65 66
	TEnd
MEnd

Move Shield_Weapon_Jump_Fall
	Requires	AIR SHIELD #COMBAT WEAPON
	Member		<DeathIRQ> <ShieldCombat> <ShieldJump> <ShieldFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Shield_Weapon_Jump_Fall_Cycle

	Type		male
	  Anim		male\shield_Weapon_Jump 67 88
	TEnd
MEnd

Move Shield_Weapon_Jump_Fall_Cycle
	Requires	AIR SHIELD #COMBAT WEAPON
	Member		<DeathIRQ> <ShieldCombat> <ShieldJump> <ShieldFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Shield_Jump_Post

	Type		male
	  Anim		male\shield_Weapon_Jump 89
	TEnd
MEnd

## WEAPON JUMP TO FORWARD ###########################################################

Move Shield_Weapon_JumpToHop
	Requires	FORWARD	JUMP AIR SHIELD WEAPON #COMBAT WEAPON
	Priority	17
	Interrupts	Shield_Weapon_Jump_Pre Shield_Weapon_Jump_Cycle
	Member		<DeathIRQ> <ShieldJump> <AttackIRQ>
	NextMove	Shield_Weapon_Hop
	Interpolate	15

	Type		male
	  Anim		male\shield_Weapon_Hop 10 11
	TEnd
MEnd

## WEAPON HOP ###########################################################

Move Shield_Weapon_Hop_Pre
	Requires	FORWARD	JUMP AIR SHIELD WEAPON #COMBAT WEAPON
	Priority	17
	Interrupts	<IRQJumps> <ShieldCombat> <ShieldMove> <ShieldFall> <PostMoveCycle> <MoveIRQ>
	Member		<DeathIRQ> <ShieldJump> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Shield_Weapon_Hop

	Type		male
	  Anim		male\shield_Weapon_Hop 10 11
	  PlayFx	FEET\JUMP1.FX
	TEnd

	Type		fem
	  Anim		male\shield_Weapon_Hop 10 11
	  PlayFx	FEET\FEMJUMP1.FX
	TEnd

	Type		huge
	  Anim		male\shield_Weapon_Hop 10 11
	  PlayFx	FEET\HUGEJUMP2.FX
	TEnd
MEnd

Move Shield_Weapon_Hop
	Requires	JUMP AIR SHIELD #COMBAT WEAPON
	Member		<DeathIRQ> <ShieldJump> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Shield_Weapon_Hop_Cycle

	Type		male
	  Anim		male\shield_Weapon_Hop 12 91
	TEnd
MEnd

Move Shield_Weapon_Hop_Cycle
	Requires	JUMP AIR SHIELD #COMBAT WEAPON
	Member		<DeathIRQ> <ShieldJump> <AttackIRQ>
	Flags		Cycle #ReqInputs
	NextMove	Shield_Weapon_Hop_Apex

	Type		male
	  Anim		male\shield_Weapon_Hop 91 92
	TEnd
MEnd

Move Shield_Weapon_Hop_Apex
	Requires	APEX AIR SHIELD #COMBAT WEAPON
	Member		<DeathIRQ> <ShieldCombat> <ShieldJump> <ShieldFall> <AttackIRQ>
	NextMove	Shield_Weapon_Hop_Fall

	Type		male
	  Anim		male\shield_Weapon_Hop 93 94
	TEnd
MEnd

Move Shield_Weapon_Hop_Fall
	Requires	AIR SHIELD #COMBAT WEAPON
	Member		<DeathIRQ> <ShieldCombat> <ShieldJump> <ShieldFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Shield_Weapon_Hop_Fall_Cycle

	Type		male
	  Anim		male\shield_Weapon_Hop 94 128
	TEnd
MEnd

Move Shield_Weapon_Hop_Fall_Cycle
	Requires	AIR SHIELD #COMBAT WEAPON
	Member		<DeathIRQ> <ShieldCombat> <ShieldJump> <ShieldFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Shield_Jump_Post

	Type		male
	  Anim		male\shield_Weapon_Hop 128
	TEnd
MEnd

## SHIELD WINDMILL JUMP (BOUNCE) ###########################################################

Move Shield_Bounce_Pre
	Requires	BOUNCING AIR SHIELD #COMBAT WEAPON
	Priority	15
	Interrupts	<IRQJumps> <ShieldCombat> <ShieldMove> <ShieldFall> <PostMoveCycle> <BlockIRQ> <HitIRQ> <MoveIRQ>
	Member		<DeathIRQ> <ShieldJump> <AttackIRQ>
	NextMove	Shield_Bounce_Up

	Type		male
	  Anim		male\windmill_jump 20 26
	  PlayFx	FEET\SUPERJUMP1.FX:JUMP
	TEnd

	Type		huge
	  Anim		male\windmill_jump 20 26
	  PlayFx	FEET\HUGEJUMP2.FX:JUMP
	TEnd

	Type		fem
	  Anim		male\windmill_jump 20 26
	  PlayFx	FEET\FEMJUMP1.FX:JUMP
	TEnd
MEnd

Move Shield_Bounce_Up
	Requires	AIR SHIELD #COMBAT WEAPON
	Member		<DeathIRQ> <ShieldJump> <AttackIRQ>
	Flags		Cycle
	NextMove	Shield_Bounce_Apex

	Type		male
	  Anim		male\windmill_jump 1 26
	TEnd
MEnd

Move Shield_Bounce_Apex
	Requires	APEX AIR SHIELD #COMBAT WEAPON
	Priority	15
	Interrupts	Shield_Bounce_Pre Shield_Bounce_Up
	Member		<DeathIRQ> <ShieldCombat> <ShieldJump> <ShieldFall> <AttackIRQ>
	NextMove	Shield_Bounce_Fall
	Scale		2

	Type		male
	  Anim		male\windmill_bigfall 1	15  ##windmill_fall 19 31
	TEnd
MEnd

Move Shield_Bounce_Fall
	Requires	AIR SHIELD #COMBAT WEAPON
	Member		<DeathIRQ> <ShieldCombat> <ShieldJump> <ShieldFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Shield_Jump_Post
	Scale		2

	Type		male
	  Anim		male\windmill_bigfall 1
	TEnd
MEnd

#############################################################
## FLIGHT
#############################################################

## HOVER - FORWARD ###########################################################

Move Shield_Hover_Forward_Pre
	Requires	FORWARD	FLY SHIELD HOVER #COMBAT WEAPON
	Priority	26
	Interrupts	<ShieldFLYReady> <ShieldMove> <FlyMoveALL> <PostMoveCycle> <MoveIRQ> Shield_BackflyCycle
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <ShieldFLYMove> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Shield_Hover_Forward_Cycle
	Wind		FLY
	Scale		0.875

	Type		male
	  Anim		male\shield_hover_forward 1 10
	TEnd
MEnd

Move Shield_Hover_Forward_Cycle
	Requires	FORWARD	FLY SHIELD HOVER #COMBAT WEAPON
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <ShieldFLYMove> <AttackIRQ>
	Flags		Cycle ReqInputs
	NextMove	Shield_Hover_Forward_Post
	Wind		FLY
	Scale		0.625

	Type		male
	  Anim		male\shield_hover_forward 10 70
	TEnd
MEnd

Move Shield_Hover_Forward_Post
	Requires	FLY SHIELD HOVER #COMBAT WEAPON
	Include sequencers\Members\Combat_Shield_Air.inc
	Flags		ReqInputs
	NextMove	Shield_FlyReady
	Scale		0.875

	Type		male
	  Anim		male\shield_hover_forward 71
	TEnd
MEnd

#############################################################

Move Shield_Flight2Hover_Trans
	Requires	FLY SHIELD HOVER #COMBAT WEAPON
	Priority	25
	Interrupts	Shield_Flying_Pre Shield_FlyingCycle
	Include sequencers\Members\Combat_Shield_Air.inc
	Flags		ReqInputs
	NextMove	Shield_Flight2Hover_Blend
	Wind		FLY
	Scale		1.5

	Type		male
	  Anim		male\shield_Fly 83 88
	TEnd
MEnd

Move Shield_Flight2Hover_Blend
	Requires	FLY SHIELD HOVER #COMBAT WEAPON
	Include sequencers\Members\Combat_Shield_Air.inc
	Flags		ReqInputs
	NextMove	Shield_Hover_Forward_Cycle
	Wind		FLY
	Interpolate	15
	Scale		0.75

	Type		male
	  Anim		male\shield_hover_forward 10 70
	TEnd
MEnd


## FLY FORWARD ###########################################################

Move Shield_Flying_Pre
	Requires	FORWARD	FLY SHIELD #COMBAT WEAPON
	Priority	25
	Interrupts	<ShieldFLYReady> <ShieldMove> <FlyMoveALL> <PostMoveCycle> <MoveIRQ> Shield_BackflyCycle
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <ShieldFLYMove> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Shield_FlyingCycle

	Type		male
	  Anim		male\shield_Fly 4 24
	  PlayFx	FEET\FlightForward.fx
	TEnd
MEnd

Move Shield_FlyingCycle
	Requires	FORWARD	FLY SHIELD #COMBAT WEAPON
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <ShieldFLYMove> <AttackIRQ>
	Flags		Cycle
	NextMove	Shield_Flying_Post
	Wind		FLY

	Type		male
	  Anim		male\shield_Fly 25 82
	TEnd
MEnd

Move Shield_Flying_Post
	Requires	FLY SHIELD #COMBAT WEAPON
	Include sequencers\Members\Combat_Shield_Air.inc
	Flags		ReqInputs
	NextMove	Shield_flyready
	Scale		1.30
	Wind		FLY

	Type		male
	  Anim		male\shield_Fly 83
	  PlayFx	FEET\FlightHover.fx
	TEnd
MEnd

## FLY BACK ###########################################################

Move Shield_BackflyPre
	Requires	BACKWARD FLY SHIELD #COMBAT WEAPON
	Priority	25
	Interrupts	<ShieldFLYReady> <ShieldMove> <FlyMoveALL> <PostMoveCycle> <MoveIRQ> Shield_FlyingCycle
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <ShieldFLYMove> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Shield_BackflyCycle

	Type		male
	  Anim		male\shield_FlyBack 3 13
	  PlayFx	FEET\FlightBackward.fx
	TEnd
MEnd

Move Shield_BackflyCycle
	Requires	BACKWARD FLY SHIELD #COMBAT WEAPON
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <ShieldFLYMove> <AttackIRQ>
	Flags		Cycle
	NextMove	Shield_BackflyPost

	Type		male
	  Anim		male\shield_FlyBack 14 63
	TEnd
MEnd

Move Shield_BackflyPost
	Requires	FLY SHIELD #COMBAT WEAPON
	Include sequencers\Members\Combat_Shield_Air.inc
	Flags		ReqInputs
	NextMove	Shield_flyready

	Type		male
	  Anim		male\shield_FlyBack 64
	  PlayFx	FEET\FlightHover.fx
	TEnd
MEnd

## FLY UP ###########################################################

Move Shield_flyUPpre
	Requires	JUMP FLY SHIELD #COMBAT WEAPON
	Priority	25
	Interrupts	<ShieldFLYReady> <ShieldMove> <FlyMoveALL> <PostMoveCycle> <MoveIRQ>
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <ShieldFLYMove> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Shield_flyUPCycle
	Scale		0.90
	Wind		FLYUP

	Type		male
	  Anim		male\shield_FlyUp 5 34
	  PlayFx	FEET\FlightForward.fx
	TEnd
MEnd

Move Shield_flyUPCycle
	Requires	JUMP FLY SHIELD #COMBAT WEAPON
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <ShieldFLYMove> <AttackIRQ>
	Flags		Cycle
	NextMove	Shield_flyUPPost
	Wind		FLYUP

	Type		male
	  Anim		male\shield_FlyUp 36 93
	TEnd
MEnd

Move Shield_flyUPPost
	Requires	FLY SHIELD #COMBAT WEAPON
	Include sequencers\Members\Combat_Shield_Air.inc
	Flags		ReqInputs
	NextMove	Shield_flyready
	Wind		FLY

	Type		male
	  Anim		male\shield_FlyUp 94
	  PlayFx	FEET\FlightHover.fx
	TEnd
MEnd

## FLY RIGHT ###########################################################

Move Shield_RightflyPre
	Requires	STEPRIGHT FLY SHIELD #COMBAT WEAPON
	Priority	25
	Interrupts	<ShieldFLYReady> <ShieldMove> <FlyMoveALL> <PostMoveCycle> <MoveIRQ> Shield_LeftflyCycle
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <ShieldFLYMove> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Shield_RightflyCycle

	Type		male
	  Anim		male\shield_FlyRight 3 9
	  PlayFx	FEET\FlightLateral.FX
	TEnd
MEnd

Move Shield_RightflyCycle
	Requires	STEPRIGHT FLY SHIELD #COMBAT WEAPON
	Interrupts	Shield_Hover_Forward_Pre Shield_Hover_Forward_Cycle Shield_Flying_Pre Shield_FlyingCycle
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <ShieldFLYMove> <AttackIRQ>
	Flags		Cycle
	NextMove	Shield_RightflyPost

	Type		male
	  Anim		male\shield_FlyRight 10 59
	TEnd
MEnd

Move Shield_RightflyPost
	Requires	FLY SHIELD #COMBAT WEAPON
	Include sequencers\Members\Combat_Shield_Air.inc
	Flags		ReqInputs
	NextMove	Shield_flyready

	Type		male
	  Anim		male\shield_FlyRight 60
	  PlayFx	FEET\FlightHover.fx
	TEnd
MEnd

## FLY LEFT ###########################################################

Move Shield_LeftflyPre
	Requires	STEPLEFT FLY SHIELD #COMBAT WEAPON
	Priority	25
	Interrupts	<ShieldFLYReady> <ShieldMove> <FlyMoveALL> <PostMoveCycle> <MoveIRQ> Shield_RightflyCycle
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <ShieldFLYMove> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Shield_LeftflyCycle

	Type		male
	  Anim		male\shield_FlyLeft 3 9
	  PlayFx	FEET\FlightLateral.FX
	TEnd
MEnd

Move Shield_LeftflyCycle
	Requires	STEPLEFT FLY SHIELD #COMBAT WEAPON
	Interrupts	Shield_Hover_Forward_Pre Shield_Hover_Forward_Cycle Shield_Flying_Pre Shield_FlyingCycle
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <ShieldFLYMove> <AttackIRQ>
	Flags		Cycle
	NextMove	Shield_LeftflyPost

	Type		male
	  Anim		male\shield_FlyLeft 10 59
	TEnd
MEnd

Move Shield_LeftflyPost
	Requires	FLY SHIELD #COMBAT WEAPON
	Include sequencers\Members\Combat_Shield_Air.inc
	Flags		ReqInputs
	NextMove	Shield_flyready

	Type		male
	  Anim		male\shield_FlyLeft 60
	  PlayFx	FEET\FlightHover.fx
	TEnd
MEnd

#############################################################
## FALLS
#############################################################

## RUN FALLS ###########################################################

Move Shield_Fall
	Requires	BIGFALL	SHIELD #COMBAT WEAPON
	Priority	14
	Interrupts	<ShieldMove>
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <ShieldFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Shield_Fall_Cycle

	Type		male
	  Anim		male\shield_Jump 65 88
	TEnd
MEnd

Move Shield_Fall_Cycle
	Requires	AIR SHIELD #COMBAT WEAPON
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <ShieldFall> <AttackIRQ>
	Flags		Cycle ReqInputs
	NextMove	Shield_Jump_Post

	Type		male
	  Anim		male\shield_Jump 89
	TEnd
MEnd

#############################################################

Move Shield_FlyFall
	Requires	AIR SHIELD #COMBAT WEAPON
	##Interrupts	<ShieldMove>
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <ShieldFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Shield_Fall_Cycle

	Type		male
	  Anim		male\shield_Jump 65 88
	TEnd
MEnd

#############################################################

Move Shield_FallForward
	Requires	BIGFALL	SHIELD FORWARD #COMBAT WEAPON
	Priority	15
	Interrupts	<ShieldMove>
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <ShieldFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Shield_FallForward_Cycle

	Type		male
	  Anim		male\shield_Hop 93 127
	TEnd
MEnd

#############################################################

Move Shield_FallFoward_Pre
	Requires	AIR SHIELD FORWARD #COMBAT WEAPON
	Priority	15
	Interrupts	<MoveIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <Fall>
	NextMove	Shield_Forward_FlyFall_Cycle

	Type		male
	  Anim		male\flydown 1 3
	TEnd

	Type		huge
	  Anim		huge\flydown 1 3
	TEnd

	Type		fem
	  Anim		fem\fly_down 1 3
	TEnd
MEnd

#############################################################

Move Shield_FallForward_Cycle
	Requires	AIR SHIELD #FORWARD COMBAT WEAPON
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <ShieldFall> <AttackIRQ>
	Flags		Cycle ReqInputs
	NextMove	Shield_Jump_Post

	Type		male
	  Anim		male\shield_Hop 128
	TEnd
MEnd

Move Shield_Forward_FlyFall
	Requires	AIR SHIELD #FORWARD
	Flags		ReqInputs
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <ShieldFall> <AttackIRQ>
	NextMove	Shield_Forward_FlyFall_Cycle

	Type		male
	  Anim		male\shield_Hop 93 127
	TEnd
MEnd

Move Shield_Forward_FlyFall_Cycle
	Requires	AIR SHIELD
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <ShieldFall> <AttackIRQ>
	Flags		Cycle ReqInputs
	NextMove	Shield_Jump_Post

	Type		male
	  Anim		male\shield_Hop 128
	TEnd
MEnd