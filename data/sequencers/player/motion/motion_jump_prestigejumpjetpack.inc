#############################################################
## PRESTIGE JUMP JET PACK
#############################################################

## JUMP	JET PACK - UP ###########################################################

Move JumpJetPack_Pre
	Requires	JUMP PRESTIGEJUMPJETPACK AIR
	Priority	3
	Interrupts	<PostMoveCycle>	<Ready>	<BasicMove> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	JumpJetPack_Up

	Type		male
	  Anim		male\superjump 3 6
	  PlayFx	FEET\SUPERJUMP1.FX:JUMP
	TEnd

	Type		huge
	  Anim		huge\superjump 3 6
	  PlayFx	FEET\HUGEJUMP2.FX:JUMP
	TEnd

	Type		fem
	  Anim		fem\superjump 3	6
	  PlayFx	FEET\FEMJUMP1.FX:JUMP
	TEnd

	Type		Sprocket
	  Anim		male\superjump 3 6
	TEnd

	Type		RockSuit
	  Anim		huge\superjump 3 6
	TEnd
MEnd

Move JumpJetPack_Up
	Requires	JUMP PRESTIGEJUMPJETPACK AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	JumpJetPack_Apex
	Priority	3

	Type		male
	  Anim		male\superjump 7 64
	TEnd

	Type		huge
	  Anim		huge\superjump 7 64
	TEnd

	Type		fem
	  Anim		fem\superjump 7	64
	TEnd
MEnd

Move JumpJetPack_Apex
	Requires	APEX PRESTIGEJUMPJETPACK AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	JumpJetPack_Fall
	Priority	3

	Type		male
	  Anim		male\superjump 65 66
	TEnd

	Type		huge
	  Anim		huge\superjump 65 66
	TEnd

	Type		fem
	  Anim		fem\superjump 65 66
	TEnd
MEnd

Move JumpJetPack_Fall
	Requires	PRESTIGEJUMPJETPACK AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	JumpJetPack_Fall2
	Priority	2

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

Move JumpJetPack_Fall2
	Requires	PRESTIGEJUMPJETPACK AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	JumpJetPack_Post
	Priority	2

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

Move JumpJetPack_Post
	Requires	PRESTIGEJUMPJETPACK
	Include sequencers\Members\readies.inc
	NextMove	postReady_back
	Priority	2
	Interpolate	1
	Scale		0.7
	SticksOnChild	LAND

	Type		male
	  Anim		male\comb_postjump 1
	  PlayFx	FEET\LAND1.FX:LAND
	TEnd

	Type		CameraShaker
	  Anim		male\comb_postjump 1
	  PlayFx	FEET\LAND1.FX:LAND
	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsRIGHT.FX:FOOTFALLRIGHT
	TEnd

	Type		huge
	  Anim		huge\combat_postjump 1
	  PlayFx	FEET\HUGELAND2.FX:LAND
	TEnd

	Type		fem
	  Anim		fem\combat_postjump 1
	  PlayFx	FEET\LAND1.FX:LAND
	TEnd

	Type		RockSuit
	  Anim		male\comb_postjump 1
	  PlayFx	FEET\LAND1.FX:LAND
	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsRIGHT.FX:FOOTFALLRIGHT
	TEnd

	Type		RockSuit
	  Anim		male\comb_postjump 1
	  PlayFx	FEET\giantfoot1.fx:FOOTFALLLEFT:FOOTFALLLEFT  2
	  PlayFx	FEET\giantfoot2.fx:FOOTFALLRIGHT:FOOTFALLRIGHT	3
	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsRIGHT.FX:FOOTFALLRIGHT
	  Scale		1
	TEnd
MEnd

#############################################################

