#############################################################
## Child_FistsAndEyes.fx
#############################################################

FxInfo

Flags NotCompatibleWithAnimalHead

#############################################################

Condition
	On	Cycle
	Time	60

	Event
		Type	Local
		At	Origin
		ChildFX	:Child_FistsAndEyes.fx
		Lifespan 45
	End
End

#############################################################

End