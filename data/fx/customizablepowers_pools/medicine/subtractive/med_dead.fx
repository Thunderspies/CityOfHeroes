#########################################################
##	tenticle fun yea!
########################################################
FxInfo

Input
	Inpname	WepL
End


##################################################################

Condition
	On	Time
	Time	29




	Event
		EName 	Prime
		Type	Local
		At	WepL
		Geom	GEO_WepL_tricorder2
	End



End


Condition
	On Time
	Time 234

	Event
		EName	Prime
		Type 	Destroy

	End

End

End
##################################

