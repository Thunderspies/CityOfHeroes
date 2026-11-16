#############################################################
## Blood Widow "Mind Link" FX
#############################################################

FxInfo

Lifespan 150

#############################################################

Condition
	On	Time
	Time	15

	Event
		EName	Ring
		Type	Local
		At	Head
		Bhvr	behaviors\GenericParticleFade.bhvr
		part1	POWERS\Leadership\LTacticsStar01.part
		part1	POWERS\Leadership\LTacticsRays01.part
		pmagnet Chest
		Lifespan 70
	End
End

Condition
	On	Time
	Time	12

	Event
		EName	Ring
		Type	Local
		At	Head
		Bhvr	behaviors\GenericParticleFade.bhvr
		part1	POWERS\Leadership\LTacticsGlowLrg01.part
		part2	POWERS\Leadership\LTacticsGlowSml01.part
		pmagnet Chest
		Lifespan 150
	End
End

#############################################################

End