#########################################################
##	Fire_Ball
##
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


Condition
	On 	Time
	Time 	15

	Event
		EName	Burst
		Type	Local 
		At	Hips
			
		Part1	:LightFormFlash.part
		Part2	:LightFormEmber.part
		Part3	:LightFormStreaks.part
		Part4	:LightFormRays.part
		Part5	:LightFormRing.part

	End
End


Condition
	On 	Time
	Time 	3

	Event
		EName	rays01
		Type	Local 
		At	Root
		
		Part1	:LightRaysUp01.part
		Part2	:LightRays.part
		Part3	:LightRays.part
		Part4	:LightRays.part
		Part5	:LightRays.part
	End

End


Condition
	On 	Time
	Time 	0

	Event
		EName	rays02
		Type	Local 
		At	Root
		
		Bhvr	behaviors/Rune_spin.bhvr
		Part1	:LightRaysOut.part
		Part2	:LightRaysUp02.part
		#Part3	:LightRays.part
	End

End


Condition
	On 	Time
	Time 	20

	Event
		ENAME	rays01
		Type	Destroy

	End

	Event
		ENAME	rays02
		Type	Destroy

	End

	Event
		ENAME	Burst
		Type	Destroy

	End

End

##################################
Condition
	On	Time
	Time	15
	
	Event
		EName	2
		Type	Local
		At	FootR
		part1	:LightFormBodyGlows.part

	End


#LegL



	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	:LightFormMotionGlows.part
		PMagnet LLEGL

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	:LightFormMotionGlows.part
		PMagnet FootL
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	:LightFormBodyGlows.part
	
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	:LightFormMotionGlows.part
		PMagnet LlegR

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:LightFormMotionGlows.part
		PMagnet FootR
	End

		
###############################################################################

	Event
		EName	9
		Type	Local
		At	Hair
		part2	:LightFormHeadGlows.part
	End

##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	:LightFormArmMotionGlows.part
		PMagnet LArmL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	:LightFormArmMotionGlows.part
		PMagnet WepL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part1	:LightFormArmGlows.part
	
	End






	Event
		EName	13
		Type	Local
		At	UArmR
		part1	:LightFormArmMotionGlows.part
		PMagnet LArmR

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	:LightFormArmMotionGlows.part
		PMagnet WepR
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part1	:LightFormArmGlows.part
	
	End


End    


End	


			