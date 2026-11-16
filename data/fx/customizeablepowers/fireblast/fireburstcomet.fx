#############################################################
## FireBurstComet.fx
#############################################################

FxInfo

LifeSpan 150

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Start
		At	Origin
		bhvr	V_COV\PhysicsEnabled\FireBurstComet.bhvr
		Part	:FireSplashBig.part
		Part1	:BurningMeteor.part
		Part2	:FireBoltTail2.part
		Part3	:FireBoltTail.part
		Part4	:SparkTrail.part
		Part5	:FireHands2.part
		CEvent 	:FireCometBurst.fx
		CThresh 	0.001
		CDestroy 	1
	End
End

#############################################################

End