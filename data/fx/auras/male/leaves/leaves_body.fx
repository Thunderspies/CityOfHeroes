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
		At	Head
		Part	:Particle_Leaves_Trail.part
		POther	Root
	End
End

Condition
	On	Cycle
	Time	15


	Event
		Ename	GEO_Leaves
		Type	StartPositOnly
		At	Head
		ChildFX	:CHILD_PARTICLE_Leaves.fx
		Lifespan	150
	End

End

Condition
	On	Cycle
	Time	20


	Event
		Ename	GEO_Leaves
		Type	StartPositOnly
		At	Head
		ChildFX	:CHILD_GEO_Leaves.fx
		Lifespan	150
	End

End
#############################################################

End