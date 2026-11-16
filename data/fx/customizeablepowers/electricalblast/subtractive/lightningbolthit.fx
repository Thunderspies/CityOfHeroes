#############################################################
## LightningBoltHit.fx
#############################################################

FxInfo
LifeSpan 120

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		Part1	:ElectricityGlow.part
		Part2	:ElectricityGlowStar.part
		Part3	:ElectricityArch4a.part
		Part4	:ElectricityArchSmalla.part
		Sound lightningbolthit 100 100 .7
		Lifespan 24
	End

	Event
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
		Type 	Start
		At	Chest
		ChildFX	:Child_LightningHit.fx
		Lifespan 90
	End

	Event
		HardwareOnly
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
		Type 	Start
		At	Chest
		ChildFX	:Child_ElectricSpark.fx
		Lifespan 90
	End

	Event
		HardwareOnly
		Type 	Start
		At	Chest
		ChildFX	:ElectricSpark.fx
		Lifespan 90
	End
End

#############################################################

End