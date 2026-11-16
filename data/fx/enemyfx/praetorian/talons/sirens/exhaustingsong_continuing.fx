#
FxInfo

Lifespan 1000

###########################################################

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			Part ENEMYFX\Praetorian\Talons\Sirens\Globs_Falling.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Glow_CoreAdd.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Glow_Core.part
			Part ENEMYFX\Praetorian\Talons\Sirens\MusicNotes_FallingC.part
			Part ENEMYFX\Praetorian\Talons\Sirens\MusicNotes_FallingD.part
		End

		Event
			At Chest
			Type PositOnly
			EName Anchor
		End

	End

	Condition
		On Time
		Time 0
		repeat 1

		Event
			At Anchor
			Type Local
			Bhvr Behaviors/FadeInOut.bhvr
			BhvrOverride
				PositionOffset 0 -1.5 0
				Scale 0.65 0.7 0.65
			End
			Geom FX_MusicRing_05
			#LifeSpan 45
		End

	End

End
