#
FxInfo

	LifeSpan 120


###########################################################

	Condition
		On Time
		Time 10

		Event
			At UArmR
			Type Local
			EName binder7
			Bhvr V_COV\EnemyPowers\GhostWidow\soulBinderArm2.bhvr
			Sound summonwidow2 80 80 0.86
			LifeSpan 45
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
		Time 10

		Event
			At UArmL
			Type Local
			EName binder8
			Bhvr V_COV\EnemyPowers\GhostWidow\soulBinderArm1.bhvr
			LifeSpan 45
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
		Time 20

		Event
			At LArmR
			Type Local
			EName binder9
			Bhvr V_COV\EnemyPowers\GhostWidow\soulBinderArm2.bhvr
			LifeSpan 60
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
		Time 20

		Event
			At LArmL
			Type Local
			EName binder10
			Bhvr V_COV\EnemyPowers\GhostWidow\soulBinderArm1.bhvr
			LifeSpan 60
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
		Time 10

		Event
			At WepR
			Type Local
			EName HandR
			Part :soulGlow1.part
			Part :soulFire2.part
			Part :soulSmoke1.part
		End

		Event
			At WepL
			Type Local
			EName HandL
			Part :soulGlow1.part
			Part :soulFire2.part
			Part :soulSmoke1.part
		End

	End

	Condition
		On Time
		Time 100

		Event
			Type Destroy
			EName All
		End

	End

End
