#########################################################
##	med_fire
##
FxInfo
	
Name 	 med_fire
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
		Part1	WORLD/fire/fire_flame1.part
		Part2	WORLD/fire/spark1.part
		Part3	WORLD/fire/fire_flame2.part
	End
End

End			