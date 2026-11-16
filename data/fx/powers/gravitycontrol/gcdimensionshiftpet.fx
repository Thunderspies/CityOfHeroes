#
FxInfo
Lifespan 60

###########################################################

	Condition
		On Time
		Time 0

		Event
			At Origin
			Type Local
			#Sound gravityzap2 80 80 0.75
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part POWERS\GravityControl\GCBigOrbits.part
			Part POWERS\GravityControl\DimensionSphere_InnerGlow.part
			Part POWERS\GravityControl\DimensionSphere_InnerGlow_Ground.part
			Part POWERS\GravityControl\DimensionSphere_Border_Vertical.part
			Part POWERS\GravityControl\DimensionSphere_InnerGlow_Add.part
			Part POWERS\GravityControl\DimensionSphere_Border_Vertical_Hilite.part
		End

		Event
			At Root
			Type PositOnly
			EName StormGeo
			BhvrOverride
				PyrRotate 0 0 20
				Scale 7.0 7.0 7.0
				Spin 0 -0.225 0
				Alpha 30
				FadeInLength 20
				FadeOutLength 20
				#TintGeom 1
				StartColor 185 230 255
			End
			Geom Tintable_ShadowField_Highlights
		End

		Event
			At Root
			Type PositOnly
			EName StormSwirls
			BhvrOverride
				PyrRotate 0 0 10
				Scale 6.9 6.9 6.9
				Spin 0 -0.2 0
				Alpha 30
				FadeInLength 20
				FadeOutLength 50
				#TintGeom 1
				StartColor 185 230 255
			End
			Geom Tintable_ShadowField
		End

	End

End
