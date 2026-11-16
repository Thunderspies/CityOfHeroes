#########################################################
##	jack
##
FxInfo

Flags    InheritAlpha

Input  
	InpName	WepR
End

Flags    InheritAlpha

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	WepR
		Geom	GEO_WepR_Revolver_1
		Bhvr	:GunslingerRevolverScale.bhvr
		Sound revolverout 50 50 .33
	End
End

End			