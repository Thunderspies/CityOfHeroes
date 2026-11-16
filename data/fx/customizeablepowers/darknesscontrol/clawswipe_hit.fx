#
FxInfo

	LifeSpan 150


###########################################################

Condition
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 1
		Sound Dog_ClawHit_01 35 35 .5
		Sound Dog_ClawHit_02 35 35 .5
		Sound Dog_ClawHit_03 35 35 .5
	End

End

	Condition
		On Time
		Time 0

		Event
			At Hips
			Type Local
			EName SwirlsSmall
			Bhvr CustomizeablePowers\DarknessControl\ClawRake_Scale.bhvr
			Geom Tintable_DarkGrasp
			LifeSpan 7
		End

		Event
			At Chest
			Type Start
			Part :Head_Shockwave.part
			LifeSpan 10
		End

		Event
			At Chest
			Type Start
			Part CustomizeablePowers\DarknessControl\ShadowPunch_Flash_Initial.part
			Part CustomizeablePowers\DarknessControl\ShadowPunch_Flash_Initial_Additive.part
			Part CustomizeablePowers\DarknessControl\ShadowPunch_Glow_Additive.part
			Part CustomizeablePowers\DarknessControl\ShadowPunch_Flash.part
			LifeSpan 5
		End

	End

	Condition
		On Time
		Time 1

		Event
			At Chest
			Type Start
			Part CustomizeablePowers\DarknessControl\ShadowPunch_Smoke_Lingering.part
			LifeSpan 5
		End

	End

	Condition
		On Time
		Time 8

		Event
			At Hips
			Type Local
			EName SwirlsSmall
			Bhvr CustomizeablePowers\DarknessControl\ClawRake_Scale.bhvr
			BhvrOverride
				PyrRotate 20 0 0
				PyrRotateJitter 10 10 10
				Spin 0 0.5 0
			End
			Geom Tintable_DarkGrasp
			LifeSpan 7
		End

	End

End
