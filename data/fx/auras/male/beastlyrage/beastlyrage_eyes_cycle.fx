#############################################################
## BeastlyRage_Eyes_Cycle.fx
#############################################################

FxInfo

Flags NotCompatibleWithAnimalHead

#############################################################

Condition
	On	Cycle
	Time	60

	Event
		Type	Local
		At	Eyes
		Bhvr	behaviors/GenericParticleFade.bhvr
		ChildFX	:BeastlyRage_Eyes.fx
		Lifespan 	30

	End
End

#############################################################

End