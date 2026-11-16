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
		At	Origin
		ChildFX	:CHILD_Wisps_Head.fx
		Lifespan 60
	End
End

#############################################################

End