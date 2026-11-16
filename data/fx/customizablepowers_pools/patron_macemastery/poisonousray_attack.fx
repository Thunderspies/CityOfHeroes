#                                                          
FxInfo

	LifeSpan 200


###########################################################

	Condition
		On Time
		Time 36

		Event
			At WepR
			Type Local
			EName Point
			Bhvr V_COV\Weapons\maceBlastEmissionPoint.bhvr
			Geom heavyMace
		End

		Event
			At WepR
			Type Local
			Bhvr V_COV\Weapons\maceBlastEmissionPoint.bhvr
			Part :EmissionPoint.part
			Part :BlastStreaks.part
			Sound PoliceBotLaser2 100 100 0.95
			LifeSpan 2
		End

		Event
			AltPiv 2
			At Point
			Type Start
			EName Prime
			LookAt Target
			Magnet Target
			Bhvr behaviors\Quillprojectile1.bhvr
			Part :EnergyHead.part
			Part :EnergyHeadInvert.part
			Part :RapidEnergyCoreLocal.part
			Part :RapidCoreLocal.part
			Part :RapidCoreLocalInvert.part
			Part :BotBubblesTrail.part
			Part :BotBallzTrail.part
			Part :EnergyStreakTrailStraight.part
			Part :BotTrailSpecks_Poisonous.part
			Part :BotTrail_Poisonous.part
			Part :BotTrail2.part
		End

	End

	Condition
		On PrimeHit

		Event
			Type Destroy
			EName ALL
		End

	End

End
