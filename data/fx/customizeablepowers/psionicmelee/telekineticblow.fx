#
FxInfo

	LifeSpan 150


###########################################################
// Condition
// 	On 	Time
// 	Time	0
// 	Event
// 		Type	Local
// 		At	Root
// 		Sound 	PsionicMelee_TelekineticBlow 100 100 .8
// 	End
// End

	Condition
		On Time
		Time 1

		Event
			At Head
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0.3 0
			End
			Part :PsioHeadRings01.part
			Part :PsioHeadRings02.part
			Part :PsioHeadGlow.part
			Part CustomizeablePowers\PsionicAssault\InfuseLight2.part
			Lifespan 10
		End

		Event
			At Head
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0.3 0
			End
			Part :EnergyCoreFlash.part
			Part :InfuseLight2.part
			Lifespan 20
		End
	End

	Condition
		On Time
		Time 5

		Event
			At Root
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0 1
			End
			Part :GroundGlow.part
			Part :GroundRings.part
			Part :GroundRings_Solid.part
			Lifespan 15
		End
	End

	Condition
		On Time
		Time 10

		Event
			At Root
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0 1
			End
		#	Part :GroundRings_Hilite.part
			Lifespan 8
		End
	End


	Condition
		On Time
		Time 18

		Event
			At Head
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0.3 0
			End
		#	Part :PsioHeadFlash.part
			Lifespan 15
		End

		Event
			Ename FistAnchor
			At Root
			Type Local
			Bhvr CustomizeablePowers\PsionicMelee\UpStrike.bhvr
			BhvrOverride
				TintGeom 1
				PositionOffset 2 0 3
				PyrRotate -140 125 -60
				Alpha 255
			End
			Geom FX_GlowFist_Curve_R
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 23

		Event
			Ename GlowAnchor
			At Root
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0.7 1.5 3
			End
		#	Geom Geo_FX_Testsphere
			Part CustomizeablePowers\PsionicMelee\FistGlow_Stretch_Blur.part
			Part CustomizeablePowers\PsionicMelee\FistGlow_Stretch.part
			Lookat GlowTarget
			Pother GlowTarget
			Pmagnet GlowTarget
			LifeSpan 5
		End

		Event
			Ename GlowTarget
			At Root
			type Local
			BhvrOverride
				PositionOffset  0.5 3.5 5.8
			End
		#	Geom Geo_FX_Testsphere
		End

	End


End
