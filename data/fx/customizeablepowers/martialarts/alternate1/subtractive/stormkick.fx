#############################################################
## StormKick.fx
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
		part1	CustomizeablePowers\MartialArts\Subtractive\MartialArtsShard.part
		pmagnet	Chest
		Lifespan 29
		Sound Stormkick4 60 60 1.0
	End
End

Condition
	On	Time
	Time	3

	Event
		ENAME	Glow
		Type	Local
		At	Chest
		part1	CustomizeablePowers\MartialArts\Subtractive\MartialArtsChestGlow.part
	End
End

Condition
	On	Time
	Time	8

	Event
		ENAME	Ring
		Type	Local
		At	Chest
		part1	CustomizeablePowers\MartialArts\Subtractive\MartialArtsRing.part
	End
End

Condition
	On	Time
	Time	4

	Event
		ENAME	FootStars
		Type	Local
		At	WepR
		part1	CustomizeablePowers\MartialArts\Subtractive\MartialArtsCraneStar01.part
		part2	CustomizeablePowers\MartialArts\Subtractive\MartialArtsCraneStar02.part
		part3	CustomizeablePowers\MartialArts\Subtractive\MartialArtsCraneStarGlow.part
	End
End

Condition
	On	Time
	Time	11

	Event
		ENAME	FootFlash
		Type	Local
		At	WepR
		part1	CustomizeablePowers\MartialArts\Subtractive\MartialArtsStarBig.part
		Lifespan 32
	End
End

Condition
	On	Time
	Time	7

	Event
		ENAME	FootBlur
		Type	Local
		At	WepR
		part1	CustomizeablePowers\MartialArts\Subtractive\MAKickStreak02.part
		Lifespan 5
	End
End

Condition
	On	Time
	Time	11

	Event
		ENAME	FootBlur2
		Type	Local
		At	WepR
		part1	CustomizeablePowers\MartialArts\Subtractive\MAKickStreak02.part
		Lifespan 5
	End
End

#############################################################

End