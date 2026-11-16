#############################################################
## ChillOfNightHitContnuing_subtractive.fx
#############################################################

FxInfo

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		ENAME   Tendrills
		Type	Local
		At	chest
		Part1	:ChillOfNightTendril.part
		Part2	:ChillOfNightTendril_Subtractive.part
		Sound miasma_loop 55 55 .55
	End

	Event
		EName 	Hit
		Type	local
		At	chest
		Part	:DefenceHitMist2_Subtractive.part
		part1	:DefenceHitMist.part
		part2	:DefenceHitMist2.part
	End
End

#############################################################

End