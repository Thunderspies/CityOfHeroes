#############################################################
## ColdHandsBoth.fx
#############################################################

FxInfo

Lifespan 60

#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part	POWERS\ColdControl\snowburst.part
		Sound chillhands 100 100 0.8
		Lifespan 60
	End

	Event
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part	POWERS\ColdControl\snowburst.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	3.5

	Event
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part	POWERS\ColdControl\ColdArmMist.part
		Lifespan 60
	End

	Event
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part	POWERS\ColdControl\ColdArmMist.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	17

	Event
		Type	Local
		At	WepR
		Sound Blizzard 100 100 0.79
	End
End

#############################################################

End