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
		Part1	:DarkBlastTenticle.part
		Part3	:DarkHandsEmberb.part
		PMagnet	LarmL
	End

	Event
		Type	Local
		At	LarmL
		Part1	:DarkBlastTenticle.part
		Part3	:DarkHandsEmberb.part
		Sound miasma3 100 100 .8
		PMagnet	WepL
	End
End

#############################################################

End