#########################################################
## template
#########################################################

FxInfo

LifeSpan	60

#########################################################

Condition
	On Time 
	Time 0
	Repeat 5
	
	Event
		Type 	Start
		At	Origin
					
		Part	:Green_ElectricSparkWhite.part
		Part	:Green_ElectricSpark.part

		Lifespan	10
		LifespanJitter	2
	End
End

Condition
	On Time 
	Time 0
	Repeat 5
	
	Event
		Type 	Start
		At	Origin
		Bhvr	:SparkOut.bhvr
		
		Part	:Green_ElectricSparkWhite.part
		Part	:Green_ElectricSpark.part
		Lifespan	35
		LifespanJitter	5
	End		
End	

Condition
	On Time 
	Time 0
	Repeat 2
		
	Event
		HardwareOnly
		Type 	Start
		At	Origin
		Bhvr	:SparkExplode.bhvr
		Part	:Green_ElectricSparkWhite.part
		Part	:Green_ElectricSpark.part
		Lifespan	45
		LifespanJitter	5
	End	
	
	Event
		HardwareOnly
		Type 	Start
		At	Origin
		Bhvr	:SparkUp.bhvr
		Part	:Green_ElectricSparkWhite.part
		Part	:Green_ElectricSpark.part
		Lifespan	45
		LifespanJitter	5
	End
End
	
#########################################################

End	