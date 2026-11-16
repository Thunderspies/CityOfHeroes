#
FxInfo

	LifeSpan 300


###########################################################

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
			#Sound DarkBlast1 80 33 0.77
			Sound widowcast5 80 80 0.96
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
			Part CustomizeablePowers\DarkBlast\BeamRapidTenticles.part
			Part CustomizeablePowers\DarkBlast\MoonBeamEmber.part
			Part CustomizeablePowers\DarkBlast\MoonBeamEmberLarge.part
			Part CustomizeablePowers\DarkBlast\MoonBeamRapidCloud.part
			Part CustomizeablePowers\DarkBlast\DarkBlastTendrill01.part
			Part CustomizeablePowers\DarkBlast\DarkBlastTendrill03.part
			Part CustomizeablePowers\DarkBlast\DarkBlastTendrill02.part
			Part CustomizeablePowers\DarkBlast\DarkBlastTendrill04.part
			Part :soulSmoke1.part
			Part :soulSmoke1.part
			Part :soulGlow0.part
		End
	End

	Condition
		On PrimeHit
		Event
			Type	Destroy
			EName	Prime
		End
		Event
			At T_chest
			Type Local
			EName HitFake
			Part CustomizeablePowers\DarkBlast\Blast.part
			Part CustomizeablePowers\DarkBlast\Blast.part
			Part CustomizeablePowers\DarkBlast\BlackStarCore.part
			Part CustomizeablePowers\DarkBlast\CreateDarknessPuddle2.part
			Sound dirtexplo3 80 80 0.8
			LifeSpan 13
		End

		Event
			At T_chest
			Type Local
			EName HitFlash
			Part :soulCloud2.part
			Part :soulSparks1.part
			Part :soulSparks2.part
			Part :soulFlash1.part
			LifeSpan 3
		End

		Event
			At T_Root
			Type Local
			EName GeomRing
			Geom RootToChestAdjustment
		End

		Event
			AltPiv 1
			At GeomRing
			Type Start
			EName RadiusFxScale
			Bhvr behaviors/powers/firecontrol/Infernobubble.bhvr
			Part :DarknessHitRing1.part
			Part :DarknessHitTendrills.part
			LifeSpan 15
		End

		Event
			AltPiv 1
			At GeomRing
			Type Start
			EName Rings
			Part :BlackStarRing.Part
			Part :DarkBlastPuddleExplosion.Part
			LifeSpan 20
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
