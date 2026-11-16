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
		At	Origin
		ChildFX	:Eyes.fx
	End

	Event
		Ename	GEO_Leaves
		Type	PositOnly
		At	Head
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Sparkles_Continuous_Hands.part
		Part	:ColdMist_Additive_Hands.part
		Part	:Smoke_Trail.part
	End
End

Condition
	On	Cycle
	Time	15

	Event
		Ename	GEO_Cards
		Type	Local
		At	Head

		ChildFX	:CHILD_GEO_TarotCards_Head.fx
		Lifespan	100
		LifespanJitter	20
	End

End

#############################################################

End