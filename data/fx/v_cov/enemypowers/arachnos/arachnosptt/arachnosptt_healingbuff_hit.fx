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


Condition
	On 	Time
	Time 	0

	Event
		ENAME   Tendrills
		Type	Local 
		At	chest
		Part3	Powers/DarknessControl/DamageHitMist2.part
		Part1	Powers/DarknessControl/SoulDrainHitTendril2.part
		Part2	Powers/DarknessControl/SoulDrainHitPuddle2.part
		Part4	Powers/DarknessControl/RedRays.part
		SOund	dclifedrain2 80 33 1.0
		LifeSpan	30
	End

End

Condition
	On	Time
	Time	10
	
	Event
		EName	Ring
		Type	Local
		At	T_Root
		
		Sound heal1 70 70 .88
		
	End


	Event
		EName	1
		Type	Local
		At	T_Hips
		part1	Powers/Healing/Healing02.part
		
		
	End

	Event
		EName	2
		Type	Local
		At	T_FootR
		part1	Powers/Healing/HealingBodyGlows.part
		part2	Powers/Healing/HealingBodyGlowsFlat.part
		
	End


#LegL



	Event
		EName	3
		Type	Local
		At	T_ULEGL
		part1	Powers/Healing/HealingMotionGlows.part
		PMagnet Knee1

	End

	Event
		EName	4
		Type	Local
		At	T_LLEGL
		part1	Powers/Healing/HealingMotionGlows.part
		PMagnet FootL
	End

	Event
		EName	5		
		Type	Local
		At	T_FootL
		part1	Powers/Healing/HealingBodyGlows.part
		part2	Powers/Healing/HealingBodyGlowsFlat.part
	End



	Event
		EName	6
		Type	Local
		At	T_ULEGR
		part1	Powers/Healing/HealingMotionGlows.part
		PMagnet Kneer

	End


#LegR


	Event
		EName	7
		Type	Local
		At	T_LLEGR
		part1	Powers/Healing/HealingMotionGlows.part
		PMagnet FootR
	End

###############################################################################

	Event
		EName	9
		Type	Local
		At	T_Hair
		part2	Powers/Healing/HealingHeadGlows.part
	End

##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	T_UArmL
		part1	Powers/Healing/HealingArmMotionGlows.part
		part2	Powers/Healing/HealingArmMotionGlows2.part
		PMagnet ElbowL

	End

	Event
		EName	11
		Type	Local
		At	T_LArmL
		part1	Powers/Healing/HealingArmMotionGlows.part
		part2	Powers/Healing/HealingArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	12
		Type	Local
		At	T_WepL
		part1	Powers/Healing/HealingArmGlows.part
	
	End

	Event
		EName	13
		Type	Local
		At	T_UArmR
		part1	Powers/Healing/HealingArmMotionGlows.part
		part2	Powers/Healing/HealingArmMotionGlows2.part
		PMagnet ElbowL

	End

	Event
		EName	14
		Type	Local
		At	T_LArmR
		part1	Powers/Healing/HealingArmMotionGlows.part
		part2	Powers/Healing/HealingArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	15
		Type	Local
		At	T_WepR
		part1	Powers/Healing/HealingArmGlows.part
	
	End

End


Condition
	On	Time
	Time	40
	
	Event
		EName	all
		Type	Destroy
				
	End

End



#####################################################################################
##Flens Flares
#####################################################

Condition
	On 	cycle
	Time 	64
	Chance	.4
	
	Event
		EName	3
		Type	Local
		At	UarmL
		part1	Powers/Empathy/FortitudeRaysSmall.part
		part2	Powers/Empathy/FortitudeRing.part
		part3	Powers/Empathy/FortitudeRing.part
		part4	Powers/Empathy/FortitudeDots.part

		LifeSpan	30
	End
	
End


Condition
	On 	cycle
	Time 	70
	Chance	.4
	
	Event
		EName	3
		Type	Local
		At	UarmR
		part1	Powers/Empathy/FortitudeRaysSmall.part
		part2	Powers/Empathy/FortitudeRing.part
		part3	Powers/Empathy/FortitudeRing.part
		part4	Powers/Empathy/FortitudeDots.part
		LifeSpan	30
	End
	

End

Condition
	On 	cycle
	Time 	75
	Chance	.4

	Event
		EName	3
		Type	Local
		At	LlegL
		part1	Powers/Empathy/FortitudeRaysSmall.part
		part2	Powers/Empathy/FortitudeRing.part
		part3	Powers/Empathy/FortitudeRing.part
		part4	Powers/Empathy/FortitudeDots.part
		LifeSpan	30
	End

End

Condition
	On 	cycle
	Time 	83
	Chance	.4

	Event
		EName	3
		Type	Local
		At	LlegR
		part1	Powers/Empathy/FortitudeRaysSmall.part
		part2	Powers/Empathy/FortitudeRing.part
		part3	Powers/Empathy/FortitudeRing.part
		part4	Powers/Empathy/FortitudeDots.part
		LifeSpan	30
	End


End

#####################################################################################

Condition
	On	Time
	Time	0
	
	Event
		EName	3
		Type	Local
		At	Hips
		part1	Powers/Empathy/ArmMotionGlowsx.part
		
	End

	Event
		EName	3
		Type	Local
		At	Hips
		part1	Powers/Empathy/ArmMotionGlows2x.part
		PMagnet	UlegL
	End

	Event
		EName	3
		Type	Local
		At	Hips
		part1	Powers/Empathy/ArmMotionGlows2x.part
		PMagnet	UlegR
	End

	Event
		EName	3
		Type	Local
		At	LlegL
		part1	Powers/Empathy/ArmMotionGlowsx.part
		PMagnet	ULEGL

	End

	Event
		EName	6
		Type	Local
		At	LlegR
		part1	Powers/Empathy/ArmMotionGlowsx.part
		PMagnet	ULEGR


	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	Powers/Empathy/ArmMotionGlowsx.part
		PMagnet	FootL

	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	Powers/Empathy/ArmMotionGlowsx.part
		PMagnet	FootR

	End




	Event
		EName	2
		Type	Local
		At	FootR
		part1	Powers/Empathy/ArmMotionGlows2x.part
		
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	Powers/Empathy/ArmMotionGlows2x.part
		
	End

####################################################################################
##Original

#####################################################################################
	
	Event
		EName	2
		Type	Local
		At	FootR
		part1	Powers/Empathy/ArmMotionGlows2.part
		PMagnet LlegR
	End


	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	Powers/Empathy/ArmMotionGlows2.part
		

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	Powers/Empathy/ArmMotionGlows2.part
		
		PMagnet UlegL
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	Powers/Empathy/ArmMotionGlows2.part
		PMagnet LlegL
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	Powers/Empathy/ArmMotionGlows2.part
		

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	Powers/Empathy/ArmMotionGlows2.part
		
		PMagnet UlegL
	End

###############################################################################

	Event
		EName	9
		Type	Local
		At	Head
		part1	Powers/Empathy/ArmMotionGlows2.part
		
	End

##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	Powers/Empathy/ArmMotionGlows2.part
		
		#PMagnet ElbowL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	Powers/Empathy/ArmMotionGlows2.part
		
		#PMagnet HandL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part1	Powers/Empathy/ArmMotionGlows2.part
	
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	Powers/Empathy/ArmMotionGlows2.part
		
		#PMagnet ElbowL

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	Powers/Empathy/ArmMotionGlows2.part
		
		#PMagnet HandL
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part1	Powers/Empathy/ArmMotionGlows2.part
	
	End

End


End