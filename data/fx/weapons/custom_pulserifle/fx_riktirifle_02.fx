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
		BhvrOverride
			Scale		0.625 0.625 0.625
		End
		Geom	GEO_WepR_Rikti_Gun_05
	End
End

#########################################################

End