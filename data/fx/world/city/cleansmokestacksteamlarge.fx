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
		Part1	:CleanSmokeStackSteamLarge.part
		Sound Steam3_Loop 40.0 40.0 .16
	End
End

End			