#############################################################
## Fireflies_Body.fx
#############################################################

FxInfo

Flags NotCompatibleWithAnimalHead

#########################################################
Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Head
		Bhvr	behaviors\GenericParticleFade.bhvr
		ChildFx	:Eyes.fx
	End

End

Condition
	On	Cycle
	Time	20



	Event
		Type	Local
		At	Origin
		ChildFX	:CHILD_Continuous_Body.fx
		Lifespan 80
	End
End

Condition
	On	Cycle
	Time	15
	Random	1

	Event
		Type	Local
		At	Origin
		ChildFX	:CHILD_Runes_Body.fx
		Lifespan 90
	End

	Event
		Type	Local
		At	Origin
		ChildFX	:CHILD_Runes2_Body.fx
		Lifespan 90
	End

	Event
		Type	Local
		At	Origin
		ChildFX	:CHILD_Runes3_Body.fx
		Lifespan 90
	End

	Event
		Type	Local
		At	Origin
		ChildFX	:CHILD_Runes4_Body.fx
		Lifespan 90
	End

	Event
		Type	Local
		At	Origin
		ChildFX	:CHILD_Runes5_Body.fx
		Lifespan 90
	End

	Event
		Type	Local
		At	Origin
		ChildFX	:CHILD_Runes6_Body.fx
		Lifespan 90
	End

	Event
		Type	Local
		At	Origin
		ChildFX	:CHILD_Runes7_Body.fx
		Lifespan 90
	End
End

#############################################################

End