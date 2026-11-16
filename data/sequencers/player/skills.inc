#############################################################
## SKILLS - Sequencers use for SSOCS
#############################################################

Move Pre_SkillMode
	Requires	SKILLMODE
	Priority        1
	Interrupts	<Ready> <Combat> <WepCombat> <TwoHandCombat> <DualHandCombat>
	Member		<ReactIRQ> <DeathIRQ> <AttackIRQ> <PreMove>
	Flags		ReqInputs
	NextMove        SkillMode
	Interpolate	15
	Scale		0.8

	Type		male
	  Anim          male\stand_Xarms 20 68
	TEnd

	Type		huge
	  Anim          huge\stand_Xarms 20 68
	TEnd

	Type		fem
	  Anim          fem\stand_Xarms 20 68
	TEnd
MEnd

Move SkillMode
	Requires	SKILLMODE
	Member		<Skill> <ReactIRQ> <DeathIRQ>  <AttackIRQ>
	Flags		Cycle
	NextMove        SkillMode_post
	Priority        1
	Scale		0.5

	Type		male
	  Anim          male\idle_xarms_look 1
	TEnd

	Type		huge
	  Anim          huge\idle_xarms_look 1
	TEnd

	Type		fem
	  Anim          fem\idle_xarms_look 1
	TEnd
MEnd

Move SkillMode_neutral
	Requires	SKILLMODE
	Member		<Skill> <ReactIRQ> <DeathIRQ>  <AttackIRQ>
	Flags		Cycle
	NextMove        SkillMode_post
	Priority        1

	Type		male
	  Anim          male\skill_neutral 1
	TEnd
MEnd

Move SkillMode_post
	Member		<Ready> <ReactIRQ> <DeathIRQ> <AttackIRQ>
	NextMove	Ready
	Interpolate	18
	Scale		0.7

	Type		male
	  Anim          male\stand_2Hhips 120
	TEnd

	Type		huge
	  Anim          huge\stand_2Hhips 120
	TEnd

	Type		fem
	  Anim          fem\stand_2Hhips 120
	TEnd
MEnd


###################
### Skill Walk Forward ##
###################

Move Skill_WalkPre
	Requires	SKILLMODE FORWARD
	Priority        2
	Interrupts	<PreMove> <Skill> <PostMoveCycle> <BasicMove>
	Member		<ReactIRQ> <DeathIRQ> <AttackIRQ> <BlockIRQ> <jumpFall>
	Flags		ReqInputs MoveScale
	NextMove        Skill_WalkCycle

	Type		male
	  Anim		male\new_walk_hero 17 33
	  MoveRate	0.2   ##0.15  ##0.20
	TEnd

	Type		fem
	  Anim          fem\new_walk_hero 17 33
	  MoveRate	0.2
	TEnd

	Type		huge
	  Anim		huge\new_walk_hero 17 33
	  MoveRate	0.2
	TEnd
MEnd

Move Skill_WalkCycle
	Requires	SKILLMODE FORWARD
	Interrupts	Skill_WalkBackCycle
	Member		<SkillMove> <ReactIRQ> <DeathIRQ> <AttackIRQ> <BlockIRQ>
	Flags		Cycle MoveScale
	NextMove        Skill_WalkPost
	Priority        2

	Type		male
	  Anim          male\new_walk_hero 1 33
	  MoveRate	0.2  ##0.45   ##0.15  ##0.20
	TEnd

	Type		fem
	Anim          fem\new_walk_hero 1 33
	  MoveRate	0.2
	TEnd

	Type		huge
	Anim          huge\new_walk_hero 1 33
	  MoveRate	0.2
	TEnd
MEnd

Move Skill_WalkPost
	Requires	SKILLMODE
	Priority        1
	Member		<ReactIRQ> <DeathIRQ> <AttackIRQ> <BlockIRQ> <PostMoveCycle>
	NextMove        SkillMode_neutral
	Scale		0.6

	Type		male
	  Anim          male\skill_post_walk 7
	TEnd
MEnd


###################
### Skill Walk Backward ##
###################

Move Skill_WalkBackPre
	Requires	SKILLMODE BACKWARD
	Interrupts	<PreMove> <Skill> ## <PostMoveCycle> <BasicMove>
	Member		<ReactIRQ> <DeathIRQ> <AttackIRQ> <BlockIRQ>
	Flags		ReqInputs MoveScale
	NextMove        Skill_WalkBackCycle
	Priority        2

	Type		male
	  Anim          male\new_walk_hero_back 1 33
	  MoveRate	0.2   ##0.15  ##0.20
	TEnd

	Type		fem
	  Anim          fem\new_walk_hero_back 1 33
	  MoveRate	0.2
	TEnd

	Type		huge
	  Anim          huge\new_walk_hero_back 1 33
	  MoveRate	0.2
	TEnd
