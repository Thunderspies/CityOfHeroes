#########################################################
## FX Geo - Right Blade - 09
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
		Geom	GEO_WepR_Sword_07
	End
End

#########################################################

End