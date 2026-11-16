#############################################################
## Sparks_Body.fx
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
		EName	UArmR
		Type	Local
		At	Origin

		ChildFX	:Child_Boil_UArmR.fx
		Lifespan 	50
	End

	Event
		EName	LArmR
		Type	Local
		At	Origin

		ChildFX	:Child_Boil_LArmR.fx
		Lifespan 	50
	End
End

#############################################################

End