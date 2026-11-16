#
FxInfo

	Input
		InpName Hips
	End

	LifeSpan 20


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			Part CustomizeablePowers\SonicAttack\Mirage.part
			Part CustomizeablePowers\SonicAttack\Mirage2.part
			Part CustomizeablePowers\SonicAttack\MirageSteam.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Fascinating_NoteC.part
#			Sound sonichit2 70 70 0.75
		End

		Event
			At Chest
			Type Local
			Part CustomizeablePowers\SonicAttack\SonicHitflash.part
			Part CustomizeablePowers\SonicAttack\SonicHitflashStar.part
			Part CustomizeablePowers\SonicAttack\SonicHitRing.part
			Part CustomizeablePowers\SonicAttack\SonicHitRingThin.part
		End

	End

End
