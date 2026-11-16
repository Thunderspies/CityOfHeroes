#############################################################
## AmmoContinuing_Cold.fx
#############################################################

FxInfo
#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	MuzzleFlashOffset_Left
		Type	Local
		At	WepR
		BhvrOverride
			PositionOffset		0.75 0 0.375
		End
#		Part1	CustomizeablePowers\IceControl\snowburst.part
#		Part1	CustomizeablePowers\IceControl\ColdArmMist.part
#		Part2	CustomizeablePowers\IceControl\FrostHands.part
	End

	Event
		EName	MuzzleFlashOffset_Right
		Type	Local
		At	WepL
		BhvrOverride
			PositionOffset		-0.75 0 0.375
		End
#		Part1	CustomizeablePowers\IceControl\snowburst.part
#		Part1	CustomizeablePowers\IceControl\ColdArmMist.part
#		Part2	CustomizeablePowers\IceControl\FrostHands.part
	End
End

#############################################################

End