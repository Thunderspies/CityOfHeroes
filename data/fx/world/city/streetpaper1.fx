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
		Part1	world/City/Streetpaper1.part
		Part2	world/City/Streetpaper2.part
		Sound WindStreetPaperL_Loop 200 50 .15
	End

End
End			