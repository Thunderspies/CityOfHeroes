#########################################################
## Snow Storm Fx
#########################################################

FxInfo
ClampMaxScale 3.5 3.5 3.5
Lifespan 30

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	T_Root
		ChildFX	:SSSnowStorm_AOE.fx
	End
End

## ANCHOR FX #######################################################

Condition
	On 	Time
	Time 	5

	Event
		EName 	rays
		Type	posit
		At	T_Chest
		Part1	:SnowStormAnchorRays1.part
	End
End

#############################################################

End