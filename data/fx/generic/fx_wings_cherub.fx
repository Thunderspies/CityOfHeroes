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
		Bhvr	behaviors/WingScale_CHERUB_MALE.bhvr
		BhvrOverride
			PositionOffset	0 .15 0
		End
		Anim	FX_wings_CHERUB
	End
End

#############################################################

End