#############################################################
## MonkeyKick.fx
#############################################################

FxInfo

LifeSpan 100

#############################################################

Condition
	On	Time
	Time	3

	Event
		ENAME	Jump
		Type	Local
		At	FootR
		part1	:MAKickStreak02.part
		Sound Monkey4 60 60 0.9
		LifeSpan 20
	End
End

Condition
	On	Time
	Time	10

	Event
		ENAME	KickStreak2
		Type	Local
		At	FootR
		part1	:MAKickStreak01.part
		part2	:MAMonkeyKickStreak01.part
		part3	:MAKickStar01.part
		LifeSpan 12
	End
End

Condition
	On	Time
	Time	10

	Event
		ENAME	KickStreakLrg
		Type	Local
		At	FootR
		part1	:MonkeyKickShard.part
		part2	:MonkeyKickSparkles.part
		LifeSpan 12
	End
End

#############################################################

Condition
	On	Time
	Time	17

	Event
		Type	Local
		At	Target
		Sound Hit6 100.0 100.0 0.9
		LifeSpan 20
	End
End

#############################################################

End