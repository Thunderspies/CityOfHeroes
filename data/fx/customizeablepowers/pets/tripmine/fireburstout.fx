#############################################################
## FireBurstOUT.fx
#############################################################

FxInfo

LifeSpan 90

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	origin
		Bhvr	V_COV\PhysicsEnabled\FireBurstOUT.bhvr
		Part1	:FireSplash.part
		Part3	:FireSplashEmbers.part
		CEvent 	:FireNapalmHit.fx
		CThresh 	0.001
		CDestroy 	0
		Lifespan	60
		LifespanJitter	30
	End
End

#############################################################

End