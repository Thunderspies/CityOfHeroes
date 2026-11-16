#########################################################
## FX Geo - Pulse Rifle - Base
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
		Geom	GEO_WepR_Mastermind_Robot_Rifle
	End
End

#########################################################

End