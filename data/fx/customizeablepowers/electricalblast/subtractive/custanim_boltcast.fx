#############################################################
## ChargedBolts.fx
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
		Part	:ElectricityControl01.part
		Part	:ElectricityControl02.part
		Part	:ElectricityInnerGlow.part
		Part	:ElectricityOuterGlow.part
		Part	:ElectricityArch.part
		Part	:ElectricityRing.part
		Sound chargebolt 100 100 0.77
		Lifespan 35
	End

	Event
		Type	local
		At	HandR
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
	Time 	18

	Event
		Type	local
		At	HandL
		Part4	:ElectricitySparkBurst.part
		LifeSpan 30
	End

	Event
		Type	local
		At	HandR
		Part4	:ElectricitySparkBurst.part
		LifeSpan 30
	End
End

## BOLTS ###########################################################

Condition
	On 	Time
	Time 	18

	Event
		Type	local
		At	WepL
		Part1	:ChargedBolt01.part
		Part2	:ChargedBolt02.part
		Part3	:ChargedBolt03.part
		POther Target
	End

	Event
		Type	local
		At	WepR
		Part1	:ChargedBolt01.part
		Part2	:ChargedBolt02.part
		Part3	:ChargedBolt03.part
		POther Target
	End
End

Condition
	On 	Time
	Time 	23

	Event
		Type	local
		At	WepL
		Part2	:ChargedBolt02.part
		Part3	:ChargedBolt03.part
		Part3	:ChargedBolt04.part
		POther  Target
	End

	Event
		Type	local
		At	WepR
		Part2	:ChargedBolt02.part
		Part3	:ChargedBolt03.part
		Part3	:ChargedBolt04.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	28

	Event
		Type	local
		At	WepL
		Part4	:ChargedBolt04.part
		Part5	:ChargedBolt05.part
		Part3	:ChargedBolt06.part
		POther  Target
	End

	Event
		Type	local
		At	WepR
		Part4	:ChargedBolt04.part
		Part5	:ChargedBolt05.part
		Part3	:ChargedBolt06.part
		POther  Target
	End
End

#############################################################

End