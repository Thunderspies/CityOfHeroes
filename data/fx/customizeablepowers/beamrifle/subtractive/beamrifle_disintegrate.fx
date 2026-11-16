#                                                          
FxInfo

	LifeSpan 100


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			Sound BR_Disintegrate_Attack_01 100 100 0.8
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
				#PositionOffset 0.25 0.0 -0.3
			#End
			#Geom FlameThrower
		#End

		Event
			AltPiv 1
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
		Time 22

		Event
			At Target
			Type Local
			EName BeamEnd
		End

		Event
			#AltPiv 1
			At WeaponOffset
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :MuzzleFlash_Continuous.part
			Part :Muzzle_Glow_Core_Continuous.part
			Part :Muzzle_Flash_Glow.part
			LifeSpan 3
		End

	End

	Condition
		On Time
		Time 25

		Event
			#AltPiv 1
			At WeaponOffset
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Muzzle_Glow_Core_Fast.part
			Part :BeamCharge_Arcs_Medium.part
			LifeSpan 20
		End

		Event
			#AltPiv 1
			At WeaponOffset
			Type Local
			PMagnet BeamEnd
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PyrRotate 0.0 90.0 0.0
			End
			Part :MuzzleFlash_Gas03.part
			LifeSpan 3
		End

		Event
			#AltPiv 1
			At WeaponOffset
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Muzzle_Glow_LargeCore_Fast.part
			Part :BeamCharge_Arcs_Large.part
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 28

		Event
			#AltPiv 1
			At WeaponOffset
			Type Local
			LookAt BeamEnd
			PMagnet BeamEnd
			POther BeamEnd
			Part :Beam_Small_Continuous.part
			Part :Beam_Small_Continuous_Add.part
			Part :Beam_Flare03.part
			LifeSpan 20
		End

		Event
			#AltPiv 1
			At WeaponOffset
			Type Local
			LookAt BeamEnd
			PMagnet BeamEnd
			POther BeamEnd
			Part :Beam_Small_Continuous_Electricity.part
			Part :Beam_Small_Continuous_Electricity_Blur.part
			LifeSpan 20
		End

		Event
			At BeamEnd
			Type Local
			Part :Beam_Flare1.Part
			LifeSpan 20
		End

	End

End
