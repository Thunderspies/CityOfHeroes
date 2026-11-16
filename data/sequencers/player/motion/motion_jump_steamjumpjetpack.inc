#############################################################
## STEAM JUMP JET PACK
#############################################################

## JUMP	JET PACK - UP ###########################################################

Move Steam_JumpJetPack_Pre
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR
	Priority	4
	Interrupts	<PostMoveCycle>	<Ready>	<BasicMove> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Steam_JumpJetPack_Up


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

Move Steam_JumpJetPack_Up
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Steam_JumpJetPack_Apex
	Priority	4

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

Move Steam_JumpJetPack_Apex
	Requires	APEX STEAMJUMP PRESTIGEJUMPJETPACK AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Steam_JumpJetPack_Fall
	Priority	4


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

Move Steam_JumpJetPack_Fall
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_JumpJetPack_Fall2
	Priority	3

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

Move Steam_JumpJetPack_Fall2
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Steam_JumpJetPack_Post
	Priority	3

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

Move Steam_JumpJetPack_Post
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK
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

Move Steam_JumpJetPack_Damage
	Requires	HEADPAIN STEAMJUMP PRESTIGEJUMPJETPACK
	Interrupts	Steam_JumpJetPack_Post
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ>
	NextMove	postReady_back
	Priority	3
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

Move Steam_JumpJetPackHopFromJump
	Requires	FORWARD	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR
	Interrupts	JumpToHop Steam_JumpJetPack_Pre Steam_JumpJetPack_Up <PostMoveCycle> <Ready> <BasicMove> <SkillMove> RunPre <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_JumpJetPackForward_Up2
	Priority	15  #   +10 priorites for hop so Prestige jump doesnt interrupt
	Interpolate	15

	Type		male
	  Anim		male\steamjumppack_hop 10 91


	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop 10 91


	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop 10 91


	TEnd
MEnd


## JUMP	JET PACK - FORWARD ###########################################################

Move Steam_JumpJetPackForward_Pre
	Requires	FORWARD	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR
	Interrupts	<PostMoveCycle>	<Ready>	<BasicMove> <SkillMove>	RunPre <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_JumpJetPackForward_Up
	Priority	15  #   +10 priorites for hop so Prestige jump doesnt interrupt

	Type		male
	  Anim		male\steamjumppack_hop 10 11

	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop 10 11

	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop 10 11

	TEnd
MEnd

Move Steam_JumpJetPackForward_Up
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_JumpJetPackForward_Up2
	Priority	14  #   +10 priorites for hop so Prestige jump doesnt interrupt

	Type		male
	  Anim		male\steamjumppack_hop 12 91
	  PlayFx	FEET\JUMP1.FX:JUMP

	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop 12 91
	  PlayFx	FEET\HUGEJUMP2.FX:JUMP

	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop 12 91
	  PlayFx	FEET\FEMJUMP1.FX:JUMP

	TEnd

	Type		sprocket
	  Anim		male\steamjumppack_hop 12 91

	TEnd

	Type		RockSuit
	  Anim		huge\hop 12 23

	TEnd
MEnd

Move Steam_JumpJetPackForward_Up2
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Steam_JumpJetPackForward_Apex
	Priority	14  #   +10 priorites for hop so Prestige jump doesnt interrupt

	Type		male
	  Anim		male\steamjumppack_hop 91 91

	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop 91 91

	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop 91 91

	TEnd
MEnd

Move Steam_JumpJetPackForward_Apex
	Requires	APEX STEAMJUMP PRESTIGEJUMPJETPACK AIR
	Priority	14  #   +10 priorites for hop so Prestige jump doesnt interrupt
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Steam_JumpJetPackForward_Fall
	Scale		0.25
	Interpolate	30

	Type		male
	  Anim		male\steamjumppack_hop 92 93
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop 92 93
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop 92 93
	TEnd
MEnd

Move Steam_JumpJetPackForward_Fall
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_JumpJetPackForward_Fall2
	Priority	13  #   +10 priorites for hop so Prestige jump doesnt interrupt

	Type		male
	  Anim		male\steamjumppack_hop 94 128
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop 94 128
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop 94 128
	TEnd
MEnd

Move Steam_JumpJetPackForward_Fall2
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR
	Priority	13  #   +10 priorites for hop so Prestige jump doesnt interrupt
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Steam_JumpJetPack_Post


	Type		male
	  Anim		male\steamjumppack_hop 128
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop 128
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop 128
	TEnd
MEnd


###########################################################
### Shield Steam jump jetpack
###########################################################

## JUMP	JET PACK - UP ###########################################################

Move Steam_Shield_JumpJetPack_Pre
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR SHIELD
	Priority	15
	Interrupts	<MoveIRQ> <IRQJumps> <TravelShieldMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelShieldJump>
	NextMove	Steam_Shield_JumpJetPack_Up


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

Move Steam_Shield_JumpJetPack_Up
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR SHIELD
	Priority	15
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelShieldJump>
	Flags		Cycle
	NextMove	Steam_Shield_JumpJetPack_Apex


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

Move Steam_Shield_JumpJetPack_Apex
	Requires	APEX STEAMJUMP PRESTIGEJUMPJETPACK AIR SHIELD
	Priority	15
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Steam_Shield_JumpJetPack_Fall


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

Move Steam_Shield_JumpJetPack_Fall
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR SHIELD
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_Shield_JumpJetPack_Fall2
	Priority	14

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

Move Steam_Shield_JumpJetPack_Fall2
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR SHIELD
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Steam_Shield_JumpJetPack_Post
	Priority	14

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

Move Steam_Shield_JumpJetPack_Post
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK SHIELD
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesALL>
	NextMove	postReady_back
	Priority	13
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

Move Steam_Shield_JumpJetPack_Damage
	Requires	HEADPAIN STEAMJUMP PRESTIGEJUMPJETPACK SHIELD
	Interrupts	Steam_Shield_JumpJetPack_Post
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <TravelShieldJump>
	NextMove	postReady_back

	Priority	14
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

Move Steam_Shield_JumpJetPackHopFromJump
	Requires	FORWARD	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR SHIELD
	Interrupts	<TravelShieldJump>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_Shield_JumpJetPackForward_Up2
	Priority	26  #   +10 priorites for hop so Prestige jump doesnt interrupt
	Interpolate	15

	Type		male
	  Anim		male\steamjumppack_hop 10 91


	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop 10 91


	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop 10 91


	TEnd
MEnd


## JUMP	JET PACK - FORWARD ###########################################################

Move Steam_Shield_JumpJetPackForward_Pre
	Requires	FORWARD	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR SHIELD
	Interrupts	Shield_Hop_Pre <MoveIRQ> <IRQJumps> <TravelShieldMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_Shield_JumpJetPackForward_Up
	Priority	26  #   +10 priorites for hop so Prestige jump doesnt interrupt

	Type		male
	  Anim		male\steamjumppack_hop_shield 10 11

	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_shield 10 11

	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_shield 10 11

	TEnd
MEnd

Move Steam_Shield_JumpJetPackForward_Up
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR SHIELD
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_Shield_JumpJetPackForward_Up2
	Priority	25  #   +10 priorites for hop so Prestige jump doesnt interrupt

	Type		male
	  Anim		male\steamjumppack_hop_shield 12 91
	  PlayFx	FEET\JUMP1.FX:JUMP

	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_shield 12 91
	  PlayFx	FEET\HUGEJUMP2.FX:JUMP

	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_shield 12 91
	  PlayFx	FEET\FEMJUMP1.FX:JUMP

	TEnd

	Type		sprocket
	  Anim		male\steamjumppack_hop_shield 12 91

	TEnd

	Type		RockSuit
	  Anim		huge\hop 12 23

	TEnd
