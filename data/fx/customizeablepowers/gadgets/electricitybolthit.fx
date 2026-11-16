#########################################################
##	EBlast
########################################################

FxInfo
LifeSpan	90

#################################################################
##Hands
#################################################################



Condition
	On	Time
	Time	0

	Event
		EName 	HitFX
		Type	local
		At	Chest
		Part1	CustomizeablePowers\Gadgets\MAHitStar01Sml.part
		Part2	CustomizeablePowers\Gadgets\MAHitStar02Sml.part
		Part3	CustomizeablePowers\Gadgets\MAHitStar03Sml.part
		Part4	CustomizeablePowers\Gadgets\MAHitGlow01Sml.part
		Part5	CustomizeablePowers\Gadgets\MAHitRays01Sml.part
		LifeSpan	10

	End

	Event
		EName	HandGlow1
		Type	Local
		At	Chest
		Part1	CustomizeablePowers\Gadgets\ElectricityGlow.part
		Part2	CustomizeablePowers\Gadgets\ElectricityGlowStar.part
		Part3	CustomizeablePowers\Gadgets\ElectricityArch4a.part
		Part4	CustomizeablePowers\Gadgets\ElectricityArchSmalla.part
		Sound elecballhit 80 80 .8
	End

	Event
		EName	continuingArchs
		Type	Local
		At	Chest
		Part1	CustomizeablePowers\Gadgets\ElectricityArchContinuing.part
		Part2	CustomizeablePowers\Gadgets\ElectricityArchContinuing2.part
	End
End

	Condition
	On 	Time
	Time 	47
	Event
		EName 	HandGlow1
		Type	Destroy

	End

End


Condition
	On Time
	Time 5

	Repeat	8

	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	:LightningHit.fx

		Lifespan	90

	End

	Event
		HardwareOnly
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	:ElectricSpark.fx

		Lifespan	90

	End

End

Condition
	On Time
	Time 2

	Repeat	5

	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	:ElectricSpark.fx

		Lifespan	90

	End

	Event
		HardwareOnly
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	:ElectricSpark.fx

		Lifespan	90

	End
End


End
