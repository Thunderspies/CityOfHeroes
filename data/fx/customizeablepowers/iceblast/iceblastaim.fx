#############################################################
## IceBlastAim.fx
#############################################################

FxInfo

Lifespan 75

#############################################################

Condition
	On	Time
	Time	3

	Event
		Type	Local
		At	CHEST
		Bhvr 	:icearmor.bhvr
		part	:SnowBurst.part
		Part	:Frost2.part
		LifeSpan 20
	End

	Event
		Type	Local
		At	UARML
		Bhvr 	:icearmor.bhvr
		part1   :SnowBurst.part
		Part	:FrostSmall.part
		LifeSpan 20
	End

	Event
		Type	Local
		At	UARMR
		Bhvr 	:icearmor.bhvr
		part1   :SnowBurst.part
		Part	:FrostSmall.part
		LifeSpan 20
	End

	Event
		Type	Local
		At	LARMR
		Bhvr 	:icearmor.bhvr
		part1   :SnowBurst.part
		Part	:FrostSmall.part
		LifeSpan 20
	End

	Event
		Type	Local
		At	LARML
		Bhvr 	:icearmor.bhvr
		part1   :SnowBurst.part
		Part	:FrostSmall.part
		LifeSpan 20
	End

	Event
		Type	Local
		At	UlegL
		Bhvr 	:icearmor.bhvr
		Part	:FrostSmall.part
		LifeSpan 17
	End

	Event
		Type	Local
		At	UlegR
		Bhvr 	:icearmor.bhvr
		Part	:FrostSmall.part
		LifeSpan 17
	End

	Event
		Type	Local
		At	LlegR
		Bhvr 	:icearmor.bhvr
		Part	:FrostSmall.part
		LifeSpan 17
	End
		Event
		Type	Local
		At	LlegL
		Bhvr 	:icearmor.bhvr
		Part	:FrostSmall.part
		LifeSpan 17
	End
End

#############################################################

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1   :Snowfall.part
		Part	:FrostSmall.part
		LifeSpan 20
	End

	Event
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1   :Snowfall.part
		Part	:FrostSmall.part
		LifeSpan 20
	End

	Event
		Type	Local
		At	FootL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1   :Snowfall.part
		Part	:FrostSmall.part
		LifeSpan 13
	End

	Event
		Type	Local
		At	FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1   :Snowfall.part
		Part	:FrostSmall.part
		LifeSpan 13
	End
End

#############################################################

Condition
	On	Time
	Time	0
	Repeat	5
	RepeatJitter 5

	Event
		Type	Local
		At	CHEST
		ChildFx	:Child_IceChunkScatterSmallNonJagged.fx
		LifeSpan 120
	End
End

## GENERIC FX ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		ChildFX	POWERS\Child_Aim.fx
	End
End

#############################################################

End