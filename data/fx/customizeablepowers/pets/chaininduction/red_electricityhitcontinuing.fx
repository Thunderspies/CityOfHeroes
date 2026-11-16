#############################################################
## Red_ElectricityHitContinuing.fx
#############################################################

FxInfo

#########################################################

Input
	Inpname	Hips
End

#########################################################




Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	hips

		Sound shackleshit 70 70 .6
	End

	On 	Time
	Time 	0

	Event
		Type	Local
		At	hips

		Sound eleccage_loop 70 70 .4
	End

	Event
		EName 	HitFX
		Type	local
		At	Chest
		Part1	Powers/MartialArts/MAHitStar01Sml.part
		Part2	Powers/MartialArts/MAHitStar02Sml.part
		Part3	Powers/MartialArts/MAHitStar03Sml.part
		Part4	Powers/MartialArts/MAHitGlow01Sml.part
		Part5	Powers/MartialArts/MAHitRays01Sml.part
		Part	Powers/MartialArts/MartialArtsHitRaysSmall.part
		LifeSpan	10

	End

	Event
		EName	HandGlow1
		Type	Local
		At	hips
		Part1	:red_ElectricityGlow3Red.part
		Part2	:red_ElectricityGlowStar3Reda.part
		Part3	:red_ElectricityArchContinuingRed.part
		Part3	:red_ElectricityArchContinuing2Red.part
		Part3	:red_ElectricityArch.part
	End

	Event
		EName	HandGlow1
		Type	Local
		At	Chest
		Part1	:red_ElectricityGlow3Red.part
		Part2	:red_ElectricityGlowStar3Reda.part
		Part3	:red_ElectricityArchContinuingRed.part
		Part3	:red_ElectricityArchContinuing2Red.part


	End

	Event
		EName	HandGlow1
		Type	Local
		At	Head

		Part	:red_ElectricityGlowSmall.part
		Part	:red_ElectricityGlowStarSmall.part
		Part1	:red_ElectricityArchContinuing.part
		Part2	:red_ElectricityArchContinuing2.part
		Part3	:red_ElectricityArchContinuingRed.part
		Part3	:red_ElectricityArchContinuing2Red.part

	End

	Event
		EName	continuingArchs
		Type	Local
		At	hips
		Part1	:red_ElectricityArchContinuing.part
		Part2	:red_ElectricityArchContinuing2.part
		Part3	:red_ElectricityArchContinuing2Red.part
		Part	:red_ElectricityArchContinuingRed.part
	End

	Event
		EName	continuingArchs
		Type	Local
		At	UlegL
		Part1	:red_ElectricityArchContinuing.part
		Part2	:red_ElectricityArchContinuing2.part
		Part3	:red_ElectricityArchContinuing2Red.part
		Part3	:red_ElectricityArchContinuingRed.part
		Part	:Red_ElectricitySparkGlow.part
		Part	:Red_ElectricitySparkBigGlow.part
	End

	Event
		EName	continuingArchs
		Type	Local
		At	UlegR
		Part1	:red_ElectricityArchContinuing.part
		Part2	:red_ElectricityArchContinuing2.part
		Part3	:red_ElectricityArchContinuing2Red.part
		Part3	:red_ElectricityArchContinuingRed.part
		Part	:Red_ElectricitySparkGlow.part
		Part	:Red_ElectricitySparkBigGlow.part
	End

	Event
		EName	continuingArchs
		Type	Local
		At	LlegR
		Part	:red_ElectricityGlowSmall.part
		Part	:red_ElectricityGlowStarSmall.part
		Part1	:red_ElectricityArchContinuing.part
		Part2	:red_ElectricityArchContinuing2.part
		Part3	:red_ElectricityArchContinuing2Red.part
		Part3	:red_ElectricityArchContinuingRed.part
		Part	:Red_ElectricitySparkGlow.part
		Part	:Red_ElectricitySparkBigGlow.part
		Part2	:red_ElectricityGlowStar3Reda2.part
	End

	Event
		EName	continuingArchs
		Type	Local
		At	LlegL
		Part	:red_ElectricityGlowSmall.part
		Part	:red_ElectricityGlowStarSmall.part
		Part1	:red_ElectricityArchContinuing.part
		Part2	:red_ElectricityArchContinuing2.part
		Part3	:red_ElectricityArchContinuing2Red.part
		Part3	:red_ElectricityArchContinuingRed.part
		Part	:Red_ElectricitySparkGlow.part
		Part	:Red_ElectricitySparkBigGlow.part
		Part2	:red_ElectricityGlowStar3Reda2.part
	End

	Event
		EName	continuingArchs
		Type	Local
		At	FootL
		Part	:red_ElectricityGlowSmall.part
		Part	:red_ElectricityGlowStarSmall.part
		Part1	:red_ElectricityArchContinuing.part
		Part2	:red_ElectricityArchContinuing2.part
		Part3	:red_ElectricityArchContinuing2Red.part
		Part3	:red_ElectricityArchContinuingRed.part
	End

	Event
		EName	continuingArchs
		Type	Local
		At	FootR
		Part	:red_ElectricityGlowSmall.part
		Part	:red_ElectricityGlowStarSmall.part
		Part1	:red_ElectricityArchContinuing.part
		Part2	:red_ElectricityArchContinuing2.part
		Part3	:red_ElectricityArchContinuing2Red.part
		Part3	:red_ElectricityArchContinuingRed.part
	End

	Event
		EName	continuingArchs
		Type	Local
		At	FootL
		Part1	:Red_ElectricityArchContinuingz.part
		Part2	:Red_ElectricityArchContinuing2z.part
		Part1	:Red_ElectricityArchContinuingzWhite.part
		Part2	:Red_ElectricityArchContinuing2zWhite.part
		Part	:Red_ElectricitySparkGlow.part
		Part	:Red_ElectricitySparkBigGlow.part
		Part	:Red_ElectricitySparkStar.part
		Part	:Red_ElectricitySparkRing.part
	End

	Event
		EName	continuingArchs
		Type	Local
		At	FootR
		Part1	:Red_ElectricityArchContinuingz.part
		Part2	:Red_ElectricityArchContinuing2z.part
		Part1	:Red_ElectricityArchContinuingzWhite.part
		Part2	:Red_ElectricityArchContinuing2zWhite.part
		Part	:Red_ElectricitySparkGlow.part
		Part	:Red_ElectricitySparkBigGlow.part
		Part	:Red_ElectricitySparkStar.part
		Part	:Red_ElectricitySparkRing.part
	End
End

Condition
	On Time
	Time 5

	Repeat	4

	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	CustomizeablePowers\ElectricalMelee\Child_LightningHit.fx

		Lifespan	90

	End

End

Condition
	On Time
	Time 2

	Repeat	6

	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	CustomizeablePowers\ElectricalMelee\Child_ElectricSpark.fx
		Lifespan	90
	End
End


Condition
	On Cycle
	Time 17

	Chance	.4

	Event
		SoftwareOnly
		EName	Sparkfar
		Type 	Local
		At	Chest
		ChildFX	CustomizeablePowers\ElectricalMelee\Child_LightningHit.fx

		Lifespan	15

	End

End


Condition
	On Cycle
	Time 23

	Chance	.5

	Event
		HardwareOnly
		EName	Sparkfar
		Type 	Local
		At	Chest
		ChildFX	CustomizeablePowers\ElectricalMelee\Child_ElectricSpark.fx

		Lifespan	15

	End

End


Condition
	On Cycle
	Time 7

	Chance	.5

	Event
		HardwareOnly
		EName	Sparkfar
		Type 	Local
		At	Chest
		ChildFX	CustomizeablePowers\ElectricalMelee\Child_ElectricSpark.fx

		Lifespan	15

	End

End

End