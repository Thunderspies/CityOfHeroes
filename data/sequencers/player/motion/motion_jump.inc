#############################################################
## JUMPS
#############################################################

## JUMP UP ###########################################################

Move jumpPre
	Requires	JUMP AIR
	Priority	2
	Interrupts	<MoveIRQ> <IRQJumps> <Ready> <GroundMoveALL> <Skill> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	jumpUp

	Type		male
	  Anim		male\superjump 3 6
	  PlayFx	FEET\SUPERJUMP1.FX:JUMP
	TEnd

	Type		PraetorianPolice
	  Anim		male\superjump 3 6
	  PlayFx	FEET\PPD_Left.fx
	  PlayFx	Feet\PPD_Jump_SFX.fx 1
	TEnd

	Type		ResistanceMale
	  Anim		male\superjump 3 6
	  PlayFx	FEET\RES_Left.fx
	  PlayFx	Feet\PPD_Jump_SFX.fx 1
	TEnd

	Type		ResistanceFem
	  Anim		male\superjump 3 6
	  PlayFx	FEET\RES_Left.fx
	  PlayFx	Feet\PPD_Jump_SFX.fx 1
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

	Type		InvisibleMaleTarget_Silent
	  Anim		male\superjump 3 6
	  #PlayFx	FEET\SUPERJUMP1.FX:JUMP
	TEnd

	Type		P_Apparitions_Minion
	  Anim		fem\superjump 3	6
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim		fem\superjump 3	6
	TEnd

	Type		P_Apparitions_Minion_Merged
	  Anim		fem\superjump 3	6
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim		fem\superjump 3	6
	TEnd

MEnd

Move jumpUp
	Requires	JUMP AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	jumpApex

	Type		male
	  Anim		male\superjump 6 65
	TEnd

	Type		huge
	  Anim		huge\superjump 6 65
	TEnd

	Type		fem
	  Anim		fem\superjump 6	65
	TEnd
MEnd

Move jumpApex
	Requires	APEX AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	jumpFall

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

Move jumpFall
	Requires	AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	jumpFall2

	Type		male
	  Anim		male\superjump 66 93
	TEnd

	Type		huge
	  Anim		huge\superjump 66 93
	TEnd

	Type		fem
	  Anim		fem\superjump 66 93
	TEnd
MEnd

Move jumpFall2
	Requires	AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
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

## JUMP POST ###########################################################

Move JumpPost
	Include sequencers\Members\readies.inc
	NextMove	postReady_back
	Interpolate	1
	Scale		0.7
	SticksOnChild	LAND

	Type		male
	  Anim		male\comb_postjump 1
	  PlayFx	FEET\LAND1.FX:LAND
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 2
	TEnd

	Type		PraetorianPolice
	  Anim		male\comb_postjump 1
	  PlayFx	FEET\PPD_Left.fx
	  PlayFx	FEET\PPD_Land2.fx 2
	TEnd

	Type		ResistanceMale
	  Anim		male\comb_postjump 1
	  PlayFx	FEET\RES_Left.fx
	  PlayFx	FEET\RES_Land2.fx 2
	TEnd

	Type		ResistanceFem
	  Anim		male\comb_postjump 1
	  PlayFx	FEET\RES_Left.fx
	  PlayFx	FEET\RES_Land2.fx 2
	TEnd

	Type		CameraShaker
	  Anim		male\comb_postjump 1
	  PlayFx	FEET\LAND1.FX:LAND
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 2
	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsRIGHT.FX:FOOTFALLRIGHT
	TEnd

	Type		huge
	  Anim		huge\combat_postjump 1
	  PlayFx	FEET\HUGELAND2.FX:LAND
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT 2
	TEnd

	Type		fem
	  Anim		fem\combat_postjump 1
	  PlayFx	FEET\LAND2.FX:LAND
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 2
	TEnd

	Type		RockSuit
	  Anim		male\comb_postjump 1
	  PlayFx	FEET\giantfoot1.fx:FOOTFALLLEFT:FOOTFALLLEFT  2
	  PlayFx	FEET\giantfoot2.fx:FOOTFALLRIGHT:FOOTFALLRIGHT	3
	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsRIGHT.FX:FOOTFALLRIGHT
	  Scale		1
	TEnd

	Type		InvisibleMaleTarget_Silent
	  Anim		male\comb_postjump 1
	  #PlayFx	FEET\LAND1.FX:LAND
	  #PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 2
	TEnd

	Type		P_Apparitions_Minion
	  Anim		fem\combat_postjump 1
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim		fem\combat_postjump 1
	TEnd

	Type		P_Apparitions_Minion_Merged
	  Anim		fem\combat_postjump 1
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim		fem\combat_postjump 1
	TEnd

MEnd

### JUMP POST DAMAGE ###########################################################
#
#Move JumpDamage
#	Requires	HEADPAIN
#	Interrupts	JumpPost
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <IRQEmotesALL>
#	NextMove	postReady_back
#	Interpolate	1
#
#	Type		male
#	  Anim		male\fall_dammage 1
#	  PlayFx	FEET\LAND1.FX:LAND
#	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 1
#	TEnd
#
#	Type		CameraShaker
#	  Anim		male\fall_dammage 1
#	  PlayFx	FEET\LAND1.FX:LAND
#	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 1
#	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsRIGHT.FX:FOOTFALLRIGHT
#	TEnd
#
#	Type		huge
#	  Anim		huge\thumbsup 1
#	TEnd
#
#	Type		fem
#	  Anim		fem\thumbsup 1
#	TEnd
#MEnd

## JUMP UP INTO FORWARD TRANSITION ###########################################################

Move JumpToHop
	Requires	FORWARD	JUMP AIR
	Priority	3
	Interrupts	jumppre	jumpup
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Hop2
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

## JUMP FORWARD ###########################################################

