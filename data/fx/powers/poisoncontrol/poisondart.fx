#########################################################
##	
##

FxInfo

LifeSpan	200

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
		
		Part1	:PoisonSpark.part
		Part2	:PoisonSpark2.part
		
	End

	Event

		EName	Gas
		Type	Local
		At	chest
				
		Part1	:MustardGasHit.part
		Part2	:MustardGasSpecksHit.part
		Part3	:MustardGasBubbles.part
		
	End

	Event

		EName	Gas1
		Type	Local
		At	UArmL
				
		Part1	:MustardGasHit.part
		Part2	:MustardGasSpecksHit.part
		Part3	:MustardGasBubbles.part
		
	End

	Event

		EName	Gas2
		Type	Local
		At	UArmR
				
		Part1	:MustardGasHit.part
		Part2	:MustardGasSpecksHit.part
		Part3	:MustardGasBubbles.part
		
	End

	Event

		EName	HeadConfusionClouds
		Type	Local
		At	Head
				
		Part1	:MustardGasHead.part
		Part2	:MustardGasSpecksHead.part
		Part3	:MustardGasBubblesHead.part
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