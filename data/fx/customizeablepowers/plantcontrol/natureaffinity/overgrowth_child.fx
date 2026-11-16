FXinfo
Lifespan 150

##########################################################


	Condition
		on time
		time 0

		Event
			At root
			type Start
			Part :Overgrowth_Grass.part
			Lifespan 75
		End

		Event
			At root
			type Start
			BhvrOverride
				PositionOffset	0 5 0
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
			Part :WildGrowth_Leaves.part
			Lifespan 75
		End
	End

	Condition
		on time
		time 0
		repeat 8

		Event
			At Root
			Type Start
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				Scale		0.1 0.1 0.1
				Endscale	3 1 3
				PositionOffset	0 -1 0
				StartJitter	18 1 18
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
				StartJitter	18 1 18
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

	Condition
		on time
		time 0

		Event
			At Root
			Type Start
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				Scale		0.1 0.1 0.1
				Endscale	3 2 3
				PositionOffset	10 -1 10
				StartJitter	8 1 8
				PyrRotateJitter 0 180 0
				FadeInLength	120
				FadeOutLength	90
				Alpha		255
				PrimaryTint	80
				SecondaryTint	20
			End
			#geom FX_Tree_MedSitterA
			geom FX_Tree_PopShort
			geom FX_Tree_PopShortB
			Lifespan 45
		End

		Event
			At Root
			Type Start
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				Scale		0.1 0.1 0.1
				Endscale	3 2 3
				PositionOffset	-10 -1 10
				StartJitter	8 1 8
				PyrRotateJitter 0 180 0
				FadeInLength	120
				FadeOutLength	90
				Alpha		255
				PrimaryTint	80
				SecondaryTint	20
			End
			#geom FX_Tree_MedSitterA
			geom FX_Tree_PopShort
			geom FX_Tree_PopShortB
			Lifespan 45
		End

		Event
			At Root
			Type Start
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				Scale		0.1 0.1 0.1
				Endscale	3 2 3
				PositionOffset	10 -1 -10
				StartJitter	8 1 8
				PyrRotateJitter 0 180 0
				FadeInLength	120
				FadeOutLength	90
				Alpha		255
				PrimaryTint	80
				SecondaryTint	20
			End
			#geom FX_Tree_MedSitterA
			geom FX_Tree_PopShort
			geom FX_Tree_PopShortB
			Lifespan 45
		End

		Event
			At Root
			Type Start
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				Scale		0.1 0.1 0.1
				Endscale	3 2 3
				PositionOffset	10 -1 10
				StartJitter	8 1 8
				PyrRotateJitter 0 180 0
				FadeInLength	120
				FadeOutLength	90
				Alpha		255
				PrimaryTint	80
				SecondaryTint	20
			End
			geom FX_Tree_UrbanA
			geom FX_Tree_UrbanB
			Lifespan 35
		End

		Event
			At Root
			Type Start
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				Scale		0.1 0.1 0.1
				Endscale	3 2 3
				PositionOffset	-10 -1 -10
				StartJitter	8 1 8
				PyrRotateJitter 0 180 0
				FadeInLength	120
				FadeOutLength	90
				Alpha		255
				PrimaryTint	80
				SecondaryTint	20
			End
			geom FX_Tree_UrbanA
			geom FX_Tree_UrbanB
			Lifespan 35
		End

		Event
			At Root
			Type Start
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				Scale		0.1 0.1 0.1
				Endscale	3 2 3
				PositionOffset	10 -1 -10
				StartJitter	8 1 8
				PyrRotateJitter 0 180 0
				FadeInLength	120
				FadeOutLength	90
				Alpha		255
				PrimaryTint	80
				SecondaryTint	20
			End
			geom FX_Tree_UrbanA
			geom FX_Tree_UrbanB
			Lifespan 35
		End
	End



##############################################################


End