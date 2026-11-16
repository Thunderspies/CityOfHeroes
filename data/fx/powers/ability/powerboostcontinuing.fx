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
	On 	Time
	Time 	0


	Event
		Type	Local 
		At	Head
		
		Part1	:BoostPower.part
		Part2	:BoostPowerSmall.part
		Part3	:AbilityDots1.part
		Part4	:AbilitySparklingDots1.part
		Sound tinkle_loop 50 50 .4
	End

End

End
##################################

