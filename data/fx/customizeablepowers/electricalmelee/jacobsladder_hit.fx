#############################################################
## HEADER
#############################################################

FxInfo

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
		Part2	CustomizeablePowers\Shared\MAHitStar03Sml.part
		Part1	CustomizeablePowers\Shared\MAHitStar01Sml.part
		Part2	CustomizeablePowers\Shared\MAHitStar02Sml.part
		Part4	CustomizeablePowers\Shared\MAHitGlow01Sml.part
		Part5	CustomizeablePowers\Shared\MAHitRays01Sml.part
		Sound elechavocpunch 80 80 0.8
		LifeSpan 10
	End

	Event
		EName	HandGlow1
		Type	Local
		At	Chest
		Part1	:ElectricityGlow3.part
		Part2	:ElectricityGlowStar3.part
		Part3	:ElectricityArch4a.part
		Part4	:ElectricityArchSmalla.part
		Sound elecfry 80 80 0.44
		Lifespan 23
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