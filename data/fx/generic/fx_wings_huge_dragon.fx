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
		Bhvr	behaviors/WingScale_DRAGON_Huge.bhvr
		BhvrOverride
			PositionOffset	0 .25 -.2
		End
		Anim	FX_wings_DRAGON
	End
End

#############################################################

End