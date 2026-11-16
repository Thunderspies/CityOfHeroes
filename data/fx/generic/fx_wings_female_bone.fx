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
		Bhvr	behaviors/WingScale_BONE_Female.bhvr
		BhvrOverride
			PositionOffset	0 .2 -.20
		End
		Anim	FX_wings_BONE
	End
End

#############################################################

End