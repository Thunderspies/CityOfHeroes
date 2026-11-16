#############################################################
## Hit_PsionicDamage.fx
#############################################################

FxInfo
Lifespan 30

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	Head
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\MentalManipulation\InfuseLight2.part
		Part2	CustomizeablePowers\MentalManipulation\Infuseglow.part
		Part3	CustomizeablePowers\MentalManipulation\InfuseRings.part
		Part3	CustomizeablePowers\MentalManipulation\Defuseglow.part
		Part2	CustomizeablePowers\MentalManipulation\HitRaysBig.part
		Part4	CustomizeablePowers\MentalManipulation\HitStreaksBig.part
		Sound Infuse4 80 80 .6

	End

	Event
		Type	local
		At	Head
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\MentalManipulation\PsionicBubblesHit.part
		Part4	CustomizeablePowers\MentalManipulation\PercentSign.part
		Part5	CustomizeablePowers\MentalManipulation\Asterick.part
		Part4	CustomizeablePowers\MentalManipulation\QuestionMark.part
		Part5	CustomizeablePowers\MentalManipulation\NumberSign.part
		Lifespan 25
	End
End

#############################################################

End