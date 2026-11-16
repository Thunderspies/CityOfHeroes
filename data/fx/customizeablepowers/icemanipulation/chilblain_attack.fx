#############################################################
## ChilBlain_Attack.fx
#############################################################

FxInfo

LifeSpan 50

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	WepL
		Part1	:snowburst.part
		Sound Frost 100 100 0.8
		LifeSpan 45
	End

	Event
		Type	Local
		At	WepR
		Part1	:snowburst.part
		LifeSpan 45
	End
End

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	WepL
		Part1	:ColdArmMist.part
		LifeSpan 45
	End

	Event
		Type	Local
		At	WepR
		Part1	:ColdArmMist.part
		LifeSpan 45
	End

	Event
		Type	Local
		At	WepL
		Part2	:FrostHands.part
		LifeSpan 47
	End

	Event
		Type	Local
		At	WepR
		Part2	:FrostHands.part
		LifeSpan 47
	End
End

Condition
	On	Time
	Time	37

	Event
		Type	Local
		At	WepL
		Part1	:snowburstskatter2.part
		Part2	:FreezingTouchRing.part
		Sound Frost 100 100 .8
		LifeSpan 25
	End
End

#############################################################

End