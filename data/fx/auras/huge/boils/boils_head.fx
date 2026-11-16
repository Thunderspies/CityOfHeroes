#############################################################
## Boils_FistL.fx
#############################################################

FxInfo
Flags InheritAlpha

#############################################################

Condition
	On	Cycle
	Time	6
	Chance	0.25
	Random	1

	Event
		EName	LArmL
		Type	Local
		At	Origin
		ChildFX	:Child_Head.fx
		Lifespan 	50
	End
End

#############################################################

End