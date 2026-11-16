#############################################################
## Header
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

## WINGS ###########################################################

Condition
	On 	Time
	Time 	1

	Event
		Ename	WingAnim
		Type	Local
		At	Back
		Anim	FX_wings_FAIRY
	End
End

#############################################################

End