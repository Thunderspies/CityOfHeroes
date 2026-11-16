#############################################################
## HEADER
#############################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

LifeSpan 100

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chest
		Part1	POWERS\MartialArts\MAHitStar01.part
		Part2	POWERS\MartialArts\MAHitStar02.part
		Part3	POWERS\MartialArts\MAHitStar03.part
		Part4	POWERS\MartialArts\MAHitGlow01.part
		Part5	POWERS\MartialArts\MAHitRays01.part
		Lifespan 200
	End

	Event
		Type	Local
		At	chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Spines\Thorns\RipperRays.part
		Part	CustomizeablePowers\Spines\Thorns\RipperStar.part
		Part	CustomizeablePowers\Spines\Thorns\RipperRing.part
		Part1	CustomizeablePowers\Spines\Thorns\MustardGasHitA.part
		Part2	CustomizeablePowers\Spines\Thorns\MustardGasSpecksHitA.part
		Part3	CustomizeablePowers\Spines\Thorns\MustardGasBubblesA.part
		Sound Quillhit5 70 70 0.7
		LifeSpan 6
	End

	Event
		Type	Local
		At	UArmL
		Part2	CustomizeablePowers\Spines\Thorns\MustardGasSpecksHitA.part
		Part3	CustomizeablePowers\Spines\Thorns\MustardGasBubblesA.part
		Lifespan 10
	End

	Event
		Type	Local
		At	UArmR
		Part2	CustomizeablePowers\Spines\Thorns\MustardGasSpecksHitA.part
		Part3	CustomizeablePowers\Spines\Thorns\MustardGasBubblesA.part
		Lifespan 10
	End

	Event
		Type	Local
		At	Head
		Part2	CustomizeablePowers\Spines\Thorns\MustardGasSpecksHeadA.part
		Part3	CustomizeablePowers\Spines\Thorns\MustardGasBubblesHeadA.part
		Lifespan 12
	End

	Event

		EName	GasSpurts
		Type	Local
		At	chest
		Part1	CustomizeablePowers\Spines\Thorns\PoisonSpark.part
		Lifespan 1
	End
End

#############################################################

Condition
	On 	Time
	Time 	0
	Repeat	11

	Event
		Type	Local
		At	chest
		ChildFX	V_COV\PhysicsEnabled\PoisonSplash.fx
		Lifespan 75
	End

	Event
		HardwareOnly
		Type	Local
		At	chest
		ChildFX	V_COV\PhysicsEnabled\PoisonSplash.fx
		Lifespan 75
	End
End

#############################################################

End