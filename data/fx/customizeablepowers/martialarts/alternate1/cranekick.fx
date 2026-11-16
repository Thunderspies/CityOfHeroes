#############################################################
## CraneKick.fx
#############################################################

FxInfo
LifeSpan	100

##################################

Condition
	On	Time
	Time	0

	Event
		ENAME	LightShards
		Type	Local
		At	Chest
		part1	CustomizeablePowers\MartialArts\MartialArtsShard.part
		pmagnet	Chest
		Sound Cobra4 60 60 .9
		Lifespan 29
	End

End

Condition
	On	Time
	Time	2

	Event
		ENAME	Glow
		Type	Local
		At	Chest
		part1	CustomizeablePowers\MartialArts\MartialArtsChestGlow.part


	End

End

Condition
	On	Time
	Time	1

	Event
		ENAME	Ring
		Type	Local
		At	Chest
		part1	CustomizeablePowers\MartialArts\MartialArtsRing.part


	End

End

Condition
	On	Time
	Time	6

	Event
		ENAME	FootStars
		Type	Local
		At	FootR
		part1	CustomizeablePowers\MartialArts\MartialArtsStar01.part
		part2	CustomizeablePowers\MartialArts\MartialArtsStar02.part
		part3	CustomizeablePowers\MartialArts\MartialArtsStarGlow.part


	End

End

Condition
	On	Time
	Time	17

	Event
		ENAME	FootFlash
		Type	Local
		At	FootR
		part1	CustomizeablePowers\MartialArts\MartialArtsStarBig.part

	End

End

Condition
	On	Time
	Time	12

	Event
		ENAME	FootBlur
		Type	Local
		At	FootR
		part1	CustomizeablePowers\MartialArts\MAKickStreak02.part
		Lifespan 7
	End

End

#############################################################

End