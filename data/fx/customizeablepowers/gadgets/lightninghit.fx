#########################################################
##	template

FxInfo

LifeSpan	30
#########################################################

Condition
	On Time
	Time 0

	Event
		Type 	Start
		At	Origin
		Bhvr	V_COV\PhysicsEnabled\LightningOut.bhvr
		CEvent 	:ElectricSparkHit.fx
		CThresh 	0.000001
		CDestroy 	1
		Part	:LightningStreak.part

		Pmagnet	Origin

		Lifespan	10
		LifespanJitter	2
	End
End

End

