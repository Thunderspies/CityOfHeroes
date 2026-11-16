#############################################################
## ThunderBoltHit.fx
#############################################################

FxInfo

Input
	Inpname	Hips
End

LifeSpan 170

#############################################################

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	Chest
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityGlow.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityGlowStar.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityArch4a.part
		Part4	CustomizeablePowers\ElectricalBlast\ElectricityArchSmalla.part
		Sound Elecballhit 80 80 .8
		Lifespan 40
	End

	Event
		Type	Local
		At	Chest
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityArchContinuing.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityArchContinuing2.part
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