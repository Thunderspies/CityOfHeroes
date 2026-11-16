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
		Bhvr	behaviors/WingScale_FAIRY_Female.bhvr
		BhvrOverride
			PositionOffset	0 .15 .1
		End
		Anim	FX_wings_FAIRY
	End
End

#############################################################

End