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
			Part ENEMYFX\Praetorian\Talons\Sirens\MusicNotes_RadOutA.part
			Part ENEMYFX\Praetorian\Talons\Sirens\MusicNotes_RadOutB.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Glow_CoreAdd.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Glow_Core.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Globs_RadOut.part
		End

		Event
			At Chest
			Type Local
			BhvrOverride
				PyrRotateJitter 30 180 30
				Scale 0.3 0.5 0.3
				Spin 0 -0.02 0
				StartJitter 1 1 1
			End
			Geom FX_MusicRing_04
		End

		Event
			At Chest
			Type Local
			BhvrOverride
				Spin 0 -0.03 0
				Scale 0.2 0.4 0.2
				PyrRotateJitter 30 180 30
				StartJitter 1 1 1
			End
			Geom FX_MusicRing_04
		End

		Event
			At Chest
			Type Local
			BhvrOverride
				Spin 0 -0.02 0
				Scale 0.3 0.5 0.3
				PyrRotateJitter 30 180 30
				StartJitter 1 1 1
			End
			Geom FX_MusicRing_04
		End

	End

End
