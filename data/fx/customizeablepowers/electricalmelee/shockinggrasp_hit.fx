#############################################################
## ShockingGrasp_Hit.fx
#############################################################

FxInfo

Input
	Inpname	Hips
End

ClampMaxScale 3.5 3.5 3.5

LifeSpan 120

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName 	HitFX
		Type	local
		At	Chest
		Part1	POWERS\MartialArts\MAHitStar01Sml.part
		Part2	POWERS\MartialArts\MAHitStar02Sml.part
		Part3	POWERS\MartialArts\MAHitStar03Sml.part
		Part4	POWERS\MartialArts\MAHitGlow01Sml.part
		Part5	POWERS\MartialArts\MAHitRays01Sml.part
		LifeSpan 10
	End

	Event
		EName	HandGlow1
		Type	Local
		At	Chest
		Part1	CustomizeablePowers\ElectricalMelee\ElectricityGlow3.part
		Part2	CustomizeablePowers\ElectricalMelee\ElectricityGlowStar3.part
		Part3	CustomizeablePowers\ElectricalMelee\ElectricityArch4a.part
		Part4	CustomizeablePowers\ElectricalMelee\ElectricityArchSmalla.part
		Sound elecballhit 80 80 .8
		Lifespan 23
	End

	Event
		EName	continuingArchs
		Type	Local
		At	Chest
		Part1	CustomizeablePowers\ElectricalMelee\ElectricityArchContinuing.part
		Part2	CustomizeablePowers\ElectricalMelee\ElectricityArchContinuing2.part
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
	Repeat	9

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