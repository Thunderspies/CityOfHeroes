#############################################################
## ChillingHands.fx
#############################################################

FxInfo

Lifespan 60

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	WepL
		Part1	POWERS\ColdControl\snowburst.part
		Part2	POWERS\ColdControl\ColdArmMist.part
		Sound chillhands 100 100 .8
	End

	Event
		Type	Local
		At	WepR
		Part2	POWERS\ColdControl\ColdArmMist.part
		Part1	POWERS\ColdControl\snowburst.part
	End
End

#############################################################

Condition
	On	Time
	Time	17

	Event
		Type	Local
		At	WepL
		Part1	POWERS\ColdControl\FreezingTouchRing.part
	End

	Event
		Type	Local
		At	WepR
		Part1	POWERS\ColdControl\FreezingTouchRing.part
	End
End

#############################################################

End