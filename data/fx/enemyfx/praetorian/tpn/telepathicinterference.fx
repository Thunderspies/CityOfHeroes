#############################################################
## HEADER
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time 	0

	Event
		Type 	Local
		At	Origin
		Sound WorldOfConfusion_Start_01 80 80 .8
	End
End

Condition
	On	Time
	Time 	10

	Event
		Type 	Local
		At	Origin
		Bhvr	behaviors\soundFade5.bhvr
		Sound WorldOfConfusion_Loop 100.0 100.0 .5
		Flags	PowerLoopingSound
##		Lifespan 120
	End
End

Condition
	On 	Time
	Time 	0

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
	Time 	50

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