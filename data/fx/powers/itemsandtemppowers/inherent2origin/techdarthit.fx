#############################################################
## HEADER
#############################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

LifeSpan  90

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Neck
		Sound tazerzaphit 60 60 .83
	End

	Event
		EName 	Impact
		Type	Local
		At	Neck
		bhvr	:SciDartHit.bhvr
		Part2	:DartHitflashStar.part
		Part1	POWERS\ElectricityControl\ElectricityGlow.part
		Part2	POWERS\ElectricityControl\ElectricityGlowStar.part
		Lifespan 3
	End

	Event
		EName 	Dicharge
		Type	Local
		At	Neck
		bhvr	:SciDartHit.bhvr
		Part2	:DartHitflashStar.part
		Part3	POWERS\ElectricityControl\ElectricityArch4a.part
		Part4	POWERS\ElectricityControl\ElectricityArchSmalla.part
		Lifespan 10
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Repeat	2
	RepeatJitter	1

	Event
		EName	Sparkfar
		Type 	Start
		At	Neck
		ChildFX	V_COV\PhysicsEnabled\ElectricSpark.fx
		Lifespan 90
	End
End

Condition
	On	Time
	Time	3

	Repeat	2
	RepeatJitter	1

	Event
		EName	Sparkfar
		Type 	Start
		At	Neck
		ChildFX	V_COV\PhysicsEnabled\ElectricSpark.fx
		Lifespan 90
	End
End

#############################################################

End