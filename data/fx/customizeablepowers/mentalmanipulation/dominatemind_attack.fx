#############################################################
## Drain Psyche
#############################################################

FxInfo

LifeSpan 150

## HANDS ###########################################################

Condition
	On	Time
	Time	10

	Event
		EName	rootnodeL
		Type	local
		At	WepL
		BHVR	CustomizeablePowers\MentalManipulation\ringSpin.bhvr
		Geom	centerdummy
	End

	Event
		Type	local
		At	WepL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\MentalManipulation\EnergyHandGlows.part
		LifeSpan 30
	End

	Event
		Type	local
		At	rootnodeL
		altpiv	1
		BHVR	CustomizeablePowers\MentalManipulation\ringSpin.bhvr
		Part	CustomizeablePowers\MentalManipulation\SpinningHandRings.part
	End
End

Condition
	On	Time
	Time	10

	Event
		EName	rootnodeR
		Type	local
		At	WepR
		BHVR	CustomizeablePowers\MentalManipulation\ringSpin.bhvr
		Geom	centerdummy
	End

	Event
		Type	local
		At	WepR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\MentalManipulation\EnergyHandGlows.part
		LifeSpan 30
	End

	Event
		Type	local
		At	rootnodeR
		altpiv	1
		BHVR	CustomizeablePowers\MentalManipulation\ringSpin.bhvr
		Part	CustomizeablePowers\MentalManipulation\SpinningHandRings.part
	End
End

#############################################################

#Condition
#	On	Time
#	Time	0
#
#	Event
#		EName	Headnode
#		Type	Local
#		At	Root
#		Geom	RootToHeadAdjustment
#		LifeSpan 20
#	End
#
#	Event
#		EName	coreR
#		Type	Local
#		At	Headnode
#		altpiv	1
#		Part	CustomizeablePowers\MentalManipulation\EnergyDown.part
#		LifeSpan 20
#	End
#End
#
#Condition
#	On	Time
#	Time	30
#
#	Event
#		EName	coreR
#		Type	Local
#		At	Root
#		Part	CustomizeablePowers\MentalManipulation\EnergyUp.part
#		Part	CustomizeablePowers\MentalManipulation\EnergyUp2.part
#		LifeSpan 15
#	End
#
#End

## SPLATS ############################################################################

#Condition
#	On	Time
#	Time	0
#
#	Event
#		EName	Hookup1
#		Type	Local
#		At	Root
#		BHVR	CustomizeablePowers\MentalManipulation\Spin.bhvr
#		Geom	CenterDummy
#	End
#End
#
#Condition
#	On	Time
#	Time	10
#
#	Event
#		Type	Local
#		At	Hookup1
#		Part	CustomizeablePowers\MentalManipulation\EnergyFlash.part
#		LifeSpan	30
#	End
#End

##############################################################################

#Condition
#	On	Time
#	Time	1
#
#	Event
#		Type	Local
#		At	Root
#		Part	CustomizeablePowers\MentalManipulation\LightEnergyInBurst.part
#		LifeSpan 10
#	End
#End
#
#Condition
#	On	Time
#	Time	18
#
#	Event
#		Type	local
#		At	Root
#		Part	CustomizeablePowers\MentalManipulation\LightEnergyOutBurst.part
#		LifeSpan 10
#	End
#End

##############################################################################

EnD