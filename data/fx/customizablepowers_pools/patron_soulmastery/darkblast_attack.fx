#
FxInfo

	Input
		InpName Hips
	End

	LifeSpan 300


###########################################################

	Condition
		On Time
		Time 8

		Event
			At Origin
			Type Local
			Sound widowcast4 80 80 0.96
		End

	End

	Condition
		On Time
		Time 10

		Event
			At UArmR
			Type Local
			EName Hand1
			PMagnet LarmR
			Part CustomizeablePowers\DarkBlast\DarkBlastTenticle.part
			Part CustomizeablePowers\DarkBlast\DarkHandsEmberb.part
			Sound DarkBlast1 80 80 0.77
			LifeSpan 20
		End

		Event
			At LArmR
			Type Local
			EName Hand2
			PMagnet WepR
			Part CustomizeablePowers\DarkBlast\DarkBlastTenticle.part
			Part CustomizeablePowers\DarkBlast\DarkHandsEmberb.part
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 3

		Event
			At UArmR
			Type Local
			EName binder7
			Bhvr :soulBinderArm2.bhvr
			LifeSpan 20
		End

		Event
			At binder7
			Type Local
			EName Offset7
			Bhvr :binderOffset.bhvr
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
		Time 11

		Event
			At LArmR
			Type Local
			EName binder9
			Bhvr :soulBinderArm2.bhvr
			LifeSpan 20
		End

		Event
			At binder9
			Type Local
			EName Offset9
			Bhvr :binderOffset.bhvr
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
		Time 22

		Event
			At WepR
			Type Start
			EName Prime
			LookAt Target
			Magnet Target
			Bhvr behaviors/slowprjct5.bhvr
			Part CustomizeablePowers\DarkBlast\MoonBeamEmberLarge.part
			Part CustomizeablePowers\DarkBlast\DarkBlastTendril04.part
			Part CustomizeablePowers\DarkBlast\DarkBlastTendril02.part
			Part CustomizeablePowers\DarkBlast\DarkBlastTendril03.part
			Part CustomizeablePowers\DarkBlast\DarkBlastTendril01.part
			Part CustomizeablePowers\DarkBlast\MoonBeamRapidCloud.part
			Part CustomizeablePowers\DarkBlast\MoonBeamEmber.part
			Part CustomizeablePowers\DarkBlast\BeamRapidTenticles.part
			Part :soulSmoke1.part
			Part :soulSmoke1.part
			Part :soulGlow0.part
		End

	End

	Condition
		On PrimeHit

		Event
			At target
			Type Local
			EName HitFake
			Part CustomizeablePowers\DarkBlast\Blast.part
			Part CustomizeablePowers\DarkBlast\Blast.part
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

	End

	Condition
		On Time
		Time 70

		Event
			Type Destroy
			EName HitFake
		End

	End

	Condition
		On Time
		Time 50

		Event
			Type Destroy
			EName Hand1
		End

		Event
			Type Destroy
			EName Hand2
		End

		Event
			Type Destroy
			EName Hand3
		End

		Event
			Type Destroy
			EName Hand4
		End

	End

End