Move HopPre
	Requires	FORWARD	JUMP AIR
	Priority	3
	Interrupts	<IRQJumps> <PostMoveCycle> <Ready> <BasicMove> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Hop

	Type		male
	  Anim		male\test_hop 10 11
	  PlayFx	FEET\LAND3.FX
	TEnd

	Type		PraetorianPolice
	  Anim		male\test_hop 10 11
	  PlayFx	Feet\PPD_Jump_SFX.fx
	TEnd

	Type		ResistanceMale
	  Anim		male\test_hop 10 11
	  PlayFx	Feet\PPD_Jump_SFX.fx
	TEnd

	Type		ResistanceFem
	  Anim		male\test_hop 10 11
	  PlayFx	Feet\PPD_Jump_SFX.fx
	TEnd

	Type		huge
	  Anim		huge\hop 10 11
	  PlayFx	FEET\HUGELAND1.FX
	TEnd

	Type		fem
	  Anim		fem\hop	10 11
	  PlayFx	FEET\LAND4.FX
	TEnd

	Type		InvisibleMaleTarget_Silent
	  Anim		male\test_hop 10 11
	 # PlayFx	FEET\LAND3.FX
	TEnd

	Type		P_Apparitions_Minion
	  Anim		fem\hop	10 11
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim		fem\hop	10 11
	TEnd

	Type		P_Apparitions_Minion_Merged
	  Anim		fem\hop	10 11
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim		fem\hop	10 11
	TEnd

MEnd

Move Hop
	Requires	JUMP AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Hop2
	Scale		0.75

	Type		male
	  Anim		male\test_hop 11 91
	  PlayFx	FEET\JUMP1.FX:JUMP
	TEnd

	Type		huge
	  Anim		huge\hop 11 91
	  PlayFx	FEET\HUGEJUMP2.FX:JUMP
	TEnd

	Type		fem
	  Anim		fem\hop	11 91
	  PlayFx	FEET\FEMJUMP1.FX:JUMP
	TEnd

	Type		InvisibleMaleTarget_Silent
	  Anim		male\test_hop 11 91
	  #PlayFx	FEET\JUMP1.FX:JUMP
	TEnd

	Type		P_Apparitions_Minion
	  Anim		fem\hop	11 91
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim		fem\hop	11 91
	TEnd

	Type		P_Apparitions_Minion_Merged
	  Anim		fem\hop	11 91
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim		fem\hop	11 91
	TEnd

MEnd

Move Hop2
	Requires	JUMP AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	HopApex

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

Move HopApex
	Requires	APEX AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	HopFall

	Type		male
	  Anim		male\test_hop 91 93
	TEnd

	Type		huge
	  Anim		huge\hop 91 93
	TEnd

	Type		fem
	  Anim		fem\hop	91 93
	TEnd
MEnd

Move HopFall
	Requires	AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	HopFall_2

	Type		male
	  Anim		male\test_hop 93 128
	TEnd

	Type		huge
	  Anim		huge\hop 93 109
	TEnd

	Type		fem
	  Anim		fem\hop	93 112
	TEnd
MEnd

Move HopFall_2
	Requires	AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	JumpPost

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

## WINDMILL JUMP (BOUNCE) ###########################################################

Move Bounce_Pre
	Requires	BOUNCING AIR
	Priority	2
	Interrupts	<IRQJumps> <PostMoveCycle> <Ready> <BasicMove> <iceslide> <Swim> <jumpFall> <Claw> RunJumpCycleB <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Bounce_Up

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

	Type		InvisibleMaleTarget_Silent
	  Anim		male\windmill_jump 20 26
	  #PlayFx	FEET\SUPERJUMP1.FX:JUMP
	TEnd

	Type		P_Apparitions_Minion
	  Anim		male\windmill_jump 20 26
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim		male\windmill_jump 20 26
	TEnd

	Type		P_Apparitions_Minion_Merged
	  Anim		male\windmill_jump 20 26
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim		male\windmill_jump 20 26
	TEnd

MEnd

Move Bounce_Up
	Requires	AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	wind_jumpApex

	Type		male
	  Anim		male\windmill_jump 1 26
	TEnd
MEnd

Move Bounce_Apex
	Requires	APEX AIR
	Priority	2
	Interrupts	Bounce_Up
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <jumpFall>
	NextMove	Bounce_Fall
	Scale		2
	Interpolate	10

	Type		male
	  Anim		male\windmill_bigfall 1	15  ##windmill_fall 19 31
	TEnd
MEnd

Move Bounce_Fall
	Requires	AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	JumpPost
	Scale		2

	Type		male
	  Anim		male\windmill_bigfall 1
	TEnd
MEnd


#############################################################
## COMBAT MODE
#############################################################

## COMBAT JUMP UP ###########################################################

Move C_jumpPre
	Requires	JUMP COMBAT AIR
	Interrupts	<MoveIRQ> <Combat> <GroundMoveAll> <iceslide> <Swim> <jumpFall> <Block> <HitIRQ> <StunMove>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	C_jumpUp

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

	Type		RockSuit
	  Anim		huge\superjump 3 6
	TEnd

	Type		InvisibleMaleTarget_Silent
	  Anim		male\superjump 3 6
	  PlayFx	FEET\SUPERJUMP1.FX:JUMP
	TEnd

	Type		P_Apparitions_Minion
	  Anim		fem\superjump 3	6
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim		fem\superjump 3	6
	TEnd

	Type		P_Apparitions_Minion_Merged
	  Anim		fem\superjump 3	6
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim		fem\superjump 3	6
	TEnd

MEnd

Move C_jumpUp
	Requires	JUMP COMBAT  AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	C_jumpApex

	Type		male
	  Anim		male\superjump 6 68
	TEnd

	Type		huge
	  Anim		huge\superjump 6 68
	TEnd

	Type		fem
	  Anim		fem\superjump 6	68
	TEnd
MEnd

Move C_jumpApex
	Requires	APEX COMBAT AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	C_jumpFall

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

