#########################################################
##	TemperatureControl
########################################################

FxInfo

#LifeSpan	100

Input  
	Inpname	origin
End

##########################
Condition
	On 	Time
	Time 	0

	


	Event
		EName	core2
		Type	Local
		At	origin
		
		
		part1	:SmallExpandingRingRed.part
		Sound targeting_loop 100 100 .2
		
		
	End

End


End