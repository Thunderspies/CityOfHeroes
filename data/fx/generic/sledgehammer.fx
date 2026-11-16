#########################################################
##	iceweapon
########################################################
FxInfo

Input  
	Inpname	WepR
End

Input  
	Inpname	Mystic
End


####################################


Condition
	On	Time
	Time	0

	Event
		EName 	Prime
		Type	Local 
		At	WepR
		Geom	FX_StoneMallet	


	End

End


End
