#########################################################
## Darkness Control - Darkest Night (Subtractive)
#########################################################

FxInfo

lifespan 15

#########################################################

Input
	Inpname	Hips
End

#########################################################

## SOUND FX #######################################################

#Condition
#
#	On	Time
#	Time	0
#
#	Event
#		Type 	Local
#		At	chest
#		Sound	miasma_loop 50.0 30.0 .4
#		bhvr	behaviors/soundFade3.bhvr
#		Flags	PowerLoopingSound
#		Lifespan 220
#	End
#End
#
#Condition
#
#	On	Time
#	Time	0
#
#	Event
#		Type 	Local
#		At	chest
#		Sound	miasma_loop 50.0 30.0 .4
#		bhvr	behaviors/soundFade3.bhvr
#		Flags	PowerLoopingSound
#		Lifespan 15
#	End
#End

## ANCHOR FX #######################################################

Condition
	On 	Time
	Time 	5

	Event
		EName 	rays
		Type	posit
		At	T_Chest
		Part1	:DarkestNightAnchorRays1.part
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
		bhvr	behaviors/Rune_spinFast.bhvr
		part1	:DarkessChillMist.part
		part2	:DarkessChillMist2.part
		part2	:DarkessChillMist3.part
	End

	Event
		EName	darkskirt
		Type	Posit
		At	T_root

		Part	:DarkBlastHitPuddle4_Subtractive.Part
		Part	:SubtractiveFlash1.part
		Part	:CreateDarknessPuddle_Subtractive_Local.part
		Part1	:CreateDarknessPuddle_Subtractive.part
		Part2	:CreateDarknessPuddle2_Subtractive.part
		Part1	:CreateDarknessTrail_Subtractive.part
		Part2	:CreateDarknessTrail2_Subtractive.part
		Part1	:CreateDarknessPuddleFlat_Subtractive.part
		Part2	:CreateDarknessPuddle2Flat_Subtractive.part
	End
End

#########################################################

End