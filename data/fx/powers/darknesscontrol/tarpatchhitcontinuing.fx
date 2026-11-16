#########################################################
## Darkness Control - Tar Patch (Continuing FX)
#########################################################

FxInfo

Lifespan 900

ClampMaxScale 3.5 3.5 3.5

#########################################################

Input
	Inpname	Hips
End

#########################################################

## SOUND FX #######################################################

Condition
	On	Time
	Time	30

	Event
		Type	posit
		At	root
		Sound forcefield5_loop 70 70 .6
		bhvr	behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 420
	End
End

## GROUND FX #######################################################

Condition
	On 	Time
	Time 	30

	Event
		Type	posit
		At	root
		Part1	Powers\DarknessControl\CreateDarknessTrail.part
		Part2	Powers\DarknessControl\CreateDarknessTrail2.part
		Part3	Powers\DarknessControl\CreateDarknessPuddleFlat.part
		Part4	Powers\DarknessControl\CreateDarknessPuddle2Flat.part
		#Part5	Powers\DarknessControl\DarkTarRing.Part
	End
End

#########################################################

End