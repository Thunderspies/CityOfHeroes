#########################################################
## Darkness Control - Darkest Night
#########################################################

FxInfo

ClampMaxScale 1.0 1.0 1.0

Lifespan 15

## GROUND FX #######################################################

Condition
	On 	Time
	Time 	 0

	Event
		Type	Posit
		At	Origin
		geom	FX_RuneStreak
		bhvr	behaviors/Rune_spinFast.bhvr
		part1	:DarkessChillMist.part
		part2	:DarkessChillMist2.part
		part2	:DarkessChillMist3.part
	End

	Event
		Type	Posit
		At	Origin
		Part1	:CreateDarknessPuddle.part
		Part2	:CreateDarknessPuddle2.part
		Part1	:CreateDarknessTrail.part
		Part2	:CreateDarknessTrail2.part
		Part1	:CreateDarknessPuddleFlat.part
		Part2	:CreateDarknessPuddle2Flat.part
	End
End

#########################################################

End