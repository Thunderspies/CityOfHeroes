#############################################################
## DOOR MOVEMENTS
#############################################################

## FADEIN RUNOUT  ###########################################################

Move FadeIn_runout
	Requires	EMERGE
	Priority	1
	Interrupts	<AttackIRQ> <BasicMove>	<CombatMove> <WepMove> <TwoHandMove> <Ready> <PostMoveCycle> <Combat> <WepCombat> <TwoHandCombat> <RunIn>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Ready
	Sets		CANTMOVE WALKTHROUGH
	Flags		DoNotDelay

	Type		male
	  Anim		male\Skel_ready2 1 15
	TEnd

	Type		huge
	  Anim		huge\Skel_ready	1 15
	TEnd

	Type		fem
	  Anim		fem\Skel_ready2	1 15
	TEnd
MEnd

## BASIC RUNOUT  ###########################################################

Move Basic_runout
	Requires	EMERGE RUNOUT
	Priority	2
	Interrupts	<AttackIRQ> <BasicMove>	<CombatMove> <WepMove> <TwoHandMove> <Ready> <PostMoveCycle> <Combat> <WepCombat> <TwoHandCombat> <RunIn>
	NextMove	Ready
	Sets		CANTMOVE WALKTHROUGH
	Flags		DoNotDelay

	Type		male
	  Anim		male\basic_runout 2
	TEnd

	Type		huge
	  Anim		huge\basic_runout 2
	TEnd

	Type		fem
	  Anim		fem\basic_runout 2
	TEnd
MEnd

## ELEVATOR RUNOUT ###########################################################

Move Elevator_Wait
	Requires	EMERGE ELEVATOR
	Priority	2
	Interrupts	<AttackIRQ> <BasicMove>	<CombatMove> <WepMove> <TwoHandMove> <Ready> <PostMoveCycle> <Combat> <WepCombat> <TwoHandCombat> <RunIn>
	NextMove	Elevator_runout
	Sets		CANTMOVE WALKTHROUGH
	Flags		DoNotDelay
	Scale		0.5

	Type		male
	  Anim		male\elevator_wait 1 30
	TEnd
MEnd

Move Elevator_runout
	NextMove	Ready
	Sets		CANTMOVE WALKTHROUGH
	Interpolate	8

	Type		male
	  Anim		male\basic_runout 57
	TEnd

	Type		huge
	  Anim		huge\basic_runout 57
	TEnd

	Type		fem
	  Anim		fem\basic_runout 57
	TEnd
MEnd

##############################################################
#### ELEVATOR RUNIN	####
####################

Move Elevator_Run_IN
	Requires	RUNIN ELEVATOR
	Priority	3
	Interrupts	<AttackIRQ> <BasicMove>	<CombatMove> <WepMove> <TwoHandMove> <Ready> <PostMoveCycle> <Combat> <WepCombat> <TwoHandCombat> <RunIn>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Elevator_Run
	Sets		CANTMOVE RUNIN
	Interpolate	8

	Type		male
	  Anim		male\walk 1 48
	  PlayFx	FEET\NPCMALELEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\NPCMALERIGHT.FX:FOOTFALLRIGHT 25
	  PlayFx	Generic\Faerie_entityFadeOut.fx	5 constant
	  MoveRate	0.146
	  Scale		1.1
	TEnd
MEnd

Move Elevator_Run
	Member		<RunIn>
	#Sets		CANTMOVE
	Flags		cycle NotSelectable
	NextMove	Elevator_Run_In_post

	Type		male
	  Anim		male\walk 1 48
	  PlayFx	FEET\NPCMALELEFT.FX:FOOTFALLLEFT 1
	  PlayFx	FEET\NPCMALERIGHT.FX:FOOTFALLRIGHT 25
	  PlayFx	Generic\Faerie_entityFadeOut.fx	0 constant
	  MoveRate	0.146
	  Scale		1.1
	TEnd
MEnd


Move Elevator_Run_In_post
	Member		<RunIn>
	#Sets		CANTMOVE
	Flags		NotSelectable
	NextMove	Ready

	Type		male
	  Anim		male\postback_right 28
	  PlayFx	Generic\Faerie_entityFadeOut.fx	0 constant
	TEnd

