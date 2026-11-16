#############################################################
## FALLS
#############################################################

## RUN FALL ###########################################################

Move RunFall
	Requires	BIGFALL
	Priority	1
	Interrupts	<BasicMove> <WalkMove> <DualPistolsMove> <CombatMove> <WepMove> <TwoHandMove> <TravelMove> <TravelWepMove> <TravelDualHandMove> <TravelLeftHandMove> <TravelShieldMove> <TravelShieldWepMove> <Ready> <Combat> <WepCombat> <TwoHandCombat> <iceslide> <MoveIRQ>
	Member		<DeathIRQ> <ReactIRQ> <AttackIRQ>  <Fall>
	Flags		ReqInputs
	NextMove	RunFall_2

	Type		male
	  Anim		male\superjump 67 93
	TEnd

	Type		huge
	  Anim		huge\superjump 67 93
	TEnd

	Type		fem
	  Anim		fem\superjump 67 93
	TEnd
MEnd

Move RunFall_2
	Requires	AIR
	Priority	1
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ>  <Fall>
	Flags		Cycle
	NextMove	JumpPost

	Type		male
	  Anim		male\superjump 93
	TEnd

	Type		huge
	  Anim		huge\superjump 93
	TEnd

	Type		fem
	  Anim		fem\superjump 93
	TEnd
MEnd


## FALL ###########################################################

Move PreFall
	Requires	AIR
	Priority	1
	#Interrupts	<MoveIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ>  <Fall>
	Flags		Cycle
	NextMove	JumpPost

	Type		male
	  Anim		male\flydown 1
	TEnd

	Type		huge
	  Anim		huge\flydown 1
	TEnd

	Type		fem
	  Anim		fem\fly_down 1
	TEnd
MEnd


## FALL FORWARD ###########################################################

Move Ffly_PreFall
	Requires	FORWARD	AIR
	Priority	2
	Interrupts	flyPost	<MoveIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ>  <Fall>
	NextMove	Ffly_PreFall_2

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

Move Ffly_PreFall_2
	Requires	AIR
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ>  <Fall>
	Flags		Cycle
	NextMove	JumpPost
	Priority	1

	Type		male
	  Anim		male\flydown 4
	TEnd

	Type		huge
	  Anim		huge\flydown 4
	TEnd

	Type		fem
	  Anim		fem\fly_down 4
	TEnd
MEnd


## FALL BACKWARD ###########################################################

Move Bfly_PreFall
	Requires	BACKWARD AIR
	Interrupts	<FlyReady> BackflyPost <MoveIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ>  <Fall>
	NextMove	PreFall
	Priority	2

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


## FALL LEFT ###########################################################

Move Lfly_PreFall
	Requires	STEPLEFT AIR
	Priority	2
	Interrupts	<FlyReady> LeftflyPost <MoveIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ>  <Fall>
	NextMove	PreFall

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


## FALL RIGHT ###########################################################

Move Rfly_PreFall
	Requires	STEPRIGHT AIR
	Priority	2
	Interrupts	<FlyReady> RightflyPost	<MoveIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ>  <Fall>
	NextMove	PreFall

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
## COMBAT MODE
#############################################################

## COMBAT FALL ###########################################################

Move C_RunFall
	Requires	BIGFALL	COMBAT
	Priority	2
	Interrupts	<CombatMove> <iceslide>	<MoveIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ>  <Fall>
	Flags		ReqInputs
	NextMove	C_RunFall_2

	Type		male
	  Anim		male\superjump 67 93
	TEnd

	Type		huge
	  Anim		huge\superjump 67 93
	TEnd

	Type		fem
	  Anim		fem\superjump 67 93
	TEnd
MEnd

Move C_RunFall_2
	Requires	AIR COMBAT
	Priority	2
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ>  <Fall>
	Flags		Cycle
	NextMove	C_JumpPost

	Type		male
	  Anim		male\superjump 93
	TEnd

	Type		huge
	  Anim		huge\superjump 93
	TEnd

	Type		fem
	  Anim		fem\superjump 93
	TEnd
MEnd


## COMBAT FALL ###########################################################

Move C_PreFall
	Requires	AIR COMBAT
	Priority	2
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ>  <Fall>
	Flags		Cycle
	NextMove	C_JumpPost

	Type		male
	  Anim		male\flydown 1
	TEnd

	Type		huge
	  Anim		huge\flydown 1
	TEnd

	Type		fem
	  Anim		fem\fly_down 1
	TEnd

	Type		Rularuu_male
	  Anim		male\mystic_float 1
	TEnd

	Type		Wisp
	  Anim		wisp\Float 1
	TEnd
MEnd


## COMBAT FALL FORWARD ###########################################################

Move C_Ffly_PreFall
	Requires	FORWARD	AIR COMBAT
	Priority	3
	Interrupts	C_flyPost <MoveIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ>  <Fall>
	NextMove	C_Ffly_PreFall_2

	Type		male
	  Anim		male\flydown 1 3
	TEnd

	Type		huge
	  Anim		huge\flydown 1 3
	TEnd

	Type		fem
	  Anim		fem\fly_down 1 3
	TEnd

	Type		Rularuu_male
	  Anim		male\mystic_float 1
	TEnd
MEnd

