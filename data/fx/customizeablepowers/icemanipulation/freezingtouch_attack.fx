#############################################################
## FreezingTouch_Attack.fx
#############################################################

FxInfo

LifeSpan 32

#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part1	CustomizeablePowers\IceManipulation\snowburst.part
		Sound chillingtouch2 80 80 .8
		LifeSpan 30
	End

	Event
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part1	CustomizeablePowers\IceManipulation\snowburst.part
		LifeSpan 30
	End
End

Condition
	On	Time
	Time	3.5

	Event
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part1	CustomizeablePowers\IceManipulation\ColdArmMist.part
		Part2	CustomizeablePowers\IceManipulation\FrostHands.part
		LifeSpan 30
	End

	Event
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part1	CustomizeablePowers\IceManipulation\ColdArmMist.part
		Part2	CustomizeablePowers\IceManipulation\FrostHands.part
		LifeSpan 30
	End
End

#############################################################

Condition
	On	Time
	Time	18

	Event
		Ename   LeftHandMist
		Type	start
		At	WepR
		Part1	CustomizeablePowers\IceManipulation\CrystalSnowBurst2.part
		Part2	CustomizeablePowers\IceManipulation\FreezingTouchRing.part
		LifeSpan 30
	End
End

#############################################################

End