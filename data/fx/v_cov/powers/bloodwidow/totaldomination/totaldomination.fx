#############################################################
## Blood Widow "Telekinetic Blast" FX
#############################################################

FxInfo

Lifespan 80

#############################################################

Condition
	On 	Time
	Time 	20

	Event
		EName	core
		Type	start
		At	head
		Bhvr	Behaviors\MentalScale.bhvr
		Geom	LocalPivot01
		Sound mental6b 60 60 .71
		Magnet	T_Chest
		Lookat	T_Chest
	End
End

Condition
	On 	Time
	Time 	20

	Event
		EName	InsideCircle
		Type	Local
		At	core
		Altpiv	5
		Geom	InnerCircle
		bhvr	behaviors\2Pinwheelspin.bhvr
		Lifespan 30
	End

	Event
		EName	OutsideCircle
		Type	Local
		At	core
		Altpiv	5
		Geom	OutsideCircle
		bhvr	behaviors\2OppositePinWheelSpin.bhvr
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	20

	Event
		EName	lightning
		Type	start
		At	head
		part2	POWERS\MentalPowers\FreezeSnow.part
		part4	POWERS\MentalPowers\FreezeMist.part
		part5	POWERS\MentalPowers\Freezesteam.part
		Lifespan 45
	End
End

#############################################################

Condition
	On 	Time
	Time 	20

	Event
		EName	alt
		Type	Local
		At	core
		Altpiv	7
		part3	POWERS\MentalPowers\LargeExpandingMindControlRing.part
		Lifespan 40
	End
End

Condition
	On 	Time
	Time 	25

	Event
		EName	alt2
		Type	Local
		At	core
		Altpiv	6
		part1	POWERS\MentalPowers\MindControlRingsBurst2.part
		Lifespan 55
	End
End

Condition
	On 	Time
	Time 	50

	Event
		EName	alt3
		Type	Local
		At	core
		Altpiv	6
		part1	POWERS\MentalPowers\MindControlRingsBurst2.part
		Lifespan 30
	End
End

#############################################################

End