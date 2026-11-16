#########################################################
##	jack
##

FxInfo

Input  
	InpName	WepR
End

Input  
	InpName	Mystic
End


Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	WepR
		Geom	GEO_WepR_spraycan
	End
End

End		