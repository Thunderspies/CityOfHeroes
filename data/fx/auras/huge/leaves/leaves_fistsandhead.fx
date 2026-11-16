#############################################################
## Fireflies_Body.fx
#############################################################

FxInfo

#########################################################


Condition
	On	Cycle
	Time	20


	Event
		Ename	GEO_Leaves
		Type	StartPositOnly
		At	Head
		ChildFX	:CHILD_PARTICLE_Leaves_Head.fx
		Lifespan	80
	End

	Event
		Ename	GEO_Leaves_L
		Type	StartPositOnly
		At	WepL
		ChildFX	:CHILD_PARTICLE_Leaves_Hand.fx
		Lifespan	80
	End

	Event
		Ename	GEO_Leaves_R
		Type	StartPositOnly
		At	WepR
		ChildFX	:CHILD_PARTICLE_Leaves_Hand.fx
		Lifespan	80
	End

End

Condition
	On	Cycle
	Time	35


	Event
		Ename	GEO_Leaves
		Type	StartPositOnly
		At	Head
		ChildFX	:CHILD_GEO_Leaves_Head.fx
		Lifespan	150
	End

	Event
		Ename	GEO_Leaves_L
		Type	StartPositOnly
		At	WepL
		ChildFX	:CHILD_GEO_Leaves_Head.fx
		Lifespan	150
	End

	Event

		Ename	GEO_Leaves_R
		Type	StartPositOnly
		At	WepR
		ChildFX	:CHILD_GEO_Leaves_Head.fx
		Lifespan	150
	End

End

#############################################################

End