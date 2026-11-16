#########################################################
## FX Geo - Katana Base
#########################################################

FxInfo

Flags InheritAlpha DontSuppress

#########################################################

Condition
	On 		Triggerbits
	Triggerbits	LEFTHAND
	DoMany		1

	Event
		EName	WeaponDrawn
		While	LEFTHAND
		Flags	OneAtATime
		Type	Local
		At	Origin
		Geom	GEO_WepL_Katana_2
	End
End

#########################################################

End