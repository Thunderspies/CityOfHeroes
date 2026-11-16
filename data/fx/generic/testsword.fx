#########################################################
##	jack
##

FxInfo

Input  
	InpName	WepR
End

Condition
	On 	Time
	Time 	20

	Event
		EName 	Prime
		Type	Local 
		At	WepR
		Geom	GEO_WepR_Sword_1
		Sound swordshing 100 100 .7
	End
End

End		