#########################################################
##	tech_lighttentacles
##
FxInfo
	
LifeSpan 0

Input  
	InpName	Origin
End

Condition
	On 	cycle
	Time 	13
	Chance	.3

	Event
		EName 	Prime
		Type	Local 
		At	Origin
		Part	:ClockWorkElectricFence_1.part
		Part	:ClockWorkElectricFence_2.part
		LifeSpan	3
	End
End


End			