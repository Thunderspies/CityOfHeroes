#############################################################
## Zapp.fx
#############################################################

FxInfo

LifeSpan 500

## HANDS ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	Hips
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityControl01.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityControl02.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityInnerGlow.part
		Lifespan 55
	End

	Event
		Type	local
		At	Hips
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityOuterGlow.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityArch.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityRing.part
		Lifespan 55
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	BurstR
		Type	local
		At	Hips
		Part4	CustomizeablePowers\ElectricalBlast\ElectricitySparkBurst.part
		Sound lightningbolt2 80 80 .76
		LifeSpan 20
	End
End

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		Type	start
		At	Hips
		Part1	CustomizeablePowers\ElectricalBlast\QuickBolt06.part
		Part2	CustomizeablePowers\ElectricalBlast\QuickBolt05.part
		Part3	CustomizeablePowers\ElectricalBlast\QuickBolt01.part
		Part4	CustomizeablePowers\ElectricalBlast\QuickBolt02.part
		POther Target
	End
End

Condition
	On 	Time
	Time 	8

	Event
		Type	start
		At	Hips
		Part4	CustomizeablePowers\ElectricalBlast\QuickBolt03.part
		Part5	CustomizeablePowers\ElectricalBlast\QuickBolt04.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	12

	Event
		Type	start
		At	Hips
		Part2	CustomizeablePowers\ElectricalBlast\QuickBolt01.part
		Part3	CustomizeablePowers\ElectricalBlast\QuickBolt02.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	15

	Event
		Type	start
		At	Hips
		Part4	CustomizeablePowers\ElectricalBlast\QuickBolt03.part
		Part5	CustomizeablePowers\ElectricalBlast\QuickBolt04.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	19

	Event
		Type	start
		At	Hips
		Part2	CustomizeablePowers\ElectricalBlast\QuickBolt01.part
		Part3	CustomizeablePowers\ElectricalBlast\QuickBolt02.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	22

	Event
		Type	start
		At	Hips
		Part4	CustomizeablePowers\ElectricalBlast\QuickBolt03.part
		Part5	CustomizeablePowers\ElectricalBlast\QuickBolt04.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	26

	Event

		Type	start
		At	Hips
		#Part2	:QuickBolt01a.part
		Part3	CustomizeablePowers\ElectricalBlast\QuickBolt06.part
		Part4	CustomizeablePowers\ElectricalBlast\QuickBolt05.part
		Part5	CustomizeablePowers\ElectricalBlast\QuickBolt04.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	30

	Event
		Type	start
		At	Hips
		Part1	CustomizeablePowers\ElectricalBlast\QuickBolt06.part
		Part2	CustomizeablePowers\ElectricalBlast\QuickBolt05.part
		Part3	CustomizeablePowers\ElectricalBlast\QuickBolt01.part
		Part4	CustomizeablePowers\ElectricalBlast\QuickBolt02.part
		POther Target
	End
End

Condition
	On 	Time
	Time 	33

	Event
		Type	start
		At	Hips
		Part4	CustomizeablePowers\ElectricalBlast\QuickBolt03.part
		Part5	CustomizeablePowers\ElectricalBlast\QuickBolt04.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	37

	Event
		Type	start
		At	Hips
		Part2	CustomizeablePowers\ElectricalBlast\QuickBolt01.part
		Part3	CustomizeablePowers\ElectricalBlast\QuickBolt02.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	40

	Event
		Type	start
		At	Hips
		Part4	CustomizeablePowers\ElectricalBlast\QuickBolt03.part
		Part5	CustomizeablePowers\ElectricalBlast\QuickBolt04.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	44

	Event
		Type	start
		At	Hips
		Part2	CustomizeablePowers\ElectricalBlast\QuickBolt01.part
		Part3	CustomizeablePowers\ElectricalBlast\QuickBolt02.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	47

	Event

		Type	start
		At	Hips
		Part2	CustomizeablePowers\ElectricalBlast\QuickBolt01.part
		Part3	CustomizeablePowers\ElectricalBlast\QuickBolt06.part
		Part4	CustomizeablePowers\ElectricalBlast\QuickBolt05.part
		Part5	CustomizeablePowers\ElectricalBlast\QuickBolt04.part
		POther  Target
	End
End

#############################################################

End