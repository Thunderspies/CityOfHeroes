
#
#######################
#### Basic Spawn  ####
######################
#
#Move Thug_Spawn_Testing
#	Requires	test2 #SPAWNEDASPET
#	Interrupts	<Ready> <Combat> <WepCombat> <FlyReady> <RaiseDeadIRQ>
#	NextMove        Thug_Spawn_Testing_Post
#	Priority        10
#	Interpolate     0
#	Sets            CANTMOVE
#	Flags		NoSizeScale NotSelectable FixArmReg
#
#
#	Type		male
#	  Anim          male\Thug_MotoSpawn 1
#	  PlayFX	V_COV\Powers\Thugs\Thugs_SpawnFadInMoto.fx 0 CONSTANT
#	  PlayFX	V_COV\Powers\Thugs\FX_Motorcycle.fx 0 CONSTANT
#	TEnd
#
#
##	Type		male
##	  Anim          male\Thugs_SpawnManHole 1
##	  PlayFX	V_COV\Powers\Thugs\Thugs_SpawnFadIn.fx 0 CONSTANT
##	  PlayFX	V_COV\Powers\Thugs\Thugs_SpawnManHoleCover.fx CONSTANT
##	TEnd
#
#MEnd

Move Thug_Spawn_Testing_Post
	Member		<Ready> <ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ>
	NextMove	Spawned_cycle
	Flags		NoSizeScale ##NotSelectable
##	Sets            CANTMOVE

	Type		male
	  Anim          male\Skel_ready2 1
	TEnd

MEnd


###################
###### TESTING ####
###################

Move Arachnoid_Testing
#	Requires	test7
	Interrupts	<Ready> <Combat> <WepCombat> <FlyReady> <RaiseDeadIRQ>
	Member		<Ready> <AttackIRQ> <ReactIRQ> <HitIRQ> <DeathIRQ>
	Flags		Cycle NoSizeScale
	Priority        10
	scale		.75


	Type		male
	  Anim          male\Arachnoid_HangCeiling 1 80
	  ##Playfx	XXXX_ArachnoidTesting.fx
	TEnd

MEnd
