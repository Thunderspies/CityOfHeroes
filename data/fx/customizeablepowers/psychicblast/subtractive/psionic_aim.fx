#############################################################
## Psionic_Aim.fx
#############################################################

FxInfo

Lifespan 75

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	rootnodeL
		Type	local
		At	WepL
		BHVR	CustomizeablePowers\PsychicBlast\ringSpin.bhvr
		Geom	centerdummy
	End

	Event
		Type	local
		At	WepL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:EnergyHandGlows.part
		LifeSpan 30
	End

	Event
		Type	local
		At	rootnodeL
		altpiv	1
		BHVR	CustomizeablePowers\PsychicBlast\ringSpin.bhvr
		Part	:SpinningHandRings.part
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	rootnodeR
		Type	local
		At	WepR
		BHVR	CustomizeablePowers\PsychicBlast\ringSpin.bhvr
		Geom	centerdummy
	End

	Event
		Type	local
		At	WepR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:EnergyHandGlows.part
		LifeSpan 30
	End

	Event
		Type	local
		At	rootnodeR
		altpiv	1
		BHVR	CustomizeablePowers\PsychicBlast\ringSpin.bhvr
		Part	:SpinningHandRings.part
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