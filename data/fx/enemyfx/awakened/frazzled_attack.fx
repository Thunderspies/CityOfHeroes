#############################################################
## PsionicShockwave_Attack.fx
#############################################################

FxInfo
Lifespan 80

#############################################################

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	Root
		Sound 	Awakened_Frazzled_Attack_01 100 100 .8
	End

	Event
		Type	PositOnly
		At	Chest
		Bhvr 	:Frazzled_Bubble.bhvr
		BhvrOverride
			Alpha		255
			StartColor	255 150 255
			FadeOutLength	5
			Spin 		0.01 0.0 0.0
		End
		Geom	Disintegration_Bubble
		Lifespan	17
	End

	Event
		Type	PositOnly
		At	Chest
		Bhvr 	:Frazzled_Bubble.bhvr
		BhvrOverride
			Alpha		255
			StartColor	255 150 255
			PYRRotateJitter	180 180 180
			FadeOutLength	5
			Spin 		0.025 0.01 0.0
		End
		Geom	Tintable_EarthGraspHit

		Lifespan	15
	End


	Event
		Type	StartPositOnly
		At	Chest
		Part	:Frazzled_Border.part
		Part	:Overload_Electric.part

		Lifespan	12
	End

	Event
		Type	StartPositOnly
		At	Root
		Part	:Frazzled_Border_Ground.part
		Part	:Glow_Flat.part

		Lifespan	10
	End
End

#############################################################

End