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
		Geom	GEO_WepL_tricorder1
	End

End

Condition
	On Time 
	Time 173

	Event
		EName	Prime
		Type 	Destroy
			
	End

End


End
##################################

