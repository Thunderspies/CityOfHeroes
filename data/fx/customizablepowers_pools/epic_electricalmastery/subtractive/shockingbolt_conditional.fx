#########################################################
##	forceBubble
########################################################
FxInfo

Input
	Inpname	Hips
End

LifeSpan	200

########################################################


######################################################################################



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
		Part1	CustomizeablePowers\ElectricalMelee\Subtractive\MAHitGlow01Sml.part
		Part2	CustomizeablePowers\ElectricalMelee\Subtractive\MAHitStar02Sml.part
		Part3	CustomizeablePowers\ElectricalMelee\Subtractive\MAHitStar03Sml.part
		Part4	CustomizeablePowers\ElectricalMelee\Subtractive\MAHitGlow01Sml.part
		Part5	CustomizeablePowers\ElectricalMelee\Subtractive\MAHitRays01Sml.part
		Part	:MartialArtsHitRaysSmall.part
		LifeSpan	10

	End

	Event
		EName	HandGlow1
		Type	Local
		At	hips
		Part1	:ElectricityGlow2.part
		Part2	:ElectricityGlowStar2.part
		Part3	:ShaklesElectricityArch.part
		Part3	:ShaklesElectricityArch2.part
		Part3	:FenceElectricityArch1.part
	End

	Event
		EName	HandGlow1
		Type	Local
		At	Chest
		Part1	:ElectricityGlow2.part
		Part2	:ElectricityGlowStar2.part
		Part3	:ShaklesElectricityArch.part
		Part3	:ShaklesElectricityArch2.part


	End

	Event
		EName	HandGlow1
		Type	Local
		At	Head

		Part	:ElectricityGlowSmall.part
		Part	:ElectricityGlowStarSmall.part
		Part1	:ElectricityArchContinuing.part
		Part2	:ElectricityArchContinuing2.part
		Part3	:ShaklesElectricityArch.part
		Part3	:ShaklesElectricityArch2.part

	End

	Event
		EName	continuingArchs
		Type	Local
		At	hips
		Part1	:ElectricityArchContinuing.part
		Part2	:ElectricityArchContinuing2.part
		Part3	:ShaklesElectricityArch2.part
		Part	:ShaklesElectricityArch.part
	End

	Event
		EName	continuingArchs
		Type	Local
		At	UlegL
		Part1	:ElectricityArchContinuing.part
		Part2	:ElectricityArchContinuing2.part
		Part3	:ShaklesElectricityArch2.part
		Part3	:ShaklesElectricityArch.part
	End

	Event
		EName	continuingArchs
		Type	Local
		At	UlegR
		Part1	:ElectricityArchContinuing.part
		Part2	:ElectricityArchContinuing2.part
		Part3	:ShaklesElectricityArch2.part
		Part3	:ShaklesElectricityArch.part
	End

	Event
		EName	continuingArchs
		Type	Local
		At	LlegR
		Part	:ElectricityGlowSmall.part
		Part	:ElectricityGlowStarSmall.part
		Part1	:ElectricityArchContinuing.part
		Part2	:ElectricityArchContinuing2.part
		Part3	:ShaklesElectricityArch2.part
		Part3	:ShaklesElectricityArch.part
	End

	Event
		EName	continuingArchs
		Type	Local
		At	LlegL
		Part	:ElectricityGlowSmall.part
		Part	:ElectricityGlowStarSmall.part
		Part1	:ElectricityArchContinuing.part
		Part2	:ElectricityArchContinuing2.part
		Part3	:ShaklesElectricityArch2.part
		Part3	:ShaklesElectricityArch.part
	End

	Event
		EName	continuingArchs
		Type	Local
		At	FootL
		Part	:ElectricityGlowSmall.part
		Part	:ElectricityGlowStarSmall.part
		Part1	:ElectricityArchContinuing.part
		Part2	:ElectricityArchContinuing2.part
		Part3	:ShaklesElectricityArch2.part
		Part3	:ShaklesElectricityArch.part
	End

	Event
		EName	continuingArchs
		Type	Local
		At	FootR
		Part	:ElectricityGlowSmall.part
		Part	:ElectricityGlowStarSmall.part
		Part1	:ElectricityArchContinuing.part
		Part2	:ElectricityArchContinuing2.part
		Part3	:ShaklesElectricityArch2.part
		Part3	:ShaklesElectricityArch.part
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
		ChildFX	CustomizeablePowers\ElectricalBlast\Subtractive\Child_LightningHit.fx

		Lifespan	90

	End

	Event
		HardwareOnly
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	CustomizeablePowers\ElectricalBlast\Subtractive\Child_ElectricSpark.fx
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
		ChildFX	CustomizeablePowers\ElectricalBlast\Subtractive\Child_ElectricSpark.fx
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
		ChildFX	CustomizeablePowers\ElectricalBlast\Subtractive\Child_LightningHit.fx

		Lifespan	15

	End

End


Condition
	On Cycle
	Time 13

	Chance	.6

	Event
		HardwareOnly
		EName	Sparkfar
		Type 	Local
		At	Chest
		ChildFX	CustomizeablePowers\ElectricalBlast\Subtractive\Child_ElectricSpark.fx

		Lifespan	15

	End

End


Condition
	On Cycle
	Time 11

	Chance	.5

	Event
		HardwareOnly
		EName	Sparkfar
		Type 	Local
		At	Chest
		ChildFX	CustomizeablePowers\ElectricalBlast\Subtractive\Child_ElectricSpark.fx
		Lifespan	15

	End

End


End