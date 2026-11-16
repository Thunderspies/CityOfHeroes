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
		Part1	Powers\MartialArts\MAHitStar01.part
		Part2	Powers\MartialArts\MAHitStar02.part
		Part3	Powers\MartialArts\MAHitStar03.part
		Part4	Powers\MartialArts\MAHitGlow01.part
		Part5	Powers\MartialArts\MAHitRays01.part
		Lifespan 200
	End

	Event
		Type	Local
		At	chest
		Part	:RipperRays.part
		Part	:RipperStar.part
		Part	:RipperRing.part
		Part1	:MustardGasHitA.part
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
		Sound Quillhit5 70 70 .7
		Bhvr	Behaviors\GenericParticleFade.bhvr
		LifeSpan	6
	End

	Event
		Type	Local
		At	UArmL
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
		Lifespan 10
	End

	Event
		Type	Local
		At	UArmR
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
		Lifespan 10
	End

	Event
		Type	Local
		At	Head
		Part2	:MustardGasSpecksHeadA.part
		Part3	:MustardGasBubblesHeadA.part
		Lifespan 12
	End

	Event

		EName	GasSpurts
		Type	Local
		At	chest
		Part1	:PoisonSpark.part
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