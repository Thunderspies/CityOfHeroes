#
FxInfo

	LifeSpan 150


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type PositOnly
			EName Anchor
		End

		Event
			Ename PRIME
			At T_Chest
			Type PositOnly
			LookAt Anchor
			Magnet Anchor
			PMagnet Anchor
			Bhvr CustomizeablePowers\BioArmor\Siphon_Geo.bhvr
			BhvrOverride
				#PositionOffset 0 3 5
				#PyrRotateJitter 0 0 90
				FadeOutLength	10
				TintGeom 1
				Alpha 255
			End
#			Geom FX_BioArmor_Tendril_01
			Geom FX_BioArmor_Tendril_02
#			Geom FX_BioArmor_Tendril_03
#			Geom FX_BioArmor_Tendril_04
#			Geom FX_BioArmor_Tendril_05
#			Geom FX_BioArmor_Tendril_06
			LifeSpan 5
		End

	End

	Condition
		On Time
		Time 5

		Event
			At Chest
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Hit_Streaks.part
			Part :Hit_Flash.part
			LifeSpan 2
		End

		Event
			At Chest
			Type PositOnly
			LookAt T_Chest
			Magnet T_chest
			PMagnet Anchor
			Bhvr CustomizeablePowers\BioArmor\Siphon_Geo.bhvr
			BhvrOverride
				#PositionOffset 0 3 5
				Scale		.3 .3 .007
				Endscale	.07 .07 .027
				FadeinLength	0
				TintGeom 1
				Alpha 255
			End
#			Geom FX_BioArmor_Tendril_01
			Geom FX_BioArmor_Tendril_02
#			Geom FX_BioArmor_Tendril_03
#			Geom FX_BioArmor_Tendril_04
#			Geom FX_BioArmor_Tendril_05
#			Geom FX_BioArmor_Tendril_06
			LifeSpan 5
		End

	End

End
