#
FxInfo

	Flags DontSuppress

	LifeSpan 100


###########################################################

	Condition
		On 	Time
		Time	0
		Event
			Type	Local
			At	Root
			Sound 	DC_HeartOfDarkness_Attack_01 100 100 .8
		End
	End

	Condition
		On Time
		Time 0

		Event
			At Hips
			Type Local
			EName SwirlsAnchor
			LifeSpan 10
		End

	End

	Condition
		On Cycle
		Time 5

		Event
			At SwirlsAnchor
			Type Start
			EName SwirlsSmall
			Bhvr CustomizeablePowers\DarknessControl\HeartOfDarkness_Shrink.bhvr
			BhvrOverride
				TintGeom 1
			End
			Geom Tintable_DarkGrasp
			LifeSpan 25
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			Part CustomizeablePowers\DarknessControl\HeartOfDarkness_Boils_Shrinking.part
			Part CustomizeablePowers\DarknessControl\HeartOfDarkness_Light_Shrinking.part
			Part :DarkGrasp_ShadowCore_Growing.part
			LifeSpan 30
		End

	End

	Condition
		On Time
		Time 30

		Event
			At Root
			Type Start
			EName SwirlsAnchor2
			LifeSpan 20
		End

	End

	Condition
		Time 35

		Event
			At Chest
			Type Start
			EName Explode
			Part CustomizeablePowers\DarknessControl\Explosion_Shockwave_Chest.part
			Part CustomizeablePowers\DarknessControl\Explosion_Boils.part
			Part CustomizeablePowers\DarknessControl\HeartOfDarkness_Flash_Additive.part
			Part CustomizeablePowers\DarknessControl\HeartOfDarkness_Glow_Additive.part
			Part CustomizeablePowers\DarknessControl\HeartOfDarkness_Flash.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 48

		Event
			At Root
			Type Start
			BhvrOverride
				Spin 0 0.1000 0
			End
			Part :Dust_Ground_Circle.part
			LifeSpan 20
		End

		Event
			At Root
			Type Start
			Part :Explosion_Shockwave.part
			LifeSpan 30
		End

	End

	Condition
		On Cycle
		Time 7

		Event
			At SwirlsAnchor2
			Type Start
			EName SwirlsSmall
			Bhvr CustomizeablePowers\DarknessControl\HeartOfDarkness_Tornado.bhvr
			BhvrOverride
				TintGeom 1
			End
			Geom Tintable_DarkGrasp
			LifeSpan 10
		End

	End

End
