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

		Part1	CustomizeablePowers\EnergyMelee\BoostPower.part
		Part2	CustomizeablePowers\EnergyMelee\BoostPowerSmall.part
		Part3	CustomizeablePowers\EnergyMelee\AbilityDots1.part
		Part4	CustomizeablePowers\EnergyMelee\AbilitySparklingDots1.part
		Sound tinkle_loop 50 50 .4
	End

End

End
##################################

