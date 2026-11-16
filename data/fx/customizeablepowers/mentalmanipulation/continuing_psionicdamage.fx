#############################################################
## Continuing_PsionicDamage.FX
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	head
		part1	CustomizeablePowers\MentalManipulation\TealHitMist.part
		part2	CustomizeablePowers\MentalManipulation\PurpleInfuseRings.part
		Part4	CustomizeablePowers\MentalManipulation\QuestionMark.part
		Part5	CustomizeablePowers\MentalManipulation\NumberSign.part
		Part3	CustomizeablePowers\MentalManipulation\PercentSign.part
		Sound teal_loop 50 50 .4
	End

	Event
		Type	local
		At	Head
		Part1	CustomizeablePowers\MentalManipulation\PsionicBubblesHit.part
		Part2	CustomizeablePowers\MentalManipulation\HitRaysBig2.part
		Part4	CustomizeablePowers\MentalManipulation\HitStreaksBig2.part
		Part5	CustomizeablePowers\MentalManipulation\Asterick.part
	End
End

#############################################################

End