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
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityControl01a.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityControl02a.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityInnerGlowa.part
		Part4	CustomizeablePowers\ElectricalBlast\ElectricityOuterGlow.part
		Part5	CustomizeablePowers\ElectricalBlast\ElectricityArch.part
	End

	Event
		Type	Local
		At	Origin
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityRing.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityArchInvert.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityInnerGlow2.part
		Part4	CustomizeablePowers\ElectricalBlast\ElectricityOuterRing.part
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