#############################################################
## SRFocusedSenses.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At 	Origin
		Sound Fast 80 80 0.8
	End
End

#############################################################

Condition
	On	Time
	Time	30

	Event
		ENAME	HeadGlow
		Type	Local
		At	Head
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\SuperReflexes\SRFSHeadCircles01.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	60

	Event
		ENAME	HeadGlow
		Type	Local
		At	Head
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\SuperReflexes\SRFSHeadSpikes01.part
		Lifespan 30
	End
End

Condition
	On	Cycle
	Time	120

	Event
		ENAME	HeadGlow
		Type	Local
		At	Head
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\SuperReflexes\SRFSHeadSpikes02.part
		Lifespan 5
	End
End

#############################################################

End