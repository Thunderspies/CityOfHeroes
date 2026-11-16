#########################################################
##	template

FxInfo


Input
	Inpname WepR
End

Input
	Inpname WepL
End

Flags    InheritAlpha

Condition
	On Time 
	Time 0
	
	Event
		EName	Right
		Type 	Local
		At	WepR
		Geom	GEO_WepR_ClawR
		#BHVR	Behaviors/ClawsFade.bhvr
		#Sound	Claw1d 100.0 30.0 .7
	End

	Event
		Ename	Left
		Type	Local
		At	WepL
		Geom	GEO_WepL_ClawL
		#BHVR	Behaviors/ClawsFade.bhvr
	End

End

	

End	

	