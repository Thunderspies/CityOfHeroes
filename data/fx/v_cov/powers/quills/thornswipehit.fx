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

LifeSpan	50
##################################
##################################
##################################

Condition
	On 	Time
	Time 	0



	Event
		
		Type	Local
		At	chest
		Sound thornyswipe 70 70 .77
	End
End

##################################
##################################
##################################

Condition
	On 	Time
	Time 	0

	Event	
		ENAME	CrossHairNode
		Type	Local
		At	Chest
		Part1	Powers/MartialArts/MAHitStar01.part
		Part2	Powers/MartialArts/MAHitStar02.part
		Part3	Powers/MartialArts/MAHitStar03.part
		Part4	Powers/MartialArts/MAHitGlow01.part
		Part5	Powers/MartialArts/MAHitRays01.part
		##Sound	poisonhit2 50 30 .7
		Lifespan 200
		
	End

	Event

		EName	Gas
		Type	Local
		At	chest
		#Part	:QuillsScatter.part		
		Part1	:MustardGasHitA.part
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
		LifeSpan	30
		
	End

	Event

		EName	Gas1
		Type	Local
		At	UArmL
				
		#Part1	:MustardGasHitA.part
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
		LifeSpan	25
	End

	Event

		EName	Gas2
		Type	Local
		At	UArmR
				
		#Part1	:MustardGasHitA.part
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
		LifeSpan	25
	End

#	Event
#
#		Type	Local
#		At	chest
#				
#		Part1	:PoisonSpark.part
#		Lifespan	1
#	End
#
#	Event
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
				
		Part1	:MustardGasHeadA.part
		Part2	:MustardGasSpecksHeadA.part
		Part3	:MustardGasBubblesHeadA.part
		LifeSpan	25
	End
	
End


Condition
	On 	Time
	Time 	0
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


End				