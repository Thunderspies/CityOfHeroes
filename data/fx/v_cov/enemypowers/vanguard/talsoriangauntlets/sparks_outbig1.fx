#########################################################
## Metal Sparks
#########################################################

FxInfo

#########################################################

Condition
	On Time 
	Time 0
	
	Event
		Type 	Start
		At	Origin
		Bhvr	V_COV\PhysicsEnabled\SparkOutBig.bhvr
		CThresh 	0.000001

		Part	:Sparks_MetalSpark1.part
		Part	:Sparks_MetalSparkStreak1.part

		Lifespan	50
		LifespanJitter	10
	End
End	

#########################################################

End