#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	150

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Start 
		At	Origin
		bhvr	:FireBurstComet.bhvr
		Part	:FireSplashBig.part
		Part1	\POWERS\FireControl\BurningMeteor.part
		Part2	\POWERS\FireControl\FireBoltTail2.part
		Part3	\POWERS\FireControl\FireBoltTail.part
		Part4	\POWERS\FireControl\SparkTrail.part
		#Part5	\POWERS\FireControl\FireHands2.part

		CEvent 	:FireCometBurst.fx
		CThresh 	.001
		CDestroy 	1
	End


End


End	


			