MEnd


#############################################################
### TV Warp IN	####
###################

Move TV_Warp_IN
	Requires	RUNIN TVWARPIN
	Priority	2
	Interrupts	<AttackIRQ> <BasicMove>	<CombatMove> <WepMove> <TwoHandMove> <Ready> <PostMoveCycle> <Combat> <WepCombat> <TwoHandCombat> <RunIn>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	TV_Warp_In_post
	Sets		CANTMOVE RUNIN
	Interpolate	8

	Type		male
	  Anim		male\portal_in 1
	  PlayFx	Generic\Faerie_entityFadeOut.fx	5 constant
	TEnd
MEnd

Move TV_Warp_In_post
	Member		<RunIn>
	Sets		CANTMOVE
	Flags		NotSelectable
	NextMove	Ready

	Type		male
	  Anim		male\absorb 60
	  PlayFx	Generic\Faerie_entityFadeOut.fx	0 constant
	TEnd
MEnd


###################
### TV Warp OUT	 ####
###################

Move TV_Warp_OUT
	Requires	EMERGE TVWARPOUT
	Priority	2
	Interrupts	<AttackIRQ> <BasicMove>	<CombatMove> <WepMove> <TwoHandMove> <Ready> <PostMoveCycle> <Combat> <WepCombat> <TwoHandCombat> <RunIn>
	NextMove	TV_Warp_post_cycle
	Sets		CANTMOVE
	Flags		DoNotDelay

	Type		male
	  Anim		male\portal_out	1
	  PlayFx	Generic\entityFadeIn.fx
	TEnd
MEnd

Move TV_Warp_post_cycle
	Member		<Ready>	<ReactIRQ> <DeathIRQ> <AttackIRQ>
	NextMove	Ready

	Type		male
	  Anim		male\lookquick 1
	TEnd
MEnd


###################
### Truck Jump IN  ####
###################

Move Truck_Jump_IN
	Requires	RUNIN JUMPINTRUCK
	Priority	2
	Interrupts	<AttackIRQ> <BasicMove>	<CombatMove> <WepMove> <TwoHandMove> <Ready> <PostMoveCycle> <Combat> <WepCombat> <TwoHandCombat> <RunIn>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Truck_Jump_In_post
	Sets		CANTMOVE RUNIN
	Interpolate	8

	Type		male
	  Anim		male\Truck_jump_in 1
	  PlayFx	Generic\Faerie_entityFadeOut.fx	0 constant
	TEnd
MEnd

Move Truck_Jump_In_post
	NextMove	Ready
	Priority	5
	Member		<RunIn>
	Sets		CANTMOVE
	Flags		NotSelectable

	Type		male
	  Anim		male\Truck_jump_in 30
	  PlayFx	Generic\Faerie_entityFadeOut.fx	0 constant
	TEnd
MEnd

###################
### Truck Jump OUT  ####
###################

Move Truck_Jump_OUT
	Requires	EMERGE JUMPOUTOFTRUCK
	Priority	2
	Interrupts	<AttackIRQ> <BasicMove>	<CombatMove> <WepMove> <TwoHandMove> <Ready> <PostMoveCycle> <Combat> <WepCombat> <TwoHandCombat> <RunIn>
	NextMove	Truck_post_cycle
	Sets		CANTMOVE
	Flags		DoNotDelay

	Type		male
	  Anim		male\Truck_jump_out 1
	  PlayFx	Generic\entityFadeIn.fx
	TEnd
MEnd

Move Truck_post_cycle
	Member		<Ready>	<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ>
	NextMove	Ready

	Type		male
	  Anim		male\lookquick 1
	TEnd
MEnd


###################
### Snake Hole Jump IN	####
###################

Move SnakeHole_Jump_IN
	Requires	RUNIN JUMPINHOLE
	Priority	2
	Interrupts	<AttackIRQ> <BasicMove>	<CombatMove> <WepMove> <TwoHandMove> <Ready> <PostMoveCycle> <Combat> <WepCombat> <TwoHandCombat> <RunIn>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Truck_Jump_In_post
	Sets		CANTMOVE RUNIN
	Flags		NotSelectable
	Interpolate	8

	Type		male
	  Anim		male\Hole_jump_IN 1
	  PlayFx	Generic\Faerie_entityFadeOut.fx	0 constant
	TEnd
