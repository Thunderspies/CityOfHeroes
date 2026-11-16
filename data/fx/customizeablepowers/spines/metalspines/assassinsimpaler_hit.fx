#############################################################
## AssassinsImpaler_Hit.fx
#############################################################

FxInfo

Lifespan 100

#############################################################

Condition
	On 	Time
	Time 	0


	Event
		Type	Local
		At	chest
		Part	CustomizeablePowers\Spines\Spines\MustardGasHitA.part
		Part	CustomizeablePowers\Spines\Spines\MustardGasSpecksHitA.part
		Part	CustomizeablePowers\Spines\Spines\MustardGasBubblesA.part
		Sound QuillsRipper 60 60 0.73
		Lifespan 10
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