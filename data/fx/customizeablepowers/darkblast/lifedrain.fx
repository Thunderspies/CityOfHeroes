#########################################################
##	chill touch hit
FxInfo

LifeSpan	100

#########################################################

Condition
	On 	Time
	Time 	5

	Event
		EName 	Hand3
		Type	Local
		At	UarmL
		Part1	CustomizeablePowers\DarkBlast\DarkBlastTenticle.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb_Alpha.part

		PMagnet	LarmL
		Lifespan 35
	End

	Event
		EName 	Hand4
		Type	Local
		At	LarmL
		Part1	CustomizeablePowers\DarkBlast\DarkBlastTenticle.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb_Alpha.part

		PMagnet	WepL
		Lifespan 35
	End
End

Condition
	On 	Time
	Time 	21

	Event
		EName 	HandGlow
		Type	Local
		At	WepL
		Part1	CustomizeablePowers\DarkBlast\DarkDrainGlow.part
		Lifespan 29
	End

End

#########################################################

End


