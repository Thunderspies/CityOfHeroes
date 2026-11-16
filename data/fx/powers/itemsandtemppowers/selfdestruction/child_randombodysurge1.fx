#############################################################
## Child_RandomBodySurge1.fx
#############################################################

FxInfo

Flags DontSuppress

#############################################################

Condition
	On	Cycle
	Time	25
	Chance	0.25

	Event
		Type	Local
		At	Chest
		ChildFX	:Child_ElectricalBodySurge1.fx
		Lifespan 30
	End
End

#############################################################

End