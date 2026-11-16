#
FxInfo

	Input
		InpName Hips
	End

	LifeSpan 200


###########################################################

	Condition
		On Time
		Time 0

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
			Sound PoliceBotBlast3 100 100 .98
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
			BhvrOverride
				TrackRate	7.5
			End
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
