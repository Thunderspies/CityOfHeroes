#############################################################
## GunSparkOUT.fx
#############################################################

FxInfo

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type 	Start
		At	Origin
		Bhvr	V_COV\PhysicsEnabled\SparkOUT.bhvr
		CThresh 	0.000001
		Part	V_COV\PhysicsEnabled\HotSparkStreak.part

		Lifespan	50
		LifespanJitter	10
	End
End

#############################################################

End