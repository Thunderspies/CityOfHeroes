#########################################################
## Veteran Pet Drone - Explosion
########################################################

FxInfo

LifeSpan 90

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	Hips
		Part1	Powers\MartialArts\MAHitStar01Sml.part
		Part2	Powers\MartialArts\MAHitStar02Sml.part
		Part3	Powers\MartialArts\MAHitStar03Sml.part
		Part4	Powers\MartialArts\MAHitGlow01Sml.part
		Part5	Powers\MartialArts\MAHitRays01Sml.part
		LifeSpan 10
	End

	Event
		Type	Local
		At	Hips
		Part1	Powers\ElectricityControl\ElectricityArchContinuing.part
		Part2	Powers\ElectricityControl\ElectricityArchContinuing2.part
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Hips
		Part1	Powers\ElectricityControl\ElectricityGlow.part
		Part2	Powers\ElectricityControl\ElectricityGlowStar.part
		Part3	Powers\ElectricityControl\ElectricityArch4a.part
		Part4	Powers\ElectricityControl\ElectricityArchSmalla.part
		Sound elecballhit 80 80 .8
		LifeSpan 47
	End
End

#############################################################

Condition
	On	Time
	Time	2
	Repeat	5

	Event
		Type 	Start
		At	Hips
		ChildFX	V_COV\PhysicsEnabled\ElectricSpark.fx
		Lifespan 90
	End

	Event
		HardwareOnly
		Type 	Start
		At	Hips
		ChildFX	V_COV\PhysicsEnabled\ElectricSpark.fx
		Lifespan 90
	End
End

Condition
	On	Time
	Time	5
	Repeat	8

	Event
		Type 	Start
		At	Hips
		ChildFX	V_COV\PhysicsEnabled\LightningHit.fx
		Lifespan 90
	End

	Event
		HardwareOnly
		Type 	Start
		At	Hips
		ChildFX	V_COV\PhysicsEnabled\ElectricSpark.fx
		Lifespan 90
	End
End

#############################################################

End