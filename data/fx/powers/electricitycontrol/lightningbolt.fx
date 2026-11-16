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
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
		SOund	lightningbolt3 80 30 .76
		Lifespan 40
	End

	Event
		Type	local
		At	HandL
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
		Lifespan 40
	End
End

Condition
	On 	Time
	Time 	10

	Event
		Type	local
		At	HandR
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
		Lifespan 30
	End

	Event
		Type	local
		At	HandL
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
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
		Part4	:ElectricitySparkBurst.part
		LifeSpan	25
	End

	Event
		EName 	BurstL
		Type	local
		At	HandL
		Part4	:ElectricitySparkBurst.part
		LifeSpan	25
	End
End

#############################################################

Condition
	On 	Time
	Time 	25

	Event
		Type	local
		At	HandL
		Part1	:ChargedBolt01.part
		Part2	:ChargedBolt02.part
		Part3	:ChargedBolt03.part
		POther Target
	End
End

Condition
	On 	Time
	Time 	30

	Event
		Type	local
		At	HandL
		Part2	:ChargedBolt02.part
		Part3	:ChargedBolt03.part
		Part3	:ChargedBolt04.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	35

	Event
		Type	local
		At	HandL
		Part4	:ChargedBolt04.part
		Part5	:ChargedBolt05.part
		Part3	:ChargedBolt06.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	40

	Event
		Type	local
		At	HandL
		Part1	:ChargedBolt01.part
		Part2	:ChargedBolt02.part
		Part3	:ChargedBolt03.part
		POther Target
	End
End

Condition
	On 	Time
	Time 	45

	Event
		Type	local
		At	HandL
		Part2	:ChargedBolt02.part
		Part3	:ChargedBolt03.part
		Part3	:ChargedBolt04.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	50

	Event
		Type	local
		At	HandL
		Part4	:ChargedBolt04.part
		Part5	:ChargedBolt05.part
		Part3	:ChargedBolt06.part
		POther  Target
	End
End

#############################################################

Condition
	On 	Time
	Time 	25

	Event
		Type	local
		At	HandR
		Part1	:ChargedBolt01.part
		Part2	:ChargedBolt02.part
		Part3	:ChargedBolt03.part
		POther Target
	End
End

Condition
	On 	Time
	Time 	30

	Event
		Type	local
		At	HandR
		Part2	:ChargedBolt02.part
		Part3	:ChargedBolt03.part
		Part3	:ChargedBolt04.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	35

	Event
		Type	local
		At	HandR
		Part4	:ChargedBolt04.part
		Part5	:ChargedBolt05.part
		Part3	:ChargedBolt06.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	40

	Event
		Type	local
		At	HandR
		Part1	:ChargedBolt01.part
		Part2	:ChargedBolt02.part
		Part3	:ChargedBolt03.part
		POther Target
	End
End

Condition
	On 	Time
	Time 	45

	Event
		Type	local
		At	HandR
		Part2	:ChargedBolt02.part
		Part3	:ChargedBolt03.part
		Part3	:ChargedBolt04.part
		POther  Target
	End
End


Condition
	On 	Time
	Time 	50

	Event
		Type	local
		At	HandR
		Part4	:ChargedBolt04.part
		Part5	:ChargedBolt05.part
		Part3	:ChargedBolt06.part
		POther  Target
	End
End

#############################################################

End