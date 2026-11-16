#############################################################
## Sparks_Hair.fx
#############################################################

FxInfo

#############################################################

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

#############################################################

End