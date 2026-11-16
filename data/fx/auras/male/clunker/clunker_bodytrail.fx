#############################################################
## Clunker_Trail.fx
#############################################################

FxInfo
#############################################################

Condition
	On	Cycle
	Time	35
	Chance	0.1

	Event
		Type	PositOnly
		At	Head
		part	:SparkBurst_Long.part
		Part	:SparkBurst_Short.part
		childFX	:Child_ClunkerParts.fx
		Lifespan 	6
		LifespanJitter	4
	End

	Event
		Type	Local
		At	Head
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

Condition
	On	Cycle
	Time	32
	Chance	0.15

	Event
		Type	PositOnly
		At	UarmL
		part	:SparkBurst_Long.part
		Part	:SparkBurst_Short.part
		childFX	:Child_ClunkerParts.fx
		Lifespan 	6
		LifespanJitter	4
	End

	Event
		Type	Local
		At	UarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow.part
		ChildFX	:Child_SteamJet.fx
		Lifespan 	1
	End
End

Condition
	On	Cycle
	Time	35
	Chance	0.1

	Event
		Type	PositOnly
		At	WepL
		part	:SparkBurst_Long.part
		Part	:SparkBurst_Short.part
		childFX	:Child_ClunkerParts.fx
		Lifespan 	6
		LifespanJitter	4
	End

	Event
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:SparkGlow.part
		ChildFX	:Child_SteamJet.fx
		Lifespan 	1
	End
End

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

Condition
	On	Time
	Time	0




	Event
		Type 	Local
		At	UarmL
		ChildFx	:Child_OilDrips.fx
	End

	Event
		Type 	Local
		At	UarmR
		ChildFx	:Child_OilDrips.fx
	End

	Event
		Type 	Local
		At	LarmL
		ChildFx	:Child_OilDrips.fx
	End

	Event
		Type 	Local
		At	LarmR
		ChildFx	:Child_OilDrips.fx
	End

	Event
		Type 	Local
		At	LlegL
		ChildFx	:Child_OilDrips.fx
	End

	Event
		Type 	Local
		At	LlegR
		ChildFx	:Child_OilDrips.fx
	End
End
#################

Condition
	On 		TRIGGERBITS
	TRIGGERBITS    	MALE
	DoMany		1

	Event
		Ename	OilPuddles
		Type 	Local
		At	Root
		Until	Air
		Flags	OneAtATime
		ChildFX	:GrandChild_OilPuddles.fx
	End

	Event
		Ename	OilPuddles2
		Type 	Local
		At	Root
		Until	Air
		Flags	OneAtATime
		ChildFX	:GrandChild_Puddle_Trail.fx
	End
End

#############################################################
End