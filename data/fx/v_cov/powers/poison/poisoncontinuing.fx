#########################################################
##	
##

FxInfo

Input  
	InpName	neck
End

Input  
	InpName	head
End

Input  
	InpName	chest
End

Input  
	InpName	hips
End

Input  
	InpName	UArmL
End

Input  
	InpName	UArmR
End

##################################
##################################
##################################

Condition
	On 	Time
	Time 	5



	Event
		
		Type	Local
		At	chest
		Sound Darthit1 70 70 .9
	End
End

##################################
##################################
##################################

Condition
	On 	cycle
	Time 	10
	Chance	.4

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
	On 	cycle
	Time 	10
	Chance	.4

	Event

		EName	Gas1
		Type	Local
		At	UArmL
				
		#Part1	:MustardGasHitA.part
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
		LifeSpan	25
	End

End

Condition
	On 	cycle
	Time 	10
	Chance	.4

	Event

		EName	Gas2
		Type	Local
		At	UArmR
				
		#Part1	:MustardGasHitA.part
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
		LifeSpan	25
	End

End

Condition
	On 	cycle
	Time 	10
	Chance	.4
	
	Event

		EName	HeadConfusionClouds
		Type	Local
		At	Head
				
		Part1	:MustardGasHeadA.part
		Part2	:MustardGasSpecksHeadA.part
		Part3	:MustardGasBubblesHeadA.part
		LifeSpan	25
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

Condition
	On 	Cycle
	Time 	3
	Chance	.3
	
	Event

		EName	GasSpurts
		Type	Local
		At	chest
				
		ChildFX	V_COV\PhysicsEnabled\PoisonDrip.fx
		Lifespan	75
	End

End


Condition
	On 	Cycle
	Time 	5
	Chance	.5
	
	Event
		HardwareOnly
		EName	GasSpurts
		Type	Local
		At	chest
				
		ChildFX	V_COV\PhysicsEnabled\PoisonDrip.fx
		Lifespan	75
	End

End


End				