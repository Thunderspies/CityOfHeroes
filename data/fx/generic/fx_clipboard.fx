#########################################################
##	
########################################################
FxInfo

Input  
	Inpname	WepR
End

Input  
	Inpname	WepL
End


##################################################################

Condition
	On	Time
	Time	0

	Event
		EName 	Prop_R
		Type	Local
		At	WepR 
		Geom	Prop_Pen
		Geom	Prop_Pencil

	End


	Event
		EName 	Prop_L
		Type	Local
		At	WepL 
		Geom	Prop_Clipboard

	End

End



End
##################################

