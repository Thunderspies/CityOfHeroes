#############################################################
## Blood Widow "Assault" FX
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
		part1	POWERS\Leadership\LAssaultStar01.part
		pmagnet Chest
		Sound heal1 70 70 .66
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
		part1	POWERS\Leadership\LAssaultGlowLrg01.part
		part2	POWERS\Leadership\LAssaultGlowSml01.part
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
		part1	POWERS\Leadership\LAssaultRays01.part
		pmagnet Chest
		Lifespan 70
	End
End

#############################################################

End