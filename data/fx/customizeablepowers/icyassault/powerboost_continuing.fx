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
		
		Part1	CustomizeablePowers\IcyAssault\BoostPower.part
		Part2	CustomizeablePowers\IcyAssault\BoostPowerSmall.part
		Part3	CustomizeablePowers\IcyAssault\AbilityDots1.part
		Part4	CustomizeablePowers\IcyAssault\AbilitySparklingDots1.part
		Sound tinkle_loop 50 50 .4
	End

End

End
##################################

