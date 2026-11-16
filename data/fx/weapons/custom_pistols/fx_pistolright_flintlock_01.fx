#########################################################
## FX Geo - Right Pistol - 01
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
		Type	Local
		At	Origin
		BhvrOverride
			Scale		0.875 0.875 0.875
		End
		Geom	GEO_WepR_Revolver_3
	End
End

#########################################################

End