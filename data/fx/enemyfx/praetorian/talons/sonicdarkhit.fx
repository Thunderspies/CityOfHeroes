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
			Part :Mirage.part
			Part :Mirage2.part
			Part :MirageSteam.part
			Sound sonichit2 70 70 0.75
		End

		Event
			At chest
			Type Start
			Part CustomizeablePowers\SonicAttack\SonicHitflash.part
			Part CustomizeablePowers\SonicAttack\SonicHitflashStar.part
			Part CustomizeablePowers\SonicAttack\SonicHitRing.part
			Part CustomizeablePowers\SonicAttack\SonicHitRingThin.part
			Part ENEMYFX\Praetorian\Talons\DarkCloud.part
			Part ENEMYFX\Praetorian\Talons\DarkTendrils.part
			Part ENEMYFX\Praetorian\Talons\DarkSmoke_Trail02.part
			Part ENEMYFX\Praetorian\Talons\DarkTendrilCloud.part
		End

	End

End
