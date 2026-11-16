#########################################################
##	Fire_Ball
##
FxInfo

Input  
	InpName	FootL
End

Input  
	InpName	FootR
End

Input  
	InpName	ULEGL
End

Input  
	InpName	ULEGR
End

Input  
	InpName	LLEGL
End

Input  
	InpName	LLEGR
End

Input  
	InpName	wepR
End

Input  
	InpName	wepL
End

Input  
	InpName	Head
End

Input  
	InpName	UARMR
End

Input  
	InpName	UARML
End

Input  
	InpName	LARML
End

Input  
	InpName	LARMR
End

Input  
	InpName	Hips
End

Input  
	InpName	chest
End

Input  
	InpName	Root
End

LifeSpan	45



Condition
	On	Time
	Time	0
	
	Event
		EName	2
		Type	Local
		At	FootR
		#part1	:Smoke.part
		part2	:RippedClothes.part
		Part3	:DarkBlastHitTendril.part
		Sound wwtransform 80 80 .9
		LifeSpan	40
	End


	Event
		EName	1
		Type	Local
		At	Chest
		#part1	:Smoke.part
		part2	:RippedClothes.part
		Part3	:DarkRays.part
		#Sound	miasmaburst 60 20 .8
		LifeSpan	40
	End

#LegL



	Event
		EName	3
		Type	Local
		At	ULEGL
		#part1	:Smoke.part
		part2	:RippedClothes.part
		Part3	:DarkBlastHitTendril.part
		#PMagnet LLEGL
		#Sound	wwtransform2 80 60 .7
		LifeSpan	40

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		#part1	:Smoke.part
		part2	:RippedClothes.part
		Part3	:DarkBlastHitTendril.part
		
		LifeSpan	40
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		#part1	:Smoke.part
		part2	:RippedClothes.part
		Part3	:DarkBlastHitTendril.part
		#Sound	rage 100 30 1.0
		LifeSpan	40
	
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		#part1	:Smoke.part
		part2	:RippedClothes.part
		Part3	:DarkBlastHitTendril.part
		LifeSpan	40

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		#part1	:Smoke.part
		part2	:RippedClothes.part
		Part3	:DarkBlastHitTendril.part
		LifeSpan	40
	End

		
###############################################################################

	Event
		EName	9
		Type	Local
		At	Head
		#part1	:Smoke.part
		part2	:RippedClothes.part
		Part3	:DarkBlastHitTendril.part
		LifeSpan	40
	End

		
##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		#part1	:Smoke.part
		part2	:RippedClothes.part
		Part3	:DarkBlastHitTendril.part
		LifeSpan	40

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		#part1	:Smoke.part
		part2	:RippedClothes.part
		Part3	:DarkBlastHitTendril.part
		LifeSpan	40
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		#part1	:Smoke.part
		part2	:RippedClothes.part
		Part3	:DarkBlastHitTendril.part
		LifeSpan	40

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		#part1	:Smoke.part
		part2	:RippedClothes.part
		Part3	:DarkBlastHitTendril.part
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
		#part1	:Smoke.part
		part2	:RippedClothes.part
		Part3	:DarkBlastHitTendril.part
		#Sound	miasma_loop 60 20 .79
		LifeSpan	45
	End

	Event
		EName	1
		Type	Local
		At	Chest
		#part1	:Smoke.part
		part2	:RippedClothes.part
		Part3	:DarkRays.part
		
		LifeSpan	20
	End


#LegL



	Event
		EName	3
		Type	Local
		At	ULEGL
		#part1	:Smoke.part
		part2	:RippedClothes.part
		Part3	:DarkBlastHitTendril.part
		LifeSpan	45
		#Sound	obsidian_loop 60 20 .49

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		#part1	:Smoke.part
		part2	:RippedClothes.part
		Part3	:DarkBlastHitTendril.part
		LifeSpan	45
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		#part1	:Smoke.part
		part2	:RippedClothes.part
		Part3	:DarkBlastHitTendril.part
		#Sound	rage 100 30 1.0
		LifeSpan	45
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		#part1	:Smoke.part
		part2	:RippedClothes.part
		Part3	:DarkBlastHitTendril.part
		LifeSpan	45

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		#part1	:Smoke.part
		part2	:RippedClothes.part
		Part3	:DarkBlastHitTendril.part
		LifeSpan	45
	End

		
###############################################################################

	Event
		EName	9
		Type	Local
		At	Head
		#part1	:Smoke.part
		part2	:RippedClothes.part
		Part3	:DarkBlastHitTendril.part
		LifeSpan	45
	End

		
##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		#part1	:Smoke.part
		part2	:RippedClothes.part
		Part3	:DarkBlastHitTendril.part
		LifeSpan	45

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		#part1	:Smoke.part
		part2	:RippedClothes.part
		Part3	:DarkBlastHitTendril.part
		LifeSpan	45
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		#part1	:Smoke.part
		part2	:RippedClothes.part
		Part3	:DarkBlastHitTendril.part
		LifeSpan	45

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		#part1	:Smoke.part
		part2	:RippedClothes.part
		Part3	:DarkBlastHitTendril.part
		LifeSpan	45
	End

End    


End	

