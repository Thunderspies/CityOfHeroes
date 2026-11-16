#############################################################
## Header
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

## WINGS ###########################################################

Condition
	On 	Time
	Time 	1

	Event
		Ename	WingAnim
		Type	Local
		At	Back
		Bhvr	behaviors/WingScale_BURNED_Huge.bhvr
		BhvrOverride
			PositionOffset	0 .4 -.30
		End
		Anim	FX_wings_BURNED
	End
End

#############################################################

End