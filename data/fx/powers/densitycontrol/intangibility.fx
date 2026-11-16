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

##################################
Condition
	On	Time
	Time	0
	
	Event
		Type	Local
		At	Hips
		part1	:Intangibility01.part
		
	End

	Event
		Type	Local
		At	FootR
		part1	:IntangibilityBodyGlows.part
		
	End


#LegL



	Event
		Type	Local
		At	ULEGL
		part1	:IntangibilityMotionGlows.part
		PMagnet Knee1

	End

	Event
		Ename	Knee1
		Type	Local
		At	LLEGL
		part1	:IntangibilityMotionGlows.part
		PMagnet FootL
	End

	Event
		ENAME   FootL

		Type	Local
		At	FootL
		part1	:IntangibilityBodyGlows.part
		
	End



	Event
		Type	Local
		At	ULEGR
		part1	:IntangibilityMotionGlows.part
		PMagnet Kneer

	End


#LegR


	Event
		Ename	Kneer
		Type	Local
		At	LLEGR
		part1	:IntangibilityMotionGlows.part
		PMagnet FootR
	End

	
###############################################################################

	Event
		ENAME   FootR

		Type	Local
		At	Hair
		part2	:IntangibilityHeadGlows.part
	End

##########################################################################
##Arm
##################################################################

	Event
		Type	Local
		At	UArmL
		part1	:IntangibilityArmMotionGlows.part
		PMagnet ElbowL

	End

	Event
		Ename	ElbowL
		Type	Local
		At	LArmL
		part1	:IntangibilityArmMotionGlows.part
		PMagnet HandL
	End

	Event
		ENAME   HandL
		Type	Local
		At	WepL
		part1	:IntangibilityArmGlows.part
	
	End


	Event
		Type	Local
		At	UArmR
		part1	:IntangibilityArmMotionGlows.part
		PMagnet ElbowR

	End

	Event
		Ename	ElbowR
		Type	Local
		At	LArmR
		part1	:IntangibilityArmMotionGlows.part
		PMagnet HandR
	End

	Event
		ENAME   HandR
		Type	Local
		At	WepR
		part1	:IntangibilityArmGlows.part
	
	End



End

End				