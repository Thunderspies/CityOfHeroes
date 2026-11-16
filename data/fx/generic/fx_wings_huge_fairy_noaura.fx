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
		Anim	FX_wings_FAIRY

		Bhvr	behaviors/WingScale_FAIRY_Huge.bhvr
		BhvrOverride
			PositionOffset	0 .45 -.27
		End
	End
End

#############################################################

End