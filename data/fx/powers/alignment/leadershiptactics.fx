#############################################################
## LeadershipTactics.fx
#############################################################

FxInfo

##################################

Condition
	On	Time
	Time	0

	Event
		EName	Ring
		Type	Local
		At	Head
		part1	:LTacticsStar01.part
		pmagnet Chest
		Sound fanfare1 70 70 .8
		Lifespan 70

	End

End