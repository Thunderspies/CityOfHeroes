#############################################################
## Sparks_BodyAndEyes.fx
#############################################################

FxInfo

Flags NotCompatibleWithAnimalHead

#############################################################

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

#############################################################

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

#############################################################

Condition
	On	Cycle
	Time	35
	Chance	0.3

	Event
		Type	PositOnly
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
		part	:SparkGlow.part
		Lifespan 	1
	End
End

Condition
	On	Cycle
	Time	25
	Chance	0.25

	Event
		Type	PositOnly
		At	UarmR
		part	:SparkBurst_Long.part
		Part	:SparkBurst_Short.part
		Lifespan 	6
		LifespanJitter	4
	End

	Event
		Type	Local
		At	UarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow.part
		Lifespan 	1
	End
End

Condition
	On	Cycle
	Time	50
	Chance	0.3

	Event
		Type	PositOnly
		At	WepR
		part	:SparkBurst_Long.part
		Part	:SparkBurst_Short.part
		Lifespan 	6
		LifespanJitter	4
	End

	Event
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow.part
		Lifespan 	1
	End
End

Condition
	On	Cycle
	Time	32
	Chance	0.15

	Event
		Type	PositOnly
		At	UarmL
		part	:SparkBurst_Long.part
		Part	:SparkBurst_Short.part
		Lifespan 	6
		LifespanJitter	4
	End

	Event
		Type	Local
		At	UarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow.part
		Lifespan 	1
	End
End

Condition
	On	Cycle
	Time	35
	Chance	0.3

	Event
		Type	PositOnly
		At	WepL
		part	:SparkBurst_Long.part
		Part	:SparkBurst_Short.part
		Lifespan 	6
		LifespanJitter	4
	End

	Event
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow.part
		Lifespan 	1
	End
End

Condition
	On	Cycle
	Time	28
	Chance	0.15

	Event
		Type	PositOnly
		At	LlegR
		part	:SparkBurst_Long.part
		Part	:SparkBurst_Short.part
		Lifespan 	6
		LifespanJitter	4
	End

	Event
		Type	Local
		At	LlegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow.part
		Lifespan 	1
	End
End

Condition
	On	Cycle
	Time	13
	Chance	0.1

	Event
		Type	PositOnly
		At	LlegL
		part	:SparkBurst_Long.part
		Part	:SparkBurst_Short.part
		Lifespan 	6
		LifespanJitter	4
	End

	Event
		Type	Local
		At	LlegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow.part
		Lifespan 1
	End
End

Condition
	On	Cycle
	Time	50
	Chance	0.3

	Event
		Type	PositOnly
		At	FootL
		part	:SparkBurst_Long.part
		Part	:SparkBurst_Short.part
		Lifespan 	6
		LifespanJitter	4
	End

	Event
		Type	Local
		At	FootL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow.part
		Lifespan 1
	End
End



Condition
	On	Cycle
	Time	35
	Chance	0.3

	Event
		Type	PositOnly
		At	FootR
		part	:SparkBurst_Long.part
		Part	:SparkBurst_Short.part
		Lifespan 	6
		LifespanJitter	4
	End

	Event
		Type	Local
		At	FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow.part
		Lifespan 1
	End
End

#############################################################

End