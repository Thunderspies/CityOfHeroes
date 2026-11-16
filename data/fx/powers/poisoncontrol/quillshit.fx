#########################################################
##	
##

FxInfo

LifeSpan	100

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
Condition
	On 	Time
	Time 	0


	Event

		EName	Gas
		Type	Local
		At	chest
				
		Part1	:MustardGasHitA.part
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
		Sound Quills3 70 70 .98
		
	End

	Event

		EName	Gas1
		Type	Local
		At	UArmL
				
		#Part1	:MustardGasHitA.part
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
		Sound PoisonHit2 70 70 .66
		
	End

	Event

		EName	Gas2
		Type	Local
		At	UArmR
				
		#Part1	:MustardGasHitA.part
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
		
	End

#
#	Event
#
#		EName	GasSpurts
#		Type	Local
#		At	chest
#				
#		Part1	:PoisonSpark2.part
#		Lifespan	1
#	End

	Event

		EName	HeadConfusionClouds
		Type	Local
		At	Head
				
		#Part1	:MustardGasHeadA.part
		Part2	:MustardGasSpecksHeadA.part
		Part3	:MustardGasBubblesHeadA.part
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
	On 	Time
	Time 	10

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
	Time 	12

	Event

		EName	HeadConfusion
		Type	destroy
			
	End

	Event

		EName	HeadConfusionClouds
		Type	destroy
			
	End

End




End				