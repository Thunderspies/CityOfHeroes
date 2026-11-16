#############################################################
## PowerSink_Attack.fx
#############################################################

FxInfo

LifeSpan 50

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Thingy
		Type	local
		At	HandR
		Part1	CustomizeablePowers\ElectricalMelee\ElectricityControl01.part
		Part2	CustomizeablePowers\ElectricalMelee\ElectricityControl02.part
		Part3	CustomizeablePowers\ElectricalMelee\ElectricityInnerGlow.part
		Sound eleccontrol2 50 50 0.55
	End

	Event
		EName 	Thingy2
		Type	local
		At	HandL
		Part1	CustomizeablePowers\ElectricalMelee\ElectricityControl01.part
		Part2	CustomizeablePowers\ElectricalMelee\ElectricityControl02.part
		Part3	CustomizeablePowers\ElectricalMelee\ElectricityInnerGlow.part
	End

	Event
		EName 	Thingy3
		Type	local
		At	HandR
		Part1	CustomizeablePowers\ElectricalMelee\ElectricityOuterGlow.part
		Part2	CustomizeablePowers\ElectricalMelee\ElectricityArch.part
		Part3	CustomizeablePowers\ElectricalMelee\ElectricityRing.part
		Part4	CustomizeablePowers\ElectricalMelee\ElectricityArchInvert.part
	End

	Event
		EName 	Thingy4
		Type	local
		At	HandL
		Part1	CustomizeablePowers\ElectricalMelee\ElectricityOuterGlow.part
		Part2	CustomizeablePowers\ElectricalMelee\ElectricityArch.part
		Part3	CustomizeablePowers\ElectricalMelee\ElectricityRing.part
		Part4	CustomizeablePowers\ElectricalMelee\ElectricityArchInvert.part
	End
End

#############################################################

End