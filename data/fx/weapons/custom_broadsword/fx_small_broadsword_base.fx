#########################################################
## FX Geo - Broadsword Base
#########################################################

FxInfo

Flags InheritAlpha DontSuppress

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
		Bhvr	WEAPONS\SmallWeaponScale.bhvr
		Geom	GEO_WepR_Sword_1
	End
End

#########################################################

End