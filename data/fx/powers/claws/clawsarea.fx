#########################################################
##	template

FxInfo


Input
	Inpname WepR
End

Input
	Inpname WepL
End

Condition
	On Time 
	Time 0
	
	Event
		EName	Right
		Type 	Local
		At	WepR
		Geom	GEO_WepR_ClawR
		BHVR	Behaviors/ClawsFade1.bhvr
	End

	Event
		Ename	Left
		Type	Local
		At	WepL
		Geom	GEO_WepL_ClawL
		BHVR	Behaviors/ClawsFade1.bhvr
	End

End
	


End	