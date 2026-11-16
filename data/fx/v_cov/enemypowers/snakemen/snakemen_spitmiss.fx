#########################################################
##	
##

FxInfo

LifeSpan	100

##################################
Condition
	On 	Time
	Time    0


	Event

		EName	Gas
		Type	Start
		At	Origin
		
		bhvr	behaviors\FX_SnakeMen_HitOffset.bhvr
		Part1	:SM_SpitGasHit.part
		Part2	:SM_SpitGasSpecksHit.part
#		Part3	:SM_SpitGasBubbles.part
#		Sound	poisonhit2 50 30 .7
		
	End

	Event

		EName	Gas1
		Type	Local
		At	UArmL

		#Part1	:SM_SpitGasHit.part
		#Part2	:SM_SpitGasSpecksHit.part
		#Part3	:SM_SpitGasBubbles.part
		
	End

	Event

		EName	Gas2
		Type	Local
		At	UArmR
		
		#Part1	:SM_SpitGasHit.part
		#Part2	:SM_SpitGasSpecksHit.part
		#Part3	:SM_SpitGasBubbles.part
		
	End


	Event

		EName	GasSpurts
		Type	Local
		At	Origin
				
		Part1	:SM_SpitPoisonSpark.part
		Lifespan	1
	End

	Event

		EName	HeadConfusionClouds
		Type	Local
		At	Head
				
		#Part1	:SM_SpitGasHead.part
		#Part2	:SM_SpitGasSpecksHead.part
		#Part3	:SM_SpitGasBubblesHead.part
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