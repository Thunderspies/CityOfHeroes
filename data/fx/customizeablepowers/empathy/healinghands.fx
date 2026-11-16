#############################################################
## HealingHands.fx
#############################################################

FxInfo

Lifespan 55

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Left
		Type	Local
		At	WepL
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\Empathy\Stamina2.part
		Part2	CustomizeablePowers\Empathy\StaminaSmall2.part
		Part3	CustomizeablePowers\Empathy\AbilityDots.part
		Part4	CustomizeablePowers\Empathy\AbilitySparklingDots.part
		Sound heal4 70 70 0.55
	End

	Event
		EName	Right
		Type	Local
		At	WepR
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\Empathy\Stamina2.part
		Part2	CustomizeablePowers\Empathy\StaminaSmall2.part
		Part3	CustomizeablePowers\Empathy\AbilityDots.part
		Part4	CustomizeablePowers\Empathy\AbilitySparklingDots.part
	End
End

#############################################################

End