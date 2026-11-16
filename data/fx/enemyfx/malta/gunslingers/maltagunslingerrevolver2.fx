#########################################################
##	Note: this 2nd FX is used to simulate a fancy gun action between the Cone and the regular attacks
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