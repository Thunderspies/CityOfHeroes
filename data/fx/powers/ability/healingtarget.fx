#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	chest
End

########################################################

LifeSpan	45

Condition
	On 	Time
	Time 	0
	
	Event
		EName	dummy
		Type	Local 
		At	chest
		
		Part1	:StaminaRingTarget.part
		Part2	Powers/Healing/Healing02.part
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
		Part1	:StaminaRingTarget.part
		#Part2	Powers/Healing/Healing02.part
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
		Part1	:StaminaRingTarget.part
		#Part2	Powers/Healing/Healing02.part
		Lifespan 20
	End


End


End
##################################

