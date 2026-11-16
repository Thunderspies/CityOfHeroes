#############################################################
## HEADER
#############################################################

FxInfo

ClampMinScale 0.75 0.75 0.75
ClampMaxScale 1.25 1.25 1.25

LifeSpan 50

#############################################################

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
		Sound poisonhit2 50 50 0.7
		Lifespan 200
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Gas
		Type	Local
		At	chest
		Part1	CustomizeablePowers\Spines\Thorns\MustardGasHitA.part
		Part2	CustomizeablePowers\Spines\Thorns\MustardGasSpecksHitA.part
		Part3	CustomizeablePowers\Spines\Thorns\MustardGasBubblesA.part
		LifeSpan 30
	End

	Event
		EName	Gas1
		Type	Local
		At	UArmL
		Part2	CustomizeablePowers\Spines\Thorns\MustardGasSpecksHitA.part
		Part3	CustomizeablePowers\Spines\Thorns\MustardGasBubblesA.part
		LifeSpan 25
	End

	Event
		EName	Gas2
		Type	Local
		At	UArmR
		Part2	CustomizeablePowers\Spines\Thorns\MustardGasSpecksHitA.part
		Part3	CustomizeablePowers\Spines\Thorns\MustardGasBubblesA.part
		LifeSpan 25
	End

	Event
		EName	HeadConfusionClouds
		Type	Local
		At	Head
		Part1	CustomizeablePowers\Spines\Thorns\MustardGasHeadA.part
		Part2	CustomizeablePowers\Spines\Thorns\MustardGasSpecksHeadA.part
		Part3	CustomizeablePowers\Spines\Thorns\MustardGasBubblesHeadA.part
		LifeSpan 25
	End
End

#############################################################

Condition
	On 	Time
	Time 	0
	Repeat	11

	Event
		EName	GasSpurts
		Type	Local
		At	chest

		ChildFX	V_COV\PhysicsEnabled\PoisonDarkSplash.fx
		Lifespan 75
	End

	Event
		HardwareOnly
		EName	GasSpurts
		Type	Local
		At	chest
		ChildFX	V_COV\PhysicsEnabled\PoisonDarkSplash.fx
		Lifespan 75
	End
End

#############################################################

End