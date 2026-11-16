#############################################################
## ChillBlainsHandsA.fx
#############################################################

FxInfo

LifeSpan 50

#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part	:snowburst.part
		Sound Frost 100 100 0.8
		Lifespan 45
	End


	Event
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part	:snowburst.part
		Lifespan 45
	End
End

Condition
	On	Time
	Time	5

	Event
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part	:ColdArmMist.part
		Part	:FrostHands.part
		Lifespan 45
	End

	Event
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part	:ColdArmMist.part
		Part	:FrostHands.part
		Lifespan 45
	End
End

Condition
	On	Time
	Time	22

	Event
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part	:snowburstskatter2.part
		Part	:FreezingTouchRing2.part
		Sound Frost 100 100 0.8
		Lifespan 20
	End
End

#############################################################

End