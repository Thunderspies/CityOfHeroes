#########################################################
##	template

FxInfo

#########################################################

Condition
	On Time 
	Time 0
	
	Event
		Type 	local
		At	Origin
		Bhvr	:SparkOUT.bhvr

		CThresh 	0.000001

		Part	:HotSpark.part
		Part	:HotSparkStreak.part

		Lifespan	50
		LifespanJitter	10
	End
End	

End	

	