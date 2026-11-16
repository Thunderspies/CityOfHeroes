#############################################################
## LightningClap_Hit.fx
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
		Type	Local
		At	Chest
		Part1	:ElectricityGlow3.part
		Part2	:ElectricityGlowStar3.part
		Part3	:ElectricityArch4a.part
		Part4	:ElectricityArchSmalla.part
		Sound elec_clap_field 80 80 0.79
		Lifespan 23
	End

	Event
		Type	Local
		At	Chest
		Part1	:ElectricityArchContinuing.part
		Part2	:ElectricityArchContinuing2.part
	End

	Event
		Type	Local
		At	head
		Geom	LocalPivot01
		Part1	:HeadDizzies.part
		Part2	:HeadDizzyRings.part
		part3	:ElectricityMeleeGlowHit2.part
	End
End

#############################################################

Condition
	On	Time
	Time	5
	Repeat	2

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
	Repeat	6

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