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
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\EmissionPoint.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\BlastStreaks.part
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
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\EnergyHead.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\EnergyHeadInvert.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\RapidEnergyCoreLocal.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\RapidCoreLocal.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\RapidCoreLocalInvert.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\RapidEnergyCoreGlow.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\BotTrail.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\BotTrailSpecks.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\BotBubblesTrail.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\BotBallzTrail.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\EnergyStreakTrailStraight.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\BotTrail2.part
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
