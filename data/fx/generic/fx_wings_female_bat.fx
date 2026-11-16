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
		Bhvr	behaviors/WingScale_BAT_Female.bhvr
		BhvrOverride
			PositionOffset	0 .20 0
		End
		Anim	FX_wings_BAT
	End
End

#############################################################

End