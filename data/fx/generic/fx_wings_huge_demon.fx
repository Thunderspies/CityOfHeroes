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
		Bhvr	behaviors/WingScale_DEMON_Huge.bhvr
		BhvrOverride
			PositionOffset	0 .25 -.33
		End
		Anim	FX_wings_DEMON
	End
End

#############################################################

End