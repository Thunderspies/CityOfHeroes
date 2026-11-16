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
		EName	rootnodeR
		Type	local
		At	WepR
		Sound Seers_DominateMind_Attack 100 100 1
		BHVR	:ringSpin.bhvr
		Geom	centerdummy
		Part	:SpinningHandRings.part
	End

	Event
		Type	local
		At	WepR
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part	:EnergyHandGlows.part
		LifeSpan	45
	End

	Event
		EName	EyeAnchor
		Type	local
		At	WepR
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part	:EnergyHandGlows.part

		LifeSpan	45
	End
End
Condition
	On	Time
	Time	45

	Event
		Type	Local
		At	WepR
		Part	:EnergyHandGlows_Growing.part
		Lifespan 10
	End
##############################################################################

End