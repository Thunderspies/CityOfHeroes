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
		part1	CustomizeablePowers\MartialArts\MartialArtsShard.part
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
		part1	CustomizeablePowers\MartialArts\MartialArtsChestGlow.part
	End
End

Condition
	On	Time
	Time	4

	Event
		ENAME	Ring
		Type	Local
		At	Chest
		part1	CustomizeablePowers\MartialArts\MartialArtsRing.part
	End
End

Condition
	On	Time
	Time	4

	Event
		ENAME	FootStars
		Type	Local
		At	FootR
		part1	CustomizeablePowers\MartialArts\MartialArtsCraneStar01.part
		part2	CustomizeablePowers\MartialArts\MartialArtsCraneStar02.part
		part3	CustomizeablePowers\MartialArts\MartialArtsCraneStarGlow.part
	End
End

Condition
	On	Time
	Time	7

	Event
		ENAME	FootFlash
		Type	Local
		At	FootR
		part1	CustomizeablePowers\MartialArts\MartialArtsStarBig.part
		Lifespan 32
	End
End

Condition
	On	Time
	Time	7

	Event
		ENAME	FootBlur
		Type	Local
		At	FootR
		part1	CustomizeablePowers\MartialArts\MAKickStreak02.part
		Lifespan 5
	End
End

Condition
	On	Time
	Time	11

	Event
		ENAME	FootBlur2
		Type	Local
		At	FootR
		part1	CustomizeablePowers\MartialArts\MAKickStreak02.part
		Lifespan 5
	End
End

#############################################################

End