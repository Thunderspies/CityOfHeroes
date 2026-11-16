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
		Anim	FX_wings_DRAGON

		Bhvr	behaviors/WingScale_DRAGON_Female.bhvr
		BhvrOverride
			PositionOffset	0 .10 .1
		End
	End
End

#############################################################

End