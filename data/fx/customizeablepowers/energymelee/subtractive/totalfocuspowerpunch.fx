#############################################################
## TotalFocusPowerPunch.fx
#############################################################

FxInfo

Lifespan 105

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\EnergyMelee\Subtractive\Largepowerpunch1.part
		Part2	CustomizeablePowers\EnergyMelee\Subtractive\Largepowerpunch2.part
		Part5	CustomizeablePowers\EnergyMelee\Subtractive\Largepowerpunch3.part
		Part3   CustomizeablePowers\EnergyMelee\Subtractive\LargepowerpunchGlow.part
		Part4   CustomizeablePowers\EnergyMelee\Subtractive\LargepowerpunchRings.part
		Sound PPfocus2 100 100 0.7
		Lifespan 85
	End

	Event
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\EnergyMelee\Subtractive\Largepowerpunch1.part
		Part2	CustomizeablePowers\EnergyMelee\Subtractive\Largepowerpunch2.part
		Part5	CustomizeablePowers\EnergyMelee\Subtractive\Largepowerpunch3.part
		Part3   CustomizeablePowers\EnergyMelee\Subtractive\LargepowerpunchGlow.part
		Part4   CustomizeablePowers\EnergyMelee\Subtractive\LargepowerpunchRings.part
		Lifespan 85
	End
End

#############################################################

Condition
	On 	Time
	Time 	63

	Event
		Type	Local
		At	WepR
		Part3   CustomizeablePowers\EnergyMelee\Subtractive\QuickRings.part
		Lifespan 30
	End

	Event
		Type	Local
		At	WepL
		Part3   CustomizeablePowers\EnergyMelee\Subtractive\QuickRings.part
		Lifespan 30
	End
End

#############################################################

End