#########################################################
## NTam_Powers.INC - NTam Power related sequencers
#########################################################

##################################
## Clockwork - ready             ##
##################################

Move Clockwork_ready
	Requires	TEST1
	Priority	1
	Interrupts	<DeathIRQ>
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <BlockIRQ> <AttackIRQ> <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Clockwork_ready_cycle

	Type		male
	  Anim          male\clockwork_ready 1
	TEnd

	Type		fem
	  Anim          male\clockwork_ready 1
	TEnd

	Type            huge
	  Anim          male\clockwork_ready 1
	TEnd
MEnd

Move Clockwork_ready_cycle
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <BlockIRQ> <AttackIRQ> <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	Flags		Cycle

	Type		male
	  Anim          male\clockwork_ready 1
	TEnd

	Type		female
	  Anim          male\clockwork_ready 1
	TEnd

	Type		huge
	  Anim          male\clockwork_ready 1
	TEnd
MEnd

##################################
## Clockwork_Wrist - attack
##################################

Move Clockwork_plasmablast
	Requires	TEST6
	Priority	1
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove	Clockwork_wrist_combat_cycle
	Interpolate     10

	Type		male
	  Anim          male\clockwork_plasmablast 1
	TEnd
MEnd

Move Clockwork_plasmaray
	Requires	TEST7
	Priority	1
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove	Clockwork_wrist_combat_cycle
	Interpolate     10

	Type		male
	  Anim          male\clockwork_plasmaray 1
	TEnd
MEnd

Move Clockwork_plasmaspray
	Requires	TEST8
	Priority	1
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove	TestEmote5_Cycle
	Interpolate     0

	Type		male
	  Anim          male\Clockwork_plasmaspray 1
	TEnd
MEnd