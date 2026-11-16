#############################################################
## FX System Name
#############################################################

FxInfo

Flags InheritAnimScale InheritAlpha DontSuppress IsWeapon

## DEAD STRING ###########################################################

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