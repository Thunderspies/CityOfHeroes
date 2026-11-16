#                                                          
FxInfo

	LifeSpan 100


###########################################################

	Condition
		On Time
		Time 10

		Event
			At Origin
			Type Local
			Sound mu_cageboltramp 120 120 0.9
		End

	End

	Condition
		On Time
		Time 30

		Event
			At Origin
			Type Local
			Sound mu_cagebolts 120 120 0.9
		End

	End

	Condition
		On Time
		Time 10

		Event
			At WepL
			Type Local
			EName HandR
			Part :mu_Cage_HandArchs.part
			LifeSpan 15
		End

		Event
			At WepR
			Type Local
			EName HandL
			Part :mu_Cage_HandArchs.part
			LifeSpan 15
		End

		Event
			At WepL
			Type Local
			EName HandR
			Part :mu_Cage_HandArchs_White.part
			LifeSpan 25
		End

		Event
			At WepR
			Type Local
			EName HandL
			Part :mu_Cage_HandArchs_White.part
			LifeSpan 25
		End

	End

	Condition
		On Time
		Time 30

		Event
			At HandL
			Type Start
			Part :mu_CageRing1.part
			LifeSpan 5
		End

		Event
			At HandR
			Type Start
			Part :mu_CageRing1.part
			LifeSpan 5
		End

	End

	Condition
		On Time
		Time 0

		Event
			At T_Root
			Type PositOnly
			EName bar
			Bhvr V_COV\EnemyPowers\Arachnos\mu_CageBar1.Bhvr
		End

		Event
			At bar
			Type Local
			EName bar1
			Bhvr V_COV\EnemyPowers\Arachnos\mu_CageBar1.Bhvr
		End

		Event
			At bar1
			Type Local
			EName baroff1
			Bhvr V_COV\EnemyPowers\Arachnos\mu_CageBarOff1.Bhvr
		End

		Event
			At bar1
			Type Local
			EName baroff1N
			Bhvr V_COV\EnemyPowers\Arachnos\mu_CageBarOffNeg1.Bhvr
		End

	End

	Condition
		On Time
		Time 10

		Event
			At Chest
			Type Local
			EName lift
			Magnet baroff1N
			Bhvr V_COV\EnemyPowers\Arachnos\mu_CageLift.Bhvr
			LifeSpan 35
		End

		Event
			At Chest
			Type Local
			EName spotR
			Magnet baroff1N
			Bhvr V_COV\EnemyPowers\Arachnos\mu_CageShockTrackerR.Bhvr
			LifeSpan 35
		End

		Event
			At Chest
			Type Local
			EName spotL
			Magnet baroff1
			Bhvr V_COV\EnemyPowers\Arachnos\mu_CageShockTrackerL.Bhvr
			LifeSpan 35
		End

	End

	Condition
		On Time
		Time 30

		Event
			At spotR
			Type Start
			EName Prime
			Magnet baroff1N
			Bhvr V_COV\EnemyPowers\Arachnos\mu_CageShockTracker.Bhvr
			LifeSpan 35
		End

		Event
			At Prime
			Type Local
			Part :mu_CageShock1.part
			Part :mu_CageShock1.part
			Part :mu_CageShock1.part
			Part :mu_CageShock2.part
			Part :mu_CageShockSpark.part
			Part :mu_CageShockSpark_White.part
			LifeSpan 35
		End

	End

	Condition
		On Time
		Time 30

		Event
			At spotL
			Type Start
			EName trackerL
			Magnet baroff1
			Bhvr V_COV\EnemyPowers\Arachnos\mu_CageShockTracker.Bhvr
			LifeSpan 35
		End

		Event
			At trackerL
			Type Local
			Part :mu_CageShock1.part
			Part :mu_CageShock1.part
			Part :mu_CageShock1.part
			Part :mu_CageShock2.part
			Part :mu_CageShockSpark.part
			Part :mu_CageShockSpark_White.part
			LifeSpan 35
		End

	End

	Condition
		On Time
		Time 10

		Event
			At Neck
			Type Local
			POther spotR
			Part :mu_CageReacher1.part
			LifeSpan 6
		End

		Event
			At Neck
			Type Local
			POther spotL
			Part :mu_CageReacher1.part
			LifeSpan 6
		End

		Event
			At Chest
			Type Local
			POther spotR
			Part :mu_CageReacher1.part
			LifeSpan 6
		End

		Event
			At Chest
			Type Local
			POther spotL
			Part :mu_CageReacher1.part
			LifeSpan 6
		End

		Event
			At Hips
			Type Local
			POther spotR
			Part :mu_CageReacher1.part
			LifeSpan 6
		End

		Event
			At Hips
			Type Local
			POther spotL
			Part :mu_CageReacher1.part
			LifeSpan 6
		End

	End

	Condition
		On Time
		Time 14

		Event
			At Neck
			Type Local
			POther spotR
			Part :mu_CageReacher1.part
			LifeSpan 6
		End

		Event
			At Neck
			Type Local
			POther spotL
			Part :mu_CageReacher1.part
			LifeSpan 6
		End

		Event
			At Chest
			Type Local
			POther spotR
			Part :mu_CageReacher1.part
			LifeSpan 6
		End

		Event
			At Chest
			Type Local
			POther spotL
			Part :mu_CageReacher1.part
			LifeSpan 6
		End

		Event
			At Hips
			Type Local
			POther spotR
			Part :mu_CageReacher1.part
			LifeSpan 6
		End

		Event
			At Hips
			Type Local
			POther spotL
			Part :mu_CageReacher1.part
			LifeSpan 6
		End

	End

	Condition
		On Time
		Time 18

		Event
			At Neck
			Type Local
			POther spotR
			Part :mu_CageReacher1.part
			LifeSpan 6
		End

		Event
			At Neck
			Type Local
			POther spotL
			Part :mu_CageReacher1.part
			LifeSpan 6
		End

		Event
			At Chest
			Type Local
			POther spotR
			Part :mu_CageReacher1.part
			LifeSpan 6
		End

		Event
			At Chest
			Type Local
			POther spotL
			Part :mu_CageReacher1.part
			LifeSpan 6
		End

		Event
			At Hips
			Type Local
			POther spotR
			Part :mu_CageReacher1.part
			LifeSpan 6
		End

		Event
			At Hips
			Type Local
			POther spotL
			Part :mu_CageReacher1.part
			LifeSpan 6
		End

	End

	Condition
		On PrimeHit

		Event
			At baroff1
			Type Local
			Part :mu_CageFenceBurst1.part
			Part :mu_CageFenceBurst2.part
			Part :mu_CageFence3.part
			LifeSpan 15
		End

		Event
			At baroff1N
			Type Local
			Part :mu_CageFenceBurst1.part
			Part :mu_CageFenceBurst2.part
			Part :mu_CageFence3.part
			LifeSpan 15
		End

	End

End
