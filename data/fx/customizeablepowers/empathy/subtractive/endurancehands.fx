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
		Part1	:Intelligence.part
		Part2	:IntelligenceSmall.part
		Part3	:AbilityDots.part
		Part4	:AbilitySparklingDots.part
		sound 	heal4 50 30 .55
	End

	Event
		EName	Right
		Type	Local
		At	WepR
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	:Intelligence.part
		Part2	:IntelligenceSmall.part
		Part3	:AbilityDots.part
		Part4	:AbilitySparklingDots.part
	End
End

#############################################################

End