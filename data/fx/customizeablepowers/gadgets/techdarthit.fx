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
		bhvr	CustomizeablePowers\Gadgets\SciDartHit.bhvr
		Part2	:DartHitflashStar.part
		Part1	:ElectricityGlow.part
		Part2	:ElectricityGlowStar.part
		Lifespan 3
	End

	Event
		EName 	Dicharge
		Type	Local
		At	Neck
		bhvr	CustomizeablePowers\Gadgets\SciDartHit.bhvr
		Part2	:DartHitflashStar.part
		Part3	:ElectricityArch4a.part
		Part4	:ElectricityArchSmalla.part
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
		ChildFX	:ElectricSpark.fx
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
		ChildFX	:ElectricSpark.fx
		Lifespan 90
	End
End

#############################################################

End