

#######  DVD Slide    ##############################################################################

####################
### DVD Slide Forward ##
###################

Move DVDSlide_Pre
	Requires	FORWARD TEST7
	Interrupts	<BasicMove> <CombatMove> <PostMoveCycle>
	Member		<ReactIRQ> <DeathIRQ> <AttackIRQ> <iceslide>
	Flags		ReqInputs
	NextMove        DVDSlide_Cycle
	Priority        2


	Type		male
	  Anim          male\iceslide_forward 15 31
	TEnd

	Type		huge
	  Anim          huge\iceslide_forward 15 31
	TEnd

	Type		fem
	  Anim          fem\iceslide_forward 15 31
	TEnd
MEnd


Move DVDSlide_Cycle
	Requires	FORWARD test7
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <iceslide>
	Flags		Cycle
	NextMove        DVDSlide_Post
	Priority        2

	Type		male
	  Anim          male\iceslide_forward 1 31
	TEnd

	Type		huge
	  Anim          huge\iceslide_forward 1 31
	TEnd

	Type		fem
	  Anim          fem\iceslide_forward 1 31
	TEnd
MEnd


Move DVDSlide_Post
	Requires	TEST7
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <iceslide>
	NextMove        Ready
	Priority        1
	Scale           0.80

	Type		male
	  Anim          male\post_ready 7 24
	TEnd

	Type		huge
	  Anim          huge\postback 3 40
	TEnd

	Type		fem
	  Anim          fem\post_ready 7 24
	TEnd
MEnd


Move C_DVDSlide_Post
	Requires	COMBAT TEST7
	Priority        2
	Interrupts	DVDSlide_Post
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <iceslide>
	NextMove        CombatReady
	Scale           0.50

	Type		male
	  Anim          male\combat_cycle 21 31
	TEnd

	Type		huge
	  Anim          huge\combat_cycle 21 31
	TEnd

	Type		fem
	  Anim          fem\combat_cycle 21 31
	TEnd
MEnd

