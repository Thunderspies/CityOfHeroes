FXinfo

	Condition
		On Time
		Time 0

		Event
			Ename RibbonLeg
			At LLegR
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\SlowGrow.bhvr
			BhvrOverride
				TintGeom 1
				FadeInLength 20
				FadeOutLength 15
				PositionOffset -0.1 -1.7 -0.5
				PyrRotate 0 -90 05
				EndScale 1.2 1 1.2
			End
			Geom FX_Tintable_Ivy_Wreath_LegL
			#LifeSpan 100
		End
	End

	Condition
		On Time
		Time 5

		Event
			Ename RibbonLeg
			At ULegR
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\SlowGrow.bhvr
			BhvrOverride
				TintGeom 1
				FadeInLength 20
				FadeOutLength 15
				PositionOffset 0 -1.3 0.2
				PyrRotate 0 180 -15
				EndScale 1.2 1 1.2
			End
			Geom FX_Tintable_Ivy_Wreath_LegL
			#LifeSpan 100
		End
	End

	Condition
		On Time
		Time 10

		Event
			Ename RibbonChest
			At Chest
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\SlowGrow.bhvr
			BhvrOverride
				TintGeom 1
				FadeInLength 20
				FadeOutLength 15
				PositionOffset -0.2 0.2 0
				PyrRotate 15 0 -20
				EndScale 1 0.9 0.8
			End
			Geom FX_Tintable_Ivy_Ring04
			#LifeSpan 100
		End
	End

	Condition
		On Time
		Time 15

		Event
			At LArmR
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\SlowGrow.bhvr
			BhvrOverride
				TintGeom 1
				FadeInLength 20
				FadeOutLength 15
				PyrRotate 180 -0 -15
				PositionOffset 0.5 0 0.0
				Scale .1 .1 .1
			End
			Geom FX_Tintable_Ivy_Wreath_ArmR
			LifeSpan 100
		End

		Event
			At LArmR
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\SlowGrow.bhvr
			BhvrOverride
				TintGeom 1
				PositionOffset 0.3 0 0.1
				PyrRotate 0 15 -5
				Scale 0.1 0.1 0.1
				FadeInLength 20
				FadeOutLength 15
			End
			Geom FX_Tintable_Ivy_Wreath_ArmR
			LifeSpan 100
		End

		Event
			At LArmL
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\SlowGrow.bhvr
			BhvrOverride
				TintGeom 1
				FadeInLength 20
				FadeOutLength 15
				PyrRotate 180 180 -15
				PositionOffset -0.5 0 0.0
				Scale .1 .1 .1
			End
			Geom FX_Tintable_Ivy_Wreath_ArmR
			LifeSpan 100
		End

		Event
			At LArmL
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\SlowGrow.bhvr
			BhvrOverride
				TintGeom 1
				PositionOffset -0.3 0 -0.1
				PyrRotate 0 195 -5
				Scale 0.1 0.1 0.1
				FadeInLength 20
				FadeOutLength 15
			End
			Geom FX_Tintable_Ivy_Wreath_ArmR
			LifeSpan 100
		End


	End

End