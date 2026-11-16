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
		
		Part	:WhiteElectricSpark.part
		Part	:RedElectricSpark.part
		Part	:RedElectricSparkStreak.part

		Lifespan	45
		LifespanJitter	5
	End
End	

End	

	