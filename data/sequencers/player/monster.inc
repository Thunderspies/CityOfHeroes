###################
###### Monster Mode ######
###################

Move Pre_Monster_ready
	Requires	MONSTER
	Interrupts	<Ready>
	Member		<MonsterGroup> <ReactIRQ> <DeathIRQ> <AttackIRQ>
	NextMove	Monster_ready
	Priority        1
	Interpolate	15

	Type		male
	  Anim          male\MonComb_cycle 1 60
	TEnd


MEnd


Move Monster_ready
	Requires	MONSTER
	Member		<MonsterGroup> <ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ>
	Flags		Cycle
	Priority        1

	Type		male
	  Anim          male\MonComb_cycle 1 60
	TEnd


MEnd



####  Monster Motion  #####################################


###################
### Monster run Forward ##
###################

Move Mon_RunPre
	Requires	FORWARD MONSTER
	Interrupts	<MonsterGroup> <Combat> <PostMoveCycle> <Claw> PreClaw <CombatAttack> <Block> <HitIRQ>
	Member		<CombatMove> <DeathIRQ> <AttackIRQ>
	Flags		ReqInputs MoveScale SmoothSprint
	MoveRate        0.7
	NextMove        Mon__RunCycle
	Priority        3

	Type		male
	  Anim          male\goblin_run 1
	  PlayFx        FEET\MALELEFT.FX  10
	  PlayFx        FEET\MALERIGHT.FX  21
	  SmoothSprint  3.3
	TEnd
MEnd




Move Mon__RunCycle
	Requires	FORWARD MONSTER
	Member		<CombatMove> <DeathIRQ> <AttackIRQ>
	Flags		Cycle MoveScale SmoothSprint
	MoveRate        0.7
	NextMove        Mon__RunPost
	Priority        3

	Type		male
	  Anim          male\goblin_run 1
	  PlayFx        FEET\MALELEFT.FX  10
	  PlayFx        FEET\MALERIGHT.FX  21
	  SmoothSprint  3.3
	TEnd
MEnd


Move Mon__RunPost
	Requires	MONSTER
	Member		<MonsterGroup> <CombatMove> <ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ>
	Flags		ReqInputs
	NextMove        Monster_ready
	Priority        1

	Type		male
	  Anim          male\MonComb_postwalk 1
	  PlayFx	FEET\LAND3.FX:LAND  3
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 12
	  scale		2
	TEnd

MEnd




###################
###### Jump  ######
###################

Move Mon_jumpPre
	Requires	JUMP MONSTER
	Interrupts	<CombatMove> <MonsterGroup> <PostMoveCycle> <Ready> <BasicMove> <WepMove> <jumpFall> <Block> <HitIRQ> <Combat> <CombatMove> <CombatAttack>
	Member		<DeathIRQ> <Jumps>
	Flags		ReqInputs
	NextMove        Mon_jumpUp
	Priority        3

	Type		male
	  Anim          male\monster_jump 10 90
	TEnd
MEnd


Move Mon_jumpUp
	Requires	JUMP MONSTER
	Member		<DeathIRQ> <Jumps>
	Flags		Cycle
	NextMove        Mon_jumpApex
	Priority        3

	Type		male
	  Anim          male\monster_jump 91 92
	TEnd
MEnd


Move Mon_jumpApex
	Requires	APEX MONSTER
	Member		<DeathIRQ> <Jumps>
	NextMove        Mon_jumpFall
	Priority        3

	Type		male
	  Anim          male\monster_jump 93 94
	TEnd
MEnd


Move Mon_jumpFall
	Requires	AIR MONSTER
	Member		<DeathIRQ> <Jumps>
	Flags		ReqInputs
	NextMove        Mon_jumpFall2
	Priority        3

	Type		male
	  Anim          male\monster_jump 95 117
	TEnd
MEnd


Move Mon_jumpFall2
	Requires	AIR MONSTER
	Member		<DeathIRQ> <Jumps> <jumpFall>
	Flags		Cycle
	NextMove        Mon_JumpPost
	Priority        3

	Type		male
	  Anim          male\monster_jump 118 120
	TEnd
MEnd


Move Mon_JumpPost
	Requires	MONSTER
	Member		<MonsterGroup> <ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ>
	Priority        1
	Interpolate     0


	Type		male
	  Anim          male\monster_postjump 1
	  PlayFx        FEET\LAND1.FX
	TEnd

MEnd





######## Idle mode  #################################


####################
#### Idle Walk Forward ##
####################

Move Mon_IWalk_Pre
	Requires	FORWARD WALK MONSTER
	Priority        4
	Interrupts	Mon_RunPre <MonsterGroup> <Ready> <BasicMove> <CombatMove> <WepMove> <TwoHandMove>
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <BlockIRQ>
	Flags		ReqInputs
	NextMove        Mon_IWalk_Cycle

	Type		male
	  Anim          male\moncomb_walk 1   ##zombie_walk_armsdown 1 27
	TEnd
MEnd


Move Mon_IWalk_Cycle
	Requires	FORWARD Walk MONSTER
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <BlockIRQ>
	Flags		Cycle
	NextMove        Mon_IWalk_Post
	Priority        3

	Type		male
	  Anim          male\moncomb_walk 1   ##zombie_walk_armsdown 1 27
	TEnd

MEnd


Move Mon_IWalk_Post
	Requires	MONSTER
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <BlockIRQ> <MonsterGroup>
	NextMove        Monster_ready   ##Mon_IdleReady
	Priority        2

	Type		male
	  Anim          male\MonComb_postwalk 1
	  scale		2
	TEnd
MEnd