MEnd

Move Steam_Shield_JumpJetPackForward_Up2
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR SHIELD
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Steam_Shield_JumpJetPackForward_Apex
	Priority	25  #   +10 priorites for hop so Prestige jump doesnt interrupt

	Type		male
	  Anim		male\steamjumppack_hop_shield 91 91

	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_shield 91 91

	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_shield 91 91

	TEnd
MEnd

Move Steam_Shield_JumpJetPackForward_Apex
	Requires	APEX STEAMJUMP PRESTIGEJUMPJETPACK AIR SHIELD
	Priority	24  #   +10 priorites for hop so Prestige jump doesnt interrupt
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	NextMove	Steam_Shield_JumpJetPackForward_Fall
	Scale		0.25
	Interpolate	30

	Type		male
	  Anim		male\steamjumppack_hop_shield 92 93
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_shield 92 93
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_shield 92 93
	TEnd
MEnd

Move Steam_Shield_JumpJetPackForward_Fall
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR SHIELD
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_Shield_JumpJetPackForward_Fall2
	Priority	24  #   +10 priorites for hop so Prestige jump doesnt interrupt

	Type		male
	  Anim		male\steamjumppack_hop_shield 94 128
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_shield 94 128
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_shield 94 128
	TEnd
MEnd

Move Steam_Shield_JumpJetPackForward_Fall2
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR SHIELD
	Priority	23  #   +10 priorites for hop so Prestige jump doesnt interrupt
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Steam_Shield_JumpJetPack_Post


	Type		male
	  Anim		male\steamjumppack_hop_shield 128
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_shield 128
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_shield 128
	TEnd
MEnd

###########################################################
### Shield w/ Weapon Steam jump jetpack
###########################################################

## JUMP	JET PACK - UP ###########################################################

Move Steam_ShieldWEP_JumpJetPack_Pre
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR SHIELD COMBAT WEAPON
	Priority	17
	Interrupts	<MoveIRQ> <IRQJumps> <TravelShieldWepMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelShieldWEPJump>
	NextMove	Steam_ShieldWEP_JumpJetPack_Up

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

Move Steam_ShieldWEP_JumpJetPack_Up
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR SHIELD COMBAT WEAPON
	Priority	17
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelShieldWEPJump>
	Flags		Cycle
	NextMove	Steam_ShieldWEP_JumpJetPack_Apex


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

Move Steam_ShieldWEP_JumpJetPack_Apex
	Requires	APEX STEAMJUMP PRESTIGEJUMPJETPACK AIR SHIELD COMBAT WEAPON
	Priority	17
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Steam_ShieldWEP_JumpJetPack_Fall

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

Move Steam_ShieldWEP_JumpJetPack_Fall
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ> <ShieldFall>
	Flags		ReqInputs
	NextMove	Steam_ShieldWEP_JumpJetPack_Fall2
	Priority	16

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

Move Steam_ShieldWEP_JumpJetPack_Fall2
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ> <ShieldFall>
	Flags		Cycle
	NextMove	Steam_ShieldWEP_JumpJetPack_Post
	Priority	16

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

Move Steam_ShieldWEP_JumpJetPack_Post
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK SHIELD COMBAT WEAPON
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesALL>
	NextMove	postReady_back
	Priority	15
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

Move Steam_ShieldWEP_JumpJetPack_Damage
	Requires	HEADPAIN STEAMJUMP PRESTIGEJUMPJETPACK SHIELD COMBAT WEAPON
	Interrupts	Steam_ShieldWEP_JumpJetPack_Post
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <TravelShieldWEPJump>
	NextMove	postReady_back
	Priority	16
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

Move Steam_ShieldWEP_JumpJetPackJUMPTOHOP
	Requires	FORWARD	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR SHIELD COMBAT WEAPON
	Interrupts	<TravelShieldWEPJump>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_ShieldWEP_JumpJetPackForward_Up2
	Priority	29  #   +10 priorites for hop so Prestige jump doesnt interrupt
	Interpolate	15

	Type		male
	  Anim		male\steamjumppack_hop_shield_weapon 10 91


	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_shield_weapon 10 91


	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_shield_weapon 10 91


	TEnd
MEnd


## JUMP	JET PACK - FORWARD ###########################################################

Move Steam_ShieldWEP_JumpJetPackForward_Pre
	Requires	FORWARD	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR SHIELD COMBAT WEAPON
	Interrupts	Shield_Weapon_Hop_Pre <MoveIRQ> <IRQJumps> <TravelShieldWepMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_ShieldWEP_JumpJetPackForward_Up
	Priority	29  #   +10 priorites for hop so Prestige jump doesnt interrupt

	Type		male
	  Anim		male\steamjumppack_hop_shield_weapon 10 11

	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_shield_weapon 10 11

	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_shield_weapon 10 11

	TEnd
MEnd

Move Steam_ShieldWEP_JumpJetPackForward_Up
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_ShieldWEP_JumpJetPackForward_Up2
	Priority	28  #   +10 priorites for hop so Prestige jump doesnt interrupt

	Type		male
	  Anim		male\steamjumppack_hop_shield_weapon 12 91
	  PlayFx	FEET\JUMP1.FX:JUMP

	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_shield_weapon 12 91
	  PlayFx	FEET\HUGEJUMP2.FX:JUMP

	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_shield_weapon 12 91
	  PlayFx	FEET\FEMJUMP1.FX:JUMP

	TEnd

	Type		sprocket
	  Anim		male\steamjumppack_hop_shield_weapon 12 91

	TEnd

	Type		RockSuit
	  Anim		huge\hop 12 23

	TEnd
MEnd

Move Steam_ShieldWEP_JumpJetPackForward_Up2
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Steam_ShieldWEP_JumpJetPackForward_Apex
	Priority	28  #   +10 priorites for hop so Prestige jump doesnt interrupt

	Type		male
	  Anim		male\steamjumppack_hop_shield_weapon 91 91

	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_shield_weapon 91 91

	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_shield_weapon 91 91

	TEnd
MEnd

Move Steam_ShieldWEP_JumpJetPackForward_Apex
	Requires	APEX STEAMJUMP PRESTIGEJUMPJETPACK AIR SHIELD COMBAT WEAPON
	Priority	28  #   +10 priorites for hop so Prestige jump doesnt interrupt
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	NextMove	Steam_ShieldWEP_JumpJetPackForward_Fall
	Scale		0.25
	Interpolate	30

	Type		male
	  Anim		male\steamjumppack_hop_shield_weapon 92 93
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_shield_weapon 92 93
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_shield_weapon 92 93
	TEnd
MEnd

Move Steam_ShieldWEP_JumpJetPackForward_Fall
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_ShieldWEP_JumpJetPackForward_Fall2
	Priority	27  #   +10 priorites for hop so Prestige jump doesnt interrupt

	Type		male
	  Anim		male\steamjumppack_hop_shield_weapon 94 128
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_shield_weapon 94 128
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_shield_weapon 94 128
	TEnd
MEnd

