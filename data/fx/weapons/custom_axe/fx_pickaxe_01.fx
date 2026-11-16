#########################################################
## FX Geo - Pickaxe 01
#########################################################

FxInfo

#########################################################

Condition
	On 		Triggerbits
	Triggerbits	RIGHTHAND
	DoMany		1

	Event
		EName	WeaponDrawn
		While	RIGHTHAND
		Flags	OneAtATime
		Type	Local
		At	Origin
		Geom	GEO_WepL_Pick_Axe_02
	End
End

#########################################################

End