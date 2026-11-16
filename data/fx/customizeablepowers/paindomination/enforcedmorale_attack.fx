#############################################################
## StrengthHands2.fx
#############################################################

FxInfo

LifeSpan 90

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		EName	Left
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\PainDomination\Stamina2.part
		Part2	CustomizeablePowers\PainDomination\StaminaSmall2.part
		Part3	CustomizeablePowers\PainDomination\AbilityDots.part
		Part4	CustomizeablePowers\PainDomination\AbilitySparklingDots.part
		Lifespan 45
	End
End
##################Sound#####################################
Condition

	On	Time
	Time	0

	Event
		EName	Sound
		Type	Local
		At	hips
		Sound Enforced_Morale_Cast_01 50 50 .85
	End

#############################################################

End