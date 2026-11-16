#######################################################################################################
## Alternate Flight Poses
#######################################################################################################

## FLIGHT POSE EMOTES #####################################################################################################
## The Interrupts and Member tags on these are setup to interrupt the fly forward animation

##############################
## Fly Forward - Alt Pose 1 ##
##############################

Move A_FlyPose1_Pre
	Requires	FLY FORWARD ALTERNATE ONE
	Interrupts	<IRQEmotesALL> <FlyForwardAlt>
	Member		<IRQEmotesALL> <ReactIRQ> <HitIRQ> <DeathIRQ> <AttackALL> <FlyForward>
	NextMove	A_FlyPose1
	Priority	4
	Wind		FLY

	Type		male
	  Anim          male\flyforward_alt1 1 26
	  PlayFx        FEET\FLIGHT3.FX
	TEnd
MEnd

Move A_FlyPose1
	Requires	FLY FORWARD
	Member		<IRQEmotesALL> <ReactIRQ> <HitIRQ> <DeathIRQ> <Flycycle> <AttackALL> <FlyForward>
	Flags		Cycle ReqInputs
	NextMove	A_FlyPose1_Post
	Priority	2
	Wind		FLY

	Type		male
	  Anim          male\flyforward_alt1 26 83
	TEnd
MEnd

Move A_FlyPose1_Post
	Requires	FLY
	Member		<IRQEmotesALL> <ReactIRQ> <HitIRQ> <DeathIRQ> <PostFLYCycle> <AttackALL> <FlyForward>
	Flags		ReqInputs
	NextMove	flyready
	Priority	1
	Scale           1.30

	Type		male
	  Anim          male\flyforward_alt1 84
	TEnd
MEnd


###############################
### Fly Forward - Alt Pose 2 ##
###############################

Move A_FlyPose2_Pre
	Requires	FLY FORWARD ALTERNATE TWO
	Interrupts	<IRQEmotesALL> <FlyForwardAlt>
	Member		<IRQEmotesALL> <ReactIRQ> <HitIRQ> <DeathIRQ> <AttackALL> <FlyForward>
	NextMove	A_FlyPose2
	Priority	4
	Wind		FLY

	Type		male
	  Anim          male\flyforward_alt2 1 26
	  PlayFx        FEET\FLIGHT3.FX
	TEnd
MEnd

Move A_FlyPose2
	Requires	FLY FORWARD
	Member		<IRQEmotesALL> <ReactIRQ> <HitIRQ> <DeathIRQ> <Flycycle> <AttackALL> <FlyForward>
	Flags		Cycle ReqInputs
	NextMove	A_FlyPose2_Post
	Priority	2
	Wind		FLY

	Type		male
	  Anim          male\flyforward_alt2 26 83
	TEnd
MEnd

Move A_FlyPose2_Post
	Requires	FLY
	Member		<IRQEmotesALL> <ReactIRQ> <HitIRQ> <DeathIRQ> <PostFLYCycle> <AttackALL> <FlyForward>
	Flags		ReqInputs
	NextMove	flyready
	Priority	1
	Scale           1.30

	Type		male
	  Anim          male\flyforward_alt2 84
	TEnd
MEnd


##############################
## Fly Forward - Alt Pose 3 ##
##############################

Move A_FlyPose3_Pre
	Requires	FLY FORWARD ALTERNATE THREE
	Interrupts	<IRQEmotesALL> <FlyForwardAlt>
	Member		<IRQEmotesALL> <ReactIRQ> <HitIRQ> <DeathIRQ> <AttackALL> <FlyForward>
	NextMove	A_FlyPose3
	Priority	4
	Wind		FLY

	Type		male
	  Anim          male\flyforward_alt3 1 26
	  PlayFx        FEET\FLIGHT3.FX
	TEnd
MEnd

Move A_FlyPose3
	Requires	FLY FORWARD
	Member		<IRQEmotesALL> <ReactIRQ> <HitIRQ> <DeathIRQ> <Flycycle> <AttackALL> <FlyForward>
	Flags		Cycle ReqInputs
	NextMove	A_FlyPose3_Post
	Priority	2
	Wind		FLY

	Type		male
	  Anim          male\flyforward_alt3 26 83
	TEnd
MEnd

Move A_FlyPose3_Post
	Requires	FLY
	Member		<IRQEmotesALL> <ReactIRQ> <HitIRQ> <DeathIRQ> <PostFLYCycle> <AttackALL> <FlyForward>
	Flags		ReqInputs
	NextMove	flyready
	Priority	1
	Scale           1.30

	Type		male
	  Anim          male\flyforward_alt3 84
	TEnd
