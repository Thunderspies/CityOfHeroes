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
		ChildFx	:Eyes.fx
	End

	Event
		Type	Local
		At	Head
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	:Sparkles_Small.part
		Part	:Smoke_Small.part
		Part	:Smoke2_Small.part
		Part	:Smoke_Trail.part
	End

	Event
		Type	Local
		At	HandL
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	:Sparkles_Small.part
		Part	:Smoke_Small.part
		Part	:Smoke2_Small.part
		Part	:Smoke_Trail.part
	End

	Event
		Type	Local
		At	HandR
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	:Sparkles_Small.part
		Part	:Smoke_Small.part
		Part	:Smoke2_Small.part
		Part	:Smoke_Trail.part
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

Condition
	On	Cycle
	Time	20
	Random	1

	Event
		Type	Local
		At	HandL
		ChildFX	:CHILD_Runes_Hand.fx
		Lifespan 75
	End

	Event
		Type	Local
		At	HandL
		ChildFX	:CHILD_Runes2_Hand.fx
		Lifespan 75
	End


	Event
		Type	Local
		At	HandL
		ChildFX	:CHILD_Runes3_Hand.fx
		Lifespan 75
	End

	Event
		Type	Local
		At	HandL
		ChildFX	:CHILD_Runes4_Hand.fx
		Lifespan 75
	End

	Event
		Type	Local
		At	HandL
		ChildFX	:CHILD_Runes5_Hand.fx
		Lifespan 75
	End

	Event
		Type	Local
		At	HandL
		ChildFX	:CHILD_Runes6_Hand.fx
		Lifespan 75
	End

	Event
		Type	Local
		At	HandL
		ChildFX	:CHILD_Runes7_Hand.fx
		Lifespan 75
	End
End

Condition
	On	Cycle
	Time	20
	Random	1

	Event
		Type	Local
		At	HandR
		ChildFX	:CHILD_Runes_Hand.fx
		Lifespan 75
	End

	Event
		Type	Local
		At	HandR
		ChildFX	:CHILD_Runes2_Hand.fx
		Lifespan 75
	End


	Event
		Type	Local
		At	HandR
		ChildFX	:CHILD_Runes3_Hand.fx
		Lifespan 75
	End

	Event
		Type	Local
		At	HandR
		ChildFX	:CHILD_Runes4_Hand.fx
		Lifespan 75
	End

	Event
		Type	Local
		At	HandR
		ChildFX	:CHILD_Runes5_Hand.fx
		Lifespan 75
	End

	Event
		Type	Local
		At	HandR
		ChildFX	:CHILD_Runes6_Hand.fx
		Lifespan 75
	End

	Event
		Type	Local
		At	HandR
		ChildFX	:CHILD_Runes7_Hand.fx
		Lifespan 75
	End
End
#############################################################

End