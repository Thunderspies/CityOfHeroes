#############################################################
## ResurectionHands.fx
#############################################################

FxInfo

LifeSpan 120

#############################################################

Condition
	On 	Time
	Time 	20

	Event
		EName	Left
		Type	Local
		At	wepL
		Part1	Powers/Ability/Stamina2.part
		Part2	Powers/Ability/StaminaSmall2.part
		Part3	Powers/Ability/AbilityDots.part
		Part4	Powers/Ability/AbilitySparklingDots.part
		Sound resurrect 70 70 .9
	End

	Event
		EName	Right
		Type	Local
		At	wepR
		Part1	Powers/Ability/Stamina2.part
		Part2	Powers/Ability/StaminaSmall2.part
		Part3	Powers/Ability/AbilityDots.part
		Part4	Powers/Ability/AbilitySparklingDots.part
	End
End

#############################################################

End