#############################################################
## WarriorsChallenge.fx
#############################################################

FxInfo
LifeSpan	100

#############################################################

Condition
	On	Time
	Time	18

	Event
		ENAME	LightShards
		Type	Local
		At	Chest
		part1	:MALightShardFaint.part
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
		part1	:MAWarrChalChestGlow01.part


	End

End

#############################################################

End