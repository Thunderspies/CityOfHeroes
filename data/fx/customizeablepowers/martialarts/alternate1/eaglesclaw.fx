#############################################################
## EaglesClaw.fx
#############################################################

FxInfo
Lifespan 100

##################################


Condition
	On	Time
	Time	5

	Event
		ENAME	Jump
		Type	Local
		At	Chest
		part1	CustomizeablePowers\MartialArts\MartialArtsShard.part
		part2	CustomizeablePowers\MartialArts\EaglesClawChestGlow.part
		Sound EaglesClaw1Custom 60 60 1.0
		LifeSpan 47

	End

End

Condition
	On	Time
	Time	35

	Event
		ENAME	KickStreak
		Type	Local
		At	WepR
		part1	CustomizeablePowers\MartialArts\MAKickStreak01.part
		part2	CustomizeablePowers\MartialArts\MAMonkeyKickStreak01.part
		part3	CustomizeablePowers\MartialArts\MAKickStar01.part
		LifeSpan 15

	End

End

Condition
	On	Time
	Time	35

	Event
		ENAME	KickStreak2
		Type	Local
		At	WepR
		part1	CustomizeablePowers\MartialArts\EaglesClawShard.part
		part2	CustomizeablePowers\MartialArts\MonkeyKickSparkles.part
		LifeSpan 17

	End

End

Condition
	On	Time
	Time	48

	Event
		ENAME	Ring1
		Type	Local
		At	Chest
		part1	CustomizeablePowers\MartialArts\EaglesClawRing01.part
		LifeSpan 50

	End

End

Condition
	On	Time
	Time	44

	Event
		ENAME	Foot
		Type	Local
		At	WepR
		part1	:MartialArtsStar01.part
		part2	:MartialArtsStar02.part
		part3	:MartialArtsStarGlow.part
		LifeSpan 1
	End
	Event
		ENAME	FootFlash
		Type	Local
		At	WepR
		part1	:MartialArtsStarBig.part
	End

End

#############################################################

End