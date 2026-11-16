#########################################################
##	endurance
##
FxInfo

LifeSpan	100

Name	endurance

Input  
	Inpname	HANDL
End

Input  
	Inpname	HANDR
End

Input  
	Inpname	CHEST
End


###############################

Condition
	On 	Time
	Time 	10

	Event
		EName 	Prime
		Type	Local 
		At	HANDL
		Part1	:hitpointflame.part

	End
		Event
		Type	Local
		At	HANDR
		Part1	:hitpointflame.part

	End


End	
Condition
	On 	Time
	Time 	30

	Event
		EName 	chest
		Type	Local 
		At	CHEST
		Part1	:hitpointflame.part

	End


End	
End		