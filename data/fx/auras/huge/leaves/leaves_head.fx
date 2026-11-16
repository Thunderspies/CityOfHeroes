#############################################################
## Fireflies_Body.fx
#############################################################

FxInfo

#########################################################


Condition
	On	Cycle
	Time	15


	Event
		Ename	PARTICLE_Leaves
		Type	StartPositOnly
		At	Head
		ChildFX	:CHILD_PARTICLE_Leaves_Head.fx
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

End
#############################################################

End