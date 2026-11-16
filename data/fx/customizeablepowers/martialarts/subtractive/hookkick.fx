#############################################################
## HookKick.fx
#############################################################

FxInfo
LifeSpan	100

##################################

Condition
	On	Time
	Time	13

	Event
		ENAME	KickStreak1
		Type	Local
		At	FootR
		part1	:MAKickStreak02dim.part
		Sound HookKick2 60 60 1.0
		LifeSpan 9

	End

End

Condition
	On	Time
	Time	30

	Event
		ENAME	KickStreak2
		Type	Local
		At	FootR
		part1	:MAKickStreak01.part
		part2	:MAKickStreak02.part
		part3	:MAKickStar01.part
		LifeSpan 13

	End

End


End