#########################################################
## FX Geo - Pulse Rifle - 02
#########################################################

FxInfo

Flags InheritAlpha DontSuppress

#########################################################

Condition
	On 		Triggerbits
	Triggerbits	GUN
	DoMany		1

	Event
		EName	WeaponDrawn
		While	GUN
		Flags	OneAtATime
		Type	Local
		At	Origin
		Geom	GEO_WepR_Shotgun_2
	End
End

#########################################################

End