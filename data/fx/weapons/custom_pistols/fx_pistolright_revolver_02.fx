#########################################################
## FX Geo - Right Pistol - 02
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
		Geom	GEO_WepR_Revolver_Magnum
	End
End

#########################################################

End