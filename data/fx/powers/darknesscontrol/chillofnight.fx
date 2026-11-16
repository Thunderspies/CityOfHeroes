#########################################################
## Darkness Control - Darkest Night
#########################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

Lifespan 15

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
		Type	posit
		At	T_root
		ChildFX	:ChillOfNight_AOE.fx
	End
End

#########################################################

End