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
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\RapidCoreLocal.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\RapidCoreLocalInvert.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\BotTrailSpecksBuildUP.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\BotBallzSparkler.part
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
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\RapidEnergyCoreLocal.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\SmallRapidEnergyCoreGlow.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\SummonPulseFlash.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\SummonPulseRing.part
			LifeSpan 24
		End

	End

End
