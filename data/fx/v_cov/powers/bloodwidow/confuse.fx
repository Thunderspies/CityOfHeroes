#############################################################
## Blood Widow "Confuse" FX
#############################################################

FxInfo

LifeSpan 100

#############################################################

Condition
	On 	Time
	Time 	20

	Event
		EName	core
		Type	Local
		At	head
		Geom	LocalPivot01
		Sound mental3 80 80 .8
		Lifespan 100
	End

	Event
		EName	core2
		Type	Local
		At	core
		Altpiv	4
		part1	POWERS\MentalPowers\MindControlRings.part
		part2	POWERS\MentalPowers\MindControlRingz.part
		Lifespan 45
	End
End

#############################################################

Condition
	On 	Time
	Time 	30

	Event
		EName	core3
		Type	Local
		At	core
		Altpiv	4
		part2	POWERS\MentalPowers\MindGlow.part
		Lifespan 50
	End
End

#############################################################

End