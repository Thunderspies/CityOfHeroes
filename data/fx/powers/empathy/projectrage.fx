#########################################################
##	Fire_Ball
##
FxInfo



LifeSpan	165


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

#####################################################################################



Condition
	On	Time
	Time	20
	
	Event
		EName	2
		Type	Local
		At	FootR
		part1	Powers/Empathy/EmpathyBodyGlows.part
		#part2	Powers/Empathy/EmpathyBodyGlowsFlat.part
	End


#LegL



	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	Powers/Empathy/EmpathyMotionGlows.part
		PMagnet Knee1

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	Powers/Empathy/EmpathyArmMotionGlows.part
		part2	Powers/Empathy/EmpathyMotionGlows.part
		PMagnet FootL
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	Powers/Empathy/EmpathyBodyGlows.part
		#part2	Powers/Empathy/EmpathyBodyGlowsFlat.part
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	Powers/Empathy/EmpathyMotionGlows.part
		PMagnet Kneer

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	Powers/Empathy/EmpathyArmMotionGlows.part
		part2	Powers/Empathy/EmpathyMotionGlows.part
		PMagnet FootR
	End

###############################################################################

	Event
		EName	9
		Type	Local
		At	Head
		part1	Powers/Empathy/EmpathyArmMotionGlows.part
		part2	Powers/Empathy/EmpathyHeadGlows.part

	End

##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	Powers/Empathy/EmpathyArmMotionGlows.part
		part2	Powers/Empathy/EmpathyArmMotionGlows2.part
		PMagnet ElbowL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	Powers/Empathy/EmpathyArmMotionGlows.part
		part2	Powers/Empathy/EmpathyArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part1	Powers/Empathy/EmpathyArmGlows.part
	
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	Powers/Empathy/EmpathyArmMotionGlows.part
		part2	Powers/Empathy/EmpathyArmMotionGlows2.part
		PMagnet ElbowL

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	Powers/Empathy/EmpathyArmMotionGlows.part
		part2	Powers/Empathy/EmpathyArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part1	Powers/Empathy/EmpathyArmGlows.part
	
	End

End



End	


			