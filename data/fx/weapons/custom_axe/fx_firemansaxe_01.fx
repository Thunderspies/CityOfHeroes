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
		Geom	GEO_WepR_FiremansAxe_01
	End
End

#########################################################

End