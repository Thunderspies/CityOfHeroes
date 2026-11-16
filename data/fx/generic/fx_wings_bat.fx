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
		Bhvr	behaviors/WingScale_BAT_Male.bhvr
		BhvrOverride
			PositionOffset	0 0.2 -.1
		End
		Anim	FX_wings_BAT
	End
End

#############################################################

End