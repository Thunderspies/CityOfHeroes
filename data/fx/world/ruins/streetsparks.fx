#########################################################
##	template

FxInfo


Input
	Inpname origin
End

#########################################################

Condition
	On Time 
	Time 0
	
	Event
		EName	Pow
		Type 	Local
		At	origin
		Part1	:StreetSparksBroad.part
		Part2	:SparkGlow.part
		Part3	:StreetSparks.part
		Part4	:SparkFlash.part
		Part5	:SparkRing.part
		Sound spark_loop 29 29 .35
	End

	Event
		EName	Pow
		Type 	Local
		At	origin
		Part1	world/City/ElectricSpark1.part
	End

End


End	

	