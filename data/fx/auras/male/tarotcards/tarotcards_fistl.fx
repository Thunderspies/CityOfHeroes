#############################################################
## Fireflies_Body.fx
#############################################################

FxInfo

#########################################################

Condition
	On	Time
	Time	0

	Event
		Ename	GEO_Leaves
		Type	Local
		At	HandL
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
		At	HandL
		ChildFX	:CHILD_GEO_TarotCards_Hand.fx
		Lifespan	80
		LifespanJitter	20
	End

End

#############################################################

End