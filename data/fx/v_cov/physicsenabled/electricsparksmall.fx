#########################################################
##	template

FxInfo

LifeSpan	200
#########################################################

Condition
	On Time 
	Time 0
	
	Event
		Type 	Start
		At	Origin
		Bhvr	:SparkOutbig.bhvr
		
		BhvrOverride
			
			InitialVelocity		0.0 0.25 0.0	
			InitialVelocityJitter	.35 0.3 .35

		End

		Part	:ElectricSpark.part
		Part	:ElectricSparkStreak.part

		Lifespan	120
		LifeSpanJitter	80
	End
End	

Condition
	On Time 
	Time 2
	
	Event
		Type 	Start
		At	Origin
		Bhvr	:SparkOutbig.bhvr
		
		BhvrOverride
			
			InitialVelocity		0.0 0.25 0.0	
			InitialVelocityJitter	.35 0.3 .35

		End

		Part	:ElectricSpark.part
		Part	:ElectricSparkStreak.part

		Lifespan	120
		LifeSpanJitter	80
	End
End	

End	

	