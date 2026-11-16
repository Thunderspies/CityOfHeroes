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
		Part1	:LavaSteamXtraSmall1.part
		Part2	:LavaSpray.part
		Sound Steam3_loop 14 14 .10
	End
End

End			