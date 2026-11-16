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
		part1	:MartialArtsShard.part
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
		part1	:MartialArtsChestGlow.part


	End

End

Condition
	On	Time
	Time	7

	Event
		ENAME	Ring
		Type	Local
		At	Chest
		part1	:MartialArtsRing.part


	End

End

Condition
	On	Time
	Time	13

	Event
		ENAME	FootStars
		Type	Local
		At	FootL
		part1	:MartialArtsStar01.part
		part2	:MartialArtsStar02.part
		part3	:MartialArtsStarGlow.part


	End

End

Condition
	On	Time
	Time	18

	Event
		ENAME	FootFlash
		Type	Local
		At	FootL
		part1	:MartialArtsStarBig.part

	End

End

Condition
	On	Time
	Time	23

	Event
		ENAME	FootBlur
		Type	Local
		At	FootL
		part1	:MAKickStreak02.part
		Lifespan 7
	End

End

#############################################################

End