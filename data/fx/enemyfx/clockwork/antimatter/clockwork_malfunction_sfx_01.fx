#############################################################
## SelfDestruction_Windup.fx
#############################################################

FxInfo
lifespan 5

#############################################################

Condition
	On Time
	Time 0
	Event
		Type	Local
		At	Root
		sound Clockwork_Malfunction_01 65 65 .5
	End
End
