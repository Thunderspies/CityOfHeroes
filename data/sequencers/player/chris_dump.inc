
####################
#### Pickaxe Walk ##
####################
#
#Move PickaxeWalk
#	Requires	FORWARD WALK
#	Interrupts	<Ready> <PostMoveCycle> RunPre RunCycle
#	Member		<BasicMove> <ReactIRQ> <HitIRQ> <DeathIRQ>
#	Flags		Cycle MoveScale PICKRANDOMLY
#	NextMove        PickaxeWalkPost
#	Priority        2
#
#
#
#	Type		Male
#	  Anim          Male\pickaxe_walk 1
#	  PlayFx        GENERIC\Pickaxe.FX 0 Constant
#	MoveRate      0.2
#	TEnd
#
#MEnd
#
#Move PickaxeWalkRelaxed
#	Requires	FORWARD WALK
#	Interrupts	<Ready> <PostMoveCycle> RunPre RunCycle
#	Member		<BasicMove> <ReactIRQ> <HitIRQ> <DeathIRQ>
#	Flags		Cycle MoveScale PICKRANDOMLY
#	NextMove        PickaxeWalkPost
#	Priority        2
#
#
#
#	Type		Male
#	  Anim          Male\pickaxe_walk_relaxed 1
#	  PlayFx        GENERIC\Pickaxe.FX 0 Constant
#	MoveRate      0.2
#	TEnd
#
#
#
#MEnd
#
#
#Move PickaxeWalkPost
#	Member		<BasicMove> <ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle>
#	NextMove        Ready
##	Interpolate	5
#
#	Type		Male
#	  Anim          Male\pickaxe_idle 1
#	  PlayFx        GENERIC\Pickaxe.FX 0 Constant
#	TEnd
#
#
#MEnd
#
#
############################
#### Guard Sword held up ###
############################
#
#Move guard_sword_l
#	Requires	BLOCK Low High SWORD TELEKINESIS ###I doubt these are correct
#	Interrupts	<Ready> <Combat>
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <basicTHREAT>
#	Flags		Cycle NoSizeScale
#	Priority        5
#	Scale		0.3
#
#	Type		male
#	  Anim          male\guard_sword_l 1
#	  ## PlayFx	WEAPONS\Sword.fx 0 Constant
#  	  PlayFx	WEAPONS\katana_left.fx 0 Constant
#	TEnd
#MEnd
#
#
####################
#### Protesting ####   ### needs pipe weapon instead of bat
####################    requires flags are prolly wrong, ripped from sword block\guard
#
#Move protest_idle
#	Requires	test7 ##2hand Stop Threat hypnotize
#	Interrupts	<Ready> <Combat>
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <basicTHREAT>
#	Flags		Cycle NoSizeScale
#	Priority        4
#	CycleMove	protest_shakefist_pre
#	CycleMove	protest_yell_pre
#	CycleMove	protest_idle
#	CycleMove	protest_rally
#	interpolate	25
#
#	Type		male
#	  Anim          male\protest_idle 1
#    	  PlayFx	WEAPONS\baseball_bat.fx 0 Constant
#
#	TEnd
#MEnd
#
#Move protest_shakefist_pre
#	Requires	test7 ##2hand Stop Threat hypnotize
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <basicTHREAT>
#	Flags		NoSizeScale
#	Priority        4
#	NextMove	protest_shakefist_cycle
#	interpolate	15
#
#	Type		male
#	  Anim          male\protest_pipe1 1 40
#    	  PlayFx	WEAPONS\baseball_bat.fx 0 Constant
#	TEnd
#MEnd
#
#Move protest_shakefist_cycle
#	Requires	test7 ##2hand Stop Threat hypnotize
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <basicTHREAT>
#	Flags		Cycle NoSizeScale
#	Priority        4
#	CycleMove	protest_shakefist_cycle
#	CycleMove	protest_shakefist_cycle
#	CycleMove	protest_shakefist_post
#	CycleMove	protest_shakefist_cycle
#	interpolate	15
#
#	Type		male
#	  Anim          male\protest_pipe1 20 40
#    	  PlayFx	WEAPONS\baseball_bat.fx 0 Constant
#
#	TEnd
#MEnd
#
#Move protest_shakefist_post
#	Requires	test7 ##2hand Stop Threat hypnotize
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <basicTHREAT>
#	Flags		Cycle NoSizeScale
#	Priority        4
#	CycleMove	protest_yell_pre
#	CycleMove	protest_idle
#	CycleMove	protest_rally
#	CycleMove	protest_shakefist_pre
#	interpolate	15
#
#	Type		male
#	  Anim          male\protest_pipe1 20
#    	  PlayFx	WEAPONS\baseball_bat.fx 0 Constant
#
#	TEnd
#MEnd
#
#Move protest_rally
#	Requires	test7 ##2hand Stop Threat hypnotize
#	Interrupts	<Ready> <Combat>
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <basicTHREAT>
#	Flags		Cycle NoSizeScale
#	Priority        4
#	CycleMove	protest_rally
#	CycleMove	protest_rally
#	CycleMove	protest_rally_post
#	CycleMove	protest_rally
#	interpolate	15
#	scale		1.5
#
#	Type		male
#	  Anim          male\protest_pipe2 1
#    	  PlayFx	WEAPONS\baseball_bat.fx 0 Constant
#
#	TEnd
#MEnd
#
#Move protest_rally_post
#	Requires	test7 ##2hand Stop Threat hypnotize
#	Interrupts	<Ready> <Combat>
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <basicTHREAT>
#	Flags		Cycle NoSizeScale
#	Priority        4
#	CycleMove	protest_shakefist_post
#	CycleMove	protest_yell_post
#	interpolate	15
#	scale		1.5
#
#	Type		male
#	  Anim          male\protest_pipe2 1
#    	  PlayFx	WEAPONS\baseball_bat.fx 0 Constant
#
#	TEnd
#MEnd
#
#Move protest_yell_pre
#	Requires	test7 ##2hand Stop Threat hypnotize
#	Interrupts	<Ready> <Combat>
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <basicTHREAT>
#	Flags		NoSizeScale
#	Priority        4
#	NextMove	protest_yell_cycle
#	interpolate	15
#	scale		1.5
#
#
#	Type		male
#	  Anim          male\protest_pipe3 1 80
#    	  PlayFx	WEAPONS\baseball_bat.fx 0 Constant
#
#	TEnd
#MEnd
#
#Move protest_yell_cycle
#	Requires	test7 ##2hand Stop Threat hypnotize
#	Interrupts	<Ready> <Combat>
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <basicTHREAT>
#	Flags		Cycle NoSizeScale
#	Priority        4
#	CycleMove	protest_yell_cycle
#	CycleMove	protest_yell_cycle
#	CycleMove	protest_yell_post
#	CycleMove	protest_yell_cycle
#	interpolate	15
#	scale		1.5
#
#
#	Type		male
#	  Anim          male\protest_pipe3 50 80
#    	  PlayFx	WEAPONS\baseball_bat.fx 0 Constant
#
#	TEnd
#MEnd
#
#Move protest_yell_post
#	Requires	test7 ##2hand Stop Threat hypnotize
#	Interrupts	<Ready> <Combat>
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <basicTHREAT>
#	Flags		Cycle NoSizeScale
#	Priority        4
#	CycleMove	protest_rally
#	CycleMove	protest_shakefist_pre
#	CycleMove	protest_yell_pre
#	CycleMove	protest_idle
#	interpolate	15
#	scale		1.5
#
#
#	Type		male
#	  Anim          male\protest_pipe3 50
#    	  PlayFx	WEAPONS\baseball_bat.fx 0 Constant
#
#	TEnd
#MEnd
#
#
#
#############################
#### Oppressed Businessman ##
#############################
#
#Move Oppressed_Walk
#	Requires	WALK FORWARD
#	Interrupts	<Ready> <PostMoveCycle> RunPre RunCycle
#	Member		<BasicMove> <ReactIRQ> <HitIRQ> <DeathIRQ>
#	Flags		Cycle MoveScale PICKRANDOMLY
#	Priority        2
#
#	Type		Male
#	  Anim          Male\walk_oppress 1
#	MoveRate      0.18
#	TEnd
#MEnd
#
#Move Oppressed_Walk_Briefcase_Left
#	Requires	 WALK FORWARD
#	Interrupts	<Ready> <PostMoveCycle> RunPre RunCycle
#	Member		<BasicMove> <ReactIRQ> <HitIRQ> <DeathIRQ>
#	Flags		Cycle MoveScale PICKRANDOMLY
#	Priority        2
#
#	Type		Male
#	  Anim          Male\walk_oppress_hold_l 1
#	  PlayFx        GENERIC\FX_briefcase_L.fx 0 Constant
#	MoveRate      0.18
#	TEnd
#MEnd
#
#
#Move Oppressed_Walk_Briefcase_Right
#	Requires	WALK FORWARD
#	Interrupts	<Ready> <PostMoveCycle> RunPre RunCycle
#	Member		<BasicMove> <ReactIRQ> <HitIRQ> <DeathIRQ>
#	Flags		Cycle MoveScale PICKRANDOMLY
#	Priority        2
#
#	Type		Male
#	  Anim          Male\walk_oppress_hold_r 1
#	  PlayFx        GENERIC\FX_briefcase_R.fx 0 Constant
#	MoveRate      0.18
#	TEnd
#MEnd
#
#
#Move Oppressed_Walk_Briefcase_Both
#	Requires	WALK FORWARD
#	Interrupts	<Ready> <PostMoveCycle> RunPre RunCycle
#	Member		<BasicMove> <ReactIRQ> <HitIRQ> <DeathIRQ>
#	Flags		Cycle MoveScale PICKRANDOMLY
#	Priority        2
#
#	Type		Male
#	  Anim          Male\walk_oppress_hold_both 1
#	  PlayFx        GENERIC\FX_briefcase_R.fx 0 Constant
#	  PlayFx        GENERIC\FX_briefcase_L.fx 0 Constant
#	MoveRate      0.18
#	TEnd
#MEnd
#
#
############################
### Carrying Groceries    ##
############################
#
#Move Walk_Groceries_Left
#	Requires	WALK FORWARD
#	Interrupts	<Ready> <PostMoveCycle> RunPre RunCycle
#	Member		<BasicMove> <ReactIRQ> <HitIRQ> <DeathIRQ>
#	Flags		Cycle MoveScale PICKRANDOMLY
#	Priority        2
#
#	Type		Male
#	  Anim          Male\walkV_groceries_L 1
#	  PlayFx        GENERIC\groceries_l.fx 0 Constant
#	MoveRate      0.18
#	TEnd
#MEnd
#
#
#Move Walk_Groceries_Right
#	Requires	WALK FORWARD
#	Interrupts	<Ready> <PostMoveCycle> RunPre RunCycle
#	Member		<BasicMove> <ReactIRQ> <HitIRQ> <DeathIRQ>
#	Flags		Cycle MoveScale PICKRANDOMLY
#	Priority        2
#
#	Type		Male
#	  Anim          Male\walkV_groceries_R 1
#	  PlayFx        GENERIC\groceries_r.fx 0 Constant
#	MoveRate      0.18
#	TEnd
#MEnd
#
#
#
#
#Move Walk_Groceries_2handed
#	Requires	WALK FORWARD
#	Interrupts	<Ready> <PostMoveCycle> RunPre RunCycle
#	Member		<BasicMove> <ReactIRQ> <HitIRQ> <DeathIRQ>
#	Flags		Cycle MoveScale PICKRANDOMLY
#	Priority        2
#
#	Type		Male
#	  Anim          Male\walkV_groceries_2h 1
#	  PlayFx        GENERIC\groceries_r.fx 0 Constant
#	  MoveRate      0.18
#	TEnd
#MEnd
#
#
#Move Walk_Groceries_Left_Right
#	Requires	WALK FORWARD
#	Interrupts	<Ready> <PostMoveCycle> RunPre RunCycle
#	Member		<BasicMove> <ReactIRQ> <HitIRQ> <DeathIRQ>
#	Flags		Cycle MoveScale PICKRANDOMLY
#	Priority        2
#
#	Type		Male
#	  Anim          Male\walkV_groceries_LR 1
#	  PlayFx        GENERIC\groceries_l.fx 0 Constant
#	  PlayFx        GENERIC\groceries_r.fx 0 Constant
#	MoveRate      0.18
#	TEnd
#MEnd
#

