#############################################################
## BeastlyRage_Chest.fx
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
		ChildFX	:Child_Chest.fx
		Lifespan 	30

	End
End

#############################################################

End