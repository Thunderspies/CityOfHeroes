#########################################################
##	
##

FxInfo

##################################

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	Origin
		Sound rampage_loop 90 90 .45
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 440

	End
End


Condition
	On	Time
	Time	0

	Event
		Type	local
		At	Origin
		Sound radiation4_loop 90 90 .28
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 440

	End
End


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

		EName	HeadConfusionClouds
		Type	Local
		At	Head
		Bhvr	behaviors/genericParticleFade.bhvr		
		#Part1	:MustardGasHitA.part
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part
	End
	
End



End				