#
FxInfo

	Input
		InpName Hips
	End

	LifeSpan 20


###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	TOV_Siren_SongHit_Minor 100 100 .8
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
			Part ENEMYFX\Praetorian\Talons\Sirens\Fascinating_NoteC.part
		End

		Event
			At Chest
			Type Local
			Part ENEMYFX\Praetorian\Talons\Sirens\Globs_RadOut.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Globs_RadOut.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Globs_RadOut.part
			Part CustomizeablePowers\SonicAttack\SonicHitflash.part
			Part CustomizeablePowers\SonicAttack\SonicHitflashStar.part
			Lifespan 5
		End

	End

End