#############################
#### Schizo walk (banging head) ##
#############################
#
#Move walk_schizo1
#	Requires	WALK FORWARD
#	Interrupts	<Ready> <PostMoveCycle> RunPre RunCycle
#	Member		<BasicMove> <ReactIRQ> <HitIRQ> <DeathIRQ>
#	Flags		Cycle MoveScale PICKRANDOMLY
#	Priority        2
#	Interpolate	20
#	cyclemove	walk_schizo2
#	cyclemove	walk_schizo3
#	cyclemove	walk_schizo1
#
#	Type		Male
#	  Anim          Male\walk_schizo1 1
#	MoveRate      0.18
#	TEnd
#MEnd
#
#Move walk_schizo2
#	Requires	WALK FORWARD
#	Interrupts	<Ready> <PostMoveCycle> RunPre RunCycle
#	Member		<BasicMove> <ReactIRQ> <HitIRQ> <DeathIRQ>
#	Flags		Cycle MoveScale PICKRANDOMLY
#	Priority        2
#	Interpolate	15
#	cyclemove	walk_schizo3
#	cyclemove	walk_schizo1
#	cyclemove	walk_schizo2
#
#	Type		Male
#	  Anim          Male\walk_schizo2 1
#	MoveRate      0.18
#	TEnd
#MEnd
#
#Move walk_schizo3
#	Requires	WALK FORWARD
#	Interrupts	<Ready> <PostMoveCycle> RunPre RunCycle
#	Member		<BasicMove> <ReactIRQ> <HitIRQ> <DeathIRQ>
#	Flags		Cycle MoveScale PICKRANDOMLY
#	Priority        2
#	Interpolate	15
#	cyclemove	walk_schizo1
#	cyclemove	walk_schizo2
#	cyclemove	walk_schizo3
#
#	Type		Male
#	  Anim          Male\walk_schizo3 1
#	MoveRate      0.18
#	TEnd
#MEnd
#
#
#
###############################
#### Archer looking around  ###
###############################
#
#Move bow_lookout
#	Requires       Sit threat Menace High Pose
#	Interrupts	<Ready> <Combat>
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <basicTHREAT>
#	Flags		Cycle NoSizeScale
#	Priority        5
#
#	Type		male
#	  Anim          male\bow_lookout 1
#	  PlayFx        WEAPONS\Bow\New_bow.fx 0 Constant
#	TEnd
#MEnd
#
#


