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
	On time
	Time 0

	Event

		Type	Local
		At	Chest
		Sound aim5 60 60 .6
		End
end



Condition
	On	Time
	Time	10
	
	Event
		EName	AuraRings
		Type	Local
		At	Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part	:HalfEnergyRing.part
		part	:HalfEnergyRing2.part
		part	:HalfEnergyRingUp.part
		part	:LightRaysThin.part
		Lifespan	20

	End

End

Condition
	On 	cycle
	Time 	14
	Chance	.4
	
	Event
		EName	3
		Type	Local
		At	UarmL
		part1	:FortitudeRaysSmall.part
		part2	:FortitudeRing.part
		part3	:FortitudeRing.part
		part4	:FortitudeDots.part
		lifeSpan	30
	End
	
End


Condition
	On 	cycle
	Time 	10
	Chance	.4
	
	Event
		EName	3
		Type	Local
		At	UarmR
		part1	:FortitudeRaysSmall.part
		part2	:FortitudeRing.part
		part3	:FortitudeRing.part
		part4	:FortitudeDots.part
		lifeSpan	30
	End
	

End

Condition
	On 	cycle
	Time 	15
	Chance	.4

	Event
		EName	3
		Type	Local
		At	LlegL
		part1	:FortitudeRaysSmall.part
		part2	:FortitudeRing.part
		part3	:FortitudeRing.part
		part4	:FortitudeDots.part
		lifeSpan	30
	End

End

Condition
	On 	cycle
	Time 	10
	Chance	.4

	Event
		EName	3
		Type	Local
		At	LlegR
		part1	:FortitudeRaysSmall.part
		part2	:FortitudeRing.part
		part3	:FortitudeRing.part
		part4	:FortitudeDots.part
		lifeSpan	30
	End


End

Condition
	On	Time
	Time	5
	
	Event
		EName	2
		Type	Local
		At	FootR
		part1	Powers/Empathy/RangeBoostBodyGlows.part
		
	End


#LegL



	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	Powers/Empathy/RangeBoostMotionGlows.part
		PMagnet Knee1

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	Powers/Empathy/RangeBoostArmMotionGlows.part
		part2	Powers/Empathy/RangeBoostMotionGlows.part
		PMagnet FootL
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	Powers/Empathy/RangeBoostBodyGlows.part
		
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	Powers/Empathy/RangeBoostMotionGlows.part
		PMagnet Kneer

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	Powers/Empathy/RangeBoostArmMotionGlows.part
		part2	Powers/Empathy/RangeBoostMotionGlows.part
		PMagnet FootR
	End

###############################################################################

	Event
		EName	9
		Type	Local
		At	Head
		part1	Powers/Empathy/RangeBoostArmMotionGlows.part
		part2	Powers/Empathy/RangeBoostBodyGlows.part

	End

##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	Powers/Empathy/RangeBoostArmMotionGlows.part
		part2	Powers/Empathy/RangeBoostArmMotionGlows2.part
		PMagnet ElbowL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	Powers/Empathy/RangeBoostArmMotionGlows.part
		part2	Powers/Empathy/RangeBoostArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part1	Powers/Empathy/RangeBoostArmGlows.part
	
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	Powers/Empathy/RangeBoostArmMotionGlows.part
		part2	Powers/Empathy/RangeBoostArmMotionGlows2.part
		PMagnet ElbowL

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	Powers/Empathy/RangeBoostArmMotionGlows.part
		part2	Powers/Empathy/RangeBoostArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part1	Powers/Empathy/RangeBoostArmGlows.part
	
	End

End



End	


			