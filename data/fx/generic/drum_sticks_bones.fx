#########################################################
##	
########################################################
FxInfo

Input  
	Inpname	Root
End

Input  
	Inpname	Mystic
End

Input  
	Inpname	WepL
End

Input  
	Inpname	WepR
End


##################################################################

Condition
	On	Time
	Time	0


	Event
		EName 	drum_stick_R
		Type	local
		At	WepR 
		Geom	BoneR
	End

	Event
		EName 	drum_stick_L
		Type	local
		At	WepL 
		Geom	BoneL
	End


End



End
##################################

