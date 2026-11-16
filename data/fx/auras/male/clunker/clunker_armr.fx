#############################################################
## Sparks_Body.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Cycle
	Time	25
	Chance	0.15

	Event
		Type	PositOnly
		At	UarmR
		part	:SparkBurst_Long.part
		Part	:SparkBurst_Short.part
		childFX	:Child_ClunkerParts.fx
		Lifespan 	6
		LifespanJitter	4
	End

	Event
		Type	Local
		At	UarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow.part
		ChildFX	:Child_SteamJet.fx
		Lifespan 	1
	End
End

Condition
	On	Cycle
	Time	25
	Chance	0.15

	Event
		Type	PositOnly
		At	LarmR
		part	:SparkBurst_Long.part
		Part	:SparkBurst_Short.part
		childFX	:Child_ClunkerParts.fx
		Lifespan 	6
		LifespanJitter	4
	End

	Event
		Type	Local
		At	LarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow.part
		ChildFX	:Child_SteamJet.fx
		Lifespan 	1
	End
End
Condition
	On	Cycle
	Time	50
	Chance	0.1

	Event
		Type	PositOnly
		At	WepR
		part	:SparkBurst_Long.part
		Part	:SparkBurst_Short.part
		childFX	:Child_ClunkerParts.fx
		Lifespan 	6
		LifespanJitter	4
	End

	Event
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow.part
		ChildFX	:Child_SteamJet.fx
		Lifespan 	1
	End
End


#############################################################

End