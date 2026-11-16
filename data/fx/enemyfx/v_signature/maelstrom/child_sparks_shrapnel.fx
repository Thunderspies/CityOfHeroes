#############################################################
## Mercenary Mastermind Assault Rifle - Slug
#############################################################

FxInfo
Lifespan 30

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Origin
	#	Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:Sparks_Flying.part
		part	:Sparks_Flying.part
		part	:SparkBurst_Equipment.part
		part	:Dust_Shrapnel.part

		Lifespan 1
	End
End

#############################################################

End