#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	45


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

Input  
	InpName	chest
End

#####################################################################################
##Flens Flares
#####################################################

Condition
	On 	time
	Time 	5
		
	Event
		EName	rings
		Type	Local
		At	chest
		part1	:PowerBoostRing.part
		part	:PowerBoostRingMulti.part

	End
	
End

Condition
	On 	time
	Time 	20
		
	Event
		EName	rings
		Type	Local
		At	chest
		part1	:PowerBoostRing.part
		part	:PowerBoostRingMulti.part


	End
	
End

Condition
	On 	time
	Time 	25
		
	Event
		EName	rings
		Type	Local
		At	chest
		part1	:PowerBoostRingFaint.part
		


	End
	
End

Condition
	On 	cycle
	Time 	15
	Chance	.3
	
	Event
		EName	3
		Type	Local
		At	UarmL
		part1	:FortitudeRaysSmall.part
		part2	:FortitudeRing.part
		part3	:FortitudeRing.part
		part4	:FortitudeDots.part

		LifeSpan	30
	End
	
End


Condition
	On 	cycle
	Time 	20
	Chance	.3
	
	Event
		EName	3
		Type	Local
		At	UarmR
		part1	:FortitudeRaysSmall.part
		part2	:FortitudeRing.part
		part3	:FortitudeRing.part
		part4	:FortitudeDots.part

		LifeSpan	30
	End
	

End

Condition
	On 	cycle
	Time 	20
	Chance	.3

	Event
		EName	3
		Type	Local
		At	LlegL
		part1	:FortitudeRaysSmall.part
		part2	:FortitudeRing.part
		part3	:FortitudeRing.part
		part4	:FortitudeDots.part

		LifeSpan	30
	End

End

Condition
	On 	cycle
	Time 	15
	Chance	.3

	Event
		EName	3
		Type	Local
		At	LlegR
		part1	:FortitudeRaysSmall.part
		part2	:FortitudeRing.part
		part3	:FortitudeRing.part
		part4	:FortitudeDots.part

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


			