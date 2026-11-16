#############################################################
## Fireflies_Body.fx
#############################################################

FxInfo
Flags DontSuppress

#########################################################

Condition
	On	Cycle
	Time	20
	Random	1

	Event
		Type	Local
		At	Origin
		ChildFX	:CHILD_Wisps.fx
		Lifespan 65
	End

	Event
		Type	Local
		At	Origin
		ChildFX	:CHILD_Wisps_CC.fx
		Lifespan 65
	End

End

#############################################################

End