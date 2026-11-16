#########################################################
FxInfo


LifeSpan	50

########################################################

Condition
	On 	Time
	Time 	0
	
	Event
		EName	dummy
		Type	Local 
		At	Root
		
		Part1	:StaminaRingUp.part
	End

End

Condition
	On 	cycle
	Time 	14
	Chance	.9999


	Event
		EName	dummy
		Type	Local 
		At	Root
		
		Part1	:StaminaRingUp.part
		Lifespan 15
	End
	
	
End


Condition
	On 	cycle
	Time 	28
	Chance	.9999

	Event
		EName	dummy
		Type	Local 
		At	Root
		
		Part1	:StaminaRingUp.part
		#Part2	:IntelligenceRingUp.part
		Lifespan 15
	End


End

End
##################################

