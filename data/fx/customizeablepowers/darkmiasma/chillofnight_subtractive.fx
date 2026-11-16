#########################################################
## Darkness Control - Darkest Night (Subtractive)
#########################################################

FxInfo
Lifespan 15

#########################################################

Input
	Inpname	Hips
End

#########################################################

## ANCHOR FX #######################################################

Condition
	On 	Time
	Time 	5

	Event
		EName 	rays
		Type	posit
		At	T_Chest
		Part1	CustomizeablePowers\DarkMiasma\DarkestNightAnchorRays1.part
	End
End

## GROUND FX #######################################################

Condition
	On 	Time
	Time 	 0

	Event
		EName 	OrbMist
		Type	posit
		At	T_root
		geom	FX_RuneStreak
		bhvr	CustomizeablePowers\DarkMiasma\Rune_spinFast.bhvr
		part	CustomizeablePowers\DarkMiasma\DarkessChillMist.part
		part	CustomizeablePowers\DarkMiasma\DarkessChillMist2.part
		part	CustomizeablePowers\DarkMiasma\DarkessChillMist3.part
	End

	Event
		EName	darkskirt
		Type	Posit
		At	T_root

		Part	CustomizeablePowers\DarkMiasma\DarkBlastHitPuddle4_Subtractive.Part
		Part	CustomizeablePowers\DarkMiasma\SubtractiveFlash1.part
		Part	CustomizeablePowers\DarkMiasma\CreateDarknessPuddle_Subtractive_Local.part
		Part	CustomizeablePowers\DarkMiasma\CreateDarknessPuddle_Subtractive.part
		Part	CustomizeablePowers\DarkMiasma\CreateDarknessPuddle2_Subtractive.part
		Part	CustomizeablePowers\DarkMiasma\CreateDarknessTrail_Subtractive.part
		Part	CustomizeablePowers\DarkMiasma\CreateDarknessTrail2_Subtractive.part
		Part	CustomizeablePowers\DarkMiasma\CreateDarknessPuddleFlat_Subtractive.part
		Part	CustomizeablePowers\DarkMiasma\CreateDarknessPuddle2Flat_Subtractive.part
	End
End

#########################################################

End