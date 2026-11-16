#########################################################
##	jack
##
FxInfo
	
LifeSpan 0

Input  
	InpName	WepR
End

Condition
	On 	Time
	Time 	0

	Event
		EName Prime
		Type	Local 
		At	WepR
		Geom	GEO_WepR_Bow_1
	End
End

End			