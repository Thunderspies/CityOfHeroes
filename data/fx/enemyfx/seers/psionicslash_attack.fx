#############################################################
## DominateMind_Attack.fx
#############################################################

FxInfo
LifeSpan	150

## HANDS ###########################################################

Condition
	On	Time
	Time	0

	Event
		EName	rootnodeL
		Type	local
		At	WepL
		Sound Seers_PsionicSlash_Attack 100 100 1
		BHVR	:ringSpin.bhvr
		Geom	centerdummy
	#	Sound	psionic_drainPsyche 100 70 .8
	End

	Event
		Type	local
		At	WepL
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part	:EnergyHandGlows.part
		LifeSpan	30
	End

	Event
		Type	local
		At	rootnodeL
		altpiv	1
		BHVR	:ringSpin.bhvr
		Part	:SpinningHandRings.part
	End

	Event
		EName	rootnodeR
		Type	local
		At	WepR
		BHVR	:ringSpin.bhvr
		Geom	centerdummy
		Part	:SpinningHandRings.part
	End

	Event
		Type	local
		At	WepR
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part	:EnergyHandGlows.part
		LifeSpan	30
	End

	Event
		EName	EyeAnchor
		Type	local
		At	WepR
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part	:EnergyHandGlows.part

		LifeSpan	30
	End
End

Condition
	On	Time
	Time	20


	Event
		EName	EyeAnchor
		Type	local
		At	WepR
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part	:EnergyHandGlows_Growing.part
		LifeSpan	30
	End

	Event
		EName	EyeAnchor
		Type	local
		At	WepL
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part	:EnergyHandGlows_Growing.part
		LifeSpan	30
	End

	Event
		EName	EyeAnchor
		Type	local
		At	WepR
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part	:Hands_Rings_Growing.part
		LifeSpan	10
	End

	Event
		EName	EyeAnchor
		Type	local
		At	WepL
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part	:Hands_Rings_Growing.part
		LifeSpan	10
	End
End

##############################################################################

End