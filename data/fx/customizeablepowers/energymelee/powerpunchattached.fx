#############################################################
## PowerPunchAttached.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\EnergyMelee\powerpunch1.part
		Part2	CustomizeablePowers\EnergyMelee\powerpunch2.part
		Part2	CustomizeablePowers\EnergyMelee\powerpunch3.part
		Part3	CustomizeablePowers\EnergyMelee\powerpunchGlow.part
		Part3	CustomizeablePowers\EnergyMelee\powerpunchRings.part
		Sound powerup7 100 100 .5
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime2
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\EnergyMelee\powerpunch1.part
		Part2	CustomizeablePowers\EnergyMelee\powerpunch2.part
		Part2	CustomizeablePowers\EnergyMelee\powerpunch3.part
		Part3	CustomizeablePowers\EnergyMelee\powerpunchRings.part
		Part3	CustomizeablePowers\EnergyMelee\powerpunchGlow.part
		Sound forcefield5_loop 60 60 .33
	End
End

#############################################################

End