#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 75

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local	#start
		At	WepR
		Part1	:ClawStreak.part
		Sound Clawspin 100 100 1.0
		Lifespan 38
	End
End

Condition
	On 	Time
	Time 	34

	Event
		Type	local	#start
		At	WepR
		part1	:ClawEmbers.part
		part2	:ClawEmbersBroad.part
		Lifespan 16
	End
End

Condition
	On 	Time
	Time 	35

	Event
		Type	start
		At	root
		Part5	:ClawsRing.part
		Lifespan 15
	End
End

#############################################################

End