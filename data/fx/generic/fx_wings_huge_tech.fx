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
		Anim	FX_wings_TECH

		Bhvr	behaviors/WingScale_Tech_Huge.bhvr
		BhvrOverride
			PositionOffset	0 .4 -.45
	End
End

#############################################################

End