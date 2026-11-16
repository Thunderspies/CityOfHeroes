#
FxInfo

	LifeSpan 150



##########################################################
Condition
	On 	Time
	Time	16
	Event
		Type	Local
		At	Root
		Sound 	NA_WildGrowth_01 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			ChildFx :Custom_Regrowth_Activation.fx
		End

	End

	Condition
		On Time
		Time 15

		Event
			At LArmR
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\QuickGrow.bhvr
			BhvrOverride
				TintGeom 1
				PositionOffset 0.5 0 0
				PyrRotate 180 0 -15
				Scale 0.1 0.1 0.1
				FadeInLength 20
				FadeOutLength 15
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

	Condition
		On Time
		Time 30

		Event
			At Root
			Type Start
			Part CustomizeablePowers\PlantControl\NatureAffinity\Tintable_WildGrowth_Grass.part
			#Part :WildGrowth_Grass.part
			LifeSpan 75
		End

		Event
			At Root
			Type Start
			BhvrOverride
				PositionOffset 0 0 0
			End
			Part :WildGrowth_Glow.part
			LifeSpan 75
		End

		Event
			At Root
			Type Start
			BhvrOverride
				PositionOffset 0 2 0
			End
			Part :WildGrowth_Specks.part
			LifeSpan 75
		End

		Event
			At Root
			Type Start
			BhvrOverride
				PositionOffset 0 0 0
			End
			Part CustomizeablePowers\PlantControl\NatureAffinity\Tintable_WildGrowth_Leaves.part
			#Part :WildGrowth_Leaves.part
			LifeSpan 25
		End

	End

	Condition
		On Time
		Time 30
		Repeat 10

		Event
			At Root
			Type Start
			EName ButtAnchor
		End

		Event
			At ButtAnchor
			Type Local
			ChildFx :Butterflies_Child.fx
			EName Buttflies
			BhvrOverride
				PositionOffset 0 5 0
				StartJitter 15 2 15
			End
		End

		Event
			At Root
			Type Start
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom 1
				Scale 0.1 0.1 0.1
				EndScale 3 1.35 3
				PositionOffset 0 -1 0
				StartJitter 15 1 15
				PyrRotateJitter 0 180 0
				FadeInLength 120
				Alpha 255
				PrimaryTint 80
				SecondaryTint 20
			End
			Geom FX_Tintable_Bush_AzXLgAFol
			Geom FX_Tintable_Bush_AzSmDDFol
			LifeSpan 75
		End

		Event
			At Root
			Type Start
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom 1
				Scale 0.1 0.1 0.1
				EndScale 3 2 3
				PositionOffset 0 -1 0
				StartJitter 15 1 15
				PyrRotateJitter 0 180 0
				FadeInLength 120
				Alpha 255
				PrimaryTint 80
				SecondaryTint 20
			End
#			Geom FX_Bush_AzMedA
#			Geom FX_Bush_AzSmBFol
#			Geom FX_Bush_AzLgAFol
#			Geom FX_Bush_AzXLgAFol
#			Geom FX_Bush_AzSmDDFol
#			Geom FX_Bush_ShrubMedA
#			Geom FX_Bush_ShrubSmA
#			Geom FX_Bush_PrivSmBFol
#			Geom FX_Bush_PrivSmAFol
#			Geom FX_Bush_WillowLongA
#			Geom FX_Bush_WillowSmA
#			Geom FX_Bush_AzLongA
#			Geom FX_Bush_AzSmA
			Geom FX_Tintable_Bush_AzMedA
			Geom FX_Tintable_Bush_AzSmBFol
			Geom FX_Tintable_Bush_AzLgAFol
			Geom FX_Tintable_Bush_PrivLgAFol
			Geom FX_Tintable_Bush_PrivLgBFol
			Geom FX_Tintable_Bush_AzXLgAFol
			Geom FX_Tintable_Bush_AzSmDDFol
			Geom FX_Tintable_Bush_ShrubMedA
			Geom FX_Tintable_Bush_ShrubSmA
			Geom FX_Tintable_Bush_PrivSmBFol
			Geom FX_Tintable_Bush_PrivSmAFol
			LifeSpan 75
		End

	End

End
