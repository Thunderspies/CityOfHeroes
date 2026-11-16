#############################################################
## TemporalMending_Hit.fx
#############################################################

FxInfo
Lifespan 60

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event

		Type	PositOnly
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Spiral_Explode_Small.part
		Part	:Swirl_Explode_Small.part
		Part	:Swirl_Explode_Small_Highlights.part
		Part	:Jaunt_Flash_Impact_Small.part
		Part	:Jaunt_Glow_Grow_Small.part
		Part	:Particles_Explode_Quick.part
		Part	:Streaks_Explode_Small.part
		Lifespan 1
	End
End

Condition
	On	Time
	Time	3

	Event
		Type	PositOnly
		At	Chest
		Bhvr	:TimeManip_Grow_Fast.bhvr
		BhvrOverride

			Alpha		255
			StartColor	255 255 255
			FadeInLength	20
			FadeOutLength	20

			Spin 		0.0 -0.0025 0
		End
		Geom	Tintable_Dark_EarthGraspHit
		Lifespan 5
	End
End



#############################################################

End