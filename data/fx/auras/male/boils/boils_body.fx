#############################################################
## Sparks_Body.fx
#############################################################

FxInfo
Flags InheritAlpha

#############################################################

Condition
	On	Cycle
	Time	5
	Chance	0.25

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
	Time	2
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

###############################################################

	Event
		EName	ULegL
		Type	Local
		At	Origin
		Bhvr	behaviors/GenericParticleFade.bhvr
		ChildFX	:Child_Boil_ULegL.fx
		Lifespan 	35
	End

	Event
		EName	ULegR
		Type	Local
		At	Origin
		Bhvr	behaviors/GenericParticleFade.bhvr
		ChildFX	:Child_Boil_ULegR.fx
		Lifespan 	35
	End

#############################################################

	Event
		EName	LLegL
		Type	Local
		At	Origin
		ChildFX	:Child_Boil_LLegL.fx
		Lifespan 	35
	End

	Event
		EName	LLegR
		Type	Local
		At	Origin
		ChildFX	:Child_Boil_LLegR.fx
		Lifespan 	35
	End
End

#############################################################

End