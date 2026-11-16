#############################################################
## HEADER
#############################################################

FxInfo

LifeSpan 30

ClampMaxScale 3.5 3.5 3.5

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\EarthControl\EarthRocksA.part
		Part2	CustomizeablePowers\EarthControl\EarthMistA.part
		Part2	CustomizeablePowers\EarthControl\EarthMistB.part
	End
End

#############################################################

End