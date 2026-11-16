#############################################################
## Header
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	2

	Event
		Type	Local
		At	Back
		Anim	FX_wings_bugs_blur_2
		Bhvr	behaviors/WingScale_INSECT_Female.bhvr
		BhvrOverride
			PositionOffset	0 .1 .1
		End
	End
End

#############################################################

End