#########################################################
## FX Geo - Pulse Rifle - 01
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
		Geom	GEO_WepR_Mastermind_Military_Rifle_Silencer
	End
End

#########################################################

End