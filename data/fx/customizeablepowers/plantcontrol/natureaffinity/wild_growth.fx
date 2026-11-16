FXinfo
Lifespan 150



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


	Condition
		on time
		time 30

		Event
			At root
			type Start
			Part :WildGrowth_Grass.part
			Lifespan 75
		End

		Event
			At root
			type Start
			BhvrOverride
				PositionOffset	0 0 0
			End
			Part :WildGrowth_Glow.part
			Lifespan 75
		End

		Event
			At root
			type Start
			BhvrOverride
				PositionOffset	0 2 0
			End
			Part :WildGrowth_Specks.part
			Lifespan 75
		End

		Event
			At root
			type Start
			BhvrOverride
				PositionOffset	0 0 0
			End
			Part :WildGrowth_Leaves.part
			Lifespan 25
		End
	End

	Condition
		on time
		time 30
		repeat 10

		Event
			Ename ButtAnchor
			At Root
			Type Start
		End

		Event
			Ename Buttflies
			at ButtAnchor
			Type Local
			BhvrOverride
				PositionOffset 0 5 0
				StartJitter 15 2 15
			End
			ChildFX :Butterflies_Child.fx
		End

		Event
			At Root
			Type Start
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				Scale		0.1 0.1 0.1
				Endscale	3 1.35 3
				PositionOffset	0 -1 0
				StartJitter	15 1 15
				PyrRotateJitter 0 180 0
				FadeInLength	120
				Alpha		255
				PrimaryTint	80
				SecondaryTint	20
			End
			geom FX_Bush_AzXLgAFol
			geom FX_Bush_AzSmDDFol

			Lifespan 75
		End

		Event
			At Root
			Type Start
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				Scale		0.1 0.1 0.1
				Endscale	3 2 3
				PositionOffset	0 -1 0
				StartJitter	15 1 15
				PyrRotateJitter 0 180 0
				FadeInLength	120
				Alpha		255
				PrimaryTint	80
				SecondaryTint	20
			End
			geom FX_Bush_AzMedA
			geom FX_Bush_AzSmBFol
			geom FX_Bush_AzLgAFol
			geom FX_Bush_AzXLgAFol
			geom FX_Bush_AzSmDDFol
			geom FX_Bush_ShrubMedA
			geom FX_Bush_ShrubSmA
			geom FX_Bush_PrivSmBFol
			geom FX_Bush_PrivSmAFol
			geom FX_Bush_WillowLongA
			geom FX_Bush_WillowSmA
			geom FX_Bush_AzLongA
			geom FX_Bush_AzSmA
	#		geom FX_Tintable_Bush_AzMedA
	#		geom FX_Tintable_Bush_AzSmBFol
	#		geom FX_Tintable_Bush_AzLgAFol
	#		geom FX_Tintable_Bush_PrivLgAFol
	#		geom FX_Tintable_Bush_PrivLgBFol
	#		geom FX_Tintable_Bush_AzXLgAFol
	#		geom FX_Tintable_Bush_AzSmDDFol
	#		geom FX_Tintable_Bush_ShrubMedA
	#		geom FX_Tintable_Bush_ShrubSmA
	#		geom FX_Tintable_Bush_PrivSmBFol
	#		geom FX_Tintable_Bush_PrivSmAFol
			Lifespan 75
		End
	End


##############################################################


End