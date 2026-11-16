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
		Type	local
		At	Chest
		Part1	Powers\MartialArts\MAHitStar01Sml.part
		Part2	Powers\MartialArts\MAHitStar02Sml.part
		Part3	Powers\MartialArts\MAHitStar03Sml.part
		Part4	Powers\MartialArts\MAHitGlow01Sml.part
		Part5	Powers\MartialArts\MAHitRays01Sml.part
		LifeSpan	10
	End

	Event
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
		ChildFX	V_COV\PhysicsEnabled\Red_LightningHit.fx
		Lifespan 90
	End

	Event
		HardwareOnly
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\Red_ElectricSpark.fx
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
		ChildFX	V_COV\PhysicsEnabled\Red_ElectricSpark.fx
		Lifespan 90
	End

	Event
		HardwareOnly
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\Red_ElectricSpark.fx
		Lifespan 90
	End
End

#############################################################

End