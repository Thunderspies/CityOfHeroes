#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	45

Condition
	On	Time
	Time	0

	Event
		EName	2
		Type	Local
		At	T_FootR
		BHVR	:OffsetChest.bhvr
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		Sound darkmoonbeam2 80 80 .65
		LifeSpan	40
	End

	Event
		EName	1
		Type	Local
		At	T_hips
		BHVR	:OffsetChest.bhvr
		part1	:BlackCover.part

		LifeSpan	40
	End

	Event
		EName	1
		Type	Local
		At	T_head
		BHVR	:OffsetChest.bhvr
		part1	:BlackCover.part

		LifeSpan	40
	End

	Event
		EName	1
		Type	Local
		At	T_Root
		BHVR	:OffsetChest.bhvr
		part1	:BlackCover.part

		LifeSpan	40
	End

	Event
		EName	1
		Type	Local
		At	Chest
		BHVR	:OffsetChest.bhvr
		part1	:BlackCover.part
#		part2	:RippedClothesSummon.part
#		Part3	:DarkRays.part

		LifeSpan	40
	End

#LegL



	Event
		EName	3
		Type	Local
		At	ULEGL
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		#PMagnet LLEGL
		#Sound	wwtransform2 80 60 .7
		LifeSpan	40

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part

		LifeSpan	40
	End

	Event
		EName	5
		Type	Local
		At	FootL
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		#Sound	rage 100 30 1.0
		LifeSpan	40

	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		LifeSpan	40

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		LifeSpan	40
	End


###############################################################################

	Event
		EName	9
		Type	Local
		At	Head
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		LifeSpan	40
	End


##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		LifeSpan	40

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		LifeSpan	40
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		LifeSpan	40

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		LifeSpan	40
	End

End


####################################################################
##################################


Condition
	On	Time
	Time	6

	Event
		EName	2
		Type	Local
		At	FootR
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		#Sound	miasma_loop 60 20 .79
		LifeSpan	45
	End

	Event
		EName	1
		Type	Local
		At	Chest
		part2	:RippedClothesSummon.part
		#Part3	:DarkRays.part

		LifeSpan	20
	End


#LegL



	Event
		EName	3
		Type	Local
		At	ULEGL
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		LifeSpan	45
		#Sound	obsidian_loop 60 20 .49

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		LifeSpan	45
	End

	Event
		EName	5
		Type	Local
		At	FootL
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		#Sound	rage 100 30 1.0
		LifeSpan	45
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		LifeSpan	45

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		LifeSpan	45
	End


###############################################################################

	Event
		EName	9
		Type	Local
		At	Head
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		LifeSpan	45
	End


##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		LifeSpan	45

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		LifeSpan	45
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		LifeSpan	45

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		LifeSpan	45
	End

End


End

