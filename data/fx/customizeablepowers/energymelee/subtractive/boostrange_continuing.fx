#############################################################
## BoostRange_Continuing.FX
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Head
		Part3	CustomizeablePowers\EnergyMelee\Subtractive\AbilityDotsA.part
		Part4	CustomizeablePowers\EnergyMelee\Subtractive\AbilitySparklingDotsA.part
		Sound phase_loop 50 50 .3
	End

	Event
		Type	Local
		At	Head
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	CustomizeablePowers\EnergyMelee\Subtractive\BoostRangeRing.part
		part	CustomizeablePowers\EnergyMelee\Subtractive\BoostRangeRing2.part
		part	CustomizeablePowers\EnergyMelee\Subtractive\BoostRangeRingUp.part
		part	CustomizeablePowers\EnergyMelee\Subtractive\BoostRangeRaysThin.part
	End
End

#############################################################

End