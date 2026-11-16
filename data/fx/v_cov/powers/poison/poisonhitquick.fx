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
		Part1	:MustardGasHitA.part
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
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

		ChildFX	V_COV\PhysicsEnabled\PoisonSplash.fx
		Lifespan	75
	End

	Event
		HardwareOnly
		EName	GasSpurts
		Type	Local
		At	chest

		ChildFX	V_COV\PhysicsEnabled\PoisonSplash.fx
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

		Part1	:PoisonSpark.part
		Part1	:PoisonSparkdark.part
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

		Part1	:PoisonSpark2dark.part
		Part1	:PoisonSpark2.part
		Lifespan	1
	End

End



End