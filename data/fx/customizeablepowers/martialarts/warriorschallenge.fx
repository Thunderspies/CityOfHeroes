#############################################################
## WarriorsChallenge.fx
#############################################################

FxInfo
LifeSpan	100

##################################

Condition
	On	Time
	Time	18

	Event
		ENAME	LightShards
		Type	Local
		At	Chest
		part1	CustomizeablePowers\MartialArts\MALightShardFaint.part
		pmagnet	Chest
		Lifespan 18
	End

End

Condition
	On	Time
	Time	15

	Event
		ENAME	Glow
		Type	Local
		At	Chest
		part1	CustomizeablePowers\MartialArts\MAWarrChalChestGlow01.part


	End

End

Condition
	On	Time
	Time	43

	Event
		ENAME	Ring
		Type	Local
		At	Chest
		#part1	:MAWarrChalRing01.part


	End

End

Condition
	On	Time
	Time	41

	Event
		ENAME	Rays
		Type	Local
		At	Chest
		#part1	:MartialArtsStarBig.part


	End

End


End