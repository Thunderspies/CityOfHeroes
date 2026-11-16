#############################################################
## HeadCastMentalPowers.fx
#############################################################

FxInfo

Lifespan 100

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	core
		Type	Local
		At	head
		Geom	LocalPivot01
		Sound mental9 80 80 0.8
	End


	Event
		EName	core2
		Type	Local
		At	core
		Altpiv	4
		Part	:MindControlRingsSmall.part
		Part	:MindControlRingzSmall.part
		Lifespan 60
	End
End

Condition
	On 	Time
	Time 	10

	Event
		EName	core3
		Type	Local
		At	core
		Altpiv	4
		Part	:MindGlow2.part
		Lifespan 50
	End
End

#############################################################

End