#############################################################
## CobraStrike.fx
#############################################################

FxInfo
LifeSpan	100

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	LightShards
		Type	Local
		At	Chest
		part1	CustomizeablePowers\MartialArts\Subtractive\MartialArtsShard.part
		pmagnet	Chest
		Sound Cobra2 60 60 .9
		Lifespan 29
	End

End

Condition
	On	Time
	Time	5

	Event
		ENAME	Glow
		Type	Local
		At	Chest
		part1	CustomizeablePowers\MartialArts\Subtractive\MartialArtsChestGlow.part


	End

End

Condition
	On	Time
	Time	15

	Event
		ENAME	Ring
		Type	Local
		At	Chest
		part1	CustomizeablePowers\MartialArts\Subtractive\MartialArtsRing.part


	End

End

Condition
	On	Time
	Time	20

	Event
		ENAME	Fist
		Type	Local
		At	WepR
		part1	CustomizeablePowers\MartialArts\Subtractive\MartialArtsStar01.part
		part2	CustomizeablePowers\MartialArts\Subtractive\MartialArtsStar02.part
		part3	CustomizeablePowers\MartialArts\Subtractive\MartialArtsStarGlow.part

	End

End

Condition
	On	Time
	Time	18

	Event
		ENAME	FistBlur2
		Type	Local
		At	WepR
		part1	CustomizeablePowers\MartialArts\Subtractive\MAKickStreak02.part
		Lifespan 13
	End

End

Condition
	On	Time
	Time	25

	Event
		ENAME	FistFlash
		Type	Local
		At	WepR
		part1	CustomizeablePowers\MartialArts\Subtractive\MartialArtsStarBig.part

	End

End


End