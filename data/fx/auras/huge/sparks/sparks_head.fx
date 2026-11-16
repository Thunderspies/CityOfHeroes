#############################################################
## Sparks_Head.fx
#############################################################

FxInfo

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		ChildFX :Sparks_Eyes.fx
		Lifespan 	5
		LifespanJitter	3
	End
End

Condition
	On	Cycle
	Time	20
	Chance	0.5

	Event
		Type	Local
		At	Head
		part	:SparkBurst_Long.part
		Part	:SparkBurst_Short.part
		Lifespan 	6
		LifespanJitter	4
	End

	Event
		Type	Local
		At	Head
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow_Head.part
		Lifespan 1
	End
End

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
			PositionOffset	0.0 -0.05 -0.15
		End
		Part	:Eye_Glow.part
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 -0.05 -0.15
		End
		Part	:Eye_Glow.part
	End
End

#########################################################

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