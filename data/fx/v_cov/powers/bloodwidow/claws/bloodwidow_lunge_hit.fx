#############################################################
## Blood Widow "Lunge" Hit FX
#############################################################

FxInfo

LifeSpan 100

#############################################################

Condition
	On 	Time
	Time 	0

	Event

		EName	Gas
		Type	Local
		At	chest
		Part1	powers\PoisonControl\MustardGasHitA.part
		Part2	powers\PoisonControl\MustardGasSpecksHitA.part
		Part3	powers\PoisonControl\MustardGasBubblesA.part
		Sound Quills3 70 70 .98
		Lifespan 10
	End

	Event
		EName	Gas1
		Type	Local
		At	UArmL
		Part2	powers\PoisonControl\MustardGasSpecksHitA.part
		Part3	powers\PoisonControl\MustardGasBubblesA.part
		Sound PoisonHit2 70 70 .66
		Lifespan 10
	End

	Event
		EName	Gas2
		Type	Local
		At	UArmR
		Part2	powers\PoisonControl\MustardGasSpecksHitA.part
		Part3	powers\PoisonControl\MustardGasBubblesA.part
		Lifespan 10
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	HeadConfusionClouds
		Type	Local
		At	Head
		Part2	powers\PoisonControl\MustardGasSpecksHeadA.part
		Part3	powers\PoisonControl\MustardGasBubblesHeadA.part
		Lifespan 12
	End
End

#############################################################

Condition
	On 	Time
	Time 	0
	Repeat	6

	Event
		EName	GasSpurts
		Type	Local
		At	chest
		ChildFX	V_COV\PhysicsEnabled\PoisonSplash.fx
		Lifespan	75
	End
End

#############################################################

Condition
	On 	Time
	Time 	0
	Repeat	6

	Event
		HardwareOnly
		EName	GasSpurts
		Type	Local
		At	chest
		ChildFX	V_COV\PhysicsEnabled\PoisonSplash.fx
		Lifespan	75
	End
End

#############################################################

End