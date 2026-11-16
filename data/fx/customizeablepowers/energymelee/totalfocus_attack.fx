#############################################################
## TotalFocus_Attack.fx
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
		Part1	CustomizeablePowers\EnergyMelee\Largepowerpunch1.part
		Part2	CustomizeablePowers\EnergyMelee\Largepowerpunch2.part
		Part5	CustomizeablePowers\EnergyMelee\Largepowerpunch3.part
		Part3   CustomizeablePowers\EnergyMelee\LargepowerpunchGlow.part
		Part4   CustomizeablePowers\EnergyMelee\powerpunchRings.part
		Sound PPfocus2 100 100 0.7
		Lifespan 85
	End

	Event
		Type	Local
		At	WepR
		Part1	CustomizeablePowers\EnergyMelee\Largepowerpunch1.part
		Part2	CustomizeablePowers\EnergyMelee\Largepowerpunch2.part
		Part5	CustomizeablePowers\EnergyMelee\Largepowerpunch3.part
		Part3   CustomizeablePowers\EnergyMelee\LargepowerpunchGlow.part
		Part4   CustomizeablePowers\EnergyMelee\powerpunchRings.part
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
		Part3   CustomizeablePowers\EnergyMelee\QuickRings.part
	End

	Event
		Type	Local
		At	WepL
		Part3   CustomizeablePowers\EnergyMelee\QuickRings.part
	End
End

#############################################################

End