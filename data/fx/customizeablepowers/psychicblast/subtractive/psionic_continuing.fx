#############################################################
## Psionic_Continuing.FX
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	Head
		part1	CustomizeablePowers\PsychicBlast\TealHitMist.part
		part2	CustomizeablePowers\PsychicBlast\PurpleInfuseRings.part
		Part4	CustomizeablePowers\PsychicBlast\QuestionMark.part
		Part5	CustomizeablePowers\PsychicBlast\NumberSign.part
		Part3	CustomizeablePowers\PsychicBlast\PercentSign.part
		Sound teal_loop 50 50 .4
	End

	Event
		Type	local
		At	Head
		Part1	CustomizeablePowers\PsychicBlast\PsionicBubblesHit.part
		Part2	CustomizeablePowers\PsychicBlast\HitRaysBig2.part
		Part4	CustomizeablePowers\PsychicBlast\HitStreaksBig2.part
		Part5	CustomizeablePowers\PsychicBlast\Asterick.part
	End
End

#############################################################

End