###############################################################
## Tactics_Activation.fx
#############################################################

FxInfo

#############################################################

Input
	InpName	Hips
End

#############################################################

Condition
	On	Time
	Time	15

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

Condition
	On	Time
	Time	12

	Event
		EName	Ring
		Type	Local
		At	Head
		part1	:LTacticsGlowLrg01.part
		part2	:LTacticsGlowSml01.part
		pmagnet Chest
		Sound heal1 70 70 .66
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
		part1	:LTacticsRays01.part
		pmagnet Chest
		Lifespan 70

	End

End

End