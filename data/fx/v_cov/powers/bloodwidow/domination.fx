#############################################################
## Blood Widow "Domination" FX
#############################################################

FxInfo

Lifespan 100

#############################################################

Condition
	On 	Time
	Time 	20

	Event
		EName	core
		Type	Local
		At	head
		Geom	LocalPivot01
		Sound Mental1 80 80 .8
	End

	Event
		EName	InsideCircle
		Type	Local
		At	core
		Altpiv	5
		Bhvr	behaviors/Pinwheelspin.bhvr
		Geom	InnerCircle
		Lifespan 27
	End

	Event
		EName	OutsideCircle
		Type	Local
		At	core
		Altpiv	5
		Bhvr	behaviors/OppositePinWheelSpin.bhvr
		Geom	OutsideCircle
		Lifespan 27
	End
End

Condition
	On 	Time
	Time 	20

	Event
		EName	alt
		Type	Local
		At	core
		Altpiv	7
		part3	POWERS\MentalPowers\LargeExpandingMindControlRing.part
		Lifespan 50
	End
End

Condition
	On 	Time
	Time 	20

	Event
		EName	alt1
		Type	Local
		At	core
		Altpiv	6
		part1	POWERS\MentalPowers\MindControlRingsBurst2.part
	End
End

Condition
	On 	Time
	Time 	47

	Event
		EName	alt2
		Type	Local
		At	core
		Altpiv	6
		part1	POWERS\MentalPowers\MindControlRingsBurst2.part
	End
End

#############################################################

End