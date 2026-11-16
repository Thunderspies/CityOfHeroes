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
		Bhvr	:LightSparkExplodeUP.bhvr
		CThresh 	0.000001

		Part	:MetalSpark.part
		Part	:MetalSparkStreak.part

		Lifespan	50
		LifespanJitter	10
	End
End	

End	

	