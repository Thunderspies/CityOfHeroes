#########################################################
##	tech_spark_twirl.fx
##
FxInfo
	
LifeSpan 0

Input  
	InpName	Origin
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	Origin
		Part1	world/Lairs/tech/tech_spark_twirl_1.part
		##Part2	world/Lairs/tech/tech_sparkbolt_2.part
		Sound energycore4_loop 100 100 .33
	End
End


End			