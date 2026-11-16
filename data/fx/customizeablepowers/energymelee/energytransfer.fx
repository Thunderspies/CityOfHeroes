#############################################################
## EnergyTransfer.fx
#############################################################

FxInfo

Lifespan 200

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part5	CustomizeablePowers\EnergyMelee\Largepowerpunch3.part
		Part1	CustomizeablePowers\EnergyMelee\BuildupRings.part
		Part2	CustomizeablePowers\EnergyMelee\BuildupSparks.part
		Part3	CustomizeablePowers\EnergyMelee\BuildupGlow.part
		Sound chargeUp_1 50 50 .8
		Lifespan 30
	End

	Event
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part5	CustomizeablePowers\EnergyMelee\Largepowerpunch3.part
		Part1	CustomizeablePowers\EnergyMelee\BuildupRings.part
		Part2	CustomizeablePowers\EnergyMelee\BuildupSparks.part
		Part3	CustomizeablePowers\EnergyMelee\BuildupGlow.part
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	55

	Event
		Type	Local
		At	WepR
		Part3	CustomizeablePowers\EnergyMelee\QuickRings.part
		Lifespan 30
	End

	Event
		Type	Local
		At	WepL
		Part3	CustomizeablePowers\EnergyMelee\QuickRings.part
		Lifespan 30
	End
End

#############################################################

End