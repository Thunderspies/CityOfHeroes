#############################################################
## SparksBody.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Cycle
	Time	30
	Chance	0.5

	Event
		Type	Local
		At	Origin
		Part	:SparkBurst_Equipment_Falling.part

		Lifespan 	10
		LifespanJitter	4
	End

	Event
		Type	Local
		At	Origin
	#	Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow_Equipment.part
		part	:SparkGlow_Equipment_Highlight.part
		part	:SparkBurst_Equipment.part
		ChildFX :Sparks_Equipment_SFX.fx
		Lifespan 3
		Lifespan 2
	End
End

Condition
	On	Cycle
	Time	10
	Chance	0.25

	Event
		Type	Local
		At	Origin
		Part	:SparkBurst_Equipment_Falling.part
		Lifespan 	6
		LifespanJitter	4
	End

	Event
		Type	Local
		At	Origin
	#	Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow_Equipment.part
		part	:SparkGlow_Equipment_Highlight.part
		part	:SparkBurst_Equipment.part
		ChildFX :Sparks_Equipment_SFX.fx
		Lifespan 3
		Lifespan 2
	End
End


#############################################################

End