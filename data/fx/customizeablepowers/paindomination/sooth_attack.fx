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
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\PainDomination\Stamina2.part
		Part2	CustomizeablePowers\PainDomination\StaminaSmall2.part
		Part3	CustomizeablePowers\PainDomination\AbilityDots.part
		Part4	CustomizeablePowers\PainDomination\AbilitySparklingDots.part
		Sound Soothe_01 70 70 .75
		Lifespan 45
	End

	Event
		EName	Right
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\PainDomination\Stamina2.part
		Part2	CustomizeablePowers\PainDomination\StaminaSmall2.part
		Part3	CustomizeablePowers\PainDomination\AbilityDots.part
		Part4	CustomizeablePowers\PainDomination\AbilitySparklingDots.part
		Lifespan 45
	End
End

#############################################################

End