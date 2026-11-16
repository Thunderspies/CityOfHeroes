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
		EName 	Prop_L
		Type	Local
		At	WepL 
		Geom	Prop_Cellphone
	End

End



End
##################################

