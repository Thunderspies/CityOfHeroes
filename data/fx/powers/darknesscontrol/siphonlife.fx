#############################################################
## SiphonLife.fx
#############################################################

FxInfo

LifeSpan 100

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		EName 	Hand3
		Type	Local
		At	UarmL
		Part1	:DarkBlastTenticle.part
		Part3	:DarkHandsEmberb.part
		Sound Breathattack 70 70 .8
		PMagnet	LarmL
		Lifespan 60
	End

	Event
		EName 	Hand4
		Type	Local
		At	LarmL
		Part1	:DarkBlastTenticle.part
		Part3	:DarkHandsEmberb.part
		PMagnet	WepL
		Lifespan 60
	End
End

Condition
	On 	Time
	Time 	21

	Event
		EName 	HandGlow
		Type	Local
		At	WepL
		Part1	:DarkDrainGlow.part
		Lifespan 39
	End
End

#############################################################

End