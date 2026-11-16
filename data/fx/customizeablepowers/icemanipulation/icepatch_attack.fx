#############################################################
## IcePatch_Attack.fx
#############################################################

FxInfo

LifeSpan 110

#############################################################

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	WepL
		Part1	CustomizeablePowers\IceManipulation\snowburst.part
		Part1	CustomizeablePowers\IceManipulation\ColdArmMist.part
		Part2	CustomizeablePowers\IceManipulation\FrostHands.part
		LifeSpan 95
	End

	Event
		Type	Local
		At	WepR
		Part1	CustomizeablePowers\IceManipulation\snowburst.part
		Part1	CustomizeablePowers\IceManipulation\ColdArmMist.part
		Part2	CustomizeablePowers\IceManipulation\FrostHands.part
		LifeSpan 95
	End
End

#############################################################

End