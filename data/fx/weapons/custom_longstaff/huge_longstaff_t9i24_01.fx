#############################################################
## Costume Anim Entity - Tech Knight Staff
#############################################################

FxInfo

	Flags InheritAlpha InheritAnimScale DontSuppress

#############################################################

Condition
	On 		Time
	Time 	0
	// Triggerbits	COMBAT WEAPON 2HLARGE	## In case Staff alone doesn't quite work, use these additional triggerbits: COMBAT WEAPON 2HLARGE
#	Triggerbits	STAFF
## removed the above line since it seemed to cause the Staff to disappear during combat in-game.
	// DoMany		1

	Event
		// EName	StaffBase
		Type	Local
		At	WepL
		Bhvr	WEAPONS\Custom_TitanWeapon\Huge_WepLScale.bhvr
		Anim	CustomWeapon_Staff_T9I24_01
		BhvrOverride
			TintGeom		1
			PositionOffset		-0.1 -0.05 0
		End
		// While	COMBAT WEAPON 2HLARGE
		// Flags	OneAtATime
		Geom	FX_Staff_T9i24_01
	End

	Event
		// EName	StaffGems
		Type	Local
		At	WepL
		Bhvr	WEAPONS\Custom_TitanWeapon\Huge_WepLScale.bhvr
		BhvrOverride
			TintGeom 		1
			PositionOffset		-0.1 -0.05 0
#			PyrRotate		10 0 175
		End
		// While	COMBAT WEAPON 2HLARGE
		// Flags	OneAtATime
		Geom	FX_Staff_Gems_T9i24_01
	End

	Event
		EName	FXAnchor
		Type	Local
		At	WepR
		// While	COMBAT WEAPON 2HLARGE
		// Flags	OneAtATime
		CHILDFX 	WEAPONS\Custom_Mace\FX_TechKnightMace\CHILDFX_TechKnight_Mace.fx
	End

End

#############################################################

End