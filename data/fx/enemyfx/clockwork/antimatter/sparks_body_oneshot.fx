#############################################################
## SparksBody.fx
#############################################################

FxInfo
Lifespan 90

#############################################################

Condition
	On	Cycle
	Time	30
	Chance	0.15

	Event
		Type	PositOnly
		At	Hips
		part	:SparkBurst01.part
		Part	:SparkBurst02.part
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	PositOnly
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow01.part

		POther	Neck
		Lifespan 1
	End
End

Condition
	On	Cycle
	Time	35
	Chance	0.3

	Event
		Type	PositOnly
		At	Head
		part	:SparkBurst01.part
		Part	:SparkBurst02.part

		POther  Chest
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	Head
		part	:SparkGlow01.part

		POther  Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	25
	Chance	0.25

	Event
		Type	PositOnly
		At	UarmR
		part	:SparkBurst01.part
		Part	:SparkBurst02.part
		POther  LarmR
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	UarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow01.part
		POther  LarmR
		Lifespan 1
	End
End

Condition
	On	Cycle
	Time	50
	Chance	0.3

	Event
		Type	PositOnly
		At	WepR
		part	:SparkBurst01.part
		Part	:SparkBurst02.part
		POther  LarmR
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow01.part

		POther  LarmR
		Lifespan 1
	End
End

Condition
	On	Cycle
	Time	32
	Chance	0.15

	Event
		Type	PositOnly
		At	UarmL
		part	:SparkBurst01.part
		Part	:SparkBurst02.part
		POther  LarmL
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	UarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow01.part

		POther  LarmL
		Lifespan 1
	End
End

Condition
	On	Cycle
	Time	35
	Chance	0.3

	Event
		Type	PositOnly
		At	WepL
		part	:SparkBurst01.part
		Part	:SparkBurst02.part
		POther  LarmL
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow01.part

		POther  LarmL
		Lifespan 1
	End
End

Condition
	On	Cycle
	Time	25
	Chance	0.25

	Event
		Type	PositOnly
		At	UlegL
		part	:SparkBurst01.part
		Part	:SparkBurst02.part
		POther  LlegL
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	UlegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow01.part

		POther  LlegL
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
		part	:SparkBurst01.part
		Part	:SparkBurst02.part
		POther  Llegl
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

Condition
	On	Cycle
	Time	28
	Chance	0.15

	Event
		Type	PositOnly
		At	UlegR
		part	:SparkBurst01.part
		Part	:SparkBurst02.part
		POther  LlegR
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	UlegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow01.part

		POther  LlegR
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
		part	:SparkBurst01.part
		Part	:SparkBurst02.part
		POther  LlegR
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

#############################################################

End