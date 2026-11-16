#########################################################
##	chill touch hit
FxInfo



Input  
	InpName	Hips
End

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
	InpName	HIPS
End

Input  
	InpName	CHEST
End

Input  
	InpName	Hair
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
	InpName	Root
End

##################################
Condition
	On	Time
	Time	0
	
	Event
		EName	2
		Type	Local
		At	FootR
		part1	:InvisabilityBodyGlows.part

	End


#LegL



	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	:InvisabilityMotionGlows.part
		PMagnet LLEGL

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	:InvisabilityMotionGlows.part
		PMagnet FootL
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	:InvisabilityBodyGlows.part
	
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	:InvisabilityMotionGlows.part
		PMagnet LlegR

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:InvisabilityMotionGlows.part
		PMagnet FootR
	End

		
###############################################################################

	Event
		EName	9
		Type	Local
		At	Hair
		part2	:InvisabilityHeadGlows.part
	End

##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	:InvisabilityArmMotionGlows.part
		PMagnet LArmL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	:InvisabilityArmMotionGlows.part
		PMagnet WepL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part1	:InvisabilityArmGlows.part
	
	End






	Event
		EName	13
		Type	Local
		At	UArmR
		part1	:InvisabilityArmMotionGlows.part
		PMagnet LArmR

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	:InvisabilityArmMotionGlows.part
		PMagnet WepR
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part1	:InvisabilityArmGlows.part
	
	End

End


End				