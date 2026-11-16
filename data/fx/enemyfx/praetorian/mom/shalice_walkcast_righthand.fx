#
FxInfo


###########################################################

	Condition
		On Time
		Time 25

		Event
			At HandR
			Type Local
			Part ENEMYFX\Praetorian\MoM\Shalice_WalkCast_HandGlow01.part
			LifeSpan 60
		End

	End


	Condition
		On Time
		Time 85

		Event
			At Root
			Type Local
			Sound Awakened_Frazzled_Attack_01 100 100 0.8
		End

		Event
			At Chest
			Type PositOnly
			Bhvr ENEMYFX\Awakened\Frazzled_Bubble.bhvr
			BhvrOverride
				Spin 0.01 0 0
				Alpha 255
				FadeOutLength 5
				StartColor 255 150 255
			End
			Geom Disintegration_Bubble
			LifeSpan 17
		End

		Event
			At Chest
			Type PositOnly
			Bhvr ENEMYFX\Awakened\Frazzled_Bubble.bhvr
			BhvrOverride
				Alpha 255
				StartColor 255 150 255
				PyrRotateJitter 180 180 180
				FadeOutLength 5
				Spin 0.025 0.01 0.0
			End
			Geom Tintable_EarthGraspHit
			LifeSpan 15
		End

		Event
			At Chest
			Type StartPositOnly
			Part ENEMYFX\Praetorian\MoM\Shalice_WalkCast_Electric.part
			Part ENEMYFX\Praetorian\MoM\Shalice_WalkCast_Border.part
			LifeSpan 12
		End

		Event
			At Root
			Type StartPositOnly
			Part ENEMYFX\Praetorian\MoM\Shalice_WalkCast_Glow_Flat.part
			Part ENEMYFX\Praetorian\MoM\Shalice_WalkCast_Border_Ground.part
			LifeSpan 10
		End

	End

End
