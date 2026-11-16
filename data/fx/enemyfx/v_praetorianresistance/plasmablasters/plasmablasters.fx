#############################################################
## Header
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 		Triggerbits
	Triggerbits	DUALHAND
	DoMany		1

	Event
		EName	Pistol_Right
		While	DUALHAND
		Flags	OneAtATime
		Type	Local
		At	WepR
		Geom	GEO_WepR_PlasmaPistol_01
		Sound pistolout1 60 60 0.28
	End

	Event
		EName	Pistol_Left
		While	DUALHAND
		Flags	OneAtATime
		Type	Local
		At	WepL
		Bhvr	Behaviors\Reverse_wep.bhvr
		Geom	GEO_WepR_PlasmaPistol_01
	End
End

#############################################################

End