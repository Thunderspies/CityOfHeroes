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
		Part1	:CreateDarknessTrail.part
		Part2	:CreateDarknessTrail2.part
		Part3	:CreateDarknessPuddleFlat.part
		Part4	:CreateDarknessPuddle2Flat.part
	End
End

#########################################################

End