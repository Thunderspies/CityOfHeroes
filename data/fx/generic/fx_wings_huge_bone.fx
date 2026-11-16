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
		Bhvr	behaviors/WingScale_BONE_Huge.bhvr
		BhvrOverride
			PositionOffset	0 .45 -.87
		End
		Anim	FX_wings_BONE
	End
End

#############################################################

End