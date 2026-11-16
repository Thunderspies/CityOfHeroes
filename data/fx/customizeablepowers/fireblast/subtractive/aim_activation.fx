#############################################################
## FireBlastAim.fx
#############################################################

FxInfo

Lifespan 75

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:ChestFire.part
		LifeSpan 20
	End

	Event
		Type	Local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:HealFire.part
		Part	:FireBuildupSparks.part
		LifeSpan 30
	End

	Event
		EName	InitialSwordFire
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	:HealFire.part
		LifeSpan 30
	End

	Event
		EName	InitialSwordFire
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	:HealFire.part
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
End

## GENERIC FX ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		ChildFX	POWERS\Child_Aim.fx
	End
End

#############################################################

End