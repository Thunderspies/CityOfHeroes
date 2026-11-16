#
FxInfo

	LifeSpan 25


###########################################################

Condition
	On 	Time
	Time	23
	Event
		Type	Local
		At	Root
		Sound 	DD_EarthShatter_Attack_01 300 100 .8
	End
End

	Condition
		On Time
		Time 23

		Event
			At FootR
			Type StartPositOnly
			EName DustAnchor
			BhvrOverride
				PositionOffset 0 30 0
			End
		End

		Event
			At Mystic
			Type StartPositOnly
			Bhvr behaviors/CameraShake_Strong20.bhvr
			BhvrOverride
				PositionOffset 0.0 -1.0 0.0
			End
			Part :Earthshatter_Dust.part
			Part :Earthshatter_Shockwave.part
			Part :Earthshatter_Shockwave_Fill.part
			Part :Earthshatter_Flash.part
			Part :Earthshatter_Flash_Additive.part
			LifeSpan 8
		End

	End

End
