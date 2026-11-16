#########################################################
## FX Geo - Katana 02
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
		Geom	GEO_WepL_Pick_Axe_02
	End
End

#########################################################

End