Move Steam_ShieldWEP_JumpJetPackForward_Fall2
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR SHIELD COMBAT WEAPON
	Priority	27  #   +10 priorites for hop so Prestige jump doesnt interrupt
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Steam_ShieldWEP_JumpJetPack_Post


	Type		male
	  Anim		male\steamjumppack_hop_shield_weapon 128
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_shield_weapon 128
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_shield_weapon 128
	TEnd
MEnd



###########################################################
### Combat Weapon Dual Steam Jump Jetpack
###########################################################

## JUMP	JET PACK - UP ###########################################################

Move Steam_DualWield_JumpJetPack_Pre
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON DUAL
	Priority	17
	Interrupts	<MoveIRQ> <IRQJumps> <TravelDualHandMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelDualWEPJump>
	NextMove	Steam_DualWield_JumpJetPack_Up

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

Move Steam_DualWield_JumpJetPack_Up
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON
	Priority	17
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelDualWEPJump>
	Flags		Cycle
	NextMove	Steam_DualWield_JumpJetPack_Apex


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

Move Steam_DualWield_JumpJetPack_Apex
	Requires	APEX STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON
	Priority	17
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Steam_DualWield_JumpJetPack_Fall

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

Move Steam_DualWield_JumpJetPack_Fall
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_DualWield_JumpJetPack_Fall2
	Priority	16

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

Move Steam_DualWield_JumpJetPack_Fall2
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Steam_DualWield_JumpJetPack_Post
	Priority	16

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

Move Steam_DualWield_JumpJetPack_Post
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK DUAL COMBAT WEAPON
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesALL>
	NextMove	postReady_back
	Priority	15
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

Move Steam_DualWield_JumpJetPack_Damage
	Requires	HEADPAIN STEAMJUMP PRESTIGEJUMPJETPACK DUAL COMBAT WEAPON
	Interrupts	Steam_DualWield_JumpJetPack_Post
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <TravelDualWEPJump>
	NextMove	postReady_back
	Priority	16
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

Move Steam_DualWield_JumpJetPackJumpToHop
	Requires	FORWARD	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON
	Interrupts	<TravelDualWEPJump>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_DualWield_JumpJetPackForward_Up2
	Priority	29   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep
	Interpolate	15

	Type		male
	  Anim		male\steamjumppack_hop_dual_wield 10 91


	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_dual_wield 10 91


	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_dual_wield 10 91


	TEnd
MEnd


## JUMP	JET PACK - FORWARD ###########################################################

Move Steam_DualWield_JumpJetPackForward_Pre
	Requires	FORWARD	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON
	Interrupts	DualWield_HopPre <MoveIRQ> <IRQJumps> <TravelDualHandMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_DualWield_JumpJetPackForward_Up
	Priority	29   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep

	Type		male
	  Anim		male\steamjumppack_hop_dual_wield 10 11

	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_dual_wield 10 11

	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_dual_wield 10 11

	TEnd
MEnd

Move Steam_DualWield_JumpJetPackForward_Up
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_DualWield_JumpJetPackForward_Up2
	Priority	28   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep

	Type		male
	  Anim		male\steamjumppack_hop_dual_wield 12 91
	  PlayFx	FEET\JUMP1.FX:JUMP

	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_dual_wield 12 91
	  PlayFx	FEET\HUGEJUMP2.FX:JUMP

	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_dual_wield 12 91
	  PlayFx	FEET\FEMJUMP1.FX:JUMP

	TEnd

	Type		sprocket
	  Anim		male\steamjumppack_hop_dual_wield 12 91

	TEnd

	Type		RockSuit
	  Anim		huge\hop 12 23

	TEnd
MEnd

Move Steam_DualWield_JumpJetPackForward_Up2
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Steam_DualWield_JumpJetPackForward_Apex
	Priority	28   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep

	Type		male
	  Anim		male\steamjumppack_hop_dual_wield 91 91

	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_dual_wield 91 91

	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_dual_wield 91 91

	TEnd
MEnd

Move Steam_DualWield_JumpJetPackForward_Apex
	Requires	APEX STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON
	Priority	28   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	NextMove	Steam_DualWield_JumpJetPackForward_Fall
	Scale		0.25
	Interpolate	30

	Type		male
	  Anim		male\steamjumppack_hop_dual_wield 92 93
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_dual_wield 92 93
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_dual_wield 92 93
	TEnd
MEnd

Move Steam_DualWield_JumpJetPackForward_Fall
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_DualWield_JumpJetPackForward_Fall2
	Priority	27   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep

	Type		male
	  Anim		male\steamjumppack_hop_dual_wield 94 128
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_dual_wield 94 128
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_dual_wield 94 128
	TEnd
MEnd

Move Steam_DualWield_JumpJetPackForward_Fall2
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON
	Priority	27   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Steam_DualWield_JumpJetPack_Post


	Type		male
	  Anim		male\steamjumppack_hop_dual_wield 128
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_dual_wield 128
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_dual_wield 128
	TEnd
MEnd


###########################################################
### Combat Weapon Dual Claws Steam Jump Jetpack
###########################################################

## JUMP	JET PACK - UP ###########################################################

Move Steam_DualWieldClaws_JumpJetPack_Pre
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON DUAL CLAW
	Priority	18
	Interrupts	<MoveIRQ> <IRQJumps> <TravelDualHandMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelClawsJump>
	NextMove	Steam_DualWieldClaws_JumpJetPack_Up

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

Move Steam_DualWieldClaws_JumpJetPack_Up
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON CLAW
	Priority	18
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelClawsJump>
	Flags		Cycle
	NextMove	Steam_DualWieldClaws_JumpJetPack_Apex


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

Move Steam_DualWieldClaws_JumpJetPack_Apex
	Requires	APEX STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON CLAW
	Priority	18
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Steam_DualWieldClaws_JumpJetPack_Fall

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

Move Steam_DualWieldClaws_JumpJetPack_Fall
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON CLAW
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_DualWieldClaws_JumpJetPack_Fall2
	Priority	17

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

Move Steam_DualWieldClaws_JumpJetPack_Fall2
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON CLAW
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Steam_DualWieldClaws_JumpJetPack_Post
	Priority	16

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

Move Steam_DualWieldClaws_JumpJetPack_Post
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK DUAL COMBAT WEAPON CLAW
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesALL>
	NextMove	postReady_back
	Priority	16
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

Move Steam_DualWieldClaws_JumpJetPack_Damage
	Requires	HEADPAIN STEAMJUMP PRESTIGEJUMPJETPACK DUAL COMBAT WEAPON CLAW
	Interrupts	Steam_DualWieldClaws_JumpJetPack_Post
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <TravelClawsJump>
	NextMove	postReady_back
	Priority	17
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

Move Steam_DualWieldClaws_JumpJetPackJumpToHop
	Requires	FORWARD	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON CLAW
	Interrupts	<TravelClawsJump>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_DualWieldClaws_JumpJetPackForward_Up2
	Priority	30   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep
	Interpolate	15

	Type		male
	  Anim		male\steamjumppack_hop_dual_pistols 10 91


	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_dual_pistols 10 91


	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_dual_pistols 10 91


	TEnd
MEnd


## JUMP	JET PACK - FORWARD ###########################################################

