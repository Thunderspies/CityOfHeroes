#                                                          
FxInfo

	LifeSpan 300


###########################################################

	Condition
		On Time
		Time 36

		Event
			At WepR
			Type Local
			EName Point
			Geom heavyMace
		End

		Event
			AltPiv 2
			At Point
			Type Local
			Part :EmissionPoint.part
			Part :BlastStreaks.part
			Sound PoliceBotBlast3 100 100 0.98
			LifeSpan 2
		End

		Event
			AltPiv 2
			At Point
			Type Start
			EName Prime
			LookAt Target
			Magnet Target
			Bhvr V_COV\EnemyPowers\Arachnos\BaneMace\Projectile1.bhvr
			Part :EnergyHead.part
			Part :EnergyHeadInvert.part
			Part :RapidEnergyCoreLocal.part
			Part :RapidCoreLocal.part
			Part :RapidCoreLocalInvert.part
			Part :RapidEnergyCoreGlow.part
			Part :BotTrail.part
			Part :BotTrailSpecks.part
			Part :BotBubblesTrail.part
			Part :BotBallzTrail.part
			Part :EnergyStreakTrailStraight.part
			Part :BotTrail2.part
			#Sound PoliceBotLaser2 100 90 0.95
		End

	End

	Condition
		On PrimeHit

		Event
			At T_Chest
			Type Start
			Part :MartialArtsHit.part
			Part :MartialArtsHit1.part
			Part :MartialArtsHit2.part
			Part :MartialArtsHitRaysSmall.part
			Sound banedisruptorhit 80 80 0.8
			LifeSpan 3
		End

		Event
			At T_Root
			Type Start
			EName Rings
			Bhvr Behaviors/RootToChestadjustment.bhvr
			Part :HitRingflashthin.part
			Part :HitRingflashthin2.part
			Part :HitRingflash.part
			LifeSpan 20
		End

		Event
			At Rings
			Type Local
			EName R
			Bhvr V_COV\EnemyPowers\Arachnos\BaneMace\Inferno.bhvr
			Geom Centerdummy
		End

		Event
			AltPiv 1
			At R
			Type Local
			Bhvr V_COV\EnemyPowers\Arachnos\BaneMace\Infernobubble.bhvr
			Part :aoeDarkEnergy.part
			Part :aoeLightEnergy.part
			Part :aoeEnergy.part
			Part :aoeEnergyDots.part
		End

		Event
			Type Destroy
			EName Prime
		End

	End

End
