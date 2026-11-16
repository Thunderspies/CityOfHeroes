#############################################################
## ArcticIceAir.fx
#############################################################

FxInfo

###########################################################

Condition
	On	Time
	Time	4

	Event
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part1	CustomizeablePowers\IceControl\snowburst.part
		Sound Coldblast2 100 100 .8
		LifeSpan	70
	End


	Event
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part1	CustomizeablePowers\IceControl\snowburst.part
		LifeSpan	70
	End

	Event
		Ename   LeftHandMist
		Type	Local
		At	WepL

		Part2	CustomizeablePowers\IceControl\ColdArmMist.part

		LifeSpan	70
	End


	Event
		Ename   RightHandMist
		Type	Local
		At	WepR

		Part2	CustomizeablePowers\IceControl\ColdArmMist.part

		LifeSpan	70

	End

End


Condition
	On	Time
	Time	20

	Event
		EName	Pri
		Type	posit
		At	Root
		Part4	CustomizeablePowers\IceControl\IceSlickRing.part

	End

	Event
		Ename	ExplosiveForce
		Type	Local
		At	Hips

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		30
			PhysForcePowerJitter	20
		End

		Lifespan	10
	End
	Event
		Ename	ExplosiveForce
		Type	Local
		At	Root

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		35
			PhysForcePower		10
			PhysForcePowerJitter	5
		End

	End
End

Condition
	On	Time
	Time	27


	Event
		EName	Pri
		Type	posit
		At	Root

		Part1	CustomizeablePowers\IceControl\IceSlickSparklies.part
		part2	CustomizeablePowers\IceControl\IceSlickInsideMistA.part
		part3	CustomizeablePowers\IceControl\IceSlickInsideMist2.part
		Sound cold_loop 80 80 .4

	End

	Event
		EName	Pri
		Type	posit
		At	Root
		Part4	CustomizeablePowers\IceControl\IceSlickRing2.part


	End

End


Condition
	On	Time
	Time	31


	Event
		EName	Pri
		Type	posit
		At	Root

		part1	CustomizeablePowers\IceControl\IceSlickMist.part
		part3	CustomizeablePowers\IceControl\IceSlickMist2.part


	End


End


Condition
	On	Time
	Time	30


	Event
		EName	Pri
		Type	posit
		At	Root
		Part4	CustomizeablePowers\IceControl\IceSlickRings.part
	End

End


End