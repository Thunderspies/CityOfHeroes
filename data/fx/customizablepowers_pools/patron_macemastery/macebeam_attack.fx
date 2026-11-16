#
FxInfo

	Input
		InpName Hips
	End

	LifeSpan 200


###########################################################

	Condition
		On Time
		Time 26

		Event
			At Origin
			Type Local
			Sound PoliceBotBlast2 100 100 0.8
		End

	End

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
			Type Destroy
			EName Prime
		End

	End

End
