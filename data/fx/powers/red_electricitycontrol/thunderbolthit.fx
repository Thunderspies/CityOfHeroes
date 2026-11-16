#########################################################
##	EBlast
########################################################
FxInfo

Input  
	Inpname	Chest
End

LifeSpan	170

#################################################################
##Hands
#################################################################



Condition
	On	Time
	Time	5

	Event
		EName	HandGlow1
		Type	Local
		At	Chest
		Part1	:ElectricityGlow.part
		Part2	:ElectricityGlowStar.part
		Part3	:ElectricityArch4a.part
		Part4	:ElectricityArchSmalla.part
		Sound Elecballhit 80 80 .8
	End

	Event
		EName	continuingArchs
		Type	Local
		At	Chest
		Part1	:ElectricityArchContinuing.part
		Part2	:ElectricityArchContinuing2.part
	End
End

	Condition
	On 	Time
	Time 	45
	Event
		EName 	HandGlow1
		Type	Destroy
				
	End

End



End
