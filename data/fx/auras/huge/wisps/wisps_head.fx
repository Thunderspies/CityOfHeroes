#############################################################
## Wisps_Head.fx
#############################################################

FxInfo
Flags DontSuppress

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		ChildFX	:Wisps_Eyes.fx
	End
End

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