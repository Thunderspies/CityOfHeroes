#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	Head
End

#lifespan	60
########################################################

Condition
	On 	cycle
	Time 	20


	Event
		Type	Local 
		At	Head
		
		Part1	:WillPower.part
		Part2	:WillPowerSmall.part
		Part3	:AbilityDots.part
		lifespan	25
	End

End

End
##################################

