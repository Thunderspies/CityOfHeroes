#############################################################
## HEADER
#############################################################

FxInfo

LifeSpan 50

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	UarmL
		Part1	CustomizeablePowers\DarkMiasma\DarkBlastTenticle.part
		Part3	CustomizeablePowers\DarkMiasma\DarkHandsEmberb.part
		Part3	CustomizeablePowers\DarkMiasma\DarkHandsEmberb_Alpha.part

		PMagnet	LarmL
	End

	Event
		Type	Local
		At	LarmL
		Part1	CustomizeablePowers\DarkMiasma\DarkBlastTenticle.part
		Part3	CustomizeablePowers\DarkMiasma\DarkHandsEmberb.part
		Part3	CustomizeablePowers\DarkMiasma\DarkHandsEmberb_Alpha.part

		Sound miasma3 100 100 .8
		PMagnet	WepL
	End
End

#############################################################

End