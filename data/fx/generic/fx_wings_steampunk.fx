#############################################################
## Header
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Back
		Bhvr	behaviors/WingScale_Angel_Male.bhvr
		Anim	FX_wings_STEAMPUNK
	End
End

#############################################################

End