Move Steam_DualWieldClaws_JumpJetPackForward_Pre
	Requires	FORWARD	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON CLAW
	Interrupts	Claw_HopPre <MoveIRQ> <IRQJumps> <TravelDualHandMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_DualWieldClaws_JumpJetPackForward_Up
	Priority	30   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep

	Type		male
	  Anim		male\steamjumppack_hop_dual_pistols 10 11

	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_dual_pistols 10 11

	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_dual_pistols 10 11

	TEnd
MEnd

Move Steam_DualWieldClaws_JumpJetPackForward_Up
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON CLAW
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_DualWieldClaws_JumpJetPackForward_Up2
	Priority	29   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep

	Type		male
	  Anim		male\steamjumppack_hop_dual_pistols 12 91
	  PlayFx	FEET\JUMP1.FX:JUMP

	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_dual_pistols 12 91
	  PlayFx	FEET\HUGEJUMP2.FX:JUMP

	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_dual_pistols 12 91
	  PlayFx	FEET\FEMJUMP1.FX:JUMP

	TEnd

	Type		sprocket
	  Anim		male\steamjumppack_hop_dual_pistols 12 91

	TEnd

	Type		RockSuit
	  Anim		huge\hop 12 23

	TEnd
MEnd

Move Steam_DualWieldClaws_JumpJetPackForward_Up2
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON CLAW
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Steam_DualWieldClaws_JumpJetPackForward_Apex
	Priority	29   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep

	Type		male
	  Anim		male\steamjumppack_hop_dual_pistols 91 91

	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_dual_pistols 91 91

	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_dual_pistols 91 91

	TEnd
MEnd

Move Steam_DualWieldClaws_JumpJetPackForward_Apex
	Requires	APEX STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON CLAW
	Priority	29   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	NextMove	Steam_DualWieldClaws_JumpJetPackForward_Fall
	Scale		0.25
	Interpolate	30

	Type		male
	  Anim		male\steamjumppack_hop_dual_pistols 92 93
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_dual_pistols 92 93
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_dual_pistols 92 93
	TEnd
MEnd

Move Steam_DualWieldClaws_JumpJetPackForward_Fall
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON CLAW
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_DualWieldClaws_JumpJetPackForward_Fall2
	Priority	27   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep

	Type		male
	  Anim		male\steamjumppack_hop_dual_pistols 94 128
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_dual_pistols 94 128
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_dual_pistols 94 128
	TEnd
MEnd

Move Steam_DualWieldClaws_JumpJetPackForward_Fall2
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON CLAW
	Priority	27   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Steam_DualWieldClaws_JumpJetPack_Post


	Type		male
	  Anim		male\steamjumppack_hop_dual_pistols 128
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_dual_pistols 128
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_dual_pistols 128
	TEnd
MEnd


###########################################################
### Combat Weapon Dual LeftHand Steam Jump Jetpack
###########################################################

## JUMP	JET PACK - UP ###########################################################

Move Steam_LefthandWep_JumpJetPack_Pre
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON DUAL REV
	Priority	18
	Interrupts	<MoveIRQ> <IRQJumps> <TravelLeftHandMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelLeftWepJump>
	NextMove	Steam_LefthandWep_JumpJetPack_Up

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

Move Steam_LefthandWep_JumpJetPack_Up
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON REV
	Priority	18
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelLeftWepJump>
	Flags		Cycle
	NextMove	Steam_LefthandWep_JumpJetPack_Apex


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

Move Steam_LefthandWep_JumpJetPack_Apex
	Requires	APEX STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON REV
	Priority	18
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Steam_LefthandWep_JumpJetPack_Fall

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

Move Steam_LefthandWep_JumpJetPack_Fall
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON REV
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_LefthandWep_JumpJetPack_Fall2
	Priority	17

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

Move Steam_LefthandWep_JumpJetPack_Fall2
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON REV
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Steam_LefthandWep_JumpJetPack_Post
	Priority	16

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

Move Steam_LefthandWep_JumpJetPack_Post
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK DUAL COMBAT WEAPON REV
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesALL>
	NextMove	postReady_back
	Priority	16
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

Move Steam_LefthandWep_JumpJetPack_Damage
	Requires	HEADPAIN STEAMJUMP PRESTIGEJUMPJETPACK DUAL COMBAT WEAPON REV
	Interrupts	Steam_LefthandWep_JumpJetPack_Post
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <TravelLeftWepJump>
	NextMove	postReady_back
	Priority	17
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

Move Steam_LefthandWep_JumpJetPackJumpToHop
	Requires	FORWARD	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON REV
	Interrupts	<TravelLeftWepJump>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_LefthandWep_JumpJetPackForward_Up2
	Priority	30   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep
	Interpolate	15

	Type		male
	  Anim		male\steamjumppack_hop_left_weapon 10 91
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_left_weapon 10 91
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_left_weapon 10 91
	TEnd
MEnd


## JUMP	JET PACK - FORWARD ###########################################################

Move Steam_LefthandWep_JumpJetPackForward_Pre
	Requires	FORWARD	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON REV
	Interrupts	W_jumpPre <MoveIRQ> <IRQJumps> <TravelLeftHandMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_LefthandWep_JumpJetPackForward_Up
	Priority	30   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep

	Type		male
	  Anim		male\steamjumppack_hop_left_weapon 10 11

	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_left_weapon 10 11

	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_left_weapon 10 11

	TEnd
MEnd

Move Steam_LefthandWep_JumpJetPackForward_Up
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON REV
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_LefthandWep_JumpJetPackForward_Up2
	Priority	29   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep

	Type		male
	  Anim		male\steamjumppack_hop_left_weapon 12 91
	  PlayFx	FEET\JUMP1.FX:JUMP
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_left_weapon 12 91
	  PlayFx	FEET\HUGEJUMP2.FX:JUMP
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_left_weapon 12 91
	  PlayFx	FEET\FEMJUMP1.FX:JUMP
	TEnd

	Type		sprocket
	  Anim		male\steamjumppack_hop_left_weapon 12 91
	TEnd

	Type		RockSuit
	  Anim		huge\hop 12 23
	TEnd
MEnd

Move Steam_LefthandWep_JumpJetPackForward_Up2
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON REV
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Steam_LefthandWep_JumpJetPackForward_Apex
	Priority	29   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep

	Type		male
	  Anim		male\steamjumppack_hop_left_weapon 91 91
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_left_weapon 91 91
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_left_weapon 91 91
	TEnd
MEnd

Move Steam_LefthandWep_JumpJetPackForward_Apex
	Requires	APEX STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON REV
	Priority	29   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	NextMove	Steam_LefthandWep_JumpJetPackForward_Fall
	Scale		0.25
	Interpolate	30

	Type		male
	  Anim		male\steamjumppack_hop_left_weapon 92 93
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_left_weapon 92 93
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_left_weapon 92 93
	TEnd
MEnd

Move Steam_LefthandWep_JumpJetPackForward_Fall
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON REV
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_LefthandWep_JumpJetPackForward_Fall2
	Priority	27   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep

	Type		male
	  Anim		male\steamjumppack_hop_left_weapon 94 128
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_left_weapon 94 128
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_left_weapon 94 128
	TEnd
MEnd

Move Steam_LefthandWep_JumpJetPackForward_Fall2
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON REV
	Priority	27   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Steam_LefthandWep_JumpJetPack_Post


	Type		male
	  Anim		male\steamjumppack_hop_left_weapon 128
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_left_weapon 128
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_left_weapon 128
	TEnd
