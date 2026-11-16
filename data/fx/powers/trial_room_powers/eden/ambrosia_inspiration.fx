#########################################################
##	Fire_Ball
##
FxInfo

Condition
	On 	cycle
	Time 	20
	Chance	.2
	
	Event
		EName	3
		Type	Local
		At	LarmL
		part1	:FortitudeRaysSmall2.part
		part2	:FortitudeRing2.part
		part3	:FortitudeRing2.part
		part4	:FortitudeDots2.part

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
		part1	:FortitudeRaysSmall2.part
		part2	:FortitudeRing2.part
		part3	:FortitudeRing2.part
		part4	:FortitudeDots2.part

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
		part1	:FortitudeRaysSmall2.part
		part2	:FortitudeRing2.part
		part3	:FortitudeRing2.part
		part4	:FortitudeDots2.part

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
		part1	:FortitudeRaysSmall2.part
		part2	:FortitudeRing2.part
		part3	:FortitudeRing2.part
		part4	:FortitudeDots2.part

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
		part1	:FortitudeRaysSmall2.part
		part2	:FortitudeRing2.part
		part3	:FortitudeRing2.part
		part4	:FortitudeDots2.part

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
		part1	:FortitudeRaysSmall2.part
		part2	:FortitudeRing2.part
		part3	:FortitudeRing2.part
		part4	:FortitudeDots2.part

		lifespan	25
	End


End

Condition
	On 	Time
	Time 	0


	Event
		Type	Local 
		At	Head
		
		Part1	:Strength.part
		Part2	:StrengthSmall.part
		Part3	:AbilityDots.part
		Part4	:AbilitySparklingDots.part
		Sound stimulate1 60 60 .45
	End

End

Condition
	On 	Time
	Time 	0


	Event
		Type	Local 
		At	Origin
		
		#Sound	Strength 60 20 .6
		Sound syndrone 60 60 .6
	End

End
End	


			