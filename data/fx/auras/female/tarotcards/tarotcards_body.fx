#############################################################
## Fireflies_Body.fx
#############################################################

FxInfo

#########################################################

Condition
	On	Cycle
	Time	20
	Repeat		1
	RepeatJitter	1

	Event
		Ename	GEO_Leaves
		Type	StartPositOnly
		At	Head
		ChildFX	:CHILD_GEO_TarotCards.fx
		Lifespan	200
	End

End

#############################################################

End