Move C_jumpFall
	Requires	COMBAT AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	C_jumpFall2

	Type		male
	  Anim		male\superjump 66 89
	TEnd

	Type		huge
	  Anim		huge\superjump 66 93
	TEnd

	Type		fem
	  Anim		fem\superjump 66 93
	TEnd
MEnd

Move C_jumpFall2
	Requires	COMBAT AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	C_JumpPost

	Type		male
	  Anim		male\superjump 89
	TEnd

	Type		huge
	  Anim		huge\superjump 93
	TEnd

	Type		fem
	  Anim		fem\superjump 93
	TEnd
MEnd

## COMBAT JUMP POST ###########################################################

Move C_JumpPost
	Requires	COMBAT
	Include sequencers\Members\combat.inc
	Flags		ReqInputs
	NextMove	CombatReady
	Interpolate	1
	SticksONChild	LAND

	Type		male
	  Anim		male\comb_postjump 1
	  PlayFx	FEET\LAND1.FX:LAND
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 2
	  Scale		0.7
	TEnd

	Type		CameraShaker
	  Anim		male\comb_postjump 1
	  PlayFx	FEET\HUGELAND2.FX:LAND	1
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT 2
	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsRIGHT.FX:FOOTFALLRIGHT
	TEnd

	Type		huge
	  Anim		huge\combat_postjump 1
	  PlayFx	FEET\HUGELAND2.FX:LAND	1
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT 2
	TEnd

	Type		fem
	  Anim		fem\combat_postjump 1
	  PlayFx	FEET\LAND2.FX:LAND
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 2
	  Scale		0.6
	TEnd

	Type		RockSuit
	  Anim		male\comb_postjump 1
	  PlayFx	FEET\giantfoot1.fx:FOOTFALLLEFT	 2
	  PlayFx	FEET\giantfoot2.fx:FOOTFALLRIGHT  3
	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsRIGHT.FX:FOOTFALLRIGHT
	TEnd

	Type		InvisibleMaleTarget_Silent
	  Anim		male\comb_postjump 1
	  #PlayFx	FEET\LAND1.FX:LAND
	  #PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 2
	  Scale		0.7
	TEnd

	Type		P_Apparitions_Minion
	  Anim		fem\combat_postjump 1
	 # PlayFx	FEET\LAND2.FX:LAND
	#  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 2
	  Scale		0.6
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim		fem\combat_postjump 1
	 # PlayFx	FEET\LAND2.FX:LAND
	#  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 2
	  Scale		0.6
	TEnd

	Type		P_Apparitions_Minion_Merged
	  Anim		fem\combat_postjump 1
	 # PlayFx	FEET\LAND2.FX:LAND
	#  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 2
	  Scale		0.6
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim		fem\combat_postjump 1
	 # PlayFx	FEET\LAND2.FX:LAND
	#  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 2
	  Scale		0.6
	TEnd


MEnd

## COMBAT JUMP FORWARD ###########################################################

Move C_JumpToHop
	Requires	FORWARD	JUMP COMBAT AIR
	Priority	4
	Interrupts	C_jumpPre C_jumpUp
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	C_Hop
	Interpolate	15

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

## COMBAT JUMP FORWARD ###########################################################

Move C_HopPre
	Requires	FORWARD	JUMP COMBAT AIR
	Priority	4
	Interrupts	<PostMoveCycle>	<Combat> <CombatMove> <iceslide> <Swim>	<CombatAttack> <jumpFall> <Block> <HitIRQ> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	C_Hop

	Type		male
	  Anim		male\test_hop 10 11
	  PlayFx	FEET\SUPERJUMP1.FX:JUMP
	TEnd

	Type		huge
	  Anim		huge\hop 10 11
	  PlayFx	FEET\HUGEJUMP2.FX:JUMP
	TEnd

	Type		fem
	  Anim		fem\hop	10 11
	  PlayFx	FEET\FEMJUMP1.FX:JUMP
	TEnd

	Type		InvisibleMaleTarget_Silent
	  Anim		male\test_hop 10 11
	 # PlayFx	FEET\SUPERJUMP1.FX:JUMP
	TEnd

	Type		P_Apparitions_Minion
	  Anim		fem\hop	10 11
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim		fem\hop	10 11
	TEnd

	Type		P_Apparitions_Minion_Merged
	  Anim		fem\hop	10 11
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim		fem\hop	10 11
	TEnd

MEnd

Move C_Hop
	Requires	JUMP COMBAT AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	C_Hop2

	Type		male
	  Anim		male\test_hop 11 91
	TEnd

	Type		huge
	  Anim		huge\hop 11 23
	TEnd

	Type		fem
	  Anim		fem\hop	11 23
	TEnd
MEnd

Move C_Hop2
	Requires	JUMP COMBAT AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	C_HopApex

	Type		male
	  Anim		male\test_hop 91 92
	TEnd

	Type		huge
	  Anim		huge\hop 23 92
	TEnd

	Type		fem
	  Anim		fem\hop	23 92
	TEnd
MEnd

Move C_HopApex
	Requires	APEX COMBAT AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	C_HopFall

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

Move C_HopFall
	Requires	COMBAT AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	C_HopFall_2

	Type		male
	  Anim		male\test_hop 93 128
	TEnd

	Type		huge
	  Anim		huge\hop 93 109
	TEnd

	Type		fem
	  Anim		fem\hop	93 112
	TEnd
MEnd

Move C_HopFall_2
	Requires	COMBAT AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	C_JumpPost

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


#############################################################
## WEAPON MODE
#############################################################

## WEAPON JUMP UP ###########################################################

