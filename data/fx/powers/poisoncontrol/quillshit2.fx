#############################################################
## HEADER
#############################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

LifeSpan 40

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
		Part	:RipperRays.part
		Part	:RipperStar.part
		Part	:RipperRing.part
		Part1	:MustardGasHitA.part
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
		Sound quillhit3 60 60 .9
		Bhvr	Behaviors\GenericParticleFade.bhvr
		LifeSpan 3
	End

	Event
		Type	Local
		At	Chest
		Part1	Powers\MartialArts\MAHitStar01.part
		Part2	Powers\MartialArts\MAHitStar02.part
		Part3	Powers\MartialArts\MAHitStar03.part
		Part4	Powers\MartialArts\MAHitGlow01.part
		Part5	Powers\MartialArts\MAHitRays01.part
		Lifespan 10
	End

	Event

		Type	Local
		At	UArmL
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
		LifeSpan 10
	End

	Event
		Type	Local
		At	UArmR
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
		LifeSpan 10
	End

	Event
		Type	Local
		At	Head
		Part2	:MustardGasSpecksHeadA.part
		Part3	:MustardGasBubblesHeadA.part
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
#		Part1	Powers\MartialArts\MAHitStar01.part
#		Part2	Powers\MartialArts\MAHitStar02.part
	#	Part3	Powers\MartialArts\MAHitStar03.part
	#	Part4	Powers\MartialArts\MAHitGlow01.part
	#	Part5	Powers\MartialArts\MAHitRays01.part
		Lifespan 200
	End

	Event
		Type	Local
		At	chest
#		Part	:RipperRays.part
#		Part	:RipperStar.part
#		Part	:RipperRing.part
#		Part1	:MustardGasHitA.part
#		Part2	:MustardGasSpecksHitA.part
#		Part3	:MustardGasBubblesA.part
		Sound quillhit4 60 60 .76
		Bhvr	Behaviors\GenericParticleFade.bhvr
		LifeSpan 3
	End

	Event
		Type	Local
		At	UArmL
#		Part2	:MustardGasSpecksHitA.part
#		Part3	:MustardGasBubblesA.part
		Lifespan 10
	End

	Event
		Type	Local
		At	UArmR
#		Part3	:MustardGasBubblesA.part
#		Lifespan 10
	End

	Event
		EName	HeadConfusionCloudsx
		Type	Local
		At	Head
	#	Part2	:MustardGasSpecksHeadA.part
	#	Part3	:MustardGasBubblesHeadA.part
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
#		ChildFX	V_COV\PhysicsEnabled\PoisonSplash.fx
		Lifespan 75
	End
End

#############################################################

End