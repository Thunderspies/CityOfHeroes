#############################################################
## Psionic_Continuing.FX
#############################################################

FxInfo

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	Head
		part1	:TealHitMist.part
		part2	:PurpleInfuseRings.part
		Part4	:QuestionMark.part
		Part5	:NumberSign.part
		Part3	:PercentSign.part
		Sound teal_loop 50 50 .4
	End

	Event
		Type	local
		At	Head
		Part1	:PsionicBubblesHit.part
		Part2	:HitRaysBig2.part
		Part4	:HitStreaksBig2.part
		Part5	:Asterick.part
	End
End

#############################################################

End