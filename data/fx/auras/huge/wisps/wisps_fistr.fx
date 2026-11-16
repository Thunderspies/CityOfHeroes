#############################################################
## Wisps_FistR.fx
#############################################################

FxInfo
Flags DontSuppress

#########################################################

Condition
	On	Cycle
	Time	20

	Event
		Type	Local
		At	HandR
		ChildFX	:CHILD_Wisps_FistR.fx
		Lifespan 60
	End
End

#############################################################

End