#############################################################
## Fireflies_BodyAndEyes.fx
#############################################################

FxInfo

#########################################################

Condition
	On	Time
	Time	0

	Event
		EName 	EyeAnchor
		Type	Local
		Bhvr	behaviors/GenericParticleFade.bhvr
		At	Eyes
		Geom	CircleOfThorns
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	1
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.0 -0.05
		End
		Part	:EyeGlow_Pupil.part
		Part	:EyeGlow_BG.part
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.0 -0.05
		End
		Part	:EyeGlow_Pupil.part
		Part	:EyeGlow_BG.part

	End
End

#############################################################

End