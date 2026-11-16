#                                                          
FxInfo

	LifeSpan 100


###########################################################

	Condition
		On Time
		Time 15

		Event
			At Root
			Type Local
			Sound BR_LancerShot_Attack_01 100 100 0.8
		End

	End

	Condition
		On Time
		Time 15

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
				#PositionOffset 0.25 0.0 -0.3
			#End
			#Geom FlameThrower
		#End

		Event
			#AltPiv 1
			At WeaponOffset
			Type Local
			LookAt Target
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PyrRotate 0.0 90.0 0.0
			End
			Part :MuzzleFlash_Continuous.part
			Part :Muzzle_Glow_Core_Continuous.part
			Part :Muzzle_Flash_Glow.part
			LifeSpan 3
		End

	End

	Condition
		On Time
		Time 15

		Event
			#AltPiv 1
			At WeaponOffset
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :BeamCharge_Arcs_Medium.part
			LifeSpan 5
		End

		Event
			#AltPiv 1
			At WeaponOffset
			Type Local
			PMagnet Target
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PyrRotate 0.0 90.0 0.0
			End
			Part :MuzzleFlash_Gas03.part
			LifeSpan 5
		End

	End

	Condition
		On Time
		Time 15

		Event
			#AltPiv 1
			At WeaponOffset
			Type Local
			LookAt Target
			PMagnet Target
			POther Target
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Tracer_Trail06.part
			Part :PreciseBeam03.part
			Part :Beam_Long_Continuous.part
			Part :Beam_Long_Continuous_Add.part
			Part :Beam_Flare03.part
			LifeSpan 20
		End

		Event
			#AltPiv 1
			At WeaponOffset
			Type Local
			LookAt Target
			PMagnet Target
			POther Target
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Beam_Long_Continuous_Electricity.part
			Part :Beam_Long_Continuous_Electricity_Blur.part
			LifeSpan 20
		End

		Event
			#AltPiv 1
			At WeaponOffset
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Muzzle_Glow_LargeCore_Fast.part
			Part :BeamCharge_Arcs_Large.part
			LifeSpan 15
		End

	End

End
