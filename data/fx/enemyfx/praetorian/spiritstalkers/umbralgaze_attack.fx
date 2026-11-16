#
FxInfo

	Flags DontSuppress

	LifeSpan 150


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	DC_FearsomeStare_Attack_01 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Head
			Type Local
			Ename Anchor
			BhvrOverride
				#PositionOffset 0 -0.35 0.7
				#PyrRotate 0 -90 0
			End
		End

		Event
			At Anchor
			Type CREATE
  			Inherit	Rotation Position
  			Update	Rotation Position
			Bhvr behaviors/GenericParticleFade.bhvr
			Part CustomizeablePowers\DarknessControl\FearsomeStare_Trail_Large.part
			Part CustomizeablePowers\DarknessControl\FearsomeStare_Trail_Large_Solid.part
			LifeSpan 50
		End

		Event
			At Head
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part CustomizeablePowers\DarknessControl\FearsomeStare_Trail_Large_Highlights.part
			LifeSpan 40
		End

	End

	Condition
		On Time
		Time 5

		Event
			At Head
			Type Local
			Bhvr CustomizeablePowers\DarknessControl\FearsomeStare_Tunnel.bhvr
			Geom Tintable_DarkGrasp_Flipped
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 10

		Event
			At Head
			Type Local
			Bhvr CustomizeablePowers\DarknessControl\FearsomeStare_Tunnel.bhvr
			Geom Tintable_DarkGrasp_Flipped
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 15

		Event
			At Head
			Type Local
			Bhvr CustomizeablePowers\DarknessControl\FearsomeStare_Tunnel.bhvr
			Geom Tintable_DarkGrasp_Flipped
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 20

		Event
			At Head
			Type Local
			Bhvr CustomizeablePowers\DarknessControl\FearsomeStare_Tunnel.bhvr
			Geom Tintable_DarkGrasp_Flipped
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 25

		Event
			At Head
			Type Local
			Bhvr CustomizeablePowers\DarknessControl\FearsomeStare_Tunnel.bhvr
			Geom Tintable_DarkGrasp_Flipped
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 30

		Event
			At Head
			Type Local
			Bhvr CustomizeablePowers\DarknessControl\FearsomeStare_Tunnel.bhvr
			Geom Tintable_DarkGrasp_Flipped
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 35

		Event
			At Head
			Type Local
			Bhvr CustomizeablePowers\DarknessControl\FearsomeStare_Tunnel.bhvr
			Geom Tintable_DarkGrasp_Flipped
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 20

		Event
			At Head
			Type Local
			EName EyeAnchor
			Bhvr behaviors/GenericParticleFade.bhvr
			Geom CircleOfThorns
		End

		Event
			AltPiv 1
			At EyeAnchor
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset -0.08 0.10 0.35
			End
			Part :Jaunt_Flash_Head_Big.part
			Part :Jaunt_Glow_Head_Big.part
		End

		Event
			AltPiv 2
			At EyeAnchor
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0.08 0.10 0.35
			End
			Part :Jaunt_Flash_Head_Big.part
			Part :Jaunt_Glow_Head_Big.part
		End

	End

End
