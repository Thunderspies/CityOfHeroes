#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	Head
End

lifespan	60
########################################################

Condition
	On 	Time
	Time 	0


	Event
		Type	Local 
		At	Head
		
		Part1	:WillPower.part
		Part2	:WillPowerSmall.part
		Part3	:AbilityDots.part
		Part4	:AbilitySparklingDots.part
		Sound enrage 60 60 .88
	End

End

End
##################################

