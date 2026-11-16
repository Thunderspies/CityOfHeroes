#########################################################
##	template

FxInfo

LifeSpan	260
#########################################################

Condition
	On Time 
	Time 0
	
	Event
		Type 	Start
		At	Origin
		Bhvr	:GenericSpark.bhvr
		
		Part	:GenericElectricSpark.part

		Cdestroy	1
		Lifespan	145
#		LifespanJitter	5
	End
End	

End	

	