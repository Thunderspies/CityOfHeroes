#############################################################
## SparksBody.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Cycle
	Time	14
	Chance	0.25

	Event
		Type	PositOnly
		At	LArmR
		part	:SparkBurst01.part
		Part	:SparkBurst02.part
		POther  WepR
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	LArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow01.part
		POther  WepR
		Lifespan 1
	End
End

Condition
	On	Cycle
	Time	20
	Chance	0.2

	Event
		Type	PositOnly
		At	WepR
		part	:SparkBurst01.part
		Part	:SparkBurst02.part
		POther  LArmR
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow01.part

		POther  LArmR
		Lifespan 1
	End
End


#############################################################

End