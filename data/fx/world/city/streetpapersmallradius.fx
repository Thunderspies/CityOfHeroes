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
		Part1	world/City/StreetpaperSmallRadius1.part
		Part2	world/City/StreetpaperSmallRadius2.part
		
	End

End
End			