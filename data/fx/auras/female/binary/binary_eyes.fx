#############################################################
## Binary_Eyes.fx
#############################################################

FxInfo

Condition
	On	Time
	Time 	0

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
			PositionOffset	0.0 0.01 -0.05
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
			PositionOffset	0.0 0.01 -0.05
		End
		Part	:Binary_EyeR.part
		Part	:Eye_Glow.part
	End

End

#############################################################

End