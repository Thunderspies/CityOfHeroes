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
		BHVR	V_COV\Powers\Psionics\ringSpin.bhvr
		Geom	centerdummy
		Lifespan 45
	End

	Event
		Type	local
		At	WepL
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part	V_COV\Powers\Psionics\EnergyHandGlows.part
		Lifespan 45
	End

	Event
		Type	local
		At	rootnodeL
		altpiv	1
		BHVR	V_COV\Powers\Psionics\ringSpin.bhvr
		Part	V_COV\Powers\Psionics\SpinningHandRings.part
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
		BHVR	V_COV\Powers\Psionics\ringSpin.bhvr
		Geom	centerdummy
		Lifespan 45
	End

	Event
		Type	local
		At	WepR
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part	V_COV\Powers\Psionics\EnergyHandGlows.part
		LifeSpan 30
		Lifespan 45
	End

	Event
		Type	local
		At	rootnodeR
		altpiv	1
		BHVR	V_COV\Powers\Psionics\ringSpin.bhvr
		Part	V_COV\Powers\Psionics\SpinningHandRings.part
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
		Bhvr	behaviors\soundFade5.bhvr
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
		Bhvr 	:WorldOfConfusion_Expand.bhvr
		BhvrOverride
			Spin	0.0 0.01 0.0
		End
		Part1	:WorldOfConfusion_Motes1.part
		Part1	:WorldOfConfusion_Motes2.part
		Part1	:WorldOfConfusion_BubbleEdge1.part
		Geom	FX_PillBoxForceField #FX_ForceBubble
	End

	Event
		Type	PositOnly
		At	HIPS
		Bhvr 	:WorldOfConfusion_Expand.bhvr
		BhvrOverride
			Spin	0.0 -0.01 0.0
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
		Bhvr 	behaviors\GenericParticleFade.bhvr
		Part1	:WorldOfConfusion_Rings1.part
		Lifespan 30
	End
End

#############################################################

End