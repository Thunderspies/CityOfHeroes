#############################################################
## Fireflies_Body.fx
#############################################################

FxInfo

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Head
		Bhvr	behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.0 0.0
		End
		Part	:Sparkles_Small.part
		Part	:Smoke_Small.part
		Part	:Smoke2_Small.part
	End
End

Condition
	On	Cycle
	Time	20
	Random	1

	Event
		Type	Local
		At	Head
		ChildFX	:CHILD_Runes_Small.fx
		Lifespan 75
	End

	Event
		Type	Local
		At	Head
		ChildFX	:CHILD_Runes2_Small.fx
		Lifespan 75
	End


	Event
		Type	Local
		At	Head
		ChildFX	:CHILD_Runes3_Small.fx
		Lifespan 75
	End

	Event
		Type	Local
		At	Head
		ChildFX	:CHILD_Runes4_Small.fx
		Lifespan 75
	End

	Event
		Type	Local
		At	Head
		ChildFX	:CHILD_Runes5_Small.fx
		Lifespan 75
	End

	Event
		Type	Local
		At	Head
		ChildFX	:CHILD_Runes6_Small.fx
		Lifespan 75
	End

	Event
		Type	Local
		At	Head
		ChildFX	:CHILD_Runes7_Small.fx
		Lifespan 75
	End
End

#############################################################

End