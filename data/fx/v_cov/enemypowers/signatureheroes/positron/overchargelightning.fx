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
		#Sound	CABuildup 70 60 .83
	End
End


Condition
	On	Time
	Time	13

	Event
		Type	Local
		At	Origin
		#Sound	chargedarmor_loop 70 60 .24
	End
End

Condition
	On 	cycle
	Time 	5
	Chance	.1
	
	Event
		EName	3
		Type	Local
		At	LarmL
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		Part	:LightningArcs.part

		lifespan	7
	End
	
End

Condition
	On 	cycle
	Time 	5
	Chance	.1
	
	Event
		EName	3
		Type	Local
		At	LarmR
		Part1	:ElectricityArchContinuingz.part
		Part2	:ElectricityArchContinuing2z.part
		Part	:LightningArcs.part

		lifespan	7
	End
	
End

Condition
	On 	cycle
	Time 	5
	Chance	.1
	
	Event
		EName	3
		Type	Local
		At	UarmL
		Part1	:ElectricityArchContinuingz.part
		Part2	:ElectricityArchContinuing2z.part
		Part	:LightningArcs.part

		lifespan	7
	End
	
End


Condition
	On 	cycle
	Time 	5
	Chance	.1
	
	Event
		EName	3
		Type	Local
		At	UarmR
		Part1	:ElectricityArchContinuingz.part
		Part2	:ElectricityArchContinuing2z.part
		Part	:LightningArcs.part

		lifespan	7
	End
	

End

Condition
	On 	cycle
	Time 	5
	Chance	.1

	Event
		EName	3
		Type	Local
		At	LlegL
		Part1	:ElectricityArchContinuingz.part
		Part2	:ElectricityArchContinuing2z.part
		Part	:LightningArcs.part

		lifespan	7
	End

End

Condition
	On 	cycle
	Time 	5
	Chance	.1

	Event
		EName	3
		Type	Local
		At	LlegR
		Part1	:ElectricityArchContinuingz.part
		Part2	:ElectricityArchContinuing2z.part
		Part	:LightningArcs.part

		lifespan	7
	End


End

Condition
	On 	cycle
	Time 	5
	Chance	.2

	Event
		Ename	Ring
		Type	Local
		At	Hips
		
		Part1	:ElectricityArchContinuingz.part
		Part2	:ElectricityArchContinuing2z.part
		Part	:LightningArcs.part
	
		lifespan	7
	End

End


Condition
	On 	cycle
	Time 	5
	Chance	.1
	
	Event
		EName	3
		Type	Local
		At	FootL
		Part1	:ElectricityArchContinuingz.part
		Part2	:ElectricityArchContinuing2z.part
		Part	:LightningArcs.part

		lifespan	7
	End
	
End


Condition
	On 	cycle
	Time 	5
	Chance	.1
	
	Event
		EName	3
		Type	Local
		At	FootR
		Part1	:ElectricityArchContinuingz.part
		Part2	:ElectricityArchContinuing2z.part
		Part	:LightningArcs.part

		lifespan	7
	End
	

End

Condition
	On 	cycle
	Time 	5
	Chance	.1
	
	Event
		EName	3
		Type	Local
		At	WepL
		Part1	:ElectricityArchContinuingz.part
		Part2	:ElectricityArchContinuing2z.part
		Part	:LightningArcs.part

		lifespan	7
	End
	
End


Condition
	On 	cycle
	Time 	5
	Chance	.1
	
	Event
		EName	3
		Type	Local
		At	WepR
		Part1	:ElectricityArchContinuingz.part
		Part2	:ElectricityArchContinuing2z.part
		Part	:LightningArcs.part

		lifespan	7
	End

End

Condition
	On 	cycle
	Time 	5
	Chance	.1
	
	Event
		EName	3
		Type	Local
		At	Head
		Part1	:ElectricityArchContinuingz.part
		Part2	:ElectricityArchContinuing2z.part
		Part	:LightningArcs.part

		lifespan	7
	End
	

End

End	


			