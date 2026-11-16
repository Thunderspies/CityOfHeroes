#############################################################
## GhostLifeDrainHit.fx
#############################################################

FxInfo

Lifespan	60

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition

	Event
		EName 	Orb
		Type	Local
		At	chest
		BHVR	Behaviors/GenericParticleFade.bhvr
		part	:GhostLifDrainHit.part
		part	:GhostLifDrainHit2.part
		part	:LifeDrainLightRays.part
		Sound miasma_loop 100 100 .5

		Lifespan	30

	End


End

End