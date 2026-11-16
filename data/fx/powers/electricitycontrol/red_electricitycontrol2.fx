#############################################################
## Red_ElectricityControl2.fx
#############################################################

FxInfo
LifeSpan	170

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On Time
	Time 2

	Event
		Type 	local
		At	Origin
		Sound electhunderpunch 70 70 .8

	End
End

Condition
	On 	Time
	Time 	70


	Event
		EName 	Mallet
		Type	start
		At	T_Chest
		Geom	RadiationShield

		BhvrOverride

			Alpha	20

		End

		bhvr	behaviors/RedScaleBubble.bhvr

		LifeSpan	10
	End

End

Condition
	On 	Time
	Time 	37

	Event
		EName 	Thingy
		Type	local
		At	HandR
		Part1	:Red_ElectricityControl01.part
		Part2	:Red_ElectricityControl02.part
		Part	:Red_ElectricityControl01White.part
		Part	:Red_ElectricityControl02white.part

		Part1	:Red_ElectricityControl01Large.part
		Part2	:Red_ElectricityControl02Large.part
		Part	:Red_ElectricityControl01WhiteLarge.part
		Part	:Red_ElectricityControl02whiteLarge.part
		Part3	:Red_ElectricityInnerGlow.part
		Sound elechandson2 60 60 .8
		LifeSpan	50

	End

	Event
		EName 	Thingy2
		Type	local
		At	HandL
		Part1	:Red_ElectricityControl01.part
		Part2	:Red_ElectricityControl02.part
		Part	:Red_ElectricityControl01White.part
		Part	:Red_ElectricityControl02white.part

		Part1	:Red_ElectricityControl01Large.part
		Part2	:Red_ElectricityControl02Large.part
		Part	:Red_ElectricityControl01WhiteLarge.part
		Part	:Red_ElectricityControl02whiteLarge.part
		Part3	:Red_ElectricityInnerGlow.part
		LifeSpan	50

	End


	Event
		EName 	Thingy3
		Type	local
		At	HandR
		Part1	:Red_ElectricityOuterGlow.part
		Part2	:Red_ElectricityArch.part
		Part3	:Red_ElectricityRing.part
		Part4	:Red_ElectricityArchInvert.part
		LifeSpan	50
	End

	Event
		EName 	Thingy4
		Type	local
		At	HandL
		Part1	:Red_ElectricityOuterGlow.part
		Part2	:Red_ElectricityArch.part
		Part3	:Red_ElectricityRing.part
		Part4	:Red_ElectricityArchInvert.part
		LifeSpan	50

	End

End

Condition
	On 	Time
	Time 	55


	Event
		EName 	Thingy
		Type	local
		At	HandR

		Part1	:Red_ElectricityControl01Large.part
		Part2	:Red_ElectricityControl02Large.part
		Part	:Red_ElectricityControl01WhiteLarge.part
		Part	:Red_ElectricityControl02whiteLarge.part
		Part3	:Red_ElectricityInnerGlow.part
		#Sound	punchb 70 60 .8
		LifeSpan	45

	End

	Event
		EName 	Thingy2
		Type	local
		At	HandL
		Part1	:Red_ElectricityControl01Large.part
		Part2	:Red_ElectricityControl02Large.part
		Part	:Red_ElectricityControl01WhiteLarge.part
		Part	:Red_ElectricityControl02whiteLarge.part
		Part3	:Red_ElectricityInnerGlow.part
		LifeSpan	45

	End

End

End
