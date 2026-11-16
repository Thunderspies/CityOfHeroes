#############################################################
## Wisps_BodyAndEyes.fx
#############################################################



FxInfo
Flags NotCompatibleWithAnimalHead DontSuppress

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