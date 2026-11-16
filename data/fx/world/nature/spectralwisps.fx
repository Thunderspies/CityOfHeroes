#############################################################################
##Head dizzy
########################################################
FxInfo

Condition
	On 	cycle
	Time 	50
	Chance	1	#.4

	Event
		EName	core
		Type	Local
		At	head
		Part1	:WispTail.part
		Part2	:WispTail2.part
		Lifespan 140
	End

End

End
