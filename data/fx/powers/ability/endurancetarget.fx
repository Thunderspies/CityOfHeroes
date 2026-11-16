#########################################################
##	Fireweapon
########################################################
FxInfo


########################################################

LifeSpan	45

Condition
	On 	Time
	Time 	0
	
	Event
		EName	dummy
		Type	Local 
		At	chest
		
		Part1	:IntelligenceRingTarget.part
		Part2	Powers/Healing/Endurance02.part
	End

End

Condition
	On 	cycle
	Time 	14
	Chance	1


	Event
		EName	dummy
		Type	Local 
		At	chest
		Part1	:IntelligenceRingTarget.part
		#Part2	Powers/Healing/Endurance02.part
		Lifespan 20
	End
	
	
End



Condition
	On 	cycle
	Time 	28
	Chance	1

	Event
		EName	dummy
		Type	Local 
		At	chest
		Part1	:IntelligenceRingTarget.part
		#Part2	Powers/Healing/Endurance02.part
		Lifespan 20
	End

End

End
##################################
