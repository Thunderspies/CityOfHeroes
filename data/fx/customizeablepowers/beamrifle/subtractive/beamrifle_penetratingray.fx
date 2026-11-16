#                                                          
FxInfo

	LifeSpan 300


###########################################################

	Condition
		On Time
		Time 7

		Event
			At Root
			Type Local
			Sound BR_PenetratingRay_Attack_01 100 100 0.8
		End

	End

	Condition
		On Time
		Time 0

		Event
			At C_Chest
			Type Local
			EName WeaponOffset
		End

		Event
			At WeaponOffset
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PyrRotate 0 90 0
			End
			Part :Muzzle_Smoke01.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 0

		Event
			At WeaponOffset
			Type Local
			Part :Muzzle_Glow_LargeCore_Fast.part
			Part :BeamCharge_Arcs_Large.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 12

		Event
			At WeaponOffset
			Type Start
			EName Prime
			LookAt Target
			Magnet Target
			Bhvr :Tracer_Fade.bhvr
			BhvrOverride
				TrackRate 6.0
			End
			#Geom GEO_FX_TestSphere
			LifeSpan 300
		End

		Event
			At Prime
			Type Local
			EName Incendiary2
			Bhvr :Tracer_Fade.bhvr
			Part :Smoke_Trail01.part
			LifeSpan 10
		End

		Event
			At Prime
			Type Local
			Bhvr :Tracer_Fade.bhvr
			BhvrOverride
				PositionOffset 0 0 2
			End
			Part :BeamSnipeFlash_01.part
			LifeSpan 10
		End

		Event
			At WeaponOffset
			Type Local
			Part :Muzzle_Flash_MD_Shockwave.part
			LifeSpan 10
		End

		Event
			At Prime
			Type Local
			Bhvr :Tracer_Fade.bhvr
			Part :Tracer_Trail07.part
			LifeSpan 10
		End

		Event
			At Prime
			Type Local
			EName MuzzleFlash
			LookAt Target
			POther Target
			Bhvr :Tracer_Fade.bhvr
			Part :PreciseBeam05.part
			Part :BeamShot_Large.part
			Part :BeamShot_Large_Additive.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 12

		Event
			At WeaponOffset
			Type Start
			EName Prime2
			LookAt Target
			Magnet Target
			Bhvr behaviors\Braz\Projectile_Fast_01.bhvr
			BhvrOverride
				TrackRate 6.0
			End
			#Geom GEO_FX_TestSphere
			LifeSpan 300
		End

		Event
			At Prime2
			Type Local
			EName MuzzleFlash2
			LookAt Target
			POther Target
			Bhvr :Tracer_Fade.bhvr
			Part :Beam_Large_Electricity.part
			Part :Beam_Large_Electricity_Blur.part
			LifeSpan 10
		End

	End

	Condition
		On PrimeHit

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
			EName Incendiary1
		End

		Event
			Type Destroy
			EName Incendiary2
		End

	End

End
