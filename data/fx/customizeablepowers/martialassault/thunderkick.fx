#############################################################
## ThunderKick.fx
#############################################################

FxInfo
Lifespan 100

##################################

Condition
	On	Time
	Time	9

	Event
		ENAME	KickFlash
		Type	Local
		At	FootR
		part1	CustomizeablePowers\MartialArts\MAKickStreak01.part
		part2	CustomizeablePowers\MartialArts\MAKickStreak02.part
		part3	CustomizeablePowers\MartialArts\MAKickStar01.part
		LifeSpan 10

	End

End

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	FootR
		Sound whoosh13 60 60 .88
		LifeSpan 10

	End

End

##################################
End

