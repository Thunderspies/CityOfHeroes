#########################################################
##	SStreetSteeam01
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
		Part1	world/City/ElectricSpark1.part
		#Part2	world/City/ElectricSpark2.part
		#Part3	world/City/ElectricSpark3.part
		Sound elecarc_loop 34 34 .2
	End
End

End			