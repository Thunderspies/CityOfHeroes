#############################################################
## Header
#############################################################

FxInfo

Flags InheritAnimScale InheritAlpha DontSuppress IsWeapon

LifeSpan 68

#############################################################

Condition

	On	Time
	Time	10

	Event
		Type	Local
		At	C_LLegR
		Sound arrowout2 80 80 .77
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	String_dead
		Type	Posit
		At	C_LLegR
		Bhvr	behaviors/XRayBeam.bhvr
		geom	Bow_String
		LookAt	C_LLegL
		LifeSpan 9
	End
End

#############################################################

Condition
	On 	Time
	Time 	9

	Event
		EName	String_R
		Type	Posit
		At	C_LLegR
		Bhvr	behaviors/XRayBeam.bhvr
		geom	Bow_String
		LookAt	WepR
		LifeSpan 39
	End

	Event
		EName	String_L
		Type	Posit
		At	C_LLegL
		Bhvr	behaviors/XRayBeam.bhvr
		geom	Bow_String
		LookAt	  WepR
		LifeSpan 39
	End
End

#############################################################

Condition
	On 	Time
	Time 	48

	Event
		EName	String_dead
		Type	Posit
		At	C_LLegR
		Bhvr	behaviors/XRayBeam.bhvr
		geom	Bow_String
		Sound bowtwang3 100.0 100.0 .7
		LookAt	C_LLegL
		LifeSpan 20
	End
End

#############################################################

End