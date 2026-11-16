#############################################################
## Red_ElectricityControlOneHand.fx
#############################################################

FxInfo

LifeSpan 40

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Thingy
		Type	local
		At	HandR
		Part1	:Red_ElectricityControl01.part
		Part2	:Red_ElectricityControl02.part
		Part3	:Red_ElectricityInnerGlow.part
		Sound elecpunch3 50 50 .7
	End

	Event
		EName 	Thingy2
		Type	local
		At	HandL
		Part1	:Red_ElectricityControl01.part
		Part2	:Red_ElectricityControl02.part
		Part3	:Red_ElectricityInnerGlow.part
	End

	Event
		EName 	Thingy3
		Type	local
		At	HandR
		Part1	:Red_ElectricityOuterGlow.part
		Part2	:Red_ElectricityArch.part
		Part3	:Red_ElectricityRing.part
		Part4	:Red_ElectricityArchInvert.part
	End

	Event
		EName 	Thingy4
		Type	local
		At	HandL
		Part1	:Red_ElectricityOuterGlow.part
		Part2	:Red_ElectricityArch.part
		Part3	:Red_ElectricityRing.part
		Part4	:Red_ElectricityArchInvert.part
	End
End

#############################################################

End