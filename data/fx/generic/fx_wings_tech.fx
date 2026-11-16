#############################################################
## Header
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		At	Back
		Bhvr	behaviors/WingScale_Tech_Male.bhvr
		BhvrOverride
			PositionOffset	0 .2 -.2
		End
		Anim	FX_wings_TECH
	End
End

#############################################################

End