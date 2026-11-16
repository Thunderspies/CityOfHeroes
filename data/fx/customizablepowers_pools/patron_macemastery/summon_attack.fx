#                                                          
FxInfo

	LifeSpan 90


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

	End

	Condition
		On Time
		Time 6

		Event
			AltPiv 2
			At Point
			Type Local
			EName Prime
			LookAt Prime
			Magnet Prime
			Part :RapidCoreLocal.part
			Part :RapidCoreLocalInvert.part
			Part :BotTrailSpecksBuildUP.part
			Part :BotBallzSparkler.part
			Sound BaneMaceSummon2 100 100 0.95
		End

	End

	Condition
		On Time
		Time 36

		Event
			AltPiv 2
			At Point
			Type Local
			EName Flash
			LookAt Prime
			Magnet Prime
			Part :RapidEnergyCoreLocal.part
			Part :SmallRapidEnergyCoreGlow.part
			Part :SummonPulseFlash.part
			Part :SummonPulseRing.part
			LifeSpan 24
		End

	End

End
