#                                                          
FxInfo

	Input
		InpName Origin
	End

	LifeSpan 250


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Start
			EName DummyChest
		End

		Event
			At DummyChest
			Type Start
			EName FogRing1
			Part ENEMYFX\Hydra\Hydra_Head_Death\CoTEnemyGateRingSmall.part
			Part ENEMYFX\Hydra\Hydra_Head_Death\CoT_EnemyPortal.part
			Sound hurricane_loop 100 100 0.7
			LifeSpan 80
		End

		Event
			At DummyChest
			Type Start
			EName Hurricane
			Part ENEMYFX\Hydra\Hydra_Head_Death\CoTEnemyGate01.part
			LifeSpan 80
		End

	End

	Condition
		On Time
		Time 0

		Event
			At DummyChest
			Type Start
			EName Hydra_Head_Explosion
			Part ENEMYFX\Hydra\Hydra_Head_Death\Death_Glow_02.part
			LifeSpan 190
		End

	End

End
