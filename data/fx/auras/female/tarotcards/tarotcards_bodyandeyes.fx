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
		At	Origin
		ChildFX :Eyes.fx
	End
End

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
		Lifespan	80
	End

End

#############################################################

End