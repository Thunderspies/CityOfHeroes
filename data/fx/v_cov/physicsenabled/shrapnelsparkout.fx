#########################################################
##	template

FxInfo

#########################################################

Condition
	On Time 
	Time 0
	
	Event
		Type 	Start
		At	Origin
		Bhvr	:SparkOUT.bhvr
		BhvrOverride
			physRestitution 	0.5
			physSFriction 	 	0.75
			physDFriction 	 	0.75
		End

		CThresh 	0.000001

		Part	:ShrapnelSpark.part
		Part	:HotSpark.part
		Part	:HotSparkStreak.part

		Lifespan	60
		LifespanJitter	10
	End
End	

End	

	