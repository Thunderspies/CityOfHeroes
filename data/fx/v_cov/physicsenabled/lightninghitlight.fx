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
		CEvent 	V_COV\PhysicsEnabled\ElectricSparkHitLite.fx
		CThresh 	0.000001
		CDestroy 	1
		Part	:LightningStreakLight.part

		Pmagnet	Origin

		Lifespan	10
		LifespanJitter	2
	End
End	

End	

	