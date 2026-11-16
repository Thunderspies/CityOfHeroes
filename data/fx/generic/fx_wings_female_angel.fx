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
		Bhvr	behaviors/WingScale_ANGEL_Female.bhvr
		BhvrOverride
			PositionOffset	0 .15 .1
		End
		Anim	FX_wings_ANGEL
	End
End

#############################################################

End