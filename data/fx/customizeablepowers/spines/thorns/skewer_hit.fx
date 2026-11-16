#############################################################
## HEADER
#############################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

LifeSpan 70

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
		At	chest
		Sound Darthit1 70 70 .7
	End
End

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
		Lifespan 200
	End

	Event
		Type	Local
		At	chest
		Part1	CustomizeablePowers\Spines\Thorns\MustardGasHitA.part
		Part2	CustomizeablePowers\Spines\Thorns\MustardGasSpecksHitA.part
		Part3	CustomizeablePowers\Spines\Thorns\MustardGasBubblesA.part
		LifeSpan 30
	End

	Event
		Type	Local
		At	UArmL
		Part2	CustomizeablePowers\Spines\Thorns\MustardGasSpecksHitA.part
		Part3	CustomizeablePowers\Spines\Thorns\MustardGasBubblesA.part
		LifeSpan 25
	End

	Event
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