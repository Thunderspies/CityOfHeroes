#############################################################
## FireMeleeFollowUpHit.fx
#############################################################

FxInfo

Lifespan 75

## FIRE FX #######################################################

Condition
	On	Time
	Time	0

	Event
		EName	InitialSwordFire
		Type	Local
		At	WepL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	:FireHandz.part
		Part1	:FireHandz_Alpha.part
		LifeSpan 32
	End

	Event
		EName	InitialSwordFire
		Type	Local
		At	WepR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	:FireHandz.part
		Part1	:FireHandz_Alpha.part
		LifeSpan 32
	End

	Event
		Type	Local
		At	Chest
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:HealFire.part
		LifeSpan 30
	End

	Event
		Type	Local
		At	head
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:HealFire.part
		LifeSpan 30
	End

	Event
		Type	Local
		At	SpadR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:HealFire.part
		LifeSpan 30
	End

	Event
		Type	Local
		At	SpadL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:HealFire.part
		LifeSpan 30
	End

	Event
		Type	Local
		At	UarmR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:HealFire.part
		LifeSpan 30
	End

	Event
		Type	Local
		At	UarmL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:HealFire.part
		LifeSpan 30
	End

	Event
		Type	Local
		At	Hips
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:HealFirehips.part
		LifeSpan 30
	End

	Event
		Type	Local
		At	LarmR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:HealFire.part
		LifeSpan 30
	End

	Event
		Type	Local
		At	LarmL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:HealFire.part
		LifeSpan 30
	End

	Event
		Type	Local
		At	chest
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:FireBuildupSparks.part
		LifeSpan 30
	End
End

## BUILD-UP GENERIC EFFECT ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type 	local
		At	Root
		ChildFX	POWERS\Child_BuildUp.fx
	End
End

#############################################################

End