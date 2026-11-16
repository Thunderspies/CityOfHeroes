#############################################################
## Sparks_Body.fx
#############################################################

FxInfo
Flags InheritAlpha

#############################################################

Condition
	On	Cycle
	Time	3
	Chance	0.25
	Random	1

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