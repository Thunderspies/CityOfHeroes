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
		Part1	CustomizeablePowers\EnergyMelee\Subtractive\powerpunch1.part
		Part2	CustomizeablePowers\EnergyMelee\Subtractive\powerpunch2.part
		Part2	CustomizeablePowers\EnergyMelee\Subtractive\powerpunch3.part

		Part3	CustomizeablePowers\EnergyMelee\Subtractive\powerpunchGlow.part
		Part3	CustomizeablePowers\EnergyMelee\Subtractive\powerpunchRings.part
		Sound powerup7 100 100 0.5
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
		Part1	CustomizeablePowers\EnergyMelee\Subtractive\powerpunch1.part
		Part2	CustomizeablePowers\EnergyMelee\Subtractive\powerpunch2.part
		Part2	CustomizeablePowers\EnergyMelee\Subtractive\powerpunch3.part

		Part3	CustomizeablePowers\EnergyMelee\Subtractive\powerpunchRings.part
		Part3	CustomizeablePowers\EnergyMelee\Subtractive\powerpunchGlow.part
		Sound forcefield5_loop 60 60 0.33
	End
End

#############################################################

End