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
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part1	CustomizeablePowers/Empathy/Stamina2.part
		Part2	CustomizeablePowers/Empathy/StaminaSmall2.part
		Part3	CustomizeablePowers/Empathy/AbilityDots.part
		Part4	CustomizeablePowers/Empathy/AbilitySparklingDots.part
		Sound resurrect 70 70 .9
	End

	Event
		EName	Right
		Type	Local
		At	wepR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part1	CustomizeablePowers/Empathy/Stamina2.part
		Part2	CustomizeablePowers/Empathy/StaminaSmall2.part
		Part3	CustomizeablePowers/Empathy/AbilityDots.part
		Part4	CustomizeablePowers/Empathy/AbilitySparklingDots.part
	End
End

#############################################################

End