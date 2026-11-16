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
		Part5	CustomizeablePowers\EnergyMelee\Subtractive\Largepowerpunch3.part
		Part1	CustomizeablePowers\EnergyMelee\Subtractive\BuildupRings.part
		Part2	CustomizeablePowers\EnergyMelee\Subtractive\BuildupSparks.part
		Part3	CustomizeablePowers\EnergyMelee\Subtractive\BuildupGlow.part
		Sound chargeUp_1 50 50 0.8
		Lifespan 30
	End

	Event
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part5	CustomizeablePowers\EnergyMelee\Subtractive\Largepowerpunch3.part
		Part1	CustomizeablePowers\EnergyMelee\Subtractive\BuildupRings.part
		Part2	CustomizeablePowers\EnergyMelee\Subtractive\BuildupSparks.part
		Part3	CustomizeablePowers\EnergyMelee\Subtractive\BuildupGlow.part
		Lifespan 30
	End
End

#############################################################

Condition
	On 	Time
	Time 	55

	Event
		Type	Local
		At	WepR
		Part3	CustomizeablePowers\EnergyMelee\Subtractive\QuickRings.part
		Lifespan 30
	End

	Event
		Type	Local
		At	WepL
		Part3	CustomizeablePowers\EnergyMelee\Subtractive\QuickRings.part
		Lifespan 30
	End
End

#############################################################

End