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
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\Empathy\Strength.part
		Part2	CustomizeablePowers\Empathy\StrengthSmall.part
		Part3	CustomizeablePowers\Empathy\AbilityDots.part
		Part4	CustomizeablePowers\Empathy\AbilitySparklingDots.part
		Sound radcharge 60 60 .65
	End

	Event
		EName	Right
		Type	Local
		At	WepL
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\Empathy\Strength.part
		Part2	CustomizeablePowers\Empathy\StrengthSmall.part
		Part3	CustomizeablePowers\Empathy\AbilityDots.part
		Part4	CustomizeablePowers\Empathy\AbilitySparklingDots.part
	End
End

#############################################################

End