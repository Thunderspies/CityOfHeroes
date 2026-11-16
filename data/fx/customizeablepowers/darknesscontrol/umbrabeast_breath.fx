#############################################################
## BeastlyRage_Head.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Cycle
	Time	60

	Event
		Type	Local
		At	Origin
		Bhvr	behaviors/GenericParticleFade.bhvr
		ChildFX	:CHILD_UmbraBeast_Breath.fx
		Lifespan 	30

	End
End

#############################################################

End