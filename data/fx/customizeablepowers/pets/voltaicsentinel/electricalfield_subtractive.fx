#############################################################
## ElectricalField.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Origin
		Part1	CustomizeablePowers\ElectricalBlast\Subtractive\ElectricityControl01a.part
		Part2	CustomizeablePowers\ElectricalBlast\Subtractive\ElectricityControl02a.part
		Part3	CustomizeablePowers\ElectricalBlast\Subtractive\ElectricityInnerGlowa.part
		Part4	CustomizeablePowers\ElectricalBlast\Subtractive\ElectricityOuterGlow.part
		Part5	CustomizeablePowers\ElectricalBlast\Subtractive\ElectricityArch.part
	End

	Event
		Type	Local
		At	Origin
		Part1	CustomizeablePowers\ElectricalBlast\Subtractive\ElectricityRing.part
		Part2	CustomizeablePowers\ElectricalBlast\Subtractive\ElectricityArchInvert.part
		Part3	CustomizeablePowers\ElectricalBlast\Subtractive\ElectricityInnerGlow2.part
		Part4	CustomizeablePowers\ElectricalBlast\Subtractive\ElectricityOuterRing.part
	End
End

#############################################################

Condition
	On 	Death

	Event
		EName	ALL
		Type	Destroy
	End
End

#############################################################

End