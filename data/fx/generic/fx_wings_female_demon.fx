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
		Anim	FX_wings_DEMON

		Bhvr	behaviors/WingScale_DEMON_Female.bhvr
		BhvrOverride
			PositionOffset	0 .13 .08
		End
	End
End

#############################################################

End