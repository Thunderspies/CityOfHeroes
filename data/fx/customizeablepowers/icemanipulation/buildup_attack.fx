#############################################################
## BuildUp_Attack.fx
#############################################################

FxInfo

Lifespan 75

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

#############################################################

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	CHEST
		Bhvr	CustomizeablePowers\IceManipulation\icearmor.bhvr
		Part	CustomizeablePowers\IceManipulation\Frost2.part
		part1	CustomizeablePowers\IceManipulation\bodychillbig.part
		part1	CustomizeablePowers\IceManipulation\SnowBurst.part
		LifeSpan 20
	End

	Event
		Type	Local
		At	Hips
		Bhvr	CustomizeablePowers\IceManipulation\icearmor.bhvr
		part1	CustomizeablePowers\IceManipulation\bodychillbig.part
		part1	CustomizeablePowers\IceManipulation\SnowBurst.part
		LifeSpan 20
	End

	Event
		Type	Local
		At	UARML
		Bhvr	CustomizeablePowers\IceManipulation\icearmor.bhvr
		part1	CustomizeablePowers\IceManipulation\bodychillbig.part
		part1	CustomizeablePowers\IceManipulation\SnowBurst.part
		Part	CustomizeablePowers\IceManipulation\FrostSmall.part
		LifeSpan 20
	End

	Event
		Type	Local
		At	UARMR
		Bhvr	CustomizeablePowers\IceManipulation\icearmor.bhvr
		part1	CustomizeablePowers\IceManipulation\bodychillbig.part
		part1	CustomizeablePowers\IceManipulation\SnowBurst.part
		Part	CustomizeablePowers\IceManipulation\FrostSmall.part
		LifeSpan 20
	End

	Event
		Type	Local
		At	LARMR
		Bhvr	CustomizeablePowers\IceManipulation\icearmor.bhvr
		part1	CustomizeablePowers\IceManipulation\bodychillbig.part
		part1	CustomizeablePowers\IceManipulation\SnowBurst.part
		Part	CustomizeablePowers\IceManipulation\FrostSmall.part
		LifeSpan 20
	End

	Event
		Type	Local
		At	LARML
		Bhvr	CustomizeablePowers\IceManipulation\icearmor.bhvr
		part1	CustomizeablePowers\IceManipulation\bodychillbig.part
		part1	CustomizeablePowers\IceManipulation\SnowBurst.part
		Part	CustomizeablePowers\IceManipulation\FrostSmall.part
		LifeSpan 20
	End

	Event
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\IceManipulation\Snowfall.part
		Part	CustomizeablePowers\IceManipulation\FrostSmall.part
		LifeSpan 20
	End

	Event
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\IceManipulation\Snowfall.part
		Part	CustomizeablePowers\IceManipulation\FrostSmall.part
		LifeSpan 20
	End
End

Condition
	On	Time
	Time	7

	Event
		Type	Local
		At	UlegL
		Bhvr	CustomizeablePowers\IceManipulation\icearmor.bhvr
		Part	CustomizeablePowers\IceManipulation\FrostSmall.part
		LifeSpan 17
	End

	Event
		Type	Local
		At	UlegR
		Bhvr	CustomizeablePowers\IceManipulation\icearmor.bhvr
		Part	CustomizeablePowers\IceManipulation\FrostSmall.part
		LifeSpan 17
	End

	Event
		Type	Local
		At	LlegR
		Bhvr	CustomizeablePowers\IceManipulation\icearmor.bhvr
		Part	CustomizeablePowers\IceManipulation\FrostSmall.part
		LifeSpan 17
	End

	Event
		Type	Local
		At	LlegL
		Bhvr	CustomizeablePowers\IceManipulation\icearmor.bhvr
		Part	CustomizeablePowers\IceManipulation\FrostSmall.part
		LifeSpan 17
	End
End

Condition
	On	Time
	Time	9

	Event
		Type	Local
		At	FootL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\IceManipulation\Snowfall.part
		Part	CustomizeablePowers\IceManipulation\FrostSmall.part
		LifeSpan 13
	End

	Event
		Type	Local
		At	FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\IceManipulation\Snowfall.part
		Part	CustomizeablePowers\IceManipulation\FrostSmall.part
		LifeSpan 13
	End
End

#############################################################

Condition
	On	Time
	Time	15

	Event
		Type 	local
		At	Root
		ChildFX	POWERS\Child_BuildUp.fx
	End
End

#############################################################

End