Move W_jumpPre
	Requires	JUMP COMBAT WEAPON AIR
	Priority	4
	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> <ClubMove>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	W_jumpUp

	Type		male
	  Anim		male\wep_superjump 3 6
	  PlayFx	FEET\SUPERJUMP1.FX:JUMP
	TEnd

	Type		huge
	  Anim		huge\wep_superjump 3 6
	  PlayFx	FEET\HUGEJUMP2.FX:JUMP
	TEnd

	Type		fem
	  Anim		fem\wep_superjump 3 6
	  PlayFx	FEET\FEMJUMP1.FX:JUMP
	TEnd

	Type		RockSuit
	  Anim		huge\wep_superjump 3 6
	TEnd

	Type		InvisibleMaleTarget_Silent
	  Anim		male\wep_superjump 3 6
	  #PlayFx	FEET\SUPERJUMP1.FX:JUMP
	TEnd

	Type		P_Apparitions_Minion
	  Anim		fem\superjump 3	6
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim		fem\superjump 3	6
	TEnd

	Type		P_Apparitions_Minion_Merged
	  Anim		fem\superjump 3	6
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim		fem\superjump 3	6
	TEnd

MEnd

Move W_jumpUp
	Requires	JUMP COMBAT WEAPON AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	W_jumpApex

	Type		male
	  Anim		male\wep_superjump 6 65
	TEnd

	Type		huge
	  Anim		huge\wep_superjump 6 65
	TEnd

	Type		fem
	  Anim		fem\wep_superjump 6 65
	TEnd
MEnd

Move W_jumpApex
	Requires	APEX COMBAT WEAPON AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	W_jumpFall

	Type		male
	  Anim		male\wep_superjump 65 66
	TEnd

	Type		huge
	  Anim		huge\wep_superjump 65 66
	TEnd

	Type		fem
	  Anim		fem\wep_superjump 65 66
	TEnd
MEnd

Move W_jumpFall
	Requires	COMBAT WEAPON AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	W_jumpFall2

	Type		male
	  Anim		male\wep_superjump 66 89
	TEnd

	Type		huge
	  Anim		huge\wep_superjump 66 93
	TEnd

	Type		fem
	  Anim		fem\wep_superjump 66 93
	TEnd
MEnd

Move W_jumpFall2
	Requires	COMBAT WEAPON AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	W_JumpPost

	Type		male
	  Anim		male\wep_superjump 89
	TEnd

	Type		huge
	  Anim		huge\wep_superjump 93
	TEnd

	Type		fem
	  Anim		fem\wep_superjump 93
	TEnd
MEnd

## WEAPON JUMP POST ###########################################################

Move W_JumpPost
	Requires	COMBAT WEAPON
	Include sequencers\Members\combat_weapon.inc
	Flags		ReqInputs
	NextMove	WepReady
	Interpolate	1
	SticksOnChild	LAND

	Type		male
	  Anim		male\wep_postjump 1
	  PlayFx	FEET\LAND1.FX:LAND
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 2
	  Scale		0.7
	TEnd

	Type		huge
	  Anim		huge\wep_postjump 1
	  PlayFx	FEET\HUGELAND2.FX:LAND	1
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT 2
	TEnd

	Type		fem
	  Anim		fem\wep_postjump 1
	  PlayFx	FEET\LAND2.FX:LAND
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 2
	  Scale		0.6
	TEnd

	Type		RockSuit
	  Anim		huge\wep_postjump 1
	  PlayFx	FEET\giantfoot1.fx:FOOTFALLLEFT 2
	  PlayFx	FEET\giantfoot2.fx:FOOTFALLRIGHT 3
	  PlayFx	EnemyFx\BanishedPantheon\GiantEnemyTremorsRIGHT.FX:FOOTFALLRIGHT
	TEnd

	Type		InvisibleMaleTarget_Silent
	  Anim		male\wep_postjump 1
	  #PlayFx	FEET\LAND1.FX:LAND
	  #PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 2
	  Scale		0.7
	TEnd

	Type		P_Apparitions_Minion
	  Anim		fem\wep_postjump 1
	 # PlayFx	FEET\LAND2.FX:LAND
	 # PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 2
	  Scale		0.6
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim		fem\wep_postjump 1
	 # PlayFx	FEET\LAND2.FX:LAND
	 # PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 2
	  Scale		0.6
	TEnd

	Type		P_Apparitions_Minion_Merged
	  Anim		fem\wep_postjump 1
	 # PlayFx	FEET\LAND2.FX:LAND
	 # PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 2
	  Scale		0.6
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim		fem\wep_postjump 1
	 # PlayFx	FEET\LAND2.FX:LAND
	 # PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 2
	  Scale		0.6
	TEnd

MEnd

## WEAPON JUMP FORWARD ###########################################################

Move W_JumpToHop
	Requires	FORWARD	JUMP COMBAT WEAPON AIR
	Priority	5
	Interrupts	W_jumpPre W_jumpUp
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	W_Hop
	Interpolate	15

	Type		male
	  Anim		male\wep_hop 10	11
	TEnd

	Type		huge
	  Anim		huge\wephop 10 11
	TEnd

	Type		fem
	  Anim		fem\wep_hop 10 11
	TEnd

	Type		RockSuit
	  Anim		huge\wephop 10 11
	TEnd
MEnd

## WEAPON JUMP FORWARD ###########################################################

Move W_HopPre
	Requires	FORWARD	JUMP COMBAT WEAPON AIR
	Priority	5
	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> <ClubMove>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	W_Hop

	Type		male
	  Anim		male\wep_hop 10	11
	  PlayFx	FEET\SUPERJUMP1.FX:JUMP
	TEnd

	Type		huge
	  Anim		huge\wephop 10 11
	  PlayFx	FEET\HUGEJUMP2.FX:JUMP
	TEnd

	Type		fem
	  Anim		fem\wep_hop 10 11
	  PlayFx	FEET\FEMJUMP1.FX:JUMP
	TEnd

	Type		RockSuit
	  Anim		huge\wephop 10 11
	TEnd

	Type		InvisibleMaleTarget_Silent
	  Anim		male\wep_hop 10	11
	  #PlayFx	FEET\SUPERJUMP1.FX:JUMP
	TEnd

	Type		P_Apparitions_Minion
	  Anim		fem\wephop 10 11
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim		fem\wephop 10 11
	TEnd

	Type		P_Apparitions_Minion_Merged
	  Anim		fem\wephop 10 11
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim		fem\wephop 10 11
	TEnd

