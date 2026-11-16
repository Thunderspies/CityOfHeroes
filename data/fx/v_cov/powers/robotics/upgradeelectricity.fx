#########################################################
##	Fire_Ball
##
FxInfo

#####################################################################################
##Flens Flares
#####################################################


Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound CABuildup 70 70 .83
	End
End


Condition
	On	Time
	Time	13

	Event
		Type	Local
		At	Origin
		Sound chargedarmor_loop 70 70 .24
	End
End



Condition
	On 	cycle
	Time 	3
	Chance	.3
	
	Event
		EName	3
		Type	Local
		At	LarmL
		Part	Powers/ElectricityControl/ElectricityArchContinuingz.part
		Part	Powers/ElectricityControl/ElectricityArchContinuing2z.part
		Part	Powers/ElectricityControl/ElectricitySparkGlow.part
		Part	Powers/ElectricityControl/ElectricitySparkBigGlow.part
		Part	Powers/ElectricityControl/ElectricitySparkStar.part
		Part	Powers/ElectricityControl/ElectricitySparkRing.part

		lifespan	7
	End
	
End

Condition
	On 	cycle
	Time 	3
	Chance	.3
	
	Event
		EName	3
		Type	Local
		At	LarmR
		Part1	Powers/ElectricityControl/ElectricityArchContinuingz.part
		Part2	Powers/ElectricityControl/ElectricityArchContinuing2z.part
		Part	Powers/ElectricityControl/ElectricitySparkGlow.part
		Part	Powers/ElectricityControl/ElectricitySparkBigGlow.part
		Part	Powers/ElectricityControl/ElectricitySparkStar.part
		Part	Powers/ElectricityControl/ElectricitySparkRing.part

		lifespan	7
	End
	
End

Condition
	On 	cycle
	Time 	3
	Chance	.3
	
	Event
		EName	3
		Type	Local
		At	UarmL
		Part1	Powers/ElectricityControl/ElectricityArchContinuingz.part
		Part2	Powers/ElectricityControl/ElectricityArchContinuing2z.part
		Part	Powers/ElectricityControl/ElectricitySparkGlow.part
		Part	Powers/ElectricityControl/ElectricitySparkBigGlow.part
		Part	Powers/ElectricityControl/ElectricitySparkStar.part
		Part	Powers/ElectricityControl/ElectricitySparkRing.part

		lifespan	7
	End
	
End


Condition
	On 	cycle
	Time 	3
	Chance	.3
	
	Event
		EName	3
		Type	Local
		At	UarmR
		Part1	Powers/ElectricityControl/ElectricityArchContinuingz.part
		Part2	Powers/ElectricityControl/ElectricityArchContinuing2z.part
		Part	Powers/ElectricityControl/ElectricitySparkGlow.part
		Part	Powers/ElectricityControl/ElectricitySparkBigGlow.part
		Part	Powers/ElectricityControl/ElectricitySparkStar.part
		Part	Powers/ElectricityControl/ElectricitySparkRing.part

		lifespan	7
	End
	

End

Condition
	On 	cycle
	Time 	3
	Chance	.3

	Event
		EName	3
		Type	Local
		At	LlegL
		Part1	Powers/ElectricityControl/ElectricityArchContinuingz.part
		Part2	Powers/ElectricityControl/ElectricityArchContinuing2z.part
		Part	Powers/ElectricityControl/ElectricitySparkGlow.part
		Part	Powers/ElectricityControl/ElectricitySparkBigGlow.part
		Part	Powers/ElectricityControl/ElectricitySparkStar.part
		Part	Powers/ElectricityControl/ElectricitySparkRing.part

		lifespan	7
	End

End

Condition
	On 	cycle
	Time 	3
	Chance	.3

	Event
		EName	3
		Type	Local
		At	LlegR
		Part1	Powers/ElectricityControl/ElectricityArchContinuingz.part
		Part2	Powers/ElectricityControl/ElectricityArchContinuing2z.part
		Part	Powers/ElectricityControl/ElectricitySparkGlow.part
		Part	Powers/ElectricityControl/ElectricitySparkBigGlow.part
		Part	Powers/ElectricityControl/ElectricitySparkStar.part
		Part	Powers/ElectricityControl/ElectricitySparkRing.part

		lifespan	7
	End


