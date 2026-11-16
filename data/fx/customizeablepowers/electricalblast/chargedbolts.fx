#############################################################
## Header
#############################################################

FxInfo

LifeSpan 45

## HANDS ###########################################################

Condition
	On 	Time
	Time 	15

	Event
		Type	local
		At	HandL
		Part	CustomizeablePowers\ElectricalBlast\ElectricityControl01.part
		Part	CustomizeablePowers\ElectricalBlast\ElectricityControl02.part
		Part	CustomizeablePowers\ElectricalBlast\ElectricityInnerGlow.part
		Part	CustomizeablePowers\ElectricalBlast\ElectricityOuterGlow.part
		Part	CustomizeablePowers\ElectricalBlast\ElectricityArch.part
		Part	CustomizeablePowers\ElectricalBlast\ElectricityRing.part
		Sound chargebolt 100 100 0.77
		Lifespan 35
	End
End

Condition
	On 	Time
	Time 	18

	Event
		Type	local
		At	HandL
		Part4	CustomizeablePowers\ElectricalBlast\ElectricitySparkBurst.part
		LifeSpan 30
	End
End

## BOLTS ###########################################################

Condition
	On 	Time
	Time 	18

	Event
		Type	local
		At	HandL
		Part1	CustomizeablePowers\ElectricalBlast\ChargedBolt01.part
		Part2	CustomizeablePowers\ElectricalBlast\ChargedBolt02.part
		Part3	CustomizeablePowers\ElectricalBlast\ChargedBolt03.part
		POther Target
	End
End

Condition
	On 	Time
	Time 	23

	Event
		Type	local
		At	HandL
		Part2	CustomizeablePowers\ElectricalBlast\ChargedBolt02.part
		Part3	CustomizeablePowers\ElectricalBlast\ChargedBolt03.part
		Part3	CustomizeablePowers\ElectricalBlast\ChargedBolt04.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	28

	Event
		Type	local
		At	HandL
		Part4	CustomizeablePowers\ElectricalBlast\ChargedBolt04.part
		Part5	CustomizeablePowers\ElectricalBlast\ChargedBolt05.part
		Part3	CustomizeablePowers\ElectricalBlast\ChargedBolt06.part
		POther  Target
	End
End

#############################################################

End