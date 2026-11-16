#########################################################
##	
##
FxInfo

Input  
	InpName	CHEST
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
	InpName	LLEGL
End

Input  
	InpName	LLEGR
End

Input  
	InpName	FootL
End

Input  
	InpName	FootR
End

Input  
	InpName	Head
End

#####################################################################################
##Lens Flares
#####################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		Sound red_loop 60 60 .5
	End

End



Condition
	On 	cycle
	Time 	20
	Chance	.2
	
	Event
		EName	3
		Type	Local
		At	Chest
		part1	Powers/Empathy/FortitudeRaysSmall.part
		part2	Powers/Empathy/FortitudeRing.part
		part3	Powers/Empathy/FortitudeRing.part
		part4	Powers/Empathy/FortitudeDots.part

		lifespan	25
	End
	
End

Condition
	On 	cycle
	Time 	24
	Chance	.2
	
	Event
		EName	3
		Type	Local
		At	FootL
		part1	Powers/Empathy/FortitudeRaysSmall.part
		part2	Powers/Empathy/FortitudeRing.part
		part3	Powers/Empathy/FortitudeRing.part
		part4	Powers/Empathy/FortitudeDots.part

		lifespan	25
	End
	
End

Condition
	On 	cycle
	Time 	22
	Chance	.2
	
	Event
		EName	3
		Type	Local
		At	FootR
		part1	Powers/Empathy/FortitudeRaysSmall.part
		part2	Powers/Empathy/FortitudeRing.part
		part3	Powers/Empathy/FortitudeRing.part
		part4	Powers/Empathy/FortitudeDots.part

		lifespan	25
	End
	
End

Condition
	On 	cycle
	Time 	20
	Chance	.2
	
	Event
		EName	3
		Type	Local
		At	LarmL
		part1	Powers/Empathy/FortitudeRaysSmall.part
		part2	Powers/Empathy/FortitudeRing.part
		part3	Powers/Empathy/FortitudeRing.part
		part4	Powers/Empathy/FortitudeDots.part

		lifespan	25
	End
	
End

Condition
	On 	cycle
	Time 	19
	Chance	.2
	
	Event
		EName	3
		Type	Local
		At	LarmR
		part1	Powers/Empathy/FortitudeRaysSmall.part
		part2	Powers/Empathy/FortitudeRing.part
		part3	Powers/Empathy/FortitudeRing.part
		part4	Powers/Empathy/FortitudeDots.part

		lifespan	25
	End
	
End

Condition
	On 	cycle
	Time 	21
	Chance	.2
	
	Event
		EName	3
		Type	Local
		At	UarmL
		part1	Powers/Empathy/FortitudeRaysSmall.part
		part2	Powers/Empathy/FortitudeRing.part
		part3	Powers/Empathy/FortitudeRing.part
		part4	Powers/Empathy/FortitudeDots.part

		lifespan	25
	End
	
End


Condition
	On 	cycle
	Time 	22
	Chance	.2
	
	Event
		EName	3
		Type	Local
		At	UarmR
		part1	Powers/Empathy/FortitudeRaysSmall.part
		part2	Powers/Empathy/FortitudeRing.part
		part3	Powers/Empathy/FortitudeRing.part
		part4	Powers/Empathy/FortitudeDots.part

		lifespan	25
	End
	

End

Condition
	On 	cycle
	Time 	17
	Chance	.2

	Event
		EName	3
		Type	Local
		At	Head
		part1	Powers/Empathy/FortitudeRaysSmall.part
		part2	Powers/Empathy/FortitudeRing.part
		part3	Powers/Empathy/FortitudeRing.part
		part4	Powers/Empathy/FortitudeDots.part

		lifespan	25
	End

End

Condition
	On 	cycle
	Time 	17
	Chance	.2

	Event
		EName	3
		Type	Local
		At	LlegL
		part1	Powers/Empathy/FortitudeRaysSmall.part
		part2	Powers/Empathy/FortitudeRing.part
		part3	Powers/Empathy/FortitudeRing.part
		part4	Powers/Empathy/FortitudeDots.part

		lifespan	25
	End

End

Condition
	On 	cycle
	Time 	18
	Chance	.2

	Event
		EName	3
		Type	Local
		At	LlegR
		part1	Powers/Empathy/FortitudeRaysSmall.part
		part2	Powers/Empathy/FortitudeRing.part
		part3	Powers/Empathy/FortitudeRing.part
		part4	Powers/Empathy/FortitudeDots.part

		lifespan	25
	End


End

Condition
	On 	triggerbits
	Triggerbits	death

	Event
		Ename	All
		Type	Destroy
				
	End

	
End

End
#####################################################################################
	