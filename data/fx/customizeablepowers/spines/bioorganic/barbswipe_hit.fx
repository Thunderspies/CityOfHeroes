#############################################################
## HEADER
#############################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

LifeSpan 40

#############################################################

Condition
	On 	Time
	Time 	0

	Event

		Type	Local
		At	chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Spines\Spines\RipperRays.part
		Part	CustomizeablePowers\Spines\Spines\RipperStar.part
		Part	CustomizeablePowers\Spines\Spines\RipperRing.part
		Part	CustomizeablePowers\Spines\Spines\MustardGasHitA.part
		Part	CustomizeablePowers\Spines\Spines\MustardGasSpecksHitA.part
		Part	CustomizeablePowers\Spines\Spines\MustardGasBubblesA.part
		Sound quillhit3 60 60 0.9
		LifeSpan 3
	End

	Event
		Type	Local
		At	Chest
		Part	POWERS\MartialArts\MAHitStar01.part
		Part	POWERS\MartialArts\MAHitStar02.part
		Part	POWERS\MartialArts\MAHitStar03.part
		Part	POWERS\MartialArts\MAHitGlow01.part
		Part	POWERS\MartialArts\MAHitRays01.part
		Lifespan 10
	End

	Event

		Type	Local
		At	UArmL
		Part	CustomizeablePowers\Spines\Spines\MustardGasSpecksHitA.part
		Part	CustomizeablePowers\Spines\Spines\MustardGasBubblesA.part
		LifeSpan 10
	End

	Event
		Type	Local
		At	UArmR
		Part	CustomizeablePowers\Spines\Spines\MustardGasSpecksHitA.part
		Part	CustomizeablePowers\Spines\Spines\MustardGasBubblesA.part
		LifeSpan 10
	End

	Event
		Type	Local
		At	Head
		Part	CustomizeablePowers\Spines\Spines\MustardGasSpecksHeadA.part
		Part	CustomizeablePowers\Spines\Spines\MustardGasBubblesHeadA.part
		LifeSpan 12
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
		Inherit	Position Scale
		Update	Position
		At	chest
		ChildFX	V_COV\PhysicsEnabled\PoisonSplash.fx
		Lifespan 75
	End

	Event
		HardwareOnly
		EName	GasSpurts
		Type	Create
		Inherit	Position Scale
		Update	Position
		At	chest
		ChildFX	V_COV\PhysicsEnabled\PoisonSplash.fx
		Lifespan 75
	End
End

#############################################################

Condition
	On 	Time
	Time 	28

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chest
		Part	POWERS\MartialArts\MAHitStar01.part
		Part	POWERS\MartialArts\MAHitStar02.part
		Part	POWERS\MartialArts\MAHitStar03.part
		Part	POWERS\MartialArts\MAHitGlow01.part
		Part	POWERS\MartialArts\MAHitRays01.part
		Lifespan 200
	End

	Event
		Type	Local
		At	chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Spines\Spines\RipperRays.part
		Part	CustomizeablePowers\Spines\Spines\RipperStar.part
		Part	CustomizeablePowers\Spines\Spines\RipperRing.part
		Part	CustomizeablePowers\Spines\Spines\MustardGasHitA.part
		Part	CustomizeablePowers\Spines\Spines\MustardGasSpecksHitA.part
		Part	CustomizeablePowers\Spines\Spines\MustardGasBubblesA.part
		Sound quillhit4 60 60 0.76
		LifeSpan 3
	End

	Event
		Type	Local
		At	UArmL
		Part	CustomizeablePowers\Spines\Spines\MustardGasSpecksHitA.part
		Part	CustomizeablePowers\Spines\Spines\MustardGasBubblesA.part
		Lifespan 10
	End

	Event
		Type	Local
		At	UArmR
		Part	CustomizeablePowers\Spines\Spines\MustardGasSpecksHitA.part
		Part	CustomizeablePowers\Spines\Spines\MustardGasBubblesA.part
		Lifespan 10
	End

	Event
		EName	HeadConfusionCloudsx
		Type	Local
		At	Head
		Part	CustomizeablePowers\Spines\Spines\MustardGasSpecksHeadA.part
		Part	CustomizeablePowers\Spines\Spines\MustardGasBubblesHeadA.part
		Lifespan 12
	End
End

#############################################################

Condition
	On 	Time
	Time 	28
	Repeat	9

	Event
		EName	GasSpurts
		Type	Create
		Inherit	Position Scale
		Update	Position
		At	chest
		ChildFX	V_COV\PhysicsEnabled\PoisonSplash.fx
		Lifespan 75
	End
End

#############################################################

End