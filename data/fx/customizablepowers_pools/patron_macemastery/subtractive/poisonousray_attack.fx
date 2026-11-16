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
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\EmissionPoint.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\BlastStreaks.part
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
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\EnergyHead.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\EnergyHeadInvert.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\RapidEnergyCoreLocal.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\RapidCoreLocal.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\RapidCoreLocalInvert.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\BotBubblesTrail.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\BotBallzTrail.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\EnergyStreakTrailStraight.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\BotTrailSpecks_Poisonous.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\BotTrail_Poisonous.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\BotTrail2.part
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