MEnd

Move W_Hop
	Requires	JUMP COMBAT WEAPON AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	W_Hop2
	Scale		0.75

	Type		male
	  Anim		male\wep_hop 11	91
	TEnd

	Type		huge
	  Anim		huge\wephop 11 91
	TEnd

	Type		fem
	  Anim		fem\wep_hop 11 91
	TEnd
MEnd

Move W_Hop2
	Requires	JUMP COMBAT WEAPON AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle #ReqInputs
	NextMove	W_HopApex

	Type		male
	  Anim		male\wep_hop 91	92
	TEnd

	Type		huge
	  Anim		huge\wephop 91 92
	TEnd

	Type		fem
	  Anim		fem\wep_hop 91 92
	TEnd
MEnd

Move W_HopApex
	Requires	APEX COMBAT WEAPON AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	W_HopFall

	Type		male
	  Anim		male\wep_hop 92	94
	TEnd

	Type		huge
	  Anim		huge\wephop 92 93
	TEnd

	Type		fem
	  Anim		fem\wep_hop 92 94
	TEnd
MEnd

Move W_HopFall
	Requires	COMBAT WEAPON AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	W_HopFall_2

	Type		male
	  Anim		male\wep_hop 94	128
	TEnd

	Type		huge
	  Anim		huge\wephop 94 109
	TEnd

	Type		fem
	  Anim		fem\wep_hop 95 112
	TEnd
MEnd

Move W_HopFall_2
	Requires	COMBAT WEAPON AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	W_JumpPost

	Type		male
	  Anim		male\wep_hop 128
	TEnd

	Type		huge
	  Anim		huge\wephop 109
	TEnd

	Type		fem
	  Anim		fem\wep_hop 112
	TEnd
MEnd

#############################################################
## SHOTGUN MODE - Uses Weapon mode as base
#############################################################

## SHOTGUN JUMP POST ###########################################################

Move SG_JumpPost
	Requires	COMBAT WEAPON GUN
	Priority	3
	Interrupts	W_JumpPost
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	Flags		ReqInputs FixArmReg
	NextMove	SG_JumpPostcycle
	Interpolate	1
	SticksOnChild	LAND

	Type		male
	  Anim		male\shotgun_jumppost 1
	  PlayFx	FEET\LAND1.FX:LAND  1
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 2
	  Scale		0.9
	TEnd

	Type		huge
	  Anim		huge\shotgun_postjump 1
	  PlayFx	FEET\HUGELAND2.FX:LAND
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT 2
	  Scale		0.7
	TEnd

	Type		fem
	  Anim		fem\rocket_postjump 1
	  PlayFx	FEET\LAND2.FX:LAND  1
  	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 2
	  Scale		0.8
	TEnd
MEnd

Move SG_JumpPostcycle
	Requires	COMBAT WEAPON GUN
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <TwoHandCombat> <AttackIRQ> <BlockIRQ> <MoveIRQ> <IRQEmotesTwoHand>
	Flags		Cycle FixArmReg
	NextMove	PostShotgun
	Scale		0.50

	Type		male
	  Anim		male\shotgun_cycle 1 31
	TEnd

	Type		huge
	  Anim		huge\shotgun_cycle 1 31
	TEnd

	Type		fem
	  Anim		fem\rocket_cycle 1 31
	TEnd
MEnd


#############################################################
## DUAL WIELD MODE
#############################################################

## DUAL WIELD JUMP ###########################################################

Move DualWield_jumpPre
	Requires	JUMP COMBAT WEAPON AIR DUAL
	Priority	5
	Interrupts	<WepCombat> <DualHandCombat> <TwoHandCombat> <GroundMoveALL> <iceslide> <Swim> <DualHandAttack> <jumpFall> <Block> <HitIRQ> <StunMove>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	DualWield_jumpUp

	Type		male
	  Anim		male\dual_jump 3 6
	  PlayFx	FEET\SUPERJUMP1.FX
	TEnd

	Type		fem
	  Anim		fem\dual_jump 3 6
	  PlayFx	FEET\FEMJUMP1.FX
	TEnd

	Type		huge
	  Anim		male\dual_jump 3 6
	  PlayFx	FEET\HUGEJUMP2.FX
	TEnd
MEnd

Move DualWield_jumpUp
	Requires	JUMP COMBAT WEAPON AIR DUAL
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	DualWield_jumpUp2
	Scale		0.75
	Flags		Cycle

	Type		male
	  Anim		male\dual_jump 6 63
	TEnd

	Type		fem
	  Anim		fem\dual_jump 6 63
	TEnd
MEnd

Move DualWield_jumpUp2
	Requires	JUMP COMBAT WEAPON AIR DUAL
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	DualWield_jumpApex

	Type		male
	  Anim		male\dual_jump 63 65
	TEnd

	Type		fem
	  Anim		fem\dual_jump 63 65
	TEnd
MEnd

Move DualWield_jumpApex
	Requires	APEX COMBAT WEAPON AIR DUAL
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	DualWield_jumpFall

	Type		male
	  Anim		male\dual_jump 65 66
	TEnd

	Type		fem
	  Anim		fem\dual_jump 65 66
	TEnd
MEnd

Move DualWield_jumpFall
	Requires	AIR COMBAT WEAPON DUAL
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	DualWield_jumpFall2

	Type		male
	  Anim		male\dual_jump 66 89
	TEnd

	Type		fem
	  Anim		fem\dual_jump 66 93
	TEnd
MEnd

Move DualWield_jumpFall2
	Requires	AIR COMBAT WEAPON DUAL
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	DualWield_JumpPost

	Type		male
	  Anim		male\dual_jump 89
	TEnd

	Type		fem
	  Anim		fem\dual_jump 93
	TEnd
