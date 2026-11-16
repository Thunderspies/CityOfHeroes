#
FxInfo

	Input
		InpName Hips
	End

	LifeSpan 200


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
			Sound BaneMaceScatterShot 100 100 0.98
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
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\SmallEnergyHead.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\SmallEnergyHeadInvert.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\SmallRapidEnergyCoreLocal.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\SmallRapidEnergyCoreGlow.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\SmallBotBubblesTrail.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\SmallBotBallzTrail.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\SmallBotTrailSpecks.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\SmallBotTrail.part
		End

	End

	Condition
		On Time
		Time 37

		Event
			AltPiv 2
			At Point
			Type Start
			EName Prime2
			LookAt Target
			Magnet Target
			Bhvr V_COV\EnemyPowers\Arachnos\BaneMace\Projectile1.bhvr
			BhvrOverride
				TrackRate 0.01
			End
		End

	End

	Condition
		On Time
		Time 37
		Repeat 6

		Event
			At Prime2
			Type Local
			EName Ancillary
			LookAt Target
			Bhvr behaviors/Quillprojectile2.bhvr
			BhvrOverride
				InitialVelocity 0 0 6.25
				InitialVelocityJitter 2 .25 .75
			End
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\SmallEnergyHead.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\SmallEnergyHeadInvert.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\SmallRapidEnergyCoreLocal.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\SmallRapidEnergyCoreGlow.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\SmallBotBubblesTrail.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\SmallBotBallzTrail.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\SmallBotTrailSpecks.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\SmallBotTrail.part
			LifeSpan 30
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
