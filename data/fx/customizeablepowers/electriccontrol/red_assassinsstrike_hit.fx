#########################################################
##	EBlast
########################################################
FxInfo

LifeSpan	120

## HANDS ###########################################################

Condition
	On	Time
	Time	0

	Event
		EName 	HitFX
		Type	local
		At	Chest
		Part1	Powers\MartialArts\MAHitStar01Sml.part
		Part2	Powers\MartialArts\MAHitStar02Sml.part
		Part3	Powers\MartialArts\MAHitStar03Sml.part
		Part4	Powers\MartialArts\MAHitGlow01Sml.part
		Part5	Powers\MartialArts\MAHitRays01Sml.part
		LifeSpan 10
	End

	Event
		EName	HandGlow1
		Type	Local
		At	Chest
		Part1	Powers\ElectricityControl\Red_ElectricityGlow3Red.part
		Part2	Powers\ElectricityControl\Red_ElectricityGlowStar3Red.part
		Part3	Powers\ElectricityControl\Red_ElectricityArch4aRed.part
		Part4	Powers\ElectricityControl\Red_ElectricityArchSmallaRed.part
		Part	Powers\ElectricityControl\Red_ElectricityArchContinuingWhite.part
		Sound elechit5 80 80 .8
		LifeSpan 23
	End

	Event
		EName	continuingArchs
		Type	Local
		At	Chest
		Part1	Powers\ElectricityControl\Red_ElectricityArchContinuingRed.part
		Part2	Powers\ElectricityControl\Red_ElectricityArchContinuing2Red.part
		Sound elec_arcs1 60 60 .75
		LifeSpan 50
	End
End

#############################################################

Condition
	On	Time
	Time	5
	Repeat	4

	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\RedLightningHit.fx
		Lifespan	90
	End

	Event
		HardwareOnly
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\RedElectricSpark.fx
		Lifespan	90
	End
End

Condition
	On	Time
	Time	2
	Repeat	9

	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\RedElectricSpark.fx
		Lifespan	90
	End

	Event
		HardwareOnly
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\RedElectricSpark.fx
		Lifespan	90
	End
End

#############################################################

End