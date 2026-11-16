#                                                          
FxInfo

	LifeSpan 120


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			Sound BR_PenetratingRay_Chargeup_01 100 100 0.8
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
		Time 3

		Event
			At WeaponOffset
			Type Local
			EName ChargeUp
			Bhvr :GrowInPlace.bhvr
		End

		Event
			At ChargeUp
			Type Local
			Part :Muzzle_Glow_Core_Fast.part
			Part :BeamCharge_Arcs_Medium.part
			LifeSpan 7
		End

	End

	Condition
		On Time
		Time 10

		Event
			At ChargeUp
			Type Local
			Part :Muzzle_Glow_LargeCore_Fast.part
			Part :BeamCharge_Arcs_Large.part
		End

	End

	Condition
		On Time
		Time 20

		Event
			At WepR
			Type Local
			EName MuzzleOffset
			Bhvr :Tracer_Fade.bhvr
			BhvrOverride
				PositionOffset 1 0.0 0.1
			End
			Geom FlameThrower
		End

		Event
			At MuzzleOffset
			Type PositOnly
			Part :Muzzle_Smoke01.part
		End

	End

End
