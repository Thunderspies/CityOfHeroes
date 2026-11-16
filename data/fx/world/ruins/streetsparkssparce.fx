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
		Part1	:StreetSparksSparceBroad.part
		Part2	:SparkGlowSparce.part
		Part3	:StreetSparksSparce.part
		Part4	:SparkFlashSparce.part
		Part5	:SparkRing.part
		Sound spark_loop 29 29 .35
	End

	Event
		EName	Pow
		Type 	Local
		At	origin
		Part1	:ElectricSparkSparce1.part
	End

End


End	

	