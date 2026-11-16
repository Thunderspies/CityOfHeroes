#########################################################
##	jack
##

FxInfo

Input  
	InpName	WepR
End

LifeSpan 10

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	WepR
###		Geom	GEO_WepR_Vandal_hammer
		Sound hammer1 60 60 .4
		Sound hammer2 60 60 .4
		Sound hammer3 60 60 .4
	End
End

End		