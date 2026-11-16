#############################################################
## RUN IN/OUT
#############################################################

###################
### Truck Jump IN  ####
###################

Move Truck_Jump_IN
	Requires	RUNIN JUMPINTRUCK
	Interrupts	<AttackIRQ> <BasicMove> <CombatMove> <WepMove> <TwoHandMove> <Ready>  <PostMoveCycle> <Combat> <WepCombat> <TwoHandCombat> <RunIn>
	Member		<RunIn>
	NextMove        Truck_Jump_In_post
	Sets		CANTMOVE RUNIN
	Priority        9
	Interpolate	8

	Type		5cw
	  Anim          male\Truck_jump_in 1
	  PlayFx	Generic\Faerie_entityFadeOut.fx 0 CONSTANT
	TEnd
MEnd

Move Truck_Jump_In_post
	NextMove        Ready
	Member		<RunIn>
	Sets            CANTMOVE
	Flags		NotSelectable reqinputs

	Type		5cw
	  Anim          male\absorb 60
	  PlayFx	Generic\Faerie_entityFadeOut.fx 0 CONSTANT
	TEnd
MEnd

###################
### Truck Jump OUT  ####
###################

Move Truck_Jump_OUT
	Requires	EMERGE JUMPOUTOFTRUCK
	Priority        10
	Interrupts	<AttackIRQ> <BasicMove> <CombatMove> <WepMove> <TwoHandMove> <Ready> <PostMoveCycle> <Combat> <WepCombat> <TwoHandCombat> <RunIn>
	NextMove        Ready
	Sets            CANTMOVE
	Flags		DoNotDelay

	Type		5cw
	  Anim          5cw\Truck_out 1
	  PlayFx	Generic\entityFadeIn.fx
	TEnd
MEnd

###################
### Snake Hole Jump IN  ####
###################

Move SnakeHole_Jump_IN
	Requires	RUNIN JUMPINHOLE
	Interrupts	<AttackIRQ> <BasicMove> <CombatMove> <WepMove> <TwoHandMove> <Ready> <PostMoveCycle> <Combat> <WepCombat> <TwoHandCombat> <RunIn>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Truck_Jump_In_post
	Sets		CANTMOVE RUNIN
	Flags		NotSelectable
	Priority        9
	Interpolate	8

	Type		5cw
	  Anim          male\Hole_jump_IN 1
	  PlayFx	Generic\Faerie_entityFadeOut.fx 0 CONSTANT
	TEnd
MEnd

########### (Continues in the truck jump in animation, but RUNIN bit isn't set, so the transfer begins)

Move SnakeHole_Jump_In_post
	NextMove        Ready
	Priority        5
	Member		<RunIn>
	Sets            CANTMOVE
	Flags		NotSelectable

	Type		5cw
	  Anim          male\absorb 60
	  PlayFx	Generic\Faerie_entityFadeOut.fx 0 CONSTANT
	TEnd
MEnd

###################
### Snake Hole Jump OUT  ####
###################

Move SnakeHole_Jump_OUT
	Requires	EMERGE JUMPOUTOFHOLE
	Priority        10
	Interrupts	<AttackIRQ> <BasicMove> <CombatMove> <WepMove> <TwoHandMove> <Ready> <PostMoveCycle> <Combat> <WepCombat> <TwoHandCombat> <RunIn>
	NextMove        Truck_post_cycle
	Sets            CANTMOVE
	Flags		NotSelectable DoNotDelay

	Type		5cw
	  Anim          male\hole_jump_out 1
	TEnd
MEnd

Move SnakeHole_post_cycle
	Include sequencers\Members\Readies.inc
	NextMove	Ready

	Type		5cw
	  Anim          male\lookquick 1
	TEnd
MEnd