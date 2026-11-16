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
		Part1	CustomizeablePowers\StormSummoning\snowburst.part
		Part2	CustomizeablePowers\StormSummoning\ColdArmMist.part
		Sound chillhands 100 100 .8
	End

	Event
		Type	Local
		At	WepR
		Part2	CustomizeablePowers\StormSummoning\ColdArmMist.part
		Part1	CustomizeablePowers\StormSummoning\snowburst.part
	End
End

#############################################################

Condition
	On	Time
	Time	17

	Event
		Type	Local
		At	WepL
		Part1	CustomizeablePowers\StormSummoning\FreezingTouchRing.part
	End

	Event
		Type	Local
		At	WepR
		Part1	CustomizeablePowers\StormSummoning\FreezingTouchRing.part
	End
End

#############################################################

End