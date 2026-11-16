#############################################################
## SparksBody.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Cycle
	Time	14
	Chance	0.18

	Event
		Type	PositOnly
		At	LLegR
		part	:SparkBurst01.part
		Part	:SparkBurst02.part
		POther  FootR
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	LLegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow01.part
		POther  FootR
		Lifespan 1
	End
End

Condition
	On	Cycle
	Time	20
	Chance	0.25

	Event
		Type	PositOnly
		At	FootR
		part	:SparkBurst01.part
		Part	:SparkBurst02.part
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow01.part
		Lifespan 1
	End
End

Condition
	On	Cycle
	Time	18
	Chance	0.25

	Event
		Type	PositOnly
		At	LLegL
		part	:SparkBurst01.part
		Part	:SparkBurst02.part
		POther  FootL
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	LLegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow01.part

		POther  FootL
		Lifespan 1
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.3

	Event
		Type	PositOnly
		At	FootL
		part	:SparkBurst01.part
		Part	:SparkBurst02.part
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	FootL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow01.part
		Lifespan 1
	End
End

#############################################################

End