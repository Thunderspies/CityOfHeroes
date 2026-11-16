#############################################################
## Fireflies_BodyAndEyes.fx
#############################################################

FxInfo

Flags NotCompatibleWithAnimalHead

#########################################################

Condition
	On	Time
	Time	0

	Event
		EName 	EyeAnchor
		Type	Local
		At	Eyes
		Geom	CircleOfThorns
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.75
	Random	1


	Event
		Type	Local
		At	EyeAnchor
		AltPiv	1
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.01 -0.1
		End
		Part	:EyeGlow_Pupil.part
		Part	:EyeGlow_BG.part
		Part	:Fireworks_Tendril_Eyes.part
		Lifespan 10

	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.01 -0.1
		End
		Part	:EyeGlow_Pupil.part
		Part	:EyeGlow_BG.part
		Part	:Fireworks_Tendril_Eyes.part
		Lifespan 10

	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	1
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.01 -0.1
		End
		Part	:EyeGlow_Pupil2.part
		Part	:EyeGlow_BG2.part
		Part	:Fireworks_Tendril2_Eyes.part
		Lifespan 10

	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.01 -0.1
		End
		Part	:EyeGlow_Pupil2.part
		Part	:EyeGlow_BG2.part
		Part	:Fireworks_Tendril2_Eyes.part
		Lifespan 10

	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	1
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.01 -0.1
		End
		Part	:EyeGlow_Pupil3.part
		Part	:EyeGlow_BG3.part
		Part	:Fireworks_Tendril3_Eyes.part
		Lifespan 10

	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.01 -0.1
		End
		Part	:EyeGlow_Pupil3.part
		Part	:EyeGlow_BG3.part
		Part	:Fireworks_Tendril3_Eyes.part
		Lifespan 10

	End
End

#############################################################

End