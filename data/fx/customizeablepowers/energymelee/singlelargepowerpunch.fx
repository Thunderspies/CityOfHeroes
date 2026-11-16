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
		Part1	CustomizeablePowers\EnergyMelee\Largepowerpunch1.part
		Part2	CustomizeablePowers\EnergyMelee\Largepowerpunch2.part
		Part3   CustomizeablePowers\EnergyMelee\LargepowerpunchGlow.part
		Part1	CustomizeablePowers\EnergyMelee\Largepowerpunch3.part
		Part3   CustomizeablePowers\EnergyMelee\LargepowerpunchRings.part
		Sound PunchA 100 100 .6
	End

	Event
		Type	Local
		At	WepR
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\EnergyMelee\Largepowerpunch1.part
		Part2	CustomizeablePowers\EnergyMelee\Largepowerpunch2.part
		Part3   CustomizeablePowers\EnergyMelee\LargepowerpunchGlow.part
		Part1	CustomizeablePowers\EnergyMelee\Largepowerpunch3.part
		Part3   CustomizeablePowers\EnergyMelee\LargepowerpunchRings.part
	End
End

#############################################################

End