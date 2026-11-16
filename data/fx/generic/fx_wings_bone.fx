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
		Bhvr	behaviors/WingScale_Bone_Male.bhvr
		BhvrOverride
			PositionOffset	0 .25 -.40
		End
		Anim	FX_wings_BONE
	End
End

#############################################################

End