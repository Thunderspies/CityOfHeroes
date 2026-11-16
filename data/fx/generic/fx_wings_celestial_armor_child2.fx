#############################################################
## Header
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On		TRIGGERBITS
	TRIGGERBITS	FLY
	DoMany		1

	Event
		EName	NOBACK
		Type	Local
		At	Origin
		UNTIL	BACKWARD
		Flags	OneAtATime
		CHILDFX	:FX_Wings_Celestial_Armor_CHILD4.fx

	End
End

#############################################################

End