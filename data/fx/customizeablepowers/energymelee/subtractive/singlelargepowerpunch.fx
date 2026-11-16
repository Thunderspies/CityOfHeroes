#############################################################
## SingleLargePowerPunch.fx
#############################################################

FxInfo

Lifespan 22

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	WepL
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\EnergyMelee\Subtractive\Largepowerpunch1.part
		Part2	CustomizeablePowers\EnergyMelee\Subtractive\Largepowerpunch2.part
		Part3   CustomizeablePowers\EnergyMelee\Subtractive\LargepowerpunchGlow.part
		Part1	CustomizeablePowers\EnergyMelee\Subtractive\Largepowerpunch3.part
		Part3   CustomizeablePowers\EnergyMelee\Subtractive\LargepowerpunchRings.part
		Sound PunchA 100 100 0.6
	End

	Event
		Type	Local
		At	WepR
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\EnergyMelee\Subtractive\Largepowerpunch1.part
		Part2	CustomizeablePowers\EnergyMelee\Subtractive\Largepowerpunch2.part
		Part3   CustomizeablePowers\EnergyMelee\Subtractive\LargepowerpunchGlow.part
		Part1	CustomizeablePowers\EnergyMelee\Subtractive\Largepowerpunch3.part
		Part3   CustomizeablePowers\EnergyMelee\Subtractive\LargepowerpunchRings.part
	End
End

#############################################################

End