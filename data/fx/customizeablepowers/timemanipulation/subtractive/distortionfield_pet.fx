#############################################################
## DistortionField_Pet.fx
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
		Sound 	TM_TemporalDistortion_Continuing_Loop 100 100 .25
		bhvr	behaviors/Sound/SoundIn2Out10.bhvr
		lifespan 500
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Create
		Inherit	Position
		Update	Position
		At	Root
		Bhvr	:TimeManip_Shrink_Big.bhvr
		BhvrOverride
			Alpha		70
			FadeInLength	100
			FadeOutLength	50
			Spin 		0.0 0.0025 -0.0025
			PYRRotate	0 0 70
			TintGeom	1
		End
		Geom	Tintable_Dark_EarthGraspHit
		Lifespan 20
	End

	Event
		Type	PositOnly
		At	Root
		Part	:Spiral_Slow_Big.part
		Part	:Spiral_Slow_Big_Flat.part
	#	Part	:Particles_Slow_Big.part
		Part	:Glow_Big_Flat.part

		PMagnet	Chest
	End
End

Condition
	On	Time
	Time	30

	Event
		Type	Create
		Inherit	Position
		Update	Position
		At	Root
		Bhvr	:TimeManip_Big_Stationary.bhvr
		BhvrOverride
			Alpha		50
			FadeInLength	100
			FadeOutLength	100
			Spin 		0.0 0.0 0.0
			PYRRotate	0 0 70
			TintGeom	1
		End
		Geom	Tintable_TimeManip
		Lifespan 30
	End
End

Condition
	On	Cycle
	Time	100

	Event
		Type	Create
		Inherit	Position
		Update	Position
		At	Root
		Bhvr	:TimeManip_Big_Stationary.bhvr
		BhvrOverride
			Alpha		50
			FadeInLength	100
			FadeOutLength	100
			Spin 		0.0 0.0025 -0.0025
			PYRRotate	0 0 70
			TintGeom	1
		End
		Geom	Tintable_TimeManip
		Lifespan 30
	End

#############################################################

End