#############################################################
## Binary_Body.fx
#############################################################

FxInfo
Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################
Condition
	On	Time
	Time 	0

#########################	EYES

	Event
		EName 	EyeAnchor
		Type	Local
		At	Eyes
		Geom	CircleOfThorns
	End
	Event
		Type	Local
		At	EyeAnchor
		AltPiv	1
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 -0.06 -0.125
		End
		Part	:Binary_EyeL.part
		Part	:Eye_Glow.part
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 -0.06 -0.125
		End
		Part	:Binary_EyeR.part
		Part	:Eye_Glow.part
	End


#########################	BODY

	Event
		EName 	Anchor
		Type	Local
		At	Root
		Part	:LightBlobs.part
		Part	:DataBeamsUp.part
		BhvrOverride
			PositionOffset	0.0 3 0
		End
	End

	Event
		Type	PositOnly
		At	Hips
		BhvrOverride
			PositionOffset	0.0 0 0
		End
		ChildFX	:Child_DataStrings.fx
	End

	Event
		Type	Local
		At	Root
		Part	:LightBlobs02.part
		BhvrOverride
			PositionOffset	0 2 0.5
		End
	End

	Event
		Type	Local
		At	Root
		part	:DataStringsUp02.part
		BhvrOverride
			PositionOffset	0 2.5 0
		End
	End

	Event
		Type	Local
		At	Root
		Part	:DataBeamsUp02.part
		BhvrOverride
			PositionOffset	0 2 0
		End
	End

End
#########################################################
End