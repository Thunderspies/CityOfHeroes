#############################################################
## Villain Electircal Blast - Aim
#############################################################

FxInfo

Lifespan 90

## GENERIC AIM FX ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		Sound aim2 60 60 .6
	End

	Event
		EName	Pow2
		Type 	local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	Powers\YellowUpGlow.part
		Part2	Powers\YellowUpSpecks2.part
		Lifespan 13
	End
End

Condition
	On	Time
	Time	10

	Event
		EName	Pow
		Type 	local
		At	Chest
		Part2	Powers\YellowUpRing01.part
		Part3	Powers\YellowUpRing01a.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 65
	End
End

Condition
	On	Time
	Time	20

	Event
		Type 	local
		At	Chest
		Part3	Powers\YellowUpRing02.part
		Part4	Powers\YellowUpRing02a.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 55
	End
End

Condition
	On	Time
	Time	30

	Event
		Type 	local
		At	Chest
		Part2	Powers\YellowUpRingSlow.part
		Part3	Powers\YellowUpRing03.part
		Part4	Powers\YellowUpRing03a.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 45
	End
End

## ELECTRICAL FX ###########################################################

Condition
	On	Time
	Time	0

	Event
		EName	HandGlow1
		Type	Local
		At	Chest
		Part1	:ElectricityGlow3.part
		Part2	:ElectricityGlowStar3.part
		Part3	:ElectricityArch4a.part
		Part4	:ElectricityArchSmalla.part
		Sound pp12 50 50 .7
		LifeSpan 70
	End

	Event
		EName	continuingArchs
		Type	Local
		At	Chest
		Part1	:ElectricityArchContinuing.part
		Part2	:ElectricityArchContinuing2.part
		LifeSpan 70
	End
End

## PHYSICS ###########################################################

Condition
	On	Time
	Time	2
	Repeat	2

	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\RedElectricSparkSmall.fx
		Lifespan 120
		LifeSpanJitter 80
	End
End

Condition
	On	Time
	Time	2
	Repeat	2

	Event
		HardwareOnly
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\RedElectricSparkSmall.fx
		Lifespan 120
		LifeSpanJitter 80
	End
End

Condition
	On	Time
	Time	2
	Repeat	2

	Event
		HardwareOnly
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\RedElectricSparkSmall.fx
		Lifespan 120
		LifeSpanJitter 80

	End
End

#############################################################

End