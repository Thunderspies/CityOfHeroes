#############################################################
## Freeze.fx
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
		Bhvr	:MentalScale.bhvr
		Geom	LocalPivot01
		Magnet	T_Chest
		Lookat	T_Chest
		Sound mental6b 60 60 0.71
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

	Event
		Type	start
		At	head
		Part	:FreezeSnow.part
		Part	:FreezeMist.part
		Part	:Freezesteam.part
		Lifespan 45
	End
End

Condition
	On 	Time
	Time 	25

	Event
		Type	Local
		At	core
		Altpiv	6
		Part	:MindControlRingsBurst2.part
		Lifespan 35
	End
End

Condition
	On 	Time
	Time 	50

	Event
		Type	Local
		At	core
		Altpiv	6
		Part	:MindControlRingsBurst2.part
		Lifespan 35
	End
End

#############################################################

End