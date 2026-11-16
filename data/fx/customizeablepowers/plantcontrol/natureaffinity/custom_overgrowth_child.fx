#                                                          
FxInfo

	LifeSpan 150


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Start
			Part CustomizeablePowers\PlantControl\NatureAffinity\Tintable_Overgrowth_Grass.part
			LifeSpan 75
		End

		Event
			At Root
			Type Start
			Part CustomizeablePowers\PlantControl\NatureAffinity\Tintable_Overgrowth_GrassRound.part
			#Part :LifeSpore_Tintable_Grass.part
			LifeSpan 1
		End

		Event
			At Root
			Type Start
			BhvrOverride
				PositionOffset 0 5 0
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
			Part CustomizeablePowers\PlantControl\NatureAffinity\Tintable_WildGrowth_Leaves.part
			#Part :WildGrowth_Leaves.part
			LifeSpan 75
		End

	End

	Condition
		On Time
		Time 0
		Repeat 8

		Event
			At Root
			Type Start
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom 1
				Scale 0.1 0.1 0.1
				EndScale 3 1 3
				PositionOffset 0 -1 0
				StartJitter 18 1 18
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
				StartJitter 18 1 18
				PyrRotateJitter 0 180 0
				FadeInLength 120
				Alpha 255
				PrimaryTint 80
				SecondaryTint 20
			End
			#Geom FX_Bush_AzMedA
			#Geom FX_Bush_AzSmBFol
			#Geom FX_Bush_AzLgAFol
			#Geom FX_Bush_AzXLgAFol
			#Geom FX_Bush_AzSmDDFol
			#Geom FX_Bush_ShrubMedA
			#Geom FX_Bush_ShrubSmA
			#Geom FX_Bush_PrivSmBFol
			#Geom FX_Bush_PrivSmAFol
			#Geom FX_Bush_WillowLongA
			#Geom FX_Bush_WillowSmA
			#Geom FX_Bush_AzLongA
			#Geom FX_Bush_AzSmA
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

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Start
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom 1
				Scale 0.1 0.1 0.1
				EndScale 3 2 3
				PositionOffset 10 -1 10
				StartJitter 8 1 8
				PyrRotateJitter 0 180 0
				FadeInLength 120
				FadeOutLength 90
				Alpha 255
				PrimaryTint 80
				SecondaryTint 20
			End
			#Geom FX_Tree_MedSitterA
			Geom FX_Tintable_Tree_PopShort
			Geom FX_Tintable_Tree_PopShortB
			LifeSpan 45
		End

		Event
			At Root
			Type Start
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom 1
				Scale 0.1 0.1 0.1
				EndScale 3 2 3
				PositionOffset -10 -1 10
				StartJitter 8 1 8
				PyrRotateJitter 0 180 0
				FadeInLength 120
				FadeOutLength 90
				Alpha 255
				PrimaryTint 80
				SecondaryTint 20
			End
			#Geom FX_Tree_MedSitterA
			Geom FX_Tintable_Tree_PopShort
			Geom FX_Tintable_Tree_PopShortB
			LifeSpan 45
		End

		Event
			At Root
			Type Start
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom 1
				Scale 0.1 0.1 0.1
				EndScale 3 2 3
				PositionOffset 10 -1 -10
				StartJitter 8 1 8
				PyrRotateJitter 0 180 0
				FadeInLength 120
				FadeOutLength 90
				Alpha 255
				PrimaryTint 80
				SecondaryTint 20
			End
			#Geom FX_Tree_MedSitterA
			Geom FX_Tintable_Tree_PopShort
			Geom FX_Tintable_Tree_PopShortB
			LifeSpan 45
		End

		Event
			At Root
			Type Start
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom 1
				Scale 0.1 0.1 0.1
				EndScale 3 2 3
				PositionOffset 10 -1 10
				StartJitter 8 1 8
				PyrRotateJitter 0 180 0
				FadeInLength 120
				FadeOutLength 90
				Alpha 255
				PrimaryTint 80
				SecondaryTint 20
			End
			Geom FX_Tintable_Tree_UrbanA
			Geom FX_Tintable_Tree_UrbanB
			LifeSpan 35
		End

		Event
			At Root
			Type Start
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom 1
				Scale 0.1 0.1 0.1
				EndScale 3 2 3
				PositionOffset -10 -1 -10
				StartJitter 8 1 8
				PyrRotateJitter 0 180 0
				FadeInLength 120
				FadeOutLength 90
				Alpha 255
				PrimaryTint 80
				SecondaryTint 20
			End
			Geom FX_Tintable_Tree_UrbanA
			Geom FX_Tintable_Tree_UrbanB
			LifeSpan 35
		End

		Event
			At Root
			Type Start
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom 1
				Scale 0.1 0.1 0.1
				EndScale 3 2 3
				PositionOffset 10 -1 -10
				StartJitter 8 1 8
				PyrRotateJitter 0 180 0
				FadeInLength 120
				FadeOutLength 90
				Alpha 255
				PrimaryTint 80
				SecondaryTint 20
			End
			Geom FX_Tintable_Tree_UrbanA
			Geom FX_Tintable_Tree_UrbanB
			LifeSpan 35
		End

	End

End
