#############################################################
## ChillingTouchHands.fx
#############################################################

FxInfo

Lifespan 32

#############################################################

Condition
	On	Time
	Time	0


	Event
		Type	Local
		At	WepL
		Part	:snowburst.part
		Sound chillingtouch2 80 80 0.8
		Lifespan 30
	End

	Event
		Type	Local
		At	WepR
		Part	:snowburst.part
		Lifespan 30
	End
End

Condition
	On	Time
	Time	4


	Event
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part	:ColdArmMist.part
		Part	:FrostHands.part
		Lifespan 30
	End

	Event
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part	:ColdArmMist.part
		Part	:FrostHands.part
		Lifespan 30
	End
End

Condition
	On	Time
	Time	18


	Event
		Ename   LeftHandMist
		Type	start
		At	WepR
		Part	:CrystalSnowBurst2.part
		Part	:FreezingTouchRing.part
		Lifespan 30
	End
End

#############################################################

End