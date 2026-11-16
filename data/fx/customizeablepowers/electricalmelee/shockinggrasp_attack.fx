#############################################################
## ShockingGrasp_Attack.fx
#############################################################

FxInfo

LifeSpan 40

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	HandR
		Part1	CustomizeablePowers\ElectricalMelee\ElectricityControl01.part
		Part2	CustomizeablePowers\ElectricalMelee\ElectricityControl02.part
		Part3	CustomizeablePowers\ElectricalMelee\ElectricityInnerGlow.part
		Sound eleccontrol2 50 50 0.4
	End

	Event
		Type	local
		At	HandL
		Part1	CustomizeablePowers\ElectricalMelee\ElectricityControl01.part
		Part2	CustomizeablePowers\ElectricalMelee\ElectricityControl02.part
		Part3	CustomizeablePowers\ElectricalMelee\ElectricityInnerGlow.part
	End

	Event
		Type	local
		At	HandR
		Part1	CustomizeablePowers\ElectricalMelee\ElectricityOuterGlow.part
		Part2	CustomizeablePowers\ElectricalMelee\ElectricityArch.part
		Part3	CustomizeablePowers\ElectricalMelee\ElectricityRing.part
		Part4	CustomizeablePowers\ElectricalMelee\ElectricityArchInvert.part
	End

	Event
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