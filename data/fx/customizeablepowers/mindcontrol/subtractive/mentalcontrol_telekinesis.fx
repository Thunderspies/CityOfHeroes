#############################################################
## MentalControl_Telekinesis.fx
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
	End

	Event
		EName	core2
		Type	Local
		At	core
		Altpiv	4
		Part	:MindControlRingsSmall.part
		Part	:MindControlRingzSmall.part
		Sound telekinetic 80 80 0.8
		Lifespan 76
	End
End

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		EName	core3
		Type	Local
		At	core
		Altpiv	4
		Sound telekinetic3 80 80 0.8
		Part	:MindGlow2.part
		Lifespan 80
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	Core
		Sound pbexplosive 80 80 0.6
	End
End

#############################################################

End