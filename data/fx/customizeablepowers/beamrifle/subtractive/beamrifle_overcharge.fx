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
			Sound BR_Overcharge_Attack_01 100 100 0.8
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
			PMagnet WepL
			Part :Muzzle_Rings_Plasma.part
			LifeSpan 45
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
			LifeSpan 120
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
			LifeSpan 40
		End

	End

	Condition
		On Time
		Time 50

		Event
			At WeaponOffset
			Type Start
			EName Prime
			LookAt Target
			Magnet Target
			Bhvr :Tracer_Fade.bhvr
			BhvrOverride
				TrackRate 3.0
			End
			LifeSpan 300
		End

		Event
			At WeaponOffset
			Type Local
			EName MainBeam
			LookAt Target
			POther Target
			Part :PreciseBeam09.part
			Part :Beam_Large.part
			Part :Beam_Large_Additive.part
			Part :BeamCharge_Large.part
			Part :BeamCharge_Large_Blur.part
			LifeSpan 20
		End

	End

	Condition
		On PrimeHit

		Event
			Type Destroy
			EName Prime
		End

		Event
			At T_Root
			Type Local
			ChildFx :BeamRifle_Overcharge_Hit.fx
		End

	End

	Condition
		On Time
		Time 70

		Event
			At P2
			Type PositOnly
			Part :Muzzle_Smoke01.part
			LifeSpan 90
		End

	End

End
