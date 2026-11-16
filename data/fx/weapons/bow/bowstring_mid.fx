#############################################################
## FX System Name
#############################################################

FxInfo

Flags InheritAnimScale InheritAlpha DontSuppress IsWeapon

Lifespan 33

## DEAD STRING - PRE ###########################################################

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
		LifeSpan 4
	End
End

### DRAWN STRING ###########################################################

Condition
	On 	Time
	Time 	4

	Event
		EName	String_R
		Type	Posit
		At	C_LLegR
		Bhvr	behaviors/XRayBeam.bhvr
		geom	Bow_String
		LookAt	WepR
		LifeSpan 19
	End

	Event
		EName	String_L
		Type	Posit
		At	C_LLegL
		Bhvr	behaviors/XRayBeam.bhvr
		geom	Bow_String
		LookAt	  WepR
		LifeSpan 19
	End
End

### DEAD STRING - POST ###########################################################

Condition
	On 	Time
	Time 	23

	Event
		EName	String_dead
		Type	Posit
		At	C_LLegR
		Bhvr	behaviors/XRayBeam.bhvr
		geom	Bow_String
		LookAt	C_LLegL
		Sound bowtwang3 100.0 100.0 .7
		LifeSpan 10
	End
End

#############################################################

End