####################
#### DVD Slide_ Backward ##
####################
#
#Move DVDSlide_Pre_B
#	Requires	BACKWARD test7
#	Interrupts	DVDSlide_Cycle DVDSlide_Pre <BasicMove> <CombatMove> <PostMoveCycle>
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <iceslide>
#	Flags		ReqInputs
#	NextMove        DVDSlide_Cycle_B
#	Priority        2
#
#	Type		male
#	  Anim          male\iceslide_backward 15 31
#	TEnd
#
#	Type		huge
#	  Anim          huge\iceslide_backward 15 31
#	TEnd
#
#	Type		fem
#	  Anim          fem\iceslide_backward 15 31
#	TEnd
#MEnd
#
#
#Move DVDSlide_Cycle_B
#	Requires	BACKWARD test7
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <iceslide>
#	Flags		Cycle
#	NextMove        DVDSlide_Post
#	Priority        2
#
#	Type		male
#	  Anim          male\iceslide_backward 1 31
#	TEnd
#
#	Type		huge
#	  Anim          huge\iceslide_backward 1 31
#	TEnd
#
#	Type		fem
#	  Anim          fem\iceslide_backward 1 31
#	TEnd
#MEnd
#
####################
#### Iceslide Right ##
####################
#
#Move DVDSlide_Pre_R
#	Requires	STEPRIGHT test7
#	Interrupts	DVDSlide_Cycle DVDSlide_Pre <BasicMove> <CombatMove> <PostMoveCycle>
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <iceslide>
#	Flags		ReqInputs
#	NextMove        DVDSlide_Cycle_R
#	Priority        2
#
#	Type		male
#	  Anim          male\iceslide_34right 15 31
#	TEnd
#
#	Type		huge
#	  Anim          huge\iceslide_right 15 31
#	TEnd
#
#	Type		fem
#	  Anim          fem\iceslide_right 15 31
#	TEnd
#MEnd
#
#
#Move DVDSlide_Cycle_R
#	Requires	STEPRIGHT test7
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <iceslide>
#	Flags		Cycle
#	NextMove        DVDSlide_Post
#	Priority        2
#
#	Type		male
#	  Anim          male\iceslide_34right 1 31
#	TEnd
#
#	Type		huge
#	  Anim          huge\iceslide_right 1 31
#	TEnd
#
#	Type		fem
#	  Anim          fem\iceslide_right 1 31
#	TEnd
#MEnd
#
####################
#### Iceslide left ##
####################
#
#Move DVDSlide_Pre_L
#	Requires	STEPLEFT test7
#	Interrupts	DVDSlide_Cycle DVDSlide_Pre <BasicMove> <CombatMove> <PostMoveCycle>
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <iceslide>
#	Flags		ReqInputs
#	NextMove        DVDSlide_Cycle_L
#	Priority        2
#
#	Type		male
#	  Anim          male\iceslide_34left 15 31
#	TEnd
#
#	Type		huge
#	  Anim          huge\iceslide_left 15 31
#	TEnd
#
#	Type		fem
#	  Anim          fem\iceslide_left 15 31
#	TEnd
#MEnd
#
#
#Move DVDSlide_Cycle_L
#	Requires	STEPLEFT test7
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <iceslide>
#	Flags		Cycle
#	NextMove        DVDSlide_Post
#	Priority        2
#
#	Type		male
#	  Anim          male\iceslide_34left 1 31
#	TEnd
#
#	Type		huge
#	  Anim          huge\iceslide_left 1 31
#	TEnd
#
#	Type		fem
#	  Anim          fem\iceslide_left 1 31
#	TEnd
#MEnd
#
###################################################################################
#
#
####################
#### Wep Iceslide Forward ##
####################
#
#Move W_DVDSlide_Pre
#	Requires	FORWARD WEAPON test7
#	Interrupts	<WepMove> <TwoHandMove> <PostMoveCycle>
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <iceslide>
#	Flags		ReqInputs
#	NextMove        W_DVDSlide_Cycle
#	Priority        3
#
#	Type		male
#	  Anim          male\wep_iceslide_forward 15 31
#	TEnd
#
#	Type		huge
#	  Anim          huge\wep_iceslide_forward 15 31
#	TEnd
#
#	Type		fem
#	  Anim          fem\wep_iceslide_forward 15 31
#	TEnd
#MEnd
#
#
#Move W_DVDSlide_Cycle
#	Requires	FORWARD WEAPON test7
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <iceslide>
#	Flags		Cycle
#	NextMove        W_DVDSlide_Post
#	Priority        3
#
#	Type		male
#	  Anim          male\wep_iceslide_forward 1 31
#	TEnd
#
#	Type		huge
#	  Anim          huge\wep_iceslide_forward 1 31
#	TEnd
#
#	Type		fem
#	  Anim          fem\wep_iceslide_forward 1 31
#	TEnd
#MEnd
#
#
#Move W_DVDSlide_Post
#	Requires	WEAPON test7
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <iceslide>
#	NextMove        WepReady
#	Priority        2
#	Scale           0.50
#
#	Type		male
#	  Anim          male\wep_cycle 15 31
#	TEnd
#
#	Type		huge
#	  Anim          huge\wep_cycle 15 31
#	TEnd
#
#	Type		fem
#	  Anim          fem\wep_cycle 15 31
#	TEnd
#MEnd
#
#
#Move SG_DVDSlide_Post
#	Requires	WEAPON GUN test7
#	Interrupts	W_DVDSlide_Post
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <iceslide>
#	NextMove        ShotgunReady
#	Priority        3
#	Scale           0.50
#
#	Type		male
#	  Anim          male\rocket_cycle 15 31
#	TEnd
#
#	Type		huge
#	  Anim          huge\rocket_cycle 15 31
#	TEnd
#
#	Type		fem
#	  Anim          fem\rocket_cycle 15 31
#	TEnd
#MEnd
#
####################
#### Wep Iceslide Backward ##
####################
#
#Move W_DVDSlide_Pre_B
#	Requires	BACKWARD WEAPON test7
#	Interrupts	W_DVDSlide_Cycle W_DVDSlide_Pre <WepMove> <TwoHandMove> <PostMoveCycle>
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <iceslide>
#	Flags		ReqInputs
#	NextMove        W_DVDSlide_Cycle_B
#	Priority        3
#
#	Type		male
#	  Anim          male\wep_iceslide_backward 15 31
#	TEnd
#
#	Type		huge
#	  Anim          huge\wep_iceslide_backward 15 31
#	TEnd
#
#	Type		fem
#	  Anim          fem\wep_iceslide_backward 15 31
#	TEnd
#MEnd
#
#
#Move W_DVDSlide_Cycle_B
#	Requires	BACKWARD WEAPON test7
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <iceslide>
#	Flags		Cycle
#	NextMove        W_DVDSlide_Post
#	Priority        3
#
#	Type		male
#	  Anim          male\wep_iceslide_backward 1 31
#	TEnd
#
#	Type		huge
#	  Anim          huge\wep_iceslide_backward 1 31
#	TEnd
#
#	Type		fem
#	  Anim          fem\wep_iceslide_backward 1 31
#	TEnd
#MEnd
#
####################
#### Wep Iceslide Right ##
####################
#
#Move W_DVDSlide_Pre_R
#	Requires	STEPRIGHT WEAPON test7
#	Interrupts	W_DVDSlide_Cycle W_DVDSlide_Pre <WepMove> <TwoHandMove> <PostMoveCycle>
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <iceslide>
#	Flags		ReqInputs
#	NextMove        W_DVDSlide_Cycle_R
#	Priority        3
#
#	Type		male
#	  Anim          male\wep_iceslide_right 15 31
#	TEnd
#
#	Type		huge
#	  Anim          huge\wep_iceslide_right 15 31
#	TEnd
#
#	Type		fem
#	  Anim          fem\wep_iceslide_right 15 31
#	TEnd
#MEnd
#
#
#Move W_DVDSlide_Cycle_R
#	Requires	STEPRIGHT WEAPON test7
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <iceslide>
#	Flags		Cycle
#	NextMove        W_DVDSlide_Post
#	Priority        3
#
#	Type		male
#	  Anim          male\wep_iceslide_right 1 31
#	TEnd
#
#	Type		huge
#	  Anim          huge\wep_iceslide_right 1 31
#	TEnd
#
#	Type		fem
#	  Anim          fem\wep_iceslide_right 1 31
#	TEnd
#MEnd
#
####################
#### Wep Iceslide left ##
####################
#
#Move W_DVDSlide_Pre_L
#	Requires	STEPLEFT WEAPON test7
#	Interrupts	W_DVDSlide_Cycle W_DVDSlide_Pre <WepMove> <TwoHandMove> <PostMoveCycle>
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <iceslide>
#	Flags		ReqInputs
#	NextMove        W_DVDSlide_Cycle_L
#	Priority        3
#
#	Type		male
#	  Anim          male\wep_iceslide_left 15 31
#	TEnd
#
#	Type		huge
#	  Anim          huge\wep_iceslide_left 15 31
#	TEnd
#
#	Type		fem
#	  Anim          fem\wep_iceslide_left 15 31
#	TEnd
#MEnd
#
#
#Move W_DVDSlide_Cycle_L
#	Requires	STEPLEFT WEAPON test7
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <iceslide>
#	Flags		Cycle
#	NextMove        W_DVDSlide_Post
#	Priority        3
#
#	Type		male
#	  Anim          male\wep_iceslide_left 1 31
#	TEnd
#
#	Type		huge
#	  Anim          huge\wep_iceslide_left 1 31
#	TEnd
#
#	Type		fem
#	  Anim          fem\wep_iceslide_left 1 31
#	TEnd
#MEnd
#
#
