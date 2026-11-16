#############################################################
## Terrify.fx
#############################################################

FxInfo

Lifespan 80

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	core
		Type	Local
		At	head
		Geom	LocalPivot01
		Sound mental7c 80 80 0.8
	End
End

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	core
		Altpiv	5
		bhvr	:2Pinwheelspin.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_InnerCircle
		Lifespan 30
	End

	Event
		Type	Local
		At	core
		Altpiv	5
		bhvr	:2OppositePinWheelSpin.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_OutsideCircle
		Lifespan 30
	End

	Event
		Type	Local
		At	core
		Altpiv	7
		Part	:LargeExpandingMindControlRing.part
		Lifespan 40
	End
End

#############################################################

Condition
	On 	Time
	Time 	25

	Event
		Type	Local
		At	head
		Part	:TerrifyElectricity.part
		Part	:TerrifyElectricity2.part
		Lifespan 30
	End
End

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		EName	alt1
		Type	Local
		At	core
		Altpiv	6
		Part	:MindControlRingsBurst2.part
	End
End

Condition
	On 	Time
	Time 	20

	Event
		EName	alt2
		Type	Local
		At	core
		Altpiv	6
		Part	:MindControlRingsBurst2.part
	End
End

#############################################################

End