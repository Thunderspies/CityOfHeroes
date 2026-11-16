#############################################################
## Fireflies_Body.fx
#############################################################

FxInfo
Flags DontSuppress

#########################################################

Condition
	On	Cycle
	Time	20

	Event
		Type	Local
		At	HandL
		ChildFX	:CHILD_Wisps_FistL.fx
		Lifespan 60
	End
End

#############################################################

End