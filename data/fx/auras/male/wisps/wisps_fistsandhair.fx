#############################################################
## Wisps_FistsAndHair.fx
#############################################################

FxInfo
Flags DontSuppress

#########################################################


Condition
	On	Cycle
	Time	20

	Event
		Type	Start
		At	Origin
		ChildFX	:CHILD_Wisps_Head.fx
		Lifespan 60
	End
End


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