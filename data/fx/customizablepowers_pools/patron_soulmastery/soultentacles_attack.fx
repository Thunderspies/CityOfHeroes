#                                                          
FxInfo

	LifeSpan 120


###########################################################

	Condition
		On Time
		Time 12

		Event
			At Origin
			Type Local
			Sound widowcast1 80 80 1.0
		End

	End

	Condition
		On Time
		Time 15

		Event
			At UArmR
			Type Local
			EName binder7
			Bhvr V_COV\EnemyPowers\GhostWidow\soulBinderArm2.bhvr
			LifeSpan 20
		End

		Event
			At binder7
			Type Local
			EName Offset7
			Bhvr V_COV\EnemyPowers\GhostWidow\binderOffset.bhvr
			Part :soulGlow0.part
			Part :soulFire0.part
			Part :soulSmoke0.part
			Geom stormFXsoul
		End

		Event
			AltPiv 2
			At Offset7
			Type Local
			EName Arm1
			Part :soulFire0.part
		End

		Event
			AltPiv 4
			At Offset7
			Type Local
			EName Arm2
			Part :soulFire0.part
		End

	End

	Condition
		On Time
		Time 15

		Event
			At UArmL
			Type Local
			EName binder8
			Bhvr V_COV\EnemyPowers\GhostWidow\soulBinderArm1.bhvr
			LifeSpan 20
		End

		Event
			At binder8
			Type Local
			EName Offset8
			Bhvr V_COV\EnemyPowers\GhostWidow\binderOffset.bhvr
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
		Time 23

		Event
			At LArmR
			Type Local
			EName binder9
			Bhvr V_COV\EnemyPowers\GhostWidow\soulBinderArm2.bhvr
			LifeSpan 20
		End

		Event
			At binder9
			Type Local
			EName Offset9
			Bhvr V_COV\EnemyPowers\GhostWidow\binderOffset.bhvr
			Part :soulGlow0.part
			Part :soulFire0.part
			Part :soulSmoke0.part
			Geom stormFXsoul
		End

		Event
			AltPiv 2
			At Offset9
			Type Local
			EName Arm1
			Part :soulFire0.part
		End

		Event
			AltPiv 4
			At Offset9
			Type Local
			EName Arm2
			Part :soulFire0.part
		End

	End

	Condition
		On Time
		Time 23

		Event
			At LArmL
			Type Local
			EName binder10
			Bhvr V_COV\EnemyPowers\GhostWidow\soulBinderArm1.bhvr
			LifeSpan 20
		End

		Event
			At binder10
			Type Local
			EName Offset10
			Bhvr V_COV\EnemyPowers\GhostWidow\binderOffset.bhvr
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
		Time 30

		Event
			At WepR
			Type Local
			EName HandR
			Part :soulGlow1.part
			Part :soulFire2.part
			Part :soulSmoke1.part
			LifeSpan 23
		End

		Event
			At WepL
			Type Local
			EName HandL
			Part :soulGlow1.part
			Part :soulFire2.part
			Part :soulSmoke1.part
			LifeSpan 23
		End

	End

	Condition
		On Time
		Time 20

		Event
			Type SetLight
			EName Pulse
			Bhvr V_COV\EnemyPowers\GhostWidow\soulLights.bhvr
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 30

		Event
			At WepL
			Type Start
			EName Prime
			LookAt Target
			Magnet Target
			Bhvr V_COV\EnemyPowers\GhostWidow\soulProjectile1.bhvr
			LifeSpan 150
		End

		Event
			At Prime
			Type Local
			EName Offset1
			Bhvr V_COV\EnemyPowers\GhostWidow\soulOffset.bhvr
			Part :soulGlow1.part
			Part :soulFire2.part
			Part :soulSmoke1.part
			Geom stormFXsoul
		End

		Event
			AltPiv 2
			At Offset1
			Type Local
			EName Arm1
			Part :soulFire1.part
		End

		Event
			AltPiv 4
			At Offset1
			Type Local
			EName Arm2
			Part :soulFire1.part
		End

	End

	Condition
		On Time
		Time 33

		Event
			At WepR
			Type Start
			EName Prime2
			LookAt Target
			Magnet Target
			Bhvr V_COV\EnemyPowers\GhostWidow\soulProjectile2.bhvr
			LifeSpan 150
		End

		Event
			At Prime2
			Type Local
			EName Offset2
			Bhvr V_COV\EnemyPowers\GhostWidow\soulOffset3.bhvr
			Part :soulGlow1.part
			Part :soulFire2.part
			Part :soulSmoke1.part
			Geom stormFXsoul
		End

		Event
			AltPiv 2
			At Offset2
			Type Local
			EName Arm1
			Part :soulFire1.part
		End

		Event
			AltPiv 4
			At Offset2
			Type Local
			EName Arm2
			Part :soulFire1.part
		End

	End

	Condition
		On Time
		Time 36

		Event
			At WepL
			Type Start
			EName Prime3
			LookAt Target
			Magnet Target
			Bhvr V_COV\EnemyPowers\GhostWidow\soulProjectile1.bhvr
			LifeSpan 150
		End

		Event
			At Prime3
			Type Local
			EName Offset3
			Bhvr V_COV\EnemyPowers\GhostWidow\soulOffset3.bhvr
			Part :soulGlow1.part
			Part :soulFire2.part
			Part :soulSmoke1.part
			Geom stormFXsoul
		End

		Event
			AltPiv 2
			At Offset3
			Type Local
			EName Arm1
			Part :soulFire1.part
		End

		Event
			AltPiv 4
			At Offset3
			Type Local
			EName Arm2
			Part :soulFire1.part
		End

	End

	Condition
		On Time
		Time 39

		Event
			At WepR
			Type Start
			EName Prime4
			LookAt Target
			Magnet Target
			Bhvr V_COV\EnemyPowers\GhostWidow\soulProjectile2.bhvr
			LifeSpan 150
		End

		Event
			At Prime4
			Type Local
			EName Offset4
			Bhvr V_COV\EnemyPowers\GhostWidow\soulOffset.bhvr
			Part :soulGlow1.part
			Part :soulFire2.part
			Part :soulSmoke1.part
			Geom stormFXsoul
		End

		Event
			AltPiv 2
			At Offset4
			Type Local
			EName Arm1
			Part :soulFire1.part
		End

		Event
			AltPiv 4
			At Offset4
			Type Local
			EName Arm2
			Part :soulFire1.part
		End

	End

	Condition
		On PrimeHit

		Event
			At T_Root
			Type Start
			EName Explode
			Part :soulFlash2.part
			Part :soulExplode2.part
			Part :soulSparks1.part
			Part :soulCloud1.part
			Part :DarkBlastPuddleExplosion.Part
			Sound dirtexplo 70 70 0.77
			LifeSpan 5
		End

		Event
			Type Destroy
			EName Prime
		End

	End

	Condition
		On Prime1Hit

		Event
			Type Destroy
			EName Prime1
		End

	End

	Condition
		On Prime2Hit

		Event
			Type Destroy
			EName Prime2
		End

	End

	Condition
		On Prime3Hit

		Event
			Type Destroy
			EName Prime3
		End

	End

	Condition
		On Prime4Hit

		Event
			Type Destroy
			EName Prime4
		End

	End

End