MEnd

Move SnakeHole_Jump_In_post
	Member		<RunIn>
	Sets		CANTMOVE
	Flags		NotSelectable
	NextMove	Ready

	Type		male
	  Anim		male\absorb 60
	  PlayFx	Generic\Faerie_entityFadeOut.fx	0 constant
	TEnd
MEnd


###################
### Snake Hole Jump OUT	 ####
###################

Move SnakeHole_Jump_OUT
	Requires	EMERGE JUMPOUTOFHOLE
	Priority	2
	Interrupts	<AttackIRQ> <BasicMove>	<CombatMove> <WepMove> <TwoHandMove> <Ready> <PostMoveCycle> <Combat> <WepCombat> <TwoHandCombat> <RunIn>
	NextMove	Truck_post_cycle
	Sets		CANTMOVE
	Flags		NotSelectable DoNotDelay

	Type		male
	  Anim		male\hole_jump_out 1
	TEnd
MEnd

Move SnakeHole_post_cycle
	Member		<Ready>	<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ>
	NextMove	Ready

	Type		male
	  Anim		male\lookquick 1
	TEnd
MEnd


###################
### Warp In    ####
###################

Move Warp_runin
	Requires	RUNIN WARP
	Priority	2
	Interrupts	<AttackIRQ> <BasicMove>	<CombatMove> <WepMove> <TwoHandMove> <Ready> <PostMoveCycle> <Combat> <WepCombat> <TwoHandCombat>
	Member		<RunIn>
	NextMove	Warp_runin_post
	Sets		RUNIN #CANTMOVE

	Type		male
	  Anim		male\absorb 1 59
	  #PlayFx	WORLD\City\Arena\WaitingRoomTeleport.fx
	  PlayFx	Generic\Faerie_entityFadeOut.fx	0 constant
	TEnd

	Type		huge
	  Anim		huge\boostup 1 59
	  #PlayFx	WORLD\City\Arena\WaitingRoomTeleport.fx
	  PlayFx	Generic\Faerie_entityFadeOut.fx	0 constant
	TEnd

	Type		fem
	  Anim		fem\boostup 1 59
	  #PlayFx	WORLD\City\Arena\WaitingRoomTeleport.fx
	  PlayFx	Generic\Faerie_entityFadeOut.fx	0 constant
	TEnd
MEnd

Move Warp_runin_post
	NextMove	Ready
	Member		<RunIn>
	#Sets		CANTMOVE
	Flags		NotSelectable

	Type		male
	  Anim		male\absorb 60
	  PlayFx	Generic\Faerie_entityFadeOut.fx	0 constant
	TEnd

	Type		huge
	  Anim		huge\boostup 60
	  PlayFx	Generic\Faerie_entityFadeOut.fx	0 constant
	TEnd

	Type		fem
	  Anim		fem\boostup 60
	  PlayFx	Generic\Faerie_entityFadeOut.fx	0 constant
	TEnd
MEnd


###################
### Warp Out	####
###################

Move Warp_runout
	Requires	ARENAEMERGE
	Priority	1
	Interrupts	Warp_runin_post	<AttackIRQ> <BasicMove>	<CombatMove> <WepMove> <TwoHandMove> <Ready>  <PostMoveCycle> <Combat> <WepCombat> <TwoHandCombat> <RunIn>
	NextMove	Ready
	Sets		CANTMOVE WALKTHROUGH
	Flags		DoNotDelay
	Interpolate	0

	Type		male
	  Anim		male\hover 57 106
	  #Anim		 male\portal_out 1
	  PLAYFX	WORLD\City\Arena\WaitingRoomTeleportEntry.fx
	TEnd
MEnd


####################
### Faerie Warp	In  ####
####################

