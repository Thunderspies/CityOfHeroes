#############################################################
## Sparks_Body.fx
#############################################################

FxInfo
Flags InheritAlpha InheritGeoScale

#############################################################

Condition
	On	Cycle
	Time	5
	Chance	0.3

	Event
		EName	Arm
		Type	Local
		At	Origin
		ChildFX	:CHILD_Boil_Chest.fx
		Lifespan 	50
	End

End

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
		EName	UArmR
		Type	Local
		At	Origin

		ChildFX	:Child_Boil_UArmR.fx
		Lifespan 	50
	End

	Event
		EName	LArmL
		Type	Local
		At	Origin
		ChildFX	:Child_Boil_LArmL.fx
		Lifespan 	50
	End

	Event
		EName	LArmR
		Type	Local
		At	Origin
		ChildFX	:CHILD_Boil_LArmR.fx
		Lifespan 	50
	End
End

#############################################################

End