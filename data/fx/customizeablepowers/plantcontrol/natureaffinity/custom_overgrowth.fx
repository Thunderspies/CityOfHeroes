FXinfo
Lifespan 150

##########################################################
Condition
	On 	Time
	Time	5
	Event
		Type	Local
		At	Root
		Sound 	NA_Overgrowth_01 100 100 .8
	End
End

	Condition
		on time
		time 0

		Event
			At root
			type Local
			ChildFX :Custom_Regrowth_Activation.fx
		End

		Event
			At root
			type Local
			ChildFX :Overgrowth_Child2.fx
		End
	End

	Condition
		On time
		Time 15

		Event
			At LArmR
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\QuickGrow.bhvr
			BhvrOverride
				TintGeom 1
				FadeInLength 20
				FadeOutLength 15
				PyrRotate 180 -0 -15
				PositionOffset 0.5 0 0.0
				Scale .1 .1 .1
			End
			Geom FX_Tintable_Leaf_Wreath_ArmR
			LifeSpan 100
		End

		Event
			At LArmR
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\QuickGrow.bhvr
			BhvrOverride
				TintGeom 1
				PositionOffset 0.3 0 0.1
				PyrRotate 0 15 -5
				Scale 0.1 0.1 0.1
				FadeInLength 20
				FadeOutLength 15
			End
			Geom FX_Tintable_Leaf_Wreath_ArmR
			LifeSpan 100
		End

		Event
			At LArmL
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\QuickGrow.bhvr
			BhvrOverride
				TintGeom 1
				FadeInLength 20
				FadeOutLength 15
				PyrRotate 180 180 -15
				PositionOffset -0.5 0 0.0
				Scale .1 .1 .1
			End
			Geom FX_Tintable_Leaf_Wreath_ArmR
			LifeSpan 100
		End

		Event
			At LArmL
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\QuickGrow.bhvr
			BhvrOverride
				TintGeom 1
				PositionOffset -0.3 0 -0.1
				PyrRotate 0 195 -5
				Scale 0.1 0.1 0.1
				FadeInLength 20
				FadeOutLength 15
			End
			Geom FX_Tintable_Leaf_Wreath_ArmR
			LifeSpan 100
		End
	End

##########################################################

	Condition
		On time
		Time 47

		Event
			Type Local
			At Chest
			Part :Overgrowth_Streaks.part
			Part :Overgrowth_Flash.part
			Part :Overgrowth_InitGlow.part
			LifeSpan 75
		End
	End

##########################################################

	Condition
		On time
		Time 45

		Event
			Type Local
			At Chest
			ChildFX	:Custom_Overgrowth_Child.fx
		End
	End

##############################################################


End