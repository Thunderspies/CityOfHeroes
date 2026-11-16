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
		Part1	:ElectricityGlow3.part
		Part2	:ElectricityGlowStar3.part
		Part3	:ElectricityArch4a.part
		Part4	:ElectricityArchSmalla.part
		Sound elechit5 80 80 0.8
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
		Type 	Start
		At	Chest
		ChildFX	:Child_LightningHit.fx
		Lifespan 90
	End

	Event
		HardwareOnly
		Type 	Start
		At	Chest
		ChildFX	:Child_LightningHit.fx
		Lifespan 90
	End
End

Condition
	On	Time
	Time	2
	Repeat	9

	Event
		Type 	Start
		At	Chest
		ChildFX	:Child_ElectricSparkHit.fx
		Lifespan 90
	End

	Event
		HardwareOnly
		Type 	Start
		At	Chest
		ChildFX	:Child_ElectricSparkHit.fx
		Lifespan 90
	End
End

#############################################################

End