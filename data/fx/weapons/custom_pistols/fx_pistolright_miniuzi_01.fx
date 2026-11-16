#########################################################
## FX Geo - Right Pistol - 02
#########################################################

FxInfo

Flags InheritAlpha DontSuppress

#########################################################

Condition
	On 		Triggerbits
	Triggerbits	DUALHAND
	DoMany		1

	Event
		EName	WeaponDrawn1
		While	DUALHAND
		Flags	OneAtATime
		Type	Local
		At	Origin
		Geom	GEO_MicroUzi
	End

	Event
		EName	WeaponDrawn2
		While	DUALHAND
		Flags	OneAtATime
		Type	Local
		At	WeaponDrawn1
		AltPiv	02
		Geom	GEO_MicroUzi_Clip
	End
End

#########################################################

End