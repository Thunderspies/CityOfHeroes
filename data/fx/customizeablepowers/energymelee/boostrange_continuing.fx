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
		Part3	CustomizeablePowers\EnergyMelee\AbilityDotsA.part
		Part4	CustomizeablePowers\EnergyMelee\AbilitySparklingDotsA.part
		Sound phase_loop 50 50 .3
	End

	Event
		Type	Local
		At	Head
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	CustomizeablePowers\EnergyMelee\BoostRangeRing.part
		part	CustomizeablePowers\EnergyMelee\BoostRangeRing2.part
		part	CustomizeablePowers\EnergyMelee\BoostRangeRingUp.part
		part	CustomizeablePowers\EnergyMelee\BoostRangeRaysThin.part
	End
End

#############################################################

End