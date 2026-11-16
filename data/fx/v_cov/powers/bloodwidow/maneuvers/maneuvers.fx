#############################################################
## Blood Widow "Maneuvers" FX
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
		part1	POWERS\Leadership\LeadershipStar01.part
		pmagnet Chest
		Lifespan 70
	End
End
Condition
	On	Time
	Time	10

	Event
		
		Type	Local
		At	Head
		Sound Training_Maneuvers_02 70 70 .85
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
		part1	POWERS\Leadership\LeadershipGlowLrg01.part
		part2	POWERS\Leadership\LeadershipGlowSml01.part
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
		part1	POWERS\Leadership\LDefenseRays01.part
		pmagnet Chest
		Lifespan 70
	End
End

#############################################################

End