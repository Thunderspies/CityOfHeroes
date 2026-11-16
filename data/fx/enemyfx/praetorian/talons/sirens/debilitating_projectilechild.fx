#
FxInfo

	Input
		InpName Hips
	End


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Origin
			Type Local
			EName Anchor
			Part ENEMYFX\Praetorian\Talons\Sirens\MusicNotes_RadOutA.part
			Part ENEMYFX\Praetorian\Talons\Sirens\MusicNotes_RadOutB.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Glow_CoreAdd.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Glow_Core.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Globs_RadOut.part
		End

		Event
			At Origin
			Type Local
			BhvrOverride
				PyrRotateJitter 30 180 30
				Scale 0.2 0.35 0.2
				Spin 0 -0.02 0
				StartJitter .2 .2 .2
			End
			Geom FX_MusicRing_04
		End

		Event
			At Origin
			Type Local
			BhvrOverride
				PyrRotateJitter 30 180 30
				Scale 0.2 0.35 0.2
				Spin 0 -0.02 0
				StartJitter .2 .2 .2
			End
			Geom FX_MusicRing_04
		End

	End

End
