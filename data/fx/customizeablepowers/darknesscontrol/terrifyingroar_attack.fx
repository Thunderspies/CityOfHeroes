#
FxInfo

	Flags DontSuppress

	LifeSpan 40


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	Dog_TerrifyingRoar_01 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Head
			Type Local
			EName TunnelAnchor
			BhvrOverride
				PositionOffset 0 -0.5 0
			End
			Part CustomizeablePowers\DarknessControl\TerrifyingRoar_Highlights.part
			Part :TerrifyingRoar_Core.part
		End

		Event
			At Head
			Type Local
			EName Prime
			Bhvr CustomizeablePowers\DarknessControl\TerrifyingRoar_Swirls.bhvr
			Geom Tintable_DarkGrasp_Flipped2
			LifeSpan 15
		End

	End

	Condition
		On Cycle
		Time 8

		Event
			At Head
			Type Local
			EName Prime
			Bhvr CustomizeablePowers\DarknessControl\TerrifyingRoar_Swirls.bhvr
			Geom Tintable_DarkGrasp_Flipped2
			LifeSpan 10
		End

	End

	Condition
		On Cycle
		Time 8

		Event
			At Head
			Type Local
			EName Prime2
			Bhvr CustomizeablePowers\DarknessControl\TerrifyingRoar_Swirls.bhvr
			BhvrOverride
				Spin 0 0 0.15
			End
			Geom Tintable_DarkGrasp_Flipped
			LifeSpan 10
		End

	End

End
