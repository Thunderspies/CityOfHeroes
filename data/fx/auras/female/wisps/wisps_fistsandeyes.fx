#############################################################
## Fireflies_Body.fx
#############################################################

FxInfo
Flags NotCompatibleWithAnimalHead DontSuppress

#########################################################


Condition
	On	Time
	Time	0

	Event
		Type	Start
		At	Origin
		ChildFX	:Wisps_Eyes.fx
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