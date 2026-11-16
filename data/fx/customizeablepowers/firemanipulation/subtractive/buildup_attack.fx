#############################################################
## BuildUp_Attack.fx
#############################################################

FxInfo


Lifespan 75

#############################################################

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	:FireHandz.part
		Part1	:FireHandz_Alpha.part

		LifeSpan 32
	End

	Event
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	:FireHandz.part
		Part1	:FireHandz_Alpha.part

		LifeSpan 32
	End

	Event
		Type	Local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:HealFire.part
		LifeSpan 30
	End

	Event
		Type	Local
		At	head
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:HealFire.part
		LifeSpan 30
	End

	Event
		Type	Local
		At	SpadR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:HealFire.part
		LifeSpan 30
	End

	Event
		Type	Local
		At	SpadL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:HealFire.part
		LifeSpan 30
	End

	Event
		Type	Local
		At	UarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:HealFire.part
		LifeSpan 30
	End

	Event
		Type	Local
		At	UarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:HealFire.part
		LifeSpan 30
	End

	Event
		Type	Local
		At	Hips
		Part	:HealFirehips.part
		LifeSpan 30
	End

	Event
		Type	Local
		At	LarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:HealFire.part
		LifeSpan 30
	End

	Event
		Type	Local
		At	LarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:HealFire.part
		LifeSpan 30
	End

	Event
		Type	Local
		At	chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:FireBuildupSparks.part
		LifeSpan 30
	End
End

#############################################################

Condition
	On	Time
	Time	15

	Event
		Type 	local
		At	Root
		ChildFX	POWERS\Child_BuildUp.fx
	End
End

#############################################################

End