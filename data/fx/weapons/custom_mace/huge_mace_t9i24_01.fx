#############################################################
## Costume Anim Entity - Tech Knight Mace
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 		Time
	Time 	0
	// Triggerbits	RIGHTHAND
	// DoMany		1

	Event
		// EName	MaceBase
		Type	Local
		At	WepR
		Bhvr	WEAPONS\Custom_Mace\Huge_WepRScale.bhvr
		BhvrOverride
			TintGeom 		1
		End
		// While	RIGHTHAND
		// Flags	OneAtATime
		Geom	FX_Mace_T9I24_01
	End

	Event
		// EName	MaceGems
		Type	Local
		At	WepR
		Bhvr	WEAPONS\Custom_Mace\Huge_WepRScale.bhvr
		BhvrOverride
			TintGeom 		1
		End
		// While	RIGHTHAND
		// Flags	OneAtATime
		Geom	FX_Mace_Gems_T9i24_01
	End

	Event
		EName	FXAnchor
		Type	Local
		At	WepR
		Bhvr	WEAPONS\Custom_Mace\Huge_WepRScale.bhvr
		// While	RIGHTHAND
		// Flags	OneAtATime
		CHILDFX 	WEAPONS\Custom_Mace\FX_TechKnightMace\CHILDFX_TechKnight_Mace.fx
	End

End

#############################################################

End
