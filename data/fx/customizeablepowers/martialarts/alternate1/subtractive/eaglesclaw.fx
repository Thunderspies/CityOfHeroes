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
		part1	CustomizeablePowers\MartialArts\Subtractive\MartialArtsShard.part
		part2	CustomizeablePowers\MartialArts\Subtractive\EaglesClawChestGlow.part
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
		part1	CustomizeablePowers\MartialArts\Subtractive\MAKickStreak01.part
		part2	CustomizeablePowers\MartialArts\Subtractive\MAMonkeyKickStreak01.part
		part3	CustomizeablePowers\MartialArts\Subtractive\MAKickStar01.part
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
		part1	CustomizeablePowers\MartialArts\Subtractive\EaglesClawShard.part
		part2	CustomizeablePowers\MartialArts\Subtractive\MonkeyKickSparkles.part
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
		part1	CustomizeablePowers\MartialArts\Subtractive\EaglesClawRing01.part
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
		part1	CustomizeablePowers\MartialArts\Subtractive\MartialArtsStar01.part
		part2	CustomizeablePowers\MartialArts\Subtractive\MartialArtsStar02.part
		part3	CustomizeablePowers\MartialArts\Subtractive\MartialArtsStarGlow.part
		LifeSpan 1
	End
	Event
		ENAME	FootFlash
		Type	Local
		At	WepR
		part1	CustomizeablePowers\MartialArts\Subtractive\MartialArtsStarBig.part
	End

End

#############################################################

End