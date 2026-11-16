#
FxInfo

	Flags DontSuppress

	LifeSpan 90


###########################################################

Condition
	On 	Time
	Time	5
	Event
		Type	Local
		At	Root
		Sound 	SpiritStalkers_TerrifyingRoar_Attack 100 100 .8
	End
End

	Condition
		On Time
		Time 25

		Event
			At Head
			Type Local
			Ename Anchor
			BhvrOverride
				PositionOffset 0 -0.35 0.7
				#PyrRotate 0 -90 0
			End
		End

		Event
			At Anchor
			Type CREATE
  			Inherit	Rotation Position
  			Update	Rotation Position
			EName TunnelAnchor
			BhvrOverride
				#PositionOffset 0 -0.5 0
			End
			Part CustomizeablePowers\DarknessControl\TerrifyingRoar_Highlights.part
			Part CustomizeablePowers\DarknessControl\TerrifyingRoar_Core.part
		End

		Event
			At Head
			Type Local
			EName Prime
		End

		Event
			At Prime
			Type Local
			EName stuff
			Bhvr :TerrifyingRoar_Swirls.bhvr
			Geom Tintable_DarkGrasp_Flipped2
			LifeSpan 15
		End

	End

	Condition
		On Cycle
		Time 3

		Event
			At Prime
			Type Local
			EName stuff2
			Bhvr :TerrifyingRoar_Swirls.bhvr
			Geom Tintable_DarkGrasp_Flipped2
			LifeSpan 15
		End

	End

	Condition
		On Cycle
		Time 3

		Event
			At Prime
			Type Local
			EName stuff3
			Bhvr :TerrifyingRoar_Swirls.bhvr
			BhvrOverride
				Spin 0 0 0.15
			End
			Geom Tintable_DarkGrasp_Flipped
			LifeSpan 15
		End

	End

End