MEnd


###########################################################
### Combat Weapon Steam Jump Jetpack
###########################################################

## JUMP	JET PACK - UP ###########################################################

Move Steam_DualPistols_JumpJetPack_Pre
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON DUAL PISTOLS
	Priority	18
	Interrupts	<MoveIRQ> <IRQJumps> <TravelDualHandMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelDualPistolsJump>
	NextMove	Steam_DualPistols_JumpJetPack_Up

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

Move Steam_DualPistols_JumpJetPack_Up
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON PISTOLS
	Priority	18
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelDualPistolsJump>
	Flags		Cycle
	NextMove	Steam_DualPistols_JumpJetPack_Apex


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

Move Steam_DualPistols_JumpJetPack_Apex
	Requires	APEX STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON PISTOLS
	Priority	18
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Steam_DualPistols_JumpJetPack_Fall

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

Move Steam_DualPistols_JumpJetPack_Fall
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON PISTOLS
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_DualPistols_JumpJetPack_Fall2
	Priority	17

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

Move Steam_DualPistols_JumpJetPack_Fall2
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON PISTOLS
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Steam_DualPistols_JumpJetPack_Post
	Priority	16

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

Move Steam_DualPistols_JumpJetPack_Post
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK DUAL COMBAT WEAPON PISTOLS
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesALL>
	NextMove	postReady_back
	Priority	16
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

Move Steam_DualPistols_JumpJetPack_Damage
	Requires	HEADPAIN STEAMJUMP PRESTIGEJUMPJETPACK DUAL COMBAT WEAPON PISTOLS
	Interrupts	Steam_DualPistols_JumpJetPack_Post
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <TravelDualPistolsJump>
	NextMove	postReady_back
	Priority	17
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

Move Steam_DualPistols_JumpJetPackJumpToHop
	Requires	FORWARD	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON PISTOLS
	Interrupts	<TravelDualPistolsJump>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_DualPistols_JumpJetPackForward_Up2
	Priority	30   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep
	Interpolate	15

	Type		male
	  Anim		male\steamjumppack_hop_dual_pistols 10 91
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_dual_pistols 10 91
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_dual_pistols 10 91
	TEnd
MEnd


## JUMP	JET PACK - FORWARD ###########################################################

Move Steam_DualPistols_JumpJetPackForward_Pre
	Requires	FORWARD	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON PISTOLS
	Interrupts	<MoveIRQ> <IRQJumps> <TravelDualHandMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_DualPistols_JumpJetPackForward_Up
	Priority	30   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep

	Type		male
	  Anim		male\steamjumppack_hop_dual_pistols 10 11

	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_dual_pistols 10 11

	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_dual_pistols 10 11

	TEnd
MEnd

Move Steam_DualPistols_JumpJetPackForward_Up
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON PISTOLS
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_DualPistols_JumpJetPackForward_Up2
	Priority	29   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep

	Type		male
	  Anim		male\steamjumppack_hop_dual_pistols 12 91
	  PlayFx	FEET\JUMP1.FX:JUMP
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_dual_pistols 12 91
	  PlayFx	FEET\HUGEJUMP2.FX:JUMP
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_dual_pistols 12 91
	  PlayFx	FEET\FEMJUMP1.FX:JUMP
	TEnd

	Type		sprocket
	  Anim		male\steamjumppack_hop_dual_pistols 12 91
	TEnd

	Type		RockSuit
	  Anim		huge\hop 12 23
	TEnd
MEnd

Move Steam_DualPistols_JumpJetPackForward_Up2
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON PISTOLS
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Steam_DualPistols_JumpJetPackForward_Apex
	Priority	29   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep

	Type		male
	  Anim		male\steamjumppack_hop_dual_pistols 91 91
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_dual_pistols 91 91
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_dual_pistols 91 91
	TEnd
MEnd

Move Steam_DualPistols_JumpJetPackForward_Apex
	Requires	APEX STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON PISTOLS
	Priority	29   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	NextMove	Steam_DualPistols_JumpJetPackForward_Fall
	Scale		0.25
	Interpolate	30

	Type		male
	  Anim		male\steamjumppack_hop_dual_pistols 92 93
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_dual_pistols 92 93
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_dual_pistols 92 93
	TEnd
MEnd

Move Steam_DualPistols_JumpJetPackForward_Fall
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON PISTOLS
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_DualPistols_JumpJetPackForward_Fall2
	Priority	27   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep

	Type		male
	  Anim		male\steamjumppack_hop_dual_pistols 94 128
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_dual_pistols 94 128
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_dual_pistols 94 128
	TEnd
MEnd

Move Steam_DualPistols_JumpJetPackForward_Fall2
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR DUAL COMBAT WEAPON PISTOLS
	Priority	27   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Steam_DualPistols_JumpJetPack_Post


	Type		male
	  Anim		male\steamjumppack_hop_dual_pistols 128
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_dual_pistols 128
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_dual_pistols 128
	TEnd
MEnd


###########################################################
### Combat Weapon Steam Jump Jetpack
###########################################################

## JUMP	JET PACK - UP ###########################################################

Move Steam_RightWepCombat_JumpJetPack_Pre
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON
	Priority	16
	Interrupts	<IRQJumps> <MoveIRQ> <Ready> <GroundMoveALL> <Skill> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelRightWEPJump>
	NextMove	Steam_RightWepCombat_JumpJetPack_Up

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

Move Steam_RightWepCombat_JumpJetPack_Up
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON
	Priority	16
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelRightWEPJump>
	Flags		Cycle
	NextMove	Steam_RightWepCombat_JumpJetPack_Apex


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

Move Steam_RightWepCombat_JumpJetPack_Apex
	Requires	APEX STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON
	Priority	16
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Steam_RightWepCombat_JumpJetPack_Fall

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

Move Steam_RightWepCombat_JumpJetPack_Fall
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_RightWepCombat_JumpJetPack_Fall2
	Priority	15

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

Move Steam_RightWepCombat_JumpJetPack_Fall2
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Steam_RightWepCombat_JumpJetPack_Post
	Priority	15

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

Move Steam_RightWepCombat_JumpJetPack_Post
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK COMBAT WEAPON
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesALL>
	NextMove	postReady_back
	Priority	14
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

Move Steam_RightWepCombat_JumpJetPack_Damage
	Requires	HEADPAIN STEAMJUMP PRESTIGEJUMPJETPACK COMBAT WEAPON
	Interrupts	Steam_RightWepCombat_JumpJetPack_Post
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <TravelRightWEPJump>
	NextMove	postReady_back
	Priority	17
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

Move Steam_RightWepCombat_JumpJetPackJumpToHop
	Requires	FORWARD	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON
	Interrupts	<TravelRightWEPJump>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_RightWepCombat_JumpJetPackForward_Up2
	Priority	27   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep
	Interpolate	15

	Type		male
	  Anim		male\steamjumppack_hop_right_weapon 10 91
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_right_weapon 10 91
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_right_weapon 10 91
	TEnd
MEnd


## JUMP	JET PACK - FORWARD ###########################################################

