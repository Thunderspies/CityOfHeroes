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
		Anim	FX_wings_CHERUB
		Bhvr	behaviors/WingScale_CHERUB_Huge.bhvr
		BhvrOverride
			PositionOffset	0 .37 -.25
		End
	End
End

#############################################################

End