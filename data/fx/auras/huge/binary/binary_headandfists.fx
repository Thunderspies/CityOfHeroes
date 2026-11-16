#############################################################
## Binary_Eyes.fx
#############################################################

FxInfo

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

#########################

	Event
		Type	Local
		At	Head
		ChildFX	:CHILD_Binary_Sm.fx
	End

	Event
		Type	Local
		At	HandR
		ChildFX	:CHILD_Binary_Sm.fx
	End

	Event
		Type	Local
		At	HandL
		ChildFX	:CHILD_Binary_Sm.fx
	End

End

#############################################################

End