#############################################################
## Sparks_FistR.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Cycle
	Time	20
	Chance	0.25

	Event
		Type	PositOnly
		At	LArmR
		part	:SparkBurst_Long.part
		Part	:SparkBurst_Short.part
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	LArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow.part
		Lifespan 1
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.3

	Event
		Type	PositOnly
		At	WepR
		part	:SparkBurst_Long.part
		Part	:SparkBurst_Short.part
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow_Head.part
		Lifespan 1
	End
End


#############################################################

End