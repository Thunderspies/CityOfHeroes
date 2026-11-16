#########################################################
##	
##

FxInfo

LifeSpan	30

##################################
Condition
	On 	Time
	Time 	0


	Event

		EName	Gas
		Type	Local
		At	chest
		Bhvr	behaviors/genericParticleFade.bhvr		
		Part1	:MustardGasHitA.part
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
		Part3	:PoisonYellow2.part
		Sound poisonhit2 50 50 .7
		
	End

	Event

		EName	Gas1
		Type	Local
		At	UArmL
		Bhvr	behaviors/genericParticleFade.bhvr		
		#Part1	:MustardGasHitA.part
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
		
	End

	Event

		EName	Gas2
		Type	Local
		At	UArmR
		Bhvr	behaviors/genericParticleFade.bhvr		
		#Part1	:MustardGasHitA.part
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
		
	End


	Event

		EName	GasSpurts
		Type	Local
		At	waist
		Bhvr	behaviors/genericParticleFade.bhvr		
		Part1	:PoisonSpark2.part
		Part1	:BioToxinHitFlash.part
		#Part2	:PoisonGlows.part
		Lifespan	10
	End

	Event

		EName	HeadConfusionClouds
		Type	Local
		At	Head
		Bhvr	behaviors/genericParticleFade.bhvr		
		#Part1	:MustardGasHitA.part
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
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