Move Steam_RightWepCombat_JumpJetPackForward_Pre
	Requires	FORWARD	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON
	Interrupts	<IRQJumps> <MoveIRQ> <Ready> <GroundMoveALL> <Skill> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelRightWEPJump>
	Flags		ReqInputs
	NextMove	Steam_RightWepCombat_JumpJetPackForward_Up
	Priority	27   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep

	Type		male
	  Anim		male\steamjumppack_hop_right_weapon 10 11

	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_right_weapon 10 11

	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_right_weapon 10 11

	TEnd
MEnd

Move Steam_RightWepCombat_JumpJetPackForward_Up
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_RightWepCombat_JumpJetPackForward_Up2
	Priority	27   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep

	Type		male
	  Anim		male\steamjumppack_hop_right_weapon 12 91
	  PlayFx	FEET\JUMP1.FX:JUMP
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_right_weapon 12 91
	  PlayFx	FEET\HUGEJUMP2.FX:JUMP
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_right_weapon 12 91
	  PlayFx	FEET\FEMJUMP1.FX:JUMP
	TEnd

	Type		sprocket
	  Anim		male\steamjumppack_hop_right_weapon 12 91
	TEnd

	Type		RockSuit
	  Anim		huge\hop 12 23
	TEnd
MEnd

Move Steam_RightWepCombat_JumpJetPackForward_Up2
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Steam_RightWepCombat_JumpJetPackForward_Apex
	Priority	29   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep

	Type		male
	  Anim		male\steamjumppack_hop_right_weapon 91 91
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_right_weapon 91 91
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_right_weapon 91 91
	TEnd
MEnd

Move Steam_RightWepCombat_JumpJetPackForward_Apex
	Requires	APEX STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON
	Priority	27   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	NextMove	Steam_RightWepCombat_JumpJetPackForward_Fall
	Scale		0.25
	Interpolate	30

	Type		male
	  Anim		male\steamjumppack_hop_right_weapon 92 93
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_right_weapon 92 93
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_right_weapon 92 93
	TEnd
MEnd

Move Steam_RightWepCombat_JumpJetPackForward_Fall
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_RightWepCombat_JumpJetPackForward_Fall2
	Priority	26   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep

	Type		male
	  Anim		male\steamjumppack_hop_right_weapon 94 128
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_right_weapon 94 128
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_right_weapon 94 128
	TEnd
MEnd

Move Steam_RightWepCombat_JumpJetPackForward_Fall2
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON
	Priority	27   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Steam_RightWepCombat_JumpJetPack_Post


	Type		male
	  Anim		male\steamjumppack_hop_right_weapon 128
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_right_weapon 128
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_right_weapon 128
	TEnd
MEnd

###########################################################
### Combat Weapon 2h large Steam Jump Jetpack
###########################################################

## JUMP	JET PACK - UP ###########################################################

Move Steam_2hlarge_JumpJetPack_Pre
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON 2HLARGE
	Priority	18
	Interrupts	<MoveIRQ> <IRQJumps> <ClubMove> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TwoHandJump>
	NextMove	Steam_2hlarge_JumpJetPack_Up

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

Move Steam_2hlarge_JumpJetPack_Up
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON 2HLARGE
	Priority	18
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TwoHandJump>
	Flags		Cycle
	NextMove	Steam_2hlarge_JumpJetPack_Apex


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

Move Steam_2hlarge_JumpJetPack_Apex
	Requires	APEX STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON 2HLARGE
	Priority	18
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Steam_2hlarge_JumpJetPack_Fall

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

Move Steam_2hlarge_JumpJetPack_Fall
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_2hlarge_JumpJetPack_Fall2
	Priority	17

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

Move Steam_2hlarge_JumpJetPack_Fall2
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Steam_2hlarge_JumpJetPack_Post
	Priority	16

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

Move Steam_2hlarge_JumpJetPack_Post
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK COMBAT WEAPON 2HLARGE
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesALL>
	NextMove	postReady_back
	Priority	16
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

Move Steam_2hlarge_JumpJetPack_Damage
	Requires	HEADPAIN STEAMJUMP PRESTIGEJUMPJETPACK COMBAT WEAPON 2HLARGE
	Interrupts	Steam_2hlarge_JumpJetPack_Post
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <TwoHandJump>
	NextMove	postReady_back
	Priority	17
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

Move Steam_2hlarge_JumpJetPackJumpToHop
	Requires	FORWARD	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON 2HLARGE
	Interrupts	<TwoHandJump>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_2hlarge_JumpJetPackForward_Up2
	Priority	30   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep
	Interpolate	15

	Type		male
	  Anim		male\steamjumppack_hop_2hlarge 10 91
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_2hlarge 10 91
	TEnd

	Type		fem
	  Anim		fem\steamjumppack_hop_2hlarge 10 91
	TEnd
MEnd


## JUMP	JET PACK - FORWARD ###########################################################

Move Steam_2hlarge_JumpJetPackForward_Pre
	Requires	FORWARD	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON 2HLARGE
	Interrupts	W_jumpPre <MoveIRQ> <IRQJumps> <TwoHandJump> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <StunMove> <jumpFall> <Block> <HitIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_2hlarge_JumpJetPackForward_Up
	Priority	30   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep

	Type		male
	  Anim		male\steamjumppack_hop_2hlarge 10 11

	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_2hlarge 10 11

	TEnd

	Type		fem
	  Anim		fem\steamjumppack_hop_2hlarge 10 11

	TEnd
MEnd

Move Steam_2hlarge_JumpJetPackForward_Up
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_2hlarge_JumpJetPackForward_Up2
	Priority	29   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep

	Type		male
	  Anim		male\steamjumppack_hop_2hlarge 12 91
	  PlayFx	FEET\JUMP1.FX:JUMP
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_2hlarge 12 91
	  PlayFx	FEET\HUGEJUMP2.FX:JUMP
	TEnd

	Type		fem
	  Anim		fem\steamjumppack_hop_2hlarge 12 91
	  PlayFx	FEET\FEMJUMP1.FX:JUMP
	TEnd

	Type		sprocket
	  Anim		male\steamjumppack_hop_2hlarge 12 91
	TEnd

	Type		RockSuit
	  Anim		huge\hop 12 23
	TEnd
MEnd

Move Steam_2hlarge_JumpJetPackForward_Up2
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Steam_2hlarge_JumpJetPackForward_Apex
	Priority	29   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep

	Type		male
	  Anim		male\steamjumppack_hop_2hlarge 91 91
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_2hlarge 91 91
	TEnd

	Type		fem
	  Anim		fem\steamjumppack_hop_2hlarge 91 91
	TEnd
MEnd

Move Steam_2hlarge_JumpJetPackForward_Apex
	Requires	APEX STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON 2HLARGE
	Priority	29   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	NextMove	Steam_2hlarge_JumpJetPackForward_Fall
	Scale		0.25
	Interpolate	30

	Type		male
	  Anim		male\steamjumppack_hop_2hlarge 92 93
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_2hlarge 92 93
	TEnd

	Type		fem
	  Anim		fem\steamjumppack_hop_2hlarge 92 93
	TEnd
MEnd

Move Steam_2hlarge_JumpJetPackForward_Fall
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_2hlarge_JumpJetPackForward_Fall2
	Priority	27   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep

	Type		male
	  Anim		male\steamjumppack_hop_2hlarge 94 128
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_2hlarge 94 128
	TEnd

	Type		fem
	  Anim		fem\steamjumppack_hop_2hlarge 94 128
	TEnd
MEnd

