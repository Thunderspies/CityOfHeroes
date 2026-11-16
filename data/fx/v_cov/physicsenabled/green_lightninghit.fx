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
		CEvent 	V_COV\PhysicsEnabled\Green_ElectricSparkHit.fx
		CThresh 	0.000001
		CDestroy 	1
		Part	:Green_LightningStreak.part
		Part	:Green_LightningStreakWhite.part
		

		Pmagnet	Origin

		Lifespan	10
		LifespanJitter	2
	End
End	

End	

	