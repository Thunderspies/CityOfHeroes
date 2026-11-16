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
		Part1	CustomizeablePowers\ElectricalBlast\Subtractive\ElectricityControl01.part
		Part2	CustomizeablePowers\ElectricalBlast\Subtractive\ElectricityControl02.part
		Part3	CustomizeablePowers\ElectricalBlast\Subtractive\ElectricityInnerGlow.part
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
		Part1	CustomizeablePowers\ElectricalBlast\Subtractive\ElectricityOuterGlow.part
		Part2	CustomizeablePowers\ElectricalBlast\Subtractive\ElectricityArch.part
		Part3	CustomizeablePowers\ElectricalBlast\Subtractive\ElectricityRing.part
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
		Part4	CustomizeablePowers\ElectricalBlast\Subtractive\ElectricitySparkBurst.part
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
		Part1	CustomizeablePowers\ElectricalBlast\Subtractive\ChargedBolt01.part
		Part2	CustomizeablePowers\ElectricalBlast\Subtractive\ChargedBolt02.part
		Part3	CustomizeablePowers\ElectricalBlast\Subtractive\ChargedBolt03.part
		POther Target
	End
End

Condition
	On 	Time
	Time 	30

	Event
		Type	local
		At	HandR
		Part2	CustomizeablePowers\ElectricalBlast\Subtractive\ChargedBolt02.part
		Part3	CustomizeablePowers\ElectricalBlast\Subtractive\ChargedBolt03.part
		Part3	CustomizeablePowers\ElectricalBlast\Subtractive\ChargedBolt04.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	35

	Event
		Type	local
		At	HandR
		Part4	CustomizeablePowers\ElectricalBlast\Subtractive\ChargedBolt04.part
		Part5	CustomizeablePowers\ElectricalBlast\Subtractive\ChargedBolt05.part
		Part3	CustomizeablePowers\ElectricalBlast\Subtractive\ChargedBolt06.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	40

	Event
		Type	local
		At	HandR
		Part1	CustomizeablePowers\ElectricalBlast\Subtractive\ChargedBolt01.part
		Part2	CustomizeablePowers\ElectricalBlast\Subtractive\ChargedBolt02.part
		Part3	CustomizeablePowers\ElectricalBlast\Subtractive\ChargedBolt03.part
		POther Target
	End
End

Condition
	On 	Time
	Time 	45

	Event
		Type	local
		At	HandR
		Part2	CustomizeablePowers\ElectricalBlast\Subtractive\ChargedBolt02.part
		Part3	CustomizeablePowers\ElectricalBlast\Subtractive\ChargedBolt03.part
		Part3	CustomizeablePowers\ElectricalBlast\Subtractive\ChargedBolt04.part
		POther  Target
	End
End


Condition
	On 	Time
	Time 	50

	Event
		Type	local
		At	HandR
		Part4	CustomizeablePowers\ElectricalBlast\Subtractive\ChargedBolt04.part
		Part5	CustomizeablePowers\ElectricalBlast\Subtractive\ChargedBolt05.part
		Part3	CustomizeablePowers\ElectricalBlast\Subtractive\ChargedBolt06.part
		POther  Target
	End
End

#############################################################

End