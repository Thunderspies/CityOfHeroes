
###################
###### Test sit ######
###################
#Move Sit_pre
#	Requires	Sit
#	Interrupts	<AttackIRQ>
#       Member		<DeathIRQ>
#	Anim            models\astro 1 62
#	NextMove	Sit_cycle
#	Scale		0.7
#	Sets		CantMove
#Move Sit_cycle
#	Requires	Combat Astral
#        Member		<DeathIRQ> <AttackIRQ>
#	Anim            models\astro 63 75
#	Flags		Cycle
#	NextMove	Sit_getup
#	Sets		CantMove
#Move Sit_getup
#        Member         <ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ>
#	Anim           models\astro 75
#	NextMove	Ready
############### test emotes ###############



####### test junk #####################################################
###################
### Sit_Meditate (w\ plane)  #####
###################
#Move Sit_Meditate
#	Requires       	Test9  ##Sit Meditate
#	Interrupts	<Ready> <Combat>
#	Member         <ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ>
#	Anim            models\astro 63 64
#	Flags		Cycle
#	NextMove	Sit_Low_getup
#	PlayFX		GENERIC\plane.fx
#	FXFlags		Constant
#	PlayFX		ENEMYFX\Oscillator\Propeller.fx
#	FXFlags		Constant
###################
### Box Ready  #####
###################

#Move Box_Ready
#	Requires	CARRY BOX LOOT
#	Interrupts	<Ready> <Combat>
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ>
#	Flags		Cycle
#	Priority        3
#
#	Type		male
#	  Anim          male\box_ready 1
#	  PlayFx        GENERIC\PROP_THEFT.FX  0 Constant
#	  PlayFx        GENERIC\PROP_THEFT_EXTRA.FX  0 Constant
#	TEnd
#
#	Type		huge
#	  Anim          huge\thumbsup 1
#	TEnd
#
#	Type		fem
#	  Anim          fem\thumbsup 1
#	TEnd
#MEnd
#
##	Sets		CantMove
###################
### Box Cycle  #####
###################
#Move Box_Cycle
#	Requires       	Test9  ##Box Carry
#	Interrupts	<Ready> <Combat>
#	Member         <ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ>
#	Anim            models\box_ready 1
#	Flags		Cycle
#	PlayFX		GENERIC\Prop_Theft.fx
###################
### Box Sneek\Walk Forward ##
###################
#Move Box_Sneek
#	Requires	Test9 Forward ##Box Carry Forward
#	Interrupts	Box_Ready Box_Cycle <Ready> <BasicMove> <CombatMove> <WepMove> <TwoHandMove>
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <BlockIRQ>
#	Flags		Cycle
#	Anim            models\box_sneek 1
#	NextMove        Box_Cycle
#	Scale		0.4
#	PlayFX		GENERIC\Prop_Theft.fx
###################
### Box Ditch Run ##
###################
#Move Box_Ditch_Run
#	Requires	Forward
#	Interrupts	Box_Ready Box_Cycle
#	Member		<BasicMove> <DeathIRQ> <AttackIRQ>
#	NextMove        Box_Ditch_Run2
#	Anim           models\box_ditch 1 12
#	PlayFX		GENERIC\Prop_Theft_Toss.fx
#	SetsOnChild	ShortToss
#	Sets		CantMove
#Move Box_Ditch_Run2
#	Requires	Forward
#	Member		<BasicMove> <DeathIRQ> <AttackIRQ>
#	NextMove        WalkCycle
#	Anim           models\box_ditch 13
#	Interpolate	0
############





##############################
##### Demo move  #####
#############################

Move Demo_move
	Requires	test5
	Interrupts	<Ready> <MonsterGroup> <ReactIRQ> <HitIRQ> <Combat> <WepCombat> <TwoHandCombat> <PostMoveCycle> <Block>
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <Block>
##	NextMove        postReady_back
	Priority        1
	Flags		NoSizeScale

	Type		male
	  Anim          male\demo_a 1
	TEnd


	Type		fem
	  Anim          male\barf 1 60
	  PlayFX	Generic\Vomit_female.fx 25
	TEnd
#
#
#	Type		huge
#	  Anim          huge\emote_Chicken 1
#	TEnd


MEnd

###benpc	Signature_Ghost_Widow
###benpc	Signature_Black_Scorpion
###benpc	Malta_Hercules_Titan_01
###
###benpc	Arachnobot_Disruptor
###benpc	Plant_Flytrap
###