Move Steam_2hlarge_JumpJetPackForward_Fall2
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON 2HLARGE
	Priority	27   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Steam_2hlarge_JumpJetPack_Post


	Type		male
	  Anim		male\steamjumppack_hop_2hlarge 128
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_2hlarge 128
	TEnd

	Type		fem
	  Anim		fem\steamjumppack_hop_2hlarge 128
	TEnd
MEnd

###########################################################
### Combat Weapon Shotgun Steam Jump Jetpack
###########################################################

## JUMP	JET PACK - UP ###########################################################

Move Steam_Shotgun_JumpJetPack_Pre
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON GUN
	Priority	17
	Interrupts	<IRQJumps> <MoveIRQ> <Ready> <GroundMoveALL> <Skill> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelGunJump>
	NextMove	Steam_Shotgun_JumpJetPack_Up

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

Move Steam_Shotgun_JumpJetPack_Up
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON GUN
	Priority	17
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelGunJump>
	Flags		Cycle
	NextMove	Steam_Shotgun_JumpJetPack_Apex


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

Move Steam_Shotgun_JumpJetPack_Apex
	Requires	APEX STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON GUN
	Priority	17
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Steam_Shotgun_JumpJetPack_Fall

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

Move Steam_Shotgun_JumpJetPack_Fall
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON GUN
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_Shotgun_JumpJetPack_Fall2
	Priority	15

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

Move Steam_Shotgun_JumpJetPack_Fall2
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON GUN
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Steam_Shotgun_JumpJetPack_Post
	Priority	16

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

Move Steam_Shotgun_JumpJetPack_Post
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK COMBAT WEAPON GUN
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesALL>
	NextMove	postReady_back
	Priority	15
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

Move Steam_Shotgun_JumpJetPack_Damage
	Requires	HEADPAIN STEAMJUMP PRESTIGEJUMPJETPACK COMBAT WEAPON GUN
	Interrupts	Steam_Shotgun_JumpJetPack_Post
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <TravelGunJump>
	NextMove	postReady_back
	Priority	17
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

Move Steam_Shotgun_JumpJetPackJumpToHop
	Requires	FORWARD	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON GUN
	Interrupts	<TravelGunJump>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_Shotgun_JumpJetPackForward_Up2
	Priority	27   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep
	Interpolate	15

	Type		male
	  Anim		male\steamjumppack_hop_right_gun 10 91
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_right_gun 10 91
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_right_gun 10 91
	TEnd
MEnd


## JUMP	JET PACK - FORWARD ###########################################################

Move Steam_Shotgun_JumpJetPackForward_Pre
	Requires	FORWARD	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON GUN
	Interrupts	<IRQJumps> <MoveIRQ> <Ready> <GroundMoveALL> <Skill> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelGunJump>
	Flags		ReqInputs
	NextMove	Steam_Shotgun_JumpJetPackForward_Up
	Priority	28   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep

	Type		male
	  Anim		male\steamjumppack_hop_right_gun 10 11

	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_right_gun 10 11

	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_right_gun 10 11

	TEnd
MEnd

Move Steam_Shotgun_JumpJetPackForward_Up
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON GUN
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_Shotgun_JumpJetPackForward_Up2
	Priority	28   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep

	Type		male
	  Anim		male\steamjumppack_hop_right_gun 12 91
	  PlayFx	FEET\JUMP1.FX:JUMP
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_right_gun 12 91
	  PlayFx	FEET\HUGEJUMP2.FX:JUMP
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_right_gun 12 91
	  PlayFx	FEET\FEMJUMP1.FX:JUMP
	TEnd

	Type		sprocket
	  Anim		male\steamjumppack_hop_right_gun 12 91
	TEnd

	Type		RockSuit
	  Anim		huge\hop 12 23
	TEnd
MEnd

Move Steam_Shotgun_JumpJetPackForward_Up2
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON GUN
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Steam_Shotgun_JumpJetPackForward_Apex
	Priority	30   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep

	Type		male
	  Anim		male\steamjumppack_hop_right_gun 91 91
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_right_gun 91 91
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_right_gun 91 91
	TEnd
MEnd

Move Steam_Shotgun_JumpJetPackForward_Apex
	Requires	APEX STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON GUN
	Priority	28   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	NextMove	Steam_Shotgun_JumpJetPackForward_Fall
	Scale		0.25
	Interpolate	30

	Type		male
	  Anim		male\steamjumppack_hop_right_gun 92 93
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_right_gun 92 93
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_right_gun 92 93
	TEnd
MEnd

Move Steam_Shotgun_JumpJetPackForward_Fall
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON GUN
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_Shotgun_JumpJetPackForward_Fall2
	Priority	26   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep

	Type		male
	  Anim		male\steamjumppack_hop_right_gun 94 128
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_right_gun 94 128
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_right_gun 94 128
	TEnd
MEnd

Move Steam_Shotgun_JumpJetPackForward_Fall2
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR COMBAT WEAPON GUN
	Priority	28   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Steam_Shotgun_JumpJetPack_Post


	Type		male
	  Anim		male\steamjumppack_hop_right_gun 128
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop_right_gun 128
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop_right_gun 128
	TEnd
MEnd



###########################################################
### Ninja Steam Jump Jetpack
###########################################################

## JUMP	JET PACK - UP ###########################################################

Move Steam_Ninja_JumpJetPack_Pre
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR NINJARUN
	Priority	16
	Interrupts	<MoveIRQ> <Ready> <Combat> <IRQJumps> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelJump>
	NextMove	Steam_Ninja_JumpJetPack_Up

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

Move Steam_Ninja_JumpJetPack_Up
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR NINJARUN
	Priority	16
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelJump>
	Flags		Cycle
	NextMove	Steam_Ninja_JumpJetPack_Apex


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

Move Steam_Ninja_JumpJetPack_Apex
	Requires	APEX STEAMJUMP PRESTIGEJUMPJETPACK AIR NINJARUN
	Priority	16
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelJump>
	NextMove	Steam_Ninja_JumpJetPack_Fall

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

Move Steam_Ninja_JumpJetPack_Fall
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR NINJARUN
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ> <TravelJump>
	Flags		ReqInputs
	NextMove	Steam_Ninja_JumpJetPack_Fall2
	Priority	15

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

Move Steam_Ninja_JumpJetPack_Fall2
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR NINJARUN
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ> <TravelJump>
	Flags		Cycle
	NextMove	Steam_Ninja_JumpJetPack_Post
	Priority	16

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

Move Steam_Ninja_JumpJetPack_Post
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK NINJARUN
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesALL>
	NextMove	postReady_back
	Priority	15
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

Move Steam_Ninja_JumpJetPack_Damage
	Requires	HEADPAIN STEAMJUMP PRESTIGEJUMPJETPACK NINJARUN
	Interrupts	Steam_Ninja_JumpJetPack_Post
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <TravelJump>
	NextMove	postReady_back
	Priority	15
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

Move Steam_Ninja_JumpJetPackJumpToHop
	Requires	FORWARD	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR NINJARUN
	Interrupts	<TravelJump>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_Ninja_JumpJetPackForward_Up2
	Priority	26   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep
	Interpolate	15

	Type		male
	  Anim		male\steamjumppack_hop 10 91
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop 10 91
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop 10 91
	TEnd
MEnd


## JUMP	JET PACK - FORWARD ###########################################################

