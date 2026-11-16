#############################################################
## Assault_Activation.fx
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
		part1	:LAssaultStar01.part
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
		part1	:LAssaultGlowLrg01.part
		part2	:LAssaultGlowSml01.part
		pmagnet Chest
		Sound Fanfare1 70 70 .66
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
		part1	:LAssaultRays01.part
		pmagnet Chest
		Lifespan 70

	End

End

End