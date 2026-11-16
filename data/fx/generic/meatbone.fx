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
		EName 	Yummy
		Type	local
		At	WepR
		Geom	Dinner
##		Geom	BoneR
	End




End



End
##################################

