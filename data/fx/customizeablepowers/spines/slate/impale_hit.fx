#############################################################
## HEADER
#############################################################

FxInfo

ClampMinScale 0.75 0.75 0.75
ClampMaxScale 1.25 1.25 1.25

LifeSpan 300

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
		Sound Quills6 70 70 0.7
		Lifespan 1
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
		Part	POWERS\MartialArts\MAHitStar01.part
		Part	POWERS\MartialArts\MAHitStar02.part
		Part	POWERS\MartialArts\MAHitStar03.part
		Part	POWERS\MartialArts\MAHitGlow01.part
		Part	POWERS\MartialArts\MAHitRays01.part
		Sound poisonhit2 50 50 0.7
		Lifespan 200
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Chest
		Part	CustomizeablePowers\Spines\Spines\MustardGasHitA.part
		Part	CustomizeablePowers\Spines\Spines\MustardGasSpecksHitA.part
		Part	CustomizeablePowers\Spines\Spines\MustardGasBubblesA.part
		LifeSpan 30
	End

	Event
		Type	Local
		At	UArmL
		Part	CustomizeablePowers\Spines\Spines\MustardGasSpecksHitA.part
		Part	CustomizeablePowers\Spines\Spines\MustardGasBubblesA.part
		LifeSpan 25
	End

	Event
		Type	Local
		At	UArmR
		Part	CustomizeablePowers\Spines\Spines\MustardGasSpecksHitA.part
		Part	CustomizeablePowers\Spines\Spines\MustardGasBubblesA.part
		LifeSpan 25
	End

	Event
		Type	Local
		At	Head
		Part	CustomizeablePowers\Spines\Spines\MustardGasHeadA.part
		Part	CustomizeablePowers\Spines\Spines\MustardGasSpecksHeadA.part
		Part	CustomizeablePowers\Spines\Spines\MustardGasBubblesHeadA.part
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
		ChildFX	:Child_ThornsScatter.fx
		Lifespan 75
	End

	Event
		HardwareOnly
		EName	GasHurts
		Type	Create
		Inherit	ALL
		Update	Position Rotation
		At	chest
		ChildFX	:Child_ThornsScatter.fx
		Lifespan 75
	End
End

#############################################################

End