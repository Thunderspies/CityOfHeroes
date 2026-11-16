#############################################################
## HEADER
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	5

	Event
		EName	rootnodeL
		Type	local
		At	WepL
		BHVR	CustomizeablePowers\MentalManipulation\ringSpin.bhvr
		Geom	centerdummy
		Lifespan 45
	End

	Event
		Type	local
		At	WepL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:EnergyHandGlows.part
		Lifespan 45
	End

	Event
		Type	local
		At	rootnodeL
		altpiv	1
		BHVR	CustomizeablePowers\MentalManipulation\ringSpin.bhvr
		Part	:SpinningHandRings.part
		Lifespan 45
	End
End

Condition
	On	Time
	Time	5

	Event
		EName	rootnodeR
		Type	local
		At	WepR
		BHVR	CustomizeablePowers\MentalManipulation\ringSpin.bhvr
		Geom	centerdummy
		Lifespan 45
	End

	Event
		Type	local
		At	WepR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:EnergyHandGlows.part
		LifeSpan 30
		Lifespan 45
	End

	Event
		Type	local
		At	rootnodeR
		altpiv	1
		BHVR	CustomizeablePowers\MentalManipulation\ringSpin.bhvr
		Part	:SpinningHandRings.part
		Lifespan 45
	End
End

#############################################################

Condition
	On	Time
	Time 	7

	Event
		Type 	Local
		At	Origin
		Sound WorldOfConfusion_Start_01 80 80 .8
	End
End

Condition
	On	Time
	Time 	17

	Event
		Type 	Local
		At	Origin
		Bhvr	Behaviors\soundFade5.bhvr
		Sound WorldOfConfusion_Loop 100.0 100.0 .5
		Flags	PowerLoopingSound
		Lifespan 120
	End
End

Condition
	On 	Time
	Time 	17

	Event
		Type	PositOnly
		At	HIPS
		Bhvr	CustomizeablePowers\MentalManipulation\WorldOfConfusion_Expand.bhvr
		BhvrOverride

			Alpha		20
			Spin		0.0 0.01 0.0
			TintGeom	1
		End
		Part1	:WorldOfConfusion_Motes1.part
		Part1	:WorldOfConfusion_Motes2.part
		Part1	:WorldOfConfusion_BubbleEdge1.part
		Geom	FX_Tintable_Dark_PillBoxForceField
	End

	Event
		Type	PositOnly
		At	HIPS
		Bhvr	CustomizeablePowers\MentalManipulation\WorldOfConfusion_Expand.bhvr
		BhvrOverride
			Spin		0.0 -0.01 0.0
		End
		Part1	:WorldOfConfusion_Motes3.part
		Part1	:WorldOfConfusion_Motes4.part
	End
End

Condition
	On 	Cycle
	Time 	60

	Event
		Type	PositOnly
		At	HIPS
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	:WorldOfConfusion_Rings1.part
		Lifespan 30
	End
End

#############################################################

End