#############################################################
## PoisonHitQuick.fx
#############################################################

FxInfo
LifeSpan	30

#########################################################

Input
	Inpname	Hips
End

#########################################################

##################################
##################################
##################################

Condition
	On 	Time
	Time 	5



	Event

		Type	Local
		At	chest
		Sound poisonhit5 70 70 .76
	End
End

##################################
##################################
##################################

Condition

	Event

		EName	Gas
		Type	Local
		At	chest
		Part1	CustomizeablePowers\Poison\MustardGasHitA.part
		Part2	CustomizeablePowers\Poison\MustardGasSpecksHitA.part
		Part3	CustomizeablePowers\Poison\MustardGasBubblesA.part
		LifeSpan	30

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

		ChildFX	CustomizeablePowers\Poison\PoisonSplash.fx
		Lifespan	75
	End

	Event
		HardwareOnly
		EName	GasSpurts
		Type	Local
		At	chest

		ChildFX	CustomizeablePowers\Poison\PoisonSplash.fx
		Lifespan	75
	End
End

Condition
	On 	cycle
	Time 	3
	Chance	.3

	Event

		Type	Local
		At	chest

		Part1	CustomizeablePowers\Poison\PoisonSpark.part
		Part1	CustomizeablePowers\Poison\PoisonSparkdark.part
		Lifespan	1
	End

End

Condition
	On 	cycle
	Time 	3
	Chance	.3

	Event
		Type	Local
		At	chest

		Part1	CustomizeablePowers\Poison\PoisonSpark2dark.part
		Part1	CustomizeablePowers\Poison\PoisonSpark2.part
		Lifespan	1
	End

End



End