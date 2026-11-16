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
		EName	HeadAnchor
		Type	Local
		At	Origin
		ChildFX	:Child_Head.fx
		Lifespan 	50
	End

	Event
		EName	FistsAnchor
		Type	Local
		At	Origin
		ChildFX	:Boils_Fists.fx
		Lifespan 	50
	End
End

#############################################################

End