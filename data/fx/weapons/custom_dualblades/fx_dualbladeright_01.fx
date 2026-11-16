#########################################################
## FX Geo - Right Blade - 01
#########################################################

FxInfo

Flags InheritAlpha DontSuppress

#########################################################

Condition
	On 		Triggerbits
	Triggerbits	DUALHAND
	DoMany		1

	Event
		EName	WeaponDrawn
		While	DUALHAND
		Flags	OneAtATime
		Type	Local
		At	Origin
		Geom	GEO_WepR_Croatoa_Dagger
	End
End

#########################################################

End