MEnd

##############################
## Fly Forward - Alt Pose 4 ##
##############################

Move A_FlyPose4_Pre
	Requires	FLY FORWARD ALTERNATE FOUR
	Interrupts	<IRQEmotesALL> <FlyForwardAlt>
	Member		<IRQEmotesALL> <ReactIRQ> <HitIRQ> <DeathIRQ> <AttackALL> <FlyForward>
	NextMove	A_FlyPose4
	Priority	4
	Wind		FLY

	Type		male
	  Anim          male\flyforward_alt4 1 26
	  PlayFx        FEET\FLIGHT3.FX
	TEnd
MEnd

Move A_FlyPose4
	Requires	FLY FORWARD
	Member		<IRQEmotesALL> <ReactIRQ> <HitIRQ> <DeathIRQ> <Flycycle> <AttackALL> <FlyForward>
	Flags		Cycle ReqInputs
	NextMove	A_FlyPose4_Post
	Priority	2
	Wind		FLY

	Type		male
	  Anim          male\flyforward_alt4 26 83
	TEnd
MEnd

Move A_FlyPose4_Post
	Requires	FLY
	Member		<IRQEmotesALL> <ReactIRQ> <HitIRQ> <DeathIRQ> <PostFLYCycle> <AttackALL> <FlyForward>
	Flags		ReqInputs
	NextMove	flyready
	Priority	1
	Scale           1.30

	Type		male
	  Anim          male\flyforward_alt4 84
	TEnd
MEnd

