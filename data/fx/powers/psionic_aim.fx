#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 75

#############################################################

Condition
	On	time
	Time	0

	Event
		Type 	local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	:YellowUpGlow.part
		Part2	:YellowUpSpecks2.part
		Sound aim2 60 60 .6
		Lifespan 13
	End
End

Condition
	On	Time
	Time	10

	Event
		Type 	local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part2	:YellowUpRing01.part
		Part3	:YellowUpRing01a.part
	End
End

Condition
	On	Time
	Time	20

	Event
		Type 	local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part3	:YellowUpRing02.part
		Part4	:YellowUpRing02a.part
	End
End

Condition
	On	Time
	Time	30

	Event
		EName	Pow
		Type 	local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part2	:YellowUpRingSlow.part
		Part3	:YellowUpRing03.part
		Part4	:YellowUpRing03a.part
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	rootnodeL
		Type	local
		At	WepL
		BHVR	V_COV\Powers\Psionics\ringSpin.bhvr
		Geom	centerdummy
	End

	Event
		Type	local
		At	WepL
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part	V_COV\Powers\Psionics\EnergyHandGlows.part
		LifeSpan 30
	End

	Event
		Type	local
		At	rootnodeL
		altpiv	1
		BHVR	V_COV\Powers\Psionics\ringSpin.bhvr
		Part	V_COV\Powers\Psionics\SpinningHandRings.part
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	rootnodeR
		Type	local
		At	WepR
		BHVR	V_COV\Powers\Psionics\ringSpin.bhvr
		Geom	centerdummy
	End

	Event
		Type	local
		At	WepR
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part	V_COV\Powers\Psionics\EnergyHandGlows.part
		LifeSpan 30
	End

	Event
		Type	local
		At	rootnodeR
		altpiv	1
		BHVR	V_COV\Powers\Psionics\ringSpin.bhvr
		Part	V_COV\Powers\Psionics\SpinningHandRings.part
	End
End

#############################################################

End