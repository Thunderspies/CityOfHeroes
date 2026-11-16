#                                                          
FxInfo

	LifeSpan 200


###########################################################

	Condition
		On Time
		Time 0

		Event
			At WepR
			Type Local
			EName Hookup
			Geom LaserRifle
		End

	End

	Condition
		On Time
		Time 7

		Event
			At C_Chest
			Type Start
			LookAt Target
			Magnet Target
			Bhvr behaviors\powers\coldcontrol\iceWeaponblast.bhvr
			Part :IceWeaponSnowBlast.part
			Part :IceWeaponMussleFlash2.part
			Part :IceWeaponMussleFlash.part
		End

		Event
			At C_Chest
			Type Start
			EName mush
			Part :HeavyMussleFlash.part
			Part :HeavyMussleFlash2.part
			Part :MussleSnowStreaks.part
			Sound laserblast3 100.0 100.0 1.0
			LifeSpan 3
		End

	End

	Condition
		On Time
		Time 7

		Event
			At C_Chest
			Type Start
			EName Prime
			LookAt Target
			Magnet Target
			Bhvr behaviors\powers\coldcontrol\iceWeaponHeavyblast.bhvr
			Part :IceBoltSnowtrail2.part
			Part :IceBoltMistTrail3.part
			Part :IceWeaponBlastHeavyTail.part
			Part :IceWeaponBlastHeavyTail.part
			Part :IceWeaponBlastTailGlow.part
			Geom Tintable_FX_IceBlast
		End

	End

	Condition
		On Time
		Time 7

		Event
			At C_Chest
			Type Start
			EName Prime2
			LookAt Target
			Magnet Target
			Bhvr behaviors\Creyspin3b.bhvr
			BhvrOverride
				PositionOffset -0.125 0.0 0.0
			End
			Geom LightDummy2
		End

		Event
			AltPiv 1
			At Prime2
			Type Local
			Part :CreyWeaponStreamer.part
			Part :IceWeaponBlastTailGlowSmall.part
		End

		Event
			AltPiv 2
			At Prime2
			Type Local
			Part :CreyWeaponStreamer.part
			Part :IceWeaponBlastTailGlowSmall.part
		End

	End

	Condition
		On Time
		Time 7

		Event
			At C_Chest
			Type Start
			EName Prime3
			LookAt Target
			Magnet Target
			Bhvr behaviors\Creyspin3a.bhvr
			BhvrOverride
				PositionOffset 0.125 0.0 0.0
			End
			Geom LightDummy2
		End

		Event
			AltPiv 1
			At Prime3
			Type Local
			Part :CreyWeaponStreamer.part
			Part :IceWeaponBlastTailGlowSmall.part
		End

		Event
			AltPiv 2
			At Prime3
			Type Local
			Part :CreyWeaponStreamer.part
			Part :IceWeaponBlastTailGlowSmall.part
		End

	End

	Condition
		On PrimeHit

		Event
			Type Destroy
			EName Prime
		End

		Event
			At target
			Type Posit
			EName IceShards
			Part :IceBoltMist.part
			Part :ChillTouchSnow.part
			Part :ChillingTouchFlash.part
			LifeSpan 20
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

End