MEnd

## DUAL WIELD JUMP POST ###########################################################

Move DualWield_JumpPost
	Requires	COMBAT WEAPON DUAL
	Priority	3
	Interrupts	W_JumpPost
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	DualWield_Ready
	Interpolate	1
	SticksOnChild	LAND
	Scale		1.5

	Type		male
	  Anim		male\dual_post_jump 1
	  PlayFx	FEET\LAND1.FX
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 2
	  Scale		1.2
	TEnd
MEnd

## DUAL WIELD JUMP TO HOP ###########################################################

Move DualWield_JumpToHop
	Requires	FORWARD	JUMP COMBAT WEAPON AIR DUAL
	Priority	6
	Interrupts	DualWield_jumpPre DualWield_jumpUp
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	DualWield_Hop
	Interpolate	15

	Type		male
	  Anim		male\dual_hop 10 11
	TEnd

	Type		huge
	  Anim		male\dual_hop 10 11
	TEnd

	Type		fem
	  Anim		fem\dual_hop 10	11
	TEnd
MEnd

## DUAL WIELD JUMP FORWARD (HOP) ###########################################################

Move DualWield_HopPre
	Requires	FORWARD	JUMP COMBAT WEAPON AIR DUAL
	Priority	6
	Interrupts	<WepCombat> <DualHandCombat> <TwoHandCombat> <GroundMoveALL> <iceslide> <Swim> <DualHandAttack> <jumpFall> <Block> <HitIRQ> <StunMove>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	DualWield_Hop

	Type		male
	  Anim		male\dual_hop 10 11
	  PlayFx	FEET\SUPERJUMP1.FX:JUMP
	TEnd

	Type		huge
	  Anim		male\dual_hop 10 11
	  PlayFx	FEET\HUGEJUMP2.FX:JUMP
	TEnd

	Type		fem
	  Anim		fem\dual_hop 10	11
	  PlayFx	FEET\FEMJUMP1.FX:JUMP
	TEnd
MEnd

Move DualWield_Hop
	Requires	JUMP COMBAT WEAPON AIR DUAL
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	DualWield_Hop2

	Type		male
	  Anim		male\dual_hop 11 91
	TEnd

	Type		fem
	  Anim		fem\dual_hop 11	91
	TEnd
MEnd

Move DualWield_Hop2
	Requires	JUMP COMBAT WEAPON AIR DUAL
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle #ReqInputs
	NextMove	DualWield_HopApex

	Type		male
	  Anim		male\dual_hop 91 93
	TEnd

	Type		fem
	  Anim		fem\dual_hop 91	93
	TEnd
MEnd

Move DualWield_HopApex
	Requires	APEX COMBAT WEAPON AIR DUAL
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	DualWield_HopFall

	Type		male
	  Anim		male\dual_hop 93 94
	TEnd

	Type		fem
	  Anim		fem\dual_hop 93	94
	TEnd
MEnd

Move DualWield_HopFall
	Requires	AIR COMBAT WEAPON DUAL
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	DualWield_HopFall_2

	Type		male
	  Anim		male\dual_hop 94 128
	TEnd

	Type		fem
	  Anim		fem\dual_hop 94	112
	TEnd
MEnd

Move DualWield_HopFall_2
	Requires	AIR COMBAT WEAPON DUAL
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	DualWield_JumpPost

	Type		male
	  Anim		male\dual_hop 128
	TEnd

	Type		fem
	  Anim		fem\dual_hop 112
	TEnd
MEnd

#############################################################
## CLAW MODE - Now uses Dual wield mode
#############################################################

## CLAW JUMP ###########################################################

Move Claw_jumpPre
	Requires	JUMP COMBAT WEAPON DUAL CLAW AIR
	Priority	6
	Interrupts	<WepCombat> <DualHandCombat> <TwoHandCombat> <GroundMoveALL> <iceslide> <Swim> <DualHandAttack> <jumpFall> <Block> <HitIRQ> <StunMove>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Claw_jumpUp

	Type		male
	  Anim		male\dual_jump 3 6
	  PlayFx	FEET\SUPERJUMP1.FX:JUMP
	TEnd

	Type		huge
	  Anim		male\dual_jump 3 6
	  PlayFx	FEET\HUGEJUMP2.FX:JUMP
	TEnd

	Type		fem
	  Anim		fem\dual_jump 3	6
	  PlayFx	FEET\FEMJUMP1.FX:JUMP
	TEnd
MEnd

Move Claw_jumpUp
	Requires	JUMP COMBAT WEAPON DUAL CLAW AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Claw_jumpApex

	Type		male
	  Anim		male\dual_jump 7 64
	TEnd

	Type		fem
	  Anim		fem\dual_jump 7 64
	TEnd
MEnd

Move Claw_jumpApex
	Requires	APEX COMBAT WEAPON DUAL CLAW AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Claw_jumpFall

	Type		male
	  Anim		male\dual_jump 65 66
	TEnd

	Type		huge
	  Anim		male\dual_jump 65 66
	TEnd

	Type		fem
	  Anim		fem\dual_jump 65 66
	TEnd
MEnd

Move Claw_jumpFall
	Requires	COMBAT WEAPON DUAL CLAW AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Claw_jumpFall2

	Type		male
	  Anim		male\dual_jump 67 89
	TEnd

	Type		huge
	  Anim		male\dual_jump 67 93
	TEnd

	Type		fem
	  Anim		fem\dual_jump 67 93
	TEnd
MEnd

Move Claw_jumpFall2
	Requires	COMBAT WEAPON DUAL CLAW AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Claw_JumpPost

	Type		male
	  Anim		male\dual_jump 89
	TEnd

	Type		huge
	  Anim		male\dual_jump 93
	TEnd

	Type		fem
	  Anim		fem\dual_jump 93
	TEnd
MEnd

