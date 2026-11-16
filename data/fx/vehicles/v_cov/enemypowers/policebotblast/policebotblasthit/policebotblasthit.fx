#########################################################
##	
##

FxInfo

LifeSpan	20

Input  
	InpName	head
End

Input  
	InpName	chest
End

Input  
	InpName	UArmL
End

Input  
	InpName	UArmR
End

##################################
Condition
	On 	Time
	Time 	0

	Event

		EName	GasSpurts
		Type	Local
		At	chest
		
		Part	:PoisonSpark.part
		Part	:PoisonSpark2.part
		Part	:PoisonSparkDark.part
		Part	:PoisonSpark2Dark.part
		
	End

	Event

		EName	Gas
		Type	Local
		At	chest
			
		Part	:MustardGasHitRed.part	
		Part1	:MustardGasHit.part
		Part2	:MustardGasSpecksHit.part
		Part3	:MustardGasBubbles.part
		Part	:MustardGasBackstreaks.part
		Part	:MustardGasBackstreaksInvert.part
	End

	Event

		EName	Gas1
		Type	Local
		At	UArmL
		
		Part	:MustardGasHitRed.part
		Part	:MustardGasHit.part
		Part	:MustardGasSpecksHit.part
		Part	:MustardGasBubbles.part
		Part	:MustardGasBackstreaks.part
		Part	:MustardGasBackstreaksInvert.part

		
	End

	Event

		EName	Gas2
		Type	Local
		At	UArmR
			
		Part	:MustardGasHitRed.part	
		Part1	:MustardGasHit.part
		Part2	:MustardGasSpecksHit.part
		Part3	:MustardGasBubbles.part
		Part	:MustardGasBackstreaks.part
		Part	:MustardGasBackstreaksInvert.part
		
	End

	Event

		EName	HeadConfusionClouds
		Type	Local
		At	Head
		
		Part	:MustardGasHeadRed.part
		Part	:MustardGasHitRed.part
		Part	:MustardGasstreaks.part	
		Part	:MustardGasHead.part
		Part	:MustardGasSpecksHead.part
		Part	:MustardGasBubblesHead.part
	End
	
End


Condition
	On 	Time
	Time 	7

	Event

		EName	Gas
		Type	destroy
			
	End

	Event

		EName	Gas1
		Type	destroy
			
	End

	Event

		EName	Gas2
		Type	destroy
			
	End

End

Condition
	On 	Time
	Time 	15

	Event

		EName	HeadConfusionClouds
		Type	destroy
			
	End

End

Condition
	On 	Time
	Time 	5

	Event

		EName	GasSpurts
		Type	destroy
			
	End

	
End


End				