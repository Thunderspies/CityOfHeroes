#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	chest
End

Input  
	Inpname	root
End

########################################################

LifeSpan	50

Condition
	On 	Time
	Time 	0
	
	Event
		EName	dummy
		Type	Local 
		At	chest
		
		Part1	:AbsorbPainRings.part
		Part2	Powers/Healing/Healing02.part
		sound	steal3 80 30 .7
	End

End

Condition
	On 	Time
	Time 	43
	
	Event
		EName	dummy2
		Type	Local 
		At	Root
		Geom	RootToChestAdjustment
	End

	Event
		
		Type	start 
		At	dummy2
		altpiv	1
		Part1	:AbsorbPainRingFinal.part
	End


End

Condition
	On 	cycle
	Time 	14
	Chance	1


	Event
		EName	dummy3
		Type	Local 
		At	chest
		Part1	:AbsorbPainRing1.part
		LifeSpan	14
	End
	
	
End

Condition
	On 	cycle
	Time 	28
	Chance	1

	Event
		EName	dummy4
		Type	Local 
		At	chest
		Part1	:AbsorbPainRing1.part
		LifeSpan	14
	End


End

Condition
	On 	Time
	Time 	45
	
	Event
		EName	dummy
		Type	Destroy
		
	End

	Event
		EName	dummy3
		Type	Destroy
		
	End

	Event
		EName	dummy4
		Type	Destroy
		
	End
	
End



End
##################################

