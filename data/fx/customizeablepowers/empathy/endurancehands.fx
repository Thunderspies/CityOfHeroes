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
		EName	left
		Type	Local
		At	WepL
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\Empathy\Intelligence.part
		Part2	CustomizeablePowers\Empathy\IntelligenceSmall.part
		Part3	CustomizeablePowers\Empathy\AbilityDots.part
		Part4	CustomizeablePowers\Empathy\AbilitySparklingDots.part
		sound 	heal4 50 30 .55
	End

	Event
		EName	Right
		Type	Local
		At	WepR
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\Empathy\Intelligence.part
		Part2	CustomizeablePowers\Empathy\IntelligenceSmall.part
		Part3	CustomizeablePowers\Empathy\AbilityDots.part
		Part4	CustomizeablePowers\Empathy\AbilitySparklingDots.part
	End
End

#############################################################

End