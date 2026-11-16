#
FxInfo

	LifeSpan 150


###########################################################

	Condition
		On Time
		Time 0
	Event
		Type	Local
		At	Root
		SoundNoRepeat 2
		Sound 	DC_Haunt_Spirit_Hit_01 100 100 .8
		Sound 	DC_Haunt_Spirit_Hit_02 100 100 .8
		Sound 	DC_Haunt_Spirit_Hit_03 100 100 .8
		Sound 	DC_Haunt_Spirit_Hit_04 100 100 .8
		Sound 	DC_Haunt_Spirit_Hit_05 100 100 .8
	End


		Event
			At Head
			Type Local
			EName ConeOffset
			BhvrOverride
				Scale 2 2 2
			End
			Geom FX_ConeOffsets
		End

		Event
			At Head
			Type Start
			Part CustomizeablePowers\DarknessControl\Head_Shockwave_Small.part
			LifeSpan 10
		End

		Event
			At Head
			Type Start
			Part CustomizeablePowers\DarknessControl\ShadowPunch_Flash_Small.part
			Part CustomizeablePowers\DarknessControl\ShadowPunch_Glow_Additive_Small.part
			Part CustomizeablePowers\DarknessControl\ShadowPunch_Flash_Initial_Additive_Small.part
			Part CustomizeablePowers\DarknessControl\ShadowPunch_Flash_Initial_Small.part
			LifeSpan 5
		End

	End

	Condition
		On Time
		Time 1

		Event
			At Head
			Type Start
			Part CustomizeablePowers\DarknessControl\ShadowPunch_Smoke_Lingering_Small.part
			LifeSpan 5
		End

	End

End
