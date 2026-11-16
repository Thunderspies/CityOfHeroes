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
		Part1	:Stamina2.part
		Part2	:StaminaSmall2.part
		Part3	:AbilityDots.part
		Part4	:AbilitySparklingDots.part
		Sound resurrect 70 70 .9
	End

	Event
		EName	Right
		Type	Local
		At	wepR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part1	:Stamina2.part
		Part2	:StaminaSmall2.part
		Part3	:AbilityDots.part
		Part4	:AbilitySparklingDots.part
	End
End

#############################################################

End