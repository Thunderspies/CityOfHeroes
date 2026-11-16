#########################################################
##	jack
##

FxInfo

Input  
	InpName	WepR
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	WepR
		Geom	GEO_WepR_Grenade_Launcher_1
	End
End

End		