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
		EName	NORIGHT
		Type	Local
		At	Origin
		UNTIL	STEPRIGHT
		Flags	OneAtATime
		CHILDFX	:FX_Wings_Celestial_Armor_CHILD2.fx
	End
End

#############################################################

End