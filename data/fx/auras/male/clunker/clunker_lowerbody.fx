#############################################################
## Sparks_Body.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Cycle
	Time	28
	Chance	0.1

	Event
		Type	PositOnly
		At	LlegR
		part	:SparkBurst_Long.part
		Part	:SparkBurst_Short.part
		childFX	:Child_ClunkerParts.fx
		Lifespan 	6
		LifespanJitter	4
	End

	Event
		Type	Local
		At	LlegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow.part
		ChildFX	:Child_SteamJet.fx
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
		childFX	:Child_ClunkerParts.fx
		Lifespan 	6
		LifespanJitter	4
	End

	Event
		Type	Local
		At	LlegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow.part
		ChildFX	:Child_SteamJet.fx
		Lifespan 1
	End
End

Condition
	On	Cycle
	Time	50
	Chance	0.05

	Event
		Type	PositOnly
		At	FootL
		part	:SparkBurst_Long.part
		Part	:SparkBurst_Short.part
		childFX	:Child_ClunkerParts.fx
		Lifespan 	6
		LifespanJitter	4
	End

	Event
		Type	Local
		At	FootL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow.part
		ChildFX	:Child_SteamJet.fx
		Lifespan 1
	End
End



Condition
	On	Cycle
	Time	35
	Chance	0.05

	Event
		Type	PositOnly
		At	FootR
		part	:SparkBurst_Long.part
		Part	:SparkBurst_Short.part
		childFX	:Child_ClunkerParts.fx
		Lifespan 	6
		LifespanJitter	4
	End

	Event
		Type	Local
		At	FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow.part
		ChildFX	:Child_SteamJet.fx
		Lifespan 1
	End
End

#############################################################

End