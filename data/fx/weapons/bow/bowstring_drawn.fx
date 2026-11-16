#############################################################
## FX System Name
#############################################################

FxInfo

Flags InheritAnimScale InheritAlpha DontSuppress IsWeapon


## DEAD STRING - PRE ###########################################################

Condition
	On 	Time
	Time 	0

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
		LookAt	  WepR
	End
End

#############################################################

End