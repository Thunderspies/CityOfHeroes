FXinfo
Lifespan 170



##########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	silence 100 100 .8
	End
End

	Condition
		on time
		time 0

		Event
			At root
			type Local
			ChildFX :Regrowth_Activation.fx
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
				FadeInLength 20
				FadeOutLength 15
				PyrRotate 180 -0 -15
				PositionOffset 0.5 0 0.0
				Scale .1 .1 .1
			End
			Geom FX_Leaf_Wreath_ArmR
			LifeSpan 100
		End

		Event
			At LArmR
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\QuickGrow.bhvr
			BhvrOverride
				PositionOffset 0.3 0 0.1
				PyrRotate 0 15 -5
				Scale 0.1 0.1 0.1
				FadeInLength 20
				FadeOutLength 15
			End
			Geom FX_Leaf_Wreath_ArmR
			LifeSpan 100
		End

		Event
			At LArmL
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\QuickGrow.bhvr
			BhvrOverride
				FadeInLength 20
				FadeOutLength 15
				PyrRotate 180 180 -15
				PositionOffset -0.5 0 0.0
				Scale .1 .1 .1
			End
			Geom FX_Leaf_Wreath_ArmR
			LifeSpan 100
		End

		Event
			At LArmL
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\QuickGrow.bhvr
			BhvrOverride
				PositionOffset -0.3 0 -0.1
				PyrRotate 0 195 -5
				Scale 0.1 0.1 0.1
				FadeInLength 20
				FadeOutLength 15
			End
			Geom FX_Leaf_Wreath_ArmR
			LifeSpan 100
		End
	End

##########################################################

End