#############################################################
## StrengthHands2.fx
#############################################################

FxInfo

LifeSpan 90

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Left
		Type	Local
		At	WepR
		Part1	POWERS\Ability\Strength.part
		Part2	POWERS\Ability\StrengthSmall.part
		Part3	POWERS\Ability\AbilityDots.part
		Part4	POWERS\Ability\AbilitySparklingDots.part
		Sound radcharge 60 60 .65
	End

	Event
		EName	Right
		Type	Local
		At	WepL
		Part1	POWERS\Ability\Strength.part
		Part2	POWERS\Ability\StrengthSmall.part
		Part3	POWERS\Ability\AbilityDots.part
		Part4	POWERS\Ability\AbilitySparklingDots.part
	End
End

#############################################################

End