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
	Time	15
	
	Event
		EName	Ring
		Type	Local
		At	Root
		part1	Powers/Ability/StaminaRingUp.part
		Sound heal1 70 70 .8
		
	End

End

Condition
	On	Time
	Time	20
	
	Event
		EName	Ring
		Type	Local
		At	Root
		part1	Powers/Ability/StaminaRingUp.part
		
	End

End

Condition
	On	Time
	Time	35
	
	Event
		EName	Ring
		Type	Local
		At	Root
		part1	Powers/Ability/StaminaRingUp.part
		
	End

End

Condition
	On	Time
	Time	18
	
	Event
		EName	2
		Type	Local
		At	FootR
		part1	:HealingBodyGlows.part
		part2	:HealingBodyGlowsFlat.part
	End


#LegL



	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	:HealingMotionGlows.part
		PMagnet Knee1

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	:HealingMotionGlows.part
		PMagnet FootL
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	:HealingBodyGlows.part
		part2	:HealingBodyGlowsFlat.part
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	:HealingMotionGlows.part
		PMagnet Kneer

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:HealingMotionGlows.part
		PMagnet FootR
	End

###############################################################################

	Event
		EName	9
		Type	Local
		At	Hair
		part2	:HealingHeadGlows.part
	End

##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	:HealingArmMotionGlows.part
		part2	:HealingArmMotionGlows2.part
		PMagnet ElbowL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	:HealingArmMotionGlows.part
		part2	:HealingArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part1	:HealingArmGlows.part
	
	End


	Event
		EName	13
		Type	Local
		At	UArmR
		part1	:HealingArmMotionGlows.part
		part2	:HealingArmMotionGlows2.part
		PMagnet ElbowL

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	:HealingArmMotionGlows.part
		part2	:HealingArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part1	:HealingArmGlows.part
	
	End



End


End				