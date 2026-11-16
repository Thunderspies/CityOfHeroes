#############################################################
## MartialArtsHitsBig2.fx
#############################################################

FxInfo
Lifespan 10

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Mallet
		Type	start
		At	Chest

		Part	CustomizeablePowers\MartialArts\Subtractive\MartialArtsHitRaysLarge2.part

		Part	CustomizeablePowers\MartialArts\Subtractive\MartialArtsHit.part
		Part	CustomizeablePowers\MartialArts\Subtractive\MartialArtsHit1.part
		Part	CustomizeablePowers\MartialArts\Subtractive\MartialArtsHit2.part
		Part	CustomizeablePowers\MartialArts\Subtractive\MartialArtsHitRaysSmall.part
		Part	CustomizeablePowers\MartialArts\Subtractive\MAHitGlow01.part
		Sound PP13 60 60 .86
	End

End

#############################################################

End
