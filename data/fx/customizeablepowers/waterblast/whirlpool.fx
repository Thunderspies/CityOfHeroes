#
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Start
			EName WhirlAnchor
			BhvrOverride
				PositionOffset 0 0 0
			End
		End

		Event
			At WhirlAnchor
			Type Local
			EName WhirlAdd
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Whirl_Splash_OuterInit.part
			Part :Whirl_Splash_OuterInit2.part
			Part :Whirl_Splash_OuterInit_Flat.part
			Part :Whirl_Splash_OuterInit_Flat2.part
			LifeSpan 20
		End
	End

	Condition
		On Time
		Time 20

		Event
			At WhirlAnchor
			Type Local
			EName WhirlAdd
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				Spin 0 -0.012 0
			End
			Part :Whirl_Splash_OuterInit.part
			Part :Whirl_Splash_OuterInit2.part
			Part :Whirl_Splash_OuterInit_Flat.part
			Part :Whirl_Splash_OuterInit_Flat2.part
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 40

		Event
			At WhirlAnchor
			Type Local
			EName WhirlAdd
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				Spin 0 -0.030 0
			End
			Part :Whirl_Splash_OuterInit.part
			Part :Whirl_Splash_OuterInit2.part
			Part :Whirl_Splash_OuterInit_Flat.part
			Part :Whirl_Splash_OuterInit_Flat2.part
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 60

		Event
			At WhirlAnchor
			Type Local
			EName WhirlAdd
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				Spin 0 -0.050 0
			End
			Part :Whirl_Splash_OuterInit.part
			Part :Whirl_Splash_OuterInit2.part
			Part :Whirl_Splash_OuterInit_Flat.part
			Part :Whirl_Splash_OuterInit_Flat2.part
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 45

		Event
			At WhirlAnchor
			Type Local
			EName Whirl
			Bhvr :Whirlpool_Scale.bhvr
			BhvrOverride
				PyrRotateJitter 0 180 0
				Spin 0 -0.007 0
			End
		End

		Event
			At Whirl
			Type Local
			EName WhirlAdd
			Bhvr :Whirlpool_Fade.bhvr
			BhvrOverride
				TintGeom 1
				Alpha 160
			End
			Geom FX_WaterWhirl_Add
		End

		Event
			At Whirl
			Type Local
			EName WhirlAlpha
			Bhvr :Whirlpool_Fade.bhvr
			BhvrOverride
				PositionOffset 0 -0.1 0
				Alpha 160
				TintGeom 1
			End
			Geom FX_WaterWhirl
		End

	End

	Condition
		On Time
		Time 80

		Event
			At WhirlAdd
			Type PositOnly
			EName WhirlAdd
			BhvrOverride
				Spin 0 -0.050 0
			End
			Part :Whirl_Splash_01.part
			Part :Whirl_Splash_02.part
			Part :Whirl_Splash_03.part
		End

		Event
			At WhirlAnchor
			Type Local
			EName WhirlAdd
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				Spin 0 -0.030 0
			End
			Part :Whirl_Splash_Outer.part
			Part :Whirl_Splash_Outer2.part
			Part :Whirl_Splash_Outer3.part
			Part :Whirl_Splash_Outer_Flat.part
			Part :Whirl_Splash_Outer_Flat2.part
			#LifeSpan 20
		End

	End

	Condition
		On Time
		Time 150

		Event
			At WhirlAdd
			Type PositOnly
			EName WhirlAdd
			BhvrOverride
				Spin 0 -0.050 0
			End
			Part :Whirl_Splash_01.part
			Part :Whirl_Splash_02.part
			#Part :Whirl_Splash_03.part
		End

	End

End
