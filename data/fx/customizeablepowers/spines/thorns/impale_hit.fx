#############################################################
## HEADER
#############################################################

FxInfo

ClampMinScale 0.75 0.75 0.75
ClampMaxScale 1.25 1.25 1.25

LifeSpan 150

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
		Sound Darthit1 70 70 0.8
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Chest
		Part1	POWERS\MartialArts\MAHitStar01.part
		Part2	POWERS\MartialArts\MAHitStar02.part
		Part3	POWERS\MartialArts\MAHitStar03.part
		Part4	POWERS\MartialArts\MAHitGlow01.part
		Part5	POWERS\MartialArts\MAHitRays01.part
		Sound poisonhit2 50 50 0.7
		Lifespan 10
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	chest
		Part1	CustomizeablePowers\Spines\Thorns\MustardGasHitA.part
		Part2	CustomizeablePowers\Spines\Thorns\MustardGasSpecksHitA.part
		Part3	CustomizeablePowers\Spines\Thorns\MustardGasBubblesA.part
		LifeSpan 60
	End

	Event
		Type	Local
		At	UArmL
		Part2	CustomizeablePowers\Spines\Thorns\MustardGasSpecksHitA.part
		Part3	CustomizeablePowers\Spines\Thorns\MustardGasBubblesA.part
		LifeSpan 45
	End

	Event
		Type	Local
		At	UArmR
		Part2	CustomizeablePowers\Spines\Thorns\MustardGasSpecksHitA.part
		Part3	CustomizeablePowers\Spines\Thorns\MustardGasBubblesA.part
		LifeSpan 45
	End

	Event

		EName	HeadConfusionClouds
		Type	Local
		At	Head
		Part1	CustomizeablePowers\Spines\Thorns\MustardGasHeadA.part
		Part2	CustomizeablePowers\Spines\Thorns\MustardGasSpecksHeadA.part
		Part3	CustomizeablePowers\Spines\Thorns\MustardGasBubblesHeadA.part
		LifeSpan 45
	End
End

#############################################################

Condition
	On 	Time
	Time 	0
	Repeat	11

	Event
		EName	GasSpurts
		Type	Create
		Inherit	ALL
		Update	Position Rotation
		At	chest
		ChildFX	V_COV\PhysicsEnabled\PoisonDarkSplash.fx
		Lifespan 75
	End

	Event
		HardwareOnly
		EName	GasSpurts
		Type	Create
		Inherit	ALL
		Update	Position Rotation
		At	chest
		ChildFX	V_COV\PhysicsEnabled\PoisonDarkSplash.fx
		Lifespan 75
	End
End

#############################################################

Condition
	On 	Time
	Time 	0
	Repeat	4

	Event
		EName	GasHurts
		Type	Create
		Inherit	ALL
		Update	Position Rotation
		At	chest
		ChildFX	CustomizeablePowers\Spines\Thorns\Child_ThornsScatter.fx
		Lifespan 75
	End

	Event
		HardwareOnly
		EName	GasHurts
		Type	Create
		Inherit	ALL
		Update	Position Rotation
		At	chest
		ChildFX	CustomizeablePowers\Spines\Thorns\Child_ThornsScatter.fx
		Lifespan 75
	End
End

#############################################################

End