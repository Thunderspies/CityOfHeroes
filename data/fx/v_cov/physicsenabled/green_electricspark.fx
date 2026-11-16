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
		Part	:Green_ElectricSpark.part
		Part	:Green_ElectricSparkStreak.part

		Lifespan	45
		LifespanJitter	5
	End
End	

End	

	