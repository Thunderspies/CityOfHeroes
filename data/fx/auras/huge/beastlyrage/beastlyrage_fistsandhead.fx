#############################################################
## Child_FistsAndEyes.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Cycle
	Time	60

	Event
		Type	Local
		At	Origin
		ChildFX	:Child_FistsAndHead.fx
		Lifespan 45
	End
End

#############################################################

End