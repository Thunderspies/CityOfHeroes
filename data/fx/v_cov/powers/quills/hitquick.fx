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
		Part1	:MustardGasHitA.part
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
		LifeSpan 30
	End

	Event
		Type	Local
		At	UArmL
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
		LifeSpan 25
	End

	Event
		Type	Local
		At	UArmR
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
		LifeSpan 25
	End

	Event
		EName	HeadConfusionClouds
		Type	Local
		At	Head
		Part1	:MustardGasHeadA.part
		Part2	:MustardGasSpecksHeadA.part
		Part3	:MustardGasBubblesHeadA.part
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