End

Condition
	On 	cycle
	Time 	3
	Chance	.3

	Event
		Ename	Ring
		Type	Local
		At	Hips
		
		Part1	Powers/ElectricityControl/ElectricityArchContinuingz.part
		Part2	Powers/ElectricityControl/ElectricityArchContinuing2z.part
		Part	Powers/ElectricityControl/ElectricitySparkGlow.part
		Part	Powers/ElectricityControl/ElectricitySparkBigGlow.part
		Part	Powers/ElectricityControl/ElectricitySparkStar.part
		Part	Powers/ElectricityControl/ElectricitySparkRing.part
	
		lifespan	7
	End

End
Condition
	
	Event
		Type	Local
		At	Hips
		
		Part1	Powers/ElectricityControl/ElectricitySparkShieldRing.part
	
	End

End


Condition
	On 	cycle
	Time 	3
	Chance	.3
	
	Event
		EName	3
		Type	Local
		At	FootL
		Part1	Powers/ElectricityControl/ElectricityArchContinuingz.part
		Part2	Powers/ElectricityControl/ElectricityArchContinuing2z.part
		Part	Powers/ElectricityControl/ElectricitySparkGlow.part
		Part	Powers/ElectricityControl/ElectricitySparkBigGlow.part
		Part	Powers/ElectricityControl/ElectricitySparkStar.part
		Part	Powers/ElectricityControl/ElectricitySparkRing.part

		lifespan	7
	End
	
End


Condition
	On 	cycle
	Time 	3
	Chance	.3
	
	Event
		EName	3
		Type	Local
		At	FootR
		Part1	Powers/ElectricityControl/ElectricityArchContinuingz.part
		Part2	Powers/ElectricityControl/ElectricityArchContinuing2z.part
		Part	Powers/ElectricityControl/ElectricitySparkGlow.part
		Part	Powers/ElectricityControl/ElectricitySparkBigGlow.part
		Part	Powers/ElectricityControl/ElectricitySparkStar.part
		Part	Powers/ElectricityControl/ElectricitySparkRing.part

		lifespan	7
	End
	

End

Condition
	On 	cycle
	Time 	3
	Chance	.3
	
	Event
		EName	3
		Type	Local
		At	WepL
		Part1	Powers/ElectricityControl/ElectricityArchContinuingz.part
		Part2	Powers/ElectricityControl/ElectricityArchContinuing2z.part
		Part	Powers/ElectricityControl/ElectricitySparkGlow.part
		Part	Powers/ElectricityControl/ElectricitySparkBigGlow.part
		Part	Powers/ElectricityControl/ElectricitySparkStar.part
		Part	Powers/ElectricityControl/ElectricitySparkRing.part

		lifespan	7
	End
	
End


Condition
	On 	cycle
	Time 	3
	Chance	.3
	
	Event
		EName	3
		Type	Local
		At	WepR
		Part1	Powers/ElectricityControl/ElectricityArchContinuingz.part
		Part2	Powers/ElectricityControl/ElectricityArchContinuing2z.part
		Part	Powers/ElectricityControl/ElectricitySparkGlow.part
		Part	Powers/ElectricityControl/ElectricitySparkBigGlow.part
		Part	Powers/ElectricityControl/ElectricitySparkStar.part
		Part	Powers/ElectricityControl/ElectricitySparkRing.part

		lifespan	7
	End

End

Condition
	On 	cycle
	Time 	3
	Chance	.3
	
	Event
		EName	3
		Type	Local
		At	Head
		Part1	Powers/ElectricityControl/ElectricityArchContinuingz.part
		Part2	Powers/ElectricityControl/ElectricityArchContinuing2z.part
		Part	Powers/ElectricityControl/ElectricitySparkGlow.part
		Part	Powers/ElectricityControl/ElectricitySparkBigGlow.part
		Part	Powers/ElectricityControl/ElectricitySparkStar.part
		Part	Powers/ElectricityControl/ElectricitySparkRing.part

		lifespan	7
	End
	

End

End	


			