Move FaerieWarp_runin
	Requires	MAGIC RUNIN WARP
	Priority	3
	Interrupts	<AttackIRQ> <BasicMove>	<CombatMove> <WepMove> <TwoHandMove> <Ready> <PostMoveCycle> <Combat> <WepCombat> <TwoHandCombat>
	Member		<RunIn>
	NextMove	FaerieWarp_runin_post
	Sets		CANTMOVE RUNIN

	Type		male
	  Anim		male\absorb 1 59
	  PlayFx	Generic\Faerie_entityFadeOut.fx	0 constant
	  PlayFX	EnemyFx\DevouringEarth\AOEPowers\mushroomportalClicked.fx 0 Constant
	TEnd
MEnd

Move FaerieWarp_runin_post
	Member		<RunIn>
	NextMove	Ready

	Type		male
	  Anim		male\absorb 60
	  PlayFx	Generic\Faerie_entityFadeOut.fx	0 constant
	TEnd
MEnd


###################
### Warp Out	####
###################

Move FaerieWarp_runout
	Requires	MAGIC ARENAEMERGE
	Priority	2
	Interrupts	FaerieWarp_runin_post <AttackIRQ> <BasicMove> <CombatMove> <WepMove> <TwoHandMove> <Ready> <PostMoveCycle> <Combat> <WepCombat>	<TwoHandCombat>	<RunIn>
	NextMove	Ready
	Sets		CANTMOVE WALKTHROUGH
	Interpolate	0

	Type		male
	  Anim		male\portal_out	30
	  PlayFX	EnemyFx\DevouringEarth\AOEPowers\mushroomportalClicked.fx 0
	TEnd
MEnd


#######################################################################################################
## Vanguard Base - Warp	In
#######################################################################################################

Move VanguardWarp_runin
	Requires	RUNIN WARP VARIATIONA
	Priority	3
	Interrupts	<AttackIRQ> <BasicMove>	<CombatMove> <WepMove> <TwoHandMove> <Ready> <PostMoveCycle> <Combat> <WepCombat> <TwoHandCombat>
	Member		<RunIn>
	NextMove	Warp_runin_post
	Sets		RUNIN CANTMOVE

	Type		male
	  Anim		male\absorb 1 59
	  #PlayFx	WORLD\City\Arena\WaitingRoomTeleport.fx
	  PlayFx	Generic\Faerie_entityFadeOut.fx	0 constant
	TEnd

	Type		huge
	  Anim		huge\boostup 1 59
	  #PlayFx	WORLD\City\Arena\WaitingRoomTeleport.fx
	  PlayFx	Generic\Faerie_entityFadeOut.fx	0 constant
	TEnd

	Type		fem
	  Anim		fem\boostup 1 59
	  #PlayFx	WORLD\City\Arena\WaitingRoomTeleport.fx
	  PlayFx	Generic\Faerie_entityFadeOut.fx	0 constant
	TEnd
MEnd

Move VanguardWarp_runin_post
	NextMove	Ready
	Member		<RunIn>
	#Sets		 CANTMOVE
	Flags		NotSelectable

	Type		male
	  Anim		male\absorb 60
	  PlayFx	Generic\Faerie_entityFadeOut.fx	0 constant
	TEnd

	Type		huge
	  Anim		huge\boostup 60
	  PlayFx	Generic\Faerie_entityFadeOut.fx	0 constant
	TEnd

	Type		fem
	  Anim		fem\boostup 60
	  PlayFx	Generic\Faerie_entityFadeOut.fx	0 constant
	TEnd
MEnd


#######################################################################################################
## Vanguard Base - Warp	Out
#######################################################################################################

Move VanguardWarp_runout
	Requires	ARENAEMERGE VARIATIONA
	Priority	2
	Interrupts	Warp_runin_post	<AttackIRQ> <BasicMove>	<CombatMove> <WepMove> <TwoHandMove> <Ready> <PostMoveCycle> <Combat> <WepCombat> <TwoHandCombat> <RunIn>
	NextMove	Ready
	Sets		CANTMOVE WALKTHROUGH
	Flags		DoNotDelay
	Interpolate	0

	Type		male
	  Anim		male\hover 57 106
	  #Anim		 male\portal_out 1
	  PLAYFX	WORLD\City\Arena\WaitingRoomTeleportEntry.fx
	TEnd
