#############################################################
## EnduranceHands.fx
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
		Part1	:Stamina2.part
		Part2	:StaminaSmall2.part
		Part3	:AbilityDots.part
		Part4	:AbilitySparklingDots.part
		sound 	Painbringer_01 50 30 .85
		Lifespan 45
	End

	Event
		EName	Right
		Type	Local
		At	WepR
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	:Stamina2.part
		Part2	:StaminaSmall2.part
		Part3	:AbilityDots.part
		Part4	:AbilitySparklingDots.part
		Lifespan 45
	End
End

#############################################################

End