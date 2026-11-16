#
FxInfo

	LifeSpan 200


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
			At Chest
			Type Local
			EName Anchor2
		End

		Event
			At T_Chest
			Type PositOnly
			EName PRIME
			LookAt Anchor
			Magnet Anchor
			PMagnet Anchor
			Bhvr CustomizeablePowers\BioArmor\Siphon_Geo.bhvr
			BhvrOverride
				#PositionOffset 0 3 5
				#PyrRotateJitter 0 0 90
				Scale 0.105 0.105 0.007
				EndScale 0.45 0.45 0.027
				TintGeom 1
				Alpha 255
				FadeOutLength 10
			End
			#Geom FX_BioArmor_Tendril_01
			#Geom FX_BioArmor_Tendril_02
			#Geom FX_BioArmor_Tendril_03
			#Geom FX_BioArmor_Tendril_04
			Geom FX_BioArmor_Tendril_05
			#Geom FX_BioArmor_Tendril_06
			LifeSpan 5
		End

		Event
			At T_Chest
			Type PositOnly
			EName PRIME2
			LookAt Anchor
			Magnet Anchor
			PMagnet Anchor
			Bhvr CustomizeablePowers\BioArmor\Siphon_Geo.bhvr
			BhvrOverride
				#PositionOffset 0 3 5
				#PyrRotateJitter 0 0 90
				Scale .105 .105 .007
				EndScale .45 .45 .027
				FadeOutLength 10
				TintGeom 1
				Alpha 255
			End
			Geom FX_BioArmor_Tendril_01
			#Geom FX_BioArmor_Tendril_02
			#Geom FX_BioArmor_Tendril_03
			#Geom FX_BioArmor_Tendril_04
			#Geom FX_BioArmor_Tendril_05
			#Geom FX_BioArmor_Tendril_06
			LifeSpan 5
		End

	End

	Condition
		On Time
		Time 5

		Event
			At Anchor2
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
				Scale .45 .45 .007
				EndScale .105 .105 .027
				FadeInLength 0
				TintGeom 1
				Alpha 255
			End
			#Geom FX_BioArmor_Tendril_01
			#Geom FX_BioArmor_Tendril_02
			#Geom FX_BioArmor_Tendril_03
			#Geom FX_BioArmor_Tendril_04
			Geom FX_BioArmor_Tendril_05
			#Geom FX_BioArmor_Tendril_06
			LifeSpan 5
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
				Scale .45 .45 .007
				EndScale .105 .105 .027
				FadeInLength 0
				TintGeom 1
				Alpha 255
			End
			Geom FX_BioArmor_Tendril_01
			#Geom FX_BioArmor_Tendril_02
			#Geom FX_BioArmor_Tendril_03
			#Geom FX_BioArmor_Tendril_04
			#Geom FX_BioArmor_Tendril_05
			#Geom FX_BioArmor_Tendril_06
			LifeSpan 5
		End

	End

	Condition
		On Time
		Time 3

		Event
			At Chest
			Type Start
			EName Prime
			LookAt T_Chest
			Magnet T_Chest
			Bhvr CustomizeablePowers\DarkArmor\Skullprojectile1.bhvr
			BhvrOverride
				TrackRate 2
			End
			Part CustomizeablePowers\DarkArmor\DarknessRapidCloud.part
		#	Part CustomizeablePowers\DarkArmor\DarknessRapidCloud2.part
			LifeSpan 30
		End

	End

	Condition
		On PrimeHit

		Event
			Type Destroy
			EName Prime
		End

	End

End
