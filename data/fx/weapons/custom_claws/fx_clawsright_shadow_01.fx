#########################################################
## FX Geo - Right Claws - Base
#########################################################

FxInfo

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
		Geom	GEO_WepR_Claw_02
	End
End

#########################################################

End