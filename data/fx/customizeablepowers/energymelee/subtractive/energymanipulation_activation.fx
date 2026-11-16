#############################################################
## EnergyManipulation_Activation.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\EnergyMelee\Subtractive\powerpunch1.part
		Part2	CustomizeablePowers\EnergyMelee\Subtractive\powerpunch2.part
		Part3	CustomizeablePowers\EnergyMelee\Subtractive\powerpunchGlow.part
		Part3	CustomizeablePowers\EnergyMelee\Subtractive\powerpunchRings.part
		Sound powerup7 100 100 0.5
	End

	Event
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\EnergyMelee\Subtractive\powerpunch1.part
		Part2	CustomizeablePowers\EnergyMelee\Subtractive\powerpunch2.part
		Part3	CustomizeablePowers\EnergyMelee\Subtractive\powerpunchGlow.part
		Part3	CustomizeablePowers\EnergyMelee\Subtractive\powerpunchRings.part
		Sound forcefield5_loop 60 60 0.33
	End
End

#############################################################

End