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
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\EmissionPoint.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\BlastStreaks.part
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
			At T_Chest
			Type Start
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MartialArtsHit.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MartialArtsHit1.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MartialArtsHit2.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MartialArtsHitRaysSmall.part
			Sound banedisruptorhit 80 80 0.8
			LifeSpan 3
		End

		Event
			At T_Root
			Type Start
			EName Rings
			Bhvr Behaviors/RootToChestadjustment.bhvr
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\HitRingflashthin.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\HitRingflashthin2.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\HitRingflash.part
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
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\aoeDarkEnergy.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\aoeLightEnergy.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\aoeEnergy.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\aoeEnergyDots.part
		End

		Event
			Type Destroy
			EName Prime
		End

	End

End
