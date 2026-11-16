#########################################################
##	EBlast
########################################################
FxInfo

Input
	Inpname	HandR
End
Input
	Inpname	HandL
End

LifeSpan	40

################################


Condition
	On 	Time
	Time 	0

	Event
		EName 	Thingy
		Type	local
		At	HandR
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityControl01.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityControl02.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityInnerGlow.part
		Sound eleccontrol2 50 50 .4


	End

	Event
		EName 	Thingy2
		Type	local
		At	HandL
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityControl01.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityControl02.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityInnerGlow.part

	End


	Event
		EName 	Thingy3
		Type	local
		At	HandR
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityOuterGlow.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityArch.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityRing.part
		Part4	CustomizeablePowers\ElectricalBlast\ElectricityArchInvert.part
	End

	Event
		EName 	Thingy4
		Type	local
		At	HandL
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityOuterGlow.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityArch.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityRing.part
		Part4	CustomizeablePowers\ElectricalBlast\ElectricityArchInvert.part

	End

End
################################


End
