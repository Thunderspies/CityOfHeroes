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
		Geom	GEO_WepR_Rikti_Sword_02
	End

	Event
		EName	WeaponDrawn2
		While	RIGHTHAND
		Flags	OneAtATime
		Type	Local
		At	Origin
		Geom	RiktiBlade02
	End
End

#########################################################

End