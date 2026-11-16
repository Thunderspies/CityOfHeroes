#############################################################
## HookKick.fx
#############################################################

FxInfo
LifeSpan	100

##################################
Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	WepR
		Sound HookKick2 60 60 1.0
	End

End
Condition
	On	Time
	Time	10

	Event
		ENAME	KickStreak1
		Type	Local
		At	WepR
		part1	CustomizeablePowers\MartialArts\MAKickStreak02dim.part
		#Sound 	HookKick2 60 30 1.0
		LifeSpan 9

	End

End

Condition
	On	Time
	Time	15

	Event
		ENAME	KickStreak2
		Type	Local
		At	WepR
		part1	CustomizeablePowers\MartialArts\MAKickStreak01.part
		part2	CustomizeablePowers\MartialArts\MAKickStreak02.part
		part3	CustomizeablePowers\MartialArts\MAKickStar01.part
		LifeSpan 13

	End

End


End