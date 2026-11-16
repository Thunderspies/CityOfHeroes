#############################################################
## IceSlickHands.fx
#############################################################

FxInfo

Lifespan 110

#############################################################

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	WepL
		Part	:snowburst.part
		Lifespan 95
	End

	Event
		Type	Local
		At	WepR
		Part	:snowburst.part
		Lifespan 95
	End
End

Condition
	On	Time
	Time	9

	Event
		Type	Local
		At	WepL
		Part	:ColdArmMist.part
		Part	:FrostHands.part
		Lifespan 95
	End

	Event
		Type	Local
		At	WepR
		Part	:ColdArmMist.part
		Part	:FrostHands.part
		Lifespan 95
	End
End

#############################################################

End