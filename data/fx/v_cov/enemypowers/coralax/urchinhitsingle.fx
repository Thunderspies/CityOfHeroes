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
		Sound SpinyUrchin4 70 70 .7
		Sound SpinyUrchin5 70 70 .7
	
	End
End

##################################
##################################
##################################

Condition
	On 	Time
	Time 	0

	Event

		
		Type	local
		At	chest
		Bhvr	:QuillHit01.bhvr
		Geom	Corolax_Urchin
#		Lookat	Target
		
	End

	Event

		EName	Gas
		Type	Local
		At	chest
		Part	:QuillsScatter.part		
		Part1	:MustardGasHitA.part
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
		LifeSpan	30
		
	End

	Event

		EName	Gas1
		Type	Local
		At	UArmL
				
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
		LifeSpan	25
	End

	Event

		EName	Gas2
		Type	Local
		At	UArmR
				
		Part2	:MustardGasSpecksHitA.part
		Part2	:MustardGasSpecksHitB.part
		Part3	:MustardGasBubblesA.part
		LifeSpan	25
	End

	Event

		Type	Local
		At	chest
				
		Part1	:PoisonSpark.part
		Lifespan	1
	End

	Event
		Type	Local
		At	chest
				
		Part1	:PoisonSpark2.part
		Lifespan	1
	End

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



End				