MEnd

#############################################################
## Mission Architect Warp In
#############################################################

Move MA_Warp_IN
	Requires	MISSIONARCHITECT RUNIN
	Priority	2
	Interrupts	<AttackIRQ> <BasicMove>	<CombatMove> <WepMove> <TwoHandMove> <Ready> <PostMoveCycle> <Combat> <WepCombat> <TwoHandCombat> <RunIn>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	MA_Warp_In_Post
	Sets		CANTMOVE RUNIN

	Type		male
	  Anim		male\WarpUp 1 40
	  PlayFx	WORLD\Bases\MissionArchitect\WarpOut.fx 0 CONSTANT
	TEnd
MEnd

Move MA_Warp_In_Post
	NextMove	Ready
	Member		<RunIn> <MoveIRQ>
	Flags		NotSelectable

	Type		male
	  Anim		male\WarpUp 40 45
	  PlayFx	WORLD\Bases\MissionArchitect\WarpOut.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
## Mission Architect Warp Out
#############################################################

Move MA_Warp_OUT
	Requires	MISSIONARCHITECT EMERGE
	Priority	2
	Interrupts	<AttackIRQ> <BasicMove>	<CombatMove> <WepMove> <TwoHandMove> <Ready> <PostMoveCycle> <Combat> <WepCombat> <TwoHandCombat> <RunIn>
	Interpolate	0
	NextMove	MA_Warp_OUT_PostCycle
	Sets		CANTMOVE
	Flags		DoNotDelay

	Type		male
	  Anim		male\WarpUp 45
	  PlayFx	WORLD\Bases\MissionArchitect\WarpIn.fx 0 CONSTANT
	TEnd
MEnd

Move MA_Warp_OUT_PostCycle
	Member		<Ready>	<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <MoveIRQ>
	NextMove	Ready

	Type		male
	  Anim		male\lookquick 1
	TEnd
MEnd

#############################################################
## 5TH COLUMN PORTAL - TELEPORT IN/OUT
#############################################################

Move 5thColumnPortal_WarpIn
	Requires	RUNIN FIFTHCOLUMN
	Priority	2
	Interrupts	<AttackIRQ> <BasicMove>	<CombatMove> <WepMove> <TwoHandMove> <Ready> <PostMoveCycle> <Combat> <WepCombat> <TwoHandCombat> <RunIn>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	5thColumnPortal_WarpIn_PostCycle
	Sets		CANTMOVE RUNIN

	Type		male
	  Anim		male\absorb 1 60
	  PlayFX	ENEMYFX\5thColumn\Portal\FifthCol_Portal_Enter.fx 0 CONSTANT
	TEnd
MEnd

Move 5thColumnPortal_WarpIn_PostCycle
	Member		<DeathIRQ> <Ready> <ReactIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ>
	Flags		NotSelectable
	NextMove	Ready

	Type		male
	  Anim		male\absorb 60 61
	  PlayFX	ENEMYFX\5thColumn\Portal\FifthCol_Portal_Enter.fx 0 CONSTANT
	TEnd
MEnd

#############################################################

Move 5thColumnPortal_WarpOut
	Requires	EMERGE FIFTHCOLUMN
	Priority	2
	Interrupts	<AttackIRQ> <BasicMove>	<CombatMove> <WepMove> <TwoHandMove> <Ready> <PostMoveCycle> <Combat> <WepCombat> <TwoHandCombat> <RunIn>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	5thColumnPortal_WarpOut_PostCycle
	Sets		CANTMOVE
	Flags		DoNotDelay

	Type		male
	  Anim		male\absorb 1 60
	  PlayFX	ENEMYFX\5thColumn\Portal\FifthCol_Portal_Exit.fx 0 CONSTANT
	TEnd
MEnd

Move 5thColumnPortal_WarpOut_PostCycle
	Member		<DeathIRQ> <Ready> <ReactIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ>
	NextMove	Ready

	Type		male
	  Anim		male\absorb 60
	  PlayFX	ENEMYFX\5thColumn\Portal\FifthCol_Portal_Exit.fx 0 CONSTANT
	TEnd
MEnd