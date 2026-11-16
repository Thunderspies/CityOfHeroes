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
		At	LArmL
		part	:SparkBurst01.part
		Part	:SparkBurst02.part
		POther  WepL
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	LArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow01.part
		POther  WepL
		Lifespan 1
	End
End

Condition
	On	Cycle
	Time	20
	Chance	0.2

	Event
		Type	PositOnly
		At	WepL
		part	:SparkBurst01.part
		Part	:SparkBurst02.part
		POther  LArmL
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow01.part

		POther  LArmL
		Lifespan 1
	End
End


#############################################################

End