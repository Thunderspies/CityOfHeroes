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
		Part1	:snowburst.part
		Part2	:ColdArmMist.part
		Sound chillhands 100 100 0.8
	End

	Event
		Type	Local
		At	WepR
		Part2	:ColdArmMist.part
		Part1	:snowburst.part
	End
End

#############################################################

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	WepL
		Part1	:FreezingTouchRing.part
	End

	Event
		Type	Local
		At	WepR
		Part1	:FreezingTouchRing.part
	End
End

#############################################################

End