Move Claw_JumpPost
	Requires	COMBAT WEAPON DUAL CLAW
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <MoveIRQ> <GroundJumpPost> <IRQEmotesDualHand>
	Flags		ReqInputs
	NextMove	Claw_PostBlend
	Interpolate	0
	SticksONChild	LAND
	Scale		1.25

	Type		male
	  Anim		male\dual_post_jump 1
	  PlayFx	FEET\LAND1.FX:LAND
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 2
	TEnd
MEnd

## CLAW JUMP TO HOP FORWARD ###########################################################

Move Claw_JumpToHop
	Requires	FORWARD	JUMP COMBAT WEAPON DUAL CLAW AIR
	Priority	7
	Interrupts	Claw_jumpPre Claw_jumpUp
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Claw_Hop
	Interpolate	15

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

## CLAW JUMP FORWARD ###########################################################

Move Claw_HopPre
	Requires	FORWARD	JUMP COMBAT WEAPON DUAL CLAW AIR
	Priority	7
	Interrupts	<WepCombat> <DualHandCombat> <TwoHandCombat> <GroundMoveALL> <iceslide> <Swim> <DualHandAttack> <jumpFall> <Block> <HitIRQ> <StunMove>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Claw_Hop

	Type		male
	  Anim		male\dual_hop 10 11
	  PlayFx	FEET\SUPERJUMP1.FX:JUMP
	TEnd

	Type		huge
	  Anim		male\dual_hop 10 11
	  PlayFx	FEET\HUGEJUMP2.FX:JUMP
	TEnd

	Type		fem
	  Anim		fem\dual_hop 10 11
	  PlayFx	FEET\FEMJUMP1.FX:JUMP
	TEnd
MEnd

Move Claw_Hop
	Requires	JUMP COMBAT WEAPON DUAL CLAW AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Claw_Hop2

	Type		male
	  Anim		male\dual_hop 11 91
	TEnd

	Type		fem
	  Anim		fem\dual_hop 11 91
	TEnd
MEnd

Move Claw_Hop2
	Requires	JUMP COMBAT WEAPON DUAL CLAW AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	Claw_HopApex

	Type		male
	  Anim		male\dual_hop 91 92
	TEnd

	Type		fem
	  Anim		fem\dual_hop 91 92
	TEnd
MEnd

Move Claw_HopApex
	Requires	APEX COMBAT WEAPON DUAL CLAW AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	Claw_HopFall

	Type		male
	  Anim		male\dual_hop 92 94
	TEnd

	Type		fem
	  Anim		fem\dual_hop 92 94
	TEnd
MEnd

Move Claw_HopFall
	Requires	COMBAT WEAPON DUAL CLAW AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	Claw_HopFall_2

	Type		male
	  Anim		male\dual_hop 94 128
	TEnd

	Type		fem
	  Anim		fem\dual_hop 94 112
	TEnd
MEnd

Move Claw_HopFall_2
	Requires	COMBAT WEAPON DUAL CLAW AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	Claw_JumpPost

	Type		male
	  Anim		male\dual_hop 128
	TEnd

	Type		fem
	  Anim		fem\dual_hop 112
	TEnd
MEnd


#############################################################
## SLIDE
#############################################################

## SLIDE JUMP ###########################################################

Move SlidejumpPre
	Requires	JUMP AIR
	Priority	2
	Interrupts	SlideCycle SlideCycle_left SlideCycle_right SlidePre SlidePre_left SlidePre_right SlideJumpPost
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	SlidejumpUp
	Flags		ReqInputs

	Type		male
	  Anim		male\slide_jump	2 31
	TEnd
MEnd

Move SlidejumpUp
	Requires	JUMP AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	SlidejumpApex

	Type		male
	  Anim		male\slide_jump	32 33
	TEnd
MEnd

Move SlidejumpApex
	Requires	APEX AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	SlidejumpFall

	Type		male
	  Anim		male\slide_jump	36 37
	TEnd
MEnd

Move SlidejumpFall
	Requires	AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	SlidejumpFall2

	Type		male
	  Anim		male\slide_jump	38 56
	TEnd
MEnd

Move SlidejumpFall2
	Requires	AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	SlideJumpPost

	Type		male
	  Anim		male\slide_jump	57 59
	TEnd
MEnd

#############################################################

Move SlideJumpPost
	Requires	SLIDING
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <MoveIRQ> <GroundJumpPost> <BasicSlide> <IRQEmotesALL>
	NextMove	SlideCycle
	Interpolate	1
	SticksOnChild	LAND

	Type		male
	  Anim		male\slippery_postjump 5
	TEnd
MEnd

## 2hlarge JUMP UP ###########################################################

Move 2hlarge_jumpPre
	Requires	JUMP COMBAT WEAPON AIR
	Priority	4
	Interrupts	<Ready> <Combat> <WepCombat> <MoveIRQ> <BasicMove> <CombatMove> <WepMove> <ClubMove>
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TwoHandJump>
	Flags		ReqInputs
	NextMove	2hlarge_jumpUp

	Type		male
	  Anim		male\2hlarge_jump 3 6
	  PlayFx	FEET\SUPERJUMP1.FX:JUMP
	TEnd

	Type		huge
	  Anim		male\2hlarge_jump 3 6
	  PlayFx	FEET\HUGEJUMP2.FX:JUMP
	TEnd

	Type		fem
	  Anim		fem\2hlarge_jump 3 6
	  PlayFx	FEET\FEMJUMP1.FX:JUMP
	TEnd
MEnd

Move 2hlarge_jumpUp
	Requires	JUMP COMBAT WEAPON AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ> <TwoHandJump>
	Flags		Cycle
	NextMove	2hlarge_jumpApex

	Type		male
	  Anim		male\2hlarge_jump 6 65
	TEnd

	Type		fem
	  Anim		fem\2hlarge_jump 6 65
	TEnd

MEnd

