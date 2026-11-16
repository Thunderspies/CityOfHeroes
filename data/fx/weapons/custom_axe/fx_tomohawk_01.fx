#########################################################
## FX Geo - Axe 02
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
		Geom	GEO_WepR_Axe_3
	End
End

#########################################################

End