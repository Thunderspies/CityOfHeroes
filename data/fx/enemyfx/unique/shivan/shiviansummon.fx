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
		At	T_Chest
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
		At	T_ULEGL
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		#PMagnet LLEGL
		#Sound	wwtransform2 80 60 .7
		LifeSpan	40

	End

	Event
		EName	4
		Type	Local
		At	T_LLEGL
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		
		LifeSpan	40
	End

	Event
		EName	5		
		Type	Local
		At	T_FootL
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		#Sound	rage 100 30 1.0
		LifeSpan	40
	
	End



	Event
		EName	6
		Type	Local
		At	T_ULEGR
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		LifeSpan	40

	End


#LegR


	Event
		EName	7
		Type	Local
		At	T_LLEGR
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		LifeSpan	40
	End

		
###############################################################################

	Event
		EName	9
		Type	Local
		At	T_Head
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
		At	T_UArmL
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		LifeSpan	40

	End

	Event
		EName	11
		Type	Local
		At	T_LArmL
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		LifeSpan	40
	End

	Event
		EName	13
		Type	Local
		At	T_UArmR
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		LifeSpan	40

	End

	Event
		EName	14
		Type	Local
		At	T_LArmR
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
		At	T_FootR
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		#Sound	miasma_loop 60 20 .79
		LifeSpan	45
	End

	Event
		EName	1
		Type	Local
		At	T_Chest
		part2	:RippedClothesSummon.part
		#Part3	:DarkRays.part
		
		LifeSpan	20
	End


#LegL



	Event
		EName	3
		Type	Local
		At	T_ULEGL
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		LifeSpan	45
		#Sound	obsidian_loop 60 20 .49

	End

	Event
		EName	4
		Type	Local
		At	T_LLEGL
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		LifeSpan	45
	End

	Event
		EName	5		
		Type	Local
		At	T_FootL
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		#Sound	rage 100 30 1.0
		LifeSpan	45
	End



	Event
		EName	6
		Type	Local
		At	T_ULEGR
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		LifeSpan	45

	End


#LegR


	Event
		EName	7
		Type	Local
		At	T_LLEGR
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		LifeSpan	45
	End

		
###############################################################################

	Event
		EName	9
		Type	Local
		At	T_Head
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
		At	T_UArmL
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		LifeSpan	45

	End

	Event
		EName	11
		Type	Local
		At	T_LArmL
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		LifeSpan	45
	End

	Event
		EName	13
		Type	Local
		At	T_UArmR
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		LifeSpan	45

	End

	Event
		EName	14
		Type	Local
		At	T_LArmR
		part2	:RippedClothesSummon.part
		Part3	:DarkBlastHitTendrilSummon.part
		LifeSpan	45
	End

End    


End	

