#############################################################
## Sparks_Eyes.fx
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

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	1
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.01 -0.1
		End
		Part	:Eye_Glow.part
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.01 -0.1
		End
		Part	:Eye_Glow.part
	End
End

Condition
	On	Cycle
	Time	14
	Chance	0.25

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	1
		Part	:SparkBurst_Eyes.part
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	1
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:SparkGlow_Eyes.part
		Lifespan 1
	End
End

Condition
	On	Cycle
	Time	21
	Chance	0.37

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Part	:SparkBurst_Eyes.part
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:SparkGlow_Eyes.part
		Lifespan 1
	End
End

#########################################################

End