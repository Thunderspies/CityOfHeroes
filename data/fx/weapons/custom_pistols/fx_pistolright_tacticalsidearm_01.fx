#########################################################
## FX Geo - Right Pistol - 03
#########################################################

FxInfo

Flags InheritAlpha DontSuppress

#########################################################

Condition
	On 		Triggerbits
	Triggerbits	DUALHAND
	DoMany		1

	Event
		EName	WeaponDrawn
		While	DUALHAND
		Flags	OneAtATime
		Bhvr	Behaviors/InheritGroupTint.bhvr
		Type	Local
		At	Origin
		Geom	GEO_WepR_Pistol_Tactical
	End
End

#########################################################

End