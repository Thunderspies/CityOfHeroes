#########################################################
## FX Geo - Katana 01
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
		Geom	GEO_WepL_Katana_3
	End
End

#########################################################

End