#############################################################
## Mind Control - Mesmerize
#############################################################

FxInfo
Lifespan 105

#############################################################

Condition
	On 	Time
	Time 	17

	Event
		EName	core
		Type	Local
		At	Mystic
		Geom	LocalPivot01
		Sound mental4 80 80 0.75
	End
End

Condition
	On 	Time
	Time 	17

	Event
		EName	InsideCircle
		Type	Local
		At	core
		Altpiv	5
		bhvr	:Pinwheelspin.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	tintable_InnerCircle
		Lifespan 14
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
		Geom	tintable_OutsideCircle
		Lifespan 14
	End

	Event
		EName	alt
		Type	Local
		At	core
		Altpiv	6
		part3	:ExpandingMindControlRing.part
		Lifespan 20
	End

	Event
		EName	alt2
		Type	Local
		At	core
		Altpiv	6
		part1	:LargeMindControlRing.part
		Lifespan 20
	End
End

#############################################################

End