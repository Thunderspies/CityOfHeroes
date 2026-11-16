#############################################################
## FireBurstUP.fx
#############################################################

FxInfo

LifeSpan 115

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	origin
		Bhvr	V_COV\PhysicsEnabled\FireBurstUP.bhvr
		Part1	:FireSplash.part
		Part5	:FireSplash.part
		Part3	:FireSplashEmbers.part
		#CEvent 	:FireNapalmHit.fx
		CThresh 	.001
		CDestroy 	0
		Lifespan	75
		LifespanJitter	30
	End
End

#############################################################

End