#
FxInfo

	LifeSpan 150


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		SoundNoRepeat 2
		Sound 	DC_Haunt_Spirit_Hit_01 50 50 .8
		Sound 	DC_Haunt_Spirit_Hit_02 50 50 .8
		Sound 	DC_Haunt_Spirit_Hit_03 50 50 .8
		Sound 	DC_Haunt_Spirit_Hit_04 50 50 .8
		Sound 	DC_Haunt_Spirit_Hit_05 50 50 .8
	End
End


	Condition
		On Time
		Time 0

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

End
