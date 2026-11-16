#############################################################
## ShockField.fx
#############################################################

FxInfo

LifeSpan 90

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	HandR
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
		Sound shockfield 80 80 .85
	End

	Event
		Type	local
		At	HandR
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
		Part4	:ElectricityArchInvert.part
	End

	Event
		Type	local
		At	HandL
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
	End

	Event
		Type	local
		At	HandL
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
		Part4	:ElectricityArchInvert.part
	End
End

#############################################################

Condition
	On 	Time
	Time 	56

	Event
		Type	Local
		At	Hips
		Part1	:ShockFieldRing.part
		LifeSpan 27
	End

	Event
		Type	Local
		At	Hips
		Part2	:ShockFieldGlow.part
		LifeSpan 18
	End
End

Condition
	On 	Time
	Time 	60

	Event

		Type	Local
		At	Hips
		Part3	:ShockFieldArchs2.part
		Part4	:ShockFieldArchs1.part
		Part5	:ShockFieldArchs.part
	End
End

#############################################################

End