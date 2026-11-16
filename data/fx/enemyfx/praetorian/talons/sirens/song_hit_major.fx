#
FxInfo
Lifespan 30

###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	TOV_Siren_SongHit_Major 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			Part CustomizeablePowers\SonicAttack\Mirage.part
			Part CustomizeablePowers\SonicAttack\MirageSteam.part
			Part ENEMYFX\Praetorian\Talons\Sirens\MusicNotes_RadOutA.part
			Part ENEMYFX\Praetorian\Talons\Sirens\MusicNotes_RadOutB.part
			Part ENEMYFX\Praetorian\Talons\Sirens\MusicNotes_RadOutA.part
			Part ENEMYFX\Praetorian\Talons\Sirens\MusicNotes_RadOutB.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Glow_Core.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Glow_CoreAdd.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Glow_InwardCoreAdd.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Song_RadOut.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Fascinating_NoteC.part
		End

		Event
			At Chest
			Type Local
			Part ENEMYFX\Praetorian\Talons\Sirens\Globs_RadOut.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Globs_RadOut.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Globs_RadOut.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Globs_BigHit.part
			Part CustomizeablePowers\SonicAttack\SonicHitflash.part
			Part CustomizeablePowers\SonicAttack\SonicHitflashStar.part
			Part CustomizeablePowers\SonicAttack\SonicHitRing.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Sonic_BigHit_Flash.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Globs_BigHit_Spike.part
			Part CustomizeablePowers\SonicAttack\SonicHitRingThin.part
			LifeSpan 5
		End

		Event
			At Chest
			Type PositOnly
			Bhvr ENEMYFX\Praetorian\Talons\Sirens\MusicHit_Grow.bhvr
			BhvrOverride
				PyrRotate 0 90 90
				PyrRotateJitter 0 90 0
				EndScale 0.65 0.65 0.65
				StartJitter 0.25 0.5 0.25
			End
			Geom FX_MusicRing_06
			LifeSpan 10
		End

	End

End
