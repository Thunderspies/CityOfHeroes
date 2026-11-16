#############################################################
## Command2.fx
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
		Sound Mental1 80 80 0.8

	End

	Event
		EName	InsideCircle
		Type	Local
		At	core
		Altpiv	5
		bhvr	:Pinwheelspin.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	tintable_Dark_InnerCircle
		Lifespan 27
	End

	Event
		EName	OutsideCircle
		Type	Local
		At	core
		Altpiv	5
		bhvr	:OppositePinWheelSpin.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	tintable_Dark_OutsideCircle

		Lifespan 27
	End

	Event
		EName	alt
		Type	Local
		At	core
		Altpiv	7
		Part	:LargeExpandingMindControlRing.part
		Lifespan 30
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

		Part	:MindControlRingsBurst2.part
		Lifespan 60
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
		Part	:MindControlRingsBurst2.part
		Lifespan 33
	End
End

#############################################################

End