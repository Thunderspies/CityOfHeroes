#############################################################
## Child_LightningHit.fx
#############################################################

FxInfo

LifeSpan 30

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type 	Start
		At	Origin
		Bhvr	V_COV\PhysicsEnabled\LightningOut.bhvr
		Part	:LightningStreak.part
		Pmagnet	Origin
		CEvent 	:Child_ElectricSparkHit.fx
		CThresh 	0.000001
		CDestroy 	1
		Lifespan	10
		LifespanJitter	2
	End
End

#############################################################

End