Move 2hlarge_jumpApex
	Requires	APEX COMBAT WEAPON AIR
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	2hlarge_jumpFall

	Type		male
	  Anim		male\2hlarge_jump 65 66
	TEnd

	Type		fem
	  Anim		fem\2hlarge_jump 65 66
	TEnd

MEnd

Move 2hlarge_jumpFall
	Requires	COMBAT WEAPON AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	2hlarge_jumpFall2

	Type		male
	  Anim		male\2hlarge_jump 66 89
	TEnd

	Type		fem
	  Anim		fem\2hlarge_jump 66 89
	TEnd

MEnd

Move 2hlarge_jumpFall2
	Requires	COMBAT WEAPON AIR
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	2hlarge_JumpPost

	Type		male
	  Anim		male\2hlarge_jump 89
	TEnd

	Type		fem
	  Anim		fem\2hlarge_jump 89
	TEnd

MEnd

## 2hlarge JUMP POST ###########################################################

Move 2hlarge_JumpPost
	Requires	COMBAT WEAPON 2HLARGE
	Priority	3
	Interrupts	W_JumpPost
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	Flags		ReqInputs #FixArmReg
	NextMove	2hlarge_JumpPostcycle
	Interpolate	1
	SticksOnChild	LAND

	Type		male
	  Anim		male\2hlarge_jumppost 1
	  PlayFx	FEET\LAND1.FX:LAND  1
	  PlayFx	FEET\MALERIGHT.FX:FOOTFALLRIGHT 2
	  Scale		0.9
	TEnd

	Type		huge
	  Anim		huge\2hlarge_jumppost 1
	  PlayFx	FEET\LAND1.FX:LAND  1
	  PlayFx	FEET\HUGERIGHT.FX:FOOTFALLRIGHT 2
	  Scale		0.9
	TEnd

	Type		fem
	  Anim		fem\2hlarge_jumppost 1
	  PlayFx	FEET\LAND1.FX:LAND
	  PlayFx	FEET\FEMRIGHT.FX:FOOTFALLRIGHT 2
	  Scale		0.9
	TEnd

MEnd

Move 2hlarge_JumpPostcycle
	Requires	COMBAT WEAPON 2HLARGE
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <TwoHandCombat> <AttackIRQ> <BlockIRQ> <MoveIRQ> <IRQEmotesTwoHand>
	Flags		Cycle FixArmReg
	NextMove	PostShotgun
	Scale		0.50

	Type		male
	  Anim		male\2hlarge_ready 1
	TEnd

	Type		huge
	  Anim          huge\2hlarge_ready 1
	TEnd

	Type		fem
	  Anim          fem\2hlarge_ready 1
	TEnd

MEnd

## 2hlarge JUMP UP INTO FORWARD TRANSITION ###########################################################

Move 2hlarge_JumpToHop
	Requires	FORWARD	JUMP AIR WEAPON 2HLARGE
	Priority	5
	Interrupts	<TwoHandJump>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	2hlarge_Hop2
	Interpolate	15

	Type		male
	  Anim		male\2hlarge_hop 10 91
	TEnd

	Type		fem
	  Anim		fem\2hlarge_hop 10 91
	TEnd

MEnd

## 2hlarge JUMP FORWARD ###########################################################

Move 2hlarge_HopPre
	Requires	FORWARD	JUMP AIR WEAPON 2HLARGE
	Priority	5
	Interrupts	<IRQJumps> <PostMoveCycle> <Ready> <BasicMove> <jumpFall> <Block> <HitIRQ> <StunMove> <MoveIRQ>
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	2hlarge_Hop

	Type		male
	  Anim		male\2hlarge_hop 10 11
	  PlayFx	FEET\LAND3.FX
	TEnd


	Type		fem
	  Anim		fem\2hlarge_hop 10 11
	  PlayFx	FEET\FEMJUMP1.FX
	TEnd

	Type		huge
	  Anim		male\2hlarge_hop 10 11
	  PlayFx	FEET\LAND3.FX
	TEnd

MEnd

Move 2hlarge_Hop
	Requires	JUMP AIR WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		ReqInputs
	NextMove	2hlarge_Hop2
	Scale		0.75

	Type		male
	  Anim		male\2hlarge_hop 11 91
	  PlayFx	FEET\JUMP1.FX:JUMP
	TEnd

	Type		fem
	  Anim		fem\2hlarge_hop 11 91
	  PlayFx	FEET\LAND4.FX
	TEnd
	Type		huge
	  Anim		male\2hlarge_hop 11 91
	  PlayFx	FEET\JUMP1.FX:JUMP
	TEnd

MEnd

Move 2hlarge_Hop2
	Requires	JUMP AIR WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	Flags		Cycle
	NextMove	2hlarge_HopApex

	Type		male
	  Anim		male\2hlarge_hop 91 91
	TEnd

	Type		fem
	  Anim		fem\2hlarge_hop 91 91
	TEnd

MEnd

Move 2hlarge_HopApex
	Requires	APEX AIR WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <AttackIRQ>
	NextMove	2hlarge_HopFall

	Type		male
	  Anim		male\2hlarge_hop 91 93
	TEnd

	Type		fem
	  Anim		fem\2hlarge_hop 91 93
	TEnd

MEnd

Move 2hlarge_HopFall
	Requires	AIR WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove	2hlarge_HopFall_2

	Type		male
	  Anim		male\2hlarge_hop 93 120
	TEnd

	Type		fem
	  Anim		fem\2hlarge_hop 93 120
	TEnd

MEnd

Move 2hlarge_HopFall_2
	Requires	AIR WEAPON 2HLARGE
	Member		<DeathIRQ> <Jumps> <jumpFall> <AttackIRQ>
	Flags		Cycle
	NextMove	2hlarge_JumpPost

	Type		male
	  Anim		male\2hlarge_hop 120 128
	TEnd

	Type		fem
	  Anim		fem\2hlarge_hop 120 128
	TEnd

MEnd


