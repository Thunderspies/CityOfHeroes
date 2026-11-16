#############################################################
## Paramilitary_BoostSerum.fx
#############################################################


FxInfo

Flags InheritAnimScale

Lifespan 300

#############################################################

Condition
	On 	Time
	Time 	15
	Event
		Type	Local
		At	origin
		Sound Mercenary_serum 60 60 .6
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	CatchTarget
		Type	Local
		At	T_Root

		bhvr	V_COV\Powers\Paramilitary\Paramilitary_B_BoostTarget.bhvr
	End
End

Condition
	On	Time
	Time	13

	Event
		EName	SerumAppear
		Type	Local
		At	WepL
		bhvr	V_COV\Powers\Paramilitary\Paramilitary_B_BoostSerumAppear.bhvr
		Geom	GEO_BoostSerum
	End
End

#############################################################

Condition
	On	Time
	Time	22

	Event
		EName	Prime
		Type	Start
		At	WepL

		bhvr	V_COV\Powers\Paramilitary\Paramilitary_B_BoostTravel.bhvr
		Magnet	CatchTarget
		LookAt	CatchTarget
	End
End

Condition
	On	Time
	Time	37

	Event
		EName	SerumAppear
		Type	Destroy
	End

	Event
		EName	Prime1
		Type	Start
		At	WepL

		bhvr	V_COV\Powers\Paramilitary\Paramilitary_B_BoostTravel.bhvr
		Geom	GEO_BoostSerum
		Magnet	CatchTarget
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		EName	Prime
		Type	Destroy
	End

End

Condition
	On	Prime1Hit

	Event
		EName	Prime1
		Type	Destroy
	End

End

#############################################################

End