Move Steam_Ninja_JumpJetPackForward_Pre
	Requires	FORWARD	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR NINJARUN
	Interrupts	<MoveIRQ> <Ready> <Combat> <IRQJumps> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_Ninja_JumpJetPackForward_Up
	Priority	27   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep

	Type		male
	  Anim		male\steamjumppack_hop 10 11

	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop 10 11

	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop 10 11

	TEnd
MEnd

Move Steam_Ninja_JumpJetPackForward_Up
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR NINJARUN
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_Ninja_JumpJetPackForward_Up2
	Priority	26   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep

	Type		male
	  Anim		male\steamjumppack_hop 12 91
	  PlayFx	FEET\JUMP1.FX:JUMP
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop 12 91
	  PlayFx	FEET\HUGEJUMP2.FX:JUMP
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop 12 91
	  PlayFx	FEET\FEMJUMP1.FX:JUMP
	TEnd

	Type		sprocket
	  Anim		male\steamjumppack_hop 12 91
	TEnd

	Type		RockSuit
	  Anim		huge\hop 12 23
	TEnd
MEnd

Move Steam_Ninja_JumpJetPackForward_Up2
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR NINJARUN
	Priority	26   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Steam_Ninja_JumpJetPackForward_Apex

	Type		male
	  Anim		male\steamjumppack_hop 91 91
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop 91 91
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop 91 91
	TEnd
MEnd

Move Steam_Ninja_JumpJetPackForward_Apex
	Requires	APEX STEAMJUMP PRESTIGEJUMPJETPACK AIR NINJARUN
	Priority	26   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	NextMove	Steam_Ninja_JumpJetPackForward_Fall
	Scale		0.25
	Interpolate	30

	Type		male
	  Anim		male\steamjumppack_hop 92 93
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop 92 93
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop 92 93
	TEnd
MEnd

Move Steam_Ninja_JumpJetPackForward_Fall
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR NINJARUN
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_Ninja_JumpJetPackForward_Fall2
	Priority	25   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep

	Type		male
	  Anim		male\steamjumppack_hop 94 128
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop 94 128
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop 94 128
	TEnd
MEnd

Move Steam_Ninja_JumpJetPackForward_Fall2
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR NINJARUN
	Priority	25   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Steam_Ninja_JumpJetPack_Post


	Type		male
	  Anim		male\steamjumppack_hop 128
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop 128
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop 128
	TEnd
MEnd


###########################################################
### Beast Steam Jump Jetpack
###########################################################

## JUMP	JET PACK - UP ###########################################################

Move Steam_Beast_JumpJetPack_Pre
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR BeastRUN
	Priority	16
	Interrupts	<MoveIRQ> <Ready> <Combat> <IRQJumps> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelJump>
	NextMove	Steam_Beast_JumpJetPack_Up

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

Move Steam_Beast_JumpJetPack_Up
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR BeastRUN
	Priority	16
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelJump>
	Flags		Cycle
	NextMove	Steam_Beast_JumpJetPack_Apex


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

Move Steam_Beast_JumpJetPack_Apex
	Requires	APEX STEAMJUMP PRESTIGEJUMPJETPACK AIR BeastRUN
	Priority	16
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TravelJump>
	NextMove	Steam_Beast_JumpJetPack_Fall

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

Move Steam_Beast_JumpJetPack_Fall
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR BeastRUN
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ> <TravelJump>
	Flags		ReqInputs
	NextMove	Steam_Beast_JumpJetPack_Fall2
	Priority	15

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

Move Steam_Beast_JumpJetPack_Fall2
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR BeastRUN
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ> <TravelJump>
	Flags		Cycle
	NextMove	Steam_Beast_JumpJetPack_Post
	Priority	16

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

Move Steam_Beast_JumpJetPack_Post
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK BeastRUN
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesALL>
	NextMove	postReady_back
	Priority	15
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

Move Steam_Beast_JumpJetPack_Damage
	Requires	HEADPAIN STEAMJUMP PRESTIGEJUMPJETPACK BeastRUN
	Interrupts	Steam_Beast_JumpJetPack_Post
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <TravelJump>
	NextMove	postReady_back
	Priority	15
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

Move Steam_Beast_JumpJetPackJumpToHop
	Requires	FORWARD	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR BeastRUN
	Interrupts	<TravelJump>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_Beast_JumpJetPackForward_Up2
	Priority	26   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep
	Interpolate	15

	Type		male
	  Anim		male\steamjumppack_hop 10 91
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop 10 91
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop 10 91
	TEnd
MEnd


## JUMP	JET PACK - FORWARD ###########################################################

Move Steam_Beast_JumpJetPackForward_Pre
	Requires	FORWARD	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR BeastRUN
	Interrupts	<MoveIRQ> <Ready> <Combat> <IRQJumps> <RunsALL> <GroundMoveALL> <Skill> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_Beast_JumpJetPackForward_Up
	Priority	27   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep

	Type		male
	  Anim		male\steamjumppack_hop 10 11

	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop 10 11

	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop 10 11

	TEnd
MEnd

Move Steam_Beast_JumpJetPackForward_Up
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR BeastRUN
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_Beast_JumpJetPackForward_Up2
	Priority	26   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep

	Type		male
	  Anim		male\steamjumppack_hop 12 91
	  PlayFx	FEET\JUMP1.FX:JUMP
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop 12 91
	  PlayFx	FEET\HUGEJUMP2.FX:JUMP
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop 12 91
	  PlayFx	FEET\FEMJUMP1.FX:JUMP
	TEnd

	Type		sprocket
	  Anim		male\steamjumppack_hop 12 91
	TEnd

	Type		RockSuit
	  Anim		huge\hop 12 23
	TEnd
MEnd

Move Steam_Beast_JumpJetPackForward_Up2
	Requires	JUMP STEAMJUMP PRESTIGEJUMPJETPACK AIR BeastRUN
	Priority	26   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Steam_Beast_JumpJetPackForward_Apex

	Type		male
	  Anim		male\steamjumppack_hop 91 91
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop 91 91
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop 91 91
	TEnd
MEnd

Move Steam_Beast_JumpJetPackForward_Apex
	Requires	APEX STEAMJUMP PRESTIGEJUMPJETPACK AIR BeastRUN
	Priority	26   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	NextMove	Steam_Beast_JumpJetPackForward_Fall
	Scale		0.25
	Interpolate	30

	Type		male
	  Anim		male\steamjumppack_hop 92 93
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop 92 93
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop 92 93
	TEnd
MEnd

Move Steam_Beast_JumpJetPackForward_Fall
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR BeastRUN
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Steam_Beast_JumpJetPackForward_Fall2
	Priority	25   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep

	Type		male
	  Anim		male\steamjumppack_hop 94 128
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop 94 128
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop 94 128
	TEnd
MEnd

Move Steam_Beast_JumpJetPackForward_Fall2
	Requires	STEAMJUMP PRESTIGEJUMPJETPACK AIR BeastRUN
	Priority	25   #   +20 priorites for hop so Prestige jump doesnt interrupt and for shield and wep
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Steam_Beast_JumpJetPack_Post


	Type		male
	  Anim		male\steamjumppack_hop 128
	TEnd

	Type		huge
	  Anim		male\steamjumppack_hop 128
	TEnd

	Type		fem
	  Anim		male\steamjumppack_hop 128
	TEnd
MEnd