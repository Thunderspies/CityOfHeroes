#                                                          
FxInfo

	LifeSpan 300


###########################################################

	Condition
		On Time
		Time 10

		Event
			At Root
			Type Local
			Sound BR_SingleShot_Attack_01 100 100 0.8
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

		#Event
			#At WepR
			#Type Local
			#EName WeaponOffset
			#Bhvr :Tracer_Fade.bhvr
			#BhvrOverride
				#Alpha 1
				#PositionOffset 0.25 0.0 -0.1
			#End
			#Geom FlameThrower
		#End

		Event
			At WepL
			Type Local
			EName P2
			BhvrOverride
				Alpha 1
			End
			Geom FlameThrower
		End

		Event
			At P2
			Type PositOnly
			Part :Muzzle_Smoke01.part
			LifeSpan 25
		End

	End

	Condition
		On Time
		Time 10

		Event
			#AltPiv 1
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
			Part :Tracer_Trail01.part
			LifeSpan 10
		End

		Event
			At Prime
			Type Local
			ChildFx :Child_SingleShot.fx
			EName MuzzleFlash
			LookAt Target
			POther Target
			Bhvr :Tracer_Fade.bhvr
			Part :PreciseBeam_Small.part
			Part :Beam_Small.part
			Part :Beam_Small_Additive.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 10

		Event
			At Prime
			Type Local
			EName MuzzleFlash2
			LookAt Target
			POther Target
			Bhvr :Tracer_Fade.bhvr
			Part :Projectile_Trail_Electricity.part
			Part :Projectile_Trail_Electricity_Blur.part
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
