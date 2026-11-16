#############################################################
## ThunderStrike_Attack.fx
#############################################################

FxInfo

LifeSpan 70

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	WepR
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
		Part4	:ElectricityArchInvert.part
		Sound punchb 70 70 0.7
	End

	Event
		Type	local
		At	WepL
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
		Part4	:ElectricityArchInvert.part
		Sound elechandson3 70 70 0.8
	End
End

#############################################################

End