#############################################################
## Android_Particle_Stunner.fx
#############################################################

FxInfo

Lifespan 90

#############################################################

Condition
	On	Cycle
	Time	1
	Repeat	1
	RepeatJitter 1

	Event
		Type	Local
		At	Root
		ChildFX	ENEMYFX\Clockwork\antimatter\Child_ElectricalArc01.fx
		Lifespan 3
	End
End

#############################################################################

End