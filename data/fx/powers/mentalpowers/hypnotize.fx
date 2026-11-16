#############################################################
## Hypnotize.fx
#############################################################

FxInfo

LifeSpan 150

#############################################################

Condition
	On 	Time
	Time 	20

	Event
		EName	core
		Type	Local
		At	head
		Geom	LocalPivot01
		Sound mental3 80 80 0.8
	End

	Event
		EName	core2
		Type	Local
		At	core
		Altpiv	4
		part1	:MindControlRings.part
		part2	:MindControlRingz.part
		Lifespan 105
	End
End

Condition
	On 	Time
	Time 	30

	Event
		EName	core3
		Type	Local
		At	core
		Altpiv   4
		part2	:MindGlow.part
	End
End

#############################################################

End