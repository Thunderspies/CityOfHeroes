#############################################################
## Confuse.fx
#############################################################

FxInfo

Lifespan 80

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		EName	core
		Type	Local
		At	head
		Geom	LocalPivot01
		Sound mental8 80 80 0.7
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
		Lifespan 20
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
		Lifespan 20
	End

	Event
		Type	Local
		At	core
		Altpiv	7
		Part	:LargeExpandingMindControlRing.part
		Lifespan 30
	End
End

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		Type	Start
		At	core
		Altpiv	7
		Part	:MindControlExplosion.part
		Part	:MindControlStarExplosion.part
		Part	:PowerPunchRingz.part
		Part	:PowerPunchRings.part
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	30

	Event
		Type	Start
		At	core
		Altpiv	7
		Part	:MindControlExplosion.part
		Part	:MindControlStarExplosion.part
		Part	:PowerPunchRingz.part
		Part	:PowerPunchRings.part
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	45

	Event
		Type	Start
		At	core
		Altpiv	7
		Part	:MindControlExplosion.part
		Part	:MindControlStarExplosion.part
		Part	:PowerPunchRingz.part
		Part	:PowerPunchRings.part
		Lifespan 30
		Lifespan 30
	End
End

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		EName	alt2
		Type	Start
		At	core
		Altpiv	6
		Part	:MindControlRingsBurst2.part
		Lifespan 35
	End
End

Condition
	On 	Time
	Time 	30

	Event
		EName	alt3
		Type	Start
		At	core
		Altpiv	6
		Part	:MindControlRingsBurst2.part
		Lifespan 35
	End
End

#############################################################

End