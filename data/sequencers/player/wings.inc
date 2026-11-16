###################
### Wing Fly Mode  ####
###################

Move Wing_Fall_Fready
	Requires	WINGS FLY
	Interrupts	<FlyFall>
	Member		<ReactIRQ> <DeathIRQ> <FlyReady> <AttackIRQ>
	NextMove        Wing_flyready
	Priority        2
	Interpolate	5
	Scale		2

	Type		male
	  Anim          male\wing_fly_up 105 180
	TEnd
MEnd

Move Wing_postAir_C_W
	Requires	WINGS FLY
	Interrupts	Wing_C_PreFall Wing_W_PreFall
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <FlyReady> <AttackIRQ>
	NextMove        flyready
	Priority        2

	Type		male
	  Anim          male\prefloat_combat 5
	TEnd
MEnd

Move Wing_Preflyready
	Requires	WINGS FLY
	Interrupts	<Ready> <Jumps> <PostMoveCycle>
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <FlyReady> <AttackIRQ>
	NextMove        Wing_flyready
	Priority        2
	Scale		1.7

	Type		male
	  Anim          male\wing_pre_float 1 115
	TEnd

MEnd


Move Wing_flyready
	Requires	WINGS FLY
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <FlyReady> <AttackIRQ>
	Flags		Cycle
	NextMove        Wing_PreFall
	Priority        2
	Scale		1.5
##	SticksOnChild   Club ##just a test

	Type		male
	  Anim          male\wing_float_cycle 1 40
	TEnd

MEnd



###################
### Wing Fly forward  ####
###################

Move Wing_flypre
	Requires	FORWARD FLY WINGS
	Interrupts	<FlyReady> <PostMoveCycle> <PostFLYCycle> <BasicMove> <Fall> <Jumps>
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ>
##	Flags		ReqInputs
	NextMove        Wing_flyCycle
	Priority        3
	Wind		FLY
	Scale		2


	Type		male
	  Anim          male\wing_pre_post_forward 1 39
	TEnd
MEnd


Move Wing_flyCycle
	Requires	FORWARD FLY WINGS
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Flycycle> <AttackIRQ>
	Flags		Cycle
	NextMove        Wing_flyPost
	Priority        3
	Wind		FLY
	Scale		2

	Type		male
	  Anim          male\wing_fly_forward 40 79
	TEnd

MEnd


Move Wing_flyPost
	Requires	FLY WINGS
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostFLYCycle> <AttackIRQ>
	Flags		ReqInputs
	NextMove        Wing_flyready
	Priority        1
	Interpolate	5
	Scale		2

	Type		male
	  Anim          male\wing_pre_post_forward 40
	TEnd


MEnd


###################
## Wing fly Backward ##
###################

Move Wing_BackflyPre
	Requires	BACKWARD FLY WINGS
	Interrupts	<FlyReady> <PostMoveCycle> <PostFLYCycle> <BasicMove> <Fall> <iceslide>
	Member		<Flymove> <ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ>
##	Flags		ReqInputs
	NextMove        Wing_BackflyCycle
	Priority        3
	Scale           2

	Type		male
	  Anim          male\wing_fly_back 1 39
	TEnd

MEnd


Move Wing_BackflyCycle
	Requires	BACKWARD FLY WINGS
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Flycycle> <AttackIRQ>
	Flags		Cycle
	NextMove        Wing_BackflyPost
	Priority        3
	Scale           2

	Type		male
	  Anim          male\wing_fly_back 40 79
	TEnd

MEnd


Move Wing_BackflyPost
	Requires	FLY WINGS
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostFLYCycle> <AttackIRQ>
	Flags		ReqInputs
	NextMove        Wing_flyready
	Priority        2
	Scale           2

	Type		male
	  Anim          male\wing_fly_back 80 163
	TEnd

MEnd



###################
#### Wing Fly Up  ####
####################

Move Wing_flyUPpre
	Requires	JUMP FLY WINGS
	Interrupts	<FlyReady> <PostMoveCycle> <PostFLYCycle> <BasicMove> <Fall> <iceslide>
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ>
##	Flags		ReqInputs
	NextMove        Wing_flyUPCycle
	Priority        3
	Scale           2
	Wind		FLYUP

	Type		male
	  Anim          male\wing_fly_up 1 39
	TEnd

