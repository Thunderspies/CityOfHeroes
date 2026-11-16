#############################################################
## Drain Psyche
#############################################################

FxInfo
	
LifeSpan	150

## HANDS ###########################################################

Condition
	On	Time
	Time	10

	Event
		EName	rootnodeL
		Type	local
		At	WepL
		BHVR	CustomizeablePowers\PsionicAssault\ringSpin.bhvr
		Geom	centerdummy
	End

	Event
		Type	local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\PsionicAssault\EnergyHandGlows.part
		LifeSpan	30	
	End

	Event
		Type	local
		At	rootnodeL
		altpiv	1
		BHVR	CustomizeablePowers\PsionicAssault\ringSpin.bhvr
		Part	CustomizeablePowers\PsionicAssault\SpinningHandRings.part
	End
End

Condition
	On	Time
	Time	10

	Event
		EName	rootnodeR
		Type	local
		At	WepR
		BHVR	CustomizeablePowers\PsionicAssault\ringSpin.bhvr
		Geom	centerdummy
	End

	Event
		Type	local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\PsionicAssault\EnergyHandGlows.part
		LifeSpan	30	
	End
	
	Event
		Type	local
		At	rootnodeR
		altpiv	1
		BHVR	CustomizeablePowers\PsionicAssault\ringSpin.bhvr
		Part	CustomizeablePowers\PsionicAssault\SpinningHandRings.part
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Headnode
		Type	Local
		At	Root
		Geom	RootToHeadAdjustment
		LifeSpan	20		
	End

	Event
		EName	coreR
		Type	Local
		At	Headnode
		altpiv	1
		Part	CustomizeablePowers\PsionicAssault\EnergyDown.part
		LifeSpan	20
	End
End

Condition
	On	Time
	Time	30

	Event
		EName	coreR
		Type	Local
		At	Root
		Part	CustomizeablePowers\PsionicAssault\EnergyUp.part
		Part	CustomizeablePowers\PsionicAssault\EnergyUp2.part
		LifeSpan	15
	End

End

## SPLATS ############################################################################

Condition
	On	Time
	Time	0
	
	Event
		EName	Hookup1
		Type	Local
		At	Root
		BHVR	CustomizeablePowers\PsionicAssault\Spin.bhvr
		Geom	CenterDummy
	End
End

Condition
	On	Time
	Time	10
	
	Event
		Type	Local
		At	Hookup1
		Part	CustomizeablePowers\PsionicAssault\EnergyFlash.part
		LifeSpan	30
	End
End

##############################################################################

Condition
	On	Time
	Time	1

#	Event
#		EName	Hookup1
#		Type	Local
#		At	Root
#		BHVR	:DrainPsyche_Splat1.bhvr
#		Splat	Generic_Ring
#		LifeSpan	10
#	End

	Event
		Type	Local
		At	Root
		Part	CustomizeablePowers\PsionicAssault\LightEnergyInBurst.part
		LifeSpan	10
	End
End

Condition
	On	Time
	Time	18

#	Event
#		EName	rootnode
#		Type	local
#		At	Root
#		BHVR	:SplatRings2.bhvr
#		Splat	Generic_Ring
#		LifeSpan	10
#	End

	Event
		Type	local
		At	Root
		Part	CustomizeablePowers\PsionicAssault\LightEnergyOutBurst.part
		LifeSpan	10
	End
End

##############################################################################

EnD