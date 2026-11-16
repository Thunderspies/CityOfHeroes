#############################################################
## SecondaryMutation_PainTolerance_Hit.fx
#############################################################

FxInfo
Lifespan 300

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type Local
		At Origin
		Sound PainTolerance_Child_01 30 30 .1
	End

	Event
		Type	Local
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Chest_Heartbeat.part
		Part	:Chest_Ring.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Chest_Heartbeat.part
		Part	:Chest_Ring.part

		Lifespan 20
	End
End

#############################################################

End