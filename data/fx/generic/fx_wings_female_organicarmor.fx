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
		Bhvr	behaviors/WingScale_ORGANICARMOR_Female.bhvr
		BhvrOverride
			PositionOffset	0 .10 0
		End
		Anim	FX_wings_ORGANICARMOR
	End
End

#############################################################

End