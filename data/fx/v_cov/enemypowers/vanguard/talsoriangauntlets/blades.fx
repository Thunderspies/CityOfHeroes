#########################################################
## Talsorian Gauntlets - Monomolecular Blade (Male)
#########################################################

FxInfo

Flags    InheritAlpha

## SOUND FX #######################################################

Condition
	On Time 
	Time 0
	
	Event
		EName	Right
		Type 	Local
		At	WepR
		Sound Claw1d 100.0 100.0 .7
	End
End

## BLADES ##############/fxkill#########################################

Condition
	On Time 
	Time 0
	
	Event
		Ename	Left
		Type	Local
		At	LArmL
		BhvrOverride
			PositionOffset	-1.25 0.25 0
		End
		Part	:Blades_GlowL01.part
	End

	Event
		EName	Right
		Type 	Local
		At	LArmR
		BhvrOverride
			PositionOffset	1.25 0.25 0
		End
		Part	:Blades_GlowR01.part
	End
End

Condition
	On Time 
	Time 15

	Event
		Ename	Left
		Type	Local
		At	LArmL
		Geom	GEO_WepL_Vanguard_Blade
	End

	Event
		EName	Right
		Type 	Local
		At	LArmR
		Geom	GEO_WepR_Vanguard_Blade
	End
End

#########################################################

End	