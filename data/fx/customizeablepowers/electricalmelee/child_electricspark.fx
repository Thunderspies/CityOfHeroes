#############################################################
## Child_ElectricSpark.fx
#############################################################

FxInfo

LifeSpan 60
#############################################################

Condition
	On	Time
	Time	0

	Event
		Type 	Start
		At	Origin
		Bhvr	V_COV\PhysicsEnabled\SparkOutbig.bhvr
		Part	:ElectricSpark.part
		Part	:ElectricSparkStreak.part
		Lifespan	45
		LifespanJitter	5
	End
End

#############################################################

End

