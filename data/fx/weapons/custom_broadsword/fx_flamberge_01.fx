#########################################################
## FX Geo - Broadsword 02
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
		Geom	GEO_WepR_Sword_5
	End
End

#########################################################

End