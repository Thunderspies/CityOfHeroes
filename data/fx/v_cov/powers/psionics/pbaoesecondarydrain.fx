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
		BHVR	:ringSpin.bhvr
		Geom	centerdummy
		Sound psionic_drainPsyche 100 100 .8
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
End

Condition
	On	Time
	Time	10

	Event
		EName	rootnodeR
		Type	local
		At	WepR
		BHVR	:ringSpin.bhvr
		Geom	centerdummy
	End

	Event
		Type	local
		At	WepR
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part	:EnergyHandGlows.part
		LifeSpan	30	
	End
	
	Event
		Type	local
		At	rootnodeR
		altpiv	1
		BHVR	:ringSpin.bhvr
		Part	:SpinningHandRings.part
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
		Part	:EnergyDown.part
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
		Part	:EnergyUp.part
		Part	:EnergyUp2.part
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
		BHVR	:Spin.bhvr
		Geom	CenterDummy
	End
End

Condition
	On	Time
	Time	10
	
	Event
		Type	Local
		At	Hookup1
		Part	:EnergyFlash.part
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
		Part	:LightEnergyInBurst.part
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
		Part	:LightEnergyOutBurst.part
		LifeSpan	10
	End
End

##############################################################################

EnD