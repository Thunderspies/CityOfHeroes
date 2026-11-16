#########################################################
## FX Geo - Broadsword 01
#########################################################

FxInfo

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
			Scale		0.75 0.75 0.75
		End
		Geom	GEO_WepR_Rikti_Sword_02
	End

	Event
		EName	WeaponDrawn2
		While	RIGHTHAND
		Flags	OneAtATime
		Type	Local
		At	Origin
		BhvrOverride
			Scale		0.75 0.75 0.75
		End
		Geom	RiktiBlade02
	End
End

#########################################################

End