#############################################################
## Encounter_CustomBow_Mid.fx
#############################################################

FxInfo

Flags InheritAnimScale InheritAlpha IsWeapon

LifeSpan 120

################# Pre Fake dead string #######################

Condition
	On 	Time
	Time 	0

	Event
		EName	String_dead
		Type	Posit
		At	C_LLegL
		Bhvr	behaviors\XRayBeam.bhvr
		geom	Bow_String
		LookAt	C_LLegR
		LifeSpan 19
	End
End

################## Pull String fx  ########################

Condition
	On 	Time
	Time 	20

	Event
		Type	Posit
		At	C_LLegL
		Bhvr	behaviors\XRayBeam.bhvr
		geom	Bow_String
		LookAt	WepR
		LifeSpan 21
	End

	Event
		Type	Posit
		At	C_LLegr
		Bhvr	behaviors\XRayBeam.bhvr
		geom	Bow_String
		LookAt	WepR
		LifeSpan 21
	End
End

################# Post Fake dead string #######################

Condition
	On 	Time
	Time 	41

	Event
		EName	String_dead
		Type	Posit
		At	C_LLegL
		Bhvr	behaviors/XRayBeam.bhvr
		geom	Bow_String
		LookAt	C_LLegR
	End
End

######## arrow in hand   ###########################

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	WepR
		Bhvr	WEAPONS\Bow\Archery\HandRotate.bhvr
		Geom	NormalArrow
		Sound arrowout2 50 50 0.25
		LifeSpan 31
	End
End

Condition
	On	Time
	Time	40

	Event
		Type	Local
		At	WepR
		Sound bowtwang3 54.0 54.0 0.5
	End
End

#############################################################

End