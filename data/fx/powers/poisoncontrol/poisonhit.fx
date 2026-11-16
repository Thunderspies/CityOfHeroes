#############################################################
## PoisonHit.fx
#############################################################

FxInfo

Lifespan 75

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Gas
		Type	Local
		At	chest
		Part	POWERS\PoisonControl\MustardGasHitA.part
		Part	POWERS\PoisonControl\MustardGasSpecksHitA.part
		Part	POWERS\PoisonControl\MustardGasBubblesA.part
		Sound poisonhit 50 50 0.7
		Sound poisonhit2 50 50 0.7
		Sound poisonhit3 50 50 0.7
		Lifespan 10
	End

	Event
		EName	Gas1
		Type	Local
		At	UArmL
		Part	POWERS\PoisonControl\MustardGasSpecksHitA.part
		Part	POWERS\PoisonControl\MustardGasBubblesA.part
		Lifespan 10
	End

	Event
		EName	Gas2
		Type	Local
		At	UArmR
		Part	POWERS\PoisonControl\MustardGasSpecksHitA.part
		Part	POWERS\PoisonControl\MustardGasBubblesA.part
		Lifespan 10
	End

	Event
		EName	HeadConfusionClouds
		Type	Local
		At	Head
		Part	POWERS\PoisonControl\MustardGasSpecksHeadA.part
		Part	POWERS\PoisonControl\MustardGasBubblesHeadA.part
		Lifespan 12
	End
End

Condition
	On 	Time
	Time 	0
	Repeat	6

	Event
		EName	GasSpurts
		Type	Local
		At	chest
		ChildFX	V_COV\PhysicsEnabled\PoisonSplash.fx
		Lifespan 75
	End

	Event
		HardwareOnly
		EName	GasSpurts
		Type	Local
		At	chest
		ChildFX	V_COV\PhysicsEnabled\PoisonSplash.fx
		Lifespan 75
	End
End

#############################################################

End