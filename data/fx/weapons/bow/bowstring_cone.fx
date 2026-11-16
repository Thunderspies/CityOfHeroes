#############################################################
## FX System Name
#############################################################

FxInfo

Flags InheritAnimScale InheritAlpha DontSuppress IsWeapon

Lifespan 57

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
		LifeSpan 10
	End
End

## DRAWN STRING ###########################################################

Condition
	On 	Time
	Time 	10

	Event
		EName	String_R
		Type	Posit
		At	C_LLegR
		Bhvr	behaviors/XRayBeam.bhvr
		geom	Bow_String
		LookAt	WepR
		LifeSpan 17
	End

	Event
		EName	String_L
		Type	Posit
		At	C_LLegL
		Bhvr	behaviors/XRayBeam.bhvr
		geom	Bow_String
		LookAt	  WepR
		LifeSpan 17
	End
End

## DEAD STRING - POST ###########################################################

Condition
	On 	Time
	Time 	27

	Event
		EName	String_dead
		Type	Posit
		At	C_LLegR
		Bhvr	behaviors/XRayBeam.bhvr
		geom	Bow_String
		LookAt	C_LLegL
		LifeSpan 30
	End
End

#############################################################

End