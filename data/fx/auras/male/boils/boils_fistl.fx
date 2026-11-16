#############################################################
## Boils_FistL.fx
#############################################################

FxInfo
Flags InheritAlpha

#############################################################

Condition
	On	Cycle
	Time	4
	Chance	0.25
	Random	1

	Event
		EName	UArmL
		Type	Local
		At	Origin
		ChildFX	:Child_Boil_UArmL.fx
		Lifespan 	50
	End

	Event
		EName	LArmL
		Type	Local
		At	Origin
		ChildFX	:Child_Boil_LArmL.fx
		Lifespan 	50
	End
End

#############################################################

End