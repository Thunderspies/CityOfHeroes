#############################################################
## HEADER
#############################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

LifeSpan 40

#############################################################

Condition

	On 	Time
	Time	0

	Event 	Type 	Local
		At 	origin
		Sound placate2 70 70 .8
	End
End

#############################################################

Condition
	On	Cycle
	Time	15

	Event
		ENAME	AfraidSkull01
		Type	Local
		At	Head
		part	CustomizeablePowers\MartialArts\PlacateCore.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 15
	End

	Event
		ENAME	Rings
		Type	Local
		At	node
		Altpiv	1
		Bhvr	CustomizeablePowers\MartialArts\NodeRotate.bhvr
		part	CustomizeablePowers\MartialArts\PlacateStreams01.part
		part	CustomizeablePowers\MartialArts\AfraidRings01.part
		part	CustomizeablePowers\MartialArts\PlacateCore2.part
		part	CustomizeablePowers\MartialArts\PlacateStreams02.part
		Lifespan 15
	End
End

Condition
	On	Time
	Time	0

	Event
		ENAME	node
		Type	Local
		At	Head
		Geom	CenterDummy
	End

	Event
		ENAME	Rings
		Type	Local
		At	node
		Altpiv	1
		Bhvr	CustomizeablePowers\MartialArts\NodeRotate.bhvr
	End
End

#############################################################

End