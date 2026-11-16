#                                                          
FxInfo


###########################################################

	Condition
		On Time
		Time 50

		Event
			At Eyes
			Type Local
			Sound TransBurst 60 60 0.7
		End

	End

	Condition
		On Time
		Time 60

		Event
			At Chest
			Type Local
			EName 9
			Flags PowerLoopingSound
			Bhvr behaviors/soundFade3.bhvr
			Sound intel2_loop 60 60 0.66
			LifeSpan 330
		End

		Event
			At Chest
			Type Local
			EName 9
			Flags PowerLoopingSound
			Bhvr behaviors/soundFade3.bhvr
			Sound green_loop 60 60 0.22
			LifeSpan 400
		End

	End

	Condition
		On Time
		Time 0

		Event
			At FootR
			Type Local
			EName binder1
			Bhvr V_COV\EnemyPowers\GhostWidow\soulBinder1.bhvr
			LifeSpan 45
		End

		Event
			At binder1
			Type Local
			EName Offset1
			Bhvr :binderOffset.bhvr
			Part :soulGlow0.part
			Part :soulFire0.part
			Part :soulSmoke0.part
			Geom stormFXsoul
		End

		Event
			AltPiv 2
			At Offset1
			Type Local
			EName Arm1
			Part :soulFire0.part
		End

		Event
			AltPiv 4
			At Offset1
			Type Local
			EName Arm2
			Part :soulFire0.part
		End

	End

	Condition
		On Time
		Time 0

		Event
			At FootL
			Type Local
			EName binder2
			Bhvr V_COV\EnemyPowers\GhostWidow\soulBinder2.bhvr
			LifeSpan 45
		End

		Event
			At binder2
			Type Local
			EName Offset2
			Bhvr V_COV\EnemyPowers\GhostWidow\binderOffset.bhvr
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
		Time 10

		Event
			At LLegR
			Type Local
			EName binder3
			Bhvr V_COV\EnemyPowers\GhostWidow\soulBinder1.bhvr
			LifeSpan 40
		End

		Event
			At binder3
			Type Local
			EName Offset3
			Bhvr V_COV\EnemyPowers\GhostWidow\binderOffset.bhvr
			Part :soulGlow0.part
			Part :soulFire0.part
			Part :soulSmoke0.part
			Geom stormFXsoul
		End

		Event
			AltPiv 2
			At Offset3
			Type Local
			EName Arm1
			Part :soulFire0.part
		End

		Event
			AltPiv 4
			At Offset3
			Type Local
			EName Arm2
			Part :soulFire0.part
		End

	End

	Condition
		On Time
		Time 10

		Event
			At LLegL
			Type Local
			EName binder4
			Bhvr V_COV\EnemyPowers\GhostWidow\soulBinder2.bhvr
			LifeSpan 40
		End

		Event
			At binder4
			Type Local
			EName Offset4
			Bhvr V_COV\EnemyPowers\GhostWidow\binderOffset.bhvr
			Part :soulGlow0.part
			Part :soulFire0.part
			Part :soulSmoke0.part
			Geom stormFXsoul
		End

		Event
			AltPiv 2
			At Offset4
			Type Local
			EName Arm1
			Part :soulFire0.part
		End

		Event
			AltPiv 4
			At Offset4
			Type Local
			EName Arm2
			Part :soulFire0.part
		End

	End

	Condition
		On Time
		Time 15

		Event
			At Hips
			Type Local
			EName binder5
			Bhvr V_COV\EnemyPowers\GhostWidow\soulBinder1.bhvr
			LifeSpan 35
		End

		Event
			At binder5
			Type Local
			EName Offset5
			Bhvr V_COV\EnemyPowers\GhostWidow\binderOffset.bhvr
			Part :soulGlow0.part
			Part :soulFire0.part
			Part :soulSmoke0.part
			Geom stormFXsoul
		End

		Event
			AltPiv 2
			At Offset5
			Type Local
			EName Arm1
			Part :soulFire0.part
		End

		Event
			AltPiv 4
			At Offset5
			Type Local
			EName Arm2
			Part :soulFire0.part
		End

	End

	Condition
		On Time
		Time 20

		Event
			At Waist
			Type Local
			EName binder6
			Bhvr V_COV\EnemyPowers\GhostWidow\soulBinder2.bhvr
			LifeSpan 25
		End

		Event
			At binder6
			Type Local
			EName Offset6
			Bhvr V_COV\EnemyPowers\GhostWidow\binderOffset.bhvr
			Part :soulGlow0.part
			Part :soulFire0.part
			Part :soulSmoke0.part
			Geom stormFXsoul
		End

		Event
			AltPiv 2
			At Offset6
			Type Local
			EName Arm1
			Part :soulFire0.part
		End

		Event
			AltPiv 4
			At Offset6
			Type Local
			EName Arm2
			Part :soulFire0.part
		End

	End

	Condition
		On Time
		Time 5

		Event
			At HandR
			Type Local
			EName binder7
			Bhvr V_COV\EnemyPowers\GhostWidow\soulBinderArm1.bhvr
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
		Time 5

		Event
			At HandL
			Type Local
			EName binder8
			Bhvr V_COV\EnemyPowers\GhostWidow\soulBinderArm2.bhvr
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
		Time 15

		Event
			At LArmR
			Type Local
			EName binder9
			Bhvr V_COV\EnemyPowers\GhostWidow\soulBinderArm1.bhvr
			LifeSpan 35
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
		Time 15

		Event
			At LArmL
			Type Local
			EName binder10
			Bhvr V_COV\EnemyPowers\GhostWidow\soulBinderArm2.bhvr
			LifeSpan 35
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
		Time 0

		Event
			At Head
			Type Local
			EName 9
			Part :PurpleTendrils.part
			Part :BlueTendrils.part
		End

		Event
			At UArmL
			Type Local
			EName 10
			Part :PurpleTendrils.part
			Part :BlueTendrils.part
		End

		Event
			At UArmR
			Type Local
			EName 13
			Part :PurpleTendrils.part
			Part :BlueTendrils.part
		End

		Event
			At LArmL
			Type Local
			EName 11
			Part :PurpleTendrils.part
			Part :BlueTendrils.part
		End

		Event
			At LArmR
			Type Local
			EName 14
			Part :PurpleTendrils.part
			Part :BlueTendrils.part
		End

		Event
			At Hips
			Type Local
			EName 3
			Part :PurpleTendrils.part
			Part :BlueTendrils.part
		End

		Event
			At LLegL
			Type Local
			EName 4
			PMagnet ULegL
			Part :PurpleTendrils.part
			Part :BlueTendrils.part
		End

		Event
			At LLegR
			Type Local
			EName 7
			PMagnet ULegR
			Part :PurpleTendrils.part
			Part :BlueTendrils.part
		End

		Event
			At FootL
			Type Local
			EName 5
			PMagnet LLegL
			Part :PurpleTendrils.part
			Part :BlueTendrils.part
		End

		Event
			At FootR
			Type Local
			EName 2
			PMagnet LLegR
			Part :PurpleTendrils.part
			Part :BlueTendrils.part
		End

	End

End
