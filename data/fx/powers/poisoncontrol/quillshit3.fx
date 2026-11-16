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
		Type	Local
		At	chest
		Part1	:MustardGasHitA.part
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
		Sound Quills3 70 70 .98
		Lifespan 10
	End

	Event

		Type	Local
		At	UArmL
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
		Sound PoisonHit2 70 70 .66
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
End

#############################################################

Condition
	On 	Time
	Time 	0
	Repeat	6

	Event
		EName	GasSpurts
		Type	Create
		Inherit	Position Scale
		Update	Position
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\PoisonSplash.fx
		Lifespan 75
	End

	Event
		HardwareOnly
		EName	GasSpurts
		Inherit	Position Scale
		Update	Position
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\PoisonSplash.fx
		Lifespan 75
	End
End

#############################################################

End