#############################################################
## TimeCrawl_Continuing.fx
#############################################################

FxInfo

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	TM_TimeCrawl_Continuing_Loop 100 100 .35
		bhvr	behaviors/Sound/SoundIn2Out10.bhvr
		lifespan 500
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	SwirlAnchor
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 4.0 0.0
		End
	End

	Event

		Type	PositOnly
		At	SwirlAnchor
		Part	:Swirl_Quick_Outer_Small.part
		Part	:Particles_Quick.part
		PMagnet	Chest
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	PositOnly
		At	SwirlAnchor
		Part	:Swirl_Slow_Small.part
		Part	:Particles_Slow.part
	End
End

Condition
	On	Time
	Time	30

	Event
		Type	PositOnly
		At	SwirlAnchor
		Bhvr	:TimeManip_Grow.bhvr
		BhvrOverride
			Alpha		255
			FadeInLength	20
			FadeOutLength	20
			Spin 		0.0025 -0.0025 0.0
			TintGeom	1
		End
		Geom	Tintable_Dark_EarthGraspHit
	End
End

#############################################################

End