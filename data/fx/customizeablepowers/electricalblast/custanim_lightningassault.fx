#############################################################
## Header
#############################################################

FxInfo

LifeSpan 60

## HANDS ###########################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	local
		At	HandR
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityControl01.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityControl02.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityInnerGlow.part
		SOund	lightningbolt3 80 30 .76
		Lifespan 40
	End
End

Condition
	On 	Time
	Time 	10

	Event
		Type	local
		At	HandR
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityOuterGlow.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityArch.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityRing.part
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	25

	Event
		EName 	BurstR
		Type	local
		At	HandR
		Part4	CustomizeablePowers\ElectricalBlast\ElectricitySparkBurst.part
		LifeSpan	25
	End
End

#############################################################

Condition
	On 	Time
	Time 	25

	Event
		Type	local
		At	HandR
		Part1	CustomizeablePowers\ElectricalBlast\ChargedBolt01.part
		Part2	CustomizeablePowers\ElectricalBlast\ChargedBolt02.part
		Part3	CustomizeablePowers\ElectricalBlast\ChargedBolt03.part
		POther Target
	End
End

Condition
	On 	Time
	Time 	30

	Event
		Type	local
		At	HandR
		Part2	CustomizeablePowers\ElectricalBlast\ChargedBolt02.part
		Part3	CustomizeablePowers\ElectricalBlast\ChargedBolt03.part
		Part3	CustomizeablePowers\ElectricalBlast\ChargedBolt04.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	35

	Event
		Type	local
		At	HandR
		Part4	CustomizeablePowers\ElectricalBlast\ChargedBolt04.part
		Part5	CustomizeablePowers\ElectricalBlast\ChargedBolt05.part
		Part3	CustomizeablePowers\ElectricalBlast\ChargedBolt06.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	40

	Event
		Type	local
		At	HandR
		Part1	CustomizeablePowers\ElectricalBlast\ChargedBolt01.part
		Part2	CustomizeablePowers\ElectricalBlast\ChargedBolt02.part
		Part3	CustomizeablePowers\ElectricalBlast\ChargedBolt03.part
		POther Target
	End
End

Condition
	On 	Time
	Time 	45

	Event
		Type	local
		At	HandR
		Part2	CustomizeablePowers\ElectricalBlast\ChargedBolt02.part
		Part3	CustomizeablePowers\ElectricalBlast\ChargedBolt03.part
		Part3	CustomizeablePowers\ElectricalBlast\ChargedBolt04.part
		POther  Target
	End
End


Condition
	On 	Time
	Time 	50

	Event
		Type	local
		At	HandR
		Part4	CustomizeablePowers\ElectricalBlast\ChargedBolt04.part
		Part5	CustomizeablePowers\ElectricalBlast\ChargedBolt05.part
		Part3	CustomizeablePowers\ElectricalBlast\ChargedBolt06.part
		POther  Target
	End
End

#############################################################

End