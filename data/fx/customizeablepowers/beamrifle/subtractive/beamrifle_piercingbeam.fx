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
			Sound BR_PiercingBeam_Attack_01 100 100 0.8
		End

	End

	Condition
		On Time
		Time 50

		Event
			At C_Chest
			Type Local
			EName BeamAnchor
		End

		Event
			At BeamAnchor
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				#PositionOffset -0.25 0.3 2
			End
			Part :MuzzleFlash_Continuous.part
			Part :Muzzle_Glow_Core_Continuous.part
			Part :Muzzle_Flash_Glow.part
			LifeSpan 3
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

	End

	Condition
		On Time
		Time 5

		Event
			At WeaponOffset
			Type Local
			Part :Muzzle_Glow.part
			Part :Muzzle_Glow_Core.part
			LifeSpan 30
		End

	End

	Condition
		On Time
		Time 15

		Event
			At WeaponOffset
			Type Local
			PMagnet WepL
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :BeamCharge_Arcs_Subtle.part
			LifeSpan 15
		End

	End

	Condition
		On Time
		Time 30

		Event
			At WeaponOffset
			Type Local
			PMagnet WepL
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :BeamCharge_Arcs.part
			LifeSpan 15
		End

	End

	Condition
		On Time
		Time 45

		Event
			At WeaponOffset
			Type Local
			EName Flash
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Muzzle_Flash.part
			Part :Muzzle_Flash_Flash.part
			Part :BeamCharge_Arcs.part
			Part :Muzzle_Flash_Shockwave.part
			LifeSpan 5
		End

	End

	Condition
		On Time
		Time 50

		Event
			At WeaponOffset
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :BeamCharge_Arcs_Medium.part
			LifeSpan 5
		End

		Event
			At WeaponOffset
			Type Local
			PMagnet Target
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :MuzzleFlash_Gas03.part
			LifeSpan 5
		End

	End

	Condition
		On Time
		Time 50

		Event
			At BeamAnchor
			Type Local
			LookAt Target
			PMagnet Target
			POther Target
			Part :Tracer_Trail08.part
			Part :Beam_Wide_Continuous.part
			Part :Beam_Wide_Continuous_Add.part
			Part :Beam_Flare03.part
			Part :Beam_Wide_Continuous_Electricity.part
			Part :Beam_Wide_Continuous_Electricity_Blur.part
			LifeSpan 12
		End

	End

	Condition
		On Time
		Time 55

		Event
			At Target
			Type PositOnly
			LookAt WeaponOffset
			PMagnet WeaponOffset
			POther WeaponOffset
			Part :Beam_Long_ContinuousOffset_Electricity.part
		End

	End

	Condition
		On Time
		Time 60

		Event
			At BeamAnchor
			Type Local
			LookAt Target
			PMagnet Target
			POther Target
			Part :Beam_Long_Continuous.part
			Part :Beam_Long_Continuous_Add.part
			Part :Beam_Long_Continuous_Electricity.part
			Part :Beam_Long_Continuous_Electricity_Blur.part
			LifeSpan 10
		End

		Event
			At Target
			Type PositOnly
			LookAt WeaponOffset
			PMagnet WeaponOffset
			POther WeaponOffset
			Part :Beam_Long_ContinuousOffset_Electricity.part
		End

	End

	Condition
		On Time
		Time 65

		Event
			At Target
			Type PositOnly
			LookAt WeaponOffset
			PMagnet WeaponOffset
			POther WeaponOffset
			Part :Beam_Long_ContinuousOffset_Electricity.part
		End

	End

End
