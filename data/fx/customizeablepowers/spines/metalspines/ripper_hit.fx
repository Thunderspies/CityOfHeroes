#############################################################
## Ripper_Hit.fx
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
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Spines\Spines\RipperRays.part
		Part	CustomizeablePowers\Spines\Spines\RipperStar.part
		Part	CustomizeablePowers\Spines\Spines\RipperRing.part
		Part	CustomizeablePowers\Spines\Spines\MustardGasHitA.part
		Part	CustomizeablePowers\Spines\Spines\MustardGasSpecksHitA.part
		Part	CustomizeablePowers\Spines\Spines\MustardGasBubblesA.part
		Sound Quillhit5 70 70 0.7
		LifeSpan 6
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
		Type	Local
		At	Head
		Part	CustomizeablePowers\Spines\Spines\MustardGasSpecksHeadA.part
		Part	CustomizeablePowers\Spines\Spines\MustardGasBubblesHeadA.part
		Lifespan 12
	End

	Event

		EName	GasSpurts
		Type	Local
		At	chest
		Part	CustomizeablePowers\Spines\Spines\PoisonSpark.part
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