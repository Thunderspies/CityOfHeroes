#                                                          
FxInfo

	LifeSpan 300


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			Sound BR_ChargedShot_Attack_01 100 100 0.8
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
				#PositionOffset 0.25 0 -0.1
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
		Time 0

		Event
			#AltPiv 1
			At WeaponOffset
			Type Local
			EName ChargeUp
			Bhvr :GrowInPlace.bhvr
			BhvrOverride
				Spin 90 0 0
			End
		End

		Event
			At ChargeUp
			Type Local
			Part :Muzzle_Glow_Core_Fast.part
			Part :BeamCharge_Arcs_Medium.part
			LifeSpan 15
		End

	End

	Condition
		On Time
		Time 23

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
			EName Incendiary
			Bhvr :Tracer_Fade.bhvr
			Part :Smoke_Trail01.part
			Part :Tracer_Trail02.part
			LifeSpan 10
		End

		Event
			At Prime
			Type Local
			ChildFx :Child_SingleShot.fx
			EName Beams
			LookAt Target
			POther Target
			Bhvr :Tracer_Fade.bhvr
			Part :PreciseBeam04.part
			Part :Beam_Medium.part
			Part :Beam_Medium_Additive.part
			LifeSpan 10
		End

		Event
			At Prime
			Type Local
			EName ChargeBeams
			LookAt Target
			POther Target
			Bhvr :Tracer_Fade.bhvr
			Part :Beam_Medium_Electricity.part
			Part :Beam_Medium_Electricity_Blur.part
			LifeSpan 10
		End

	End

	Condition
		On PrimeHit

		Event
			Type Destroy
			EName Prime
		End

	End

End
