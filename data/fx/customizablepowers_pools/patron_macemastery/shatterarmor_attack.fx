#
FxInfo

	LifeSpan 70


###########################################################

	Condition
		On Time
		Time 14

		Event
			At Origin
			Type Local
			Sound Whooshbig3 100.0 100.0 0.8
		End

	End

	Condition
		On Time
		Time 35

		Event
			At Mystic
			Type Local
			EName Streak
			Bhvr behaviors/Medieval1.bhvr
			BhvrOverride
				TintGeom 1
			End
			Geom swoosh
			LifeSpan 1
		End

	End

	Condition
		On Time
		Time 32

		Event
			At FootR
			Type Start
			EName Prime
			LookAt T_Root
			Magnet T_Root
			Bhvr behaviors/MaceEnergyprojectile.bhvr
			BhvrOverride
				TintGeom 1
			End
			Part :MaceRaysTall.part
			Part :MaceRaysCenter1.part
			Part :MaceRaysTall.part
			Part :MaceRaysCenter1.part
			Part :MaceRaysTall.part
			Part :MaceRaysCenter1.part
			Part :MaceRaysTall.part
			Part :MaceRaysCenter1.part
			Part :MaceEnergyRings.part
		End

		Event
			At FootR
			Type Start
			EName Prime1
			LookAt T_Root
			Magnet T_Root
			Bhvr behaviors/MaceEnergyprojectile.bhvr
			BhvrOverride
				TintGeom 0
			End
			Part :MaceEnergyRingsFlat.part
			Part :MaceEnergyRings3.part
		End

	End

	Condition
		On Time
		Time 30

		Event
			At FootR
			Type Start
			EName Prime2
			LookAt T_Root
			Magnet T_Root
			Bhvr behaviors/BattleAxePrjct1.bhvr
			BhvrOverride
				TintGeom 1
			End
			Geom Tintable_BattleAxe
		End

		Event
			At FootR
			Type Start
			EName Prime3
			LookAt T_Root
			Magnet T_Root
			Bhvr behaviors/BattleAxePrjct1.bhvr
			BhvrOverride
				TintGeom 1
			End
			Geom Tintable_BattleAxe
		End

		Event
			At FootR
			Type Start
			EName Prime4
			LookAt T_Root
			Magnet T_Root
			Bhvr behaviors/BattleAxePrjct1.bhvr
			BhvrOverride
				TintGeom 1
			End
			Geom Tintable_BattleAxe
		End

	End

	Condition
		On Time
		Time 41

		Event
			Type Destroy
			EName streak
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
			EName Prime1
		End

		Event
			Type Destroy
			EName Prime2
		End

		Event
			Type Destroy
			EName Prime3
		End

	End

	Condition
		On Prime4Hit

		Event
			Type Destroy
			EName Prime4
		End

	End

End