################################
#### Fly Forward - Alt Pose 1 ##
################################
##
##Move flyforward_alt1_pre
##	Requires	FORWARD FLY
##	Interrupts	<FlyReady> <PostMoveCycle> <PostFLYCycle> <BasicMove> <FlyFall> <Jumps>
##	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackALL> <FlyForward>
##	Flags		ReqInputs
##	NextMove        flyforward_alt1_intro
##	Priority        2
##	Wind		FLY
##
##	Type		male
##	  Anim          male\fly 1 25
##	  PlayFx        FEET\FLIGHT3.FX
##	TEnd
##
##	Type		fem_floater
##	  Anim          fem\hover 1 54
##	  PlayFx        FEET\FLIGHT2.FX
##	TEnd
##
##	Type		male_floater
##	  Anim          male\hover 1 54
##	  PlayFx        FEET\FLIGHT2.FX
##	TEnd
##
##	Type		Rularuu_male
##	  Anim          male\mystic_float 1
##	TEnd
##
##	Type		Wisp
##	  Anim          wisp\fly_forward 1 50
##	TEnd
##MEnd
##
##Move flyforward_alt1_intro
##	Requires	FORWARD FLY
##	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Flycycle> <AttackALL> <FlyForward>
##	Flags		Cycle
##	CycleMove       flyforward_alt1_transition
##	NextMove        flypost
##	Priority        2
##
##	Wind		FLY
##
##	Type		male
##	  Anim          male\fly 25 81
##	TEnd
##
##	Type		fem_floater
##	  Anim          fem\hover 57 106
##	TEnd
##
##	Type		male_floater
##	  Anim          male\hover 57 106
##	TEnd
##
##	Type		Rularuu_male
##	  Anim          male\mystic_float 1
##	TEnd
##
##	Type		Wisp
##	  Anim          wisp\fly_forward_cycle 1
##	TEnd
##MEnd
##
##Move flyforward_alt1_transition
##	Requires	FORWARD FLY
##	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackALL> <FlyForward>
##	Flags		ReqInputs
##	NextMove        flyforward_alt1_loop
##	Priority        2
##	Wind		FLY
##
##	Type		male
##	  Anim          male\flyforward_alt1 1 26
##	  PlayFx        FEET\FLIGHT3.FX
##	TEnd
##
##	Type		fem_floater
##	  Anim          fem\hover 57 106
##	TEnd
##
##	Type		male_floater
##	  Anim          male\hover 57 106
##	TEnd
##
##	Type		Rularuu_male
##	  Anim          male\mystic_float 1
##	TEnd
##
##	Type		Wisp
##	  Anim          wisp\fly_forward_cycle 1
##	TEnd
##MEnd
##
##Move flyforward_alt1_loop
##	Requires	FORWARD FLY
##	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Flycycle> <AttackALL> <FlyForward>
##	Flags		Cycle
##	NextMove        flyforward_alt1_post
##	Priority        2
##	Wind		FLY
##
##	Type		male
##	  Anim          male\flyforward_alt1 26 83
##	TEnd
##
##	Type		fem_floater
##	  Anim          fem\hover 57 106
##	TEnd
##
##	Type		male_floater
##	  Anim          male\hover 57 106
##	TEnd
##
##	Type		Rularuu_male
##	  Anim          male\mystic_float 1
##	TEnd
##
##	Type		Wisp
##	  Anim          wisp\fly_forward_cycle 1
##	TEnd
##MEnd
##
##Move flyforward_alt1_post
##	Requires	FLY
##	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostFLYCycle> <AttackALL> <FlyForward>
##	Flags		ReqInputs
##	NextMove        flyready
##	Priority        1
##	Scale           1.30
##
##	Type		male
##	  Anim          male\flyforward_alt1 84
##	TEnd
##
##	Type		fem_floater
##	  Anim          fem\hover 57 106
##	TEnd
##
##	Type		male_floater
##	  Anim          male\hover 57 106
##	TEnd
##
##	Type		Rularuu_male
##	  Anim          male\mystic_float 1
##	TEnd
##
##	Type		Wisp
##	  Anim          wisp\fly_forward_cycle 1
##	TEnd
##MEnd
##
##
################################
#### Fly Forward - Alt Pose 2 ##
################################
##
##Move flyforward_alt2_pre
##	Requires	FORWARD FLY
##	Interrupts	<FlyReady> <PostMoveCycle> <PostFLYCycle> <BasicMove> <FlyFall> <Jumps>
##	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackALL> <FlyForward>
##	Flags		ReqInputs
##	NextMove        flyforward_alt2_intro
##	Priority        2
##	Wind		FLY
##
##	Type		male
##	  Anim          male\fly 1 25
##	  PlayFx        FEET\FLIGHT3.FX
##	TEnd
##
##	Type		fem_floater
##	  Anim          fem\hover 1 54
##	  PlayFx        FEET\FLIGHT2.FX
##	TEnd
##
##	Type		male_floater
##	  Anim          male\hover 1 54
##	  PlayFx        FEET\FLIGHT2.FX
##	TEnd
##
##	Type		Rularuu_male
##	  Anim          male\mystic_float 1
##	TEnd
##
##	Type		Wisp
##	  Anim          wisp\fly_forward 1 50
##	TEnd
##MEnd
##
##Move flyforward_alt2_intro
##	Requires	FORWARD FLY
##	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Flycycle> <AttackALL> <FlyForward>
##	Flags		Cycle
##	CycleMove       flyforward_alt2_transition
##	NextMove        flypost
##	Priority        2
##
##	Wind		FLY
##
##	Type		male
##	  Anim          male\fly 25 81
##	TEnd
##
##	Type		fem_floater
##	  Anim          fem\hover 57 106
##	TEnd
##
##	Type		male_floater
##	  Anim          male\hover 57 106
##	TEnd
##
##	Type		Rularuu_male
##	  Anim          male\mystic_float 1
##	TEnd
##
##	Type		Wisp
##	  Anim          wisp\fly_forward_cycle 1
##	TEnd
##MEnd
##
##Move flyforward_alt2_transition
##	Requires	FORWARD FLY
##	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackALL> <FlyForward>
##	Flags		ReqInputs
##	NextMove        flyforward_alt2_loop
##	Priority        2
##	Wind		FLY
##
##	Type		male
##	  Anim          male\flyforward_alt2 1 26
##	  PlayFx        FEET\FLIGHT3.FX
##	TEnd
##
##	Type		fem_floater
##	  Anim          fem\hover 57 106
##	TEnd
##
##	Type		male_floater
##	  Anim          male\hover 57 106
##	TEnd
##
##	Type		Rularuu_male
##	  Anim          male\mystic_float 1
##	TEnd
##
##	Type		Wisp
##	  Anim          wisp\fly_forward_cycle 1
##	TEnd
##MEnd
##
##Move flyforward_alt2_loop
##	Requires	FORWARD FLY
##	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Flycycle> <AttackALL> <FlyForward>
##	Flags		Cycle
##	NextMove        flyforward_alt2_post
##	Priority        2
##	Wind		FLY
##
##	Type		male
##	  Anim          male\flyforward_alt2 26 82
##	TEnd
##
##	Type		fem_floater
##	  Anim          fem\hover 57 106
##	TEnd
##
##	Type		male_floater
##	  Anim          male\hover 57 106
##	TEnd
##
##	Type		Rularuu_male
##	  Anim          male\mystic_float 1
##	TEnd
##
##	Type		Wisp
##	  Anim          wisp\fly_forward_cycle 1
##	TEnd
##MEnd
##
##Move flyforward_alt2_post
##	Requires	FLY
##	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostFLYCycle> <AttackALL> <FlyForward>
##	Flags		ReqInputs
##	NextMove        flyready
##	Priority        1
##	Scale           1.30
##
##	Type		male
##	  Anim          male\flyforward_alt2 83
##	TEnd
##
##	Type		fem_floater
##	  Anim          fem\hover 57 106
##	TEnd
##
##	Type		male_floater
##	  Anim          male\hover 57 106
##	TEnd
##
##	Type		Rularuu_male
##	  Anim          male\mystic_float 1
##	TEnd
##
##	Type		Wisp
##	  Anim          wisp\fly_forward_cycle 1
##	TEnd
##MEnd
##
##
################################
#### Fly Forward - Alt Pose 3 ##
################################
##
##Move flyforward_alt3_pre
##	Requires	FORWARD FLY
##	Interrupts	<FlyReady> <PostMoveCycle> <PostFLYCycle> <BasicMove> <FlyFall> <Jumps>
##	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackALL> <FlyForward>
##	Flags		ReqInputs
##	NextMove        flyforward_alt3_intro
##	Priority        2
##	Wind		FLY
##
##	Type		male
##	  Anim          male\fly 1 25
##	  PlayFx        FEET\FLIGHT3.FX
##	TEnd
##
##	Type		fem_floater
##	  Anim          fem\hover 1 54
##	  PlayFx        FEET\FLIGHT2.FX
##	TEnd
##
##	Type		male_floater
##	  Anim          male\hover 1 54
##	  PlayFx        FEET\FLIGHT2.FX
##	TEnd
##
##	Type		Rularuu_male
##	  Anim          male\mystic_float 1
##	TEnd
##
##	Type		Wisp
##	  Anim          wisp\fly_forward 1 50
##	TEnd
##MEnd
##
##Move flyforward_alt3_intro
##	Requires	FORWARD FLY
##	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Flycycle> <AttackALL> <FlyForward>
##	Flags		Cycle
##	CycleMove       flyforward_alt3_transition
##	NextMove        flypost
##	Priority        2
##
##	Wind		FLY
##
##	Type		male
##	  Anim          male\fly 25 81
##	TEnd
##
##	Type		fem_floater
##	  Anim          fem\hover 57 106
##	TEnd
##
##	Type		male_floater
##	  Anim          male\hover 57 106
##	TEnd
##
##	Type		Rularuu_male
##	  Anim          male\mystic_float 1
##	TEnd
##
##	Type		Wisp
##	  Anim          wisp\fly_forward_cycle 1
##	TEnd
##MEnd
##
##Move flyforward_alt3_transition
##	Requires	FORWARD FLY
##	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackALL> <FlyForward>
##	Flags		ReqInputs
##	NextMove        flyforward_alt3_loop
##	Priority        2
##	Wind		FLY
##
##	Type		male
##	  Anim          male\flyforward_alt3 1 25
##	  PlayFx        FEET\FLIGHT3.FX
##	TEnd
##
##	Type		fem_floater
##	  Anim          fem\hover 57 106
##	TEnd
##
##	Type		male_floater
##	  Anim          male\hover 57 106
##	TEnd
##
##	Type		Rularuu_male
##	  Anim          male\mystic_float 1
##	TEnd
##
##	Type		Wisp
##	  Anim          wisp\fly_forward_cycle 1
##	TEnd
##MEnd
##
##Move flyforward_alt3_loop
##	Requires	FORWARD FLY
##	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Flycycle> <AttackALL> <FlyForward>
##	Flags		Cycle
##	NextMove        flyforward_alt3_post
##	Priority        2
##	Wind		FLY
##
##	Type		male
##	  Anim          male\flyforward_alt3 26 82
##	TEnd
##
##	Type		fem_floater
##	  Anim          fem\hover 57 106
##	TEnd
##
##	Type		male_floater
##	  Anim          male\hover 57 106
##	TEnd
##
##	Type		Rularuu_male
##	  Anim          male\mystic_float 1
##	TEnd
##
##	Type		Wisp
##	  Anim          wisp\fly_forward_cycle 1
##	TEnd
##MEnd
##
##
##Move flyforward_alt3_post
##	Requires	FLY
##	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostFLYCycle> <AttackALL> <FlyForward>
##	Flags		ReqInputs
##	NextMove        flyready
##	Priority        1
##	Scale           2
##
##	Type		male
##	  Anim          male\flyforward_alt3 83
##	TEnd
##
##	Type		fem_floater
##	  Anim          fem\hover 57 106
##	TEnd
##
##	Type		male_floater
##	  Anim          male\hover 57 106
##	TEnd
##
##	Type		Rularuu_male
##	  Anim          male\mystic_float 1
##	TEnd
##
##	Type		Wisp
##	  Anim          wisp\fly_forward_cycle 1
##	TEnd
##MEnd
##
################################
#### Fly Forward - Alt Pose 4 ##
################################
##
##Move flyforward_alt4_pre
##	Requires	FORWARD FLY
##	Interrupts	<FlyReady> <PostMoveCycle> <PostFLYCycle> <BasicMove> <FlyFall> <Jumps>
##	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackALL> <FlyForward>
##	Flags		ReqInputs
##	NextMove        flyforward_alt4_intro
##	Priority        2
##	Wind		FLY
##
##	Type		male
##	  Anim          male\fly 1 25
##	  PlayFx        FEET\FLIGHT3.FX
##	TEnd
##
##	Type		fem_floater
##	  Anim          fem\hover 1 54
##	  PlayFx        FEET\FLIGHT2.FX
##	TEnd
##
##	Type		male_floater
##	  Anim          male\hover 1 54
##	  PlayFx        FEET\FLIGHT2.FX
##	TEnd
##
##	Type		Rularuu_male
##	  Anim          male\mystic_float 1
##	TEnd
##
##	Type		Wisp
##	  Anim          wisp\fly_forward 1 50
##	TEnd
##MEnd
##
##Move flyforward_alt4_intro
##	Requires	FORWARD FLY
##	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Flycycle> <AttackALL> <FlyForward>
##	Flags		Cycle
##	CycleMove       flyforward_alt4_transition
##	NextMove        flypost
##	Priority        2
##	Wind		FLY
##
##	Type		male
##	  Anim          male\fly 25 81
##	TEnd
##
##	Type		fem_floater
##	  Anim          fem\hover 57 106
##	TEnd
##
##	Type		male_floater
##	  Anim          male\hover 57 106
##	TEnd
##
##	Type		Rularuu_male
##	  Anim          male\mystic_float 1
##	TEnd
##
##	Type		Wisp
##	  Anim          wisp\fly_forward_cycle 1
##	TEnd
##MEnd
##
##Move flyforward_alt4_transition
##	Requires	FORWARD FLY
##	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackALL> <FlyForward>
##	Flags		ReqInputs
##	NextMove        flyforward_alt4_loop
##	Priority        2
##	Wind		FLY
##
##	Type		male
##	  Anim          male\flyforward_alt4 1 25
##	  PlayFx        FEET\FLIGHT3.FX
##	TEnd
##
##	Type		fem_floater
##	  Anim          fem\hover 57 106
##	TEnd
##
##	Type		male_floater
##	  Anim          male\hover 57 106
##	TEnd
##
##	Type		Rularuu_male
##	  Anim          male\mystic_float 1
##	TEnd
##
##	Type		Wisp
##	  Anim          wisp\fly_forward_cycle 1
##	TEnd
##MEnd
##
##Move flyforward_alt4_loop
##	Requires	FORWARD FLY
##	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Flycycle> <AttackALL> <FlyForward>
##	Flags		Cycle
##	NextMove        flyforward_alt4_post
##	Priority        2
##	Wind		FLY
##
##	Type		male
##	  Anim          male\flyforward_alt4 26 82
##	TEnd
##
##	Type		fem_floater
##	  Anim          fem\hover 57 106
##	TEnd
##
##	Type		male_floater
##	  Anim          male\hover 57 106
##	TEnd
##
##	Type		Rularuu_male
##	  Anim          male\mystic_float 1
##	TEnd
##
##	Type		Wisp
##	  Anim          wisp\fly_forward_cycle 1
##	TEnd
##MEnd
##
##Move flyforward_alt4_post
##	Requires	FLY
##	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostFLYCycle> <AttackALL> <FlyForward>
##	Flags		ReqInputs
##	NextMove        flyready
##	Priority        1
##	Scale           2
##
##	Type		male
##	  Anim          male\flyforward_alt4 83
##	TEnd
##
##	Type		fem_floater
##	  Anim          fem\hover 57 106
##	TEnd
##
##	Type		male_floater
##	  Anim          male\hover 57 106
##	TEnd
##
##	Type		Rularuu_male
##	  Anim          male\mystic_float 1
##	TEnd
##
##	Type		Wisp
##	  Anim          wisp\fly_forward_cycle 1
##	TEnd
##MEnd