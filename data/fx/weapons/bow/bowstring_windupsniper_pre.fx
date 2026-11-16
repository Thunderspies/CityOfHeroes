#############################################################
## FX System Name
#############################################################

FxInfo

Flags InheritAnimScale InheritAlpha DontSuppress IsWeapon

Lifespan 300

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
		LifeSpan 20
	End
End

## DRAWN STRING ###########################################################

Condition
	On 	Time
	Time 	20

	Event
		EName	String_R
		Type	Posit
		At	C_LLegR
		Bhvr	behaviors/XRayBeam.bhvr
		geom	Bow_String
		LookAt	WepR
	End

	Event
		EName	String_L
		Type	Posit
		At	C_LLegL
		Bhvr	behaviors/XRayBeam.bhvr
		geom	Bow_String
		LookAt	WepR
	End
End

#############################################################

Condition
	On 	Death

	Event
		EName	ALL
		Type	Destroy
	End
End

#############################################################

End