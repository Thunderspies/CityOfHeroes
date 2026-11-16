#############################################################
## Sparks_Body.fx
#############################################################

FxInfo

#############################################################


Condition
	On		Cycle
	Time		1
#	Chance		0.5

	Event
		Type	PositOnly
		At	Origin
		Bhvr	Behaviors\GenericParticleFade.bhvr
		ChildFX	:CHILD_LavaFall_Impact.fx
		Lifespan 	8
	End


End


#############################################################

End