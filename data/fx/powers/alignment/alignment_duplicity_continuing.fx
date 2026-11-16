#############################################################
## LeadershipTactics.fx
#############################################################

FxInfo
Lifespan 450

##################################

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	Head
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	:Rings_Spinning.part
		Part	:Rings_Spinning_Additive_Small.part
	#	Part	:Smoke_Expanding.part
		Lifespan 	450
	End
End