#########################################################
## Player Emote Howl - Sound FX
#########################################################

FxInfo

LifeSpan 15


Condition
	On	Time
	Time	15
	Event
		Type Local
		At origin
		Sound AnimalPack_SniffEmote 50 50 .1
	End

End