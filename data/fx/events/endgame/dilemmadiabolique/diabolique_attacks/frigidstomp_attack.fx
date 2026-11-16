#
FxInfo

	LifeSpan 100


###########################################################

	Condition
		On Time
		Time 10

		Event
			At Hips
			Type Local
			Sound BP_FrigidStomp_Attack_01 200 200 0.5
		End

	End

	Condition
		On Time
		Time 24

		Event
			At Mystic
			Type StartPositOnly
			EName DustAnchor
			BhvrOverride
				PositionOffset 0 30 0
			End
		End

		Event
			At Mystic
			Type StartPositOnly
			Bhvr behaviors/CameraShake01.bhvr
			BhvrOverride
				PositionOffset 0.0 -1.0 0.0
			End
			Part :Dust_Circle_Large.part
			Part :Dust_Circle_Lingering.part
			Part :Dust_Shockwave_Large.part
			Part :Dust_Shockwave_Fill.part
			Part :Dust_Flash.part
			Part :Sparks_Flash.part
			LifeSpan 8
		End

		Event
			At Mystic
			Type Start
			Bhvr behaviors\Splat_Cracks.bhvr
			BhvrOverride
				Alpha 255
				PositionOffset 0 0.05 0
				Scale 20.0 20.0 20.0
				PyrRotate 0 180 0
			End
			Splat ImpactCracks.tga
			LifeSpan 150
		End

		Event
			At Mystic
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0.0 -3.0 0.0
			End
			Part :Sparks_Vertical.part
			Part :Sparks_Long.part
			Part :Rocks.part
			LifeSpan 8
		End

		Event
			At Mystic
			Type StartPositOnly
			POther DustAnchor
			BhvrOverride
				PositionOffset 0.0 8.0 0.0
			End
			Part :Dust_Pillar.part
			Part :Dirt_Pillar.part
			LifeSpan 5
		End

	End
###########################################################

End
