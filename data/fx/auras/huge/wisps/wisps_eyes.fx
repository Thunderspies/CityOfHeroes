#############################################################
## Wisps_Eyes.fx
#############################################################

FxInfo
Flags NotCompatibleWithAnimalHead DontSuppress

#########################################################

Condition
	On	Time
	Time	0

	Event
		EName 	EyeAnchor
		Type	Local
		At	Eyes
		Bhvr	behaviors/GenericParticleFade.bhvr

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
		Part	:EyeGlow_Pupil.part
		Part	:EyeGlow_BG.part
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 -0.06 -0.125
		End
		Part	:EyeGlow_Pupil.part
		Part	:EyeGlow_BG.part
	End
End

#############################################################

End