#########################################################
##	PhysicsRing
##
FxInfo

LifeSpan 120

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	origin

		Bhvr	V_COV\PhysicsEnabled\FireBurstScatter.bhvr
		Part1	:FireSplash.part
		Part2	:FireSplash.part
		Part3	:FireSplashEmbers.part
		Lifespan	90
		LifespanJitter	30

		CEvent 	:FireNapalmHit.fx
		CThresh 	.001
		CDestroy 	0
	End

End

End
