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
	On Time 
	Time 20
	
	Event

		EName	Gas
		Type	Local
		At	chest
				
		Part1	:MustardGasHitA.part
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
		Sound poisonhit2 50 50 .7
		
	End

	Event

		EName	Gas1
		Type	Local
		At	UArmL
				
		#Part1	:MustardGasHitA.part
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
		
	End

	Event

		EName	Gas2
		Type	Local
		At	UArmR
				
		#Part1	:MustardGasHitA.part
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
		
	End


	Event

		EName	GasSpurts
		Type	Local
		At	chest
				
		Part1	:PoisonSpark2.part
		LifeSpan	51
	End

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
	Time 	1


	Event

		EName	GasSpurts2
		Type	Local
		At	hips
				
		Part1	:PoisonSpark2.part
		LifeSpan	58
	End

End
###################################################################

Condition
	On 	Time
	Time 	35

	Event	
		ENAME	CrossHairNode
		Type	Local
		At	Chest
		Part1	Powers/MartialArts/MAHitStar01.part
		Part2	Powers/MartialArts/MAHitStar02.part
		Part3	Powers/MartialArts/MAHitStar03.part
		Part4	Powers/MartialArts/MAHitGlow01.part
		Part5	Powers/MartialArts/MAHitRays01.part
		Lifespan 200
		
	End

	Event

		EName	Gas
		Type	Local
		At	chest
		Part	:RipperRays.part
		Part	:RipperStar.part
		Part	:RipperRing.part
		Part1	:MustardGasHitA.part
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
		Sound Quillhit5 70 70 .7
		Bhvr	Behaviors/GenericParticleFade.bhvr
		LifeSpan	6
	End

	Event

		EName	Gas1
		Type	Local
		At	UArmL
				
		#Part1	:MustardGasHitA.part
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
		
	End

	Event

		EName	Gas2
		Type	Local
		At	UArmR
				
		#Part1	:MustardGasHitA.part
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
		
	End


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
	
#	Event
#
#		EName	GasSpurts
#		Type	Local
#		At	chest
#				
#		Part1	:PoisonSpark.part
#		Lifespan	1
#	End

End

Condition
	On 	Time
	Time 	35
	Repeat	11
	
	Event

		EName	GasSpurts
		Type	Local
		At	chest
				
		ChildFX	V_COV\PhysicsEnabled\PoisonDarkSplash.fx
		Lifespan	75
	End
		
	Event
		HardwareOnly
		EName	GasSpurts
		Type	Local
		At	chest
				
		ChildFX	V_COV\PhysicsEnabled\PoisonDarkSplash.fx
		Lifespan	75
	End

End

Condition
	On 	Time
	Time 	30

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
	Time 	32

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