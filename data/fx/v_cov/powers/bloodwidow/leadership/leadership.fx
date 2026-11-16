#############################################################
## Blood Widow "Leadership" FX
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	15

	Event
		EName	Ring
		Type	Local
		At	Head
		part1	POWERS\Leadership\LTacticsStar01.part
		pmagnet Chest
		Sound Training_Leadership_01 70 70 .75
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
		part1	POWERS\Leadership\LTacticsGlowLrg01.part
		part2	POWERS\Leadership\LTacticsGlowSml01.part
		pmagnet Chest
		Lifespan 150
	End
End

Condition
	On	Time
	Time	15

	Event
		EName	Ring
		Type	Local
		At	Head
		part1	POWERS\Leadership\LTacticsRays01.part
		pmagnet Chest
		Lifespan 70
	End
End

#############################################################

End