#########################################################
## FX Geo - Broadsword 01
#########################################################

FxInfo

Flags InheritAlpha DontSuppress

#########################################################

Condition
	On 		Triggerbits
	Triggerbits	RIGHTHAND
	DoMany		1

	Event
		EName	WeaponDrawn1
		While	RIGHTHAND
		Flags	OneAtATime
		Type	Local
		At	Origin
		BhvrOverride
			Scale		0.875 0.875 0.875
		End
		Geom	GEO_WepR_Rikti_Sword_01
	End

	Event
		EName	WeaponDrawn2
		While	RIGHTHAND
		Flags	OneAtATime
		Type	Local
		At	Origin
		BhvrOverride
			Scale		0.875 0.875 0.875
		End
		Geom	RiktiBlade01
	End
End

#########################################################

End