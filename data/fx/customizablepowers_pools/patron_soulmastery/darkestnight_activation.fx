#                                                          
FxInfo

	LifeSpan 220


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			Flags PowerLoopingSound
			Bhvr behaviors/soundFade3.bhvr
			Sound miasma_loop 50.0 50.0 0.4
			LifeSpan 220
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			Flags PowerLoopingSound
			Bhvr behaviors/soundFade3.bhvr
			Sound miasma_loop 50.0 50.0 0.4
			LifeSpan 220
		End

	End

	Condition
		On Time
		Time 3

		Event
			At UArmR
			Type Local
			EName Rbinder7
			Bhvr V_COV\EnemyPowers\GhostWidow\soulBinderArm2.bhvr
			LifeSpan 20
		End

		Event
			At Rbinder7
			Type Local
			EName ROffset7
			Bhvr V_COV\EnemyPowers\GhostWidow\binderOffset.bhvr
			Part :soulGlow0.part
			Part :soulFire0.part
			Part :soulSmoke0.part
			Geom stormFXsoul
		End

		Event
			AltPiv 2
			At ROffset7
			Type Local
			EName Arm1
			Part :soulFire0.part
		End

		Event
			AltPiv 4
			At ROffset7
			Type Local
			EName Arm2
			Part :soulFire0.part
		End

	End

	Condition
		On Time
		Time 11

		Event
			At LArmR
			Type Local
			EName Rbinder9
			Bhvr V_COV\EnemyPowers\GhostWidow\soulBinderArm2.bhvr
			LifeSpan 20
		End

		Event
			At Rbinder9
			Type Local
			EName ROffset9
			Bhvr :binderOffset.bhvr
			Part :soulGlow0.part
			Part :soulFire0.part
			Part :soulSmoke0.part
			Geom stormFXsoul
		End

		Event
			AltPiv 2
			At ROffset9
			Type Local
			EName Arm1
			Part :soulFire0.part
		End

		Event
			AltPiv 4
			At ROffset9
			Type Local
			EName Arm2
			Part :soulFire0.part
		End

	End

	Condition
		On Time
		Time 13

		Event
			At WepR
			Type Local
			EName HandR
			Part :soulGlow1.part
			Part :soulFire2.part
			Part :soulSmoke1.part
			LifeSpan 9
		End

	End

	Condition
		On Time
		Time 3

		Event
			At UArmL
			Type Local
			EName Lbinder7
			Bhvr V_COV\EnemyPowers\GhostWidow\soulBinderArm2.bhvr
			BhvrOverride
				InitialVelocity -0.02 0.0 0.0
				InitialVelocityJitter 0.005 0.00 0.00
			End
			LifeSpan 20
		End

		Event
			At Lbinder7
			Type Local
			EName LOffset7
			Bhvr V_COV\EnemyPowers\GhostWidow\binderOffset.bhvr
			BhvrOverride
				PositionOffset -1 0 2
				Spin 0.0 0.0 0.1
				SpinJitter 0.0 0.0 0.05
			End
			Part :soulGlow0.part
			Part :soulFire0.part
			Part :soulSmoke0.part
			Geom stormFXsoul
		End

		Event
			AltPiv 2
			At LOffset7
			Type Local
			EName LArm1
			Part :soulFire0.part
		End

		Event
			AltPiv 4
			At LOffset7
			Type Local
			EName LArm2
			Part :soulFire0.part
		End

	End

	Condition
		On Time
		Time 11

		Event
			At LArmL
			Type Local
			EName Lbinder9
			Bhvr V_COV\EnemyPowers\GhostWidow\soulBinderArm2.bhvr
			BhvrOverride
				InitialVelocity -0.02 0.0 0.0
				InitialVelocityJitter 0.005 0.00 0.00
			End
			LifeSpan 20
		End

		Event
			At Lbinder9
			Type Local
			EName LOffset9
			Bhvr V_COV\EnemyPowers\GhostWidow\binderOffset.bhvr
			BhvrOverride
				PositionOffset -1 0 2
				Spin 0.0 0.0 0.1
				SpinJitter 0.0 0.0 0.05
			End
			Part :soulGlow0.part
			Part :soulFire0.part
			Part :soulSmoke0.part
			Geom stormFXsoul
		End

		Event
			AltPiv 2
			At LOffset9
			Type Local
			EName LArm1
			Part :soulFire0.part
		End

		Event
			AltPiv 4
			At LOffset9
			Type Local
			EName LArm2
			Part :soulFire0.part
		End

	End

	Condition
		On Time
		Time 13

		Event
			At WepL
			Type Local
			EName HandL
			Part :soulGlow1.part
			Part :soulFire2.part
			Part :soulSmoke1.part
			LifeSpan 9
		End

	End

	Condition
		On Time
		Time 0

		Event
			At WepR
			Type Local
			Sound miasmaburst 100 100 0.8
			LifeSpan 90
		End

		Event
			At WepL
			Type Local
			Sound miasma_loop 100 100 0.5
			LifeSpan 90
		End

		Event
			At WepR
			Type Local
			EName Base
			Part CustomizablePowers_Pools\Patron_SoulMastery\DarkHandsEmber.part
			Part CustomizablePowers_Pools\Patron_SoulMastery\DarkHand.part
			Sound darkwindup2 60 60 0.3
			LifeSpan 90
		End

		Event
			At WepL
			Type Local
			EName Base
			Part CustomizablePowers_Pools\Patron_SoulMastery\DarkHand.part
			Part CustomizablePowers_Pools\Patron_SoulMastery\DarkHandsEmber.part
			LifeSpan 90
		End

	End

End
