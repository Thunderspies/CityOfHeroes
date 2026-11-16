#########################################################
##	onfire
##
FxInfo  

LifeSpan 300

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	head

		Bhvr 	behaviors/physSparkBurst.bhvr
#		Part1	:FireBallTargetSteam.part
#		Part2	:FireBallTargetFlame2.part
#		Part3	:FireBallSpark.part	
		Lifespan	300
	End

End

End			
