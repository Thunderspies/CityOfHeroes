#############################################################
## ShockField.fx
#############################################################

FxInfo

LifeSpan 90

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	HandR
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityControl01.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityControl02.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityInnerGlow.part
		Sound shockfield 80 80 .85
	End

	Event
		Type	local
		At	HandR
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityOuterGlow.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityArch.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityRing.part
		Part4	CustomizeablePowers\ElectricalBlast\ElectricityArchInvert.part
	End

	Event
		Type	local
		At	HandL
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityControl01.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityControl02.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityInnerGlow.part
	End

	Event
		Type	local
		At	HandL
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityOuterGlow.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityArch.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityRing.part
		Part4	CustomizeablePowers\ElectricalBlast\ElectricityArchInvert.part
	End
End

#############################################################

Condition
	On 	Time
	Time 	56

	Event
		Type	Local
		At	Hips
		Part1	CustomizeablePowers\ElectricalBlast\ShockFieldRing.part
		LifeSpan 27
	End

	Event
		Type	Local
		At	Hips
		Part2	CustomizeablePowers\ElectricalBlast\ShockFieldGlow.part
		LifeSpan 18
	End
End

Condition
	On 	Time
	Time 	60

	Event

		Type	Local
		At	Hips
		Part3	CustomizeablePowers\ElectricalBlast\ShockFieldArchs2.part
		Part4	CustomizeablePowers\ElectricalBlast\ShockFieldArchs1.part
		Part5	CustomizeablePowers\ElectricalBlast\ShockFieldArchs.part
	End
End

#############################################################

End