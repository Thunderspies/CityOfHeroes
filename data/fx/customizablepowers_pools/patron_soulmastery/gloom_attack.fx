#                                                          
FxInfo

	LifeSpan 300


###########################################################

	Condition
		On Time
		Time 7

		Event
			At Origin
			Type Local
			Sound widowspincast3 70 70 0.7
		End

	End

	Condition
		On Time
		Time 10

		Event
			At UArmL
			Type Local
			EName Hand3
			PMagnet LarmL
			Part CustomizeablePowers\DarkBlast\DarkBlastTenticle.part
			Part CustomizeablePowers\DarkBlast\DarkHandsEmberb.part
			Sound SoulDrain2 80 80 0.7
			LifeSpan 50
		End

	End

	Condition
		On Time
		Time 14

		Event
			At LArmL
			Type Local
			EName Hand4
			PMagnet WepL
			Part CustomizeablePowers\DarkBlast\DarkBlastTenticle.part
			Part CustomizeablePowers\DarkBlast\DarkHandsEmberb.part
			LifeSpan 42
		End

	End

	Condition
		On Time
		Time 10

		Event
			At UArmL
			Type Local
			EName binder8
			Bhvr :soulBinderArm1.bhvr
			BhvrOverride
				PositionOffset -0.2 0 0
			End
			LifeSpan 20
		End

		Event
			At binder8
			Type Local
			EName Offset8
			Bhvr :binderOffset.bhvr
			Part :soulGlow0.part
			Part :soulFire0.part
			Part :soulSmoke0.part
			Geom stormFXsoul
		End

		Event
			AltPiv 2
			At Offset8
			Type Local
			EName Arm1
			Part :soulFire0.part
		End

		Event
			AltPiv 4
			At Offset8
			Type Local
			EName Arm2
			Part :soulFire0.part
		End

	End

	Condition
		On Time
		Time 16

		Event
			At LArmL
			Type Local
			EName binder10
			Bhvr :soulBinderArm1.bhvr
			LifeSpan 20
		End

		Event
			At binder10
			Type Local
			EName Offset10
			Bhvr :binderOffset.bhvr
			Part :soulGlow0.part
			Part :soulFire0.part
			Part :soulSmoke0.part
			Geom stormFXsoul
		End

		Event
			AltPiv 2
			At Offset10
			Type Local
			EName Arm1
			Part :soulFire0.part
		End

		Event
			AltPiv 4
			At Offset10
			Type Local
			EName Arm2
			Part :soulFire0.part
		End

	End

	Condition
		On Time
		Time 22

		Event
			At WepL
			Type Local
			EName binder2
			Bhvr :soulBinderArm1.bhvr
			BhvrOverride
				InitialVelocity 0 0 0
				InitialVelocityJitter 0 0 0
				SpinJitter 0.05 0.015 0.015
			End
			LifeSpan 25
		End

		Event
			At binder2
			Type Local
			EName Offset2
			Bhvr :binderOffset.bhvr
			Part :soulGlow0.part
			Part :soulFire0.part
			Part :soulSmoke0.part
			Geom stormFXsoul
		End

		Event
			AltPiv 2
			At Offset2
			Type Local
			EName Arm1
			Part :soulFire0.part
		End

		Event
			AltPiv 4
			At Offset2
			Type Local
			EName Arm2
			Part :soulFire0.part
		End

	End

	Condition
		On Time
		Time 22

		Event
			At WepL
			Type Local
			EName HandL
			Part :soulGlow1.part
			Part :soulFire2.part
			Part :soulSmoke0.part
			LifeSpan 23
		End

	End

	Condition
		On Time
		Time 25

		Event
			At WepL
			Type Start
			EName Prime
			LookAt Target
			Magnet Target
			Bhvr behaviors\Skullprojectile1.bhvr
			Geom Skull
		End

		Event
			AltPiv 1
			At Prime
			Type Start
			EName Prime2
			LookAt Target
			Magnet Target
			Bhvr behaviors\Skullprojectile1.bhvr
			Part CustomizeablePowers\DarkBlast\DarknessRapidCloud.part
			Part CustomizeablePowers\DarkBlast\DarknessRapidCloud2.part
			Part CustomizeablePowers\DarkBlast\SoulDrainSkullGlow.part
			Part :soulSmoke1.part
			Part :soulGlow0.part
			#LifeSpan 0
		End

		Event
			AltPiv 2
			At Prime
			Type Local
			EName Prime3
			Part CustomizeablePowers\DarkBlast\DaemonEyes2.part
			Part CustomizeablePowers\DarkBlast\DaemonPupil2.part
		End

		Event
			AltPiv 3
			At Prime
			Type Local
			EName Prime4
			Part CustomizeablePowers\DarkBlast\DaemonEyes2.part
			Part CustomizeablePowers\DarkBlast\DaemonPupil2.part
		End

	End

	Condition
		On PrimeHit

		Event
			At T_Chest
			Type Local
			Part CustomizeablePowers\DarkBlast\SkullHit.part
			Part CustomizeablePowers\DarkBlast\SkullPuddle.Part
			Sound headcrunch 70 70 0.98
			LifeSpan 25
		End

		Event
			Type Destroy
			EName Prime
		End

		Event
			Type Destroy
			EName Prime2
		End

		Event
			Type Destroy
			EName Prime3
		End

		Event
			Type Destroy
			EName Prime4
		End

	End

End
