#############################################################
## Wisps_Fists.fx
#############################################################

FxInfo
Flags DontSuppress

#########################################################

Condition
	On	Cycle
	Time	15
	Random	1

	Event
		Type	Local
		At	HandL
		ChildFX	:CHILD_Wisps_FistL.fx
		Lifespan 60
	End

	Event
		Type	Local
		At	HandR
		ChildFX	:CHILD_Wisps_FistR.fx
		Lifespan 60
	End
End

#############################################################

End