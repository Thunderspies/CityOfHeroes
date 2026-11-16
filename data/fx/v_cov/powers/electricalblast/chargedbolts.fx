#############################################################
## Header
#############################################################

FxInfo

LifeSpan 45

## HANDS ###########################################################

Condition
	On 	Time
	Time 	10

	Event
		EName 	Thingy1
		Type	local
		At	HandL
		Part	:ElectricityControl01.part
		Part	:ElectricityControl02.part
		Part	:ElectricityInnerGlow.part
		Part	:ElectricityOuterGlow.part
		Part	:ElectricityArch.part
		Part	:ElectricityRing.part
		Sound chargebolt 100 100 0.77
		Lifespan 35
	End
End

Condition
	On 	Time
	Time 	15

	Event
		EName 	BurstL
		Type	local
		At	HandL
		Part4	:ElectricitySparkBurst.part
		LifeSpan 30
	End
End

## BOLTS ###########################################################

Condition
	On 	Time
	Time 	15

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
	Time 	20

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
	Time 	25

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

End