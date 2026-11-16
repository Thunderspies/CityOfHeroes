#############################################################
## Blood Widow "Poison Dart" Hit FX
#############################################################

FxInfo

ClampMaxScale	1.25 1.25 1.25
ClampMaxScale	0.75 0.75 0.75

LifeSpan 75

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		Part1	powers\PoisonControl\MustardGasHitA.part
		Part2	powers\PoisonControl\MustardGasSpecksHitA.part
		Part3	powers\PoisonControl\MustardGasBubblesA.part
		LifeSpan	30
	End

	Event
		Type	Local
		At	UArmL
		Part2	powers\PoisonControl\MustardGasSpecksHitA.part
		Part3	powers\PoisonControl\MustardGasBubblesA.part
		LifeSpan	25
	End

	Event
		Type	Local
		At	UArmR
		Part2	powers\PoisonControl\MustardGasSpecksHitA.part
		Part3	powers\PoisonControl\MustardGasBubblesA.part
		LifeSpan	25
	End

	Event
		Type	Local
		At	Head
		Part1	powers\PoisonControl\MustardGasHeadA.part
		Part2	powers\PoisonControl\MustardGasSpecksHeadA.part
		Part3	powers\PoisonControl\MustardGasBubblesHeadA.part
		LifeSpan	25
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
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\PoisonSplash.fx
		Lifespan	75
	End

	Event
		HardwareOnly
		EName	GasSpurts
		Type	Local
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\PoisonSplash.fx
		Lifespan	75
	End
End

#############################################################

End