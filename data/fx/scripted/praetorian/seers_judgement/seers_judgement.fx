#############################################################
## DominateMind_Attack.fx
#############################################################

FxInfo
#LifeSpan	300

## HANDS ###########################################################

Condition
	On	Time
	Time	0

	Event
		EName	rootnodeR
		Type	local
		At	WepL
		Sound Seers_DominateMind_Attack 100 100 1
		BHVR	:Seers_Judgement_ringSpin.bhvr
		Geom	centerdummy
		Part	:Seers_Judgement_SpinningHandRings.part
		LifeSpan	0
	End

	Event
		Type	local
		At	WepL
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part	:Seers_Judgement_EnergyHandGlows.part
		LifeSpan	0
	End

	Event
		EName	EyeAnchor
		Type	local
		At	WepL
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part	:Seers_Judgement_EnergyHandGlows.part

		LifeSpan	0
	End
End
Condition
	On	Time
	Time	45

	Event
		Type	Local
		At	WepL
#		Part	:Seers_Judgement_EnergyHandGlows_Growing.part
		Lifespan 0
	End
##############################################################################

End