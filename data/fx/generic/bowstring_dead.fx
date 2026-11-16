#############################################################
## FX System Name
#############################################################

FxInfo

Flags InheritAnimScale IsWeapon

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Posit
		At	LLegL
		Bhvr	behaviors/XRayBeam.bhvr
		geom	Bow_String
		LookAt	LLegR
	End
End

#############################################################

End		