MEnd


Move Wing_flyUPCycle
	Requires	JUMP FLY WINGS
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Flycycle> <AttackIRQ>
	Flags		Cycle
	NextMove        Wing_flyUPPost
	Priority        3
	Wind		FLYUP
	Scale		2


	Type		male
	  Anim          male\wing_fly_up 40 79
	TEnd

MEnd


Move Wing_flyUPPost
	Requires	FLY WINGS
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostFLYCycle> <AttackIRQ>
	Flags		ReqInputs
	NextMove        Wing_flyready
	Priority        2
	Wind		FLY
	Scale		2

	Type		male
	  Anim          male\wing_fly_up 80 180
	TEnd

MEnd



###################
###### Wing Fly Down  ######
###################

Move Wing_PreFall
	Requires	AIR WINGS
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Fall> <AttackIRQ>
	Flags		Cycle
	NextMove        JumpPost
	Priority        2

	Type		male
	  Anim          male\wing_fly_down 1 30
	TEnd

MEnd






########################
### Wing Dive forward  ####
#######################

Move Wing_Divepre
	Requires	FORWARD FLY WINGS Dive
	Interrupts	<Flycycle>  ###<FlyReady> <PostMoveCycle> <PostFLYCycle> <BasicMove> <Fall> <Jumps>
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ>
##	Flags		ReqInputs
	NextMove        Wing_DiveCycle
	Priority        4
	Wind		FLY
	Scale		2


	Type		male
	  Anim          male\wing_fly_dive 1 58
	TEnd
MEnd


Move Wing_DiveCycle
	Requires	FORWARD FLY WINGS Dive
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Flycycle> <AttackIRQ>
	Flags		Cycle
	NextMove        Wing_flyPost  ##Wing_divePost
	Priority        4
	Wind		FLY
	Scale		2

	Type		male
	  Anim          male\wing_fly_dive 1 58
	TEnd

MEnd

####################################  wing combat flight  #############################################

###################
#### Wing Combat Fly readys  ####
####################

Move Wing_C_Fall_Fready
	Requires	WINGS FLY COMBAT
	Interrupts	Wing_C_FallCycle Wing_C_PreFall Wing_C_Ffly_PreFall Wing_C_Ffly_PreFall_2
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ>
	NextMove        Wing_C_flycycle
	Priority        3

	Type		male
	  Anim          male\flydown 180
	TEnd
MEnd

Move Wing_air_T_airCombat ##(while in the air)
	Requires	WINGS FLY COMBAT
	Interrupts	<FlyReady>
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <FlyCombat> <AttackIRQ>
	NextMove        Wing_C_flycycle
	Priority        3
	Scale		2

	Type		male
	  Anim          male\wing_combat_cycle 1 40
	TEnd
MEnd

Move Wing_C_Preflyready ##(from ground)
	Requires	WINGS FLY COMBAT
	Interrupts	<Combat> <CombatMove> <Jumps> <Claw> <CombatAttack> Preflyready Wing_Preflyready
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <FlyCombat> <AttackIRQ>
	NextMove        Wing_C_flycycle
	Priority        3

	Type		male
	  Anim          male\wing_combat_cycle 1 40
	TEnd
MEnd

#### actual combat fly ready  #############################

Move Wing_C_flycycle
	Requires	WINGS FLY COMBAT
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <FlyCombat> <AttackIRQ>
	Flags		Cycle
	NextMove        Wing_PreFall
	Priority        3
	Wind		FLY
	Scale		1.5

	Type		male
	  Anim          male\wing_combat_cycle 1 40
	TEnd
MEnd


###### Wing Melee test  (delux method)  ##############################################

###################
###### Wing Brawling ######
###################

Move Wing_Brawl_R
	Requires	WINGS FLY COMBAT PUNCH
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Wing_Brawl_Rpost
	Sets            CANTMOVE

	Type		male
	  Anim          male\wing_brawl 1 24
	TEnd
MEnd

Move Wing_Brawl_Rpost
	Requires	WINGS FLY COMBAT
	Priority        3
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <FlyCombatAttack> <BlockIRQ> <AttackIRQ>
	NextMove        Wing_C_flycycle
	Scale		1.5

	Type		male
	  Anim          male\wing_brawl 24
	TEnd
MEnd