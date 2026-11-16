#############################################################
## Terrify.fx
#############################################################

FxInfo

Lifespan 50

#############################################################

Condition
	On	Time
	Time	20

	Event
		EName	MysticAnchor
		Type	Local
		At	Head
		Geom	PsionicRigging
		BhvrOverride
			PositionOffset	0.0 0.0 1.5
		End
		Sound mental7c 80 80 0.8
	End

	Event
		EName	Core
		Type	Start
		At	MysticAnchor
		Geom	LocalPivot01
		Sound mental7c 80 80 0.8
	End

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
	Time 	40

	Event
		Type	Local
		At	Core
		Part	:TerrifyElectricity.part
		Part	:TerrifyElectricity2.part
		Lifespan 30
	End
End

#############################################################

Condition
	On 	Time
	Time 	20

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
	Time 	35

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