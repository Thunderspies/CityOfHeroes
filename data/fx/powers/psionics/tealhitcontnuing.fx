#############################################################
## TealHitContnuing.fx
#############################################################

FxInfo

#########################################################

Input
	InpName	Hips
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event

		EName 	Hit
		Type	local
		At	head

		part1	:TealHitMist.part
		part2	:PurpleInfuseRings.part
		Part4	:QuestionMark.part
		Part5	:NumberSign.part
		Part3	:PercentSign.part
		Sound teal_loop 50 50 .4

	End

	Event
		EName	facerings2
		Type	local
		At	Head

		Part1	:PsionicBubblesHit.part
		Part2	:HitRaysBig2.part
		Part4	:HitStreaksBig2.part
		Part5	:Asterick.part
	End

End

End