#
#
#############################
#### human rooted by plant ##
#############################
#
#Move plant_rooted1
#	Requires	test1 ##WALK FORWARD
#	Interrupts	<Ready> <PostMoveCycle> RunPre RunCycle
#	Member		<BasicMove> <ReactIRQ> <HitIRQ> <DeathIRQ>
#	Flags		Cycle ##PICKRANDOMLY
#	Priority        3
##	cyclemove	plant_rooted2
##	cyclemove	plant_rooted3
##	cyclemove	plant_rooted1
#
#	Type		Male
#	  Anim          Male\plant_rooted1 1
#	  PlayFx        GENERIC\Plant_Root.fx 0 Constant
#	TEnd
#MEnd

#Move plant_rooted2
#	Requires	test8 ##WALK FORWARD
#	Interrupts	<Ready> <PostMoveCycle> RunPre RunCycle
#	Member		<BasicMove> <ReactIRQ> <HitIRQ> <DeathIRQ>
#	Flags		Cycle MoveScale PICKRANDOMLY
#	Priority        3
#	cyclemove	plant_rooted3
#	cyclemove	plant_rooted1
#	cyclemove	plant_rooted2
#
#	Type		Male
#	  Anim          Male\plant_rooted2 1
#	TEnd
#MEnd
#
#Move plant_rooted3
#	Requires	test8 ##WALK FORWARD
#	Interrupts	<Ready> <PostMoveCycle> RunPre RunCycle
#	Member		<BasicMove> <ReactIRQ> <HitIRQ> <DeathIRQ>
#	Flags		Cycle MoveScale PICKRANDOMLY
#	Priority        3
#	cyclemove	plant_rooted1
#	cyclemove	plant_rooted2
#	cyclemove	plant_rooted3
#
#	Type		Male
#	  Anim          Male\plant_rooted3 1
#	TEnd
#MEnd
#






