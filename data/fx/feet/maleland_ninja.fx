#############################################################
## SmokeBomb.fx
#############################################################

FxInfo
Lifespan 30

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	StartPositOnly
		At	Root
		Part	Emotes\CostumeChanges\NinjaJump\SmokePillar_Linger.part
		Part	Emotes\CostumeChanges\NinjaJump\SmokePillar_Linger_Long.part

		Lifespan 2
	End
End

#############################################################