Move JumpJetPack_Damage
	Requires	HEADPAIN PRESTIGEJUMPJETPACK
	Interrupts	JumpJetPack_Post
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ>
	NextMove	postReady_back
	Priority	2
	Interpolate	1

	Type		male
	  Anim		male\fall_dammage 1
	  PlayFx	FEET\LAND1.FX:LAND
	TEnd

	Type		CameraShaker
	  Anim		male\fall_dammage 1
	  PlayFx	FEET\LAND1.FX:LAND
	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsRIGHT.FX:FOOTFALLRIGHT
	TEnd

	Type		huge
	  Anim		huge\thumbsup 1
	TEnd

	Type		fem
	  Anim		fem\thumbsup 1
	TEnd
MEnd

#############################################################

Move JumpJetPackHopFromJump
	Requires	FORWARD	JUMP PRESTIGEJUMPJETPACK AIR
	Interrupts	JumpJetPack_Pre	JumpJetPack_Up <PostMoveCycle> <Ready> <BasicMove> <SkillMove> RunPre <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	JumpJetPackForward_Up2
	Priority	4
	Interpolate	15

	Type		male
	  Anim		male\test_hop 10 91
	TEnd

	Type		huge
	  Anim		huge\hop 10 91
	TEnd

	Type		fem
	  Anim		fem\hop	10 91
	TEnd
MEnd


## JUMP	JET PACK - FORWARD ###########################################################

Move JumpJetPackForward_Pre
	Requires	FORWARD	JUMP PRESTIGEJUMPJETPACK AIR
	Interrupts	<PostMoveCycle>	<Ready>	<BasicMove> <SkillMove>	RunPre <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	JumpJetPackForward_Up
	Priority	5

	Type		male
	  Anim		male\test_hop 10 11
	TEnd

	Type		huge
	  Anim		huge\hop 10 11
	TEnd

	Type		fem
	  Anim		fem\hop	10 11
	TEnd
MEnd

Move JumpJetPackForward_Up
	Requires	JUMP PRESTIGEJUMPJETPACK AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	JumpJetPackForward_Up2
	Priority	4

	Type		male
	  Anim		male\test_hop 12 91
	  PlayFx	FEET\JUMP1.FX:JUMP
	TEnd

	Type		huge
	  Anim		huge\hop 12 91
	  PlayFx	FEET\HUGEJUMP2.FX:JUMP
	TEnd

	Type		fem
	  Anim		fem\hop	12 91
	  PlayFx	FEET\FEMJUMP1.FX:JUMP
	TEnd

	Type		sprocket
	  Anim		male\test_hop 12 91
	TEnd

	Type		RockSuit
	  Anim		huge\hop 12 23
	TEnd
MEnd

Move JumpJetPackForward_Up2
	Requires	JUMP PRESTIGEJUMPJETPACK AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	JumpJetPackForward_Apex
	Priority	3

	Type		male
	  Anim		male\test_hop 91 91
	TEnd

	Type		huge
	  Anim		huge\hop 91 91
	TEnd

	Type		fem
	  Anim		fem\hop	91 91
	TEnd
MEnd

Move JumpJetPackForward_Apex
	Requires	APEX PRESTIGEJUMPJETPACK AIR
	Priority	3
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	JumpJetPackForward_Fall
	Scale		0.25
	Interpolate	30

	Type		male
	  Anim		male\test_hop 92 93
	TEnd

	Type		huge
	  Anim		huge\hop 92 93
	TEnd

	Type		fem
	  Anim		fem\hop	92 93
	TEnd
MEnd

Move JumpJetPackForward_Fall
	Requires	PRESTIGEJUMPJETPACK AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	JumpJetPackForward_Fall2
	Priority	2

	Type		male
	  Anim		male\test_hop 94 128
	TEnd

	Type		huge
	  Anim		huge\hop 94 109
	TEnd

	Type		fem
	  Anim		fem\hop	94 112
	TEnd
MEnd

Move JumpJetPackForward_Fall2
	Requires	PRESTIGEJUMPJETPACK AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	JumpJetPack_Post
	Priority	2

	Type		male
	  Anim		male\test_hop 128
	TEnd

	Type		huge
	  Anim		huge\hop 109
	TEnd

	Type		fem
	  Anim		fem\hop	112
	TEnd
MEnd