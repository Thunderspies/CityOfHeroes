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
		Bhvr	:LightningOut.bhvr
		CEvent 	V_COV\PhysicsEnabled\Reichsman_ElectricSparkHit.fx
		CThresh 	0.000001
		CDestroy 	1
		Part	:Reichsman_LightningStreak.part

		Pmagnet	Origin

		Lifespan	10
		LifespanJitter	2
	End
End

End