Move C_Ffly_PreFall_2
	Requires	AIR COMBAT
	Priority	2
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ>  <Fall>
	Flags		Cycle
	NextMove	C_JumpPost

	Type		male
	  Anim		male\flydown 4
	TEnd

	Type		huge
	  Anim		huge\flydown 4
	TEnd

	Type		fem
	  Anim		fem\fly_down 4
	TEnd

	Type		Rularuu_male
	  Anim		male\mystic_float 1
	TEnd
MEnd

#############################################################
## WEAPON MODE
#############################################################

## WEAPON FALL ###########################################################

Move W_RunFall
	Requires	BIGFALL	WEAPON
	Priority	2
	Interrupts	<WepMove> <iceslide> <MoveIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ>  <Fall>
	Flags		ReqInputs
	NextMove	W_RunFall_2

	Type		male
	  Anim		male\wep_superjump 67 89
	TEnd

	Type		huge
	  Anim		huge\wep_superjump 67 89
	TEnd

	Type		fem
	  Anim		fem\superjump 67 89
	TEnd
MEnd

Move W_RunFall_2
	Requires	AIR WEAPON
	Priority	2
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ>  <Fall>
	Flags		Cycle
	NextMove	W_JumpPost

	Type		male
	  Anim		male\wep_superjump 89
	TEnd

	Type		huge
	  Anim		huge\wep_superjump 89
	TEnd

	Type		fem
	  Anim		fem\superjump 89
	TEnd
MEnd


## WEAPON FALL ###########################################################

Move W_PreFall
	Requires	AIR COMBAT WEAPON
	Priority	3
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ>  <Fall>
	Flags		Cycle
	NextMove	W_JumpPost

	Type		male
	  Anim		male\wep_flydown 1
	TEnd

	Type		huge
	  Anim		huge\air_wep_flydown 1
	TEnd

	Type		fem
	  Anim		fem\air_wep_flydown 1
	TEnd
MEnd


## WEAPON FALL FORWARD ###########################################################

Move W_Ffly_PreFall
	Requires	FORWARD	AIR COMBAT WEAPON
	Priority	4
	Interrupts	W_flyPost <MoveIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ>  <Fall>
	NextMove	W_Ffly_PreFall_2

	Type		male
	  Anim		male\wep_flydown 1 3
	TEnd

	Type		huge
	  Anim		huge\air_wep_flydown 1 3
	TEnd

	Type		fem
	  Anim		fem\air_wep_flydown 1 3
	TEnd
MEnd

Move W_Ffly_PreFall_2
	Requires	AIR COMBAT WEAPON
	Priority	3
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ>  <Fall>
	Flags		Cycle
	NextMove	W_JumpPost

	Type		male
	  Anim		male\wep_flydown 4
	TEnd

	Type		huge
	  Anim		huge\air_wep_flydown 4
	TEnd

	Type		fem
	  Anim		fem\air_wep_flydown 4
	TEnd
MEnd


#############################################################
## DUAL WIELD MODE
#############################################################

## DUAL WIELD FALL ###########################################################

Move DualWield_PreFall
	Requires	AIR COMBAT WEAPON DUAL
	Priority	4
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ>  <Fall>
	Flags		Cycle
	NextMove	DualWield_JumpPost

	Type		male
	  Anim		male\wep_flydown 1
	TEnd
MEnd

Move DualWield_RunFall
	Requires	BIGFALL	WEAPON DUAL
	Priority	3
	Interrupts	<WepMove> <DualHandMove> <iceslide>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ>  <Fall>
	Flags		ReqInputs
	NextMove	DualWield_RunFall_2

	Type		male
	  Anim		male\dual_hop 94 128
	TEnd
MEnd

Move DualWield_RunFall_2
	Requires	AIR WEAPON DUAL
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ>  <Fall>
	Flags		Cycle
	NextMove	DualWield_JumpPost
	Priority	3

	Type		male
	  Anim		male\dual_hop 128
	TEnd
MEnd

## DUAL WIELD FALL FORWARD ###########################################################

Move DualWield_ForwardFly_PreFall
	Requires	FORWARD	AIR COMBAT WEAPON DUAL
	Priority	5
	Interrupts	D_flyPost
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ>  <Fall>
	NextMove	DualWield_ForwardFly_PreFall_2

	Type		male
	  Anim		male\wep_flydown 1 3
	TEnd
MEnd

Move DualWield_ForwardFly_PreFall_2
	Requires	AIR COMBAT WEAPON DUAL
	Priority	4
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ>  <Fall>
	Flags		Cycle
	NextMove	DualWield_JumpPost

	Type		male
	  Anim		male\wep_flydown 4
	TEnd
MEnd

#############################################################
## SLIDE MODE
#############################################################

## SLIDE FALL ###########################################################

Move Slide_RunFall
	Requires	BIGFALL
	Priority	1
	Interrupts	<BasicSlide> <MoveIRQ>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ>  <Fall>
	Flags		ReqInputs
	NextMove	Slide_RunFall_2

	Type		male
	  Anim		male\windmill_jump 20 26
	TEnd
MEnd

Move Slide_RunFall_2
	Requires	AIR
	Priority	1
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ>  <Fall>
	Flags		Cycle
	NextMove	JumpPost

	Type		male
	  Anim		male\windmill_jump 1 26
	TEnd
MEnd