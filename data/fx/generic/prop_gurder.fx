#########################################################
##	
########################################################
FxInfo

Input  
	Inpname	WepL
End


##################################################################

Condition
	On	Time
	Time	0


	Event
		EName 	Prime
		Type	Local 
		At	WepL 
		Geom	Prop_Planks 
		Geom	GEO_WepL_prop_scrap
		Geom	GEO_WepL_prop_scrap

	End

End

End
##################################

