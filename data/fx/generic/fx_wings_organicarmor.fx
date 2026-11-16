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
		Bhvr	behaviors/WingScale_ORGANICARMOR_Male.bhvr
		BhvrOverride
			PositionOffset	0 .05 -.2
		End
		Anim	FX_wings_ORGANICARMOR
	End
End

#############################################################

End