#############################################################
## Hypnotize.fx
#############################################################

FxInfo

Lifespan 150

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
		Part	:MindControlRings.part
		Part	:MindControlRingz.part
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
		Altpiv	4
		Part	:MindGlow.part
		Lifespan 120
	End
End

#############################################################

End