#############################################################
## ElectricityBoltHit.fx
#############################################################

FxInfo
LifeSpan 90

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	HandGlow1
		Type	Local
		At	Chest
		Part1	:ElectricityGlow.part
		Part2	:ElectricityGlowStar.part
		Part3	:ElectricityArch4a.part
		Part4	:ElectricityArchSmalla.part
		Sound elecballhit 80 80 .8
		Lifespan 47
	End

	Event
		EName	continuingArchs
		Type	Local
		At	Chest
		Part1	:ElectricityArchContinuing.part
		Part2	:ElectricityArchContinuing2.part
	End
End

#############################################################

Condition
	On	Time
	Time	5
	Repeat	8

	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	:Child_LightningHit.fx
		Lifespan 90
	End

	Event
		HardwareOnly
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	:Child_ElectricSpark.fx
		Lifespan 90
	End
End

Condition
	On	Time
	Time	2
	Repeat	5

	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	:Child_ElectricSpark.fx
		Lifespan 90
	End

	Event
		HardwareOnly
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	:Child_ElectricSpark.fx
		Lifespan 90
	End
End

#############################################################

End