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
				FadeInLength 20
				FadeOutLength 15
				PositionOffset 0 -1.7 -0.8
				PyrRotate 0 -90 05
				EndScale 1.6 1 1.6
			End
			Geom FX_Ivy_Wreath_LegL
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
				FadeInLength 20
				FadeOutLength 15
				PositionOffset -0.1 -1.3 0.2
				PyrRotate 0 180 -15
				EndScale 1.6 1 1.6
			End
			Geom FX_Ivy_Wreath_LegL
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
				FadeInLength 20
				FadeOutLength 15
				PositionOffset -0.5 0.25 -0.15
				PyrRotate 15 0 -20
				Scale	1 0.7 1
				EndScale 1.5 1 1.4
			End
			Geom FX_Ivy_Ring04
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
				FadeInLength 20
				FadeOutLength 15
				PyrRotate 180 -0 -15
				PositionOffset 0.5 0 0.0
				Scale 0.8 0.8 0.8
				EndScale 1.4 1.4 1.4
			End
			Geom FX_Ivy_Wreath_ArmR
			LifeSpan 100
		End

		Event
			At LArmR
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\SlowGrow.bhvr
			BhvrOverride
				PositionOffset 0.3 0 0.1
				PyrRotate 0 15 -5
				FadeInLength 20
				FadeOutLength 15
				Scale 0.8 0.8 0.8
				EndScale 1.4 1.4 1.4
			End
			Geom FX_Ivy_Wreath_ArmR
			LifeSpan 100
		End

		Event
			At LArmL
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\SlowGrow.bhvr
			BhvrOverride
				FadeInLength 20
				FadeOutLength 15
				PyrRotate 180 180 -15
				PositionOffset -0.5 0 0.0
				Scale 0.8 0.8 0.8
				EndScale 1.4 1.4 1.4
			End
			Geom FX_Ivy_Wreath_ArmR
			LifeSpan 100
		End

		Event
			At LArmL
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\SlowGrow.bhvr
			BhvrOverride
				PositionOffset -0.3 0 -0.1
				PyrRotate 0 195 -5
				FadeInLength 20
				FadeOutLength 15
				Scale 0.8 0.8 0.8
				EndScale 1.4 1.4 1.4
			End
			Geom FX_Ivy_Wreath_ArmR
			LifeSpan 100
		End


	End

End