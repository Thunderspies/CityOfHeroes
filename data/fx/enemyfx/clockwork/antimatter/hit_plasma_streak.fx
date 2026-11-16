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
		Bhvr	:Plasma_Streaks_Out.bhvr
		CEvent 	V_COV\PhysicsEnabled\ElectricSparkHit.fx
		CThresh 	0.000001
		CDestroy 	1
		Part	:Plasma_Streak.part

		Pmagnet	Origin

		Lifespan	20
		LifespanJitter	2
	End
End

End

