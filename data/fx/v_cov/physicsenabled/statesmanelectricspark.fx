#########################################################
##	template

FxInfo

LifeSpan	60
#########################################################

Condition
	On Time 
	Time 0
	
	Event
		Type 	Start
		At	Origin
		Bhvr	:SparkOutbig.bhvr
		
		Part	:StatesmanElectricSpark.part
		Part	:StatesmanElectricSparkStreak.part

		Lifespan	45
		LifespanJitter	5
	End
End	

End	

	