MEnd

Move Skill_WalkBackCycle
	Requires	SKILLMODE BACKWARD
	Interrupts	Skill_WalkCycle <PostMoveCycle>
	Member		<SkillMove> <ReactIRQ> <DeathIRQ> <AttackIRQ> <BlockIRQ>
	Flags		Cycle MoveScale
	NextMove        Skill_WalkPost
	Priority        2

	Type		male
	  Anim          male\new_walk_hero_back 1 33
	  MoveRate	0.2   ##0.15  ##0.20
	TEnd

	Type		fem
	  Anim          fem\new_walk_hero_back 1 33
	  MoveRate	0.2
	TEnd

	Type		huge
	  Anim          huge\new_walk_hero_back 1 33
	  MoveRate	0.2
	TEnd
MEnd

#################################
###### Detective skill  ######
#################################

Move 1st_Detective
	Requires	SKILLMODE BUSY LOW SPAWNHIP
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        1st_Detective_2
	Priority        4
	Sets            CANTMOVE
##	Flags		FixArmReg


	Type		male
	  Anim          male\skill_new_detective 1 96
	TEnd

MEnd


Move 1st_Detective_2
	Requires	SKILLMODE
	Member		<ReactIRQ> <DeathIRQ> <Skill> <AttackIRQ> <PostMoveCycle>
	NextMove        Detective_cycle
	Priority        1
	Interpolate     0
#	Flags		FixArmReg

	Type		male
	  Anim          male\skill_new_detective 97
	TEnd


MEnd


Move 2nd_Detective
	Requires	SKILLMODE BUSY Low
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        2nd_Detective_2
	Priority        3
	Sets            CANTMOVE
	Interpolate	8
#	Flags		FixArmReg
	Interpolate	15

	Type		male
	  Anim          male\skill_new_detective_2nd 1 85
	TEnd


MEnd


Move 2nd_Detective_2
	Requires	SKILLMODE
	Member		<ReactIRQ> <DeathIRQ> <Skill> <AttackIRQ> <PostMoveCycle>
	NextMove        Detective_cycle
	Priority        1
	Interpolate     0
	Flags		FixArmReg

	Type		male
	   Anim         male\skill_new_detective_2nd 86
	TEnd
MEnd

Move Detective_cycle
	Requires	SKILLMODE
	Priority        1
	Member		<Skill> <ReactIRQ> <DeathIRQ>  <AttackIRQ>
	Flags		Cycle
	NextMove        Detective_Post

	Type		male
	  Anim          male\skill_new_detective 165
	TEnd
MEnd

Move Detective_Post	##(when detective power is off - see if it gets used)
	Requires	SKILLMODE
	Priority        1
	Member		<Skill> <ReactIRQ> <DeathIRQ>  <AttackIRQ>
	Flags		Cycle
	NextMove        SkillMode

	Type		male
	  Anim          male\skill_new_detective 165 170
	TEnd
MEnd


#################################
###### Academic skill  ######
#################################

Move 1st_Academic
	Requires	SKILLMODE BUSY HIGH SPAWNHIP
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        1st_Academic_2
	Sets            CANTMOVE

	Type		male
	  Anim          male\skill_academic 1 40
	TEnd
MEnd

Move 1st_Academic_2
	Requires	SKILLMODE
	Member		<ReactIRQ> <DeathIRQ> <Skill> <AttackIRQ> <BlockIRQ>
	NextMove        1st_Academic_cycle
	Priority        1
	Interpolate     0

	Type		male
	  Anim          male\skill_academic 41 70
	TEnd

MEnd

Move 1st_Academic_cycle
	Requires	SKILLMODE
	Member		<Skill> <ReactIRQ> <DeathIRQ> <AttackIRQ>
	Flags		Cycle
	NextMove	Pre_SkillMode
	Priority        1

	Type		male
	  Anim          male\skill_academic 71 130
	TEnd

MEnd

####

Move 2nd_Academic
	Requires	SKILLMODE BUSY high
	Interrupts	1st_Academic_cycle
	Member		<DeathIRQ>
	NextMove        2nd_Academic_2
	Priority        5
	Sets            CANTMOVE
	Interpolate     8

	Type		male
	  Anim          male\skill_academic_mirror 10 40
	TEnd

