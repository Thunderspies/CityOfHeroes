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
	Time	3
	Random	1
	Chance 	0.5



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
		Lifespan 	4
		LifespanJitter	2
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
		Lifespan 	4
		LifespanJitter	2
	End
End

#############################################################

End