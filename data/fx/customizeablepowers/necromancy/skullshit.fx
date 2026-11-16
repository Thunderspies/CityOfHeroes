#############################################################
## SkullsHit.fx
#############################################################

FxInfo

Lifespan	60

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound skullhit 80 80 .6
	End
End

Condition

	Event
		EName 	Orb
		Type	Local
		At	chest
		BHVR	Behaviors/GenericParticleFade.bhvr
		part	:SkullsHit.part
		part	:SkullsHit2.part
		part	:SkullsHitLightRays.part
		Sound miasma_loop 100 100 .5

		Lifespan	30

	End


End

End