MEnd


Move 2nd_Academic_2
	Requires	SKILLMODE
	Member		<ReactIRQ> <DeathIRQ> <Skill> <AttackIRQ> <BlockIRQ>
	NextMove        2nd_Academic_cycle
	Priority        1
	Interpolate     0

	Type		male
	  Anim          male\skill_academic_mirror 41 69
	TEnd

MEnd

Move 2nd_Academic_cycle
	Requires	SKILLMODE
	Member		<Skill> <ReactIRQ> <DeathIRQ> <AttackIRQ>
	Flags		Cycle
	Priority        1

	Type		male
	  Anim          male\skill_academic_mirror 70 80
	TEnd

MEnd

###

Move 3rd_Academic
	Requires	SKILLMODE BUSY high
	Interrupts	2nd_Academic_cycle
	Member		<DeathIRQ>
	NextMove        3rd_Academic_2
	Priority        5
	Sets            CANTMOVE
	Interpolate     8

	Type		male
	  Anim          male\skill_academic 10 40
	TEnd

MEnd


Move 3rd_Academic_2
	Requires	SKILLMODE
	Member		<ReactIRQ> <DeathIRQ> <Skill> <AttackIRQ> <BlockIRQ>
	NextMove        1st_Academic_cycle
	Priority        1
	Interpolate     0

	Type		male
	  Anim          male\skill_academic 41 69
	TEnd

MEnd



#################################
###### Scientific skill  ######
#################################

Move Scientific
	Requires	SKILLMODE BUSY Mid
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Scientific_2
	Priority        3
	Sets            CANTMOVE
	Interpolate     8

	Type		male
	  Anim          male\med_dead 1 60
	  PlayFX	POWERS\Medicine\MReviveWU.fx 0 Constant
	TEnd

	Type		huge
	  Anim          huge\med_dead 1 60
	  PlayFX	POWERS\Medicine\MReviveWU.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\med_dead 1 60
	  PlayFX	POWERS\Medicine\MReviveWU.fx 0 Constant
	TEnd

MEnd

Move Scientific_2
	Requires	SKILLMODE
	Member		<ReactIRQ> <DeathIRQ> <CombatAttack> <AttackIRQ> <BlockIRQ> <Skill>
	NextMove        Scientific_post
	Priority        1
	Interpolate     8

	Type		male
	  Anim          male\med_dead 61 90
	  PlayFX	POWERS\Medicine\MReviveWU.fx 0 Constant
	TEnd

	Type		huge
	  Anim          huge\med_dead 61 90
	  PlayFX	POWERS\Medicine\MReviveWU.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\med_dead 61 90
	  PlayFX	POWERS\Medicine\MReviveWU.fx 0 Constant
	TEnd

MEnd

Move Scientific_post
	Requires	SKILLMODE
	Member		<ReactIRQ> <DeathIRQ> <CombatAttack> <AttackIRQ> <BlockIRQ> <Skill>
	NextMove        Scientific_post_cycle
	Priority        1
	Interpolate     8

	Type		male
	  Anim          male\med_dead 201
	TEnd

	Type		huge
	  Anim          huge\med_dead 201
	TEnd

	Type		fem
	  Anim          fem\med_dead 201
	TEnd

MEnd

Move Scientific_post_cycle
	Requires	SKILLMODE
	Member		<Skill> <ReactIRQ> <DeathIRQ>  <AttackIRQ>
	Flags		Cycle
	Priority        1

	Type		male
	  Anim          male\Skel_ready2 1
	TEnd

	Type		huge
	  Anim          huge\Skel_ready 1 60
	TEnd

	Type		fem
	  Anim          fem\Skel_ready2 1
	TEnd


MEnd


##### crap tests  #####################


#################################
###### Crap tests  ######
#################################
#
#
#Move Craptest2
#	Requires	Test6
#	Interrupts	<AttackIRQ>
#	Priority        3
#	Interpolate     8
#	Flags		Cycle
#
#	Type		male
#	  Anim          male\skill 1
#	TEnd
#
#MEnd
#
#
#Move Craptest3
#	Requires	Test6
#	Interrupts	<AttackIRQ>
#	Priority        3
#	Interpolate     8
#	Flags		Cycle
#
#	Type		male
#	  Anim          male\skill_security 1
#	TEnd
#
#MEnd